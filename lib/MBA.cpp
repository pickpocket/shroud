#include "Shroud/MBA.h"
#include <sstream>
#include <cstring>

namespace shroud {

// ============================================================================
// MBAExpr implementation
// ============================================================================

uint32_t MBAExpr::eval(uint32_t x, uint32_t y, uint32_t z) const {
    switch (op) {
    case MBAOp::VarX: return x;
    case MBAOp::VarY: return y;
    case MBAOp::VarZ: return z;
    case MBAOp::Constant: return (uint32_t)constant;
    case MBAOp::Not: return ~left->eval(x, y, z);
    case MBAOp::Neg: return (uint32_t)(-(int32_t)left->eval(x, y, z));
    case MBAOp::Add: return left->eval(x, y, z) + right->eval(x, y, z);
    case MBAOp::Sub: return left->eval(x, y, z) - right->eval(x, y, z);
    case MBAOp::Mul: return left->eval(x, y, z) * right->eval(x, y, z);
    case MBAOp::And: return left->eval(x, y, z) & right->eval(x, y, z);
    case MBAOp::Or:  return left->eval(x, y, z) | right->eval(x, y, z);
    case MBAOp::Xor: return left->eval(x, y, z) ^ right->eval(x, y, z);
    case MBAOp::Shl: return left->eval(x, y, z) << (right->eval(x, y, z) & 31);
    case MBAOp::Shr: return left->eval(x, y, z) >> (right->eval(x, y, z) & 31);
    }
    return 0;
}

std::string MBAExpr::toString() const {
    switch (op) {
    case MBAOp::VarX: return "x";
    case MBAOp::VarY: return "y";
    case MBAOp::VarZ: return "z";
    case MBAOp::Constant: return std::to_string(constant);
    case MBAOp::Not: return "(~" + left->toString() + ")";
    case MBAOp::Neg: return "(-" + left->toString() + ")";
    case MBAOp::Add: return "(" + left->toString() + " + " + right->toString() + ")";
    case MBAOp::Sub: return "(" + left->toString() + " - " + right->toString() + ")";
    case MBAOp::Mul: return "(" + left->toString() + " * " + right->toString() + ")";
    case MBAOp::And: return "(" + left->toString() + " & " + right->toString() + ")";
    case MBAOp::Or:  return "(" + left->toString() + " | " + right->toString() + ")";
    case MBAOp::Xor: return "(" + left->toString() + " ^ " + right->toString() + ")";
    case MBAOp::Shl: return "(" + left->toString() + " << " + right->toString() + ")";
    case MBAOp::Shr: return "(" + left->toString() + " >> " + right->toString() + ")";
    }
    return "?";
}

int MBAExpr::nodeCount() const {
    int count = 1;
    if (left) count += left->nodeCount();
    if (right) count += right->nodeCount();
    return count;
}

std::shared_ptr<MBAExpr> MBAExpr::var(MBAOp v) {
    auto e = std::make_shared<MBAExpr>();
    e->op = v;
    return e;
}

std::shared_ptr<MBAExpr> MBAExpr::con(int64_t c) {
    auto e = std::make_shared<MBAExpr>();
    e->op = MBAOp::Constant;
    e->constant = c;
    return e;
}

std::shared_ptr<MBAExpr> MBAExpr::unary(MBAOp op, std::shared_ptr<MBAExpr> child) {
    auto e = std::make_shared<MBAExpr>();
    e->op = op;
    e->left = child;
    return e;
}

std::shared_ptr<MBAExpr> MBAExpr::binary(MBAOp op, std::shared_ptr<MBAExpr> l, std::shared_ptr<MBAExpr> r) {
    auto e = std::make_shared<MBAExpr>();
    e->op = op;
    e->left = l;
    e->right = r;
    return e;
}

// ============================================================================
// 4a: Static substitution rules
// ============================================================================

// Helper to make expression builders shorter
using E = MBAExpr;

std::vector<MBARule> getAddRules() {
    return {
        {ArithOp::Add, "add_xor_carry",
            [](auto x, auto y) { return E::binary(MBAOp::Add, E::binary(MBAOp::Xor, x, y), E::binary(MBAOp::Mul, E::con(2), E::binary(MBAOp::And, x, y))); },
            [](uint32_t x, uint32_t y) { return (x ^ y) + 2 * (x & y); }},

        {ArithOp::Add, "add_or_and",
            [](auto x, auto y) { return E::binary(MBAOp::Add, E::binary(MBAOp::Or, x, y), E::binary(MBAOp::And, x, y)); },
            [](uint32_t x, uint32_t y) { return (x | y) + (x & y); }},

        {ArithOp::Add, "add_complement",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Sub, x, E::unary(MBAOp::Not, y)), E::con(1)); },
            [](uint32_t x, uint32_t y) { return (x - ~y) - 1; }},

        {ArithOp::Add, "add_double_or",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Mul, E::con(2), E::binary(MBAOp::Or, x, y)), E::binary(MBAOp::Xor, x, y)); },
            [](uint32_t x, uint32_t y) { return 2 * (x | y) - (x ^ y); }},

        {ArithOp::Add, "add_full_decomp",
            [](auto x, auto y) {
                auto x_and_not_y = E::binary(MBAOp::And, x, E::unary(MBAOp::Not, y));
                auto y_and_not_x = E::binary(MBAOp::And, y, E::unary(MBAOp::Not, x));
                auto two_and = E::binary(MBAOp::Mul, E::con(2), E::binary(MBAOp::And, x, y));
                return E::binary(MBAOp::Add, E::binary(MBAOp::Add, x_and_not_y, y_and_not_x), two_and);
            },
            [](uint32_t x, uint32_t y) { return (x & ~y) + (y & ~x) + 2 * (x & y); }},
    };
}

