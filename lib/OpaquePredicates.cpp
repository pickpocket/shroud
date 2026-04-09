#include "Shroud/OpaquePredicates.h"
#include <cmath>
#include <unordered_set>

namespace shroud {

// ============================================================================
// 3a: Number-Theoretic Invariant Predicates
// ============================================================================

std::vector<OpaquePredicate> createNumberTheoreticPredicates() {
    std::vector<OpaquePredicate> preds;

    // x*(x+1) % 2 == 0 — consecutive integers product is even
    preds.push_back({"consec_even", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) { return ((uint64_t)x * (x + 1)) % 2 == 0; },
        "x*(x+1) mod 2 == 0"});

    // x*x % 4 in {0, 1} — quadratic residues mod 4
    preds.push_back({"qr_mod4", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) {
            uint32_t r = (x * x) % 4;
            return r == 0 || r == 1;
        },
        "x^2 mod 4 in {0,1}"});

    // x^3 - x ≡ 0 (mod 3) — Fermat's Little Theorem
    preds.push_back({"fermat_mod3", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) {
            uint64_t x64 = x;
            return (x64 * x64 * x64 - x64) % 3 == 0;
        },
        "x^3 - x ≡ 0 (mod 3)"});

    // 7*y^2 - 1 != x^2 — quadratic non-residues mod 7
    preds.push_back({"qnr_mod7", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 2,
        [](uint32_t x, uint32_t y) {
            // 7*y^2 - 1 can never be a perfect square because
            // -1 is not a QR mod 7. We check mod 7 directly.
            (void)x; (void)y;
            return true; // always true by QNR argument (placeholder, real logic in override below)
        },
        "7*y^2 - 1 != x^2 (always true by QR theory)"});

    // Actually simplify the above: just return true since it's always true
    // But we need the evaluation to actually compute something non-trivial
    preds.back().evaluate = [](uint32_t x, uint32_t y) -> bool {
        // 7y^2 mod 7 = 0, so 7y^2 - 1 ≡ 6 (mod 7)
        // QRs mod 7 = {0,1,2,4}, and 6 is not among them
        (void)y;
        uint32_t rhs = ((x % 7u) * (x % 7u)) % 7u;
        return 6u != rhs; // always true since 6 is not a QR mod 7
    };

    // 3 | x*(x^2 + 2) — case analysis mod 3
    preds.push_back({"div3_cubic", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) {
            uint64_t x64 = x;
            return (x64 * (x64 * x64 + 2)) % 3 == 0;
        },
        "3 | x*(x^2 + 2)"});

    // x*(x+1)*(x+2) % 6 == 0 — product of 3 consecutive integers
    preds.push_back({"consec3_div6", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) {
            uint64_t x64 = x;
            return (x64 * (x64 + 1) * (x64 + 2)) % 6 == 0;
        },
        "x*(x+1)*(x+2) mod 6 == 0"});

    // (x^2 + x) & 1 == 0 — bitwise form: x(x+1) always even
    preds.push_back({"consec_even_bitwise", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) {
            return ((x * x + x) & 1u) == 0;
        },
        "(x^2 + x) & 1 == 0"});

    // x^4 % 8 in {0, 1} — fourth power residues mod 8
    preds.push_back({"pow4_mod8", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) {
            uint64_t x4 = (uint64_t)(x % 8) * (x % 8) % 8;
            x4 = x4 * x4 % 8; // Wait, that's (x^2)^2 which is wrong approach
            // Actually x^4 mod 8: x even -> x^4 mod 8 = 0, x odd -> x^4 mod 8 = 1
            uint32_t xm = x % 8;
            uint64_t p = 1;
            for (int i = 0; i < 4; i++) p = (p * xm) % 8;
            return p == 0 || p == 1;
        },
        "x^4 mod 8 in {0,1}"});

    // ((x ^ y) | (x & y)) == (x | y) — bitwise identity
    preds.push_back({"xor_and_or_identity", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 2,
        [](uint32_t x, uint32_t y) {
            return ((x ^ y) | (x & y)) == (x | y);
        },
        "((x ^ y) | (x & y)) == (x | y)"});

    // (x | (x-1)) + 1 is a power of 2 when x is a power of 2
    // More general: (x | ~x) == 0xFFFFFFFF — always true
    preds.push_back({"or_complement", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 1,
        [](uint32_t x, uint32_t) {
            return (x | ~x) == 0xFFFFFFFF;
        },
        "(x | ~x) == -1"});

    // (x + y) - (x ^ y) == 2*(x & y) — carry identity
    preds.push_back({"carry_identity", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 2,
        [](uint32_t x, uint32_t y) {
            return (x + y) - (x ^ y) == 2 * (x & y);
        },
        "(x + y) - (x ^ y) == 2*(x & y)"});

    // (x & y) + (x | y) == x + y — partition identity
    preds.push_back({"partition_identity", PredicateKind::AlwaysTrue, PredicateCategory::NumberTheoretic, 2,
        [](uint32_t x, uint32_t y) {
            return (x & y) + (x | y) == x + y;
        },
        "(x & y) + (x | y) == x + y"});

    return preds;
}

// ============================================================================
// 3b: Modular Arithmetic Predicates
// ============================================================================

std::vector<OpaquePredicate> createModularArithmeticPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // Generate quadratic with no roots mod p (discriminant is QNR)
    for (int i = 0; i < 3; i++) {
        int64_t p = ModArith::nextPrime(rng.nextInRange(7, 31));
        int64_t a = rng.nextInRange(1, (uint32_t)p - 1);
        int64_t b = rng.nextInRange(0, (uint32_t)p - 1);
        int64_t c;

        // Find c such that b^2 - 4ac is a QNR mod p
        do {
            c = rng.nextInRange(1, (uint32_t)p - 1);
        } while (ModArith::isQuadraticResidue(
            ModArith::mod(b * b - 4 * a * c, p), p));

        int64_t ca = a, cb = b, cc = c, cp = p;
        preds.push_back({
            "quad_no_root_" + std::to_string(i),
            PredicateKind::AlwaysTrue,
            PredicateCategory::ModularArithmetic, 1,
            [ca, cb, cc, cp](uint32_t x, uint32_t) -> bool {
                int64_t xm = ModArith::mod(x, cp);
                int64_t val = ModArith::mod(ca * xm * xm + cb * xm + cc, cp);
                return val != 0; // always true since no roots exist
            },
            "(" + std::to_string(a) + "*x^2 + " + std::to_string(b) +
            "*x + " + std::to_string(c) + ") mod " + std::to_string(p) + " != 0"
        });
    }

    // Euler's criterion derived: x^((p-1)/2) mod p in {0, 1, p-1}
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(11, 53));
        int64_t cp = p;
        preds.push_back({
            "euler_criterion",
            PredicateKind::AlwaysTrue,
            PredicateCategory::ModularArithmetic, 1,
            [cp](uint32_t x, uint32_t) -> bool {
                int64_t xm = ModArith::mod(x, cp);
                if (xm == 0) return true;
                int64_t result = ModArith::powMod(xm, (cp - 1) / 2, cp);
                return result == 1 || result == cp - 1;
            },
            "x^((p-1)/2) mod p in {0, 1, p-1}"
        });
    }

    // Wilson's theorem: (p-1)! ≡ -1 (mod p) for prime p
    // Predicate: precomputed (p-1)! + 1 ≡ 0 (mod p)
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(5, 13));
        int64_t factorial = 1;
        for (int64_t i = 2; i < p; i++)
            factorial = ModArith::mod(factorial * i, p);
        int64_t cp = p;
        int64_t cf = factorial;
        preds.push_back({
            "wilson_theorem",
            PredicateKind::AlwaysTrue,
            PredicateCategory::ModularArithmetic, 1,
            [cp, cf](uint32_t, uint32_t) -> bool {
                return ModArith::mod(cf + 1, cp) == 0;
            },
            "Wilson's theorem: (p-1)! + 1 ≡ 0 (mod p)"
        });
    }

    return preds;
}

// ============================================================================
// 3c: Bitwise & Machine-Level Predicates
// ============================================================================

std::vector<OpaquePredicate> createBitwisePredicates() {
    std::vector<OpaquePredicate> preds;

    // popcount(x & ~x) == 0
    preds.push_back({"popcount_zero", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 1,
        [](uint32_t x, uint32_t) { return popcount32(x & ~x) == 0; },
        "popcount(x & ~x) == 0"});

    // (x ^ x) == 0
    preds.push_back({"xor_self", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 1,
        [](uint32_t x, uint32_t) { return (x ^ x) == 0; },
        "(x ^ x) == 0"});

    // CLZ(1) < 32
    preds.push_back({"clz_one", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 1,
        [](uint32_t, uint32_t) { return clz32(1) < 32; },
        "CLZ(1) < 32"});

    // Rotate round-trip: (x ror n) rol n == x
    preds.push_back({"rotate_roundtrip", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 2,
        [](uint32_t x, uint32_t y) {
            unsigned n = y & 31;
            return rotl32(rotr32(x, n), n) == x;
        },
        "(x ror n) rol n == x"});

    // De Morgan: ~(x & y) == (~x | ~y)
    preds.push_back({"demorgan_and", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 2,
        [](uint32_t x, uint32_t y) { return ~(x & y) == (~x | ~y); },
        "~(x & y) == (~x | ~y)"});

    // De Morgan: ~(x | y) == (~x & ~y)
    preds.push_back({"demorgan_or", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 2,
        [](uint32_t x, uint32_t y) { return ~(x | y) == (~x & ~y); },
        "~(x | y) == (~x & ~y)"});

    // x & (x - 1) has one fewer set bit than x (or equals 0 if x is power of 2)
    // popcount(x & (x-1)) < popcount(x) || x == 0
    preds.push_back({"clear_lowest_bit", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 1,
        [](uint32_t x, uint32_t) {
            if (x == 0) return true;
            return popcount32(x & (x - 1)) < popcount32(x);
        },
        "popcount(x & (x-1)) < popcount(x) for x != 0"});

    // (x & -x) is always a power of 2 (or 0)
    preds.push_back({"isolate_lowest", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 1,
        [](uint32_t x, uint32_t) {
            uint32_t isolated = x & (uint32_t)(-(int32_t)x);
            return isolated == 0 || popcount32(isolated) == 1;
        },
        "(x & -x) is power of 2 or 0"});

    // Two's complement: -x == ~x + 1
    preds.push_back({"twos_complement", PredicateKind::AlwaysTrue, PredicateCategory::Bitwise, 1,
        [](uint32_t x, uint32_t) {
            return (uint32_t)(-(int32_t)x) == (~x + 1);
        },
        "-x == ~x + 1"});

    return preds;
}

// ============================================================================
// 3d: Floating-Point Predicates
// ============================================================================

std::vector<OpaquePredicate> createFloatingPointPredicates() {
    std::vector<OpaquePredicate> preds;

    // floor(x) <= x for any non-NaN
    preds.push_back({"floor_le", PredicateKind::AlwaysTrue, PredicateCategory::FloatingPoint, 1,
        [](uint32_t x, uint32_t) {
            float f;
            // Use non-NaN float
            uint32_t bits = (x & 0x7F7FFFFF) | 0x00800000; // normalized, non-NaN
            memcpy(&f, &bits, 4);
            return std::floor(f) <= f;
        },
        "floor(x) <= x"});

    // abs(x) >= 0 for any non-NaN
    preds.push_back({"abs_nonneg", PredicateKind::AlwaysTrue, PredicateCategory::FloatingPoint, 1,
        [](uint32_t x, uint32_t) {
            float f;
            uint32_t bits = (x & 0x7F7FFFFF) | 0x00800000;
            memcpy(&f, &bits, 4);
            return std::fabs(f) >= 0.0f;
        },
        "abs(x) >= 0"});

    return preds;
}

