#include <gtest/gtest.h>
#include "Shroud/MBA.h"
#include <set>

using namespace shroud;

// ============================================================================
// Helper: verify MBA rule by exhaustive 8-bit and random 32-bit testing
// ============================================================================

static void verifyMBARule(const MBARule& rule, uint32_t bits = 8) {
    uint32_t max = (bits <= 16) ? (1u << bits) : 1000u;

    if (rule.target == ArithOp::Not) {
        for (uint32_t x = 0; x < max; x++) {
            uint32_t expected = ~x;
            uint32_t got = rule.eval(x, 0);
            EXPECT_EQ(expected, got)
                << "Rule '" << rule.name << "' failed for x=" << x
                << " expected=" << expected << " got=" << got;
            if (::testing::Test::HasFailure()) return;
        }
        return;
    }

    for (uint32_t x = 0; x < max; x++) {
        for (uint32_t y = 0; y < max; y++) {
            uint32_t expected;
            switch (rule.target) {
            case ArithOp::Add: expected = x + y; break;
            case ArithOp::Sub: expected = x - y; break;
            case ArithOp::Xor: expected = x ^ y; break;
            case ArithOp::And: expected = x & y; break;
            case ArithOp::Or:  expected = x | y; break;
            case ArithOp::Mul: expected = x * y; break;
            default: expected = 0;
            }
            uint32_t got = rule.eval(x, y);
            EXPECT_EQ(expected, got)
                << "Rule '" << rule.name << "' failed for x=" << x
                << " y=" << y << " expected=" << expected << " got=" << got;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

static void verifyMBARule32Random(const MBARule& rule, int numTests = 10000) {
    ObfRNG rng(42);

    for (int i = 0; i < numTests; i++) {
        uint32_t x = rng.next32();
        uint32_t y = rng.next32();
        uint32_t expected;
        switch (rule.target) {
        case ArithOp::Add: expected = x + y; break;
        case ArithOp::Sub: expected = x - y; break;
        case ArithOp::Xor: expected = x ^ y; break;
        case ArithOp::And: expected = x & y; break;
        case ArithOp::Or:  expected = x | y; break;
        case ArithOp::Not: expected = ~x; break;
        case ArithOp::Mul: expected = x * y; break;
        }
        uint32_t got = rule.eval(x, y);
        EXPECT_EQ(expected, got)
            << "Rule '" << rule.name << "' failed for x=" << x
            << " y=" << y;
        if (::testing::Test::HasFailure()) return;
    }
}

// ============================================================================
// 4a: Static substitution rules — exhaustive 8-bit verification
// ============================================================================

TEST(MBA, AddRules_Exhaustive8Bit) {
    for (auto& rule : getAddRules()) {
        SCOPED_TRACE("Rule: " + rule.name);
        verifyMBARule(rule, 8);
    }
}

TEST(MBA, SubRules_Exhaustive8Bit) {
    for (auto& rule : getSubRules()) {
        SCOPED_TRACE("Rule: " + rule.name);
        verifyMBARule(rule, 8);
    }
}

TEST(MBA, XorRules_Exhaustive8Bit) {
    for (auto& rule : getXorRules()) {
        SCOPED_TRACE("Rule: " + rule.name);
        verifyMBARule(rule, 8);
    }
}

TEST(MBA, AndRules_Exhaustive8Bit) {
    for (auto& rule : getAndRules()) {
        SCOPED_TRACE("Rule: " + rule.name);
        verifyMBARule(rule, 8);
    }
}

TEST(MBA, OrRules_Exhaustive8Bit) {
    for (auto& rule : getOrRules()) {
        SCOPED_TRACE("Rule: " + rule.name);
        verifyMBARule(rule, 8);
    }
}

TEST(MBA, NotRules_Exhaustive8Bit) {
    for (auto& rule : getNotRules()) {
        SCOPED_TRACE("Rule: " + rule.name);
        verifyMBARule(rule, 8);
    }
}

// ============================================================================
// Static rules — random 32-bit verification
// ============================================================================

TEST(MBA, AllStaticRules_Random32Bit) {
    auto allRules = getAllStaticRules();
    EXPECT_GE(allRules.size(), 15u);

    for (auto& rule : allRules) {
        SCOPED_TRACE("Rule: " + rule.name);
        verifyMBARule32Random(rule, 5000);
    }
}

// ============================================================================
// MBAExpr tree evaluation
// ============================================================================

TEST(MBA, Expr_VarX) {
    auto x = MBAExpr::var(MBAOp::VarX);
    EXPECT_EQ(x->eval(42, 7), 42u);
}

TEST(MBA, Expr_VarY) {
    auto y = MBAExpr::var(MBAOp::VarY);
    EXPECT_EQ(y->eval(42, 7), 7u);
}

TEST(MBA, Expr_Constant) {
    auto c = MBAExpr::con(123);
    EXPECT_EQ(c->eval(42, 7), 123u);
}

TEST(MBA, Expr_AddTree) {
    // (x ^ y) + 2*(x & y) should equal x + y
    auto x = MBAExpr::var(MBAOp::VarX);
    auto y = MBAExpr::var(MBAOp::VarY);
    auto expr = MBAExpr::binary(MBAOp::Add,
        MBAExpr::binary(MBAOp::Xor, x, y),
        MBAExpr::binary(MBAOp::Mul, MBAExpr::con(2),
            MBAExpr::binary(MBAOp::And, x, y)));

    ObfRNG rng(42);
    for (int i = 0; i < 10000; i++) {
        uint32_t a = rng.next32();
        uint32_t b = rng.next32();
        EXPECT_EQ(expr->eval(a, b), a + b);
    }
}

TEST(MBA, Expr_ToString) {
    auto x = MBAExpr::var(MBAOp::VarX);
    auto y = MBAExpr::var(MBAOp::VarY);
    auto expr = MBAExpr::binary(MBAOp::Add, x, y);
    std::string s = expr->toString();
    EXPECT_NE(s.find("x"), std::string::npos);
    EXPECT_NE(s.find("y"), std::string::npos);
    EXPECT_NE(s.find("+"), std::string::npos);
}

// ============================================================================
// BitwiseBasis2 tests
// ============================================================================

TEST(MBA, BitwiseBasis_EvalBasis1Bit) {
    // func=0: always 0
    for (int x = 0; x <= 1; x++)
        for (int y = 0; y <= 1; y++)
            EXPECT_EQ(BitwiseBasis2::evalBasis(0, x, y), 0);

    // func=15: always 1
    for (int x = 0; x <= 1; x++)
        for (int y = 0; y <= 1; y++)
            EXPECT_EQ(BitwiseBasis2::evalBasis(15, x, y), 1);

    // func=6 (x^y): XOR
    EXPECT_EQ(BitwiseBasis2::evalBasis(6, 0, 0), 0);
    EXPECT_EQ(BitwiseBasis2::evalBasis(6, 0, 1), 1);
    EXPECT_EQ(BitwiseBasis2::evalBasis(6, 1, 0), 1);
    EXPECT_EQ(BitwiseBasis2::evalBasis(6, 1, 1), 0);
}

TEST(MBA, BitwiseBasis_EvalBasisFull) {
    ObfRNG rng(42);
    // func=6 should be XOR for full integers
    for (int i = 0; i < 1000; i++) {
        uint32_t x = rng.next32();
        uint32_t y = rng.next32();
        EXPECT_EQ(BitwiseBasis2::evalBasisFull(6, x, y), x ^ y);
    }

    // func=0: always 0
    EXPECT_EQ(BitwiseBasis2::evalBasisFull(0, 0xDEADBEEF, 0xCAFEBABE), 0u);

    // func=15: always -1
    EXPECT_EQ(BitwiseBasis2::evalBasisFull(15, 0xDEADBEEF, 0xCAFEBABE), 0xFFFFFFFF);
}

TEST(MBA, BitwiseBasis_AllFuncsAreDistinct) {
    // Each of the 16 basis functions should have a unique truth table
    std::set<int> truthTables;
    for (int func = 0; func < 16; func++) {
        int tt = 0;
        for (int i = 0; i < 4; i++) {
            int x = (i >> 1) & 1;
            int y = i & 1;
            tt |= (BitwiseBasis2::evalBasis(func, x, y) << i);
        }
        truthTables.insert(tt);
    }
    EXPECT_EQ(truthTables.size(), 16u);
}

// ============================================================================
// Truth-table matrix
// ============================================================================

TEST(MBA, TruthTableMatrix_Correct) {
    auto A = buildTruthTableMatrix2();

    // Row 0 (x=0, y=0): only func with bit0=1 should have A[0][func]=1
    for (int j = 0; j < 16; j++) {
        EXPECT_EQ(A[0][j], (j >> 0) & 1) << "A[0][" << j << "]";
    }

    // Row 3 (x=1, y=1): only func with bit3=1
    for (int j = 0; j < 16; j++) {
        EXPECT_EQ(A[3][j], (j >> 3) & 1) << "A[3][" << j << "]";
    }
}

// ============================================================================
// Linear MBA solver
// ============================================================================

TEST(MBA, LinearMBA_Add) {
    // Target: x + y at 1-bit = [0, 1, 1, 2]
    std::array<int, 4> tt = {0, 1, 1, 2};
    auto sol = solveLinearMBA(tt, 32);
    EXPECT_TRUE(sol.valid);

    // Verify the solution evaluates to x + y for 8-bit exhaustive
    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            uint32_t result = 0;
            for (int j = 0; j < 16; j++) {
                if (sol.coefficients[j] != 0) {
                    result += (uint32_t)sol.coefficients[j] * BitwiseBasis2::evalBasisFull(j, x, y);
                }
            }
            EXPECT_EQ(result, x + y)
                << "LinearMBA(Add) failed for x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, LinearMBA_Xor) {
    std::array<int, 4> tt = {0, 1, 1, 0};
    auto sol = solveLinearMBA(tt, 32);
    EXPECT_TRUE(sol.valid);

    ObfRNG rng(42);
    for (int i = 0; i < 10000; i++) {
        uint32_t x = rng.next32();
        uint32_t y = rng.next32();
        uint32_t result = 0;
        for (int j = 0; j < 16; j++) {
            if (sol.coefficients[j] != 0)
                result += (uint32_t)sol.coefficients[j] * BitwiseBasis2::evalBasisFull(j, x, y);
        }
        EXPECT_EQ(result, x ^ y);
        if (::testing::Test::HasFailure()) return;
    }
}

// ============================================================================
// Null-space
// ============================================================================

TEST(MBA, NullSpace_AllZero) {
    auto nullVecs = computeNullSpace(32);
    EXPECT_GE(nullVecs.size(), 3u);

    // Each null vector should produce zero for all inputs
    for (auto& nv : nullVecs) {
        for (uint32_t x = 0; x < 256; x++) {
            for (uint32_t y = 0; y < 256; y++) {
                uint32_t result = 0;
                for (int j = 0; j < 16; j++) {
                    result += (uint32_t)nv[j] * BitwiseBasis2::evalBasisFull(j, x, y);
                }
                EXPECT_EQ(result, 0u)
                    << "Null vector not zero for x=" << x << " y=" << y;
                if (::testing::Test::HasFailure()) return;
            }
        }
    }
}

// ============================================================================
// Dynamic MBA generation
// ============================================================================

TEST(MBA, DynamicMBA_Add_Exhaustive8Bit) {
    ObfRNG rng(42);
    auto mba = generateDynamicMBA(ArithOp::Add, rng);

    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(mba.eval(x, y), x + y)
                << "Dynamic MBA(Add) failed at x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, DynamicMBA_Sub_Exhaustive8Bit) {
    ObfRNG rng(42);
    auto mba = generateDynamicMBA(ArithOp::Sub, rng);

    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(mba.eval(x, y), x - y)
                << "Dynamic MBA(Sub) failed at x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, DynamicMBA_Xor_Exhaustive8Bit) {
    ObfRNG rng(42);
    auto mba = generateDynamicMBA(ArithOp::Xor, rng);

    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(mba.eval(x, y), x ^ y)
                << "Dynamic MBA(Xor) failed at x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, DynamicMBA_And_Exhaustive8Bit) {
    ObfRNG rng(42);
    auto mba = generateDynamicMBA(ArithOp::And, rng);

    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(mba.eval(x, y), x & y);
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, DynamicMBA_Or_Exhaustive8Bit) {
    ObfRNG rng(42);
    auto mba = generateDynamicMBA(ArithOp::Or, rng);

    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(mba.eval(x, y), x | y);
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, DynamicMBA_DifferentSeedsProduceDifferentExpressions) {
    std::set<std::string> expressions;
    for (uint64_t seed = 0; seed < 20; seed++) {
        ObfRNG rng(seed);
        auto mba = generateDynamicMBA(ArithOp::Add, rng);
        expressions.insert(mba.toString());
    }
    // Should produce many different expressions
    EXPECT_GT(expressions.size(), 5u);
}

TEST(MBA, DynamicMBA_Random32Bit) {
    ObfRNG rng(42);
    auto mba = generateDynamicMBA(ArithOp::Add, rng);

    ObfRNG testRng(999);
    for (int i = 0; i < 100000; i++) {
        uint32_t x = testRng.next32();
        uint32_t y = testRng.next32();
        EXPECT_EQ(mba.eval(x, y), x + y);
        if (::testing::Test::HasFailure()) return;
    }
}

// ============================================================================
// Zero identities
// ============================================================================

TEST(MBA, ZeroIdentities_Exhaustive8Bit) {
    auto identities = getZeroIdentities();
    EXPECT_GE(identities.size(), 5u);

    for (auto& id : identities) {
        SCOPED_TRACE("Identity: " + id.name);
        for (uint32_t x = 0; x < 256; x++) {
            for (uint32_t y = 0; y < 256; y++) {
                EXPECT_EQ(id.eval(x, y), 0u)
                    << "Zero identity '" << id.name
                    << "' not zero for x=" << x << " y=" << y;
                if (::testing::Test::HasFailure()) return;
            }
        }
    }
}

TEST(MBA, ZeroIdentities_Random32Bit) {
    auto identities = getZeroIdentities();
    ObfRNG rng(42);

    for (auto& id : identities) {
        SCOPED_TRACE("Identity: " + id.name);
        for (int i = 0; i < 10000; i++) {
            uint32_t x = rng.next32();
            uint32_t y = rng.next32();
            EXPECT_EQ(id.eval(x, y), 0u);
            if (::testing::Test::HasFailure()) return;
        }
    }
}

// ============================================================================
// Constant obfuscation
// ============================================================================

TEST(MBA, ConstantObfuscation_SpecificValues) {
    ObfRNG rng(42);
    uint32_t constants[] = {0, 1, 42, 255, 0xDEADBEEF, 0xCAFEBABE, 0xFFFFFFFF};

    for (uint32_t c : constants) {
        SCOPED_TRACE("Constant: " + std::to_string(c));
        auto mba = obfuscateConstant(c, rng, 32);

        // Should evaluate to c regardless of input
        ObfRNG testRng(999);
        for (int i = 0; i < 1000; i++) {
            uint32_t x = testRng.next32();
            uint32_t y = testRng.next32();
            EXPECT_EQ(mba.eval(x, y), c)
                << "Constant " << c << " obfuscation failed for x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, ConstantObfuscation_DifferentSeedsProduceDifferentExpressions) {
    std::set<std::string> expressions;
    for (uint64_t seed = 0; seed < 20; seed++) {
        ObfRNG rng(seed);
        auto mba = obfuscateConstant(42, rng, 32);
        expressions.insert(mba.toString());
    }
    EXPECT_GT(expressions.size(), 5u);
}

// ============================================================================
// Polynomial MBA
// ============================================================================

TEST(MBA, PolynomialMBA_CorrectnessAdd) {
    ObfRNG rng(42);
    auto poly = generatePolynomialMBA(ArithOp::Add, rng, 2);

    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(poly.eval(x, y), x + y)
                << "PolynomialMBA(Add) failed at x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

// ============================================================================
// Permutation Polynomial
// ============================================================================

TEST(MBA, PermutationPoly_IsPermutation) {
    ObfRNG rng(42);
    auto pp = PermutationPoly::generate(rng, 3, 8);

    EXPECT_TRUE(PermutationPoly::isPermutation(pp.coefficients, pp.bitWidth));

    // Verify it's actually a permutation (bijection) for 8-bit
    std::set<uint32_t> outputs;
    for (uint32_t x = 0; x < 256; x++) {
        outputs.insert(pp.eval(x));
    }
    EXPECT_EQ(outputs.size(), 256u) << "Not a permutation — duplicate outputs";
}

TEST(MBA, PermutationPoly_RivestCriteria) {
    // Manually construct a known permutation polynomial
    // p(x) = 1 + 3x + 2x^2 + 4x^3 (a1=3 is odd, a2+a4..=2 even, a3+a5..=4 even)
    std::vector<int64_t> coeffs = {1, 3, 2, 4};
    EXPECT_TRUE(PermutationPoly::isPermutation(coeffs, 8));

    // Not a permutation: a1=2 (even)
    std::vector<int64_t> bad = {1, 2, 0};
    EXPECT_FALSE(PermutationPoly::isPermutation(bad, 8));
}

// ============================================================================
// Mixed Radix MBA
// ============================================================================

TEST(MBA, MixedRadixMBA_Add) {
    ObfRNG rng(42);
    auto mba = MixedRadixMBA::generate(ArithOp::Add, rng);
    EXPECT_GE(mba.components.size(), 3u);

    // The CRT reconstruction should give x + y mod M (product of moduli)
    // For small values, this matches exact arithmetic
    for (uint32_t x = 0; x < 100; x++) {
        for (uint32_t y = 0; y < 100; y++) {
            uint32_t expected = x + y;
            uint32_t got = mba.eval(x, y);
            // Note: result is mod M (product of chosen primes)
            int64_t M = 1;
            for (auto& c : mba.components) M *= c.modulus;
            EXPECT_EQ(got, expected % (uint32_t)M)
                << "MixedRadix failed at x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

// ============================================================================
// Multi-round chaining
// ============================================================================

TEST(MBA, ChainMBA_Round1) {
    ObfRNG rng(42);
    auto expr = chainMBA(ArithOp::Add, rng, 1);
    ASSERT_NE(expr, nullptr);

    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(expr->eval(x, y), x + y)
                << "ChainMBA round 1 failed at x=" << x << " y=" << y;
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, ChainMBA_Round2) {
    ObfRNG rng(42);
    auto expr = chainMBA(ArithOp::Add, rng, 2);
    ASSERT_NE(expr, nullptr);

    // Round 2 adds complexity but should still be correct
    for (uint32_t x = 0; x < 256; x++) {
        for (uint32_t y = 0; y < 256; y++) {
            EXPECT_EQ(expr->eval(x, y), x + y);
            if (::testing::Test::HasFailure()) return;
        }
    }
}

TEST(MBA, ChainMBA_MoreComplexThanRound1) {
    ObfRNG rng(42);
    auto r1 = chainMBA(ArithOp::Add, rng, 1);
    ObfRNG rng2(42);
    auto r2 = chainMBA(ArithOp::Add, rng2, 2);

    ASSERT_NE(r1, nullptr);
    ASSERT_NE(r2, nullptr);

    // Round 2 should have more nodes
    EXPECT_GE(r2->nodeCount(), r1->nodeCount());
}

// ============================================================================
// Rule tree application
// ============================================================================

TEST(MBA, RuleTreeApplication_AddXorCarry) {
    auto rules = getAddRules();
    ASSERT_GE(rules.size(), 1u);

    auto& rule = rules[0]; // add_xor_carry
    auto x = MBAExpr::var(MBAOp::VarX);
    auto y = MBAExpr::var(MBAOp::VarY);
    auto expr = rule.apply(x, y);

    ASSERT_NE(expr, nullptr);

    for (uint32_t a = 0; a < 256; a++) {
        for (uint32_t b = 0; b < 256; b++) {
            EXPECT_EQ(expr->eval(a, b), a + b);
            if (::testing::Test::HasFailure()) return;
        }
    }
}
