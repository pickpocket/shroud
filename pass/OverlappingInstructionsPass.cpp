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

// Dynamic anti-disassembly generator — every instance is unique.

static const char* gpr32_intel[] = {
    "eax", "ebx", "ecx", "edx", "esi", "edi"
};
static const char* gpr32_att[] = {
    "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi"
};
static constexpr int NUM_GPR32 = 6;

static const char* clobberNames[] = {
    "eax", "ebx", "ecx", "edx", "esi", "edi"
};

// Rogue byte sequences — opcodes that swallow following bytes when decoded linearly.
static std::string generateRogueBytes(ObfRNG &rng) {
    std::ostringstream ss;
    int choice = rng.nextInRange(0, 8);
    switch (choice) {
    case 0: // CALL rel32
        ss << "0xE8";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 1: // JMP rel32
        ss << "0xE9";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 2: // PUSH imm32
        ss << "0x68";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 3: // REX.W MOV RAX, imm64
        ss << "0x48, 0xB8";
        for (int i = 0; i < rng.nextInRange(0, 5); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 4: // CALL [rip+disp32]
        ss << "0xFF, 0x15";
        for (int i = 0; i < rng.nextInRange(0, 3); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    case 5: // REX.W MOV RAX, imm32
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
    case 8: // LEA reg, [rip+disp32]
        ss << "0x48, 0x8D, 0x05";
        for (int i = 0; i < rng.nextInRange(1, 4); i++)
            ss << ", 0x" << std::hex << rng.nextInRange(0, 255);
        break;
    }
    return ss.str();
}

static std::pair<std::string, std::string>
generateDynamicPattern(ObfRNG &rng, bool intel) {
    std::ostringstream asm_ss;
    std::ostringstream clob_ss;

    int regIdx = rng.nextInRange(0, NUM_GPR32 - 1);
    const char *reg = intel ? gpr32_intel[regIdx] : gpr32_att[regIdx];
    const char *clobber = clobberNames[regIdx];

    int strategy;
    int roll = rng.nextInRange(0, 99);
    if (roll < 25)
        strategy = 23 + rng.nextInRange(0, 11); // 15-byte NOP opaque + delayed traps (23-34)
    else if (roll < 50)
        strategy = 15 + rng.nextInRange(0, 7);  // NOP control flow (15-22)
    else if (roll < 70)
        strategy = 10 + rng.nextInRange(0, 4);  // NOP overlap (10-14)
    else
        strategy = rng.nextInRange(0, 9);        // standard (0-9)

    static uint32_t labelCounter = 0;
    uint32_t labelId = labelCounter++;
    std::string L1 = ".Ls" + std::to_string(labelId) + "a";
    std::string L2 = ".Ls" + std::to_string(labelId) + "b";

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

    // Exotic NOP overlap: real instructions hidden in NOP displacement field.
    // Format: [0F NN 84 00] [4 bytes of real instructions]

    case 10: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        if (intel) asm_ss << "xor " << reg << ", " << reg << "\n";
        else       asm_ss << "xorl " << reg << ", " << reg << "\n";
        asm_ss << "nop\nnop\n"; // pad to ensure NOP header bytes consumed
        asm_ss << L2 << ":\n";
        break;
    }
    case 11: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        uint32_t imm = rng.nextInRange(1, 127);
        if (intel) asm_ss << "xor " << reg << ", " << reg << "\n";
        else       asm_ss << "xorl " << reg << ", " << reg << "\n";
        asm_ss << "jz " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        if (intel) asm_ss << "add " << reg << ", " << std::dec << imm << "\nnop\n";
        else       asm_ss << "addl $$" << std::dec << imm << ", " << reg << "\nnop\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 12: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        uint32_t imm = rng.nextInRange(1, 127);
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        if (intel) asm_ss << "sub " << reg << ", " << std::dec << imm << "\nnop\n";
        else       asm_ss << "subl $$" << std::dec << imm << ", " << reg << "\nnop\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 13: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        uint32_t imm = rng.nextInRange(1, 255);
        if (intel) asm_ss << "sub " << reg << ", " << reg << "\n";
        else       asm_ss << "subl " << reg << ", " << reg << "\n";
        asm_ss << "jnz " << L2 << "\n";
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        if (intel) asm_ss << "xor " << reg << ", " << std::dec << imm << "\nnop\n";
        else       asm_ss << "xorl $$" << std::dec << imm << ", " << reg << "\nnop\n";
        asm_ss << L2 << ":\n";
        break;
    }
    case 14: {
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0xF3, 0x0F, 0x1E, 0xFA\n"; // ENDBR64
        asm_ss << ".byte 0xE8\n";
        asm_ss << L1 << ":\n";
        if (intel) {
            asm_ss << "add " << reg << ", " << reg << "\n";
            asm_ss << "sub " << reg << ", " << reg << "\n";
        } else {
            asm_ss << "addl " << reg << ", " << reg << "\n";
            asm_ss << "subl " << reg << ", " << reg << "\n";
        }
        asm_ss << L2 << ":\n";
        break;
    }

    // NOP as control flow: JMP through displacement field makes the NOP load-bearing.
    // Removing the NOP severs the CFG edge.

    case 15: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255)
               << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 16: {
        uint8_t nopOp1 = 0x19 + rng.nextInRange(0, 6);
        uint8_t nopOp2 = 0x19 + rng.nextInRange(0, 6);
        std::string Lmid = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp1 << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lmid << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255)
               << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp2 << ", 0x84, 0x00\n";
        asm_ss << Lmid << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255)
               << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 17: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint32_t imm = rng.nextInRange(1, 127);
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        if (intel)
            asm_ss << "xor " << reg << ", " << reg << "\n"
                   << "add " << reg << ", " << std::dec << imm << "\n";
        else
            asm_ss << "xorl " << reg << ", " << reg << "\n"
                   << "addl $$" << std::dec << imm << ", " << reg << "\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255)
               << ", 0x" << std::hex << rng.nextInRange(0,255)
               << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 18: {
        std::string Lm1 = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lm2 = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t op1 = 0x19 + rng.nextInRange(0, 6);
        uint8_t op2 = 0x19 + rng.nextInRange(0, 6);
        uint8_t op3 = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op1 << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\njmp " << Lm1 << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op2 << ", 0x84, 0x00\n";
        asm_ss << Lm1 << ":\njmp " << Lm2 << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op3 << ", 0x84, 0x00\n";
        asm_ss << Lm2 << ":\njmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 19: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) asm_ss << "xor " << reg << ", " << reg << "\n";
        else       asm_ss << "xorl " << reg << ", " << reg << "\n";
        asm_ss << "jz " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\njmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 20: {
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0xF3, 0x0F, 0x1E, 0xFA\n"; // ENDBR64
        asm_ss << L1 << ":\njmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 21: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint32_t imm = rng.nextInRange(1, 200);
        if (intel) asm_ss << "sub " << reg << ", " << reg << "\n";
        else       asm_ss << "subl " << reg << ", " << reg << "\n";
        asm_ss << "jz " << L1 << "\n"; // always taken
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        if (intel) asm_ss << "add " << reg << ", " << std::dec << imm << "\n";
        else       asm_ss << "addl $$" << std::dec << imm << ", " << reg << "\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 22: {
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        int numPfx = rng.nextInRange(2, 4);
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte ";
        for (int i = 0; i < numPfx; i++) asm_ss << "0x66, ";
        asm_ss << "0x90\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }

    // ================================================================
    // 15-BYTE NOP OPAQUE PREDICATES (strategies 23-30)
    //
    // Format: 7x 66 prefix + 0F 1F 84 00 + 4-byte displacement
    // Execution JMPs to byte 11 (displacement field) which contains
    // an opaque predicate + conditional skip over a trap byte.
    //
    // Hidden code at byte 11 (4 bytes):
    //   [2-byte opaque] [JZ/JC/JNC +1] → skip trap → real code
    // Byte 15 (after NOP): trap instruction (F4=HLT or CC=INT3)
    // ================================================================

    case 23: {
        // XOR reg,reg (ZF=1) + JZ +1 over HLT trap
        // 33 XX = XOR reg,reg; 74 01 = JZ +1; F4 = HLT
        uint8_t xorModRM[] = {0xC0,0xC9,0xD2,0xDB,0xF6,0xFF}; // EAX..EDI
        uint8_t modrm = xorModRM[regIdx];
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n"; // 7 prefixes
        asm_ss << ".byte 0x0F, 0x1F, 0x84, 0x00\n";                    // NOP opcode+ModRM+SIB
        asm_ss << L1 << ":\n";                                          // byte 11: displacement
        asm_ss << ".byte 0x33, 0x" << std::hex << (int)modrm << "\n";   // XOR reg,reg
        asm_ss << ".byte 0x74, 0x01\n";                                  // JZ +1
        asm_ss << ".byte 0xF4\n";                                        // HLT trap
        asm_ss << Lexit << ":\n";
        break;
    }
    case 24: {
        // SUB reg,reg (ZF=1) + JZ +1 over INT3 trap
        uint8_t subModRM[] = {0xC0,0xC9,0xD2,0xDB,0xF6,0xFF};
        uint8_t modrm = subModRM[regIdx];
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x1F, 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0x29, 0x" << std::hex << (int)modrm << "\n";   // SUB reg,reg
        asm_ss << ".byte 0x74, 0x01\n";                                  // JZ +1
        asm_ss << ".byte 0xCC\n";                                        // INT3 trap
        asm_ss << Lexit << ":\n";
        break;
    }
    case 25: {
        // XOR reg,reg (SF=0) + JNS +1 over HLT
        uint8_t xorModRM[] = {0xC0,0xC9,0xD2,0xDB,0xF6,0xFF};
        uint8_t modrm = xorModRM[regIdx];
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x1F, 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0x33, 0x" << std::hex << (int)modrm << "\n";   // XOR reg,reg
        asm_ss << ".byte 0x79, 0x01\n";                                  // JNS +1
        asm_ss << ".byte 0xF4\n";                                        // HLT trap
        asm_ss << Lexit << ":\n";
        break;
    }
    case 26: {
        // XOR reg,reg (PF=1) + JP +1 over INT3
        uint8_t xorModRM[] = {0xC0,0xC9,0xD2,0xDB,0xF6,0xFF};
        uint8_t modrm = xorModRM[regIdx];
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x1F, 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0x33, 0x" << std::hex << (int)modrm << "\n";   // XOR reg,reg
        asm_ss << ".byte 0x7A, 0x01\n";                                  // JP +1 (parity even)
        asm_ss << ".byte 0xCC\n";                                        // INT3 trap
        asm_ss << Lexit << ":\n";
        break;
    }
    case 27: {
        // STC (CF=1) + JC +1 over HLT, inside 15-byte NOP
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x1F, 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0xF9\n";                                        // STC
        asm_ss << ".byte 0x72, 0x01\n";                                  // JC +1
        asm_ss << ".byte 0xF4\n";                                        // HLT trap
        asm_ss << Lexit << ":\n";
        break;
    }
    case 28: {
        // CLC (CF=0) + JNC +1 over INT3, inside 15-byte NOP
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x1F, 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0xF8\n";                                        // CLC
        asm_ss << ".byte 0x73, 0x01\n";                                  // JNC +1
        asm_ss << ".byte 0xCC\n";                                        // INT3 trap
        asm_ss << Lexit << ":\n";
        break;
    }
    case 29: {
        // XOR reg,reg + JZ +1 with undocumented NOP opcode (0F 1D instead of 0F 1F)
        uint8_t xorModRM[] = {0xC0,0xC9,0xD2,0xDB,0xF6,0xFF};
        uint8_t modrm = xorModRM[regIdx];
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 4); // 0F 19..0F 1D
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0x33, 0x" << std::hex << (int)modrm << "\n";
        asm_ss << ".byte 0x74, 0x01\n";
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 30: {
        // SUB + JZ, random trap, random undocumented NOP, random prefix count
        uint8_t subModRM[] = {0xC0,0xC9,0xD2,0xDB,0xF6,0xFF};
        uint8_t modrm = subModRM[regIdx];
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        int trapChoice = rng.nextInRange(0, 2);
        int numPfx = 5 + rng.nextInRange(0, 2);
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte ";
        for (int i = 0; i < numPfx; i++) {
            asm_ss << "0x66";
            if (i < numPfx - 1) asm_ss << ", ";
        }
        asm_ss << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0x29, 0x" << std::hex << (int)modrm << "\n";
        if (trapChoice == 2) {
            asm_ss << ".byte 0x74, 0x02\n";
            asm_ss << ".byte 0xEB, 0xFE\n"; // infinite loop
        } else {
            asm_ss << ".byte 0x74, 0x01\n";
            asm_ss << ".byte 0x" << std::hex << (trapChoice == 0 ? 0xF4 : 0xCC) << "\n";
        }
        asm_ss << Lexit << ":\n";
        break;
    }

    // ================================================================
    // DELAYED TRAPS (strategies 31-34)
    // If a deobfuscator doesn't follow the correct NOP-bridged path,
    // the dead path runs several real-looking instructions before
    // hitting an infinite loop or crash many bytes later.
    // ================================================================

    case 31: {
        // JMP over 15-byte NOP to exit, dead path has fake computation + infinite loop
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        // Real path: JMP into NOP displacement → JMP to exit (no register clobber)
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        // Dead path: fake computation → infinite loop (never reached)
        if (intel) asm_ss << "add " << reg << ", " << std::dec << rng.nextInRange(1,127) << "\n";
        else       asm_ss << "addl $$" << std::dec << rng.nextInRange(1,127) << ", " << reg << "\n";
        if (intel) asm_ss << "sub " << reg << ", " << std::dec << rng.nextInRange(1,127) << "\n";
        else       asm_ss << "subl $$" << std::dec << rng.nextInRange(1,127) << ", " << reg << "\n";
        asm_ss << ".byte 0xEB, 0xFE\n"; // infinite loop
        asm_ss << Lexit << ":\n";
        break;
    }
    case 32: {
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        // Dead: computation then HLT
        if (intel) asm_ss << "xor " << reg << ", " << std::dec << rng.nextInRange(1,255) << "\n";
        else       asm_ss << "xorl $$" << std::dec << rng.nextInRange(1,255) << ", " << reg << "\n";
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 33: {
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ltrap = ".Ls" + std::to_string(labelCounter++) + "t";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        // Dead: infinite loop that looks like real computation
        asm_ss << Ltrap << ":\n";
        if (intel) asm_ss << "add " << reg << ", 1\n";
        else       asm_ss << "addl $$1, " << reg << "\n";
        asm_ss << "jmp " << Ltrap << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 34: {
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        // Dead zone: plausible code with hidden traps
        if (intel) {
            asm_ss << "add " << reg << ", " << std::dec << rng.nextInRange(1,100) << "\n";
            asm_ss << ".byte 0xCC\n";
            asm_ss << "sub " << reg << ", " << std::dec << rng.nextInRange(1,100) << "\n";
        } else {
            asm_ss << "addl $$" << std::dec << rng.nextInRange(1,100) << ", " << reg << "\n";
            asm_ss << ".byte 0xCC\n";
            asm_ss << "subl $$" << std::dec << rng.nextInRange(1,100) << ", " << reg << "\n";
        }
        asm_ss << ".byte 0xEB, 0xFE\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    }

    clob_ss << "~{" << clobber << "},~{cc},~{dirflag},~{fpsr},~{flags}";

    return { asm_ss.str(), clob_ss.str() };
}

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

        if (rng.nextBool(0.4)) {
            std::string nop = generateExoticNop(rng);
            IRBuilder<> Builder(insertPt);
            InlineAsm *IA = InlineAsm::get(VoidFTy, nop, "",
                true, false,
                isIntel ? InlineAsm::AD_Intel : InlineAsm::AD_ATT);
            Builder.CreateCall(IA);
            Changed = true;
        }

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
