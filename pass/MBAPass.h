#pragma once

#include "llvm/IR/PassManager.h"

namespace shroud {

struct MBAPass : public llvm::PassInfoMixin<MBAPass> {
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &AM);
    static bool isRequired() { return true; }
};

} // namespace shroud
