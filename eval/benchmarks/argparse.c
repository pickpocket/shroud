/*
 * argparse.c - Benchmark: Command-line argument parser
 *
 * Usage: ./argparse [options] [positional args...]
 *   Parses arguments according to a hardcoded schema:
 *     --verbose / -v       : flag
 *     --output=FILE / -o   : string option
 *     --count=N / -n       : integer option
 *     --threshold=F / -t   : float option
 *     --help / -h          : flag
 *     --debug / -d         : flag
 *     --format=FMT / -f    : string option (json|csv|text)
 *     --                   : end of options
 *     positional args      : collected into a list
 *
 * If no arguments given, runs built-in test suite.
 * Returns 0 on success, 1 on failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ---------- Configuration ---------- */

#define MAX_POSITIONAL  32
#define MAX_OPTIONS     16
#define MAX_VALUE_LEN   256
#define MAX_NAME_LEN    64

/* ---------- Option types ---------- */

typedef enum {
    OPT_FLAG,
    OPT_STRING,
    OPT_INT,
    OPT_FLOAT
} OptType;

typedef struct {
    char     long_name[MAX_NAME_LEN];   /* e.g. "verbose" */
    char     short_name;                 /* e.g. 'v' */
    OptType  type;
    int      required;
    char     description[MAX_VALUE_LEN];
    /* Validation for string type */
    const char *allowed_values[8];       /* NULL-terminated list */
    int      allow_count;
    /* Defaults */
    int      default_int;
    double   default_float;
    const char *default_string;
} OptSpec;

typedef struct {
    int    is_set;
    int    flag_value;
    char   string_value[MAX_VALUE_LEN];
    int    int_value;
    double float_value;
} OptValue;

typedef struct {
    OptValue    options[MAX_OPTIONS];
    int         option_count;
    char        positional[MAX_POSITIONAL][MAX_VALUE_LEN];
    int         positional_count;
    int         has_error;
    char        error_msg[MAX_VALUE_LEN];
} ParseResult;

/* ---------- Schema definition ---------- */

static const OptSpec schema[] = {
    {"verbose",   'v', OPT_FLAG,   0, "Enable verbose output",
     {NULL}, 0, 0, 0.0, NULL},
    {"output",    'o', OPT_STRING, 0, "Output file path",
     {NULL}, 0, 0, 0.0, "stdout"},
    {"count",     'n', OPT_INT,    0, "Number of iterations",
     {NULL}, 0, 1, 0.0, NULL},
    {"threshold", 't', OPT_FLOAT,  0, "Threshold value",
     {NULL}, 0, 0, 0.5, NULL},
    {"help",      'h', OPT_FLAG,   0, "Show help",
     {NULL}, 0, 0, 0.0, NULL},
    {"debug",     'd', OPT_FLAG,   0, "Enable debug mode",
     {NULL}, 0, 0, 0.0, NULL},
    {"format",    'f', OPT_STRING, 0, "Output format",
     {"json", "csv", "text", NULL}, 3, 0, 0.0, "text"},
};

#define SCHEMA_SIZE (int)(sizeof(schema) / sizeof(schema[0]))

/* ---------- String utilities ---------- */

