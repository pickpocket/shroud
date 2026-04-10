/*
 * sort.c - Benchmark: Quicksort and Mergesort on integer arrays
 *
 * Usage: ./sort N
 *   Generates a deterministic pseudo-random sequence of N integers (seed=42),
 *   sorts with both quicksort and mergesort, verifies they match,
 *   prints a checksum of the sorted array.
 *
 * Returns 0 on success, 1 on failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ---------- Deterministic PRNG (xorshift32) ---------- */

static uint32_t prng_state = 42;

static void prng_seed(uint32_t seed) {
    prng_state = seed ? seed : 1;
}

static uint32_t prng_next(void) {
    uint32_t x = prng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    prng_state = x;
    return x;
}

/* ---------- Array utilities ---------- */

static void fill_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = (int)(prng_next() % 1000000) - 500000;
    }
}

static void copy_array(const int *src, int *dst, int n) {
    memcpy(dst, src, (size_t)n * sizeof(int));
}

static int arrays_equal(const int *a, const int *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

static int is_sorted(const int *arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) return 0;
    }
    return 1;
}

/* Compute a simple checksum: XOR of (element ^ index) with rotations */
static uint32_t compute_checksum(const int *arr, int n) {
    uint32_t cksum = 0;
    for (int i = 0; i < n; i++) {
        uint32_t val = (uint32_t)arr[i] ^ (uint32_t)i;
        val = (val << 7) | (val >> 25);   /* rotate left 7 */
        cksum ^= val;
        cksum += (uint32_t)arr[i] * 2654435761u;  /* Knuth multiplicative */
    }
    return cksum;
}

/* ---------- Quicksort ---------- */

static void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Median-of-three pivot selection */
static int median_of_three(int *arr, int lo, int hi) {
    int mid = lo + (hi - lo) / 2;
    if (arr[lo] > arr[mid]) swap(&arr[lo], &arr[mid]);
    if (arr[lo] > arr[hi])  swap(&arr[lo], &arr[hi]);
    if (arr[mid] > arr[hi]) swap(&arr[mid], &arr[hi]);
    return mid;
}

/* Lomuto partition scheme */
static int partition(int *arr, int lo, int hi) {
    int pivot_idx = median_of_three(arr, lo, hi);
    swap(&arr[pivot_idx], &arr[hi]);
    int pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[hi]);
    return i + 1;
}

