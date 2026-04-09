#include "StringHidePass.h"
#include "Shroud/MBA.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

using namespace llvm;

namespace shroud {

// ============================================================================
// String hiding via MBA: each character is computed at runtime as a unique
// MBA expression. No key, no decryption loop, no recognizable pattern.
//
// For each string constant "Hello":
//   1. Replace the global constant with a zeroinitializer
//   2. Create a constructor function that runs before main()
//   3. In the constructor, compute each character via MBA and store it
//
// Each character C is expressed as:
//   C = sum(coeff[j] * basisFunction_j(aux1, aux2))
// where coeff[] are random per-character and aux1/aux2 are runtime values
// (read from a volatile global so the optimizer can't fold them).
//
// The MBA expressions evaluate to C for ALL values of aux1/aux2, so the
// actual runtime values don't matter — but the disassembler/decompiler
// sees a massive computation with random constants instead of a string.
// ============================================================================

static void buildMBAChar(IRBuilder<> &B, Value *StrPtr, unsigned idx,
                          uint8_t charVal, Value *Aux1, Value *Aux2,
                          ObfRNG &rng) {
    Type *I32 = Type::getInt32Ty(B.getContext());
    Type *I8 = Type::getInt8Ty(B.getContext());

    // Generate MBA that evaluates to charVal for all inputs
    auto mba = obfuscateConstant((uint32_t)charVal, rng, 32);

    // Compute the 4 minterms WITHOUT using NOT (same as MBAPass)
    Value *m3 = B.CreateAnd(Aux1, Aux2);
    Value *m2 = B.CreateSub(Aux1, m3);
    Value *m1 = B.CreateSub(Aux2, m3);
    Value *Zero = ConstantInt::get(I32, 0);
    Value *One = ConstantInt::get(I32, 1);
    Value *m0 = B.CreateSub(B.CreateSub(B.CreateSub(B.CreateSub(Zero, m1), m2), m3), One);

    Value *Minterms[4] = { m0, m1, m2, m3 };

    // Build: sum(coeff[j] * basis_j)
    Value *Result = Zero;
    bool first = true;
    for (int j = 0; j < 16; j++) {
        if (mba.coefficients[j] == 0) continue;

        Value *BasisVal = Zero;
        bool bFirst = true;
        for (int bit = 0; bit < 4; bit++) {
            if ((j >> bit) & 1) {
                if (bFirst) { BasisVal = Minterms[bit]; bFirst = false; }
                else BasisVal = B.CreateAdd(BasisVal, Minterms[bit]);
            }
        }

        Value *Coeff = ConstantInt::get(I32, (uint64_t)mba.coefficients[j]);
        Value *Term = B.CreateMul(Coeff, BasisVal);

        if (first) { Result = Term; first = false; }
        else Result = B.CreateAdd(Result, Term);
    }

    // Truncate to i8 and store
    Value *CharVal = B.CreateTrunc(Result, I8);
    Value *GEP = B.CreateInBoundsGEP(I8, StrPtr, ConstantInt::get(I32, idx));
    B.CreateStore(CharVal, GEP);
}

PreservedAnalyses StringHidePass::run(Module &M, ModuleAnalysisManager &AM) {
    uint64_t seed = std::hash<std::string>{}(M.getName().str()) ^ 0x57121D;
    ObfRNG rng(seed);

    SmallVector<GlobalVariable*, 16> stringsToHide;

    // Find string constants worth hiding
    for (auto &GV : M.globals()) {
        if (!GV.hasInitializer() || !GV.isConstant()) continue;
        if (GV.hasSection()) continue; // skip special sections
        if (GV.getName().starts_with("llvm.") || GV.getName().starts_with(".")) continue;

        auto *Init = GV.getInitializer();
        auto *CDA = dyn_cast<ConstantDataArray>(Init);
        if (!CDA || !CDA->isString()) continue;

        StringRef Str = CDA->getAsString();
        // Only hide strings >= 4 chars (skip tiny ones)
        if (Str.size() < 4) continue;
        // Skip strings that are all zeros
        if (Str.find_first_not_of('\0') == StringRef::npos) continue;

        stringsToHide.push_back(&GV);
    }

    if (stringsToHide.empty())
        return PreservedAnalyses::all();

    // Create a volatile global for MBA auxiliary variables
    Type *I32 = Type::getInt32Ty(M.getContext());
    auto *AuxGV1 = new GlobalVariable(M, I32, false,
        GlobalValue::PrivateLinkage,
        ConstantInt::get(I32, rng.next32()),
        ".str_aux1");
    auto *AuxGV2 = new GlobalVariable(M, I32, false,
        GlobalValue::PrivateLinkage,
        ConstantInt::get(I32, rng.next32()),
        ".str_aux2");

    // Create constructor function
    FunctionType *CtorFTy = FunctionType::get(Type::getVoidTy(M.getContext()), false);
    Function *Ctor = Function::Create(CtorFTy, Function::InternalLinkage,
        ".shroud_str_init", &M);
    // Mark optnone so MBA/opaque/overlap passes skip this function
    Ctor->addFnAttr(Attribute::OptimizeNone);
    Ctor->addFnAttr(Attribute::NoInline);
    BasicBlock *BB = BasicBlock::Create(M.getContext(), "entry", Ctor);
    IRBuilder<> B(BB);

    // Load auxiliary variables (volatile so optimizer can't fold)
    auto *Aux1 = B.CreateLoad(I32, AuxGV1);
    cast<LoadInst>(Aux1)->setVolatile(true);
    auto *Aux2 = B.CreateLoad(I32, AuxGV2);
    cast<LoadInst>(Aux2)->setVolatile(true);

    for (auto *GV : stringsToHide) {
        auto *CDA = cast<ConstantDataArray>(GV->getInitializer());
        StringRef Str = CDA->getAsString();
        unsigned len = Str.size();

        // Make the global non-constant and zero-initialized
        // (we'll fill it at runtime via the constructor)
        GV->setConstant(false);
        Type *ArrTy = ArrayType::get(Type::getInt8Ty(M.getContext()), len);
        GV->setInitializer(ConstantAggregateZero::get(ArrTy));

        // GV is already a pointer in opaque pointer mode
        Value *StrPtr = GV;

        // Compute each character via MBA
        for (unsigned i = 0; i < len; i++) {
            uint8_t c = (uint8_t)Str[i];
            if (c == 0 && i == len - 1) {
                // Null terminator: just store 0 directly (it's already zeroed)
                continue;
            }
            buildMBAChar(B, StrPtr, i, c, Aux1, Aux2, rng);
        }
    }

    B.CreateRetVoid();

    // Register as a global constructor (priority 65535 = runs early)
    appendToGlobalCtors(M, Ctor, 65535);

    return PreservedAnalyses::none();
}

} // namespace shroud
