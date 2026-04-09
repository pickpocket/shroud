#pragma once

#include "Utils.h"
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include <array>
#include <map>

namespace shroud {

// Represents an MBA expression as a tree of operations
enum class MBAOp {
    // Leaf nodes
    VarX, VarY, VarZ, Constant,
    // Unary
    Not, Neg,
    // Binary arithmetic
    Add, Sub, Mul,
    // Binary bitwise
    And, Or, Xor,
    // Derived
    Shl, Shr
};

struct MBAExpr {
    MBAOp op;
    int64_t constant = 0; // for Constant nodes
    std::shared_ptr<MBAExpr> left;
    std::shared_ptr<MBAExpr> right;

    // Evaluate the expression
    uint32_t eval(uint32_t x, uint32_t y = 0, uint32_t z = 0) const;

    // Pretty-print
    std::string toString() const;

    // Count nodes
    int nodeCount() const;

    // Static constructors
    static std::shared_ptr<MBAExpr> var(MBAOp v);
    static std::shared_ptr<MBAExpr> con(int64_t c);
    static std::shared_ptr<MBAExpr> unary(MBAOp op, std::shared_ptr<MBAExpr> child);
    static std::shared_ptr<MBAExpr> binary(MBAOp op, std::shared_ptr<MBAExpr> l, std::shared_ptr<MBAExpr> r);
};

// ============================================================================
// Static MBA substitution rules
// ============================================================================

enum class ArithOp {
    Add, Sub, Xor, And, Or, Not, Mul
};

// A substitution rule: replaces one operation with an MBA expression
struct MBARule {
    ArithOp target;
    std::string name;

    // For binary ops: takes (x, y) operands, returns MBA expression tree
    // For unary ops (Not): takes x, returns tree
    std::function<std::shared_ptr<MBAExpr>(std::shared_ptr<MBAExpr>, std::shared_ptr<MBAExpr>)> apply;

    // Direct evaluation for testing (bypass tree)
    std::function<uint32_t(uint32_t, uint32_t)> eval;
};

// 4a: Get all static substitution rules
std::vector<MBARule> getAddRules();
std::vector<MBARule> getSubRules();
std::vector<MBARule> getXorRules();
std::vector<MBARule> getAndRules();
std::vector<MBARule> getOrRules();
std::vector<MBARule> getNotRules();
std::vector<MBARule> getAllStaticRules();

// ============================================================================
// Dynamic MBA generation (truth-table / linear algebra)
// ============================================================================

// Bitwise basis for 2 variables: 16 functions from {0,1}^2 -> {0,1}
// Index by truth table value (4 bits)
struct BitwiseBasis2 {
    // All 16 bitwise functions of 2 variables
    // Index i has truth table [f(0,0), f(0,1), f(1,0), f(1,1)] encoded as i
    static constexpr int NUM_FUNCTIONS = 16;
    static constexpr int NUM_INPUTS = 4; // 2^2

    // Evaluate basis function 'func' at input (x, y) where x, y in {0, 1}
    static int evalBasis(int func, int x, int y);

    // Evaluate basis function for full-width integers
    static uint32_t evalBasisFull(int func, uint32_t x, uint32_t y);

    // Name of basis function
    static std::string basisName(int func);
};

// Truth-table matrix for 2-variable MBA
// Rows = input combinations, Cols = basis functions
// A[i][j] = basis_j(input_i)
using TruthTableMatrix = std::array<std::array<int, 16>, 4>;
TruthTableMatrix buildTruthTableMatrix2();

// Target truth table for common operations
std::array<int, 4> targetTruthTable2(ArithOp op);

// Solve linear MBA: find coefficients c[] such that
// sum(c[j] * basis_j(x, y)) == target(x, y) for all x, y in {0, 1}
// Returns coefficients mod 2^w (w = bit width)
struct LinearMBASolution {
    std::array<int64_t, 16> coefficients;
    bool valid;
};

LinearMBASolution solveLinearMBA(const std::array<int, 4>& target, int bitWidth = 32);

// Null-space vectors: MBA expressions that equal zero
std::vector<std::array<int64_t, 16>> computeNullSpace(int bitWidth = 32);

// Generate a random MBA expression for a target operation
// by adding null-space perturbations to a base solution
struct DynamicMBAResult {
    std::array<int64_t, 16> coefficients;
    int numNonZeroTerms;

    // Evaluate the expression
    uint32_t eval(uint32_t x, uint32_t y) const;

    // Pretty print
    std::string toString() const;
};

DynamicMBAResult generateDynamicMBA(ArithOp target, ObfRNG& rng,
                                     int minTerms = 4, int maxTerms = 12,
                                     int bitWidth = 32);

// ============================================================================
// MBA Identity Insertions (zero-expressions)
// ============================================================================

struct MBAIdentity {
    std::string name;
    std::function<uint32_t(uint32_t, uint32_t)> eval; // should always return 0
};

std::vector<MBAIdentity> getZeroIdentities();

// ============================================================================
// Constant Obfuscation via MBA
// ============================================================================

// Generate an MBA expression that evaluates to the given constant
// for all values of the auxiliary variables
DynamicMBAResult obfuscateConstant(uint32_t constant, ObfRNG& rng,
                                    int bitWidth = 32);

// ============================================================================
// Polynomial MBA (non-linear, quantum-resistant)
// ============================================================================

struct PolynomialMBA {
    // Products of basis functions with coefficients
    struct Term {
        int64_t coefficient;
        std::vector<int> basisIndices; // multiply these basis functions
    };
    std::vector<Term> terms;

    uint32_t eval(uint32_t x, uint32_t y) const;
    std::string toString() const;
};

PolynomialMBA generatePolynomialMBA(ArithOp target, ObfRNG& rng, int degree = 2);

// ============================================================================
// Permutation Polynomial MBA
// ============================================================================

struct PermutationPoly {
    std::vector<int64_t> coefficients; // a0 + a1*x + a2*x^2 + ...
    int bitWidth;

    uint32_t eval(uint32_t x) const;
    static PermutationPoly generate(ObfRNG& rng, int degree = 3, int bitWidth = 8);
    static bool isPermutation(const std::vector<int64_t>& coeffs, int bitWidth);

    // Compute inverse permutation polynomial
    PermutationPoly inverse() const;
};

// ============================================================================
// Mixed Radix MBA (CRT-based)
// ============================================================================

struct MixedRadixMBA {
    struct RadixComponent {
        int64_t modulus;
        std::function<uint32_t(uint32_t, uint32_t)> compute; // computes result mod modulus
    };
    std::vector<RadixComponent> components;

    uint32_t eval(uint32_t x, uint32_t y) const; // CRT reconstruction
    static MixedRadixMBA generate(ArithOp target, ObfRNG& rng);
};

// ============================================================================
// Multi-round chaining
// ============================================================================

// Apply MBA substitution recursively for 'rounds' levels
std::shared_ptr<MBAExpr> chainMBA(ArithOp target, ObfRNG& rng, int rounds = 2);

} // namespace shroud