// ============================================================================
// 3e: Pointer/Memory-Based Predicates
// ============================================================================

std::vector<OpaquePredicate> createPointerPredicates() {
    std::vector<OpaquePredicate> preds;

    // Stack address relationships: two stack-allocated variables are always
    // at different addresses
    preds.push_back({"stack_diff_addr", PredicateKind::AlwaysTrue, PredicateCategory::Pointer, 1,
        [](uint32_t, uint32_t) {
            volatile int a = 1, b = 2;
            return &a != &b;
        },
        "&local1 != &local2"});

    // Disjoint linked list nodes
    preds.push_back({"disjoint_alloc", PredicateKind::AlwaysTrue, PredicateCategory::Pointer, 1,
        [](uint32_t, uint32_t) {
            int a, b; // two separate stack objects
            uintptr_t pa = (uintptr_t)&a;
            uintptr_t pb = (uintptr_t)&b;
            return pa != pb;
        },
        "Different allocations have different addresses"});

    return preds;
}

// ============================================================================
// 3f: Dynamic Opaque Predicates (unique per compilation)
// ============================================================================

std::vector<OpaquePredicate> createDynamicPredicates(ObfRNG& rng, int count) {
    std::vector<OpaquePredicate> preds;

    for (int i = 0; i < count; i++) {
        uint32_t kind = rng.nextInRange(0, 3);

        if (kind == 0) {
            // Parameterized polynomial: (a*x^2 + b*x + c) mod p with known residue
            int64_t p = ModArith::nextPrime(rng.nextInRange(13, 97));
            int64_t a = rng.nextInRange(1, (uint32_t)p - 1);
            int64_t b = rng.nextInRange(0, (uint32_t)p - 1);
            // Pick c to make specific residue class
            int64_t target_residue = rng.nextInRange(0, (uint32_t)p - 1);
            // We want: for ALL x, (a*x^2 + b*x + c) mod p != target_residue
            // This means target_residue - c must make the discriminant a QNR
            int64_t c;
            bool found = false;
            for (int64_t cc = 0; cc < p && !found; cc++) {
                int64_t disc = ModArith::mod(b * b - 4 * a * (cc - target_residue), p);
                if (!ModArith::isQuadraticResidue(disc, p) && disc != 0) {
                    c = cc;
                    found = true;
                }
            }
            if (!found) continue;

            int64_t ca = a, cb = b, cc_val = c, cp = p, ct = target_residue;
            preds.push_back({
                "dyn_poly_" + std::to_string(i),
                PredicateKind::AlwaysTrue,
                PredicateCategory::Dynamic, 1,
                [ca, cb, cc_val, cp, ct](uint32_t x, uint32_t) -> bool {
                    int64_t xm = ModArith::mod(x, cp);
                    int64_t val = ModArith::mod(ca * xm * xm + cb * xm + cc_val, cp);
                    return val != ct;
                },
                "dynamic polynomial predicate"
            });

        } else if (kind == 1) {
            // Random linear congruence with known property
            int64_t m = ModArith::nextPrime(rng.nextInRange(7, 61));
            int64_t a = rng.nextInRange(1, (uint32_t)m - 1);
            int64_t b = rng.nextInRange(0, (uint32_t)m - 1);
            // (a*x + b) mod m: range is always 0..m-1
            // Predicate: result < m (always true!)
            int64_t cm = m, ca = a, cb = b;
            preds.push_back({
                "dyn_linear_" + std::to_string(i),
                PredicateKind::AlwaysTrue,
                PredicateCategory::Dynamic, 1,
                [ca, cb, cm](uint32_t x, uint32_t) -> bool {
                    int64_t val = ModArith::mod(ca * (int64_t)x + cb, cm);
                    return val < cm; // always true
                },
                "dynamic linear congruence"
            });

        } else if (kind == 2) {
            // Hash-based: CRC32(x ^ salt) & mask has known property
            uint32_t salt = rng.next32();
            (void)rng.nextInRange(1, 4); // consume RNG state for consistency
            // Precompute for a specific x value... but we want always-true
            // Instead: CRC32 of any value is always < 2^32 (trivially true)
            // Better: CRC32(x) ^ CRC32(x) == 0
            uint32_t cs = salt;
            preds.push_back({
                "dyn_hash_" + std::to_string(i),
                PredicateKind::AlwaysTrue,
                PredicateCategory::Dynamic, 1,
                [cs](uint32_t x, uint32_t) -> bool {
                    uint32_t val = x ^ cs;
                    uint8_t buf[4];
                    buf[0] = val & 0xFF;
                    buf[1] = (val >> 8) & 0xFF;
                    buf[2] = (val >> 16) & 0xFF;
                    buf[3] = (val >> 24) & 0xFF;
                    uint32_t h1 = crc32(buf, 4);
                    uint32_t h2 = crc32(buf, 4);
                    return h1 == h2; // same input -> same hash (always true)
                },
                "dynamic hash consistency"
            });

        } else {
            // Composite: AND of two simple predicates
            // x*(x+1) % 2 == 0 AND (x^x) == 0
            preds.push_back({
                "dyn_composite_" + std::to_string(i),
                PredicateKind::AlwaysTrue,
                PredicateCategory::Dynamic, 1,
                [](uint32_t x, uint32_t) -> bool {
                    bool p1 = ((uint64_t)x * (x + 1)) % 2 == 0;
                    bool p2 = (x ^ x) == 0;
                    return p1 && p2;
                },
                "composite: consec_even AND xor_self"
            });
        }
    }

    return preds;
}

// ============================================================================
// 3g: Graph-Theoretic Predicates
// ============================================================================

std::vector<OpaquePredicate> createGraphTheoreticPredicates(ObfRNG& /*rng*/) {
    std::vector<OpaquePredicate> preds;

    // 3-colorable graph: embed a known 3-colorable graph (bipartite + one extra)
    // Check adjacency invariant that holds for any valid coloring
    {
        // Small graph: 6 nodes, known 3-coloring
        // Adjacency: 0-1, 1-2, 2-3, 3-4, 4-5, 5-0 (hexagon)
        // Coloring: 0->R, 1->G, 2->R, 3->G, 4->R, 5->G
        // Property: no two adjacent nodes have same color → chromatic number = 2 ≤ 3
        std::array<int, 6> coloring = {0, 1, 0, 1, 0, 1};
        std::array<std::pair<int,int>, 6> edges = {{{0,1},{1,2},{2,3},{3,4},{4,5},{5,0}}};

        auto cc = coloring;
        auto ce = edges;
        preds.push_back({
            "graph_3color",
            PredicateKind::AlwaysTrue,
            PredicateCategory::GraphTheoretic, 1,
            [cc, ce](uint32_t, uint32_t) -> bool {
                for (auto& [u, v] : ce) {
                    if (cc[u] == cc[v]) return false;
                }
                return true; // valid coloring exists
            },
            "3-colorability: embedded valid coloring"
        });
    }

    // Clique predicate: known 3-clique in a graph
    {
        // 5-node graph with known triangle {0,1,2}
        // Edges: 0-1, 0-2, 1-2, 2-3, 3-4
        preds.push_back({
            "graph_clique",
            PredicateKind::AlwaysTrue,
            PredicateCategory::GraphTheoretic, 1,
            [](uint32_t, uint32_t) -> bool {
                // Check that nodes 0,1,2 form a clique
                bool adj[5][5] = {};
                adj[0][1] = adj[1][0] = true;
                adj[0][2] = adj[2][0] = true;
                adj[1][2] = adj[2][1] = true;
                adj[2][3] = adj[3][2] = true;
                adj[3][4] = adj[4][3] = true;
                return adj[0][1] && adj[0][2] && adj[1][2]; // 3-clique exists
            },
            "known 3-clique in embedded graph"
        });
    }

    return preds;
}

// ============================================================================
// 3h: Lattice-Based Predicates (post-quantum)
// ============================================================================

std::vector<OpaquePredicate> createLatticeBasedPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // SIS-style: A*s = t (mod q) for known short vector s
    // We use small dimensions for efficiency in the predicate
    {
        const int n = 4, m = 6;
        int64_t q = ModArith::nextPrime(rng.nextInRange(97, 251));

        // Generate random matrix A (n x m)
        std::array<std::array<int64_t, 6>, 4> A;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                A[i][j] = rng.nextInRange(0, (uint32_t)q - 1);

        // Short secret vector s (entries in {-1, 0, 1})
        std::array<int64_t, 6> s;
        for (int j = 0; j < m; j++)
            s[j] = (int64_t)rng.nextInRange(0, 2) - 1;

        // Compute t = A*s mod q
        std::array<int64_t, 4> t;
        for (int i = 0; i < n; i++) {
            t[i] = 0;
            for (int j = 0; j < m; j++)
                t[i] = ModArith::mod(t[i] + A[i][j] * s[j], q);
        }

        auto cA = A; auto cs = s; auto ct = t;
        int64_t cq = q;
        preds.push_back({
            "lattice_sis",
            PredicateKind::AlwaysTrue,
            PredicateCategory::LatticeBased, 1,
            [cA, cs, ct, cq](uint32_t, uint32_t) -> bool {
                // Verify A*s ≡ t (mod q)
                for (int i = 0; i < 4; i++) {
                    int64_t sum = 0;
                    for (int j = 0; j < 6; j++)
                        sum = ModArith::mod(sum + cA[i][j] * cs[j], cq);
                    if (sum != ct[i]) return false;
                }
                return true;
            },
            "lattice SIS: A*s = t (mod q)"
        });
    }

    // LWE-style: b = A*s + e (mod q), check ||b - A*s|| is small
    {
        const int n = 4;
        int64_t q = ModArith::nextPrime(rng.nextInRange(97, 251));

        std::array<int64_t, 4> a;
        for (int i = 0; i < n; i++)
            a[i] = rng.nextInRange(0, (uint32_t)q - 1);

        int64_t s = rng.nextInRange(0, (uint32_t)q - 1);

        // b_i = a_i * s + e_i (mod q) where e_i is small
        std::array<int64_t, 4> b;
        std::array<int64_t, 4> e;
        for (int i = 0; i < n; i++) {
            e[i] = (int64_t)rng.nextInRange(0, 4) - 2; // noise in [-2, 2]
            b[i] = ModArith::mod(a[i] * s + e[i], q);
        }

        auto ca = a; auto cb = b; auto ce = e;
        int64_t cs_val = s, cq = q;
        preds.push_back({
            "lattice_lwe",
            PredicateKind::AlwaysTrue,
            PredicateCategory::LatticeBased, 1,
            [ca, cb, cs_val, ce, cq](uint32_t, uint32_t) -> bool {
                for (int i = 0; i < 4; i++) {
                    int64_t expected = ModArith::mod(ca[i] * cs_val + ce[i], cq);
                    if (expected != cb[i]) return false;
                }
                return true;
            },
            "lattice LWE: b = a*s + e (mod q)"
        });
    }

    return preds;
}

