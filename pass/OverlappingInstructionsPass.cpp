#include "OverlappingInstructionsPass.h"
#include "Shroud/OverlappingInstructions.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Module.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

namespace shroud {

PreservedAnalyses OverlappingInstructionsPass::run(Function &F, FunctionAnalysisManager &AM) {
    if (F.hasFnAttribute(Attribute::OptimizeNone))
        return PreservedAnalyses::all();
    if (F.isDeclaration())
        return PreservedAnalyses::all();

    Triple TT(F.getParent()->getTargetTriple());
    ArchTarget arch;
    if (TT.getArch() == Triple::x86_64)
        arch = ArchTarget::X86_64;
    else if (TT.getArch() == Triple::x86)
        arch = ArchTarget::X86_32;
    else
        return PreservedAnalyses::all();

    uint64_t seed = std::hash<std::string>{}(F.getName().str()) ^ 0x0E1A9ULL;
    ObfRNG rng(seed);

    auto allPatterns = getAllPatterns(arch);
    if (allPatterns.empty())
        return PreservedAnalyses::all();

    bool Changed = false;
    FunctionType *VoidFTy = FunctionType::get(Type::getVoidTy(F.getContext()), false);

    for (auto &BB : F) {
        // Insert in EVERY block with high probability
        if (BB.empty()) continue;

        // Skip blocks that start with PHI nodes for the first insertion
        Instruction *insertPt = BB.getFirstNonPHI();
        if (!insertPt || insertPt->isTerminator()) continue;

        // Use JMP-based anti-disassembly patterns
        // These are safe because the JMP skips over the rogue bytes
        // Key: the `.byte` sequence must be self-contained and the JMP
        // displacement must be correct relative to the pattern itself.
        static const char* safePatterns[] = {
            // EB 01 E8: JMP +1 over rogue CALL byte
            ".byte 0xEB, 0x01, 0xE8",
            // EB 01 E9: JMP +1 over rogue JMP byte
            ".byte 0xEB, 0x01, 0xE9",
            // EB 01 68: JMP +1 over rogue PUSH byte
            ".byte 0xEB, 0x01, 0x68",
            // EB 02 E8 CD: JMP +2 over 2 rogue bytes
            ".byte 0xEB, 0x02, 0xE8, 0xCD",
            // EB 05 E8 DE AD BE EF: JMP +5 over fake CALL
            ".byte 0xEB, 0x05, 0xE8, 0xDE, 0xAD, 0xBE, 0xEF",
            // Multi-byte NOPs (always safe)
            ".byte 0x0F, 0x1F, 0x00",
            ".byte 0x0F, 0x1F, 0x40, 0x00",
            ".byte 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00",
        };
        constexpr int numSafe = sizeof(safePatterns) / sizeof(safePatterns[0]);

        int numInsert = rng.nextInRange(2, 5);
        for (int p = 0; p < numInsert; p++) {
            if (!rng.nextBool(0.8)) continue;

            const char* pat = safePatterns[rng.nextInRange(0, numSafe - 1)];
            IRBuilder<> Builder(insertPt);
            InlineAsm *IA = InlineAsm::get(VoidFTy, pat, "", /*hasSideEffects=*/true);
            Builder.CreateCall(IA);
            Changed = true;
        }

        // Also before terminators
        Instruction *term = BB.getTerminator();
        if (term && (isa<BranchInst>(term) || isa<SwitchInst>(term))) {
            if (rng.nextBool(0.8)) {
                const char* pat = safePatterns[rng.nextInRange(0, numSafe - 1)];
                IRBuilder<> Builder(term);
                InlineAsm *IA = InlineAsm::get(VoidFTy, pat, "", true);
                Builder.CreateCall(IA);
                Changed = true;
            }
        }
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace shroud
