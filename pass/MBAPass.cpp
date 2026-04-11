#include "MBAPass.h"
#include "Shroud/MBA.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

namespace shroud {

static Value* buildDynamicMBAIR(IRBuilder<> &Builder, const DynamicMBAResult &mba,
                                 Value *X, Value *Y, Type *Ty, ObfRNG &rng) {
    Value *Zero = ConstantInt::get(Ty, 0);
    Value *One = ConstantInt::get(Ty, 1);

    // m3 = x & y
    Value *m3;
    switch (rng.nextInRange(0, 2)) {
    default:
    case 0: m3 = Builder.CreateAnd(X, Y); break;
    case 1: m3 = Builder.CreateSub(Builder.CreateAdd(X, Y), Builder.CreateOr(X, Y)); break;
    case 2: { // (x+y) - ((x^y)&x) - ((x^y)&y) - ... just use the first two
        Value *XY = Builder.CreateXor(X, Y);
        m3 = Builder.CreateSub(X, Builder.CreateAnd(XY, X));
        break;
    }
    }

    // m2 = x & ~y
    Value *m2;
    switch (rng.nextInRange(0, 2)) {
    default:
    case 0: m2 = Builder.CreateSub(X, m3); break;
    case 1: m2 = Builder.CreateAnd(Builder.CreateXor(X, Y), X); break;
    case 2: m2 = Builder.CreateXor(X, Builder.CreateAnd(X, Y)); break;
    }

    // m1 = ~x & y
    Value *m1;
    switch (rng.nextInRange(0, 2)) {
    default:
    case 0: m1 = Builder.CreateSub(Y, m3); break;
    case 1: m1 = Builder.CreateAnd(Builder.CreateXor(X, Y), Y); break;
    case 2: m1 = Builder.CreateXor(Y, Builder.CreateAnd(X, Y)); break;
    }

    // m0 = ~x & ~y
    Value *m0;
    switch (rng.nextInRange(0, 2)) {
    default:
    case 0: m0 = Builder.CreateSub(Builder.CreateSub(Builder.CreateSub(
                Builder.CreateSub(Zero, m1), m2), m3), One); break;
    case 1: m0 = Builder.CreateSub(Zero,
                Builder.CreateAdd(Builder.CreateAdd(
                    Builder.CreateAdd(m1, m2), m3), One)); break;
    case 2: m0 = Builder.CreateSub(Builder.CreateSub(Builder.CreateSub(
                Builder.CreateSub(Zero, m3), m2), One), m1); break;
    }

    Value *Minterms[4] = { m0, m1, m2, m3 };

    struct Term { int64_t coeff; int basisIdx; };
    SmallVector<Term, 16> terms;
    for (int j = 0; j < 16; j++)
        if (mba.coefficients[j] != 0)
            terms.push_back({mba.coefficients[j], j});

    for (int i = (int)terms.size() - 1; i > 0; i--) {
        int j = rng.nextInRange(0, i);
        std::swap(terms[i], terms[j]);
    }

    Value *Result = Zero;
    bool first = true;
    for (auto &t : terms) {
        Value *BasisVal = Zero;
        bool bFirst = true;
        int bits[4] = {0, 1, 2, 3};
        for (int i = 3; i > 0; i--) std::swap(bits[i], bits[rng.nextInRange(0, i)]);

        for (int b = 0; b < 4; b++) {
            int bit = bits[b];
            if ((t.basisIdx >> bit) & 1) {
                if (bFirst) { BasisVal = Minterms[bit]; bFirst = false; }
                else BasisVal = Builder.CreateAdd(BasisVal, Minterms[bit]);
            }
        }

        Value *Coeff = ConstantInt::get(Ty, (uint64_t)t.coeff);
        Value *TermVal = Builder.CreateMul(Coeff, BasisVal);

        // Noise injection: (expr - expr) = 0
        if (rng.nextBool(0.1)) {
            Value *NC = ConstantInt::get(Ty, rng.next32());
            Value *Noise;
            switch (rng.nextInRange(0, 3)) {
            default:
            case 0: Noise = Builder.CreateXor(X, NC); break;
            case 1: Noise = Builder.CreateAnd(X, NC); break;
            case 2: Noise = Builder.CreateOr(Y, NC); break;
            case 3: Noise = Builder.CreateAdd(X, NC); break;
            }
            TermVal = Builder.CreateAdd(TermVal, Builder.CreateSub(Noise, Noise));
        }

        if (first) { Result = TermVal; first = false; }
        else if (rng.nextBool()) Result = Builder.CreateAdd(Result, TermVal);
        else                     Result = Builder.CreateAdd(TermVal, Result);
    }

    return Result;
}

static Value* obfuscateConstantIR(IRBuilder<> &Builder, uint64_t C, Value *AuxVar,
                                   Type *Ty, ObfRNG &rng) {
    uint32_t c32 = (uint32_t)C;
    if (c32 == 0 || c32 == 1 || c32 == 0xFFFFFFFF) return nullptr;
    auto mba = obfuscateConstant(c32, rng, 32);
    Value *Y = Builder.CreateXor(AuxVar, ConstantInt::get(Ty, rng.next32()));
    return buildDynamicMBAIR(Builder, mba, AuxVar, Y, Ty, rng);
}

PreservedAnalyses MBAPass::run(Function &F, FunctionAnalysisManager &AM) {
    if (F.hasFnAttribute(Attribute::OptimizeNone) || F.isDeclaration())
        return PreservedAnalyses::all();

    uint64_t seed = std::hash<std::string>{}(F.getName().str()) ^ 0xBBA0;
    ObfRNG rng(seed);
    bool Changed = false;
    Value *AuxVar = nullptr;

    for (int round = 0; round < 3; round++) {
        for (auto &BB : F) {
            // Skip blocks with PHI nodes entirely to avoid PHI placement issues
            if (!BB.empty() && isa<PHINode>(BB.front())) continue;

            for (auto I = BB.begin(), E = BB.end(); I != E; ) {
                Instruction &Inst = *I++;
                auto *BinOp = dyn_cast<BinaryOperator>(&Inst);
                if (!BinOp || !BinOp->getType()->isIntegerTy(32)) continue;
                double prob = (round == 0) ? 1.0 : (round == 1) ? 0.4 : 0.15;
                if (round > 0 && !rng.nextBool(prob)) continue;

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
                auto dynamicMBA = generateDynamicMBA(targetOp, rng);
                Value *NewVal = buildDynamicMBAIR(Builder, dynamicMBA,
                    BinOp->getOperand(0), BinOp->getOperand(1), BinOp->getType(), rng);
                BinOp->replaceAllUsesWith(NewVal);
                BinOp->eraseFromParent();
                Changed = true;
            }
        }
    }

    if (!AuxVar) {
        // Insert after all PHI nodes in the entry block
        Instruction *InsertPt = &*F.getEntryBlock().getFirstNonPHIIt();
        for (auto &Arg : F.args()) {
            if (Arg.getType()->isIntegerTy(32)) { AuxVar = &Arg; break; }
            if (Arg.getType()->isIntegerTy()) {
                IRBuilder<> B(InsertPt);
                AuxVar = B.CreateZExtOrTrunc(&Arg, Type::getInt32Ty(F.getContext()));
                break;
            }
        }
        if (!AuxVar) {
            auto *GV = new GlobalVariable(*F.getParent(),
                Type::getInt32Ty(F.getContext()), false, GlobalValue::PrivateLinkage,
                ConstantInt::get(Type::getInt32Ty(F.getContext()), rng.next32()),
                ".x" + std::to_string(rng.next32()));
            IRBuilder<> B(InsertPt);
            auto *Load = B.CreateLoad(Type::getInt32Ty(F.getContext()), GV);
            Load->setVolatile(true); AuxVar = Load;
        }
    }

    for (auto &BB : F) {
        for (auto I = BB.begin(), E = BB.end(); I != E; ) {
            Instruction &Inst = *I++;
            // Never insert before PHI nodes — PHIs must be grouped at block top
            if (isa<PHINode>(Inst)) continue;
            for (unsigned op = 0; op < Inst.getNumOperands(); op++) {
                auto *CI = dyn_cast<ConstantInt>(Inst.getOperand(op));
                if (!CI || !CI->getType()->isIntegerTy(32)) continue;
                uint64_t val = CI->getZExtValue();
                if (val < 2 || val == 0xFFFFFFFF) continue;
                if (!rng.nextBool(0.2)) continue;
                IRBuilder<> Builder(&Inst);
                Value *ObfConst = obfuscateConstantIR(Builder, val, AuxVar, CI->getType(), rng);
                if (ObfConst) { Inst.setOperand(op, ObfConst); Changed = true; }
            }
        }
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace shroud
