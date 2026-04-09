#include "OpaquePredicatesPass.h"
#include "MBAPass.h"
#include "OverlappingInstructionsPass.h"
#include "StringHidePass.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"

using namespace llvm;

extern "C" __declspec(dllexport) ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "ShroudPasses",
        LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            // Register function passes
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "opaque-pred") {
                        FPM.addPass(shroud::OpaquePredicatesPass());
                        return true;
                    }
                    if (Name == "mba-obf") {
                        FPM.addPass(shroud::MBAPass());
                        return true;
                    }
                    if (Name == "overlap-inst") {
                        FPM.addPass(shroud::OverlappingInstructionsPass());
                        return true;
                    }
                    return false;
                });

            // Register module passes
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "string-hide") {
                        MPM.addPass(shroud::StringHidePass());
                        return true;
                    }
                    if (Name == "string-hide-only") {
                        // Just string hiding (use after function passes)
                        MPM.addPass(shroud::StringHidePass());
                        return true;
                    }
                    return false;
                });
        }
    };
}