static int str_equal(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static uint32_t hash_string(const char *s) {
    uint32_t h = 5381;
    while (*s) {
        h = ((h << 5) + h) ^ (uint8_t)*s++;
    }
    return h;
}

static void obfuscated_copy(char *dst, const char *src, int maxlen) {
    /* Copy with XOR round-trip (identity, but exercises bitwise ops) */
    int i = 0;
    while (src[i] && i < maxlen - 1) {
        uint8_t c = (uint8_t)src[i];
        c ^= 0xAA;          /* XOR with key */
        c = (c << 3) | (c >> 5);  /* rotate left 3 */
        c = (c >> 3) | (c << 5);  /* rotate right 3 (undo) */
        c ^= 0xAA;          /* XOR again (undo) */
        dst[i] = (char)c;
        i++;
    }
    dst[i] = '\0';
}

/* ---------- Find option by name ---------- */

static int find_long_option(const char *name) {
    for (int i = 0; i < SCHEMA_SIZE; i++) {
        if (str_equal(schema[i].long_name, name)) return i;
    }
    return -1;
}

static int find_short_option(char c) {
    for (int i = 0; i < SCHEMA_SIZE; i++) {
        if (schema[i].short_name == c) return i;
    }
    return -1;
}

/* ---------- Validate string option value ---------- */

static int validate_string_value(int opt_idx, const char *value) {
    if (schema[opt_idx].allow_count == 0) return 1; /* no restrictions */
    for (int i = 0; i < schema[opt_idx].allow_count; i++) {
        if (schema[opt_idx].allowed_values[i] &&
            str_equal(schema[opt_idx].allowed_values[i], value)) {
            return 1;
        }
    }
    return 0;
}

/* ---------- Set option value ---------- */

static int set_option(ParseResult *result, int opt_idx, const char *value) {
    OptValue *ov = &result->options[opt_idx];
    ov->is_set = 1;

    switch (schema[opt_idx].type) {
    case OPT_FLAG:
        ov->flag_value = 1;
        break;
    case OPT_STRING:
        if (!value) {
            snprintf(result->error_msg, MAX_VALUE_LEN,
                     "Option --%s requires a value", schema[opt_idx].long_name);
            result->has_error = 1;
            return -1;
        }
        if (!validate_string_value(opt_idx, value)) {
            snprintf(result->error_msg, MAX_VALUE_LEN,
                     "Invalid value '%s' for --%s", value,
                     schema[opt_idx].long_name);
            result->has_error = 1;
            return -1;
        }
        obfuscated_copy(ov->string_value, value, MAX_VALUE_LEN);
        break;
    case OPT_INT:
        if (!value) {
            snprintf(result->error_msg, MAX_VALUE_LEN,
                     "Option --%s requires a value", schema[opt_idx].long_name);
            result->has_error = 1;
            return -1;
        }
        ov->int_value = atoi(value);
        break;
    case OPT_FLOAT:
        if (!value) {
            snprintf(result->error_msg, MAX_VALUE_LEN,
                     "Option --%s requires a value", schema[opt_idx].long_name);
            result->has_error = 1;
            return -1;
        }
        ov->float_value = atof(value);
        break;
    }
    return 0;
}

/* ---------- Initialize defaults ---------- */

static void init_defaults(ParseResult *result) {
    memset(result, 0, sizeof(ParseResult));
    result->option_count = SCHEMA_SIZE;

    for (int i = 0; i < SCHEMA_SIZE; i++) {
        result->options[i].int_value = schema[i].default_int;
        result->options[i].float_value = schema[i].default_float;
        if (schema[i].default_string) {
            obfuscated_copy(result->options[i].string_value,
                           schema[i].default_string, MAX_VALUE_LEN);
        }
    }
}

/* ---------- Parse arguments ---------- */

static int parse_args(int argc, char *argv[], ParseResult *result) {
    init_defaults(result);

    int i = 1;  /* skip program name */
    int options_ended = 0;

    while (i < argc) {
        const char *arg = argv[i];

        /* End of options marker */
        if (!options_ended && str_equal(arg, "--")) {
            options_ended = 1;
            i++;
            continue;
        }

        /* Long option: --name or --name=value */
        if (!options_ended && arg[0] == '-' && arg[1] == '-' && arg[2]) {
            const char *eq = strchr(arg + 2, '=');
            char name[MAX_NAME_LEN];
            const char *value = NULL;

            if (eq) {
                int nlen = (int)(eq - arg - 2);
                if (nlen >= MAX_NAME_LEN) nlen = MAX_NAME_LEN - 1;
                memcpy(name, arg + 2, (size_t)nlen);
                name[nlen] = '\0';
                value = eq + 1;
            } else {
                strncpy(name, arg + 2, MAX_NAME_LEN - 1);
                name[MAX_NAME_LEN - 1] = '\0';
            }

            int idx = find_long_option(name);
            if (idx < 0) {
                snprintf(result->error_msg, MAX_VALUE_LEN,
                         "Unknown option: --%s", name);
                result->has_error = 1;
                return -1;
            }

            /* If option needs value and none in '=', take next arg */
            if (schema[idx].type != OPT_FLAG && !value) {
                if (i + 1 < argc) {
                    value = argv[++i];
                }
            }

            if (set_option(result, idx, value) != 0) return -1;
            i++;
            continue;
        }

        /* Short option: -x or -x value, or bundled -vd */
        if (!options_ended && arg[0] == '-' && arg[1] && arg[1] != '-') {
            int ci = 1;
            while (arg[ci]) {
                int idx = find_short_option(arg[ci]);
                if (idx < 0) {
                    snprintf(result->error_msg, MAX_VALUE_LEN,
                             "Unknown option: -%c", arg[ci]);
                    result->has_error = 1;
                    return -1;
                }

                if (schema[idx].type == OPT_FLAG) {
                    set_option(result, idx, NULL);
                    ci++;
                } else {
                    /* Non-flag short option: rest of string or next arg */
                    const char *value = NULL;
                    if (arg[ci + 1]) {
                        value = &arg[ci + 1];
                    } else if (i + 1 < argc) {
                        value = argv[++i];
                    }
                    if (set_option(result, idx, value) != 0) return -1;
                    goto next_arg;
                }
            }
            i++;
            continue;
        next_arg:
            i++;
            continue;
        }

        /* Positional argument */
        if (result->positional_count < MAX_POSITIONAL) {
            obfuscated_copy(result->positional[result->positional_count],
                           arg, MAX_VALUE_LEN);
            result->positional_count++;
        }
        i++;
    }

    /* Check required options */
    for (int j = 0; j < SCHEMA_SIZE; j++) {
        if (schema[j].required && !result->options[j].is_set) {
            snprintf(result->error_msg, MAX_VALUE_LEN,
                     "Required option --%s not provided",
                     schema[j].long_name);
            result->has_error = 1;
            return -1;
        }
    }

    return 0;
}

/* ---------- Print parsed result ---------- */

static void print_result(const ParseResult *result) {
    printf("Options:\n");
    for (int i = 0; i < SCHEMA_SIZE; i++) {
        const OptValue *ov = &result->options[i];
        printf("  --%s (-%c): ", schema[i].long_name, schema[i].short_name);
        if (!ov->is_set) {
            printf("[default] ");
        }
        switch (schema[i].type) {
        case OPT_FLAG:
            printf("%s\n", ov->flag_value ? "true" : "false");
            break;
        case OPT_STRING:
            printf("\"%s\"\n", ov->string_value);
            break;
        case OPT_INT:
            printf("%d\n", ov->int_value);
            break;
        case OPT_FLOAT:
            printf("%.4f\n", ov->float_value);
            break;
        }
    }

    printf("Positional (%d):\n", result->positional_count);
    for (int i = 0; i < result->positional_count; i++) {
        printf("  [%d] %s\n", i, result->positional[i]);
    }
}

/* ---------- Checksum ---------- */

static uint32_t result_checksum(const ParseResult *result) {
    uint32_t cksum = 0;
    for (int i = 0; i < SCHEMA_SIZE; i++) {
        const OptValue *ov = &result->options[i];
        cksum ^= hash_string(schema[i].long_name);
        if (ov->is_set) {
            cksum += (uint32_t)(i + 1) * 2654435761u;
        }
        switch (schema[i].type) {
        case OPT_FLAG:
            cksum ^= (uint32_t)ov->flag_value << (i & 15);
            break;
        case OPT_STRING:
            cksum ^= hash_string(ov->string_value);
            break;
        case OPT_INT:
            cksum += (uint32_t)ov->int_value * 1597334677u;
            break;
        case OPT_FLOAT:
            cksum ^= (uint32_t)(ov->float_value * 1000.0);
            break;
        }
    }
    for (int i = 0; i < result->positional_count; i++) {
        cksum ^= hash_string(result->positional[i]) * (uint32_t)(i + 1);
    }
    return cksum;
}

/* ---------- Built-in test cases ---------- */

typedef struct {
    const char *name;
    int argc;
    const char *argv[16];
    int expect_error;
    int expect_verbose;
    int expect_count;
    int expect_positional;
} ArgTestCase;

static const ArgTestCase arg_tests[] = {
    {
        "No arguments",
        1, {"prog"},
        0, 0, 1, 0
    },
    {
        "Long flags",
        3, {"prog", "--verbose", "--debug"},
        0, 1, 1, 0
    },
    {
        "Short flags bundled",
        2, {"prog", "-vd"},
        0, 1, 1, 0
    },
    {
        "Long option with =",
        3, {"prog", "--output=file.txt", "--count=42"},
        0, 0, 42, 0
    },
    {
        "Short option with value",
        5, {"prog", "-o", "out.dat", "-n", "10"},
        0, 0, 10, 0
    },
    {
        "Mixed flags and positional",
        6, {"prog", "-v", "--count=5", "file1.txt", "file2.txt", "file3.txt"},
        0, 1, 5, 3
    },
    {
        "Double dash separator",
        5, {"prog", "--verbose", "--", "--not-an-option", "pos"},
        0, 1, 1, 2
    },
    {
        "Format validation (valid)",
        3, {"prog", "--format=json", "input.dat"},
        0, 0, 1, 1
    },
    {
        "Format validation (invalid)",
        2, {"prog", "--format=xml"},
        1, 0, 1, 0
    },
    {
        "Unknown option",
        2, {"prog", "--nonexistent"},
        1, 0, 1, 0
    },
    {
        "Float option",
        3, {"prog", "--threshold=0.75", "-v"},
        0, 1, 1, 0
    },
    {NULL, 0, {NULL}, 0, 0, 0, 0}
};

/* ---------- Main ---------- */

int main(int argc, char *argv[]) {
    printf("=== Argparse Benchmark ===\n");

    if (argc > 1) {
        /* Parse real arguments */
        ParseResult result;
        int rc = parse_args(argc, argv, &result);
        if (rc != 0) {
            fprintf(stderr, "Parse error: %s\n", result.error_msg);
            return 1;
        }
        print_result(&result);
        printf("Checksum: %08x\n", result_checksum(&result));
        printf("RESULT: PASS\n");
        return 0;
    }

    /* Run built-in test suite */
    int pass_count = 0, test_count = 0, fail = 0;
    uint32_t total_cksum = 0;

    for (int t = 0; arg_tests[t].name; t++) {
        const ArgTestCase *tc = &arg_tests[t];
        test_count++;

        printf("\n--- Test: %s ---\n", tc->name);

        ParseResult result;
        /* Cast away const for argv simulation */
        int rc = parse_args(tc->argc, (char **)tc->argv, &result);

        if (tc->expect_error) {
            if (rc == 0) {
                fprintf(stderr, "  FAIL: expected error but got success\n");
                fail = 1;
                continue;
            }
            printf("  Got expected error: %s\n", result.error_msg);
            pass_count++;
            continue;
        }

        if (rc != 0) {
            fprintf(stderr, "  FAIL: unexpected error: %s\n",
                    result.error_msg);
            fail = 1;
            continue;
        }

        int test_ok = 1;

        /* Check verbose flag */
        int idx_verbose = find_long_option("verbose");
        if (result.options[idx_verbose].flag_value != tc->expect_verbose) {
            fprintf(stderr, "  FAIL: verbose expected %d got %d\n",
                    tc->expect_verbose,
                    result.options[idx_verbose].flag_value);
            test_ok = 0;
        }

        /* Check count value */
        int idx_count = find_long_option("count");
        if (result.options[idx_count].int_value != tc->expect_count) {
            fprintf(stderr, "  FAIL: count expected %d got %d\n",
                    tc->expect_count,
                    result.options[idx_count].int_value);
            test_ok = 0;
        }

        /* Check positional count */
        if (result.positional_count != tc->expect_positional) {
            fprintf(stderr, "  FAIL: positional expected %d got %d\n",
                    tc->expect_positional, result.positional_count);
            test_ok = 0;
        }

        print_result(&result);
        uint32_t cksum = result_checksum(&result);
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
