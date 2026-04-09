#include "Shroud/Utils.h"
#include <cmath>

namespace shroud {

ObfRNG::ObfRNG(uint64_t seed) : rng_(seed) {}

uint32_t ObfRNG::next32() {
    return static_cast<uint32_t>(rng_());
}

uint64_t ObfRNG::next64() {
    return rng_();
}

uint32_t ObfRNG::nextInRange(uint32_t lo, uint32_t hi) {
    if (lo == hi) return lo;
    std::uniform_int_distribution<uint32_t> dist(lo, hi);
    return dist(rng_);
}

bool ObfRNG::nextBool(double probability) {
    std::bernoulli_distribution dist(probability);
    return dist(rng_);
}

namespace ModArith {

int64_t mod(int64_t a, int64_t m) {
    int64_t r = a % m;
    return r < 0 ? r + m : r;
}

int64_t powMod(int64_t base, int64_t exp, int64_t m) {
    if (m == 1) return 0;
    int64_t result = 1;
    base = mod(base, m);
    while (exp > 0) {
        if (exp & 1) result = mod(result * base, m);
        exp >>= 1;
        base = mod(base * base, m);
    }
    return result;
}

int64_t gcd(int64_t a, int64_t b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b) { int64_t t = b; b = a % b; a = t; }
    return a;
}

int64_t extGcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if (a == 0) { x = 0; y = 1; return b; }
    int64_t x1, y1;
    int64_t g = extGcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return g;
}

int64_t modInverse(int64_t a, int64_t m) {
    int64_t x, y;
    int64_t g = extGcd(a, m, x, y);
    if (g != 1) return -1;
    return mod(x, m);
}

bool isPrime(int64_t n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int64_t nextPrime(int64_t n) {
    if (n <= 2) return 2;
    if (n % 2 == 0) n++;
    while (!isPrime(n)) n += 2;
    return n;
}

bool isQuadraticResidue(int64_t a, int64_t p) {
    if (p == 2) return true;
    a = mod(a, p);
    if (a == 0) return true;
    return powMod(a, (p - 1) / 2, p) == 1;
}

int jacobiSymbol(int64_t a, int64_t n) {
    if (n <= 0 || n % 2 == 0) return 0;
    a = mod(a, n);
    int result = 1;
    while (a != 0) {
        while (a % 2 == 0) {
            a /= 2;
            if (n % 8 == 3 || n % 8 == 5) result = -result;
        }
        std::swap(a, n);
        if (a % 4 == 3 && n % 4 == 3) result = -result;
        a = mod(a, n);
    }
    return n == 1 ? result : 0;
}

} // namespace ModArith

uint32_t crc32(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }
    }
    return ~crc;
}

} // namespace shroud