// ============================================================================
// 3i: Multivariate Quadratic Predicates (post-quantum)
// ============================================================================

std::vector<OpaquePredicate> createMQPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // Small MQ system over GF(2): 4 variables, 3 equations, known solution
    {
        // Solution s = (s0, s1, s2, s3) in {0, 1}^4
        std::array<int, 4> sol;
        for (int i = 0; i < 4; i++)
            sol[i] = rng.nextInRange(0, 1);

        // Generate random quadratic polynomials that vanish at sol
        // P(x) = sum_{i<=j} a_{ij} * x_i * x_j + sum_i b_i * x_i + c
        // We set c = P(sol) XOR'd to make P(sol) = 0

        struct QuadPoly {
            std::array<std::array<int, 4>, 4> quad; // upper triangular
            std::array<int, 4> linear;
            int constant;

            int eval(const std::array<int, 4>& x) const {
                int result = constant;
                for (int i = 0; i < 4; i++) {
                    result ^= (linear[i] & x[i]);
                    for (int j = i; j < 4; j++)
                        result ^= (quad[i][j] & x[i] & x[j]);
                }
                return result;
            }
        };

        std::array<QuadPoly, 3> polys;
        for (int p = 0; p < 3; p++) {
            for (int i = 0; i < 4; i++) {
                polys[p].linear[i] = rng.nextInRange(0, 1);
                for (int j = i; j < 4; j++)
                    polys[p].quad[i][j] = rng.nextInRange(0, 1);
                for (int j = 0; j < i; j++)
                    polys[p].quad[i][j] = 0;
            }
            // Set constant so P(sol) = 0
            polys[p].constant = 0;
            int val = polys[p].eval(sol);
            polys[p].constant = val; // XOR: val ^ val = 0
        }

        auto cp = polys;
        auto cs = sol;
        preds.push_back({
            "mq_system",
            PredicateKind::AlwaysTrue,
            PredicateCategory::MultivariateQuadratic, 1,
            [cp, cs](uint32_t, uint32_t) -> bool {
                for (int i = 0; i < 3; i++) {
                    if (cp[i].eval(cs) != 0) return false;
                }
                return true;
            },
            "MQ system: P_i(s) = 0 for known solution s"
        });
    }

    return preds;
}

// ============================================================================
// 3j: Expander Graph Predicates
// ============================================================================

std::vector<OpaquePredicate> createExpanderGraphPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // Expander: random walk on a complete graph converges
    // After k steps on K_n, every vertex is reachable
    {
        uint32_t seed = rng.next32();

        preds.push_back({
            "expander_walk",
            PredicateKind::AlwaysTrue,
            PredicateCategory::ExpanderGraph, 1,
            [seed](uint32_t x, uint32_t) -> bool {
                const int n = 8, steps = 10;
                // Random walk on K_n from vertex (x % n)
                // After enough steps, can reach any vertex
                // For complete graph, 1 step suffices, but we simulate more
                uint32_t state = x % n;
                uint32_t rng_state = seed ^ x;
                for (int i = 0; i < steps; i++) {
                    rng_state = rng_state * 1103515245 + 12345;
                    state = rng_state % n;
                }
                return state < (uint32_t)n; // trivially true — walk stays in graph
            },
            "expander graph random walk stays in graph"
        });
    }

    return preds;
}

// ============================================================================
// 3k: Cellular Automaton / Chaotic Map Predicates
// ============================================================================

std::vector<OpaquePredicate> createCellularAutomatonPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // Rule 30 cellular automaton: known initial state → known state at step N
    {
        uint32_t initial = rng.next32();
        const int steps = 10;

        // Precompute Rule 30 for 'steps' iterations
        uint32_t state = initial;
        for (int i = 0; i < steps; i++) {
            uint32_t left = (state << 1) | (state >> 31);
            uint32_t right = (state >> 1) | (state << 31);
            state = left ^ (state | right); // Rule 30
        }
        uint32_t expected = state;

        uint32_t ci = initial;
        uint32_t ce = expected;
        preds.push_back({
            "rule30_ca",
            PredicateKind::AlwaysTrue,
            PredicateCategory::CellularAutomaton, 1,
            [ci, ce](uint32_t, uint32_t) -> bool {
                const int steps = 10;
                uint32_t state = ci;
                for (int i = 0; i < steps; i++) {
                    uint32_t left = (state << 1) | (state >> 31);
                    uint32_t right = (state >> 1) | (state << 31);
                    state = left ^ (state | right);
                }
                return state == ce;
            },
            "Rule 30 CA: IC -> known state after N steps"
        });
    }

    // Logistic map: discrete version
    // x_{n+1} = r * x_n * (M - x_n) / M  (integer version)
    {
        const uint32_t M = 1000;
        const uint32_t r = 3; // integer r (in fixed-point: r/1 * x * (M-x) / M)
        uint32_t x0 = rng.nextInRange(100, 900);
        const int steps = 5;

        uint32_t state = x0;
        for (int i = 0; i < steps; i++) {
            state = (r * state * (M - state)) / M;
            state = state % (M + 1);
        }
        uint32_t expected = state;

        uint32_t cx = x0;
        uint32_t ce = expected;
        preds.push_back({
            "logistic_map",
            PredicateKind::AlwaysTrue,
            PredicateCategory::CellularAutomaton, 1,
            [cx, ce](uint32_t, uint32_t) -> bool {
                const uint32_t M = 1000, r = 3;
                const int steps = 5;
                uint32_t state = cx;
                for (int i = 0; i < steps; i++) {
                    state = (r * state * (M - state)) / M;
                    state = state % (M + 1);
                }
                return state == ce;
            },
            "Logistic map from known IC converges to known state"
        });
    }

    return preds;
}

// ============================================================================
// 3l: Cryptographic Hash Predicates
// ============================================================================

std::vector<OpaquePredicate> createCryptoHashPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // CRC32 consistency: hash of same input always produces same output
    {
        uint32_t salt = rng.next32();
        preds.push_back({
            "hash_consistency",
            PredicateKind::AlwaysTrue,
            PredicateCategory::CryptographicHash, 1,
            [salt](uint32_t x, uint32_t) -> bool {
                uint32_t val = x ^ salt;
                uint8_t buf[4] = {
                    (uint8_t)(val & 0xFF),
                    (uint8_t)((val >> 8) & 0xFF),
                    (uint8_t)((val >> 16) & 0xFF),
                    (uint8_t)((val >> 24) & 0xFF)
                };
                return crc32(buf, 4) == crc32(buf, 4);
            },
            "CRC32(x ^ salt) computed twice gives same result"
        });
    }

    // Precomputed hash: H(known_value) == expected
    {
        uint32_t known = rng.next32();
        uint8_t buf[4] = {
            (uint8_t)(known & 0xFF),
            (uint8_t)((known >> 8) & 0xFF),
            (uint8_t)((known >> 16) & 0xFF),
            (uint8_t)((known >> 24) & 0xFF)
        };
        uint32_t expected_hash = crc32(buf, 4);

        uint32_t ck = known, ch = expected_hash;
        preds.push_back({
            "hash_precomputed",
            PredicateKind::AlwaysTrue,
            PredicateCategory::CryptographicHash, 1,
            [ck, ch](uint32_t, uint32_t) -> bool {
                uint8_t buf[4] = {
                    (uint8_t)(ck & 0xFF),
                    (uint8_t)((ck >> 8) & 0xFF),
                    (uint8_t)((ck >> 16) & 0xFF),
                    (uint8_t)((ck >> 24) & 0xFF)
                };
                return crc32(buf, 4) == ch;
            },
            "H(known_value) == precomputed_hash"
        });
    }

    return preds;
}

// ============================================================================
// 3m: Error-Correcting Code Predicates (post-quantum)
// ============================================================================

std::vector<OpaquePredicate> createECCPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // Syndrome decoding: H*c = s for known codeword c
    // Simple parity check matrix (Hamming-like)
    {
        // [7,4] Hamming code
        // Parity check matrix H (3x7) for Hamming(7,4)
        std::array<std::array<int, 7>, 3> H = {{
            {1, 1, 1, 0, 1, 0, 0},
            {1, 1, 0, 1, 0, 1, 0},
            {1, 0, 1, 1, 0, 0, 1}
        }};

        // Generate random codeword (in null space of H over GF(2))
        // A codeword c satisfies H*c = 0
        // We'll use a systematic encoding: pick 4 data bits, compute 3 parity bits
        std::array<int, 7> codeword;
        codeword[0] = rng.nextInRange(0, 1);
        codeword[1] = rng.nextInRange(0, 1);
        codeword[2] = rng.nextInRange(0, 1);
        codeword[3] = rng.nextInRange(0, 1);
        // Parity bits to satisfy H*c = 0 mod 2
        codeword[4] = (codeword[0] ^ codeword[1] ^ codeword[2]) & 1;
        codeword[5] = (codeword[0] ^ codeword[1] ^ codeword[3]) & 1;
        codeword[6] = (codeword[0] ^ codeword[2] ^ codeword[3]) & 1;

        auto cH = H;
        auto cc = codeword;
        preds.push_back({
            "ecc_syndrome",
            PredicateKind::AlwaysTrue,
            PredicateCategory::ErrorCorrectingCode, 1,
            [cH, cc](uint32_t, uint32_t) -> bool {
                // Check H*c = 0 (mod 2)
                for (int i = 0; i < 3; i++) {
                    int sum = 0;
                    for (int j = 0; j < 7; j++)
                        sum ^= (cH[i][j] & cc[j]);
                    if (sum != 0) return false;
                }
                return true;
            },
            "Hamming(7,4) syndrome: H*c = 0"
        });
    }

    return preds;
}

// ============================================================================
// 3n: Undecidable/Intractable Predicates
// ============================================================================

std::vector<OpaquePredicate> createUndecidablePredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // Collatz: precomputed steps for a known starting value
    {
        uint32_t start = rng.nextInRange(100, 10000);
        uint32_t n = start;
        int steps = 0;
        while (n != 1 && steps < 1000) {
            n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
            steps++;
        }
        int expected_steps = steps;
        uint32_t cs = start;
        int ce = expected_steps;

        preds.push_back({
            "collatz_steps",
            PredicateKind::AlwaysTrue,
            PredicateCategory::Undecidable, 1,
            [cs, ce](uint32_t, uint32_t) -> bool {
                uint32_t n = cs;
                int steps = 0;
                while (n != 1 && steps < 1000) {
                    n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
                    steps++;
                }
                return steps == ce;
            },
            "Collatz sequence from known start has known length"
        });
    }

    // Diophantine: known solution to x^2 + y^2 = z^2 (Pythagorean triple)
    {
        // Generate a Pythagorean triple using m, n with m > n > 0
        uint32_t m = rng.nextInRange(2, 20);
        uint32_t n = rng.nextInRange(1, m - 1);
        uint32_t a = m * m - n * n;
        uint32_t b = 2 * m * n;
        uint32_t c = m * m + n * n;

        uint32_t ca = a, cb = b, cc_val = c;
        preds.push_back({
            "pythagorean_triple",
            PredicateKind::AlwaysTrue,
            PredicateCategory::Undecidable, 1,
            [ca, cb, cc_val](uint32_t, uint32_t) -> bool {
                return ca * ca + cb * cb == cc_val * cc_val;
            },
            "Known Pythagorean triple: a^2 + b^2 = c^2"
        });
    }

    return preds;
}

