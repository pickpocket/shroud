#!/bin/bash
#
# run_benchmarks.sh - Compile and test all benchmark programs
#
# Usage: ./run_benchmarks.sh [clang_path]
#   Optional: provide path to clang if not in PATH
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

CC="${1:-clang}"
CFLAGS="-O1 -std=c11 -Wall -Wextra"

PASS=0
FAIL=0
TOTAL=0

# Color output (if terminal supports it)
RED=""
GREEN=""
RESET=""
if [ -t 1 ]; then
    RED="\033[31m"
    GREEN="\033[32m"
    RESET="\033[0m"
fi

report() {
    local name="$1"
    local status="$2"
    TOTAL=$((TOTAL + 1))
    if [ "$status" = "PASS" ]; then
        PASS=$((PASS + 1))
        echo -e "  ${GREEN}PASS${RESET} - $name"
    else
        FAIL=$((FAIL + 1))
        echo -e "  ${RED}FAIL${RESET} - $name"
    fi
}

compile() {
    local src="$1"
    local out="$2"
    echo "  Compiling $src -> $out"
    if $CC $CFLAGS -o "$out" "$src" 2>&1; then
        return 0
    else
        return 1
    fi
}

echo "========================================"
echo "  Benchmark Suite"
echo "  Compiler: $CC"
echo "========================================"
echo ""

# ---------- 1. sort ----------
echo "--- sort ---"
if compile sort.c bench_sort; then
    output=$(./bench_sort 1000 2>&1)
    if echo "$output" | grep -q "RESULT: PASS"; then
        report "sort (N=1000)" "PASS"
    else
        echo "$output"
        report "sort (N=1000)" "FAIL"
    fi
    # Second run with different N for consistency check
    output2=$(./bench_sort 500 2>&1)
    if echo "$output2" | grep -q "RESULT: PASS"; then
        report "sort (N=500)" "PASS"
    else
        report "sort (N=500)" "FAIL"
    fi
else
    report "sort (compile)" "FAIL"
fi
echo ""

# ---------- 2. hashtable ----------
echo "--- hashtable ---"
if compile hashtable.c bench_hashtable; then
    # Run with built-in data (no stdin / empty pipe)
    output=$(./bench_hashtable < /dev/null 2>&1)
    if echo "$output" | grep -q "RESULT: PASS"; then
        report "hashtable (builtin)" "PASS"
    else
        echo "$output"
        report "hashtable (builtin)" "FAIL"
    fi
    # Run with piped word list
    output2=$(printf "hello\nworld\nfoo\nbar\nbaz\nqux\nhello\nworld\n" | ./bench_hashtable 2>&1)
    if echo "$output2" | grep -q "RESULT: PASS"; then
        report "hashtable (stdin)" "PASS"
    else
        echo "$output2"
        report "hashtable (stdin)" "FAIL"
    fi
else
    report "hashtable (compile)" "FAIL"
fi
echo ""

# ---------- 3. rc4 ----------
echo "--- rc4 ---"
if compile rc4.c bench_rc4; then
    output=$(./bench_rc4 "secretkey" "Hello, World! This is a test of the RC4 stream cipher implementation." 2>&1)
    if echo "$output" | grep -q "RESULT: PASS"; then
        report "rc4 (basic)" "PASS"
    else
        echo "$output"
        report "rc4 (basic)" "FAIL"
    fi
    # Second test with different key
    output2=$(./bench_rc4 "anotherkey123" "The quick brown fox jumps over the lazy dog" 2>&1)
    if echo "$output2" | grep -q "RESULT: PASS"; then
        report "rc4 (alternate key)" "PASS"
    else
        echo "$output2"
        report "rc4 (alternate key)" "FAIL"
    fi
else
    report "rc4 (compile)" "FAIL"
fi
echo ""

# ---------- 4. minihttp ----------
echo "--- minihttp ---"
if compile minihttp.c bench_minihttp; then
    # Run built-in test suite (no arguments)
    output=$(./bench_minihttp 2>&1)
    if echo "$output" | grep -q "RESULT: PASS"; then
        report "minihttp (builtin tests)" "PASS"
    else
        echo "$output"
        report "minihttp (builtin tests)" "FAIL"
    fi
    # Run with explicit request
    output2=$(./bench_minihttp 'GET /api/v1/users?page=1&limit=20 HTTP/1.1\r\nHost: api.example.com\r\nAccept: application/json\r\n\r\n' 2>&1)
    if echo "$output2" | grep -q "RESULT: PASS"; then
        report "minihttp (argv)" "PASS"
    else
        echo "$output2"
        report "minihttp (argv)" "FAIL"
    fi
else
    report "minihttp (compile)" "FAIL"
fi
echo ""

# ---------- 5. argparse ----------
echo "--- argparse ---"
if compile argparse.c bench_argparse; then
    # Run built-in test suite
    output=$(./bench_argparse 2>&1)
    if echo "$output" | grep -q "RESULT: PASS"; then
        report "argparse (builtin tests)" "PASS"
    else
        echo "$output"
        report "argparse (builtin tests)" "FAIL"
    fi
    # Run with actual arguments
    output2=$(./bench_argparse --verbose --count=42 --format=json -o result.txt file1.c file2.c 2>&1)
    if echo "$output2" | grep -q "RESULT: PASS"; then
        report "argparse (argv)" "PASS"
    else
        echo "$output2"
        report "argparse (argv)" "FAIL"
    fi
else
    report "argparse (compile)" "FAIL"
fi
echo ""

# ---------- Summary ----------
echo "========================================"
echo "  Results: $PASS/$TOTAL passed, $FAIL failed"
echo "========================================"

if [ "$FAIL" -gt 0 ]; then
    exit 1
fi
exit 0
