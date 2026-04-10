/*
 * hashtable.c - Benchmark: Open-addressing hash table
 *
 * Usage: ./hashtable < wordlist.txt
 *   Reads words from stdin (one per line), inserts into hash table,
 *   then looks up a set of test words and prints hit/miss counts.
 *
 * If no stdin is provided (or stdin is empty), uses a built-in word set.
 * Returns 0 on success, 1 on failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

/* ---------- Configuration ---------- */

#define HT_INITIAL_CAPACITY 64
#define HT_LOAD_FACTOR_NUM  7
#define HT_LOAD_FACTOR_DEN  10
#define MAX_KEY_LEN         256
#define MAX_WORDS           100000

/* ---------- Hash functions ---------- */

/* FNV-1a hash */
static uint32_t fnv1a_hash(const char *key) {
    uint32_t hash = 2166136261u;
    while (*key) {
        hash ^= (uint8_t)*key++;
        hash *= 16777619u;
    }
    return hash;
}

/* DJB2 hash (for double hashing) */
static uint32_t djb2_hash(const char *key) {
    uint32_t hash = 5381;
    while (*key) {
        hash = ((hash << 5) + hash) + (uint8_t)*key++;
    }
    return hash;
}

/* Combine hashes with bitwise mixing */
static uint32_t mix_hash(uint32_t h1, uint32_t h2) {
    uint32_t h = h1 ^ (h2 << 16 | h2 >> 16);
    h *= 2654435761u;
    h ^= h >> 13;
    h *= 1597334677u;
    h ^= h >> 15;
    return h;
}

/* ---------- Hash Table Entry ---------- */

typedef enum {
    SLOT_EMPTY = 0,
    SLOT_OCCUPIED,
    SLOT_DELETED
} SlotState;

typedef struct {
    char    *key;
    int      value;
    SlotState state;
} HTEntry;

/* ---------- Hash Table ---------- */

typedef struct {
    HTEntry *entries;
    int      capacity;
    int      size;
    int      deleted_count;
    uint32_t probe_count;    /* total probes for statistics */
    uint32_t insert_count;
    uint32_t lookup_count;
} HashTable;

static HashTable *ht_create(int capacity) {
    HashTable *ht = (HashTable *)calloc(1, sizeof(HashTable));
    if (!ht) return NULL;
    if (capacity < 8) capacity = 8;
    ht->entries = (HTEntry *)calloc((size_t)capacity, sizeof(HTEntry));
    if (!ht->entries) { free(ht); return NULL; }
    ht->capacity = capacity;
    ht->size = 0;
    ht->deleted_count = 0;
    ht->probe_count = 0;
    ht->insert_count = 0;
    ht->lookup_count = 0;
    return ht;
}

static void ht_destroy(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < ht->capacity; i++) {
        if (ht->entries[i].state == SLOT_OCCUPIED) {
            free(ht->entries[i].key);
        }
    }
    free(ht->entries);
    free(ht);
}

/* Forward declaration */
static int ht_insert(HashTable *ht, const char *key, int value);

static int ht_resize(HashTable *ht, int new_capacity) {
    HTEntry *old_entries = ht->entries;
    int old_capacity = ht->capacity;

    ht->entries = (HTEntry *)calloc((size_t)new_capacity, sizeof(HTEntry));
    if (!ht->entries) {
        ht->entries = old_entries;
        return -1;
    }
    ht->capacity = new_capacity;
    ht->size = 0;
    ht->deleted_count = 0;

    for (int i = 0; i < old_capacity; i++) {
        if (old_entries[i].state == SLOT_OCCUPIED) {
            ht_insert(ht, old_entries[i].key, old_entries[i].value);
            free(old_entries[i].key);
        }
    }
    free(old_entries);
    return 0;
}

static int ht_insert(HashTable *ht, const char *key, int value) {
    /* Check load factor and resize if needed */
    if ((ht->size + ht->deleted_count) * HT_LOAD_FACTOR_DEN >=
        ht->capacity * HT_LOAD_FACTOR_NUM) {
        if (ht_resize(ht, ht->capacity * 2) != 0) {
            return -1;
        }
    }

    uint32_t h1 = fnv1a_hash(key);
    uint32_t h2 = djb2_hash(key) | 1;  /* ensure odd for coprimality */
    uint32_t idx = h1 % (uint32_t)ht->capacity;
    int first_deleted = -1;

    for (int i = 0; i < ht->capacity; i++) {
        ht->probe_count++;
        HTEntry *e = &ht->entries[idx];

        if (e->state == SLOT_EMPTY) {
            int target = (first_deleted >= 0) ? first_deleted : (int)idx;
            HTEntry *t = &ht->entries[target];
            if (t->state == SLOT_DELETED) {
                ht->deleted_count--;
            }
            t->key = strdup(key);
            if (!t->key) return -1;
            t->value = value;
            t->state = SLOT_OCCUPIED;
            ht->size++;
            ht->insert_count++;
            return 0;
        }

        if (e->state == SLOT_DELETED && first_deleted < 0) {
            first_deleted = (int)idx;
        }

        if (e->state == SLOT_OCCUPIED && strcmp(e->key, key) == 0) {
            e->value = value;  /* update existing */
            ht->insert_count++;
            return 0;
        }

        idx = (idx + h2) % (uint32_t)ht->capacity;
    }

    return -1;  /* table full (shouldn't happen with resize) */
}

