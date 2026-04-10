/*
 * minihttp.c - Benchmark: HTTP request parser
 *
 * Usage: ./minihttp <raw_http_request>
 *   Parses an HTTP request string, extracts method, path, version,
 *   headers, and body. Prints parsed fields.
 *   Handles GET, POST, HEAD, PUT, DELETE, PATCH, OPTIONS.
 *
 * If no argument, uses a built-in test suite of HTTP requests.
 * Returns 0 on success, 1 on failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

/* ---------- Configuration ---------- */

#define MAX_HEADERS      64
#define MAX_HEADER_NAME  128
#define MAX_HEADER_VALUE 1024
#define MAX_METHOD       16
#define MAX_PATH         2048
#define MAX_VERSION      16
#define MAX_BODY         4096
#define MAX_QUERY_PARAMS 32
#define MAX_PARAM_KEY    128
#define MAX_PARAM_VALUE  512

/* ---------- Data structures ---------- */

typedef struct {
    char name[MAX_HEADER_NAME];
    char value[MAX_HEADER_VALUE];
} HTTPHeader;

typedef struct {
    char key[MAX_PARAM_KEY];
    char value[MAX_PARAM_VALUE];
} QueryParam;

typedef struct {
    char        method[MAX_METHOD];
    char        path[MAX_PATH];
    char        version[MAX_VERSION];
    char        raw_query[MAX_PATH];
    HTTPHeader  headers[MAX_HEADERS];
    int         header_count;
    QueryParam  query_params[MAX_QUERY_PARAMS];
    int         query_param_count;
    char        body[MAX_BODY];
    int         body_length;
    int         content_length;
    int         keep_alive;
    int         has_host;
    int         is_chunked;
    char        content_type[MAX_HEADER_VALUE];
    int         parse_error;
    char        error_msg[256];
} HTTPRequest;

/* ---------- String utilities ---------- */

static void str_trim(char *s) {
    /* Trim leading whitespace */
    int start = 0;
    while (s[start] && (s[start] == ' ' || s[start] == '\t')) start++;
    if (start > 0) {
        int i = 0;
        while (s[start + i]) {
            s[i] = s[start + i];
            i++;
        }
        s[i] = '\0';
    }
    /* Trim trailing whitespace */
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1] == ' ' || s[len-1] == '\t' ||
                       s[len-1] == '\r' || s[len-1] == '\n')) {
        s[--len] = '\0';
    }
}

static void str_to_lower(char *dst, const char *src, int maxlen) {
    int i = 0;
    while (src[i] && i < maxlen - 1) {
        dst[i] = (src[i] >= 'A' && src[i] <= 'Z')
                 ? src[i] + ('a' - 'A') : src[i];
        i++;
    }
    dst[i] = '\0';
}

static int str_starts_with(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s++ != *prefix++) return 0;
    }
    return 1;
}

static int str_case_eq(const char *a, const char *b) {
    while (*a && *b) {
        char ca = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a;
        char cb = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (ca != cb) return 0;
        a++; b++;
    }
    return *a == *b;
}

/* URL-decode a percent-encoded string */
static int hex_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static void url_decode(char *dst, const char *src, int maxlen) {
    int di = 0;
    while (*src && di < maxlen - 1) {
        if (*src == '%' && src[1] && src[2]) {
            int h = hex_digit(src[1]);
            int l = hex_digit(src[2]);
            if (h >= 0 && l >= 0) {
                dst[di++] = (char)(h * 16 + l);
                src += 3;
                continue;
            }
        }
        if (*src == '+') {
            dst[di++] = ' ';
            src++;
        } else {
            dst[di++] = *src++;
        }
    }
    dst[di] = '\0';
}

/* ---------- Query string parser ---------- */

static int parse_query_string(const char *query, QueryParam *params,
                               int max_params) {
    int count = 0;
    char buf[MAX_PATH];
    strncpy(buf, query, MAX_PATH - 1);
    buf[MAX_PATH - 1] = '\0';

    char *saveptr = NULL;
    char *token = strtok_r(buf, "&", &saveptr);
    while (token && count < max_params) {
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = '\0';
            url_decode(params[count].key, token, MAX_PARAM_KEY);
            url_decode(params[count].value, eq + 1, MAX_PARAM_VALUE);
        } else {
            url_decode(params[count].key, token, MAX_PARAM_KEY);
            params[count].value[0] = '\0';
        }
        count++;
        token = strtok_r(NULL, "&", &saveptr);
    }
    return count;
}

/* ---------- Method validation ---------- */

static int is_valid_method(const char *method) {
    const char *valid[] = {
        "GET", "POST", "HEAD", "PUT", "DELETE", "PATCH", "OPTIONS",
        "TRACE", "CONNECT", NULL
    };
    for (int i = 0; valid[i]; i++) {
        if (strcmp(method, valid[i]) == 0) return 1;
    }
    return 0;
}

