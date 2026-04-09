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

// Each character is computed at runtime as a unique MBA expression.
// MBA evaluates to the correct value for ALL inputs, so aux values don't matter.

static void buildMBAChar(IRBuilder<> &B, Value *StrPtr, unsigned idx,
                          uint8_t charVal, Value *Aux1, Value *Aux2,
                          ObfRNG &rng) {
    Type *I32 = Type::getInt32Ty(B.getContext());
    Type *I8 = Type::getInt8Ty(B.getContext());

    auto mba = obfuscateConstant((uint32_t)charVal, rng, 32);

    // Minterms computed without NOT (avoids ~0 folding by optimizer)
    Value *m3 = B.CreateAnd(Aux1, Aux2);
    Value *m2 = B.CreateSub(Aux1, m3);
    Value *m1 = B.CreateSub(Aux2, m3);
    Value *Zero = ConstantInt::get(I32, 0);
    Value *One = ConstantInt::get(I32, 1);
    Value *m0 = B.CreateSub(B.CreateSub(B.CreateSub(B.CreateSub(Zero, m1), m2), m3), One);

    Value *Minterms[4] = { m0, m1, m2, m3 };

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

    Value *CharVal = B.CreateTrunc(Result, I8);
    Value *GEP = B.CreateInBoundsGEP(I8, StrPtr, ConstantInt::get(I32, idx));
    B.CreateStore(CharVal, GEP);
}

PreservedAnalyses StringHidePass::run(Module &M, ModuleAnalysisManager &AM) {
    uint64_t seed = std::hash<std::string>{}(M.getName().str()) ^ 0x57121D;
    ObfRNG rng(seed);

    SmallVector<GlobalVariable*, 16> stringsToHide;

    for (auto &GV : M.globals()) {
        if (!GV.hasInitializer() || !GV.isConstant()) continue;
        if (GV.hasSection()) continue;
        if (GV.getName().starts_with("llvm.") || GV.getName().starts_with(".")) continue;

        auto *Init = GV.getInitializer();
        auto *CDA = dyn_cast<ConstantDataArray>(Init);
        if (!CDA || !CDA->isString()) continue;

        StringRef Str = CDA->getAsString();
        if (Str.size() < 4) continue;
        if (Str.find_first_not_of('\0') == StringRef::npos) continue;

        stringsToHide.push_back(&GV);
    }

    if (stringsToHide.empty())
        return PreservedAnalyses::all();

    Type *I32 = Type::getInt32Ty(M.getContext());
    auto *AuxGV1 = new GlobalVariable(M, I32, false,
        GlobalValue::PrivateLinkage,
        ConstantInt::get(I32, rng.next32()),
        ".a" + std::to_string(rng.next32()));
    auto *AuxGV2 = new GlobalVariable(M, I32, false,
        GlobalValue::PrivateLinkage,
        ConstantInt::get(I32, rng.next32()),
        ".b" + std::to_string(rng.next32()));

    FunctionType *CtorFTy = FunctionType::get(Type::getVoidTy(M.getContext()), false);
    Function *Ctor = Function::Create(CtorFTy, Function::InternalLinkage,
        ".init" + std::to_string(rng.next32()), &M);
    // optnone prevents other obfuscation passes from transforming the constructor
    Ctor->addFnAttr(Attribute::OptimizeNone);
    Ctor->addFnAttr(Attribute::NoInline);
    BasicBlock *BB = BasicBlock::Create(M.getContext(), "entry", Ctor);
    IRBuilder<> B(BB);

    auto *Aux1 = B.CreateLoad(I32, AuxGV1);
    cast<LoadInst>(Aux1)->setVolatile(true);
    auto *Aux2 = B.CreateLoad(I32, AuxGV2);
    cast<LoadInst>(Aux2)->setVolatile(true);

    for (auto *GV : stringsToHide) {
        auto *CDA = cast<ConstantDataArray>(GV->getInitializer());
        StringRef Str = CDA->getAsString();
        unsigned len = Str.size();

        GV->setConstant(false);
        Type *ArrTy = ArrayType::get(Type::getInt8Ty(M.getContext()), len);
        GV->setInitializer(ConstantAggregateZero::get(ArrTy));

        Value *StrPtr = GV;

        for (unsigned i = 0; i < len; i++) {
            uint8_t c = (uint8_t)Str[i];
            if (c == 0 && i == len - 1) {
                continue;
            }
            buildMBAChar(B, StrPtr, i, c, Aux1, Aux2, rng);
        }
    }

    B.CreateRetVoid();

    appendToGlobalCtors(M, Ctor, 65535);

    return PreservedAnalyses::none();
}

} // namespace shroud