typedef struct {
    int found;
    int value;
} LookupResult;

static LookupResult ht_lookup(HashTable *ht, const char *key) {
    LookupResult result = {0, 0};
    uint32_t h1 = fnv1a_hash(key);
    uint32_t h2 = djb2_hash(key) | 1;
    uint32_t idx = h1 % (uint32_t)ht->capacity;

    ht->lookup_count++;

    for (int i = 0; i < ht->capacity; i++) {
        ht->probe_count++;
        HTEntry *e = &ht->entries[idx];

        if (e->state == SLOT_EMPTY) {
            return result;
        }
        if (e->state == SLOT_OCCUPIED && strcmp(e->key, key) == 0) {
            result.found = 1;
            result.value = e->value;
            return result;
        }
        idx = (idx + h2) % (uint32_t)ht->capacity;
    }
    return result;
}

static int ht_delete(HashTable *ht, const char *key) {
    uint32_t h1 = fnv1a_hash(key);
    uint32_t h2 = djb2_hash(key) | 1;
    uint32_t idx = h1 % (uint32_t)ht->capacity;

    for (int i = 0; i < ht->capacity; i++) {
        HTEntry *e = &ht->entries[idx];
        if (e->state == SLOT_EMPTY) return -1;
        if (e->state == SLOT_OCCUPIED && strcmp(e->key, key) == 0) {
            free(e->key);
            e->key = NULL;
            e->state = SLOT_DELETED;
            ht->size--;
            ht->deleted_count++;
            return 0;
        }
        idx = (idx + h2) % (uint32_t)ht->capacity;
    }
    return -1;
}

/* ---------- String manipulation utilities ---------- */

/* Simple string hash for checksum purposes */
static uint32_t string_checksum(const char *s) {
    uint32_t cksum = 0;
    int i = 0;
    while (*s) {
        uint32_t c = (uint8_t)*s++;
        c ^= (uint32_t)(i & 0xFF);
        c = (c << 5) | (c >> 27);
        cksum ^= c;
        cksum = (cksum << 3) | (cksum >> 29);
        cksum += c * 2654435761u;
        i++;
    }
    return cksum;
}

/* Trim trailing whitespace/newline in place */
static void trim_right(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' ||
                       s[len - 1] == ' '  || s[len - 1] == '\t')) {
        s[--len] = '\0';
    }
}

/* Convert string to lowercase in a buffer */
static void to_lower(char *dst, const char *src, int maxlen) {
    int i = 0;
    while (src[i] && i < maxlen - 1) {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') c = c + ('a' - 'A');
        dst[i] = c;
        i++;
    }
    dst[i] = '\0';
}

/* Reverse a string in place */
static void reverse_string(char *s) {
    int len = (int)strlen(s);
    for (int i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }
}

/* ---------- Built-in test data ---------- */

static const char *builtin_words[] = {
    "alpha", "bravo", "charlie", "delta", "echo",
    "foxtrot", "golf", "hotel", "india", "juliet",
    "kilo", "lima", "mike", "november", "oscar",
    "papa", "quebec", "romeo", "sierra", "tango",
    "uniform", "victor", "whiskey", "xray", "yankee",
    "zulu", "apple", "banana", "cherry", "date",
    "elderberry", "fig", "grape", "honeydew", "kiwi",
    "lemon", "mango", "nectarine", "orange", "papaya",
    "quince", "raspberry", "strawberry", "tangerine", "watermelon",
    "algorithm", "benchmark", "compiler", "debugger", "encoder",
    "framework", "generator", "hashmap", "iterator", "junction",
    NULL
};

static const char *test_lookups[] = {
    "alpha", "zebra", "charlie", "missing", "golf",
    "nothere", "papa", "xray", "compiler", "phantom",
    "banana", "unknown", "watermelon", "foo", "tango",
    NULL
};