// ============================================================================
// 3o: Composite predicates
// ============================================================================

OpaquePredicate composePredicates(const OpaquePredicate& a, const OpaquePredicate& b,
                                   ObfRNG& rng) {
    int op = rng.nextInRange(0, 2); // AND, OR, XOR
    std::string opName;
    std::function<bool(bool, bool)> combine;

    if (op == 0) {
        opName = "AND";
        combine = [](bool x, bool y) { return x && y; };
    } else if (op == 1) {
        opName = "OR";
        combine = [](bool x, bool y) { return x || y; };
    } else {
        // XNOR — both true → true
        opName = "XNOR";
        combine = [](bool x, bool y) { return x == y; };
    }

    auto evalA = a.evaluate;
    auto evalB = b.evaluate;
    auto comb = combine;

    PredicateKind kind;
    if (a.kind == PredicateKind::AlwaysTrue && b.kind == PredicateKind::AlwaysTrue) {
        kind = PredicateKind::AlwaysTrue; // AND/OR/XNOR of true,true = true
    } else {
        kind = PredicateKind::TwoWay;
    }

    return {
        "composite_" + a.name + "_" + opName + "_" + b.name,
        kind,
        PredicateCategory::Dynamic,
        std::max(a.numInputs, b.numInputs),
        [evalA, evalB, comb](uint32_t x, uint32_t y) -> bool {
            return comb(evalA(x, y), evalB(x, y));
        },
        a.description + " " + opName + " " + b.description
    };
}

// ============================================================================
// 3p: Novel Predicates from Diverse Mathematical/CS Domains
// ============================================================================

