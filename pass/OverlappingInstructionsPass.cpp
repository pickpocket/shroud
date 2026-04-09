#include "OverlappingInstructionsPass.h"
#include "Shroud/OverlappingInstructions.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Module.h"
#include "llvm/TargetParser/Triple.h"

#include <sstream>

using namespace llvm;

namespace shroud {

// ============================================================================
// Dynamic anti-disassembly generator.
// Every pattern is unique: random register, random opaque condition,
// random rogue bytes, random dead-path junk. No two instances are identical
// so signature-based stripping is impossible.
// ============================================================================

static const char* gpr32_intel[] = {
    "eax", "ebx", "ecx", "edx", "esi", "edi"
};
static const char* gpr32_att[] = {
    "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi"
};
static constexpr int NUM_GPR32 = 6;

// All usable clobber names for the register set
static const char* clobberNames[] = {
    "eax", "ebx", "ecx", "edx", "esi", "edi"
};

// Generate a random rogue byte sequence (1-8 bytes)
// Uses opcodes that consume following bytes as operands when decoded linearly:
//   E8 = CALL rel32 (swallows 4 bytes)
//   E9 = JMP rel32 (swallows 4 bytes)
//   68 = PUSH imm32 (swallows 4 bytes)
//   48 B8 = MOV RAX, imm64 (swallows 8 bytes)
//   FF 15 = CALL [rip+disp32] (swallows 4 bytes)
//   48 C7 C0 = MOV RAX, imm32 (swallows 4 bytes)
//   0F 0B = UD2 (2 bytes, crashes if reached)
//   CC = INT3 (1 byte)
static std::string generateRogueBytes(ObfRNG &rng) {
    std::ostringstream ss;
    int choice = rng.nextInRange(0, 8);
    switch (choice) {
    case 0: // CALL rel32 (swallows 4 bytes after it)
        ss << "0xE8";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 1: // JMP rel32 (swallows 4)
        ss << "0xE9";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 2: // PUSH imm32
        ss << "0x68";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 3: // MOV RAX, imm64 (REX.W + B8)
        ss << "0x48, 0xB8";
        for (int i = 0; i < rng.nextInRange(0, 5); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 4: // CALL [rip+disp32]
        ss << "0xFF, 0x15";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 5: // MOV RAX, imm32 (REX.W + C7 C0)
        ss << "0x48, 0xC7, 0xC0";
        for (int i = 0; i < rng.nextInRange(1, 4); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 6: // UD2
        ss << "0x0F, 0x0B";
        break;
    case 7: // INT3 sled
        for (int i = 0; i < rng.nextInRange(1, 4); i++) {
            if (i > 0) ss << ", ";
            ss << "0xCC";
        }
        break;
    case 8: // LEA reg, [rip+disp32] (swallows 4)
        ss << "0x48, 0x8D, 0x05";
        for (int i = 0; i < rng.nextInRange(1, 4); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    }
    return ss.str();
}

// Build a dynamically-generated anti-disasm pattern.
// Returns: { asmString, clobberString }
static std::pair<std::string, std::string>
generateDynamicPattern(ObfRNG &rng, bool intel) {
    std::ostringstream asm_ss;
    std::ostringstream clob_ss;

    // Pick a random scratch register
    int regIdx = rng.nextInRange(0, NUM_GPR32 - 1);
    const char *reg = intel ? gpr32_intel[regIdx] : gpr32_att[regIdx];
    const char *clobber = clobberNames[regIdx];

    // Pick an opaque condition strategy
    int strategy = rng.nextInRange(0, 9);

    // Generate unique label names to avoid collisions
    static uint32_t labelCounter = 0;
    uint32_t labelId = labelCounter++;
    std::string L1 = ".Ls" + std::to_string(labelId) + "a";
    std::string L2 = ".Ls" + std::to_string(labelId) + "b";

    // Generate random rogue bytes for the dead path
    std::string rogue = generateRogueBytes(rng);

    switch (strategy) {
    case 0: {
        // XOR reg, reg → ZF=1, JZ always taken
        if (intel)
            asm_ss << "xor " << reg << ", " << reg << "\n";
        else
            asm_ss << "xorl " << reg << ", " << reg << "\n";
        asm_ss << "jz " << L1 << "\n";
        asm_ss << ".byte " << rogue << "\n";
        asm_ss << L1 << ":\n";
        break;
    }
    case 1: {
        // SUB reg, reg → ZF=1, JNZ never taken
        if (intel)
            asm_ss << "sub " << reg << ", " << reg << "\n";
        else
            asm_ss << "subl " << reg << ", " << reg << "\n";
        asm_ss << "jnz " << L1 << "\n";
        asm_ss << "jmp " << L2 << "\n";
        asm_ss << L1 << ": .byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 2: {
        // STC → CF=1, JNC never taken
        asm_ss << "stc\n";
        asm_ss << "jnc " << L1 << "\n";
        asm_ss << "jmp " << L2 << "\n";
        asm_ss << L1 << ": .byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 3: {
        // CLC → CF=0, JC never taken
        asm_ss << "clc\n";
        asm_ss << "jc " << L1 << "\n";
        asm_ss << "jmp " << L2 << "\n";
        asm_ss << L1 << ": .byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 4: {
        // MOV reg, 1 → TEST → ZF=0, JZ never taken
        uint32_t imm = rng.nextInRange(1, 0x7FFFFFFF); // nonzero
        if (intel)
            asm_ss << "mov " << reg << ", " << imm << "\n"
                   << "test " << reg << ", " << reg << "\n";
        else
            asm_ss << "movl $$" << imm << ", " << reg << "\n"
                   << "testl " << reg << ", " << reg << "\n";
        asm_ss << "jz " << L1 << "\n";
        asm_ss << "jmp " << L2 << "\n";
        asm_ss << L1 << ": .byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 5: {
        // AND reg, 0 → ZF=1, JNZ never taken
        if (intel)
            asm_ss << "and " << reg << ", 0\n";
        else
            asm_ss << "andl $$0, " << reg << "\n";
        asm_ss << "jnz " << L1 << "\n";
        asm_ss << "jmp " << L2 << "\n";
        asm_ss << L1 << ": .byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 6: {
        // XOR + complementary JZ/JNZ both jump to same target
        if (intel)
            asm_ss << "xor " << reg << ", " << reg << "\n";
        else
            asm_ss << "xorl " << reg << ", " << reg << "\n";
        asm_ss << "jz " << L2 << "\n";
        asm_ss << "jnz " << L2 << "\n";
        asm_ss << ".byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 7: {
        // JMP over rogue bytes (unconditional, but rogue bytes look like real code)
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte " << rogue << "\n";
        asm_ss << L1 << ":\n";
        break;
    }
    case 8: {
        // MOV reg, 0 → TEST → JNZ never taken (same as AND 0 but different encoding)
        if (intel)
            asm_ss << "mov " << reg << ", 0\n"
                   << "test " << reg << ", " << reg << "\n";
        else
            asm_ss << "movl $$0, " << reg << "\n"
                   << "testl " << reg << ", " << reg << "\n";
        asm_ss << "jnz " << L1 << "\n";
        asm_ss << "jmp " << L2 << "\n";
        asm_ss << L1 << ": .byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 9: {
        // CMP reg, reg → ZF=1 always, JNE never taken
        if (intel)
            asm_ss << "xor " << reg << ", " << reg << "\n"
                   << "cmp " << reg << ", " << reg << "\n";
        else
            asm_ss << "xorl " << reg << ", " << reg << "\n"
                   << "cmpl " << reg << ", " << reg << "\n";
        asm_ss << "jne " << L1 << "\n";
        asm_ss << "jmp " << L2 << "\n";
        asm_ss << L1 << ": .byte " << rogue << "\n";
        asm_ss << L2 << ":\n";
        break;
    }
    }

    // Build clobber string
    clob_ss << "~{" << clobber << "},~{cc},~{dirflag},~{fpsr},~{flags}";

    return { asm_ss.str(), clob_ss.str() };
}

// Generate a random exotic NOP with varying bytes
static std::string generateExoticNop(ObfRNG &rng) {
    std::ostringstream ss;
    int choice = rng.nextInRange(0, 9);
    switch (choice) {
    case 0: // 0F 19-1E with random /reg
        ss << ".byte 0x0F, 0x" << std::hex << (0x19 + rng.nextInRange(0, 5))
           << ", 0x" << std::hex << (0xC0 + rng.nextInRange(0, 7) * 8);
        break;
    case 1: // 0F 1F with random /reg (undocumented /1-/7)
        ss << ".byte 0x0F, 0x1F, 0x" << std::hex << (0xC0 + rng.nextInRange(1, 7) * 8);
        break;
    case 2: // ENDBR64 or ENDBR32
        ss << ".byte 0xF3, 0x0F, 0x1E, 0x" << std::hex << (rng.nextBool() ? 0xFA : 0xFB);
        break;
    case 3: { // Prefix-padded NOP (2-5 redundant 66 prefixes)
        int n = rng.nextInRange(2, 5);
        ss << ".byte ";
        for (int i = 0; i < n; i++) ss << "0x66, ";
        ss << "0x90";
        break;
    }
    case 4: // Segment override + NOP
        ss << ".byte 0x" << std::hex << (0x26 + rng.nextInRange(0, 3) * 8) << ", 0x90";
        break;
    case 5: // 0F 1D/1E with SIB + random displacement
        ss << ".byte 0x0F, 0x" << std::hex << (0x1D + rng.nextInRange(0, 1))
           << ", 0x84, 0x" << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 6: // 0F 1F with random SIB+disp32
        ss << ".byte 0x0F, 0x1F, 0x84, 0x"
           << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255)
           << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 7: // 66 + 0F 1F with random ModRM
        ss << ".byte 0x66, 0x0F, 0x1F, 0x" << std::hex << rng.nextInRange(0xC0, 0xFF);
        break;
    case 8: // 0F 0D prefetch-NOP with random /reg
        ss << ".byte 0x0F, 0x0D, 0x" << std::hex << (0xC0 + rng.nextInRange(0, 7) * 8);
        break;
    case 9: // Mixed segment overrides + address size + NOP
        ss << ".byte 0x" << std::hex << (0x64 + rng.nextInRange(0, 1))
           << ", 0x67, 0x90";
        break;
    }
    return ss.str();
}

PreservedAnalyses OverlappingInstructionsPass::run(Function &F, FunctionAnalysisManager &AM) {
    if (F.hasFnAttribute(Attribute::OptimizeNone) || F.isDeclaration())
        return PreservedAnalyses::all();

    Triple TT(F.getParent()->getTargetTriple());
    if (TT.getArch() != Triple::x86_64 && TT.getArch() != Triple::x86)
        return PreservedAnalyses::all();

    bool isIntel = TT.isWindowsMSVCEnvironment() || TT.isWindowsItaniumEnvironment();

    uint64_t seed = std::hash<std::string>{}(F.getName().str()) ^ 0x0E1A9ULL;
    ObfRNG rng(seed);

    bool Changed = false;
    FunctionType *VoidFTy = FunctionType::get(Type::getVoidTy(F.getContext()), false);

    for (auto &BB : F) {
        if (BB.empty()) continue;
        Instruction *insertPt = BB.getFirstNonPHI();
        if (!insertPt || insertPt->isTerminator()) continue;

        // 1-3 dynamic anti-disasm patterns per block
        int numInsert = rng.nextInRange(1, 3);
        for (int p = 0; p < numInsert; p++) {
            if (!rng.nextBool(0.7)) continue;

            auto [asmStr, clobStr] = generateDynamicPattern(rng, isIntel);
            IRBuilder<> Builder(insertPt);
            InlineAsm *IA = InlineAsm::get(VoidFTy, asmStr, clobStr,
                /*hasSideEffects=*/true, /*isAlignStack=*/false,
                isIntel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
            Builder.CreateCall(IA);
            Changed = true;
        }

        // Scatter exotic NOPs (same raw .byte syntax for both dialects)
        if (rng.nextBool(0.4)) {
            std::string nop = generateExoticNop(rng);
            IRBuilder<> Builder(insertPt);
            InlineAsm *IA = InlineAsm::get(VoidFTy, nop, "",
                true, false,
                isIntel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
            Builder.CreateCall(IA);
            Changed = true;
        }

        // Also before terminators
        Instruction *term = BB.getTerminator();
        if (term && (isa<BranchInst>(term) || isa<SwitchInst>(term))) {
            if (rng.nextBool(0.5)) {
                auto [asmStr, clobStr] = generateDynamicPattern(rng, isIntel);
                IRBuilder<> Builder(term);
                InlineAsm *IA = InlineAsm::get(VoidFTy, asmStr, clobStr,
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