std::vector<MBARule> getSubRules() {
    return {
        {ArithOp::Sub, "sub_xor_borrow",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Xor, x, y), E::binary(MBAOp::Mul, E::con(2), E::binary(MBAOp::And, E::unary(MBAOp::Not, x), y))); },
            [](uint32_t x, uint32_t y) { return (x ^ y) - 2 * (~x & y); }},

        {ArithOp::Sub, "sub_twos_complement",
            [](auto x, auto y) { return E::binary(MBAOp::Add, E::binary(MBAOp::Add, x, E::unary(MBAOp::Not, y)), E::con(1)); },
            [](uint32_t x, uint32_t y) { return (x + ~y) + 1; }},

        {ArithOp::Sub, "sub_set_diff",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::And, x, E::unary(MBAOp::Not, y)), E::binary(MBAOp::And, E::unary(MBAOp::Not, x), y)); },
            [](uint32_t x, uint32_t y) { return (x & ~y) - (~x & y); }},

        {ArithOp::Sub, "sub_double_mask",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Mul, E::con(2), E::binary(MBAOp::And, x, E::unary(MBAOp::Not, y))), E::binary(MBAOp::Xor, x, y)); },
            [](uint32_t x, uint32_t y) { return 2 * (x & ~y) - (x ^ y); }},
    };
}

std::vector<MBARule> getXorRules() {
    return {
        {ArithOp::Xor, "xor_or_minus_and",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Or, x, y), E::binary(MBAOp::And, x, y)); },
            [](uint32_t x, uint32_t y) { return (x | y) - (x & y); }},

        {ArithOp::Xor, "xor_symmetric_diff",
            [](auto x, auto y) { return E::binary(MBAOp::Or, E::binary(MBAOp::And, x, E::unary(MBAOp::Not, y)), E::binary(MBAOp::And, E::unary(MBAOp::Not, x), y)); },
            [](uint32_t x, uint32_t y) { return (x & ~y) | (~x & y); }},

        {ArithOp::Xor, "xor_arith_decomp",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Add, x, y), E::binary(MBAOp::Mul, E::con(2), E::binary(MBAOp::And, x, y))); },
            [](uint32_t x, uint32_t y) { return (x + y) - 2 * (x & y); }},

        {ArithOp::Xor, "xor_partition_add",
            [](auto x, auto y) { return E::binary(MBAOp::Add, E::binary(MBAOp::And, E::unary(MBAOp::Not, x), y), E::binary(MBAOp::And, x, E::unary(MBAOp::Not, y))); },
            [](uint32_t x, uint32_t y) { return (~x & y) + (x & ~y); }},
    };
}

std::vector<MBARule> getAndRules() {
    return {
        {ArithOp::And, "and_arith",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Add, x, y), E::binary(MBAOp::Or, x, y)); },
            [](uint32_t x, uint32_t y) { return (x + y) - (x | y); }},

        {ArithOp::And, "and_demorgan",
            [](auto x, auto y) { return E::unary(MBAOp::Not, E::binary(MBAOp::Or, E::unary(MBAOp::Not, x), E::unary(MBAOp::Not, y))); },
            [](uint32_t x, uint32_t y) { return ~(~x | ~y); }},

        {ArithOp::And, "and_not_or_sub",
            [](auto x, auto y) {
                // x & y = ~(~x | ~y) — De Morgan, then re-express:
                // x & y = x - (x & ~y) = x - (x ^ (x & y))
                // Simpler: x & y = x + y - (x | y) [already covered]
                // Use: x & y = (x | y) - (x ^ y) [partition identity]
                return E::binary(MBAOp::Sub, E::binary(MBAOp::Or, x, y), E::binary(MBAOp::Xor, x, y));
            },
            [](uint32_t x, uint32_t y) { return (x | y) - (x ^ y); }},
    };
}

