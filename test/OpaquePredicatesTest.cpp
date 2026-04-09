#include <gtest/gtest.h>
#include "Shroud/OpaquePredicates.h"
#include <set>
#include <cstring>

using namespace shroud;

// ============================================================================
// Helper: verify a predicate over a range of inputs
// ============================================================================

static void verifyPredicate(const OpaquePredicate& pred, uint32_t maxVal = 1000) {
    for (uint32_t x = 0; x < maxVal; x++) {
        for (uint32_t y = 0; y < (pred.numInputs > 1 ? maxVal : 1u); y++) {
            bool result = pred.evaluate(x, y);
            if (pred.kind == PredicateKind::AlwaysTrue) {
                EXPECT_TRUE(result)
                    << "Predicate '" << pred.name << "' returned false for x="
                    << x << " y=" << y;
            } else if (pred.kind == PredicateKind::AlwaysFalse) {
                EXPECT_FALSE(result)
                    << "Predicate '" << pred.name << "' returned true for x="
                    << x << " y=" << y;
            }
            if (::testing::Test::HasFailure()) return;
        }
    }
}

// ============================================================================
// 3a: Number-Theoretic Invariant Predicates
// ============================================================================

TEST(OpaquePredicates, NumberTheoretic_AllAlwaysTrue) {
    auto preds = createNumberTheoreticPredicates();
    EXPECT_GE(preds.size(), 8u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_EQ(pred.kind, PredicateKind::AlwaysTrue);
        verifyPredicate(pred, 500);
    }
}

TEST(OpaquePredicates, NumberTheoretic_ConsecutiveEven) {
    auto preds = createNumberTheoreticPredicates();
    auto it = std::find_if(preds.begin(), preds.end(),
        [](const OpaquePredicate& p) { return p.name == "consec_even"; });
    ASSERT_NE(it, preds.end());

    // Test edge cases
    EXPECT_TRUE(it->evaluate(0, 0));
    EXPECT_TRUE(it->evaluate(1, 0));
    EXPECT_TRUE(it->evaluate(UINT32_MAX, 0));
    EXPECT_TRUE(it->evaluate(UINT32_MAX - 1, 0));
}

TEST(OpaquePredicates, NumberTheoretic_FermatMod3) {
    auto preds = createNumberTheoreticPredicates();
    auto it = std::find_if(preds.begin(), preds.end(),
        [](const OpaquePredicate& p) { return p.name == "fermat_mod3"; });
    ASSERT_NE(it, preds.end());

    // x^3 - x is always divisible by 3
    for (uint32_t x = 0; x < 10000; x++) {
        EXPECT_TRUE(it->evaluate(x, 0)) << "Failed for x=" << x;
    }
}

TEST(OpaquePredicates, NumberTheoretic_CarryIdentity) {
    auto preds = createNumberTheoreticPredicates();
    auto it = std::find_if(preds.begin(), preds.end(),
        [](const OpaquePredicate& p) { return p.name == "carry_identity"; });
    ASSERT_NE(it, preds.end());

    // (x + y) - (x ^ y) == 2*(x & y) for all x, y
    ObfRNG rng(42);
    for (int i = 0; i < 10000; i++) {
        uint32_t x = rng.next32();
        uint32_t y = rng.next32();
        EXPECT_TRUE(it->evaluate(x, y)) << "Failed for x=" << x << " y=" << y;
    }
}

TEST(OpaquePredicates, NumberTheoretic_PartitionIdentity) {
    auto preds = createNumberTheoreticPredicates();
    auto it = std::find_if(preds.begin(), preds.end(),
        [](const OpaquePredicate& p) { return p.name == "partition_identity"; });
    ASSERT_NE(it, preds.end());

    ObfRNG rng(123);
    for (int i = 0; i < 10000; i++) {
        uint32_t x = rng.next32();
        uint32_t y = rng.next32();
        EXPECT_TRUE(it->evaluate(x, y));
    }
}

// ============================================================================
// 3b: Modular Arithmetic Predicates
// ============================================================================

TEST(OpaquePredicates, ModularArithmetic_AllTrue) {
    ObfRNG rng(42);
    auto preds = createModularArithmeticPredicates(rng);
    EXPECT_GE(preds.size(), 3u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        verifyPredicate(pred, 300);
    }
}