static int method_has_body(const char *method) {
    return (strcmp(method, "POST") == 0 ||
            strcmp(method, "PUT") == 0 ||
            strcmp(method, "PATCH") == 0);
}

/* ---------- HTTP Parser ---------- */

static int parse_request_line(HTTPRequest *req, const char *line) {
    /* Parse: METHOD SP PATH SP VERSION */
    const char *p = line;

    /* Extract method */
    int mi = 0;
    while (*p && *p != ' ' && mi < MAX_METHOD - 1) {
        req->method[mi++] = *p++;
    }
    req->method[mi] = '\0';

    if (!is_valid_method(req->method)) {
        snprintf(req->error_msg, sizeof(req->error_msg),
                 "Invalid method: %s", req->method);
        return -1;
    }

    /* Skip space */
    if (*p != ' ') {
        snprintf(req->error_msg, sizeof(req->error_msg),
                 "Expected space after method");
        return -1;
    }
    p++;

    /* Extract path (may include query string) */
    int pi = 0;
    while (*p && *p != ' ' && *p != '?' && pi < MAX_PATH - 1) {
        req->path[pi++] = *p++;
    }
    req->path[pi] = '\0';

    /* Extract query string if present */
    if (*p == '?') {
        p++;
        int qi = 0;
        while (*p && *p != ' ' && qi < MAX_PATH - 1) {
            req->raw_query[qi++] = *p++;
        }
        req->raw_query[qi] = '\0';
        req->query_param_count = parse_query_string(
            req->raw_query, req->query_params, MAX_QUERY_PARAMS);
    }

    /* Skip space */
    if (*p != ' ') {
        snprintf(req->error_msg, sizeof(req->error_msg),
                 "Expected space after path");
        return -1;
    }
    p++;

    /* Extract version */
    int vi = 0;
    while (*p && *p != '\r' && *p != '\n' && vi < MAX_VERSION - 1) {
        req->version[vi++] = *p++;
    }
    req->version[vi] = '\0';

    if (!str_starts_with(req->version, "HTTP/")) {
        snprintf(req->error_msg, sizeof(req->error_msg),
                 "Invalid HTTP version: %s", req->version);
        return -1;
    }

    return 0;
}

static int parse_header_line(HTTPRequest *req, const char *line) {
    if (req->header_count >= MAX_HEADERS) return -1;

    const char *colon = strchr(line, ':');
    if (!colon) {
        snprintf(req->error_msg, sizeof(req->error_msg),
                 "Malformed header (no colon)");
        return -1;
    }

    int namelen = (int)(colon - line);
    if (namelen <= 0 || namelen >= MAX_HEADER_NAME) return -1;

    HTTPHeader *h = &req->headers[req->header_count];
    memcpy(h->name, line, (size_t)namelen);
    h->name[namelen] = '\0';

    /* Skip colon and optional whitespace */
    const char *val = colon + 1;
    while (*val == ' ' || *val == '\t') val++;

    strncpy(h->value, val, MAX_HEADER_VALUE - 1);
    h->value[MAX_HEADER_VALUE - 1] = '\0';
    str_trim(h->value);

    /* Process known headers */
    if (str_case_eq(h->name, "Content-Length")) {
        req->content_length = atoi(h->value);
    } else if (str_case_eq(h->name, "Host")) {
        req->has_host = 1;
    } else if (str_case_eq(h->name, "Connection")) {
        if (str_case_eq(h->value, "keep-alive")) {
            req->keep_alive = 1;
        }
    } else if (str_case_eq(h->name, "Transfer-Encoding")) {
        char lower[MAX_HEADER_VALUE];
        str_to_lower(lower, h->value, MAX_HEADER_VALUE);
        if (strstr(lower, "chunked")) {
            req->is_chunked = 1;
        }
    } else if (str_case_eq(h->name, "Content-Type")) {
        strncpy(req->content_type, h->value, MAX_HEADER_VALUE - 1);
        req->content_type[MAX_HEADER_VALUE - 1] = '\0';
    }

    req->header_count++;
    return 0;
}

