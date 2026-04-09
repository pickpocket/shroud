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
    if (F.hasFnAttribute(Attribute::OptimizeNone) || F.isDeclaration())
        return PreservedAnalyses::all();

    Triple TT(F.getParent()->getTargetTriple());
    if (TT.getArch() != Triple::x86_64 && TT.getArch() != Triple::x86)
        return PreservedAnalyses::all();

    // Detect asm dialect: MSVC targets use Intel syntax, GNU uses AT&T
    bool isIntel = TT.isWindowsMSVCEnvironment() || TT.isWindowsItaniumEnvironment();

    uint64_t seed = std::hash<std::string>{}(F.getName().str()) ^ 0x0E1A9ULL;
    ObfRNG rng(seed);

    bool Changed = false;
    FunctionType *VoidFTy = FunctionType::get(Type::getVoidTy(F.getContext()), false);

    // Hard anti-disasm patterns using opaque conditions.
    // We provide both Intel and AT&T syntax versions.
    // Intel syntax: no % prefix on registers, uses [] for memory
    // AT&T syntax: % prefix on registers (but single % in LLVM IR asm strings)

    struct PatternPair {
        const char* intel;
        const char* att;
    };

    static const PatternPair hardPatterns[] = {
        // XOR sets ZF=1, JZ always taken, rogue CALL on dead path
        { "xor eax, eax\n jz 1f\n .byte 0xE8\n 1:\n",
          "xorl %eax, %eax\n jz 1f\n .byte 0xE8\n 1:\n" },

        // SUB self = 0, JNZ never taken, rogue JMP on dead path
        { "sub edx, edx\n jnz 1f\n jmp 2f\n 1: .byte 0xE9\n 2:\n",
          "subl %edx, %edx\n jnz 1f\n jmp 2f\n 1: .byte 0xE9\n 2:\n" },

        // STC + JNC: carry set, JNC never taken
        { "stc\n jnc 1f\n jmp 2f\n 1: .byte 0x48, 0xB8\n 2:\n",
          "stc\n jnc 1f\n jmp 2f\n 1: .byte 0x48, 0xB8\n 2:\n" },

        // CLC + JC: carry clear, JC never taken
        { "clc\n jc 1f\n jmp 2f\n 1: .byte 0xE8, 0xDE, 0xAD\n 2:\n",
          "clc\n jc 1f\n jmp 2f\n 1: .byte 0xE8, 0xDE, 0xAD\n 2:\n" },

        // MOV 1 + TEST: ZF=0, JZ never taken, UD2 on dead path
        { "mov eax, 1\n test eax, eax\n jz 1f\n jmp 2f\n 1: .byte 0x0F, 0x0B\n 2:\n",
          "movl $$1, %eax\n testl %eax, %eax\n jz 1f\n jmp 2f\n 1: .byte 0x0F, 0x0B\n 2:\n" },

        // XOR + JZ taken, INT3 sled on dead path
        { "xor eax, eax\n jz 1f\n .byte 0xCC, 0xCC, 0xE8\n 1:\n",
          "xorl %eax, %eax\n jz 1f\n .byte 0xCC, 0xCC, 0xE8\n 1:\n" },

        // JMP over rogue MOV RAX,imm32 bytes
        { "jmp 1f\n .byte 0x48, 0xC7, 0xC0, 0xDE, 0xAD, 0xBE, 0xEF\n 1:\n",
          "jmp 1f\n .byte 0x48, 0xC7, 0xC0, 0xDE, 0xAD, 0xBE, 0xEF\n 1:\n" },

        // TEST ECX + complementary JZ/JNZ both skip rogue bytes
        { "xor ecx, ecx\n test ecx, ecx\n jz 2f\n jnz 2f\n .byte 0xE8, 0x48, 0x89\n 2:\n",
          "xorl %ecx, %ecx\n testl %ecx, %ecx\n jz 2f\n jnz 2f\n .byte 0xE8, 0x48, 0x89\n 2:\n" },

        // JMP over UD2 + rogue bytes
        { "jmp 2f\n .byte 0x0F, 0x0B\n .byte 0xE8, 0xFF, 0x25\n 2:\n",
          "jmp 2f\n .byte 0x0F, 0x0B\n .byte 0xE8, 0xFF, 0x25\n 2:\n" },

        // XOR + JNZ dead, rogue CALL [rip+disp32] on dead path
        { "xor eax, eax\n jnz 1f\n jmp 2f\n 1: .byte 0xFF, 0x15\n 2:\n",
          "xorl %eax, %eax\n jnz 1f\n jmp 2f\n 1: .byte 0xFF, 0x15\n 2:\n" },
    };
    constexpr int numHard = sizeof(hardPatterns) / sizeof(hardPatterns[0]);

    // Exotic NOPs (same syntax for both dialects since they're raw bytes)
    static const char* exoticNops[] = {
        ".byte 0x0F, 0x19, 0xC0",
        ".byte 0x0F, 0x1A, 0xC0",
        ".byte 0x0F, 0x1B, 0xC0",
        ".byte 0x0F, 0x1C, 0xC0",
        ".byte 0x0F, 0x1D, 0xC0",
        ".byte 0x0F, 0x1E, 0xC0",
        ".byte 0x0F, 0x1F, 0xC8",
        ".byte 0x0F, 0x1F, 0xF8",
        ".byte 0xF3, 0x0F, 0x1E, 0xFA",  // ENDBR64
        ".byte 0xF3, 0x0F, 0x1E, 0xFB",  // ENDBR32
        ".byte 0x0F, 0x1D, 0x84, 0xDB, 0x78, 0x56, 0x34, 0x12",
        ".byte 0x66, 0x66, 0x66, 0x66, 0x90",
    };
    constexpr int numExotic = sizeof(exoticNops) / sizeof(exoticNops[0]);

    for (auto &BB : F) {
        if (BB.empty()) continue;
        Instruction *insertPt = BB.getFirstNonPHI();
        if (!insertPt || insertPt->isTerminator()) continue;

        int numInsert = rng.nextInRange(1, 3);
        for (int p = 0; p < numInsert; p++) {
            if (!rng.nextBool(0.7)) continue;

            auto &pp = hardPatterns[rng.nextInRange(0, numHard - 1)];
            const char *pat = isIntel ? pp.intel : pp.att;
            IRBuilder<> Builder(insertPt);
            InlineAsm *IA = InlineAsm::get(VoidFTy, pat,
                "~{eax},~{ecx},~{edx},~{cc},~{dirflag},~{fpsr},~{flags}",
                /*hasSideEffects=*/true,
                /*isAlignStack=*/false,
                isIntel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
            Builder.CreateCall(IA);
            Changed = true;
        }

        if (rng.nextBool(0.5)) {
            const char* nop = exoticNops[rng.nextInRange(0, numExotic - 1)];
            IRBuilder<> Builder(insertPt);
            InlineAsm *IA = InlineAsm::get(VoidFTy, nop, "",
                true, false,
                isIntel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
            Builder.CreateCall(IA);
            Changed = true;
        }

        Instruction *term = BB.getTerminator();
        if (term && (isa<BranchInst>(term) || isa<SwitchInst>(term))) {
            if (rng.nextBool(0.6)) {
                auto &pp = hardPatterns[rng.nextInRange(0, numHard - 1)];
                const char *pat = isIntel ? pp.intel : pp.att;
                IRBuilder<> Builder(term);
                InlineAsm *IA = InlineAsm::get(VoidFTy, pat,
                    "~{eax},~{ecx},~{edx},~{cc},~{dirflag},~{fpsr},~{flags}",
                    true, false,
                    isIntel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
                Builder.CreateCall(IA);
                Changed = true;
            }
        }
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace shroud