/* ---------- Main ---------- */

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    HashTable *ht = ht_create(HT_INITIAL_CAPACITY);
    if (!ht) {
        fprintf(stderr, "Failed to create hash table\n");
        return 1;
    }

    int word_count = 0;
    uint32_t input_cksum = 0;
    char line[MAX_KEY_LEN];
    int using_stdin = 0;

    /* Try reading from stdin; if empty or not a pipe, use built-in */
    if (!isatty(0)) {
        while (fgets(line, sizeof(line), stdin) && word_count < MAX_WORDS) {
            trim_right(line);
            if (line[0] == '\0') continue;

            char lower[MAX_KEY_LEN];
            to_lower(lower, line, MAX_KEY_LEN);

            input_cksum ^= string_checksum(lower);
            ht_insert(ht, lower, word_count + 1);
            word_count++;
            using_stdin = 1;
        }
    }

    if (!using_stdin) {
        /* Use built-in words */
        for (int i = 0; builtin_words[i]; i++) {
            input_cksum ^= string_checksum(builtin_words[i]);
            ht_insert(ht, builtin_words[i], i + 1);
            word_count++;
        }
    }

    int unique_count = ht->size;  /* save unique count before mutations */

    printf("=== Hash Table Benchmark ===\n");
    printf("Words inserted: %d (unique: %d)\n", word_count, unique_count);
    printf("Table size: %d, capacity: %d\n", ht->size, ht->capacity);
    printf("Input checksum: %08x\n", input_cksum);

    /* Perform lookups */
    int hits = 0, misses = 0;
    uint32_t lookup_cksum = 0;

    for (int i = 0; test_lookups[i]; i++) {
        LookupResult r = ht_lookup(ht, test_lookups[i]);
        if (r.found) {
            hits++;
            lookup_cksum ^= mix_hash(string_checksum(test_lookups[i]),
                                     (uint32_t)r.value);
        } else {
            misses++;
            /* Try reversed version too */
            char rev[MAX_KEY_LEN];
            strncpy(rev, test_lookups[i], MAX_KEY_LEN - 1);
            rev[MAX_KEY_LEN - 1] = '\0';
            reverse_string(rev);
            LookupResult r2 = ht_lookup(ht, rev);
            if (r2.found) {
                lookup_cksum ^= string_checksum(rev);
            }
        }
    }

    /* Delete some entries and verify */
    int delete_count = 0;
    const char *to_delete[] = {"alpha", "charlie", "golf", "banana", NULL};
    int was_deleted[4] = {0, 0, 0, 0};
    for (int i = 0; to_delete[i]; i++) {
        if (ht_delete(ht, to_delete[i]) == 0) {
            was_deleted[i] = 1;
            delete_count++;
        }
    }

    /* Verify deletions - only check ones that were actually deleted */
    int delete_verified = 1;
    for (int i = 0; to_delete[i]; i++) {
        LookupResult r = ht_lookup(ht, to_delete[i]);
        if (was_deleted[i] && r.found) {
            delete_verified = 0;
            fprintf(stderr, "FAIL: '%s' still found after deletion\n",
                    to_delete[i]);
        }
    }

    /* Re-insert only the ones we deleted, and verify */
    for (int i = 0; to_delete[i]; i++) {
        if (was_deleted[i]) {
            ht_insert(ht, to_delete[i], 9999 + i);
        }
    }
    int reinsert_verified = 1;
    for (int i = 0; to_delete[i]; i++) {
        if (was_deleted[i]) {
            LookupResult r = ht_lookup(ht, to_delete[i]);
            if (!r.found || r.value != 9999 + i) {
                reinsert_verified = 0;
                fprintf(stderr, "FAIL: '%s' not found after re-insertion\n",
                        to_delete[i]);
            }
        }
    }

    printf("Hits: %d, Misses: %d\n", hits, misses);
    printf("Deleted: %d, Delete verified: %s\n", delete_count,
           delete_verified ? "yes" : "no");
    printf("Re-insert verified: %s\n", reinsert_verified ? "yes" : "no");
    printf("Lookup checksum: %08x\n", lookup_cksum);
    printf("Total probes: %u\n", ht->probe_count);

    int fail = 0;
    if (!delete_verified || !reinsert_verified) fail = 1;
    if (ht->size != unique_count) {
        fprintf(stderr, "FAIL: size mismatch after operations: got %d expected %d\n",
                ht->size, unique_count);
        fail = 1;
    }

    if (!fail) {
        printf("RESULT: PASS\n");
    } else {
        printf("RESULT: FAIL\n");
    }

    ht_destroy(ht);
    return fail;
}