std::vector<MBARule> getOrRules() {
    return {
        {ArithOp::Or, "or_arith",
            [](auto x, auto y) { return E::binary(MBAOp::Sub, E::binary(MBAOp::Add, x, y), E::binary(MBAOp::And, x, y)); },
            [](uint32_t x, uint32_t y) { return (x + y) - (x & y); }},

        {ArithOp::Or, "or_partition",
            [](auto x, auto y) { return E::binary(MBAOp::Add, E::binary(MBAOp::Xor, x, y), E::binary(MBAOp::And, x, y)); },
            [](uint32_t x, uint32_t y) { return (x ^ y) + (x & y); }},

        {ArithOp::Or, "or_demorgan",
            [](auto x, auto y) { return E::unary(MBAOp::Not, E::binary(MBAOp::And, E::unary(MBAOp::Not, x), E::unary(MBAOp::Not, y))); },
            [](uint32_t x, uint32_t y) { return ~(~x & ~y); }},
    };
}

std::vector<MBARule> getNotRules() {
    return {
        {ArithOp::Not, "not_neg",
            [](auto x, auto) { return E::binary(MBAOp::Sub, E::unary(MBAOp::Neg, x), E::con(1)); },
            [](uint32_t x, uint32_t) { return (uint32_t)(-(int32_t)x) - 1; }},

        {ArithOp::Not, "not_neg_alt",
            [](auto x, auto) { return E::unary(MBAOp::Neg, E::binary(MBAOp::Add, x, E::con(1))); },
            [](uint32_t x, uint32_t) { return (uint32_t)(-(int32_t)(x + 1)); }},

        {ArithOp::Not, "not_xor_allones",
            [](auto x, auto) { return E::binary(MBAOp::Xor, x, E::con(-1LL)); },
            [](uint32_t x, uint32_t) { return x ^ 0xFFFFFFFF; }},
    };
}

