/*
 * Simple crackme — target for LLVM obfuscation passes.
 *
 * The password is checked through arithmetic operations that will be
 * obfuscated by MBA substitution. Branches will get opaque predicates
 * injected, and overlapping instructions will confuse disassemblers.
 *
 * Password: "s3cr3t_k3y"
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Simple hash function — lots of arithmetic for MBA to obfuscate */
static uint32_t hash_password(const char *password) {
    uint32_t hash = 0x5A3C1E0F;
    uint32_t len = (uint32_t)strlen(password);

    for (uint32_t i = 0; i < len; i++) {
        /* Addition, XOR, shifts — all targets for MBA */
        hash = hash + (uint32_t)password[i];
        hash = hash ^ (hash << 13);
        hash = hash + (hash >> 7);
        hash = hash ^ (hash << 5);
        hash = hash + (i * 0x9E3779B9);
    }

    /* Final mixing */
    hash = hash ^ (hash >> 16);
    hash = hash + (hash << 3);
    hash = hash ^ (hash >> 4);
    hash = hash + (hash << 10);
    hash = hash ^ (hash >> 15);

    return hash;
}

/* Multi-stage verification — multiple branches for opaque predicates */
static int verify_password(const char *input) {
    uint32_t hash = hash_password(input);
    uint32_t expected = 0xAD0E80D0; /* hash_password("s3cr3t_k3y") */

    /* Stage 1: length check */
    uint32_t len = (uint32_t)strlen(input);
    if (len < 5 || len > 20) {
        return 0;
    }

    /* Stage 2: first character check */
    if ((input[0] ^ 0x42) != ('s' ^ 0x42)) {
        return 0;
    }

    /* Stage 3: hash comparison */
    uint32_t diff = hash ^ expected;
    if (diff != 0) {
        return 0;
    }

    /* Stage 4: checksum of specific positions */
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < len && i < 10; i++) {
        checksum = checksum + ((uint32_t)input[i] * (i + 1));
    }

    if (checksum != 5111) { /* precomputed for "s3cr3t_k3y" */
        return 0;
    }

    return 1;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        return 1;
    }

    if (verify_password(argv[1])) {
        printf("Access Granted!\n");
        return 0;
    } else {
        printf("Access Denied.\n");
        return 1;
    }
}