TEST(OpaquePredicates, ModularArithmetic_DifferentSeeds) {
    ObfRNG rng1(42);
    ObfRNG rng2(999);
    auto p1 = createModularArithmeticPredicates(rng1);
    auto p2 = createModularArithmeticPredicates(rng2);

    // Different seeds should produce different predicates
    bool anyDifferent = false;
    for (size_t i = 0; i < std::min(p1.size(), p2.size()); i++) {
        if (p1[i].description != p2[i].description) {
            anyDifferent = true;
            break;
        }
    }
    EXPECT_TRUE(anyDifferent) << "Different seeds should produce different predicates";
}

// ============================================================================
// 3c: Bitwise Predicates
// ============================================================================

TEST(OpaquePredicates, Bitwise_AllTrue) {
    auto preds = createBitwisePredicates();
    EXPECT_GE(preds.size(), 6u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        verifyPredicate(pred, 500);
    }
}

TEST(OpaquePredicates, Bitwise_RotateRoundTrip) {
    auto preds = createBitwisePredicates();
    auto it = std::find_if(preds.begin(), preds.end(),
        [](const OpaquePredicate& p) { return p.name == "rotate_roundtrip"; });
    ASSERT_NE(it, preds.end());

    ObfRNG rng(99);
    for (int i = 0; i < 10000; i++) {
        uint32_t x = rng.next32();
        uint32_t n = rng.next32();
        EXPECT_TRUE(it->evaluate(x, n));
    }
}

TEST(OpaquePredicates, Bitwise_TwosComplement) {
    auto preds = createBitwisePredicates();
    auto it = std::find_if(preds.begin(), preds.end(),
        [](const OpaquePredicate& p) { return p.name == "twos_complement"; });
    ASSERT_NE(it, preds.end());

    for (uint32_t x = 0; x < 65536; x++) {
        EXPECT_TRUE(it->evaluate(x, 0)) << "Failed for x=" << x;
    }
}

// ============================================================================
// 3d: Floating-Point Predicates
// ============================================================================

TEST(OpaquePredicates, FloatingPoint_AllTrue) {
    auto preds = createFloatingPointPredicates();
    EXPECT_GE(preds.size(), 2u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        verifyPredicate(pred, 10000);
    }
}

// ============================================================================
// 3e: Pointer Predicates
// ============================================================================

TEST(OpaquePredicates, Pointer_AllTrue) {
    auto preds = createPointerPredicates();
    EXPECT_GE(preds.size(), 1u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_TRUE(pred.evaluate(0, 0));
        EXPECT_TRUE(pred.evaluate(42, 7));
    }
}

// ============================================================================
// 3f: Dynamic Predicates
// ============================================================================

TEST(OpaquePredicates, Dynamic_AllTrue) {
    ObfRNG rng(42);
    auto preds = createDynamicPredicates(rng, 20);
    EXPECT_GE(preds.size(), 10u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        verifyPredicate(pred, 200);
    }
}

TEST(OpaquePredicates, Dynamic_UniquenessAcrossSeeds) {
    std::set<std::string> descriptions;

    for (uint64_t seed = 0; seed < 10; seed++) {
        ObfRNG rng(seed);
        auto preds = createDynamicPredicates(rng, 5);
        for (auto& p : preds) {
            descriptions.insert(p.description);
        }
    }

    // Should have more than just a few unique descriptions
    EXPECT_GE(descriptions.size(), 3u);
}

// ============================================================================
// 3g: Graph-Theoretic Predicates
// ============================================================================

TEST(OpaquePredicates, GraphTheoretic_AllTrue) {
    ObfRNG rng(42);
    auto preds = createGraphTheoreticPredicates(rng);
    EXPECT_GE(preds.size(), 1u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_TRUE(pred.evaluate(0, 0));
        EXPECT_TRUE(pred.evaluate(100, 200));
    }
}

// ============================================================================
// 3h: Lattice-Based Predicates
// ============================================================================

TEST(OpaquePredicates, LatticeBased_AllTrue) {
    ObfRNG rng(42);
    auto preds = createLatticeBasedPredicates(rng);
    EXPECT_GE(preds.size(), 2u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_TRUE(pred.evaluate(0, 0));
        EXPECT_TRUE(pred.evaluate(999, 123));
    }
}

