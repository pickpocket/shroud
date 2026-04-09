#include "MBAPass.h"
#include "Shroud/MBA.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

namespace shroud {

// ============================================================================
// Build LLVM IR for dynamic MBA WITHOUT using NOT/complement operations.
//
// Key insight: the 16 bitwise basis functions of 2 variables can ALL be
// expressed using only AND, OR, XOR, ADD, SUB of x and y (no ~x, no ~y,
// no 0xFFFFFFFF constants).
//
// The 4 "positive minterms" are:
//   m3 = x & y     (bit 3 of truth table)
//   m2 = x & ~y    BUT we compute: m2 = x - (x & y) = x - m3
//   m1 = ~x & y    BUT we compute: m1 = y - (x & y) = y - m3
//   m0 = ~x & ~y   BUT we compute: m0 = (x ^ y ^ m3) - m1 - m2
//      or better:   m0 = (m2 ^ m1) - m2 - m1 + ... no
//      Actually:    The 4 minterms sum to -1 (all bits set):
//                   m0 + m1 + m2 + m3 = ~0
//      But we want to avoid -1 / 0xFFFFFFFF!
//
// BETTER APPROACH: Don't build from minterms at all.
// Instead, express each basis function directly:
//   e0  = 0
//   e1  = truth table 0001 -> only (0,0) -> needs ~x&~y -> AVOID
//   ...
// Most basis functions involving complements are problematic.
//
// SOLUTION: Restructure the coefficients to only use the 4 basis functions
// that DON'T need complements, plus arithmetic combinations:
//   e8  = x & y           (truth table 1000 in my encoding -> bit3)
//   e4  = x & ~y = x - (x&y) = x XOR (x&y)  [since x&~y = x XOR(x AND y)... no]
//         Actually: x & ~y = x - (x & y) works because the bits are disjoint!
//         Wait: x - (x&y) = x&~y only when computed bitwise? No, in integer:
//         x = (x & y) + (x & ~y), so x - (x & y) = x & ~y. YES this works
//         for unsigned integers because the bits are disjoint (no borrow).
//   e2  = ~x & y = y - (x & y) [same argument]
//   e1  = ~x & ~y = NOT(x | y) = ... still uses NOT
//         BUT: ~x & ~y = -1 - x - y + (x & y) [DeMorgan + arithmetic]
//         Still has -1! Hmm.
//
// FINAL APPROACH: Express sum(c_j * e_j) purely using x, y, x&y, x|y, x^y
// and random coefficients, without ever materializing ~x or ~y.
//
// The truth-table system gives us coefficients for the 16 basis functions.
// We can algebraically rewrite the sum using the identities:
//   e_j for j with bits only in {bit2, bit3} (i.e. j in {4,8,12}) -> use x, x&y
//   e_j for j with bits only in {bit1, bit3} (i.e. j in {2,8,10}) -> use y, x&y
// etc.
//
// But the simplest approach: compute the 4 core values WITHOUT NOT:
//   V_and = x & y
//   V_xor = x ^ y
//   V_or  = x | y
//   V_xany = x XOR (x AND y) = x & ~y  [no NOT: XOR removes shared bits]
//   V_yanx = y XOR (x AND y) = ~x & y  [same trick]
//   V_nand = ... still needs NOT for ~(x&y)
//
// KEY REALIZATION: We only need 4 independent functions to span the space.
// Use: {x & y, x ^ y, 1, x + y} or better {x, y, x&y, 1}
// But 1 = constant, not -1.
//
// PRAGMATIC APPROACH: Use the 4 minterms but compute them without NOT:
//   m3 = x & y
//   m2 = x - m3        (= x & ~y, no NOT needed)
//   m1 = y - m3        (= ~x & y, no NOT needed)
//   m0 = needs ~x & ~y
//
// For m0: we know m0 + m1 + m2 + m3 = -1 (all bits set)
// So m0 = -1 - m1 - m2 - m3 = -(1 + m1 + m2 + m3)
// BUT -1 is 0xFFFFFFFF! We want to avoid that constant.
// Alternative: m0 = -(m1 + m2 + m3 + 1) = -(m1 + m2 + m3) - 1
// = 0 - m1 - m2 - m3 - 1
// The "1" constant is fine. The "0 - ..." is NEG which doesn't use 0xFFFFFFFF.
//
// In LLVM IR:
//   m3 = CreateAnd(X, Y)
//   m2 = CreateSub(X, m3)     // x - (x&y) = x & ~y
//   m1 = CreateSub(Y, m3)     // y - (x&y) = ~x & y
//   sum_123 = CreateAdd(CreateAdd(m1, m2), m3)
//   m0 = CreateSub(CreateNeg(sum_123), ConstantInt(1))
//   or: m0 = CreateNot(CreateOr(X, Y))  -- still uses NOT
//   or: m0 = CreateXor(CreateOr(X, Y), -1)  -- still 0xFFFFFFFF
//
// BEST: just compute m0 arithmetically:
//   m0 = 0 - m1 - m2 - m3 - 1
// The constants here are 0 and 1 — no 0xFFFFFFFF!
// CreateSub(CreateSub(CreateSub(CreateSub(0, m1), m2), m3), 1)
// ============================================================================

static Value* buildDynamicMBAIR(IRBuilder<> &Builder, const DynamicMBAResult &mba,
                                 Value *X, Value *Y, Type *Ty) {
    // Compute the 4 minterms WITHOUT using NOT (~) or 0xFFFFFFFF:
    Value *m3 = Builder.CreateAnd(X, Y);                    // x & y
    Value *m2 = Builder.CreateSub(X, m3);                   // x - (x&y) = x & ~y
    Value *m1 = Builder.CreateSub(Y, m3);                   // y - (x&y) = ~x & y
    // m0 = ~x & ~y = -(m1 + m2 + m3) - 1 = 0 - m1 - m2 - m3 - 1
    Value *Zero = ConstantInt::get(Ty, 0);
    Value *One = ConstantInt::get(Ty, 1);
    Value *m0 = Builder.CreateSub(
        Builder.CreateSub(
            Builder.CreateSub(
                Builder.CreateSub(Zero, m1),
                m2),
            m3),
        One);

    Value *Minterms[4] = { m0, m1, m2, m3 };

    // Now build: sum(coefficients[j] * basis_j(x, y))
    // basis_j = OR of minterms where bit is set in j
    // But we avoid OR by using ADD since minterms are disjoint (no overlapping bits)!
    // For disjoint bit patterns: A | B = A + B
    // Minterms are always disjoint, so we can use ADD instead of OR.

    Value *Result = Zero;
    bool first = true;

    for (int j = 0; j < 16; j++) {
        if (mba.coefficients[j] == 0) continue;

        // Build basis_j = sum of minterms where bit is set
        // Since minterms are disjoint, ADD = OR (no NOT needed!)
        Value *BasisVal = Zero;
        bool basisFirst = true;
        for (int bit = 0; bit < 4; bit++) {
            if ((j >> bit) & 1) {
                if (basisFirst) {
                    BasisVal = Minterms[bit];
                    basisFirst = false;
                } else {
                    BasisVal = Builder.CreateAdd(BasisVal, Minterms[bit]);
                }
            }
        }

        // Multiply by coefficient (random, unique per-operation)
        Value *Coeff = ConstantInt::get(Ty, (uint64_t)mba.coefficients[j]);
        Value *Term = Builder.CreateMul(Coeff, BasisVal);

        if (first) {
            Result = Term;
            first = false;
        } else {
            Result = Builder.CreateAdd(Result, Term);
        }
    }

    return Result;
}

// Obfuscate integer constants using MBA (no NOT/0xFFFFFFFF)
static Value* obfuscateConstantIR(IRBuilder<> &Builder, uint64_t C, Value *AuxVar,
                                   Type *Ty, ObfRNG &rng) {
    uint32_t c32 = (uint32_t)C;
    if (c32 == 0 || c32 == 1 || c32 == 0xFFFFFFFF)
        return nullptr;

    auto mba = obfuscateConstant(c32, rng, 32);

    uint32_t salt = rng.next32();
    Value *Y = Builder.CreateXor(AuxVar, ConstantInt::get(Ty, salt));

    return buildDynamicMBAIR(Builder, mba, AuxVar, Y, Ty);
}

PreservedAnalyses MBAPass::run(Function &F, FunctionAnalysisManager &AM) {
    if (F.hasFnAttribute(Attribute::OptimizeNone) || F.isDeclaration())
        return PreservedAnalyses::all();

    uint64_t seed = std::hash<std::string>{}(F.getName().str()) ^ 0xBBA0;
    ObfRNG rng(seed);

    bool Changed = false;

    Value *AuxVar = nullptr;

    // 3 rounds of dynamic MBA substitution
    for (int round = 0; round < 3; round++) {
        for (auto &BB : F) {
            for (auto I = BB.begin(), E = BB.end(); I != E; ) {
                Instruction &Inst = *I++;
                auto *BinOp = dyn_cast<BinaryOperator>(&Inst);
                if (!BinOp || !BinOp->getType()->isIntegerTy(32))
                    continue;

                double prob = (round == 0) ? 1.0 : (round == 1) ? 0.7 : 0.5;
                if (round > 0 && !rng.nextBool(prob))
                    continue;

                ArithOp targetOp;
                switch (BinOp->getOpcode()) {
                case Instruction::Add:  targetOp = ArithOp::Add; break;
                case Instruction::Sub:  targetOp = ArithOp::Sub; break;
                case Instruction::Xor:  targetOp = ArithOp::Xor; break;
                case Instruction::And:  targetOp = ArithOp::And; break;
                case Instruction::Or:   targetOp = ArithOp::Or;  break;
                default: continue;
                }

                IRBuilder<> Builder(BinOp);
                Value *A = BinOp->getOperand(0);
                Value *B = BinOp->getOperand(1);

                // Generate unique dynamic MBA per operation site
                auto dynamicMBA = generateDynamicMBA(targetOp, rng);
                Value *NewVal = buildDynamicMBAIR(Builder, dynamicMBA, A, B,
                                                   BinOp->getType());

                // Add zero-identity noise: result + MBA_zero(A, B)
                // where MBA_zero evaluates to 0 for all inputs
                // This adds algebraic noise without changing the result
                if (rng.nextBool(0.5)) {
                    // Generate an MBA expression that's always zero
                    // by using the same target but subtracting it from itself
                    // Shortcut: (A & B) + (A - (A & B)) - A = 0
                    // Build it via MBA to make it look complex
                    Type *Ty = BinOp->getType();
                    Value *AB = Builder.CreateAnd(A, B);
                    Value *AsubAB = Builder.CreateSub(A, AB);
                    Value *ZeroExpr = Builder.CreateSub(
                        Builder.CreateAdd(AB, AsubAB), A);
                    // Scale by a random constant (0 * anything = 0)
                    Value *Scaled = Builder.CreateMul(ZeroExpr,
                        ConstantInt::get(Ty, rng.next32()));
                    NewVal = Builder.CreateAdd(NewVal, Scaled);
                }

                BinOp->replaceAllUsesWith(NewVal);
                BinOp->eraseFromParent();
                Changed = true;
            }
        }
    }

    // Constant obfuscation pass
    if (!AuxVar) {
        for (auto &Arg : F.args()) {
            if (Arg.getType()->isIntegerTy(32)) { AuxVar = &Arg; break; }
            if (Arg.getType()->isIntegerTy()) {
                IRBuilder<> B(&F.getEntryBlock().front());
                AuxVar = B.CreateZExtOrTrunc(&Arg, Type::getInt32Ty(F.getContext()));
                break;
            }
        }
        if (!AuxVar) {
            auto *GV = new GlobalVariable(*F.getParent(),
                Type::getInt32Ty(F.getContext()), false,
                GlobalValue::PrivateLinkage,
                ConstantInt::get(Type::getInt32Ty(F.getContext()), rng.next32()),
                ".mba_aux." + std::to_string(rng.next32()));
            IRBuilder<> B(&F.getEntryBlock().front());
            auto *Load = B.CreateLoad(Type::getInt32Ty(F.getContext()), GV);
            Load->setVolatile(true);
            AuxVar = Load;
        }
    }

    for (auto &BB : F) {
        for (auto I = BB.begin(), E = BB.end(); I != E; ) {
            Instruction &Inst = *I++;
            for (unsigned op = 0; op < Inst.getNumOperands(); op++) {
                auto *CI = dyn_cast<ConstantInt>(Inst.getOperand(op));
                if (!CI || !CI->getType()->isIntegerTy(32)) continue;
                uint64_t val = CI->getZExtValue();
                if (val < 2 || val == 0xFFFFFFFF) continue;
                if (!rng.nextBool(0.2)) continue;
                IRBuilder<> Builder(&Inst);
                Value *ObfConst = obfuscateConstantIR(Builder, val, AuxVar,
                    CI->getType(), rng);
                if (ObfConst) {
                    Inst.setOperand(op, ObfConst);
                    Changed = true;
                }
            }
        }
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace shroud
