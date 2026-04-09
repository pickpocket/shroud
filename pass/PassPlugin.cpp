#include "OpaquePredicatesPass.h"
#include "MBAPass.h"
#include "OverlappingInstructionsPass.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"

using namespace llvm;

extern "C" __declspec(dllexport) ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "ShroudPasses",
        LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            // Register individual passes
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
                    if (Name == "obfuscate-all") {
                        FPM.addPass(shroud::MBAPass());
                        FPM.addPass(shroud::OpaquePredicatesPass());
                        FPM.addPass(shroud::OverlappingInstructionsPass());
                        return true;
                    }
                    return false;
                });

            // Also register at a late optimization point for -O1+
            PB.registerScalarOptimizerLateEPCallback(
                [](FunctionPassManager &FPM, OptimizationLevel Level) {
                    // Only auto-apply at O2+ if user doesn't use explicit pass names
                    // (This callback is for when the pass is loaded but not explicitly invoked)
                });
        }
    };
}
