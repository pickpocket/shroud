#pragma once

#include "llvm/IR/PassManager.h"

namespace shroud {

struct StringHidePass : public llvm::PassInfoMixin<StringHidePass> {
    llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &AM);
    static bool isRequired() { return true; }
};

} // namespace shroud