static int parse_http_request(HTTPRequest *req, const char *raw) {
    memset(req, 0, sizeof(HTTPRequest));
    req->content_length = -1;

    /* Replace escaped newlines with real ones for argv usage */
    char *input = strdup(raw);
    if (!input) return -1;

    /* Handle \\r\\n sequences from command line */
    char *wp = input;
    const char *rp = input;
    while (*rp) {
        if (rp[0] == '\\' && rp[1] == 'r' && rp[2] == '\\' && rp[3] == 'n') {
            *wp++ = '\r';
            *wp++ = '\n';
            rp += 4;
        } else if (rp[0] == '\\' && rp[1] == 'n') {
            *wp++ = '\n';
            rp += 2;
        } else {
            *wp++ = *rp++;
        }
    }
    *wp = '\0';

    /* Parse request line */
    const char *line_start = input;
    const char *line_end = strstr(line_start, "\r\n");
    if (!line_end) line_end = strchr(line_start, '\n');
    if (!line_end) {
        snprintf(req->error_msg, sizeof(req->error_msg),
                 "No line ending in request");
        free(input);
        return -1;
    }

    char line_buf[MAX_PATH + MAX_METHOD + MAX_VERSION + 4];
    int line_len = (int)(line_end - line_start);
    if (line_len >= (int)sizeof(line_buf)) line_len = (int)sizeof(line_buf) - 1;
    memcpy(line_buf, line_start, (size_t)line_len);
    line_buf[line_len] = '\0';
    str_trim(line_buf);

    if (parse_request_line(req, line_buf) != 0) {
        req->parse_error = 1;
        free(input);
        return -1;
    }

    /* Advance past line ending */
    const char *p = line_end;
    if (*p == '\r') p++;
    if (*p == '\n') p++;

    /* Parse headers */
    while (*p && !(*p == '\r' || *p == '\n')) {
        line_end = strstr(p, "\r\n");
        if (!line_end) line_end = strchr(p, '\n');
        if (!line_end) line_end = p + strlen(p);

        line_len = (int)(line_end - p);
        if (line_len >= (int)sizeof(line_buf)) line_len = (int)sizeof(line_buf) - 1;
        memcpy(line_buf, p, (size_t)line_len);
        line_buf[line_len] = '\0';
        str_trim(line_buf);

        if (line_buf[0]) {
            parse_header_line(req, line_buf);
        }

        p = line_end;
        if (*p == '\r') p++;
        if (*p == '\n') p++;
    }

    /* Skip blank line separating headers from body */
    if (*p == '\r') p++;
    if (*p == '\n') p++;

    /* Extract body */
    if (*p && method_has_body(req->method)) {
        int remaining = (int)strlen(p);
        int body_len = remaining;
        if (req->content_length >= 0 && req->content_length < body_len) {
            body_len = req->content_length;
        }
        if (body_len >= MAX_BODY) body_len = MAX_BODY - 1;
        memcpy(req->body, p, (size_t)body_len);
        req->body[body_len] = '\0';
        req->body_length = body_len;
    }

    free(input);
    return 0;
}

/* ---------- Checksum ---------- */

static uint32_t request_checksum(const HTTPRequest *req) {
    uint32_t cksum = 0;

    /* Hash method */
    for (const char *p = req->method; *p; p++) {
        cksum = cksum * 31 + (uint8_t)*p;
    }

    /* Hash path */
    for (const char *p = req->path; *p; p++) {
        cksum = cksum * 37 + (uint8_t)*p;
    }
    cksum ^= (cksum << 13);
    cksum ^= (cksum >> 7);

    /* Hash headers */
    for (int i = 0; i < req->header_count; i++) {
        for (const char *p = req->headers[i].name; *p; p++) {
            cksum += (uint8_t)*p * 41;
        }
        for (const char *p = req->headers[i].value; *p; p++) {
            cksum ^= (uint8_t)*p;
            cksum = (cksum << 5) | (cksum >> 27);
        }
    }

    /* Hash body */
    for (int i = 0; i < req->body_length; i++) {
        cksum += (uint8_t)req->body[i] * 2654435761u;
    }

    return cksum;
}

/* ---------- Print parsed request ---------- */

static void print_request(const HTTPRequest *req) {
    printf("  Method: %s\n", req->method);
    printf("  Path: %s\n", req->path);
    printf("  Version: %s\n", req->version);

    if (req->raw_query[0]) {
        printf("  Query: %s\n", req->raw_query);
        printf("  Query params (%d):\n", req->query_param_count);
        for (int i = 0; i < req->query_param_count; i++) {
            printf("    %s = %s\n",
                   req->query_params[i].key,
                   req->query_params[i].value);
        }
    }

    printf("  Headers (%d):\n", req->header_count);
    for (int i = 0; i < req->header_count; i++) {
        printf("    %s: %s\n",
               req->headers[i].name,
               req->headers[i].value);
    }

    if (req->body_length > 0) {
        printf("  Body (%d bytes): %s\n", req->body_length, req->body);
    }

    if (req->content_length >= 0)
        printf("  Content-Length: %d\n", req->content_length);
    printf("  Keep-Alive: %s\n", req->keep_alive ? "yes" : "no");
    printf("  Has Host: %s\n", req->has_host ? "yes" : "no");
    printf("  Chunked: %s\n", req->is_chunked ? "yes" : "no");
    if (req->content_type[0])
        printf("  Content-Type: %s\n", req->content_type);
}

