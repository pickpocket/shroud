#pragma once

#include <cstdint>
#include <random>
#include <vector>
#include <array>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cassert>

namespace shroud {

// Seedable PRNG wrapper for deterministic-but-unique per-compilation behavior
class ObfRNG {
public:
    explicit ObfRNG(uint64_t seed = 0);

    uint32_t next32();
    uint64_t next64();
    uint32_t nextInRange(uint32_t lo, uint32_t hi); // inclusive
    bool nextBool(double probability = 0.5);

    template<typename T>
    T& pickRandom(std::vector<T>& vec) {
        return vec[nextInRange(0, static_cast<uint32_t>(vec.size()) - 1)];
    }

    template<typename T, size_t N>
    T& pickRandom(std::array<T, N>& arr) {
        return arr[nextInRange(0, N - 1)];
    }

    std::mt19937_64& engine() { return rng_; }

private:
    std::mt19937_64 rng_;
};

// Modular arithmetic helpers
namespace ModArith {
    int64_t mod(int64_t a, int64_t m);
    int64_t powMod(int64_t base, int64_t exp, int64_t m);
    int64_t gcd(int64_t a, int64_t b);
    int64_t extGcd(int64_t a, int64_t b, int64_t& x, int64_t& y);
    int64_t modInverse(int64_t a, int64_t m); // returns -1 if not invertible
    bool isPrime(int64_t n);
    int64_t nextPrime(int64_t n);

    // Quadratic residue check: is 'a' a QR mod p?
    bool isQuadraticResidue(int64_t a, int64_t p);

    // Jacobi symbol (a/n)
    int jacobiSymbol(int64_t a, int64_t n);
}

// Simple CRC32 for self-checksumming (not for crypto)
uint32_t crc32(const uint8_t* data, size_t length);

// Popcount
inline int popcount32(uint32_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcount(x);
#else
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    return (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
#endif
}

inline int clz32(uint32_t x) {
    if (x == 0) return 32;
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_clz(x);
#else
    int n = 0;
    if (x <= 0x0000FFFF) { n += 16; x <<= 16; }
    if (x <= 0x00FFFFFF) { n += 8;  x <<= 8;  }
    if (x <= 0x0FFFFFFF) { n += 4;  x <<= 4;  }
    if (x <= 0x3FFFFFFF) { n += 2;  x <<= 2;  }
    if (x <= 0x7FFFFFFF) { n += 1; }
    return n;
#endif
}

// Rotate operations
inline uint32_t rotl32(uint32_t x, unsigned n) {
    n &= 31;
    return (x << n) | (x >> (32 - n));
}

inline uint32_t rotr32(uint32_t x, unsigned n) {
    n &= 31;
    return (x >> n) | (x << (32 - n));
}

} // namespace shroud