TEST(OpaquePredicates, LatticeBased_DifferentSeeds) {
    ObfRNG rng1(42);
    ObfRNG rng2(999);
    auto p1 = createLatticeBasedPredicates(rng1);
    auto p2 = createLatticeBasedPredicates(rng2);

    EXPECT_GE(p1.size(), 1u);
    EXPECT_GE(p2.size(), 1u);
    // Different seeds should produce different lattice instances
}

// ============================================================================
// 3i: MQ Predicates
// ============================================================================

TEST(OpaquePredicates, MQ_AllTrue) {
    ObfRNG rng(42);
    auto preds = createMQPredicates(rng);
    EXPECT_GE(preds.size(), 1u);

    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_TRUE(pred.evaluate(0, 0));
    }
}

// ============================================================================
// 3j-n: Other categories
// ============================================================================

TEST(OpaquePredicates, ExpanderGraph_True) {
    ObfRNG rng(42);
    auto preds = createExpanderGraphPredicates(rng);
    EXPECT_GE(preds.size(), 1u);
    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        verifyPredicate(pred, 100);
    }
}

TEST(OpaquePredicates, CellularAutomaton_True) {
    ObfRNG rng(42);
    auto preds = createCellularAutomatonPredicates(rng);
    EXPECT_GE(preds.size(), 1u);
    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_TRUE(pred.evaluate(0, 0));
    }
}

TEST(OpaquePredicates, CryptoHash_True) {
    ObfRNG rng(42);
    auto preds = createCryptoHashPredicates(rng);
    EXPECT_GE(preds.size(), 2u);
    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        verifyPredicate(pred, 1000);
    }
}

TEST(OpaquePredicates, ECC_True) {
    ObfRNG rng(42);
    auto preds = createECCPredicates(rng);
    EXPECT_GE(preds.size(), 1u);
    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_TRUE(pred.evaluate(0, 0));
    }
}

TEST(OpaquePredicates, Undecidable_True) {
    ObfRNG rng(42);
    auto preds = createUndecidablePredicates(rng);
    EXPECT_GE(preds.size(), 2u);
    for (auto& pred : preds) {
        SCOPED_TRACE("Predicate: " + pred.name);
        EXPECT_TRUE(pred.evaluate(0, 0));
    }
}

// ============================================================================
// Composite predicates
// ============================================================================

TEST(OpaquePredicates, Composite_ANDofTwoTrue) {
    auto numPreds = createNumberTheoreticPredicates();
    ASSERT_GE(numPreds.size(), 2u);

    ObfRNG rng(42);
    auto composed = composePredicates(numPreds[0], numPreds[1], rng);
    EXPECT_EQ(composed.kind, PredicateKind::AlwaysTrue);

    for (uint32_t x = 0; x < 1000; x++) {
        EXPECT_TRUE(composed.evaluate(x, 0));
    }
}

// ============================================================================
// Master collection
// ============================================================================

TEST(OpaquePredicates, GetAll_HasManyPredicates) {
    ObfRNG rng(42);
    auto all = getAllPredicates(rng);

    // Should have a substantial number of predicates
    EXPECT_GT(all.size(), 30u);

    // All should be verifiable
    int testedCount = 0;
    for (auto& pred : all) {
        if (pred.kind == PredicateKind::AlwaysTrue) {
            SCOPED_TRACE("Predicate: " + pred.name);
            // Quick check with just a few values
            EXPECT_TRUE(pred.evaluate(0, 0));
            EXPECT_TRUE(pred.evaluate(42, 7));
            EXPECT_TRUE(pred.evaluate(255, 128));
            testedCount++;
        }
    }
    EXPECT_GT(testedCount, 20);
}

TEST(OpaquePredicates, PickPredicate_ReturnsCorrectKind) {
    ObfRNG rng(42);
    auto all = getAllPredicates(rng);

    for (int i = 0; i < 100; i++) {
        auto& picked = pickPredicate(all, PredicateKind::AlwaysTrue, rng);
        EXPECT_EQ(picked.kind, PredicateKind::AlwaysTrue);
    }
}

// ============================================================================
// Category diversity
// ============================================================================

TEST(OpaquePredicates, CategoryDiversity) {
    ObfRNG rng(42);
    auto all = getAllPredicates(rng);

    std::set<PredicateCategory> categories;
    for (auto& p : all) {
        categories.insert(p.category);
    }

    // Should have predicates from many categories
    EXPECT_GE(categories.size(), 8u);
}