std::vector<OpaquePredicate> createNovelPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> preds;

    // -----------------------------------------------------------------------
    // 1. ALGEBRAIC GEOMETRY: Elliptic curve point on curve
    // y^2 = x^3 + ax + b (mod p). Precompute a point P=(px,py) on the curve
    // by picking x, computing y. Predicate verifies py^2 == px^3 + a*px + b.
    // -----------------------------------------------------------------------
    {
        // Use a small prime for the field
        int64_t p = ModArith::nextPrime(rng.nextInRange(97, 251));
        int64_t a = rng.nextInRange(1, (uint32_t)p - 1);
        int64_t b = rng.nextInRange(1, (uint32_t)p - 1);

        // Ensure 4a^3 + 27b^2 != 0 mod p (non-singular curve)
        while (ModArith::mod(4 * a * a * a + 27 * b * b, p) == 0) {
            b = rng.nextInRange(1, (uint32_t)p - 1);
        }

        // Find a point on the curve: try random x until x^3+ax+b is a QR
        int64_t px = 0, py = 0;
        for (int64_t trial = 0; trial < p; trial++) {
            int64_t x = (rng.nextInRange(0, (uint32_t)p - 1) + trial) % p;
            int64_t rhs = ModArith::mod(x * x % p * x % p + a * x + b, p);
            if (rhs == 0) {
                px = x; py = 0; break;
            }
            if (ModArith::isQuadraticResidue(rhs, p)) {
                px = x;
                // Compute square root mod p using Tonelli-Shanks simplified
                // For p=3 mod 4: sqrt = rhs^((p+1)/4) mod p
                // For general p: use powMod with (p+1)/4 if p%4==3
                if (p % 4 == 3) {
                    py = ModArith::powMod(rhs, (p + 1) / 4, p);
                } else {
                    // Brute force for small p
                    for (int64_t y = 0; y < p; y++) {
                        if (ModArith::mod(y * y, p) == rhs) { py = y; break; }
                    }
                }
                break;
            }
        }

        int64_t ca = a, cb = b, cp = p, cpx = px, cpy = py;
        preds.push_back({
            "ec_point_on_curve",
            PredicateKind::AlwaysTrue,
            PredicateCategory::AlgebraicGeometry, 1,
            [ca, cb, cp, cpx, cpy](uint32_t, uint32_t) -> bool {
                // Verify py^2 == px^3 + a*px + b (mod p)
                int64_t lhs = ModArith::mod(cpy * cpy, cp);
                int64_t rhs = ModArith::mod(
                    ModArith::mod(cpx * cpx % cp * cpx, cp) +
                    ModArith::mod(ca * cpx, cp) + cb, cp);
                return lhs == rhs;
            },
            "Elliptic curve: P=(px,py) on y^2=x^3+ax+b mod p"
        });
    }

    // -----------------------------------------------------------------------
    // 1b. ALGEBRAIC GEOMETRY: Weil pairing order property
    // For point P of order n on curve, n*P = O (point at infinity).
    // Simulate via scalar mult: verify k*n*G wraps to identity.
    // Precompute using the group order.
    // -----------------------------------------------------------------------
    {
        // Use the fact that in a cyclic group of order N, g^N = identity
        // Pick a small multiplicative group Z/pZ* with order p-1
        int64_t p = ModArith::nextPrime(rng.nextInRange(97, 251));
        int64_t g = rng.nextInRange(2, (uint32_t)p - 1);
        int64_t order = p - 1; // order of Z/pZ*

        // g^(p-1) = 1 mod p by Fermat's little theorem
        int64_t cg = g, cp = p, cord = order;
        preds.push_back({
            "weil_pairing_order",
            PredicateKind::AlwaysTrue,
            PredicateCategory::AlgebraicGeometry, 1,
            [cg, cp, cord](uint32_t x, uint32_t) -> bool {
                // g^(order * (x%16 + 1)) mod p == 1
                int64_t exp = cord * ((x % 16) + 1);
                return ModArith::powMod(cg, exp, cp) == 1;
            },
            "Weil pairing: g^(n*k) = 1 in cyclic group of order n"
        });
    }

    // -----------------------------------------------------------------------
    // 2. REPRESENTATION THEORY: Matrix group order A^n = I
    // For a 2x2 matrix A over Z/mZ of known order n, verify A^n = I.
    // -----------------------------------------------------------------------
    {
        int64_t m = ModArith::nextPrime(rng.nextInRange(97, 251));

        // Use a rotation-like matrix of known order.
        // For a 2x2 matrix [[0, -1], [1, 0]] over Z/pZ, order divides 4.
        // A^4 = I for this matrix.
        // Randomize by conjugation: B = S*A*S^{-1} has same order.
        int64_t s00 = rng.nextInRange(1, (uint32_t)m - 1);
        int64_t s01 = rng.nextInRange(0, (uint32_t)m - 1);
        int64_t s10 = rng.nextInRange(0, (uint32_t)m - 1);
        int64_t s11;
        // Ensure S is invertible: det(S) = s00*s11 - s01*s10 != 0 mod m
        do {
            s11 = rng.nextInRange(1, (uint32_t)m - 1);
        } while (ModArith::mod(s00 * s11 - s01 * s10, m) == 0);

        int64_t det = ModArith::mod(s00 * s11 - s01 * s10, m);
        int64_t detInv = ModArith::modInverse(det, m);

        // S^{-1} = (1/det) * [[s11, -s01], [-s10, s00]]
        int64_t si00 = ModArith::mod(s11 * detInv, m);
        int64_t si01 = ModArith::mod(-s01 * detInv, m);
        int64_t si10 = ModArith::mod(-s10 * detInv, m);
        int64_t si11 = ModArith::mod(s00 * detInv, m);

        // A = [[0, m-1], [1, 0]] (rotation matrix, order 4)
        // B = S * A * S^{-1}
        // S*A: row0 = [s00*0+s01*1, s00*(m-1)+s01*0] = [s01, s00*(m-1)]
        //      row1 = [s10*0+s11*1, s10*(m-1)+s11*0] = [s11, s10*(m-1)]
        int64_t sa00 = ModArith::mod(s01, m);
        int64_t sa01 = ModArith::mod(s00 * (m - 1), m);
        int64_t sa10 = ModArith::mod(s11, m);
        int64_t sa11 = ModArith::mod(s10 * (m - 1), m);

        // B = (S*A) * S^{-1}
        int64_t b00 = ModArith::mod(sa00 * si00 + sa01 * si10, m);
        int64_t b01 = ModArith::mod(sa00 * si01 + sa01 * si11, m);
        int64_t b10 = ModArith::mod(sa10 * si00 + sa11 * si10, m);
        int64_t b11 = ModArith::mod(sa10 * si01 + sa11 * si11, m);

        int64_t cm = m;
        int64_t cb00 = b00, cb01 = b01, cb10 = b10, cb11 = b11;

        preds.push_back({
            "matrix_group_order",
            PredicateKind::AlwaysTrue,
            PredicateCategory::RepresentationTheory, 2,
            [cb00, cb01, cb10, cb11, cm](uint32_t, uint32_t) -> bool {
                // Compute B^4 mod m, should be identity
                // Start with I
                int64_t r00 = 1, r01 = 0, r10 = 0, r11 = 1;
                for (int i = 0; i < 4; i++) {
                    int64_t t00 = ModArith::mod(r00 * cb00 + r01 * cb10, cm);
                    int64_t t01 = ModArith::mod(r00 * cb01 + r01 * cb11, cm);
                    int64_t t10 = ModArith::mod(r10 * cb00 + r11 * cb10, cm);
                    int64_t t11 = ModArith::mod(r10 * cb01 + r11 * cb11, cm);
                    r00 = t00; r01 = t01; r10 = t10; r11 = t11;
                }
                return r00 == 1 && r01 == 0 && r10 == 0 && r11 == 1;
            },
            "Matrix A of order 4 over Z/mZ: A^4 = I"
        });
    }

    // -----------------------------------------------------------------------
    // 2b. REPRESENTATION THEORY: Trace identity tr(AB) = tr(BA)
    // -----------------------------------------------------------------------
    {
        int64_t m = ModArith::nextPrime(rng.nextInRange(53, 127));
        // Precompute two random 2x2 matrices
        int64_t a00 = rng.nextInRange(0, (uint32_t)m-1), a01 = rng.nextInRange(0, (uint32_t)m-1);
        int64_t a10 = rng.nextInRange(0, (uint32_t)m-1), a11 = rng.nextInRange(0, (uint32_t)m-1);
        int64_t b00 = rng.nextInRange(0, (uint32_t)m-1), b01 = rng.nextInRange(0, (uint32_t)m-1);
        int64_t b10 = rng.nextInRange(0, (uint32_t)m-1), b11 = rng.nextInRange(0, (uint32_t)m-1);

        int64_t cm = m;
        int64_t ca00=a00, ca01=a01, ca10=a10, ca11=a11;
        int64_t ccb00=b00, ccb01=b01, ccb10=b10, ccb11=b11;

        preds.push_back({
            "trace_identity",
            PredicateKind::AlwaysTrue,
            PredicateCategory::RepresentationTheory, 1,
            [ca00,ca01,ca10,ca11,ccb00,ccb01,ccb10,ccb11,cm](uint32_t x, uint32_t) -> bool {
                // Perturb matrices with x to add input dependence but preserve the identity
                int64_t xm = x % (uint32_t)cm;
                int64_t ma00 = ModArith::mod(ca00 + xm, cm), ma01 = ca01;
                int64_t ma10 = ca10, ma11 = ModArith::mod(ca11 + xm, cm);
                // tr(A*B)
                int64_t trAB = ModArith::mod(
                    ModArith::mod(ma00*ccb00 + ma01*ccb10, cm) +
                    ModArith::mod(ma10*ccb01 + ma11*ccb11, cm), cm);
                // tr(B*A)
                int64_t trBA = ModArith::mod(
                    ModArith::mod(ccb00*ma00 + ccb01*ma10, cm) +
                    ModArith::mod(ccb10*ma01 + ccb11*ma11, cm), cm);
                return trAB == trBA;
            },
            "tr(A*B) = tr(B*A) for 2x2 matrices over Z/mZ"
        });
    }

    // -----------------------------------------------------------------------
    // 3. COMBINATORIAL: Catalan number property
    // C(n) = (2n)! / ((n+1)! * n!). Verify C(n) * (n+1) == C(2n, n).
    // -----------------------------------------------------------------------
    {
        // Pick a small n to avoid overflow
        uint32_t n = rng.nextInRange(3, 10);

        // Compute C(2n, n) = (2n)! / (n! * n!)
        auto factorial = [](uint32_t k) -> uint64_t {
            uint64_t r = 1;
            for (uint32_t i = 2; i <= k; i++) r *= i;
            return r;
        };
        auto binomial = [&factorial](uint32_t nn, uint32_t kk) -> uint64_t {
            return factorial(nn) / (factorial(kk) * factorial(nn - kk));
        };

        uint64_t catalan = binomial(2*n, n) / (n + 1);
        uint64_t binom2nn = binomial(2*n, n);

        uint64_t ccat = catalan, cbinom = binom2nn;
        uint32_t cn = n;

        preds.push_back({
            "catalan_number",
            PredicateKind::AlwaysTrue,
            PredicateCategory::Combinatorial, 1,
            [ccat, cbinom, cn](uint32_t, uint32_t) -> bool {
                // Verify C(n) * (n+1) == C(2n, n)
                return ccat * (cn + 1) == cbinom;
            },
            "Catalan: C(n)*(n+1) = C(2n,n)"
        });
    }

    // -----------------------------------------------------------------------
    // 3b. COMBINATORIAL: Fibonacci identity F(n)^2 + F(n+1)^2 = F(2n+1)
    // -----------------------------------------------------------------------
    {
        uint32_t n = rng.nextInRange(5, 20);

        auto fib = [](uint32_t k) -> uint64_t {
            if (k == 0) return 0;
            if (k == 1) return 1;
            uint64_t a = 0, b = 1;
            for (uint32_t i = 2; i <= k; i++) {
                uint64_t t = a + b; a = b; b = t;
            }
            return b;
        };

        uint64_t fn = fib(n);
        uint64_t fn1 = fib(n + 1);
        uint64_t f2n1 = fib(2 * n + 1);

        uint64_t cfn = fn, cfn1 = fn1, cf2n1 = f2n1;

        preds.push_back({
            "fibonacci_identity",
            PredicateKind::AlwaysTrue,
            PredicateCategory::Combinatorial, 1,
            [cfn, cfn1, cf2n1](uint32_t, uint32_t) -> bool {
                return cfn * cfn + cfn1 * cfn1 == cf2n1;
            },
            "Fibonacci: F(n)^2 + F(n+1)^2 = F(2n+1)"
        });
    }

    // -----------------------------------------------------------------------
    // 3c. COMBINATORIAL: Lucas sequence U_n(P,Q)
    // U_0=0, U_1=1, U_n = P*U_{n-1} - Q*U_{n-2}
    // Identity: U_{2n} = U_n * V_n where V is companion sequence
    // -----------------------------------------------------------------------
    {
        int64_t P = rng.nextInRange(2, 7);
        int64_t Q = rng.nextInRange(1, 3);
        uint32_t n = rng.nextInRange(4, 12);
        int64_t mod_val = ModArith::nextPrime(rng.nextInRange(1009, 4093));

        // Compute U and V sequences mod mod_val
        std::vector<int64_t> U(2*n+1), V(2*n+1);
        U[0] = 0; U[1] = 1;
        V[0] = 2; V[1] = P;
        for (uint32_t i = 2; i <= 2*n; i++) {
            U[i] = ModArith::mod(P * U[i-1] - Q * U[i-2], mod_val);
            V[i] = ModArith::mod(P * V[i-1] - Q * V[i-2], mod_val);
        }

        int64_t u_n = U[n], v_n = V[n], u_2n = U[2*n];
        int64_t cm = mod_val;

        preds.push_back({
            "lucas_sequence",
            PredicateKind::AlwaysTrue,
            PredicateCategory::Combinatorial, 1,
            [u_n, v_n, u_2n, cm](uint32_t, uint32_t) -> bool {
                // U_{2n} = U_n * V_n (mod m)
                return ModArith::mod(u_n * v_n, cm) == u_2n;
            },
            "Lucas: U_{2n} = U_n * V_n (mod m)"
        });
    }

    // -----------------------------------------------------------------------
    // 4. P-ADIC / HENSEL LIFTING
    // f(a) = 0 mod p, f'(a) != 0 mod p => exists b: f(b)=0 mod p^2, b=a mod p
    // Use f(x) = x^2 - c where c = a^2 mod p, lift to mod p^2
    // -----------------------------------------------------------------------
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(11, 53));
        int64_t a = rng.nextInRange(1, (uint32_t)p - 1);
        int64_t c = ModArith::mod(a * a, p * p); // c = a^2 mod p^2

        // f(x) = x^2 - c, f(a) = a^2 - c
        // f'(x) = 2x, f'(a) = 2a
        // Hensel lift: b = a - f(a)/f'(a) mod p^2
        //   = a - (a^2 - c)/(2a) mod p^2
        int64_t p2 = p * p;
        int64_t fa = ModArith::mod(a * a - c, p2);
        int64_t fpa = ModArith::mod(2 * a, p2);
        int64_t fpaInv = ModArith::modInverse(fpa, p2);
        int64_t b = a;
        if (fpaInv >= 0) {
            b = ModArith::mod(a - fa * fpaInv, p2);
        }

        int64_t cc = c, cb = b, cp2 = p2;
        preds.push_back({
            "hensel_lift",
            PredicateKind::AlwaysTrue,
            PredicateCategory::PAdicArithmetic, 1,
            [cc, cb, cp2](uint32_t, uint32_t) -> bool {
                // Verify b^2 = c mod p^2
                return ModArith::mod(cb * cb - cc, cp2) == 0;
            },
            "Hensel lift: b^2 = c (mod p^2), lifted from root mod p"
        });
    }

    // -----------------------------------------------------------------------
    // 5. INFORMATION THEORY: Shannon entropy bound H(X) <= log2(|alphabet|)
    // For a fixed distribution, verify the bound.
    // -----------------------------------------------------------------------
    {
        // Create a random probability distribution over k symbols
        uint32_t k = rng.nextInRange(3, 8);
        std::vector<uint32_t> weights(k);
        uint32_t total = 0;
        for (uint32_t i = 0; i < k; i++) {
            weights[i] = rng.nextInRange(1, 100);
            total += weights[i];
        }

        // Compute entropy * 1000 (fixed-point) to avoid float
        // H = -sum(p_i * log2(p_i))
        // Use the fact that H <= log2(k)
        // We verify: sum(p_i * log2(1/p_i)) <= log2(k)
        // In integer arithmetic: sum(w_i * log2(total/w_i)) <= total * log2(k)
        // Simpler: just compute and compare in floating point
        double entropy = 0.0;
        for (uint32_t i = 0; i < k; i++) {
            double pi = (double)weights[i] / total;
            if (pi > 0) entropy -= pi * std::log2(pi);
        }
        double maxEntropy = std::log2((double)k);

        // Store as fixed-point * 10000
        int64_t ent_fp = (int64_t)(entropy * 10000);
        int64_t max_fp = (int64_t)(maxEntropy * 10000);

        preds.push_back({
            "shannon_entropy_bound",
            PredicateKind::AlwaysTrue,
            PredicateCategory::InformationTheoretic, 1,
            [ent_fp, max_fp](uint32_t, uint32_t) -> bool {
                return ent_fp <= max_fp;
            },
            "Shannon: H(X) <= log2(|alphabet|)"
        });
    }

    // -----------------------------------------------------------------------
    // 5b. INFORMATION THEORY: Kraft inequality
    // For uniquely decodable code with lengths l_i: sum(2^{-l_i}) <= 1
    // Use integer form: sum(2^{max_l - l_i}) <= 2^{max_l}
    // -----------------------------------------------------------------------
    {
        uint32_t n = rng.nextInRange(3, 6);
        // Generate valid prefix-free code lengths using a simple method
        std::vector<uint32_t> lengths;
        // Start with a balanced binary tree
        uint32_t maxLen = 0;
        for (uint32_t i = 0; i < n; i++) {
            uint32_t l = rng.nextInRange(2, 8);
            lengths.push_back(l);
            if (l > maxLen) maxLen = l;
        }

        // Compute Kraft sum: sum(2^{maxLen - l_i})
        uint64_t kraftSum = 0;
        for (auto l : lengths) {
            kraftSum += (1ULL << (maxLen - l));
        }

        // Ensure Kraft inequality holds (adjust if needed)
        uint64_t kraftMax = (1ULL << maxLen);
        while (kraftSum > kraftMax) {
            // Increase one length
            lengths[rng.nextInRange(0, (uint32_t)lengths.size()-1)]++;
            maxLen = *std::max_element(lengths.begin(), lengths.end());
            kraftSum = 0;
            kraftMax = (1ULL << maxLen);
            for (auto l : lengths) kraftSum += (1ULL << (maxLen - l));
        }

        uint64_t cks = kraftSum, ckm = kraftMax;
        preds.push_back({
            "kraft_inequality",
            PredicateKind::AlwaysTrue,
            PredicateCategory::InformationTheoretic, 1,
            [cks, ckm](uint32_t, uint32_t) -> bool {
                return cks <= ckm;
            },
            "Kraft inequality: sum(2^{-l_i}) <= 1"
        });
    }

    // -----------------------------------------------------------------------
    // 6. TOPOLOGY: Euler characteristic V - E + F = 2 for convex polyhedra
    // Embed a known polyhedron with randomized vertex labeling.
    // -----------------------------------------------------------------------
    {
        // Pick a random Platonic solid
        struct Polyhedron { const char* name; int V, E, F; };
        Polyhedron solids[] = {
            {"tetrahedron", 4, 6, 4},
            {"cube", 8, 12, 6},
            {"octahedron", 6, 12, 8},
            {"dodecahedron", 20, 30, 12},
            {"icosahedron", 12, 30, 20}
        };
        auto& solid = solids[rng.nextInRange(0, 4)];

        // Add random offset to V,E,F but keep V-E+F=2
        int32_t extraV = rng.nextInRange(0, 50);
        int32_t extraE = rng.nextInRange(0, 50);
        int32_t extraF = extraE - extraV; // so (V+eV) - (E+eE) + (F+eF) = V-E+F + eV-eE+eF = 2 + 0

        int32_t v = solid.V + extraV;
        int32_t e = solid.E + extraV + extraE; // adjust so v - e + f = 2
        int32_t f = solid.F + extraE;
        // v - e + f = (V+eV) - (E+eV+eE) + (F+eE) = V-E+F = 2

        int32_t cv = v, ce = e, cf = f;
        std::string solidName = solid.name;
        preds.push_back({
            "euler_characteristic",
            PredicateKind::AlwaysTrue,
            PredicateCategory::TopologyInspired, 1,
            [cv, ce, cf](uint32_t, uint32_t) -> bool {
                return cv - ce + cf == 2;
            },
            "Euler: V-E+F=2 for convex polyhedron (" + solidName + " derived)"
        });
    }

    // -----------------------------------------------------------------------
    // 6b. TOPOLOGY: Betti number b_0 for connected graph = 1
    // Construct a connected graph, compute b_0 via DFS, verify = 1.
    // -----------------------------------------------------------------------
    {
        // Build a random connected graph as adjacency list
        uint32_t nv = rng.nextInRange(5, 12);
        // Start with a spanning tree (path)
        std::vector<std::pair<uint32_t,uint32_t>> edges;
        for (uint32_t i = 0; i < nv - 1; i++) {
            edges.push_back({i, i+1});
        }
        // Add some random extra edges
        uint32_t extraEdges = rng.nextInRange(1, nv);
        for (uint32_t i = 0; i < extraEdges; i++) {
            uint32_t u = rng.nextInRange(0, nv-1);
            uint32_t v = rng.nextInRange(0, nv-1);
            if (u != v) edges.push_back({u, v});
        }

        uint32_t cnv = nv;
        auto cedges = edges;
        preds.push_back({
            "betti_number_b0",
            PredicateKind::AlwaysTrue,
            PredicateCategory::TopologyInspired, 1,
            [cnv, cedges](uint32_t, uint32_t) -> bool {
                // BFS/DFS to count connected components
                std::vector<bool> visited(cnv, false);
                int components = 0;
                for (uint32_t start = 0; start < cnv; start++) {
                    if (visited[start]) continue;
                    components++;
                    // BFS
                    std::vector<uint32_t> queue = {start};
                    visited[start] = true;
                    size_t qi = 0;
                    while (qi < queue.size()) {
                        uint32_t u = queue[qi++];
                        for (auto& e : cedges) {
                            uint32_t nbr = (uint32_t)-1;
                            if (e.first == u) nbr = e.second;
                            else if (e.second == u) nbr = e.first;
                            if (nbr < cnv && !visited[nbr]) {
                                visited[nbr] = true;
                                queue.push_back(nbr);
                            }
                        }
                    }
                }
                return components == 1; // b_0 = 1 for connected graph
            },
            "Betti number b_0 = 1 (connected graph)"
        });
    }

    // -----------------------------------------------------------------------
    // 7. CONTINUED FRACTIONS: Convergent property
    // |sqrt(2) - p_n/q_n| < 1/(q_n * q_{n+1})
    // -----------------------------------------------------------------------
    {
        // Continued fraction of sqrt(2) = [1; 2, 2, 2, ...]
        // Compute convergents p_n/q_n
        uint32_t depth = rng.nextInRange(4, 10);
        std::vector<int64_t> a_cf(depth + 2);
        a_cf[0] = 1;
        for (uint32_t i = 1; i <= depth + 1; i++) a_cf[i] = 2;

        // Compute convergents: p_{-1}=1, p_0=a_0, p_n = a_n*p_{n-1} + p_{n-2}
        std::vector<int64_t> pconv(depth + 2), qconv(depth + 2);
        pconv[0] = a_cf[0]; qconv[0] = 1;
        pconv[1] = a_cf[1] * a_cf[0] + 1; qconv[1] = a_cf[1];
        for (uint32_t i = 2; i <= depth; i++) {
            pconv[i] = a_cf[i] * pconv[i-1] + pconv[i-2];
            qconv[i] = a_cf[i] * qconv[i-1] + qconv[i-2];
        }

        // Verify p_n^2 - 2*q_n^2 = (-1)^{n+1} for sqrt(2) convergents
        // This is equivalent to |sqrt(2) - p_n/q_n| < 1/(q_n*q_{n+1})
        uint32_t n_idx = depth - 1;
        int64_t pn = pconv[n_idx], qn = qconv[n_idx];
        int64_t expected_sign = ((n_idx + 1) % 2 == 0) ? 1 : -1;

        int64_t cpn = pn, cqn = qn, csign = expected_sign;
        preds.push_back({
            "continued_fraction_sqrt2",
            PredicateKind::AlwaysTrue,
            PredicateCategory::ContinuedFraction, 1,
            [cpn, cqn, csign](uint32_t, uint32_t) -> bool {
                // p_n^2 - 2*q_n^2 = (-1)^{n+1}
                int64_t diff = cpn * cpn - 2 * cqn * cqn;
                return diff == csign;
            },
            "Continued fraction: p_n^2 - 2*q_n^2 = (-1)^{n+1} for sqrt(2)"
        });
    }

    // -----------------------------------------------------------------------
    // 8. AUTOMATA THEORY: DFA acceptance - universal language
    // Construct DFA that accepts everything. Simulate on input-derived string.
    // -----------------------------------------------------------------------
    {
        // DFA: single accept state, all transitions go to it
        uint32_t nStates = rng.nextInRange(3, 8);
        // Transition table: all transitions from all states lead to state 0
        // State 0 is the only accept state, and start state
        // Actually, build a more complex DFA that still accepts everything
        // Use multiple states where every state is accepting
        uint32_t cns = nStates;
        // Random transition table (but all states are accepting)
        std::vector<std::array<uint32_t, 2>> transitions(nStates);
        for (uint32_t i = 0; i < nStates; i++) {
            transitions[i][0] = rng.nextInRange(0, nStates - 1);
            transitions[i][1] = rng.nextInRange(0, nStates - 1);
        }

        auto ctrans = transitions;
        preds.push_back({
            "dfa_universal_accept",
            PredicateKind::AlwaysTrue,
            PredicateCategory::AutomataTheory, 1,
            [ctrans, cns](uint32_t x, uint32_t) -> bool {
                // Simulate DFA on binary representation of x
                // All states are accepting, so always true
                uint32_t state = 0;
                for (int bit = 31; bit >= 0; bit--) {
                    uint32_t symbol = (x >> bit) & 1;
                    state = ctrans[state % cns][symbol];
                }
                // All states are accepting
                return state < cns; // always true since state is in [0, nStates)
            },
            "DFA with all-accepting states: universal language"
        });
    }

    // -----------------------------------------------------------------------
    // 8b. AUTOMATA THEORY: Pumping lemma for regular language a*
    // For L = a*, pumping length p=1. Any s in L with |s|>=1 can be pumped.
    // Verify: x*y^i*z in L for all i, where xyz = s, |y| >= 1.
    // -----------------------------------------------------------------------
    {
        // Encode: the "language" is all numbers whose binary has some property
        // Use: L = all binary strings (universal), pumping always works
        uint32_t pumpLen = rng.nextInRange(2, 5);
        uint32_t cpump = pumpLen;
        preds.push_back({
            "pumping_lemma",
            PredicateKind::AlwaysTrue,
            PredicateCategory::AutomataTheory, 1,
            [cpump](uint32_t x, uint32_t) -> bool {
                // Decompose x into x = a * (2^cpump) + b * (2^r) + c
                // where |b| = cpump bits. Pump: a * (2^cpump)^i + c is in language
                // For universal language, always true
                (void)cpump;
                // Simulate: the string "x" in binary is in L={0,1}*
                // After pumping any substring, result is still in {0,1}*
                return true; // trivially pumps, but computation above obscures this
            },
            "Pumping lemma: all binary strings can be pumped (universal language)"
        });
        // Make the above actually compute something to look non-trivial
        preds.back().evaluate = [cpump](uint32_t x, uint32_t) -> bool {
            // Extract middle portion of x and "pump" it (repeat)
            uint32_t mid = (x >> (cpump % 16)) & ((1u << cpump) - 1);
            uint32_t prefix = x >> (cpump + (cpump % 16));
            uint32_t suffix = x & ((1u << (cpump % 16)) - 1);
            // Pumped string: prefix ++ mid ++ mid ++ suffix
            // For universal language, any pumped string is still in L
            uint32_t pumped = (prefix << (2*cpump + (cpump % 16))) |
                              (mid << (cpump + (cpump % 16))) |
                              (mid << (cpump % 16)) |
                              suffix;
            (void)pumped;
            // The language is universal, so always accepted
            return true;
        };
    }

    // -----------------------------------------------------------------------
    // 9. HASHING / BIRTHDAY BOUND
    // For n values in [0, m), if n << sqrt(m), probability of collision is low.
    // Precompute a collision-free set and verify.
    // -----------------------------------------------------------------------
    {
        uint32_t m = rng.nextInRange(10000, 65535);
        uint32_t n = rng.nextInRange(5, 15); // n << sqrt(m) ~ 100-256

        // Generate n distinct values in [0, m) using rng
        std::vector<uint32_t> values;
        std::unordered_set<uint32_t> seen;
        while (values.size() < n) {
            uint32_t v = rng.nextInRange(0, m - 1);
            if (seen.find(v) == seen.end()) {
                seen.insert(v);
                values.push_back(v);
            }
        }

        auto cvals = values;
        preds.push_back({
            "birthday_no_collision",
            PredicateKind::AlwaysTrue,
            PredicateCategory::HashingTheory, 1,
            [cvals](uint32_t, uint32_t) -> bool {
                // Verify no collisions in precomputed set
                std::unordered_set<uint32_t> s(cvals.begin(), cvals.end());
                return s.size() == cvals.size();
            },
            "Birthday bound: precomputed collision-free set verified"
        });
    }

    // -----------------------------------------------------------------------
    // 10. SIGNAL PROCESSING: Parseval's theorem
    // sum|x[n]|^2 = (1/N) * sum|X[k]|^2 for DFT
    // Use integer arithmetic: N * sum|x[n]|^2 = sum|X[k]|^2
    // Compute DFT of a small signal over Z/pZ.
    // -----------------------------------------------------------------------
    {
        // Simpler Parseval-like identity: for any values a,b,c,d:
        // (a+b+c+d)^2 + (a-b+c-d)^2 + (a+b-c-d)^2 + (a-b-c+d)^2 = 4*(a^2+b^2+c^2+d^2)
        // This is the Parseval identity for the Hadamard transform.
        int64_t p = ModArith::nextPrime(rng.nextInRange(97, 251));
        std::array<int64_t, 4> vals;
        for (int i = 0; i < 4; i++)
            vals[i] = rng.nextInRange(1, (uint32_t)p - 1);

        // Hadamard transform outputs
        int64_t h0 = ModArith::mod(vals[0]+vals[1]+vals[2]+vals[3], p);
        int64_t h1 = ModArith::mod(vals[0]-vals[1]+vals[2]-vals[3], p);
        int64_t h2 = ModArith::mod(vals[0]+vals[1]-vals[2]-vals[3], p);
        int64_t h3 = ModArith::mod(vals[0]-vals[1]-vals[2]+vals[3], p);

        int64_t lhs = ModArith::mod(
            ModArith::mod(h0*h0,p) + ModArith::mod(h1*h1,p) +
            ModArith::mod(h2*h2,p) + ModArith::mod(h3*h3,p), p);
        int64_t rhs = ModArith::mod(4 * (
            ModArith::mod(vals[0]*vals[0],p) + ModArith::mod(vals[1]*vals[1],p) +
            ModArith::mod(vals[2]*vals[2],p) + ModArith::mod(vals[3]*vals[3],p)), p);

        int64_t clhs = lhs, crhs = rhs;
        preds.push_back({
            "parseval_theorem",
            PredicateKind::AlwaysTrue,
            PredicateCategory::SignalProcessing, 1,
            [clhs, crhs](uint32_t, uint32_t) -> bool {
                return clhs == crhs;
            },
            "Parseval/Hadamard: sum|H[k]|^2 = N*sum|x[n]|^2 (mod p)"
        });
    }

    // -----------------------------------------------------------------------
    // 10b. SIGNAL PROCESSING: Nyquist criterion
    // Verify sampling_rate >= 2 * bandwidth for fixed config.
    // -----------------------------------------------------------------------
    {
        uint32_t bandwidth = rng.nextInRange(100, 5000);
        uint32_t samplingRate = bandwidth * 2 + rng.nextInRange(1, 1000);

        uint32_t cbw = bandwidth, csr = samplingRate;
        preds.push_back({
            "nyquist_criterion",
            PredicateKind::AlwaysTrue,
            PredicateCategory::SignalProcessing, 1,
            [cbw, csr](uint32_t, uint32_t) -> bool {
                return csr >= 2 * cbw;
            },
            "Nyquist: sampling_rate >= 2*bandwidth"
        });
    }

    // -----------------------------------------------------------------------
    // 11. GAME THEORY: Nash equilibrium - minimax for 2-player zero-sum
    // For a known payoff matrix, verify minimax = maximin.
    // -----------------------------------------------------------------------
    {
        // 2x2 zero-sum game with known saddle point
        // Payoff matrix: [[a, b], [c, d]]
        // Choose values so there's a saddle point: max of row mins = min of col maxes
        int32_t a = rng.nextInRange(1, 10);
        int32_t d = a; // saddle point at diagonal
        int32_t b = a + rng.nextInRange(1, 5); // b > a
        int32_t c = a - rng.nextInRange(1, (uint32_t)std::min(a, 5)); // c < a
        // Row mins: min(a,b) = a, min(c,d) = c. Max of row mins = a.
        // Col maxes: max(a,c) = a, max(b,d) = b. Min of col maxes = a.
        // Minimax = maximin = a. Saddle point exists.

        int32_t ca = a, cb = b, cc = c, cd = d;
        preds.push_back({
            "nash_minimax",
            PredicateKind::AlwaysTrue,
            PredicateCategory::GameTheory, 2,
            [ca, cb, cc, cd](uint32_t, uint32_t) -> bool {
                // maximin = max(min(row0), min(row1))
                int32_t rowMin0 = std::min(ca, cb);
                int32_t rowMin1 = std::min(cc, cd);
                int32_t maximin = std::max(rowMin0, rowMin1);
                // minimax = min(max(col0), max(col1))
                int32_t colMax0 = std::max(ca, cc);
                int32_t colMax1 = std::max(cb, cd);
                int32_t minimax = std::min(colMax0, colMax1);
                return maximin == minimax;
            },
            "Nash: minimax = maximin for zero-sum game with saddle point"
        });
    }

    // -----------------------------------------------------------------------
    // 11b. GAME THEORY: Sprague-Grundy for Nim
    // Grundy number of Nim position = XOR of all pile sizes.
    // Precompute a losing position (Grundy = 0).
    // -----------------------------------------------------------------------
    {
        // Generate piles where XOR = 0 (second player wins)
        uint32_t nPiles = rng.nextInRange(3, 6);
        std::vector<uint32_t> piles(nPiles);
        uint32_t xorSum = 0;
        for (uint32_t i = 0; i < nPiles - 1; i++) {
            piles[i] = rng.nextInRange(1, 100);
            xorSum ^= piles[i];
        }
        piles[nPiles - 1] = xorSum; // last pile makes XOR = 0

        auto cpiles = piles;
        preds.push_back({
            "sprague_grundy_nim",
            PredicateKind::AlwaysTrue,
            PredicateCategory::GameTheory, 1,
            [cpiles](uint32_t, uint32_t) -> bool {
                uint32_t g = 0;
                for (auto p : cpiles) g ^= p;
                return g == 0; // losing position, Grundy number = 0
            },
            "Sprague-Grundy: XOR of Nim piles = 0 (P-position)"
        });
    }

    // -----------------------------------------------------------------------
    // 12. THERMODYNAMIC: Entropy increase
    // S_final >= S_initial for irreversible process.
    // Precompute both, verify.
    // -----------------------------------------------------------------------
    {
        // Model: mixing of two ideal gases. Entropy increases by nR*ln(V_f/V_i)
        // Use integer approximation: S_initial < S_final where
        // S_final = S_initial + delta, delta > 0
        uint32_t s_initial = rng.nextInRange(100, 10000);
        uint32_t delta = rng.nextInRange(1, 5000);
        uint32_t s_final = s_initial + delta;

        uint32_t csi = s_initial, csf = s_final;
        preds.push_back({
            "entropy_increase",
            PredicateKind::AlwaysTrue,
            PredicateCategory::Thermodynamic, 1,
            [csi, csf](uint32_t, uint32_t) -> bool {
                return csf >= csi; // Second law: S_final >= S_initial
            },
            "Second law: S_final >= S_initial for irreversible process"
        });
    }

    // -----------------------------------------------------------------------
    // 13. POLYNOMIAL RING: Bezout's identity in Z[x] mod p
    // For coprime f(x), g(x) over Z/pZ, verify a*f + b*g = 1
    // -----------------------------------------------------------------------
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(97, 251));

        // f(x) = x + c1, g(x) = x + c2, c1 != c2 => gcd = 1
        // Bezout: a*f + b*g = 1
        // a*(x+c1) + b*(x+c2) = 1
        // (a+b)*x + (a*c1 + b*c2) = 1
        // a+b = 0 => b = -a, a*c1 - a*c2 = 1 => a*(c1-c2) = 1 => a = (c1-c2)^{-1}
        int64_t c1 = rng.nextInRange(1, (uint32_t)p - 1);
        int64_t c2;
        do { c2 = rng.nextInRange(1, (uint32_t)p - 1); } while (c2 == c1);

        int64_t diff = ModArith::mod(c1 - c2, p);
        int64_t a_coeff = ModArith::modInverse(diff, p);
        int64_t b_coeff = ModArith::mod(-a_coeff, p);

        int64_t cc1 = c1, cc2 = c2, ca = a_coeff, cbb = b_coeff, cp = p;
        preds.push_back({
            "bezout_polynomial",
            PredicateKind::AlwaysTrue,
            PredicateCategory::PolynomialRing, 1,
            [cc1, cc2, ca, cbb, cp](uint32_t x, uint32_t) -> bool {
                // Evaluate a*(x+c1) + b*(x+c2) mod p, should = 1
                int64_t xm = ModArith::mod(x, cp);
                int64_t f = ModArith::mod(xm + cc1, cp);
                int64_t g = ModArith::mod(xm + cc2, cp);
                int64_t result = ModArith::mod(ca * f + cbb * g, cp);
                return result == 1;
            },
            "Bezout: a*(x+c1) + b*(x+c2) = 1 over Z/pZ"
        });
    }

    // -----------------------------------------------------------------------
    // 13b. POLYNOMIAL RING: Irreducibility witness
    // f(x) = x^2 + bx + c irreducible mod p (discriminant is QNR)
    // Verify f(x) != 0 for all x mod p.
    // -----------------------------------------------------------------------
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(11, 53));
        int64_t b_coeff = rng.nextInRange(0, (uint32_t)p - 1);
        int64_t c_coeff;

        // Find c such that b^2 - 4c is a QNR mod p
        do {
            c_coeff = rng.nextInRange(1, (uint32_t)p - 1);
        } while (ModArith::isQuadraticResidue(
            ModArith::mod(b_coeff * b_coeff - 4 * c_coeff, p), p));

        int64_t cbc = b_coeff, ccc = c_coeff, cp = p;
        preds.push_back({
            "irreducible_polynomial",
            PredicateKind::AlwaysTrue,
            PredicateCategory::PolynomialRing, 1,
            [cbc, ccc, cp](uint32_t x, uint32_t) -> bool {
                // x^2 + b*x + c mod p != 0 for all x (discriminant is QNR)
                int64_t xm = ModArith::mod(x, cp);
                int64_t val = ModArith::mod(xm * xm + cbc * xm + ccc, cp);
                return val != 0;
            },
            "Irreducible: x^2+bx+c has no roots mod p (disc is QNR)"
        });
    }

    // -----------------------------------------------------------------------
    // 14. LFSR: Period of LFSR with primitive polynomial is 2^n - 1
    // Use a known primitive polynomial, run LFSR, verify period.
    // -----------------------------------------------------------------------
    {
        // Known primitive polynomials over GF(2):
        // x^4 + x + 1 (period 15), x^5 + x^2 + 1 (period 31),
        // x^3 + x + 1 (period 7)
        struct PrimPoly { uint32_t degree; uint32_t taps; uint32_t period; };
        PrimPoly polys[] = {
            {3, 0b1011, 7},      // x^3 + x + 1
            {4, 0b10011, 15},    // x^4 + x + 1
            {5, 0b100101, 31},   // x^5 + x^2 + 1
        };
        auto& poly = polys[rng.nextInRange(0, 2)];

        uint32_t initState = rng.nextInRange(1, (1u << poly.degree) - 1); // nonzero
        uint32_t mask = (1u << poly.degree) - 1;

        // Run LFSR for `period` steps, should return to initial state
        uint32_t state = initState;
        uint32_t feedback_taps = poly.taps;
        for (uint32_t step = 0; step < poly.period; step++) {
            // Galois LFSR step
            uint32_t lsb = state & 1;
            state >>= 1;
            if (lsb) state ^= (feedback_taps >> 1);
            state &= mask;
        }

        uint32_t cstate_final = state, cinit = initState;
        uint32_t cperiod = poly.period, cdeg = poly.degree;
        preds.push_back({
            "lfsr_period",
            PredicateKind::AlwaysTrue,
            PredicateCategory::LFSRSequence, 1,
            [cstate_final, cinit](uint32_t, uint32_t) -> bool {
                return cstate_final == cinit; // LFSR returns to initial state after period
            },
            "LFSR with primitive polynomial: period = 2^n-1 (state cycles back)"
        });
    }

    // -----------------------------------------------------------------------
    // 14b. LFSR: Berlekamp-Massey - minimal polynomial degree
    // For a maximal-length LFSR sequence of degree n, the minimal polynomial
    // has degree n. Verify by checking that degree-n LFSR reproduces sequence.
    // -----------------------------------------------------------------------
    {
        // Generate a maximal-length sequence from degree-3 LFSR (x^3+x+1)
        uint32_t degree = 3;
        uint32_t taps = 0b1011;
        uint32_t mask = (1u << degree) - 1;
        uint32_t state = rng.nextInRange(1, mask);

        // Generate 2*degree bits of the sequence
        std::vector<uint32_t> seq;
        uint32_t s = state;
        for (uint32_t i = 0; i < 2 * degree; i++) {
            seq.push_back(s & 1);
            uint32_t lsb = s & 1;
            s >>= 1;
            if (lsb) s ^= (taps >> 1);
            s &= mask;
        }

        // Berlekamp-Massey on this sequence should return degree 3
        // Simplified: the LFSR with the known taps reproduces the sequence
        auto cseq = seq;
        uint32_t ctaps = taps, cmask = mask, cstate = state, cdeg = degree;
        preds.push_back({
            "berlekamp_massey",
            PredicateKind::AlwaysTrue,
            PredicateCategory::LFSRSequence, 1,
            [cseq, ctaps, cmask, cstate, cdeg](uint32_t, uint32_t) -> bool {
                // Verify that the known LFSR reproduces the sequence
                uint32_t s = cstate;
                for (uint32_t i = 0; i < cseq.size(); i++) {
                    if ((s & 1) != cseq[i]) return false;
                    uint32_t lsb = s & 1;
                    s >>= 1;
                    if (lsb) s ^= (ctaps >> 1);
                    s &= cmask;
                }
                return true;
            },
            "Berlekamp-Massey: known LFSR reproduces precomputed sequence"
        });
    }

    // -----------------------------------------------------------------------
    // EXTRA 1: Quadratic reciprocity law
    // (p/q)(q/p) = (-1)^{((p-1)/2)*((q-1)/2)} for odd primes p,q
    // -----------------------------------------------------------------------
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(11, 53));
        int64_t q = ModArith::nextPrime(p + 1);
        while (q == p) q = ModArith::nextPrime(q + 1);

        // Compute Legendre symbols using Euler's criterion
        int64_t legendre_pq = ModArith::powMod(p % q, (q - 1) / 2, q);
        if (legendre_pq > 1) legendre_pq -= q; // normalize to {-1, 0, 1}
        int64_t legendre_qp = ModArith::powMod(q % p, (p - 1) / 2, p);
        if (legendre_qp > 1) legendre_qp -= p;

        int64_t product = legendre_pq * legendre_qp;
        int64_t expected = (((p - 1) / 2) * ((q - 1) / 2)) % 2 == 0 ? 1 : -1;

        int64_t cprod = product, cexp = expected;
        preds.push_back({
            "quadratic_reciprocity",
            PredicateKind::AlwaysTrue,
            PredicateCategory::AlgebraicGeometry, 1,
            [cprod, cexp](uint32_t, uint32_t) -> bool {
                return cprod == cexp;
            },
            "Quadratic reciprocity: (p/q)(q/p) = (-1)^{((p-1)/2)((q-1)/2)}"
        });
    }

    // -----------------------------------------------------------------------
    // EXTRA 2: Fermat's two-square theorem
    // Prime p = 1 mod 4 can be written as sum of two squares.
    // Precompute a^2 + b^2 = p, verify.
    // -----------------------------------------------------------------------
    {
        // Find a prime p = 1 mod 4
        int64_t p = ModArith::nextPrime(rng.nextInRange(13, 97));
        while (p % 4 != 1) p = ModArith::nextPrime(p + 1);

        // Find a, b such that a^2 + b^2 = p by brute force (p is small)
        int64_t fa = 0, fb = 0;
        for (int64_t a = 1; a * a < p; a++) {
            int64_t rem = p - a * a;
            // Check if rem is a perfect square
            int64_t sr = (int64_t)std::sqrt((double)rem);
            for (int64_t b = std::max((int64_t)1, sr - 1); b <= sr + 1; b++) {
                if (b * b == rem) {
                    fa = a; fb = b;
                    break;
                }
            }
            if (fa > 0) break;
        }

        int64_t cfa = fa, cfb = fb, cp = p;
        preds.push_back({
            "fermat_two_squares",
            PredicateKind::AlwaysTrue,
            PredicateCategory::AlgebraicGeometry, 1,
            [cfa, cfb, cp](uint32_t, uint32_t) -> bool {
                return cfa * cfa + cfb * cfb == cp;
            },
            "Fermat two-squares: a^2 + b^2 = p for prime p = 1 mod 4"
        });
    }

    // -----------------------------------------------------------------------
    // EXTRA 3: Chinese Remainder Theorem consistency
    // Precompute x such that x = a1 mod m1 and x = a2 mod m2 with gcd(m1,m2)=1.
    // Verify both congruences.
    // -----------------------------------------------------------------------
    {
        int64_t m1 = ModArith::nextPrime(rng.nextInRange(11, 31));
        int64_t m2 = ModArith::nextPrime(m1 + 1);
        int64_t a1 = rng.nextInRange(0, (uint32_t)m1 - 1);
        int64_t a2 = rng.nextInRange(0, (uint32_t)m2 - 1);

        // CRT: x = a1*m2*inv(m2,m1) + a2*m1*inv(m1,m2) mod (m1*m2)
        int64_t M = m1 * m2;
        int64_t inv1 = ModArith::modInverse(m2, m1);
        int64_t inv2 = ModArith::modInverse(m1, m2);
        int64_t x_crt = ModArith::mod(a1 * m2 * inv1 + a2 * m1 * inv2, M);

        int64_t cx = x_crt, cm1 = m1, cm2 = m2, ca1 = a1, ca2 = a2;
        preds.push_back({
            "crt_consistency",
            PredicateKind::AlwaysTrue,
            PredicateCategory::PAdicArithmetic, 2,
            [cx, cm1, cm2, ca1, ca2](uint32_t, uint32_t) -> bool {
                return ModArith::mod(cx, cm1) == ca1 &&
                       ModArith::mod(cx, cm2) == ca2;
            },
            "CRT: x = a1 mod m1 and x = a2 mod m2"
        });
    }

    // -----------------------------------------------------------------------
    // EXTRA 4: Determinant-trace identity for 2x2 matrix
    // det(A) = (tr(A)^2 - tr(A^2)) / 2 for 2x2 matrix
    // -----------------------------------------------------------------------
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(97, 251));
        // Need p odd so 2 is invertible
        int64_t inv2 = ModArith::modInverse(2, p);

        int64_t a00 = rng.nextInRange(0, (uint32_t)p-1);
        int64_t a01 = rng.nextInRange(0, (uint32_t)p-1);
        int64_t a10 = rng.nextInRange(0, (uint32_t)p-1);
        int64_t a11 = rng.nextInRange(0, (uint32_t)p-1);

        int64_t cp=p, ci2=inv2;
        int64_t c00=a00, c01=a01, c10=a10, c11=a11;
        preds.push_back({
            "det_trace_identity",
            PredicateKind::AlwaysTrue,
            PredicateCategory::RepresentationTheory, 1,
            [c00,c01,c10,c11,cp,ci2](uint32_t x, uint32_t) -> bool {
                // Perturb with input
                int64_t xm = x % (uint32_t)cp;
                int64_t m00 = ModArith::mod(c00+xm, cp), m01=c01, m10=c10;
                int64_t m11 = ModArith::mod(c11+xm, cp);
                // det(A) = m00*m11 - m01*m10
                int64_t det = ModArith::mod(m00*m11 - m01*m10, cp);
                // tr(A) = m00 + m11
                int64_t tr = ModArith::mod(m00 + m11, cp);
                // A^2: [m00^2+m01*m10, m00*m01+m01*m11, m10*m00+m11*m10, m10*m01+m11^2]
                int64_t a2_00 = ModArith::mod(m00*m00 + m01*m10, cp);
                int64_t a2_11 = ModArith::mod(m10*m01 + m11*m11, cp);
                int64_t trA2 = ModArith::mod(a2_00 + a2_11, cp);
                // det = (tr^2 - trA2) * inv(2) mod p
                int64_t rhs = ModArith::mod(ModArith::mod(tr*tr - trA2, cp) * ci2, cp);
                return det == rhs;
            },
            "det(A) = (tr(A)^2 - tr(A^2))/2 for 2x2 matrix"
        });
    }

    // -----------------------------------------------------------------------
    // EXTRA 5: Cayley-Hamilton theorem for 2x2 matrix
    // A^2 - tr(A)*A + det(A)*I = 0
    // -----------------------------------------------------------------------
    {
        int64_t p = ModArith::nextPrime(rng.nextInRange(97, 251));
        int64_t a00 = rng.nextInRange(0, (uint32_t)p-1);
        int64_t a01 = rng.nextInRange(0, (uint32_t)p-1);
        int64_t a10 = rng.nextInRange(0, (uint32_t)p-1);
        int64_t a11 = rng.nextInRange(0, (uint32_t)p-1);

        int64_t cp=p, c00=a00, c01=a01, c10=a10, c11=a11;
        preds.push_back({
            "cayley_hamilton",
            PredicateKind::AlwaysTrue,
            PredicateCategory::RepresentationTheory, 1,
            [c00,c01,c10,c11,cp](uint32_t, uint32_t) -> bool {
                // A^2 - tr(A)*A + det(A)*I = 0 mod p
                int64_t tr = ModArith::mod(c00 + c11, cp);
                int64_t det = ModArith::mod(c00*c11 - c01*c10, cp);

                // A^2
                int64_t a2_00 = ModArith::mod(c00*c00 + c01*c10, cp);
                int64_t a2_01 = ModArith::mod(c00*c01 + c01*c11, cp);
                int64_t a2_10 = ModArith::mod(c10*c00 + c11*c10, cp);
                int64_t a2_11 = ModArith::mod(c10*c01 + c11*c11, cp);

                // A^2 - tr*A + det*I
                int64_t r00 = ModArith::mod(a2_00 - tr*c00 + det, cp);
                int64_t r01 = ModArith::mod(a2_01 - tr*c01, cp);
                int64_t r10 = ModArith::mod(a2_10 - tr*c10, cp);
                int64_t r11 = ModArith::mod(a2_11 - tr*c11 + det, cp);

                return r00 == 0 && r01 == 0 && r10 == 0 && r11 == 0;
            },
            "Cayley-Hamilton: A^2 - tr(A)*A + det(A)*I = 0 for 2x2"
        });
    }

    return preds;
}

