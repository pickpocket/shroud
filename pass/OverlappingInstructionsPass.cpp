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

static const char* gpr64_intel[] = {
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi"
};
static const char* gpr64_att[] = {
    "%rax", "%rbx", "%rcx", "%rdx", "%rsi", "%rdi"
};

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
    const char *reg64 = intel ? gpr64_intel[regIdx] : gpr64_att[regIdx];

    int strategy;
    int roll = rng.nextInRange(0, 99);
    if (roll < 12)
        strategy = 46 + rng.nextInRange(0, 19); // exotic instructions (46-65)
    else if (roll < 24)
        strategy = 40 + rng.nextInRange(0, 5);  // RET-based NOP jump (40-45)
    else if (roll < 36)
        strategy = 35 + rng.nextInRange(0, 4);  // 15-byte NOP control flow (35-39)
    else if (roll < 48)
        strategy = 23 + rng.nextInRange(0, 11); // 15-byte NOP opaque + delayed traps (23-34)
    else if (roll < 60)
        strategy = 15 + rng.nextInRange(0, 7);  // 8-byte NOP control flow (15-22)
    else if (roll < 75)
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

    // ================================================================
    // 15-BYTE NOP AS CONTROL FLOW (strategies 35-39)
    // Same as strategies 15-22 but with 15-byte prefix-stacked NOPs.
    // Execution threads through the NOP's displacement to reach exit.
    // Stripping the NOP severs the control flow.
    // ================================================================

    case 35: {
        // Single 15-byte NOP bridge
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255)
               << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 36: {
        // Double 15-byte NOP chain
        std::string Lmid = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t op1 = 0x19 + rng.nextInRange(0, 6);
        uint8_t op2 = 0x19 + rng.nextInRange(0, 6);
        // First 15-byte NOP
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op1 << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\njmp " << Lmid << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        // Second 15-byte NOP
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op2 << ", 0x84, 0x00\n";
        asm_ss << Lmid << ":\njmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 37: {
        // Triple 15-byte NOP chain
        std::string Lm1 = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lm2 = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t op1 = 0x19 + rng.nextInRange(0, 6);
        uint8_t op2 = 0x19 + rng.nextInRange(0, 6);
        uint8_t op3 = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op1 << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\njmp " << Lm1 << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op2 << ", 0x84, 0x00\n";
        asm_ss << Lm1 << ":\njmp " << Lm2 << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op3 << ", 0x84, 0x00\n";
        asm_ss << Lm2 << ":\njmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 38: {
        // 15-byte NOP bridge with ENDBR64 prefix instead of 66 stacking
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0xF3, 0x0F, 0x1E, 0xFA\n"; // ENDBR64 (4 bytes, dead)
        asm_ss << ".byte 0x66, 0x66, 0x66\n";         // 3 prefixes
        asm_ss << ".byte 0x0F, 0x1F, 0x84, 0x00\n";   // NOP opcode
        asm_ss << L1 << ":\njmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 39: {
        // Mixed 8-byte + 15-byte NOP chain
        std::string Lmid = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t op1 = 0x19 + rng.nextInRange(0, 6);
        uint8_t op2 = 0x19 + rng.nextInRange(0, 6);
        // First: 8-byte NOP bridge
        asm_ss << "jmp " << L1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op1 << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\njmp " << Lmid << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        // Second: 15-byte NOP bridge
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op2 << ", 0x84, 0x00\n";
        asm_ss << Lmid << ":\njmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }

    // ================================================================
    // RET-BASED NOP JUMPS (strategies 40-45)
    //
    // Push the address of the NOP displacement onto the stack, then RET.
    // RET pops the address and jumps to it. Disassemblers treat RET as
    // a function return, not a branch — they never follow where it goes.
    // The call stack becomes meaningless (RET without matching CALL).
    //
    // Construction:
    //   lea reg, [rip + offset_to_displacement]
    //   push reg
    //   ret              ← jumps to NOP displacement
    //   .byte NOP header ← dead, never executed
    //   displacement:    ← execution lands here via RET
    //   jmp exit
    // ================================================================

    case 40: {
        // CALL+ADD+RET into 8-byte NOP displacement
        // CALL pushes return addr, target adjusts it to point into NOP disp, RET goes there
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ladj = ".Ls" + std::to_string(labelCounter++) + "j";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "call " << Ladj << "\n";
        // Dead: NOP header (CALL skipped past this to Ladj)
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n"; // NOP displacement — RET target
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Ladj << ":\n";
        // Adjust return addr on stack: [rsp] currently points after the CALL.
        // We want it to point to L1 instead. Use: add [rsp], (L1 - after_call)
        // Since L1 is after the NOP header, offset = size of NOP header (4 bytes)
        if (intel) asm_ss << "add qword ptr [rsp], 4\n";
        else       asm_ss << "addq $4, (%rsp)\n";
        asm_ss << "ret\n"; // pops adjusted addr, jumps to L1
        asm_ss << Lexit << ":\n";
        break;
    }
    case 41: {
        // CALL+ADD+RET into 15-byte NOP displacement
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ladj = ".Ls" + std::to_string(labelCounter++) + "j";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "call " << Ladj << "\n";
        asm_ss << ".byte 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Ladj << ":\n";
        // NOP header is 11 bytes (7 prefixes + 4 opcode/modrm/sib)
        if (intel) asm_ss << "add qword ptr [rsp], 11\n";
        else       asm_ss << "addq $11, (%rsp)\n";
        asm_ss << "ret\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 42: {
        // CALL+ADD+RET with random offset adjustment (harder to pattern match)
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ladj = ".Ls" + std::to_string(labelCounter++) + "j";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        // Use assembler-computed offset so it's always correct
        asm_ss << "call " << Ladj << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Ladj << ":\n";
        if (intel) asm_ss << "add qword ptr [rsp], 4\n";
        else       asm_ss << "addq $4, (%rsp)\n";
        asm_ss << "ret\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 43: {
        // Double CALL+RET chain through two NOP displacements
        std::string Lmid = ".Ls" + std::to_string(labelCounter++) + "m";
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ladj1 = ".Ls" + std::to_string(labelCounter++) + "j";
        std::string Ladj2 = ".Ls" + std::to_string(labelCounter++) + "j";
        uint8_t op1 = 0x19 + rng.nextInRange(0, 6);
        uint8_t op2 = 0x19 + rng.nextInRange(0, 6);
        // First CALL+RET
        asm_ss << "call " << Ladj1 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op1 << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        // Second CALL+RET
        asm_ss << "call " << Ladj2 << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)op2 << ", 0x84, 0x00\n";
        asm_ss << Lmid << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        // Adjustment routines
        asm_ss << Ladj1 << ":\n";
        if (intel) asm_ss << "add qword ptr [rsp], 4\n";
        else       asm_ss << "addq $4, (%rsp)\n";
        asm_ss << "ret\n";
        asm_ss << Ladj2 << ":\n";
        if (intel) asm_ss << "add qword ptr [rsp], 4\n";
        else       asm_ss << "addq $4, (%rsp)\n";
        asm_ss << "ret\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 44: {
        // CALL+RET with HLT trap on dead path
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ladj = ".Ls" + std::to_string(labelCounter++) + "j";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "call " << Ladj << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << ".byte 0xF4\n"; // HLT trap (dead)
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << Ladj << ":\n";
        if (intel) asm_ss << "add qword ptr [rsp], 5\n"; // 4 NOP header + 1 HLT
        else       asm_ss << "addq $5, (%rsp)\n";
        asm_ss << "ret\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 45: {
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ladj = ".Ls" + std::to_string(labelCounter++) + "j";
        asm_ss << "call " << Ladj << "\n";
        asm_ss << ".byte 0xF3, 0x0F, 0x1E, 0xFA\n"; // ENDBR64 (dead)
        asm_ss << ".byte 0xE8\n"; // rogue CALL byte (dead)
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << ".byte 0x" << std::hex << rng.nextInRange(0,255) << ", 0x" << std::hex << rng.nextInRange(0,255) << "\n";
        asm_ss << Ladj << ":\n";
        if (intel) asm_ss << "add qword ptr [rsp], 5\n"; // 4 ENDBR64 + 1 rogue E8
        else       asm_ss << "addq $5, (%rsp)\n";
        asm_ss << "ret\n";
        asm_ss << Lexit << ":\n";
        break;
    }

    // ================================================================
    // EXOTIC INSTRUCTION STRATEGIES (46-55)
    // Uses unusual x86 instructions that confuse analysis tools.
    // ================================================================

    case 46: {
        // LOOP with ECX=1: LOOP decrements ECX and jumps if nonzero.
        // With ECX=1, after decrement ECX=0, LOOP falls through (not taken).
        // Disassembler sees a backward branch and models a loop.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov ecx, 1\n";
            asm_ss << "loop " << L1 << "\n"; // not taken (ECX becomes 0)
        } else {
            asm_ss << "movl $1, %ecx\n";
            asm_ss << "loop " << L1 << "\n";
        }
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << L1 << ":\n"; // dead: LOOP target (never reached)
        asm_ss << ".byte 0xF4\n"; // HLT trap
        asm_ss << Lexit << ":\n";
        break;
    }
    case 47: {
        // LOOPE with ECX=1, ZF=0: LOOPE jumps if ECX!=0 AND ZF=1.
        // XOR sets ZF=1, but ECX becomes 0 after decrement, so not taken.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov ecx, 1\n";
            asm_ss << "xor " << reg << ", " << reg << "\n"; // ZF=1
            asm_ss << "loope " << L1 << "\n"; // ECX→0, not taken
        } else {
            asm_ss << "movl $1, %ecx\n";
            asm_ss << "xorl " << reg << ", " << reg << "\n";
            asm_ss << "loope " << L1 << "\n";
        }
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << L1 << ": .byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 48: {
        // CMC chain: STC sets CF, CMC complements it (CF=0), JNC always taken.
        // Three different opcodes for one opaque predicate.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "stc\n";  // CF=1
        asm_ss << "cmc\n";  // CF=0
        asm_ss << "jnc " << Lexit << "\n"; // always taken
        asm_ss << ".byte 0xF4\n"; // dead HLT
        asm_ss << Lexit << ":\n";
        break;
    }
    case 49: {
        // Double CMC: CLC→CMC→CMC = CF still 0, JNC taken.
        // Two CMCs cancel each other. Analysis must track flag state through both.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "clc\n";  // CF=0
        asm_ss << "cmc\n";  // CF=1
        asm_ss << "cmc\n";  // CF=0
        asm_ss << "jnc " << Lexit << "\n"; // always taken
        asm_ss << ".byte 0xCC\n"; // dead INT3
        asm_ss << Lexit << ":\n";
        break;
    }
    case 50: {
        // LAHF/SAHF opaque: known flags → LAHF stores to AH → test AH bits
        // XOR reg,reg sets ZF=1 → LAHF puts flags in AH → test AH,0x40 (ZF bit) → JNZ taken
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "xor eax, eax\n"; // ZF=1, AH undefined
            asm_ss << "lahf\n";          // AH = flags (ZF is bit 6 = 0x40)
            asm_ss << "test ah, 0x40\n"; // test ZF bit in AH
            asm_ss << "jnz " << Lexit << "\n"; // always taken (ZF was 1)
        } else {
            asm_ss << "xorl %eax, %eax\n";
            asm_ss << "lahf\n";
            asm_ss << "testb $0x40, %ah\n";
            asm_ss << "jnz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 51: {
        // SAHF opaque: load known value into AH, SAHF sets flags from AH
        // 0x40 = ZF set, then JZ taken
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov ah, 0x40\n";  // ZF bit set in AH
            asm_ss << "sahf\n";           // flags ← AH (ZF=1)
            asm_ss << "jz " << Lexit << "\n"; // always taken
        } else {
            asm_ss << "movb $0x40, %ah\n";
            asm_ss << "sahf\n";
            asm_ss << "jz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xEB, 0xFE\n"; // dead infinite loop
        asm_ss << Lexit << ":\n";
        break;
    }
    case 52: {
        // REP RET: F3 C3 — AMD branch prediction hint, acts as normal RET.
        // Used in actual compiled code (GCC emits this). Paired with CALL+ADD.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        std::string Ladj = ".Ls" + std::to_string(labelCounter++) + "j";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        asm_ss << "call " << Ladj << "\n";
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << "jmp " << Lexit << "\n";
        asm_ss << Ladj << ":\n";
        if (intel) asm_ss << "add qword ptr [rsp], 4\n";
        else       asm_ss << "addq $4, (%rsp)\n";
        asm_ss << ".byte 0xF3, 0xC3\n"; // REP RET instead of plain RET
        asm_ss << Lexit << ":\n";
        break;
    }
    case 53: {
        // XLAT-based computation: AL = [RBX + AL]. Single byte (D7).
        // Set RBX to a known table address, AL to index → computed value.
        // This is a real computed load hidden in the anti-disasm.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "jmp " << Lexit << "\n";
        // Dead path: XLAT + trap (never reached, but confuses data flow analysis)
        asm_ss << ".byte 0xD7\n"; // XLAT
        asm_ss << ".byte 0xF4\n"; // HLT
        asm_ss << Lexit << ":\n";
        break;
    }
    case 54: {
        // LOOP into NOP displacement: ECX=1, LOOP not taken, dead loop body is NOP disp
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint8_t nopOp = 0x19 + rng.nextInRange(0, 6);
        if (intel) asm_ss << "mov ecx, 1\n";
        else       asm_ss << "movl $1, %ecx\n";
        asm_ss << "loop " << L1 << "\n"; // not taken
        asm_ss << "jmp " << Lexit << "\n";
        // Dead loop body disguised as NOP
        asm_ss << ".byte 0x0F, 0x" << std::hex << (int)nopOp << ", 0x84, 0x00\n";
        asm_ss << L1 << ":\n";
        asm_ss << ".byte 0xEB, 0xFE\n"; // infinite loop (dead)
        asm_ss << Lexit << ":\n";
        break;
    }
    case 55: {
        // Triple flag chain: STC → CMC → CLC → CMC = CF=1, JC always taken.
        // Four flag-manipulating instructions, analysis must track through all.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        asm_ss << "stc\n";  // CF=1
        asm_ss << "cmc\n";  // CF=0
        asm_ss << "clc\n";  // CF=0
        asm_ss << "cmc\n";  // CF=1
        asm_ss << "jc " << Lexit << "\n"; // always taken
        // Dead path with rogue bytes
        asm_ss << ".byte " << generateRogueBytes(rng) << "\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 56: {
        // BT (bit test): known value → BT bit 0 → CF = known → JC/JNC
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov " << reg << ", 1\n"; // bit 0 = 1
            asm_ss << "bt " << reg << ", 0\n";  // CF = bit 0 = 1
            asm_ss << "jc " << Lexit << "\n";   // always taken
        } else {
            asm_ss << "movl $1, " << reg << "\n";
            asm_ss << "bt $0, " << reg << "\n";
            asm_ss << "jc " << Lexit << "\n";
        }
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 57: {
        // BSF: bit scan forward on known nonzero value → ZF=0 always
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint32_t val = rng.nextInRange(1, 0x7FFFFFFF);
        if (intel) {
            asm_ss << "mov " << reg << ", " << std::dec << val << "\n";
            asm_ss << "bsf " << reg << ", " << reg << "\n"; // ZF=0 (nonzero input)
            asm_ss << "jnz " << Lexit << "\n"; // always taken
        } else {
            asm_ss << "movl $" << std::dec << val << ", " << reg << "\n";
            asm_ss << "bsfl " << reg << ", " << reg << "\n";
            asm_ss << "jnz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xCC\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 58: {
        // BSWAP twice = identity. BSWAP+BSWAP+CMP with original = equal → JZ taken
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        uint32_t val = rng.next32();
        if (intel) {
            asm_ss << "mov " << reg << ", " << std::dec << val << "\n";
            asm_ss << "bswap " << reg << "\n";   // reverse bytes
            asm_ss << "bswap " << reg << "\n";   // reverse again = original
            asm_ss << "cmp " << reg << ", " << std::dec << val << "\n";
            asm_ss << "jz " << Lexit << "\n";    // always taken
        } else {
            asm_ss << "movl $" << std::dec << val << ", " << reg << "\n";
            asm_ss << "bswap " << reg << "\n";
            asm_ss << "bswap " << reg << "\n";
            asm_ss << "cmpl $" << std::dec << val << ", " << reg << "\n";
            asm_ss << "jz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 59: {
        // CDQ sign-extend: XOR EAX,EAX → CDQ zeros EDX → TEST EDX,EDX → JZ taken
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "xor eax, eax\n"; // EAX=0
            asm_ss << "cdq\n";           // EDX = sign-extend(EAX) = 0
            asm_ss << "test edx, edx\n"; // ZF=1
            asm_ss << "jz " << Lexit << "\n";
        } else {
            asm_ss << "xorl %eax, %eax\n";
            asm_ss << "cdq\n";
            asm_ss << "testl %edx, %edx\n";
            asm_ss << "jz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xEB, 0xFE\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 60: {
        // PUSHF/POPF: save flags, trash them, restore → flags unchanged
        // XOR sets ZF=1, PUSHF saves, STC trashes CF, POPF restores ZF=1, JZ taken
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) asm_ss << "xor " << reg << ", " << reg << "\n"; // ZF=1
        else       asm_ss << "xorl " << reg << ", " << reg << "\n";
        asm_ss << "pushfq\n";  // save flags (ZF=1)
        asm_ss << "stc\n";     // trash CF
        asm_ss << "cmc\n";     // trash CF more
        asm_ss << "popfq\n";   // restore flags (ZF=1 again)
        asm_ss << "jz " << Lexit << "\n"; // always taken (ZF restored)
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 61: {
        // XCHG twice = identity: XCHG a,b then XCHG a,b → both unchanged
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        // Pick a second register different from the first
        int regIdx2 = (regIdx + 1) % NUM_GPR32;
        const char *reg2 = intel ? gpr32_intel[regIdx2] : gpr32_att[regIdx2];
        if (intel) {
            asm_ss << "xor " << reg << ", " << reg << "\n"; // reg=0
            asm_ss << "xchg " << reg << ", " << reg2 << "\n"; // swap
            asm_ss << "xchg " << reg << ", " << reg2 << "\n"; // swap back
            asm_ss << "test " << reg << ", " << reg << "\n";  // reg still 0, ZF=1
            asm_ss << "jz " << Lexit << "\n";
        } else {
            asm_ss << "xorl " << reg << ", " << reg << "\n";
            asm_ss << "xchg " << reg << ", " << reg2 << "\n";
            asm_ss << "xchg " << reg << ", " << reg2 << "\n";
            asm_ss << "testl " << reg << ", " << reg << "\n";
            asm_ss << "jz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xCC\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 62: {
        // MOVSX sign-extend: MOV AL,0xFF → MOVSX EAX,AL → EAX=0xFFFFFFFF → always nonzero
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov al, 0xFF\n";
            asm_ss << "movsx eax, al\n"; // EAX = 0xFFFFFFFF
            asm_ss << "test eax, eax\n"; // nonzero
            asm_ss << "jnz " << Lexit << "\n";
        } else {
            asm_ss << "movb $0xFF, %al\n";
            asm_ss << "movsbl %al, %eax\n";
            asm_ss << "testl %eax, %eax\n";
            asm_ss << "jnz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 63: {
        // POPCNT opaque: POPCNT of known value → known count → compare → always true
        // POPCNT(0x55555555) = 16 always
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov " << reg << ", 0x55555555\n";
            asm_ss << "popcnt " << reg << ", " << reg << "\n"; // reg = 16
            asm_ss << "cmp " << reg << ", 16\n";
            asm_ss << "jz " << Lexit << "\n";
        } else {
            asm_ss << "movl $0x55555555, " << reg << "\n";
            asm_ss << "popcnt " << reg << ", " << reg << "\n";
            asm_ss << "cmpl $16, " << reg << "\n";
            asm_ss << "jz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xEB, 0xFE\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 64: {
        // BTC (bit test and complement) + BTC = identity on the tested bit
        // BTC flips a bit, second BTC flips it back. CF from second = original bit.
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov " << reg << ", 1\n";   // bit 0 = 1
            asm_ss << "btc " << reg << ", 0\n";   // flip bit 0 (now 0), CF=1
            asm_ss << "btc " << reg << ", 0\n";   // flip back (now 1), CF=0
            asm_ss << "jnc " << Lexit << "\n";    // CF=0, always taken
        } else {
            asm_ss << "movl $1, " << reg << "\n";
            asm_ss << "btc $0, " << reg << "\n";
            asm_ss << "btc $0, " << reg << "\n";
            asm_ss << "jnc " << Lexit << "\n";
        }
        asm_ss << ".byte 0xF4\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    case 65: {
        // LZCNT/BSR combo: BSR of known value gives known result
        // BSR(0x80000000) = 31 always
        std::string Lexit = ".Ls" + std::to_string(labelCounter++) + "x";
        if (intel) {
            asm_ss << "mov " << reg << ", 0x80000000\n";
            asm_ss << "bsr " << reg << ", " << reg << "\n"; // reg = 31
            asm_ss << "cmp " << reg << ", 31\n";
            asm_ss << "jz " << Lexit << "\n";
        } else {
            asm_ss << "movl $0x80000000, " << reg << "\n";
            asm_ss << "bsr " << reg << ", " << reg << "\n";
            asm_ss << "cmpl $31, " << reg << "\n";
            asm_ss << "jz " << Lexit << "\n";
        }
        asm_ss << ".byte 0xCC\n";
        asm_ss << Lexit << ":\n";
        break;
    }
    }

    clob_ss << "~{" << clobber << "},~{cc},~{dirflag},~{fpsr},~{flags},~{memory}";

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
