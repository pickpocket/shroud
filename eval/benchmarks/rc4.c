/*
 * rc4.c - Benchmark: RC4 stream cipher
 *
 * Usage: ./rc4 <key> <plaintext>
 *   Encrypts plaintext with the given key using RC4,
 *   then decrypts and verifies roundtrip integrity.
 *   Prints hex of ciphertext.
 *
 * Returns 0 on success, 1 on failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ---------- RC4 State ---------- */

typedef struct {
    uint8_t S[256];
    uint8_t i;
    uint8_t j;
} RC4State;

/* ---------- RC4 Key Scheduling Algorithm (KSA) ---------- */

static void rc4_init(RC4State *state, const uint8_t *key, int keylen) {
    for (int i = 0; i < 256; i++) {
        state->S[i] = (uint8_t)i;
    }

    uint8_t j = 0;
    for (int i = 0; i < 256; i++) {
        j = j + state->S[i] + key[i % keylen];
        /* Swap S[i] and S[j] */
        uint8_t tmp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = tmp;
    }

    state->i = 0;
    state->j = 0;
}

/* ---------- RC4 Pseudo-Random Generation Algorithm (PRGA) ---------- */

static uint8_t rc4_next_byte(RC4State *state) {
    state->i += 1;
    state->j += state->S[state->i];

    /* Swap S[i] and S[j] */
    uint8_t tmp = state->S[state->i];
    state->S[state->i] = state->S[state->j];
    state->S[state->j] = tmp;

    uint8_t k = state->S[(uint8_t)(state->S[state->i] + state->S[state->j])];
    return k;
}

/* ---------- RC4 Encrypt/Decrypt (symmetric) ---------- */

static void rc4_crypt(RC4State *state, const uint8_t *input,
                      uint8_t *output, int len) {
    for (int i = 0; i < len; i++) {
        output[i] = input[i] ^ rc4_next_byte(state);
    }
}

/* ---------- Hex output ---------- */

static void print_hex(const uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
}

/* ---------- Hex checksum ---------- */

static uint32_t compute_hex_checksum(const uint8_t *data, int len) {
    uint32_t cksum = 0;
    for (int i = 0; i < len; i++) {
        uint32_t v = data[i];
        v ^= (uint32_t)(i * 37);
        v = (v << 5) | (v >> 27);
        cksum ^= v;
        cksum += v * 2654435761u;
        cksum = (cksum << 7) | (cksum >> 25);
    }
    return cksum;
}

/* ---------- Bitwise analysis of keystream ---------- */

static void analyze_keystream(RC4State *state_copy, int len) {
    uint32_t byte_freq[256] = {0};
    uint32_t bit_counts[8] = {0};

    RC4State analysis_state;
    memcpy(&analysis_state, state_copy, sizeof(RC4State));

    for (int i = 0; i < len; i++) {
        uint8_t b = rc4_next_byte(&analysis_state);
        byte_freq[b]++;
        for (int bit = 0; bit < 8; bit++) {
            if (b & (1 << bit)) bit_counts[bit]++;
        }
    }

    /* Compute chi-squared-like statistic for uniformity */
    double expected = (double)len / 256.0;
    double chi2 = 0.0;
    for (int i = 0; i < 256; i++) {
        double diff = (double)byte_freq[i] - expected;
        chi2 += (diff * diff) / expected;
    }

    /* Bit balance check */
    uint32_t total_ones = 0;
    for (int bit = 0; bit < 8; bit++) {
        total_ones += bit_counts[bit];
    }

    printf("  Keystream analysis (%d bytes):\n", len);
    printf("  Chi-squared: %.2f (expected ~255 for uniform)\n", chi2);
    printf("  Total ones: %u / %u bits (%.1f%%)\n",
           total_ones, (uint32_t)(len * 8),
           100.0 * total_ones / (len * 8));
}

/* ---------- String obfuscation targets ---------- */

/* XOR-encode a string with a key byte pattern */
static void xor_encode_string(const char *input, uint8_t *output,
                               int len, uint8_t key_byte) {
    for (int i = 0; i < len; i++) {
        uint8_t k = key_byte ^ (uint8_t)(i * 31 + 17);
        k = (k << 3) | (k >> 5);  /* rotate */
        output[i] = (uint8_t)input[i] ^ k;
    }
}

static void xor_decode_string(const uint8_t *input, char *output,
                               int len, uint8_t key_byte) {
    for (int i = 0; i < len; i++) {
        uint8_t k = key_byte ^ (uint8_t)(i * 31 + 17);
        k = (k << 3) | (k >> 5);
        output[i] = (char)(input[i] ^ k);
    }
    output[len] = '\0';
}

