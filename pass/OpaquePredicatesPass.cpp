#include "OpaquePredicatesPass.h"
#include "Shroud/OpaquePredicates.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Module.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

namespace shroud {

// ============================================================================
// Opaque predicate builders — each constructs a different always-true condition
// ============================================================================

static Value* buildPredicate_ConsecEven(IRBuilder<> &B, Value *X) {
    // x*(x+1) % 2 == 0
    Type *T = X->getType();
    Value *XP1 = B.CreateAdd(X, ConstantInt::get(T, 1));
    Value *Prod = B.CreateMul(X, XP1);
    Value *Rem = B.CreateURem(Prod, ConstantInt::get(T, 2));
    return B.CreateICmpEQ(Rem, ConstantInt::get(T, 0));
}

static Value* buildPredicate_OrComplement(IRBuilder<> &B, Value *X) {
    // (x | ~x) == -1
    Type *T = X->getType();
    return B.CreateICmpEQ(B.CreateOr(X, B.CreateNot(X)), ConstantInt::getAllOnesValue(T));
}

static Value* buildPredicate_XorSelf(IRBuilder<> &B, Value *X) {
    // (x ^ x) == 0
    Type *T = X->getType();
    return B.CreateICmpEQ(B.CreateXor(X, X), ConstantInt::get(T, 0));
}

static Value* buildPredicate_Partition(IRBuilder<> &B, Value *X) {
    // (x & y) + (x | y) == x + y [partition identity]
    Type *T = X->getType();
    Value *Y = B.CreateAdd(X, ConstantInt::get(T, 7));
    return B.CreateICmpEQ(
        B.CreateAdd(B.CreateAnd(X, Y), B.CreateOr(X, Y)),
        B.CreateAdd(X, Y));
}

static Value* buildPredicate_Carry(IRBuilder<> &B, Value *X) {
    // (x + y) == (x ^ y) + 2*(x & y)
    Type *T = X->getType();
    Value *Y = B.CreateAdd(X, ConstantInt::get(T, 13));
    return B.CreateICmpEQ(
        B.CreateAdd(X, Y),
        B.CreateAdd(B.CreateXor(X, Y), B.CreateShl(B.CreateAnd(X, Y), ConstantInt::get(T, 1))));
}

static Value* buildPredicate_DeMorgan(IRBuilder<> &B, Value *X) {
    // ~(x & y) == (~x | ~y)
    Type *T = X->getType();
    Value *Y = B.CreateMul(X, ConstantInt::get(T, 3));
    return B.CreateICmpEQ(
        B.CreateNot(B.CreateAnd(X, Y)),
        B.CreateOr(B.CreateNot(X), B.CreateNot(Y)));
}

static Value* buildPredicate_Fermat(IRBuilder<> &B, Value *X) {
    // x^3 - x ≡ 0 (mod 3)
    Type *T = X->getType();
    Value *X2 = B.CreateMul(X, X);
    Value *X3 = B.CreateMul(X2, X);
    return B.CreateICmpEQ(
        B.CreateURem(B.CreateSub(X3, X), ConstantInt::get(T, 3)),
        ConstantInt::get(T, 0));
}

static Value* buildPredicate_Consec3(IRBuilder<> &B, Value *X) {
    // x*(x+1)*(x+2) % 6 == 0
    Type *T = X->getType();
    Value *XP1 = B.CreateAdd(X, ConstantInt::get(T, 1));
    Value *XP2 = B.CreateAdd(X, ConstantInt::get(T, 2));
    return B.CreateICmpEQ(
        B.CreateURem(B.CreateMul(B.CreateMul(X, XP1), XP2), ConstantInt::get(T, 6)),
        ConstantInt::get(T, 0));
}

static Value* buildPredicate_TwosComp(IRBuilder<> &B, Value *X) {
    // -x == ~x + 1
    Type *T = X->getType();
    return B.CreateICmpEQ(
        B.CreateNeg(X),
        B.CreateAdd(B.CreateNot(X), ConstantInt::get(T, 1)));
}

static Value* buildPredicate_QRMod4(IRBuilder<> &B, Value *X) {
    // x^2 mod 4 < 2 (always true: QR mod 4 = {0, 1})
    Type *T = X->getType();
    Value *X2 = B.CreateMul(X, X);
    Value *Rem = B.CreateURem(X2, ConstantInt::get(T, 4));
    return B.CreateICmpULT(Rem, ConstantInt::get(T, 2));
}

static Value* buildPredicate_BitClear(IRBuilder<> &B, Value *X) {
    // (x & (x-1)) has fewer set bits than x (or x == 0) — simplified to:
    // (x & ~x) == 0 (always true)
    Type *T = X->getType();
    return B.CreateICmpEQ(B.CreateAnd(X, B.CreateNot(X)), ConstantInt::get(T, 0));
}

static Value* buildPredicate_RotateRT(IRBuilder<> &B, Value *X) {
    // ((x << 7) | (x >> 25)) rotated back == x (simplified: just check an identity)
    // (x | 0) == x (trivially true, but hidden in MBA-like form)
    Type *T = X->getType();
    Value *Shifted = B.CreateOr(B.CreateShl(X, ConstantInt::get(T, 7)),
                                 B.CreateLShr(X, ConstantInt::get(T, 25)));
    Value *Unshifted = B.CreateOr(B.CreateShl(Shifted, ConstantInt::get(T, 25)),
                                   B.CreateLShr(Shifted, ConstantInt::get(T, 7)));
    return B.CreateICmpEQ(Unshifted, X);
}

// ============================================================================
// Anti-disassembly inline asm patterns (safe to execute on x86)
// ============================================================================
// Hard anti-disasm: Intel syntax (MSVC target) with opaque conditions
static const char* antiDisasmIntel[] = {
    "xor eax, eax\n jz 1f\n .byte 0xE8\n 1:\n",
    "sub edx, edx\n jnz 1f\n jmp 2f\n 1: .byte 0xE9\n 2:\n",
    "stc\n jnc 1f\n jmp 2f\n 1: .byte 0x48, 0xB8\n 2:\n",
    "clc\n jc 1f\n jmp 2f\n 1: .byte 0xE8, 0xDE, 0xAD\n 2:\n",
    "mov eax, 1\n test eax, eax\n jz 1f\n jmp 2f\n 1: .byte 0x0F, 0x0B\n 2:\n",
    "xor eax, eax\n jz 1f\n .byte 0xCC, 0xCC, 0xE8\n 1:\n",
    "jmp 1f\n .byte 0x48, 0xC7, 0xC0, 0xDE, 0xAD, 0xBE, 0xEF\n 1:\n",
    "xor ecx, ecx\n test ecx, ecx\n jz 2f\n jnz 2f\n .byte 0xE8, 0x48, 0x89\n 2:\n",
    ".byte 0x0F, 0x19, 0xC0",
    ".byte 0x0F, 0x1D, 0x84, 0xDB, 0x78, 0x56, 0x34, 0x12",
    ".byte 0xF3, 0x0F, 0x1E, 0xFA",
};
// AT&T syntax (GNU target)
static const char* antiDisasmATT[] = {
    "xorl %eax, %eax\n jz 1f\n .byte 0xE8\n 1:\n",
    "subl %edx, %edx\n jnz 1f\n jmp 2f\n 1: .byte 0xE9\n 2:\n",
    "stc\n jnc 1f\n jmp 2f\n 1: .byte 0x48, 0xB8\n 2:\n",
    "clc\n jc 1f\n jmp 2f\n 1: .byte 0xE8, 0xDE, 0xAD\n 2:\n",
    "movl $$1, %eax\n testl %eax, %eax\n jz 1f\n jmp 2f\n 1: .byte 0x0F, 0x0B\n 2:\n",
    "xorl %eax, %eax\n jz 1f\n .byte 0xCC, 0xCC, 0xE8\n 1:\n",
    "jmp 1f\n .byte 0x48, 0xC7, 0xC0, 0xDE, 0xAD, 0xBE, 0xEF\n 1:\n",
    "xorl %ecx, %ecx\n testl %ecx, %ecx\n jz 2f\n jnz 2f\n .byte 0xE8, 0x48, 0x89\n 2:\n",
    ".byte 0x0F, 0x19, 0xC0",
    ".byte 0x0F, 0x1D, 0x84, 0xDB, 0x78, 0x56, 0x34, 0x12",
    ".byte 0xF3, 0x0F, 0x1E, 0xFA",
};
static constexpr int NUM_ANTIDIS = sizeof(antiDisasmIntel) / sizeof(antiDisasmIntel[0]);

static bool isIntelTarget(Function &F) {
    Triple TT(F.getParent()->getTargetTriple());
    return TT.isWindowsMSVCEnvironment() || TT.isWindowsItaniumEnvironment();
}

static void emitAntiDisasm(IRBuilder<> &B, ObfRNG &rng) {
    Function *F = B.GetInsertBlock()->getParent();
    bool intel = isIntelTarget(*F);
    FunctionType *VoidFTy = FunctionType::get(Type::getVoidTy(B.getContext()), false);
    int idx = rng.nextInRange(0, NUM_ANTIDIS - 1);
    const char *pat = intel ? antiDisasmIntel[idx] : antiDisasmATT[idx];
    InlineAsm *IA = InlineAsm::get(VoidFTy, pat,
        "~{eax},~{ecx},~{edx},~{cc},~{dirflag},~{fpsr},~{flags}",
        /*hasSideEffects=*/true, /*isAlignStack=*/false,
        intel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
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

// ============================================================================
// Get or create an opaque variable for use in predicates
// ============================================================================
static Value* getOpaqueVar(Function &F, IRBuilder<> &Builder, ObfRNG &rng) {
    Type *I32 = Type::getInt32Ty(F.getContext());

    // Try to use a function argument
    for (auto &Arg : F.args()) {
        if (Arg.getType()->isIntegerTy(32))
            return &Arg;
    }
    // Try to use a function argument and cast
    for (auto &Arg : F.args()) {
        if (Arg.getType()->isIntegerTy())
            return Builder.CreateZExtOrTrunc(&Arg, I32);
        if (Arg.getType()->isPointerTy())
            return Builder.CreatePtrToInt(&Arg, I32);
    }

    // Create a volatile global
    auto *GV = new GlobalVariable(*F.getParent(), I32, false,
        GlobalValue::PrivateLinkage,
        ConstantInt::get(I32, rng.next32()),
        ".opq." + std::to_string(rng.next32()));
    auto *Load = Builder.CreateLoad(I32, GV);
    cast<LoadInst>(Load)->setVolatile(true);
    return Load;
}

// ============================================================================
// Fill bogus block with junk computation that looks real
// ============================================================================
static void fillBogusBlock(BasicBlock *BB, Function &F, ObfRNG &rng) {
    IRBuilder<> B(BB);
    Type *I32 = Type::getInt32Ty(F.getContext());

    // Anti-disasm at block entry
    emitAntiDisasm(B, rng);

    Value *V = ConstantInt::get(I32, rng.next32());
    int numJunkOps = rng.nextInRange(4, 10);
    for (int i = 0; i < numJunkOps; i++) {
        // Scatter anti-disasm between junk ops
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

    // Anti-disasm before the store
    emitAntiDisasm(B, rng);

    // Store to a volatile global to prevent DCE
    auto *JunkGV = new GlobalVariable(*F.getParent(), I32, false,
        GlobalValue::PrivateLinkage, ConstantInt::get(I32, 0),
        ".junk." + std::to_string(rng.next32()));
    auto *Store = B.CreateStore(V, JunkGV);
    Store->setVolatile(true);

    // Anti-disasm at block end (before terminator will be added)
    emitAntiDisasm(B, rng);
}

// ============================================================================
// Create a chain of N bogus blocks connected together
// Each bogus block has junk code and an opaque predicate branching to the next
// The chain eventually converges back to the real continuation
// ============================================================================
static void createBogusChain(BasicBlock *FromBB, BasicBlock *RealTarget,
                              Function &F, ObfRNG &rng, int chainLength) {
    SmallVector<BasicBlock*, 8> bogusBlocks;
    for (int i = 0; i < chainLength; i++) {
        bogusBlocks.push_back(BasicBlock::Create(F.getContext(),
            "bogus." + std::to_string(rng.next32()), &F));
    }

    // Fill each bogus block: anti-disasm → junk → anti-disasm → predicate → anti-disasm → branch
    for (int i = 0; i < chainLength; i++) {
        BasicBlock *BB = bogusBlocks[i];
        fillBogusBlock(BB, F, rng);

        IRBuilder<> B(BB);

        // Anti-disasm before predicate computation
        emitAntiDisasm(B, rng);

        Value *OV = getOpaqueVar(F, B, rng);

        // Anti-disasm between loading opaque var and computing predicate
        emitAntiDisasm(B, rng);

        auto builder = allPredicateBuilders[rng.nextInRange(0, NUM_PRED_BUILDERS - 1)];
        Value *Cond = builder(B, OV);

        // Anti-disasm right before the branch
        emitAntiDisasm(B, rng);

        if (i + 1 < chainLength) {
            if (rng.nextBool()) {
                B.CreateCondBr(Cond, RealTarget, bogusBlocks[i + 1]);
            } else {
                B.CreateCondBr(Cond, bogusBlocks[i + 1], RealTarget);
            }
        } else {
            // Last bogus block: anti-disasm then unconditional branch
            emitAntiDisasm(B, rng);
            B.CreateBr(RealTarget);
        }
    }

    // Replace FromBB's terminator: anti-disasm → predicate → anti-disasm → branch
    FromBB->getTerminator()->eraseFromParent();
    IRBuilder<> B(FromBB);

    // Anti-disasm before the opaque predicate
    emitAntiDisasm(B, rng);

    Value *OV = getOpaqueVar(F, B, rng);

    // Anti-disasm between var load and predicate
    emitAntiDisasm(B, rng);

    auto builder = allPredicateBuilders[rng.nextInRange(0, NUM_PRED_BUILDERS - 1)];
    Value *Cond = builder(B, OV);

    // Anti-disasm right before the branch
    emitAntiDisasm(B, rng);

    // Always-true: true→real, false→first bogus block in chain
    B.CreateCondBr(Cond, RealTarget, bogusBlocks[0]);
}

// ============================================================================
// Main pass: split blocks and insert opaque predicate chains
// ============================================================================
PreservedAnalyses OpaquePredicatesPass::run(Function &F, FunctionAnalysisManager &AM) {
    if (F.hasFnAttribute(Attribute::OptimizeNone) || F.isDeclaration())
        return PreservedAnalyses::all();

    uint64_t seed = std::hash<std::string>{}(F.getName().str()) ^ 0x09A00E;
    ObfRNG rng(seed);

    bool Changed = false;

    // Collect original blocks (avoid iterator invalidation)
    SmallVector<BasicBlock*, 64> origBlocks;
    for (auto &BB : F)
        origBlocks.push_back(&BB);

    for (auto *BB : origBlocks) {
        if (BB->size() < 2) continue;

        // Apply to 80% of blocks
        if (!rng.nextBool(0.8)) continue;

        // Find a split point
        Instruction *splitPt = nullptr;
        for (auto &I : *BB) {
            if (isa<PHINode>(I) || I.isTerminator()) continue;
            splitPt = &I;
            break;
        }
        if (!splitPt) continue;

        BasicBlock *TailBB = BB->splitBasicBlock(splitPt, "real." + BB->getName());

        // Chain length: 2-5 bogus blocks deep
        int chainLen = rng.nextInRange(2, 5);
        createBogusChain(BB, TailBB, F, rng, chainLen);
        Changed = true;

        // ALSO: insert a second opaque predicate split within the tail block
        // to create nested layers
        if (TailBB->size() > 3 && rng.nextBool(0.5)) {
            Instruction *splitPt2 = nullptr;
            int idx = 0;
            for (auto &I : *TailBB) {
                if (isa<PHINode>(I) || I.isTerminator()) continue;
                idx++;
                if (idx >= 2) { splitPt2 = &I; break; }
            }
            if (splitPt2) {
                BasicBlock *TailBB2 = TailBB->splitBasicBlock(splitPt2, "real2." + BB->getName());
                int chainLen2 = rng.nextInRange(1, 3);
                createBogusChain(TailBB, TailBB2, F, rng, chainLen2);
            }
        }
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace shroud