std::vector<MBARule> getAllStaticRules() {
    std::vector<MBARule> all;
    auto append = [&all](std::vector<MBARule>&& v) {
        all.insert(all.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
    };
    append(getAddRules());
    append(getSubRules());
    append(getXorRules());
    append(getAndRules());
    append(getOrRules());
    append(getNotRules());
    return all;
}

// ============================================================================
// BitwiseBasis2 implementation
// ============================================================================

int BitwiseBasis2::evalBasis(int func, int x, int y) {
    // func is a 4-bit truth table: bit 0 = f(0,0), bit 1 = f(0,1), bit 2 = f(1,0), bit 3 = f(1,1)
    int input = (x << 1) | y; // 0=00, 1=01, 2=10, 3=11
    return (func >> input) & 1;
}

uint32_t BitwiseBasis2::evalBasisFull(int func, uint32_t x, uint32_t y) {
    // Evaluate basis function bitwise for full 32-bit values
    // func encodes: bit0 = f(0,0), bit1 = f(0,1), bit2 = f(1,0), bit3 = f(1,1)
    uint32_t result = 0;
    if (func & 1) result |= ~x & ~y;  // f(0,0)=1
    if (func & 2) result |= ~x & y;   // f(0,1)=1
    if (func & 4) result |= x & ~y;   // f(1,0)=1
    if (func & 8) result |= x & y;    // f(1,1)=1
    return result;
}

std::string BitwiseBasis2::basisName(int func) {
    // Basis function naming: e0..e15 with truth table interpretation
    // func bit pattern: [f(0,0), f(0,1), f(1,0), f(1,1)] with input = (x<<1)|y
    //  0=0000:0  1=0001:~x&~y  2=0010:~x&y  3=0011:~x
    //  4=0100:x&~y  5=0101:~y  6=0110:x^y  7=0111:x|y
    //  8=1000:~(x|y)  9=1001:XNOR  10=1010:y  11=1011:x|~y... (simplified)
    //  12=1100:x  13=1101:~x|y  14=1110:NAND  15=1111:-1

    if (func >= 0 && func < 16) return "e" + std::to_string(func);
    return "?";
}

// ============================================================================
// Truth-table matrix construction
// ============================================================================

TruthTableMatrix buildTruthTableMatrix2() {
    TruthTableMatrix A;
    // Row i = input combination i: (x, y) where i = x*2 + y
    // Col j = basis function j evaluated at input i
    for (int i = 0; i < 4; i++) {
        int x = (i >> 1) & 1;
        int y = i & 1;
        for (int j = 0; j < 16; j++) {
            A[i][j] = BitwiseBasis2::evalBasis(j, x, y);
        }
    }
    return A;
}

std::array<int, 4> targetTruthTable2(ArithOp op) {
    // Compute target function at 1-bit inputs
    // The key insight: for w-bit integers, if f(x,y) at 1-bit equals g(x,y) at 1-bit,
    // then the MBA expression works for all w-bit values.
    std::array<int, 4> tt;
    for (int i = 0; i < 4; i++) {
        int x = (i >> 1) & 1;
        int y = i & 1;
        switch (op) {
        case ArithOp::Add: tt[i] = (x + y) & 1; break;  // 1-bit add = XOR
        case ArithOp::Sub: tt[i] = (x - y) & 1; break;  // 1-bit sub = XOR
        case ArithOp::Xor: tt[i] = x ^ y; break;
        case ArithOp::And: tt[i] = x & y; break;
        case ArithOp::Or:  tt[i] = x | y; break;
        case ArithOp::Not: tt[i] = (~x) & 1; break;
        case ArithOp::Mul: tt[i] = (x * y) & 1; break;
        }
    }
    return tt;
}

// ============================================================================
// Linear MBA solver
// ============================================================================

LinearMBASolution solveLinearMBA(const std::array<int, 4>& target, int bitWidth) {
    LinearMBASolution sol;
    sol.valid = false;
    std::fill(sol.coefficients.begin(), sol.coefficients.end(), 0);

    (void)bitWidth; // used for mod computation
    uint64_t mod = 1ULL << bitWidth;

    // Simple approach: since we have 4 equations and 16 unknowns,
    // the system is heavily underdetermined. We can use a greedy approach
    // to find a solution.

    // For the 1-bit truth table, we need:
    // sum(c_j * A[i][j]) ≡ target[i] (mod 2) for i = 0..3
    // This is a system over GF(2) first, then we lift to Z/2^w.

    // Simple greedy: try to set coefficients to satisfy each equation
    // Since there are many free variables, we can find a solution easily.

    // Actually, the simplest valid approach:
    // For each row i, find any column j where A[i][j] = 1 and set c_j = target[i]
    // This works because we have way more columns than rows.

    // Better approach: use the direct construction.
    // For target = x+y at 1-bit = XOR:
    //   truth table = [0, 1, 1, 0] = basis function 6 (XOR)
    // So c_6 = 1, all others 0 is a trivial solution.
    // But for x+y at full width, we need: (x^y) + 2*(x&y)
    //   = e_6 + 2*e_1 (where e_1 = x&y has truth table 1000, wait...)

    // Let me reconsider. The fundamental theorem says:
    // If sum(c_j * e_j(b)) = f(b) for all b in {0,1}^n, then
    // sum(c_j * e_j(x)) = f(x) for all x in Z/2^w
    // where e_j are the bitwise basis functions.

    // For x + y:
    // At (0,0): 0 = sum(c_j * e_j(0,0))
    // At (0,1): 1 = sum(c_j * e_j(0,1))
    // At (1,0): 1 = sum(c_j * e_j(1,0))
    // At (1,1): 0 = sum(c_j * e_j(1,1))  -- note: 1+1=2, 2 mod 2 = 0

    // Wait, but x+y at 1-bit is not just mod 2! The fundamental theorem
    // uses exact integer arithmetic at 1-bit, then the result holds modulo 2^w.
    // At 1 bit: 0+0=0, 0+1=1, 1+0=1, 1+1=2
    // So target = [0, 1, 1, 2]

    // Let me redo with correct 1-bit arithmetic (NOT mod 2):
    std::array<int, 4> exact_target = target;
    // The caller should pass the exact 1-bit result, not modded.

    // Solve: A * c = target (over integers, result taken mod 2^w)
    // A is 4x16, target is 4x1
    // This is a system with 12 free variables.

    // Gaussian elimination over integers mod 2^w:
    // For simplicity, use a direct construction.

    // Direct construction: since each basis function e_j has a unique 4-bit truth table,
    // and there are exactly 16 of them (one per truth table), we can express any
    // target as a single basis function if target ∈ {0,1}^4.
    // For multi-valued targets (like x+y = [0,1,1,2]), we need combinations.

    // Simple approach: decompose target into sum of basis functions
    // target = [0, 1, 1, 2]
    // e_6 (XOR) = [0, 1, 1, 0]  -> contributes 1 to positions 1,2
    // e_1 (AND at pos (1,1)) = [0, 0, 0, 1] in my encoding... wait

    // Let me reclarify the encoding. With input = (x<<1)|y:
    // e_j has truth table j (4 bits): bit 0 = e_j(0,0), bit 1 = e_j(0,1), etc.

    // e_0  = 0000 = [0,0,0,0]
    // e_1  = 0001 = [1,0,0,0] (only (0,0) is 1)
    // ...
    // e_8  = 1000 = [0,0,0,1] (only (1,1) is 1)

    // Hmm wait, bit 0 = f(input_0) where input_0 has x=0,y=0
    // func=1 means bit 0 is set, so e_1(0,0)=1, rest=0
    // That means e_1 evaluates to ~x & ~y for full integers.

    // For target x+y = [0, 1, 1, 2] at 1-bit exact:
    // We need: c_j such that sum(c_j * e_j_tt) = [0, 1, 1, 2]
    // e_2 = [0, 1, 0, 0] -> ~x & y at (0,1) only
    // e_4 = [0, 0, 1, 0] -> x & ~y at (1,0) only
    // e_8 = [0, 0, 0, 1] -> x & y at (1,1) only
    // So: 1*e_2 + 1*e_4 + 2*e_8 = [0, 1, 1, 2] ✓
    // Full expression: (~x & y) + (x & ~y) + 2*(x & y) = x + y ✓

    // General solution: for any target vector t:
    // c_1 = t[0], c_2 = t[1], c_4 = t[2], c_8 = t[3], rest = 0
    // Because e_1 = [1,0,0,0], e_2 = [0,1,0,0], e_4 = [0,0,1,0], e_8 = [0,0,0,1]

    sol.coefficients[1] = exact_target[0];  // e_1 = at (0,0)
    sol.coefficients[2] = exact_target[1];  // e_2 = at (0,1)
    sol.coefficients[4] = exact_target[2];  // e_4 = at (1,0)
    sol.coefficients[8] = exact_target[3];  // e_8 = at (1,1)

    // Apply mod 2^w to coefficients
    for (auto& c : sol.coefficients) {
        c = ((c % (int64_t)mod) + (int64_t)mod) % (int64_t)mod;
    }

    sol.valid = true;
    return sol;
}

// ============================================================================
// Null-space computation
// ============================================================================

std::vector<std::array<int64_t, 16>> computeNullSpace(int bitWidth) {
    // Find null vectors empirically: c[] such that sum(c[j] * e_j(x,y)) = 0 for all x,y
    // Basis: e_j = evalBasisFull(j, x, y) is the j-th bitwise function of 2 variables
    // e_1=~x&~y, e_2=~x&y, e_3=~x, e_4=x&~y, e_5=~y, e_8=x&y, e_12=x, etc.

    auto verify = [](const std::array<int64_t, 16>& vec) -> bool {
        // Exhaustive 8-bit check
        for (uint32_t x = 0; x < 256; x++) {
            for (uint32_t y = 0; y < 256; y++) {
                uint32_t sum = 0;
                for (int j = 0; j < 16; j++)
                    sum += (uint32_t)vec[j] * BitwiseBasis2::evalBasisFull(j, x, y);
                if (sum != 0) return false;
            }
        }
        return true;
    };

    std::vector<std::array<int64_t, 16>> nullVecs;
    uint64_t mod = 1ULL << bitWidth;

    // Enumerate relations e_i + e_j - e_k = 0
    for (int i = 1; i < 16; i++) {
        for (int j = i; j < 16; j++) {
            for (int k = 1; k < 16; k++) {
                if (k == i || (k == j && i == j)) continue;
                std::array<int64_t, 16> vec = {};
                vec[i] += 1;
                vec[j] += 1;
                vec[k] -= 1;
                if (verify(vec)) {
                    for (auto& c : vec)
                        c = ((c % (int64_t)mod) + (int64_t)mod) % (int64_t)mod;
                    nullVecs.push_back(vec);
                    if (nullVecs.size() >= 12) goto done;
                }
            }
        }
    }
done:
    return nullVecs;
}

// ============================================================================
// Dynamic MBA generation
// ============================================================================

uint32_t DynamicMBAResult::eval(uint32_t x, uint32_t y) const {
    uint32_t result = 0;
    for (int j = 0; j < 16; j++) {
        if (coefficients[j] != 0) {
            result += (uint32_t)coefficients[j] * BitwiseBasis2::evalBasisFull(j, x, y);
        }
    }
    return result;
}

std::string DynamicMBAResult::toString() const {
    std::ostringstream ss;
    bool first = true;
    for (int j = 0; j < 16; j++) {
        if (coefficients[j] != 0) {
            if (!first) ss << " + ";
            ss << coefficients[j] << "*" << BitwiseBasis2::basisName(j);
            first = false;
        }
    }
    return ss.str();
}

DynamicMBAResult generateDynamicMBA(ArithOp target, ObfRNG& rng,
                                     int /*minTerms*/, int /*maxTerms*/,
                                     int bitWidth) {
    // Get exact 1-bit target (NOT mod 2!)
    std::array<int, 4> tt;
    for (int i = 0; i < 4; i++) {
        int x = (i >> 1) & 1;
        int y = i & 1;
        switch (target) {
        case ArithOp::Add: tt[i] = x + y; break;
        case ArithOp::Sub: tt[i] = x - y; break; // can be negative but mod 2^w wraps
        case ArithOp::Xor: tt[i] = x ^ y; break;
        case ArithOp::And: tt[i] = x & y; break;
        case ArithOp::Or:  tt[i] = x | y; break;
        case ArithOp::Not: tt[i] = (~x) & 1; break; // Only for 1-bit
        case ArithOp::Mul: tt[i] = x * y; break;
        }
    }

    auto baseSol = solveLinearMBA(tt, bitWidth);
    auto nullVecs = computeNullSpace(bitWidth);
    uint64_t mod = 1ULL << bitWidth;

    // Add random null-space perturbations
    DynamicMBAResult result;
    result.coefficients = baseSol.coefficients;

    // Perturbation: 6-12 rounds with random scales
    // Targets ~2M lines of IR output for the crackme
    int numPerturbations = rng.nextInRange(6, 12);
    for (int i = 0; i < numPerturbations; i++) {
        int vecIdx = rng.nextInRange(0, (uint32_t)nullVecs.size() - 1);
        int64_t scale = (int64_t)rng.nextInRange(1, 150);
        if (rng.nextBool()) scale = -scale;

        for (int j = 0; j < 16; j++) {
            result.coefficients[j] = ((result.coefficients[j] + scale * nullVecs[vecIdx][j]) % (int64_t)mod + (int64_t)mod) % (int64_t)mod;
        }
    }

    result.numNonZeroTerms = 0;
    for (auto c : result.coefficients)
        if (c != 0) result.numNonZeroTerms++;

    return result;
}

// ============================================================================
// MBA Identity Insertions (zero-expressions)
// ============================================================================

std::vector<MBAIdentity> getZeroIdentities() {
    return {
        {"and_partition_zero",
            [](uint32_t x, uint32_t y) -> uint32_t { return (x & y) + (x & ~y) - x; }},

        {"carry_zero",
            [](uint32_t x, uint32_t y) -> uint32_t { return (x ^ y) + 2 * (x & y) - (x + y); }},

        {"or_partition_zero",
            [](uint32_t x, uint32_t y) -> uint32_t { return (x | y) + (x & y) - (x + y); }},

        {"xor_add_zero",
            [](uint32_t x, uint32_t y) -> uint32_t { return (x ^ y) + (x & y) - (x | y); }},

        {"demorgan_zero",
            [](uint32_t x, uint32_t y) -> uint32_t { return ~(x & y) - (~x | ~y); }},

        {"complement_zero",
            [](uint32_t x, uint32_t) -> uint32_t { return ~x + x + 1; }}, // ~x + x = -1, -1 + 1 = 0

        {"or_xor_and_zero",
            [](uint32_t x, uint32_t y) -> uint32_t { return (x | y) - (x ^ y) - (x & y); }},
    };
}

// ============================================================================
// Constant Obfuscation
// ============================================================================

DynamicMBAResult obfuscateConstant(uint32_t constant, ObfRNG& rng, int bitWidth) {
    // Target truth table: constant at 1-bit level
    // For a constant C, the 1-bit truth table is [C&1, C&1, C&1, C&1]
    // because the constant is the same regardless of input.

    std::array<int, 4> tt;
    for (int i = 0; i < 4; i++) {
        tt[i] = constant & 1; // Only the low bit matters at 1-bit level
    }

    // But this only gives the low bit! For full constant, we need a different approach.
    // Use the fact that all-constant functions can be expressed as:
    // C = C * e_15(x, y) (since e_15 = -1 for all inputs, so C * (-1) = -C... no)
    // Actually e_15 = all bits set = 0xFFFFFFFF = -1 in unsigned.
    // So C = -C * e_15 -> nah.
    // C = C & e_15 -> yes! C AND -1 = C.
    // But that's trivial. Better: express C using MBA with dummy vars.

    // Direct approach: C = (C+1)*e_15 + 1... wait, (C+1)*(-1) + 1 = -C-1+1 = -C, nope.

    // Actually for constant obfuscation with 2 dummy variables:
    // We want f(x,y) = C for all x, y.
    // At 1-bit: [C&1, C&1, C&1, C&1] (yes the low bit, for all inputs)
    // But the full value needs the higher bits too.

    // The trick: all 4 basis functions e_1, e_2, e_4, e_8 partition the bits.
    // e_1 + e_2 + e_4 + e_8 = -1 (all bits set) for full integers.
    // So: C = C * (e_1 + e_2 + e_4 + e_8) / (-1)... still messy.

    // Simpler: C * 1 = C. And 1 is... not directly a basis function.
    // e_15 evaluates to 0xFFFFFFFF = -1.
    // So: C = C * (-e_15) = -C * e_15 -> nah, -C * (-1) = C ✓
    // Actually: C = (-C) * e_15. Check: (-C) * (0xFFFFFFFF) = (-C) * (-1) = C ✓

    DynamicMBAResult result;
    std::fill(result.coefficients.begin(), result.coefficients.end(), 0);

    uint64_t mod = 1ULL << bitWidth;
    uint64_t negC = (mod - constant) % mod;

    result.coefficients[15] = (int64_t)negC; // -C * e_15 = C

    // Now add null-space perturbations for obfuscation
    auto nullVecs = computeNullSpace(bitWidth);
    int numPerturbations = rng.nextInRange(8, 14);
    for (int i = 0; i < numPerturbations; i++) {
        int vecIdx = rng.nextInRange(0, (uint32_t)nullVecs.size() - 1);
        int64_t scale = (int64_t)rng.nextInRange(1, 31);
        if (rng.nextBool()) scale = -scale;

        for (int j = 0; j < 16; j++) {
            result.coefficients[j] = ((result.coefficients[j] + scale * nullVecs[vecIdx][j]) % (int64_t)mod + (int64_t)mod) % (int64_t)mod;
        }
    }

    result.numNonZeroTerms = 0;
    for (auto c : result.coefficients)
        if (c != 0) result.numNonZeroTerms++;

    return result;
}

// ============================================================================
// Polynomial MBA
// ============================================================================

uint32_t PolynomialMBA::eval(uint32_t x, uint32_t y) const {
    uint32_t result = 0;
    for (auto& term : terms) {
        uint32_t prod = (uint32_t)term.coefficient;
        for (int idx : term.basisIndices) {
            prod *= BitwiseBasis2::evalBasisFull(idx, x, y);
        }
        result += prod;
    }
    return result;
}

std::string PolynomialMBA::toString() const {
    std::ostringstream ss;
    bool first = true;
    for (auto& term : terms) {
        if (!first) ss << " + ";
        ss << term.coefficient;
        for (int idx : term.basisIndices) {
            ss << "*" << BitwiseBasis2::basisName(idx);
        }
        first = false;
    }
    return ss.str();
}

PolynomialMBA generatePolynomialMBA(ArithOp target, ObfRNG& rng, int /*degree*/) {
    // Start from a linear MBA and introduce nonlinear terms
    auto linear = generateDynamicMBA(target, rng);

    PolynomialMBA poly;

    // Add linear terms
    for (int j = 0; j < 16; j++) {
        if (linear.coefficients[j] != 0) {
            poly.terms.push_back({linear.coefficients[j], {j}});
        }
    }

    // Add degree-2+ zero-terms (products that sum to zero)
    // e_j * e_k and e_j * e_k with opposite signs
    int numNonlinearPairs = rng.nextInRange(1, 3);
    for (int i = 0; i < numNonlinearPairs; i++) {
        int j = rng.nextInRange(1, 14);
        int k = rng.nextInRange(1, 14);
        int64_t coeff = (int64_t)rng.nextInRange(1, 10);
        poly.terms.push_back({coeff, {j, k}});
        poly.terms.push_back({-coeff, {j, k}}); // Cancels out
    }

    return poly;
}

// ============================================================================
// Permutation Polynomial
// ============================================================================

uint32_t PermutationPoly::eval(uint32_t x) const {
    uint64_t mod = 1ULL << bitWidth;
    uint64_t result = 0;
    uint64_t xpow = 1;
    for (size_t i = 0; i < coefficients.size(); i++) {
        result = (result + (uint64_t)coefficients[i] * xpow) % mod;
        xpow = (xpow * x) % mod;
    }
    return (uint32_t)result;
}

bool PermutationPoly::isPermutation(const std::vector<int64_t>& coeffs, int /*bitWidth*/) {
    // Rivest's characterization for Z/2^w:
    // p(x) = a0 + a1*x + a2*x^2 + ... is a permutation iff:
    // 1. a1 is odd
    // 2. a2 + a4 + a6 + ... is even
    // 3. a3 + a5 + a7 + ... is even
    if (coeffs.size() < 2) return false;
    if (coeffs[1] % 2 == 0) return false; // a1 must be odd

    int64_t sumEven = 0, sumOdd = 0;
    for (size_t i = 2; i < coeffs.size(); i++) {
        if (i % 2 == 0) sumEven += coeffs[i];
        else sumOdd += coeffs[i];
    }
    return (sumEven % 2 == 0) && (sumOdd % 2 == 0);
}

PermutationPoly PermutationPoly::generate(ObfRNG& rng, int degree, int bw) {
    PermutationPoly p;
    p.bitWidth = bw;
    p.coefficients.resize(degree + 1);

    // a0 = random
    p.coefficients[0] = rng.nextInRange(0, (1u << bw) - 1);
    // a1 = odd
    p.coefficients[1] = rng.nextInRange(0, (1u << bw) - 1) | 1;

    if (degree >= 2) {
        // Ensure sum of even-indexed (>=2) is even and sum of odd-indexed (>=3) is even
        for (int i = 2; i <= degree; i++) {
            p.coefficients[i] = rng.nextInRange(0, (1u << bw) - 1) & ~1; // make even
        }
    }

    return p;
}

PermutationPoly PermutationPoly::inverse() const {
    // For small bit widths, compute inverse by exhaustive evaluation
    uint32_t mod = 1u << bitWidth;
    PermutationPoly inv;
    inv.bitWidth = bitWidth;

    // Build lookup table
    std::vector<uint32_t> table(mod);
    for (uint32_t x = 0; x < mod; x++) {
        uint32_t y = eval(x);
        table[y] = x;
    }

    // The inverse is stored as a lookup table (not polynomial form)
    // For simplicity, store as degree-(mod-1) polynomial (impractical for large widths)
    // For practical use, we'd use the table directly.

    // For small widths (8-bit), Lagrange interpolation works but is expensive.
    // Just store the table as coefficients[0..mod-1] where coefficients[i] = table[i]
    // and eval becomes a lookup.
    // This is a hack; proper inverse polynomial computation is complex.
    inv.coefficients.resize(mod);
    for (uint32_t i = 0; i < mod; i++)
        inv.coefficients[i] = table[i];

    return inv; // NOTE: eval() won't work correctly with this; need special handling
}

// ============================================================================
// Mixed Radix MBA
// ============================================================================

uint32_t MixedRadixMBA::eval(uint32_t x, uint32_t y) const {
    // CRT reconstruction
    if (components.empty()) return 0;

    // Compute result mod each modulus
    std::vector<int64_t> remainders;
    std::vector<int64_t> moduli;
    for (auto& comp : components) {
        remainders.push_back(comp.compute(x, y) % comp.modulus);
        moduli.push_back(comp.modulus);
    }

    // CRT: find x such that x ≡ r_i (mod m_i)
    int64_t M = 1;
    for (auto m : moduli) M *= m;

    int64_t result = 0;
    for (size_t i = 0; i < moduli.size(); i++) {
        int64_t Mi = M / moduli[i];
        int64_t yi = ModArith::modInverse(Mi, moduli[i]);
        if (yi == -1) return 0; // shouldn't happen with coprime moduli
        result = ModArith::mod(result + remainders[i] * Mi * yi, M);
    }

    return (uint32_t)result;
}

MixedRadixMBA MixedRadixMBA::generate(ArithOp target, ObfRNG& rng) {
    MixedRadixMBA mba;

    // Choose coprime moduli
    std::vector<int64_t> primes = {3, 5, 7, 11, 13, 17, 19, 23};
    std::vector<int64_t> chosen;

    int numComponents = rng.nextInRange(3, 5);
    for (int i = 0; i < numComponents && i < (int)primes.size(); i++) {
        chosen.push_back(primes[i]);
    }

    for (auto p : chosen) {
        int64_t cp = p;
        ArithOp ct = target;
        mba.components.push_back({cp,
            [ct, cp](uint32_t x, uint32_t y) -> uint32_t {
                uint32_t result;
                switch (ct) {
                case ArithOp::Add: result = x + y; break;
                case ArithOp::Sub: result = x - y; break;
                case ArithOp::Xor: result = x ^ y; break;
                case ArithOp::And: result = x & y; break;
                case ArithOp::Or:  result = x | y; break;
                case ArithOp::Not: result = ~x; break;
                case ArithOp::Mul: result = x * y; break;
                }
                return (uint32_t)ModArith::mod(result, cp);
            }
        });
    }

    return mba;
}

// ============================================================================
// Multi-round chaining
// ============================================================================

std::shared_ptr<MBAExpr> chainMBA(ArithOp target, ObfRNG& rng, int rounds) {
    auto rules = getAllStaticRules();

    // Filter rules for our target
    std::vector<MBARule*> applicable;
    for (auto& r : rules) {
        if (r.target == target) applicable.push_back(&r);
    }
    if (applicable.empty()) return nullptr;

    // Pick a random rule
    auto* rule = applicable[rng.nextInRange(0, (uint32_t)applicable.size() - 1)];

    auto x = MBAExpr::var(MBAOp::VarX);
    auto y = MBAExpr::var(MBAOp::VarY);
    auto expr = rule->apply(x, y);

    if (rounds <= 1) return expr;

    // For round 2+, recursively replace sub-operations
    // This is a simplified version — a full implementation would walk the tree
    // and replace matching operations
    // For now, wrap the whole expression in another MBA layer

    // Strategy: treat the entire expression as "x'" and apply another transformation
    // But we need a binary operation to transform. Pick ADD as a wrapper:
    // result = expr + 0 -> MBA(expr + 0)
    // Or: just apply another MBA rule to the ADD/SUB/XOR nodes in the tree

    // Simple approach: wrap in identity addition and apply MBA to it
    auto zero = MBAExpr::con(0);
    auto addExpr = MBAExpr::binary(MBAOp::Add, expr, zero);

    // Now apply an ADD MBA rule to this
    auto addRules = getAddRules();
    auto* addRule = &addRules[rng.nextInRange(0, (uint32_t)addRules.size() - 1)];
    auto chained = addRule->apply(expr, zero);

    if (rounds > 2) {
        // One more round
        auto addRules2 = getAddRules();
        auto* addRule2 = &addRules2[rng.nextInRange(0, (uint32_t)addRules2.size() - 1)];
        chained = addRule2->apply(chained, zero);
    }

    return chained;
}

} // namespace shroud