/* ---------- Main ---------- */

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <key> <plaintext>\n", argv[0]);
        return 1;
    }

    const char *key_str = argv[1];
    const char *plaintext = argv[2];
    int keylen = (int)strlen(key_str);
    int ptlen = (int)strlen(plaintext);

    if (keylen == 0 || keylen > 256) {
        fprintf(stderr, "Key must be 1-256 bytes\n");
        return 1;
    }
    if (ptlen == 0) {
        fprintf(stderr, "Plaintext must not be empty\n");
        return 1;
    }

    printf("=== RC4 Benchmark ===\n");
    printf("Key length: %d bytes\n", keylen);
    printf("Plaintext length: %d bytes\n", ptlen);

    /* Encrypt */
    RC4State enc_state;
    rc4_init(&enc_state, (const uint8_t *)key_str, keylen);

    /* Save state after KSA for analysis */
    RC4State analysis_state;
    memcpy(&analysis_state, &enc_state, sizeof(RC4State));

    uint8_t *ciphertext = (uint8_t *)malloc((size_t)ptlen);
    if (!ciphertext) { fprintf(stderr, "malloc failed\n"); return 1; }

    rc4_crypt(&enc_state, (const uint8_t *)plaintext, ciphertext, ptlen);

    /* Print ciphertext hex */
    printf("Ciphertext: ");
    print_hex(ciphertext, ptlen);
    printf("\n");

    /* Compute ciphertext checksum */
    uint32_t ct_cksum = compute_hex_checksum(ciphertext, ptlen);
    printf("CT checksum: %08x\n", ct_cksum);

    /* Decrypt */
    RC4State dec_state;
    rc4_init(&dec_state, (const uint8_t *)key_str, keylen);

    uint8_t *decrypted = (uint8_t *)malloc((size_t)ptlen + 1);
    if (!decrypted) { free(ciphertext); return 1; }

    rc4_crypt(&dec_state, ciphertext, decrypted, ptlen);
    decrypted[ptlen] = '\0';

    /* Verify roundtrip */
    int fail = 0;
    if (memcmp(plaintext, decrypted, (size_t)ptlen) != 0) {
        fprintf(stderr, "FAIL: Decrypted text does not match plaintext\n");
        fprintf(stderr, "  Expected: %s\n", plaintext);
        fprintf(stderr, "  Got:      %s\n", decrypted);
        fail = 1;
    }

    /* Test XOR string encode/decode */
    uint8_t encoded[256];
    char decoded[256];
    int test_len = (ptlen < 255) ? ptlen : 255;
    xor_encode_string(plaintext, encoded, test_len, 0xAB);
    xor_decode_string(encoded, decoded, test_len, 0xAB);
    if (memcmp(plaintext, decoded, (size_t)test_len) != 0) {
        fprintf(stderr, "FAIL: XOR string encode/decode roundtrip failed\n");
        fail = 1;
    }

    /* Keystream analysis */
    if (ptlen >= 64) {
        analyze_keystream(&analysis_state, ptlen);
    }

    /* Multiple rounds test */
    printf("Multi-round test:\n");
    uint8_t *buf1 = (uint8_t *)malloc((size_t)ptlen);
    uint8_t *buf2 = (uint8_t *)malloc((size_t)ptlen);
    if (buf1 && buf2) {
        memcpy(buf1, (const uint8_t *)plaintext, (size_t)ptlen);
        for (int round = 0; round < 10; round++) {
            RC4State rs;
            rc4_init(&rs, (const uint8_t *)key_str, keylen);
            rc4_crypt(&rs, buf1, buf2, ptlen);

            /* Swap buffers */
            uint8_t *tmp = buf1;
            buf1 = buf2;
            buf2 = tmp;
        }
        /* After 10 encryptions, decrypt 10 times to recover */
        for (int round = 0; round < 10; round++) {
            RC4State rs;
            rc4_init(&rs, (const uint8_t *)key_str, keylen);
            rc4_crypt(&rs, buf1, buf2, ptlen);

            uint8_t *tmp = buf1;
            buf1 = buf2;
            buf2 = tmp;
        }
        if (memcmp(plaintext, buf1, (size_t)ptlen) != 0) {
            fprintf(stderr, "FAIL: Multi-round encrypt/decrypt failed\n");
            fail = 1;
        } else {
            printf("  20 rounds (10 enc + 10 dec): OK\n");
        }
    }
    free(buf1);
    free(buf2);

    if (!fail) {
        printf("RESULT: PASS\n");
    } else {
        printf("RESULT: FAIL\n");
    }

    free(ciphertext);
    free(decrypted);
    return fail;
}