// ============================================================================
// Master function
// ============================================================================

std::vector<OpaquePredicate> getAllPredicates(ObfRNG& rng) {
    std::vector<OpaquePredicate> all;

    auto append = [&all](std::vector<OpaquePredicate>&& v) {
        all.insert(all.end(), std::make_move_iterator(v.begin()),
                   std::make_move_iterator(v.end()));
    };

    append(createNumberTheoreticPredicates());
    append(createModularArithmeticPredicates(rng));
    append(createBitwisePredicates());
    append(createFloatingPointPredicates());
    append(createPointerPredicates());
    append(createDynamicPredicates(rng));
    append(createGraphTheoreticPredicates(rng));
    append(createLatticeBasedPredicates(rng));
    append(createMQPredicates(rng));
    append(createExpanderGraphPredicates(rng));
    append(createCellularAutomatonPredicates(rng));
    append(createCryptoHashPredicates(rng));
    append(createECCPredicates(rng));
    append(createUndecidablePredicates(rng));
    append(createNovelPredicates(rng));

    return all;
}

const OpaquePredicate& pickPredicate(const std::vector<OpaquePredicate>& predicates,
                                      PredicateKind kind, ObfRNG& rng) {
    std::vector<size_t> matching;
    for (size_t i = 0; i < predicates.size(); i++) {
        if (predicates[i].kind == kind) matching.push_back(i);
    }
    if (matching.empty()) {
        // Fallback to first predicate
        return predicates[0];
    }
    return predicates[matching[rng.nextInRange(0, (uint32_t)matching.size() - 1)]];
}

} // namespace shroud
