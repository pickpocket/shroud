#include <gtest/gtest.h>
#include "Shroud/OpaquePredicates.h"
#include "Shroud/MBA.h"
#include "Shroud/OverlappingInstructions.h"

using namespace shroud;

// ============================================================================
// Integration: combining multiple obfuscation techniques
// ============================================================================

TEST(Integration, MBAPlusOpaquePredicates_PreserveSemantics) {
    // Simulate obfuscating a simple computation: result = (a + b) ^ c
    ObfRNG rng(42);

    // Get an MBA rule for ADD
    auto addRules = getAddRules();
    auto& addRule = addRules[0]; // add_xor_carry

    // Get an MBA rule for XOR
    auto xorRules = getXorRules();
    auto& xorRule = xorRules[0]; // xor_or_minus_and

    // Verify combined transformation
    for (uint32_t a = 0; a < 100; a++) {
        for (uint32_t b = 0; b < 100; b++) {
            for (uint32_t c = 0; c < 100; c++) {
                uint32_t expected = (a + b) ^ c;

                // Step 1: MBA for addition
                uint32_t mba_add = addRule.eval(a, b);
                EXPECT_EQ(mba_add, a + b);

                // Step 2: MBA for XOR
                uint32_t mba_xor = xorRule.eval(mba_add, c);
                EXPECT_EQ(mba_xor, expected)
                    << "Failed for a=" << a << " b=" << b << " c=" << c;
                if (::testing::Test::HasFailure()) return;
            }
        }
    }
}

TEST(Integration, DynamicMBAPlusOpaquePredicates) {
    ObfRNG rng(42);

    // Generate dynamic MBA for addition
    auto mba_add = generateDynamicMBA(ArithOp::Add, rng);

    // Generate opaque predicate
    auto preds = getAllPredicates(rng);
    auto& pred = pickPredicate(preds, PredicateKind::AlwaysTrue, rng);

    // Combined: if (opaque_true) { use MBA(add) } else { bogus }
    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            // Opaque predicate is always true
            EXPECT_TRUE(pred.evaluate(x, y));

            // So we always take the MBA path
            uint32_t result = mba_add.eval(x, y);
            EXPECT_EQ(result, x + y);
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(Integration, ConstantObfuscationInComputation) {
    // Obfuscate: result = x + 42
    // Replace 42 with MBA expression, and replace + with MBA
    ObfRNG rng(42);

    auto const42 = obfuscateConstant(42, rng, 32);
    auto mba_add = generateDynamicMBA(ArithOp::Add, rng);

    ObfRNG testRng(999);
    for (int i = 0; i < 10000; i++) {
        uint32_t x = testRng.next32();
        uint32_t y = testRng.next32(); // dummy for const evaluation

        // Get obfuscated 42
        uint32_t c42 = const42.eval(x, y);
        EXPECT_EQ(c42, 42u);

        // Add using MBA
        uint32_t result = mba_add.eval(x, c42);
        EXPECT_EQ(result, x + 42);
        if (::testing::Test::HasFailure()) return;
    }
}

TEST(Integration, AllPatternsHaveInlineAsm) {
    auto patterns = getAllPatterns(ArchTarget::Both);
    for (auto& p : patterns) {
        std::string asm_att = p.inlineAsmATT();
        EXPECT_FALSE(asm_att.empty()) << "Pattern " << p.name << " has empty inline asm";
    }
}

TEST(Integration, MultiRoundMBAPreservesSemantics) {
    ObfRNG rng(42);

    // Test multi-round for all supported operations
    ArithOp ops[] = {ArithOp::Add, ArithOp::Sub, ArithOp::Xor};
    for (auto op : ops) {
        auto expr = chainMBA(op, rng, 2);
        ASSERT_NE(expr, nullptr);

        for (uint32_t x = 0; x < 256; x++) {
            for (uint32_t y = 0; y < 256; y++) {
                uint32_t expected;
                switch (op) {
                case ArithOp::Add: expected = x + y; break;
                case ArithOp::Sub: expected = x - y; break;
                case ArithOp::Xor: expected = x ^ y; break;
                default: expected = 0;
                }
                EXPECT_EQ(expr->eval(x, y), expected)
                    << "Multi-round MBA failed for op at x=" << x << " y=" << y;
                if (::testing::Test::HasFailure()) return;
            }
        }
    }
}

TEST(Integration, OverlappingPatternsDontConflict) {
    // Verify that picking multiple patterns doesn't produce duplicates
    // or conflicting patterns
    ObfRNG rng(42);

    for (int trial = 0; trial < 10; trial++) {
        auto picked = pickPatterns(rng, ArchTarget::Both, 5);
        EXPECT_EQ(picked.size(), 5u);

        for (auto& p : picked) {
            EXPECT_TRUE(validatePattern(p));
        }
    }
}

TEST(Integration, FullObfuscationPipeline) {
    // Simulate the full obfuscation pipeline on a simple function:
    // f(x, y) = (x + y) * 3
    ObfRNG rng(42);

    // Step 1: Generate MBA for addition
    auto mba_add = generateDynamicMBA(ArithOp::Add, rng);

    // Step 2: Obfuscate constant 3
    auto const3 = obfuscateConstant(3, rng, 32);

    // Step 3: Generate opaque predicates for bogus branches
    auto preds = getAllPredicates(rng);

    // Step 4: Pick overlapping instruction patterns for anti-disassembly
    auto overlap = pickPatterns(rng, ArchTarget::Both, 3);

    // Verify everything works together
    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            uint32_t expected = (x + y) * 3;

            // MBA addition
            uint32_t sum = mba_add.eval(x, y);
            EXPECT_EQ(sum, x + y);

            // Obfuscated constant
            uint32_t c3 = const3.eval(x, y);
            EXPECT_EQ(c3, 3u);

            // Final multiplication
            uint32_t result = sum * c3;
            EXPECT_EQ(result, expected);

            if (::testing::Test::HasFailure()) return;
        }
    }

    // Opaque predicates are all true
    for (int i = 0; i < 10; i++) {
        auto& pred = pickPredicate(preds, PredicateKind::AlwaysTrue, rng);
        EXPECT_TRUE(pred.evaluate(42, 7));
    }

    // Overlap patterns are valid
    for (auto& p : overlap) {
        EXPECT_TRUE(validatePattern(p));
    }
}

TEST(Integration, LargeScaleRandom32Bit) {
    ObfRNG rng(42);
    auto mba_add = generateDynamicMBA(ArithOp::Add, rng);
    auto mba_xor = generateDynamicMBA(ArithOp::Xor, rng);
    auto mba_sub = generateDynamicMBA(ArithOp::Sub, rng);
    auto mba_and = generateDynamicMBA(ArithOp::And, rng);
    auto mba_or  = generateDynamicMBA(ArithOp::Or, rng);

    ObfRNG testRng(999);
    for (int i = 0; i < 100000; i++) {
        uint32_t x = testRng.next32();
        uint32_t y = testRng.next32();

        EXPECT_EQ(mba_add.eval(x, y), x + y);
        EXPECT_EQ(mba_xor.eval(x, y), x ^ y);
        EXPECT_EQ(mba_sub.eval(x, y), x - y);
        EXPECT_EQ(mba_and.eval(x, y), x & y);
        EXPECT_EQ(mba_or.eval(x, y),  x | y);
        if (::testing::Test::HasFailure()) return;
    }
}
