#pragma once

#include "llvm/IR/PassManager.h"

namespace shroud {

struct OverlappingInstructionsPass : public llvm::PassInfoMixin<OverlappingInstructionsPass> {
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &AM);
    static bool isRequired() { return true; }
};

} // namespace shroud
