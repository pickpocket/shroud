#pragma once

#include "llvm/IR/PassManager.h"

namespace shroud {

struct OpaquePredicatesPass : public llvm::PassInfoMixin<OpaquePredicatesPass> {
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &AM);
    static bool isRequired() { return true; }
};

} // namespace shroud