/* Insertion sort for small subarrays */
static void insertion_sort(int *arr, int lo, int hi) {
    for (int i = lo + 1; i <= hi; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= lo && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

static void quicksort_impl(int *arr, int lo, int hi) {
    while (lo < hi) {
        if (hi - lo < 16) {
            insertion_sort(arr, lo, hi);
            return;
        }
        int p = partition(arr, lo, hi);
        /* Recurse on smaller half, iterate on larger */
        if (p - lo < hi - p) {
            quicksort_impl(arr, lo, p - 1);
            lo = p + 1;
        } else {
            quicksort_impl(arr, p + 1, hi);
            hi = p - 1;
        }
    }
}

static void quicksort(int *arr, int n) {
    if (n > 1) {
        quicksort_impl(arr, 0, n - 1);
    }
}

/* ---------- Mergesort ---------- */

static void merge(int *arr, int *tmp, int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k = lo;

    while (i <= mid && j <= hi) {
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        tmp[k++] = arr[i++];
    }
    while (j <= hi) {
        tmp[k++] = arr[j++];
    }
    for (int x = lo; x <= hi; x++) {
        arr[x] = tmp[x];
    }
}

static void mergesort_impl(int *arr, int *tmp, int lo, int hi) {
    if (hi - lo < 16) {
        insertion_sort(arr, lo, hi);
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergesort_impl(arr, tmp, lo, mid);
    mergesort_impl(arr, tmp, mid + 1, hi);
    /* Skip merge if already sorted at boundary */
    if (arr[mid] <= arr[mid + 1]) return;
    merge(arr, tmp, lo, mid, hi);
}

static void mergesort(int *arr, int n) {
    if (n <= 1) return;
    int *tmp = (int *)malloc((size_t)n * sizeof(int));
    if (!tmp) {
        fprintf(stderr, "mergesort: malloc failed\n");
        exit(1);
    }
    mergesort_impl(arr, tmp, 0, n - 1);
    free(tmp);
}

/* ---------- Verification ---------- */

static int verify_sort(const int *original, int n,
                       void (*sort_fn)(int *, int),
                       const char *name) {
    int *working = (int *)malloc((size_t)n * sizeof(int));
    if (!working) {
        fprintf(stderr, "malloc failed for %s\n", name);
        return 1;
    }
    copy_array(original, working, n);
    sort_fn(working, n);

    if (!is_sorted(working, n)) {
        fprintf(stderr, "FAIL: %s did not produce sorted output\n", name);
        free(working);
        return 1;
    }

    /* Count elements to verify no data loss (simple sum check) */
    int64_t sum_orig = 0, sum_sorted = 0;
    for (int i = 0; i < n; i++) {
        sum_orig += original[i];
        sum_sorted += working[i];
    }
    if (sum_orig != sum_sorted) {
        fprintf(stderr, "FAIL: %s element sum mismatch\n", name);
        free(working);
        return 1;
    }

    free(working);
    return 0;
}

/* ---------- Statistics on the array ---------- */

static void print_stats(const int *arr, int n) {
    if (n == 0) return;
    int64_t sum = 0;
    int mn = arr[0], mx = arr[0];
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        if (arr[i] < mn) mn = arr[i];
        if (arr[i] > mx) mx = arr[i];
    }
    printf("  elements: %d\n", n);
    printf("  min: %d, max: %d\n", mn, mx);
    printf("  sum: %lld\n", (long long)sum);
}

/* ---------- Bitwise analysis of sorted array ---------- */

static uint32_t bitwise_analysis(const int *arr, int n) {
    uint32_t or_all = 0, and_all = 0xFFFFFFFF, xor_all = 0;
    for (int i = 0; i < n; i++) {
        uint32_t v = (uint32_t)arr[i];
        or_all  |= v;
        and_all &= v;
        xor_all ^= v;
    }
    /* Mix the bits */
    uint32_t result = or_all;
    result ^= (and_all << 16) | (and_all >> 16);
    result += xor_all;
    result ^= result >> 11;
    result *= 2654435761u;
    result ^= result >> 13;
    return result;
}

/* ---------- Main ---------- */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s N\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0 || n > 10000000) {
        fprintf(stderr, "N must be between 1 and 10000000\n");
        return 1;
    }

    /* Generate deterministic array */
    prng_seed(42);
    int *original = (int *)malloc((size_t)n * sizeof(int));
    if (!original) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    fill_array(original, n);

    printf("=== Sort Benchmark (N=%d) ===\n", n);
    printf("Input array stats:\n");
    print_stats(original, n);

    /* Sort with quicksort */
    int *qs_result = (int *)malloc((size_t)n * sizeof(int));
    if (!qs_result) { free(original); return 1; }
    copy_array(original, qs_result, n);
    quicksort(qs_result, n);

    /* Sort with mergesort */
    int *ms_result = (int *)malloc((size_t)n * sizeof(int));
    if (!ms_result) { free(original); free(qs_result); return 1; }
    copy_array(original, ms_result, n);
    mergesort(ms_result, n);

    /* Verify both are sorted */
    int fail = 0;
    if (!is_sorted(qs_result, n)) {
        fprintf(stderr, "FAIL: quicksort output not sorted\n");
        fail = 1;
    }
    if (!is_sorted(ms_result, n)) {
        fprintf(stderr, "FAIL: mergesort output not sorted\n");
        fail = 1;
    }

    /* Verify both produce the same result */
    if (!arrays_equal(qs_result, ms_result, n)) {
        fprintf(stderr, "FAIL: quicksort and mergesort produced different results\n");
        fail = 1;
    }

    /* Verify with the verify_sort function */
    if (verify_sort(original, n, quicksort, "quicksort") != 0) fail = 1;
    if (verify_sort(original, n, mergesort, "mergesort") != 0) fail = 1;

    /* Compute checksums */
    uint32_t cksum = compute_checksum(qs_result, n);
    uint32_t bitmix = bitwise_analysis(qs_result, n);

    printf("Sorted array stats:\n");
    print_stats(qs_result, n);
    printf("Checksum: %08x\n", cksum);
    printf("Bitmix:   %08x\n", bitmix);

    if (!fail) {
        printf("RESULT: PASS\n");
    } else {
        printf("RESULT: FAIL\n");
    }

    free(original);
    free(qs_result);
    free(ms_result);

    return fail;
}
