#include "OpaquePredicatesPass.h"
#include "Shroud/OpaquePredicates.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Module.h"
#include <sstream>
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

namespace shroud {

// Opaque predicate builders — each constructs a different always-true condition.

static Value* buildPredicate_ConsecEven(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *XP1 = B.CreateAdd(X, ConstantInt::get(T, 1));
    Value *Prod = B.CreateMul(X, XP1);
    Value *Rem = B.CreateURem(Prod, ConstantInt::get(T, 2));
    return B.CreateICmpEQ(Rem, ConstantInt::get(T, 0));
}

static Value* buildPredicate_OrComplement(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    return B.CreateICmpEQ(B.CreateOr(X, B.CreateNot(X)), ConstantInt::getAllOnesValue(T));
}

static Value* buildPredicate_XorSelf(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    return B.CreateICmpEQ(B.CreateXor(X, X), ConstantInt::get(T, 0));
}

static Value* buildPredicate_Partition(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *Y = B.CreateAdd(X, ConstantInt::get(T, 7));
    return B.CreateICmpEQ(
        B.CreateAdd(B.CreateAnd(X, Y), B.CreateOr(X, Y)),
        B.CreateAdd(X, Y));
}

static Value* buildPredicate_Carry(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *Y = B.CreateAdd(X, ConstantInt::get(T, 13));
    return B.CreateICmpEQ(
        B.CreateAdd(X, Y),
        B.CreateAdd(B.CreateXor(X, Y), B.CreateShl(B.CreateAnd(X, Y), ConstantInt::get(T, 1))));
}

static Value* buildPredicate_DeMorgan(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *Y = B.CreateMul(X, ConstantInt::get(T, 3));
    return B.CreateICmpEQ(
        B.CreateNot(B.CreateAnd(X, Y)),
        B.CreateOr(B.CreateNot(X), B.CreateNot(Y)));
}

static Value* buildPredicate_Fermat(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *X2 = B.CreateMul(X, X);
    Value *X3 = B.CreateMul(X2, X);
    return B.CreateICmpEQ(
        B.CreateURem(B.CreateSub(X3, X), ConstantInt::get(T, 3)),
        ConstantInt::get(T, 0));
}

static Value* buildPredicate_Consec3(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *XP1 = B.CreateAdd(X, ConstantInt::get(T, 1));
    Value *XP2 = B.CreateAdd(X, ConstantInt::get(T, 2));
    return B.CreateICmpEQ(
        B.CreateURem(B.CreateMul(B.CreateMul(X, XP1), XP2), ConstantInt::get(T, 6)),
        ConstantInt::get(T, 0));
}

static Value* buildPredicate_TwosComp(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    return B.CreateICmpEQ(
        B.CreateNeg(X),
        B.CreateAdd(B.CreateNot(X), ConstantInt::get(T, 1)));
}

// x^2 mod 4 is always in {0, 1}
static Value* buildPredicate_QRMod4(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *X2 = B.CreateMul(X, X);
    Value *Rem = B.CreateURem(X2, ConstantInt::get(T, 4));
    return B.CreateICmpULT(Rem, ConstantInt::get(T, 2));
}

static Value* buildPredicate_BitClear(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    return B.CreateICmpEQ(B.CreateAnd(X, B.CreateNot(X)), ConstantInt::get(T, 0));
}

// Rotate left then right — result equals original.
static Value* buildPredicate_RotateRT(IRBuilder<> &B, Value *X) {
    Type *T = X->getType();
    Value *Shifted = B.CreateOr(B.CreateShl(X, ConstantInt::get(T, 7)),
                                 B.CreateLShr(X, ConstantInt::get(T, 25)));
    Value *Unshifted = B.CreateOr(B.CreateShl(Shifted, ConstantInt::get(T, 25)),
                                   B.CreateLShr(Shifted, ConstantInt::get(T, 7)));
    return B.CreateICmpEQ(Unshifted, X);
}

static const char* opqRegs_intel[] = { "eax","ebx","ecx","edx","esi","edi" };
static const char* opqRegs_att[]   = { "%eax","%ebx","%ecx","%edx","%esi","%edi" };
static const char* opqClobbers[]   = { "eax","ebx","ecx","edx","esi","edi" };

static const char* gPrefixes[] = {".g",".v",".d",".t",".m",".p",".r",".k"};

static bool isIntelTarget(Function &F) {
    Triple TT(F.getParent()->getTargetTriple());
    return TT.isWindowsMSVCEnvironment() || TT.isWindowsItaniumEnvironment();
}

static void emitAntiDisasm(IRBuilder<> &B, ObfRNG &rng) {
    Function *F = B.GetInsertBlock()->getParent();
    bool intel = isIntelTarget(*F);

    int regIdx = rng.nextInRange(0, 5);
    const char *reg = intel ? opqRegs_intel[regIdx] : opqRegs_att[regIdx];
    static uint32_t labelCounter = 0;
    uint32_t labelId = labelCounter++;
    std::string L1 = ".Lo" + std::to_string(labelId) + "a";
    std::string L2 = ".Lo" + std::to_string(labelId) + "b";

    std::ostringstream rogue;
    int rogueTy = rng.nextInRange(0, 4);
    const char* rogueStarts[] = {"0xE8","0xE9","0x68","0x48, 0xB8","0xFF, 0x15"};
    rogue << rogueStarts[rogueTy];
    for (int i = 0; i < rng.nextInRange(1, 4); i++)
        rogue << ", 0x" << std::hex << rng.nextInRange(0, 255);

    std::ostringstream ss;
    int strategy = rng.nextInRange(0, 6);
    switch (strategy) {
    case 0:
        if (intel) ss << "xor " << reg << ", " << reg << "\n";
        else       ss << "xorl " << reg << ", " << reg << "\n";
        ss << "jz " << L1 << "\n .byte " << rogue.str() << "\n" << L1 << ":\n";
        break;
    case 1:
        if (intel) ss << "sub " << reg << ", " << reg << "\n";
        else       ss << "subl " << reg << ", " << reg << "\n";
        ss << "jnz " << L1 << "\njmp " << L2 << "\n"
           << L1 << ": .byte " << rogue.str() << "\n" << L2 << ":\n";
        break;
    case 2:
        ss << "stc\njnc " << L1 << "\njmp " << L2 << "\n"
           << L1 << ": .byte " << rogue.str() << "\n" << L2 << ":\n";
        break;
    case 3:
        ss << "clc\njc " << L1 << "\njmp " << L2 << "\n"
           << L1 << ": .byte " << rogue.str() << "\n" << L2 << ":\n";
        break;
    case 4: {
        uint32_t imm = rng.nextInRange(1, 0x7FFFFFFF);
        if (intel) ss << "mov " << reg << ", " << imm << "\ntest " << reg << ", " << reg << "\n";
        else       ss << "movl $$" << imm << ", " << reg << "\ntestl " << reg << ", " << reg << "\n";
        ss << "jz " << L1 << "\njmp " << L2 << "\n"
           << L1 << ": .byte " << rogue.str() << "\n" << L2 << ":\n";
        break;
    }
    case 5:
        if (intel) ss << "and " << reg << ", 0\n";
        else       ss << "andl $$0, " << reg << "\n";
        ss << "jnz " << L1 << "\njmp " << L2 << "\n"
           << L1 << ": .byte " << rogue.str() << "\n" << L2 << ":\n";
        break;
    case 6:
        ss << "jmp " << L1 << "\n .byte " << rogue.str() << "\n" << L1 << ":\n";
        break;
    }

    std::string clobStr = std::string("~{") + opqClobbers[regIdx] + "},~{cc},~{dirflag},~{fpsr},~{flags}";
    FunctionType *VoidFTy = FunctionType::get(Type::getVoidTy(B.getContext()), false);
    InlineAsm *IA = InlineAsm::get(VoidFTy, ss.str(), clobStr,
        true, false, intel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
    B.CreateCall(IA);
}

using PredicateBuilder = Value*(*)(IRBuilder<>&, Value*);
static PredicateBuilder allPredicateBuilders[] = {
    buildPredicate_ConsecEven,
    buildPredicate_OrComplement,
    buildPredicate_XorSelf,
    buildPredicate_Partition,
    buildPredicate_Carry,
    buildPredicate_DeMorgan,
    buildPredicate_Fermat,
    buildPredicate_Consec3,
    buildPredicate_TwosComp,
    buildPredicate_QRMod4,
    buildPredicate_BitClear,
    buildPredicate_RotateRT,
};
static constexpr int NUM_PRED_BUILDERS = sizeof(allPredicateBuilders) / sizeof(allPredicateBuilders[0]);

static Value* getOpaqueVar(Function &F, IRBuilder<> &Builder, ObfRNG &rng) {
    Type *I32 = Type::getInt32Ty(F.getContext());

    for (auto &Arg : F.args()) {
        if (Arg.getType()->isIntegerTy(32))
            return &Arg;
    }
    for (auto &Arg : F.args()) {
        if (Arg.getType()->isIntegerTy())
            return Builder.CreateZExtOrTrunc(&Arg, I32);
        if (Arg.getType()->isPointerTy())
            return Builder.CreatePtrToInt(&Arg, I32);
    }

    auto *GV = new GlobalVariable(*F.getParent(), I32, false,
        GlobalValue::PrivateLinkage,
        ConstantInt::get(I32, rng.next32()),
        std::string(gPrefixes[rng.nextInRange(0, 7)]) + std::to_string(rng.next32()));
    auto *Load = Builder.CreateLoad(I32, GV);
    cast<LoadInst>(Load)->setVolatile(true);
    return Load;
}

static void fillBogusBlock(BasicBlock *BB, Function &F, ObfRNG &rng) {
    IRBuilder<> B(BB);
    Type *I32 = Type::getInt32Ty(F.getContext());

    emitAntiDisasm(B, rng);

    Value *V = ConstantInt::get(I32, rng.next32());
    int numJunkOps = rng.nextInRange(4, 10);
    for (int i = 0; i < numJunkOps; i++) {
        if (rng.nextBool(0.4))
            emitAntiDisasm(B, rng);

        switch (rng.nextInRange(0, 7)) {
        case 0: V = B.CreateXor(V, ConstantInt::get(I32, rng.next32())); break;
        case 1: V = B.CreateMul(V, ConstantInt::get(I32, rng.next32() | 1)); break;
        case 2: V = B.CreateShl(V, ConstantInt::get(I32, rng.nextInRange(1, 15))); break;
        case 3: V = B.CreateLShr(V, ConstantInt::get(I32, rng.nextInRange(1, 15))); break;
        case 4: V = B.CreateAdd(V, ConstantInt::get(I32, rng.next32())); break;
        case 5: V = B.CreateOr(V, ConstantInt::get(I32, rng.next32())); break;
        case 6: V = B.CreateAnd(V, ConstantInt::get(I32, rng.next32())); break;
        case 7: V = B.CreateSub(V, ConstantInt::get(I32, rng.next32())); break;
        }
    }

    emitAntiDisasm(B, rng);

    // Volatile store to prevent DCE
    auto *JunkGV = new GlobalVariable(*F.getParent(), I32, false,
        GlobalValue::PrivateLinkage, ConstantInt::get(I32, 0),
        std::string(gPrefixes[rng.nextInRange(0, 7)]) + std::to_string(rng.next32()));
    auto *Store = B.CreateStore(V, JunkGV);
    Store->setVolatile(true);

    emitAntiDisasm(B, rng);
}

// Fix PHI nodes in Target that don't have entries for new predecessors
static void fixPHIsForNewPred(BasicBlock *Target, BasicBlock *NewPred, BasicBlock *ExistingPred) {
    for (auto &I : *Target) {
        auto *PN = dyn_cast<PHINode>(&I);
        if (!PN) break;
        // Add an incoming value from NewPred using the same value as ExistingPred
        Value *Val = PN->getIncomingValueForBlock(ExistingPred);
        if (Val) PN->addIncoming(Val, NewPred);
        else PN->addIncoming(UndefValue::get(PN->getType()), NewPred);
    }
}

static void createBogusChain(BasicBlock *FromBB, BasicBlock *RealTarget,
                              Function &F, ObfRNG &rng, int chainLength) {
    SmallVector<BasicBlock*, 8> bogusBlocks;
    for (int i = 0; i < chainLength; i++) {
        bogusBlocks.push_back(BasicBlock::Create(F.getContext(),
            "bogus." + std::to_string(rng.next32()), &F));
    }

    // Bogus blocks only branch among themselves (never to RealTarget).
    // This avoids adding new predecessors to RealTarget which would require PHI updates.
    for (int i = 0; i < chainLength; i++) {
        BasicBlock *BB = bogusBlocks[i];
        fillBogusBlock(BB, F, rng);
        IRBuilder<> B(BB);
        emitAntiDisasm(B, rng);

        if (i + 1 < chainLength) {
            B.CreateBr(bogusBlocks[i + 1]);
        } else {
            // Last bogus: loop back to first (infinite dead loop among bogus blocks)
            B.CreateBr(bogusBlocks[0]);
        }
    }

    // Entry gate: FromBB gets the opaque predicate, true/false → real/bogus
    FromBB->getTerminator()->eraseFromParent();
    IRBuilder<> B(FromBB);

    emitAntiDisasm(B, rng);
    Value *OV = getOpaqueVar(F, B, rng);
    emitAntiDisasm(B, rng);

    auto builder = allPredicateBuilders[rng.nextInRange(0, NUM_PRED_BUILDERS - 1)];
    Value *Cond = builder(B, OV);

    bool negated = rng.nextBool();
    if (negated)
        Cond = B.CreateXor(Cond, ConstantInt::getTrue(F.getContext()));

    emitAntiDisasm(B, rng);

    if (negated) {
        B.CreateCondBr(Cond, bogusBlocks[0], RealTarget);
    } else {
        B.CreateCondBr(Cond, RealTarget, bogusBlocks[0]);
    }
    // No PHI fixup needed: only FromBB branches to RealTarget (same as before split)
}

PreservedAnalyses OpaquePredicatesPass::run(Function &F, FunctionAnalysisManager &AM) {
    if (F.hasFnAttribute(Attribute::OptimizeNone) || F.isDeclaration())
        return PreservedAnalyses::all();

    uint64_t seed = std::hash<std::string>{}(F.getName().str()) ^ 0x09A00E;
    ObfRNG rng(seed);

    bool Changed = false;

    SmallVector<BasicBlock*, 64> origBlocks;
    for (auto &BB : F)
        origBlocks.push_back(&BB);

    for (auto *BB : origBlocks) {
        if (BB->size() < 2) continue;
        if (!rng.nextBool(0.8)) continue;

        // Skip blocks with PHI nodes
        if (isa<PHINode>(BB->front())) continue;

        // Skip blocks involved with PHI nodes (successors or predecessors)
        bool phiRelated = false;
        for (auto *Succ : successors(BB)) {
            if (!Succ->empty() && isa<PHINode>(Succ->front())) {
                phiRelated = true;
                break;
            }
        }
        for (auto *Pred : predecessors(BB)) {
            if (!Pred->empty() && isa<PHINode>(Pred->front())) {
                phiRelated = true;
                break;
            }
        }
        // Also skip if any instruction in the block is a PHI
        for (auto &I : *BB) {
            if (isa<PHINode>(I)) { phiRelated = true; break; }
        }
        if (phiRelated) continue;

        // Find first non-PHI non-terminator
        Instruction *splitPt = nullptr;
        for (auto &I : *BB) {
            if (isa<PHINode>(I) || I.isTerminator()) continue;
            splitPt = &I;
            break;
        }
        if (!splitPt) continue;

        BasicBlock *TailBB = BB->splitBasicBlock(splitPt,
            "real." + std::to_string(rng.next32()));

        int chainLen = rng.nextInRange(2, 5);
        createBogusChain(BB, TailBB, F, rng, chainLen);
        Changed = true;
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace shroud