/* ---------- Built-in test cases ---------- */

typedef struct {
    const char *name;
    const char *raw;
    const char *expected_method;
    const char *expected_path;
    int         expected_headers;
} TestCase;

static const TestCase test_cases[] = {
    {
        "Simple GET",
        "GET / HTTP/1.1\\r\\nHost: example.com\\r\\n\\r\\n",
        "GET", "/", 1
    },
    {
        "GET with query",
        "GET /search?q=hello+world&lang=en HTTP/1.1\\r\\n"
        "Host: example.com\\r\\n"
        "Accept: text/html\\r\\n"
        "Connection: keep-alive\\r\\n\\r\\n",
        "GET", "/search", 3
    },
    {
        "POST with body",
        "POST /api/data HTTP/1.1\\r\\n"
        "Host: api.example.com\\r\\n"
        "Content-Type: application/json\\r\\n"
        "Content-Length: 27\\r\\n"
        "\\r\\n"
        "{\"name\":\"test\",\"value\":42}",
        "POST", "/api/data", 3
    },
    {
        "HEAD request",
        "HEAD /index.html HTTP/1.1\\r\\n"
        "Host: example.com\\r\\n"
        "Accept: */*\\r\\n\\r\\n",
        "HEAD", "/index.html", 2
    },
    {
        "PUT request",
        "PUT /resource/123 HTTP/1.1\\r\\n"
        "Host: api.example.com\\r\\n"
        "Content-Type: text/plain\\r\\n"
        "Content-Length: 13\\r\\n"
        "\\r\\n"
        "updated value",
        "PUT", "/resource/123", 3
    },
    {
        "DELETE request",
        "DELETE /items/456 HTTP/1.1\\r\\n"
        "Host: api.example.com\\r\\n"
        "Authorization: Bearer token123\\r\\n\\r\\n",
        "DELETE", "/items/456", 2
    },
    {
        "URL encoded query",
        "GET /path?name=John%20Doe&city=New%20York HTTP/1.1\\r\\n"
        "Host: example.com\\r\\n\\r\\n",
        "GET", "/path", 1
    },
    {NULL, NULL, NULL, NULL, 0}
};

/* ---------- Main ---------- */

int main(int argc, char *argv[]) {
    int fail = 0;

    printf("=== HTTP Parser Benchmark ===\n");

    if (argc >= 2) {
        /* Parse request from argv[1] */
        HTTPRequest req;
        if (parse_http_request(&req, argv[1]) != 0) {
            fprintf(stderr, "Parse error: %s\n", req.error_msg);
            return 1;
        }
        printf("Parsed request:\n");
        print_request(&req);
        printf("Checksum: %08x\n", request_checksum(&req));
        printf("RESULT: PASS\n");
        return 0;
    }

    /* Run built-in test suite */
    int pass_count = 0, test_count = 0;
    uint32_t total_cksum = 0;

    for (int t = 0; test_cases[t].name; t++) {
        const TestCase *tc = &test_cases[t];
        test_count++;

        printf("\n--- Test: %s ---\n", tc->name);

        HTTPRequest req;
        int rc = parse_http_request(&req, tc->raw);

        if (rc != 0) {
            fprintf(stderr, "  FAIL: Parse error: %s\n", req.error_msg);
            fail = 1;
            continue;
        }

        int test_ok = 1;

        if (strcmp(req.method, tc->expected_method) != 0) {
            fprintf(stderr, "  FAIL: method expected '%s' got '%s'\n",
                    tc->expected_method, req.method);
            test_ok = 0;
        }

        if (strcmp(req.path, tc->expected_path) != 0) {
            fprintf(stderr, "  FAIL: path expected '%s' got '%s'\n",
                    tc->expected_path, req.path);
            test_ok = 0;
        }

        if (req.header_count != tc->expected_headers) {
            fprintf(stderr, "  FAIL: headers expected %d got %d\n",
                    tc->expected_headers, req.header_count);
            test_ok = 0;
        }

        print_request(&req);
        uint32_t cksum = request_checksum(&req);
        printf("  Checksum: %08x\n", cksum);
        total_cksum ^= cksum;

        if (test_ok) {
            printf("  Test: PASS\n");
            pass_count++;
        } else {
            printf("  Test: FAIL\n");
            fail = 1;
        }
    }

    printf("\n=== Summary ===\n");
    printf("Tests: %d/%d passed\n", pass_count, test_count);
    printf("Total checksum: %08x\n", total_cksum);

    if (!fail) {
        printf("RESULT: PASS\n");
    } else {
        printf("RESULT: FAIL\n");
    }

    return fail;
}
