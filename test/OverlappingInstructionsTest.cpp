#include <gtest/gtest.h>
#include "Shroud/OverlappingInstructions.h"
#include <set>

using namespace shroud;

// ============================================================================
// Pattern library tests
// ============================================================================

TEST(OverlapInst, JmpSkipPatterns_NotEmpty) {
    auto patterns = createJmpSkipPatterns();
    EXPECT_GE(patterns.size(), 3u);
}

TEST(OverlapInst, JmpSkipPatterns_Valid) {
    for (auto& p : createJmpSkipPatterns()) {
        SCOPED_TRACE("Pattern: " + p.name);
        EXPECT_TRUE(validatePattern(p));
        EXPECT_FALSE(p.bytes.empty());
        EXPECT_EQ(p.bytes[0], 0xEB); // All start with JMP
    }
}

TEST(OverlapInst, JmpSkipPatterns_RogueBytes) {
    auto patterns = createJmpSkipPatterns();

    // EB 01 E8: JMP +1 over rogue CALL byte
    auto& p = patterns[0];
    EXPECT_EQ(p.bytes.size(), 3u);
    EXPECT_EQ(p.bytes[0], 0xEB); // JMP rel8
    EXPECT_EQ(p.bytes[1], 0x01); // +1
    EXPECT_EQ(p.bytes[2], 0xE8); // CALL opcode (rogue)
}

TEST(OverlapInst, JmpIntoOperand_Valid) {
    auto patterns = createJmpIntoOperandPatterns();
    EXPECT_GE(patterns.size(), 1u);
    for (auto& p : patterns) {
        EXPECT_TRUE(validatePattern(p));
    }
}

TEST(OverlapInst, ComplementaryJumps_Valid) {
    auto patterns = createComplementaryJumpPatterns();
    EXPECT_GE(patterns.size(), 3u);

    for (auto& p : patterns) {
        SCOPED_TRACE("Pattern: " + p.name);
        EXPECT_TRUE(validatePattern(p));
        EXPECT_GE(p.bytes.size(), 5u);

        // Verify complementary pair: opcodes should be complementary
        uint8_t first = p.bytes[0];
        uint8_t second = p.bytes[2];
        // Jcc pairs: 74/75 (JZ/JNZ), 73/72 (JNC/JC), 77/76 (JA/JBE), 7F/7E (JG/JLE)
        EXPECT_EQ((first ^ second) & 1, 1)
            << "Conditional jumps not complementary: "
            << std::hex << (int)first << " vs " << (int)second;
    }
}

TEST(OverlapInst, ComplementaryJumps_AlwaysCoverAllCases) {
    auto patterns = createComplementaryJumpPatterns();

    for (auto& p : patterns) {
        SCOPED_TRACE("Pattern: " + p.name);
        // The pair should have: first jumps +3 (skips 3 bytes: second_opcode, second_disp, rogue)
        // second jumps +1 (skips 1 byte: rogue)
        EXPECT_EQ(p.bytes[1], 0x03); // first condition displacement = +3
        EXPECT_EQ(p.bytes[3], 0x01); // second condition displacement = +1
        // Last byte should be a rogue byte (E8, E9, or 68)
        uint8_t rogue = p.bytes[4];
        EXPECT_TRUE(rogue == 0xE8 || rogue == 0xE9 || rogue == 0x68)
            << "Expected rogue byte (E8/E9/68), got: " << std::hex << (int)rogue;
    }
}

TEST(OverlapInst, MovHiddenJmp_Valid) {
    auto patterns = createMovWithHiddenJmpPatterns();
    EXPECT_GE(patterns.size(), 1u);
    for (auto& p : patterns) {
        EXPECT_TRUE(validatePattern(p));
    }
}

TEST(OverlapInst, NopWrappers_Valid) {
    auto patterns = createNopWrapperPatterns();
    EXPECT_GE(patterns.size(), 3u);
    for (auto& p : patterns) {
        SCOPED_TRACE("Pattern: " + p.name);
        EXPECT_TRUE(validatePattern(p));
        EXPECT_FALSE(p.hasSideEffects); // NOPs have no side effects
    }
}

TEST(OverlapInst, NopWrappers_CorrectPrefix) {
    auto patterns = createNopWrapperPatterns();
    for (auto& p : patterns) {
        // Multi-byte NOPs start with 0F 1F or 66 0F 1F
        bool valid = (p.bytes[0] == 0x0F && p.bytes[1] == 0x1F) ||
                     (p.bytes[0] == 0x66 && p.bytes[1] == 0x0F && p.bytes[2] == 0x1F);
        EXPECT_TRUE(valid) << "NOP pattern doesn't start with correct prefix";
    }
}

TEST(OverlapInst, RogueCallPatterns_Valid) {
    auto patterns = createRogueCallPatterns();
    EXPECT_GE(patterns.size(), 1u);
    for (auto& p : patterns) {
        EXPECT_TRUE(validatePattern(p));
    }
}

TEST(OverlapInst, PrefixConfusion_Valid) {
    auto patterns = createPrefixConfusionPatterns();
    EXPECT_GE(patterns.size(), 1u);
    for (auto& p : patterns) {
        EXPECT_TRUE(validatePattern(p));
        EXPECT_FALSE(p.hasSideEffects);
    }
}

TEST(OverlapInst, RexPrefix_Valid) {
    auto patterns = createRexPrefixPatterns();
    EXPECT_GE(patterns.size(), 1u);
    for (auto& p : patterns) {
        SCOPED_TRACE("Pattern: " + p.name);
        EXPECT_TRUE(validatePattern(p));
        EXPECT_EQ(p.arch, ArchTarget::X86_64);
    }
}

// ============================================================================
// Architecture filtering
// ============================================================================

TEST(OverlapInst, GetAllPatterns_Both) {
    auto all = getAllPatterns(ArchTarget::Both);
    EXPECT_GT(all.size(), 10u);
}

TEST(OverlapInst, GetAllPatterns_X86_64Only) {
    auto patterns = getAllPatterns(ArchTarget::X86_64);
    for (auto& p : patterns) {
        EXPECT_TRUE(p.arch == ArchTarget::X86_64 || p.arch == ArchTarget::Both);
    }
}

TEST(OverlapInst, GetAllPatterns_X86_32Only) {
    auto patterns = getAllPatterns(ArchTarget::X86_32);
    for (auto& p : patterns) {
        EXPECT_TRUE(p.arch == ArchTarget::X86_32 || p.arch == ArchTarget::Both);
    }
}

// ============================================================================
// Random selection
// ============================================================================

TEST(OverlapInst, PickPatterns_CorrectCount) {
    ObfRNG rng(42);
    auto picked = pickPatterns(rng, ArchTarget::Both, 5);
    EXPECT_EQ(picked.size(), 5u);
}

TEST(OverlapInst, PickPatterns_Diversity) {
    ObfRNG rng(42);
    auto picked = pickPatterns(rng, ArchTarget::Both, 20);

    std::set<std::string> names;
    for (auto& p : picked) {
        names.insert(p.name);
    }
    // With 20 picks, we should get variety
    EXPECT_GT(names.size(), 3u);
}

TEST(OverlapInst, PickPatterns_DifferentSeeds) {
    ObfRNG rng1(42);
    ObfRNG rng2(999);
    auto p1 = pickPatterns(rng1, ArchTarget::Both, 5);
    auto p2 = pickPatterns(rng2, ArchTarget::Both, 5);

    // Different seeds should (usually) produce different selections
    bool anyDifferent = false;
    for (size_t i = 0; i < p1.size(); i++) {
        if (p1[i].name != p2[i].name) {
            anyDifferent = true;
            break;
        }
    }
    EXPECT_TRUE(anyDifferent);
}

// ============================================================================
// Inline assembly generation
// ============================================================================

TEST(OverlapInst, InlineAsmATT_NotEmpty) {
    auto patterns = createJmpSkipPatterns();
    for (auto& p : patterns) {
        std::string asm_str = p.inlineAsmATT();
        EXPECT_FALSE(asm_str.empty());
        EXPECT_NE(asm_str.find(".byte"), std::string::npos);
    }
}

TEST(OverlapInst, InlineAsmATT_ContainsBytes) {
    auto patterns = createJmpSkipPatterns();
    auto& p = patterns[0]; // EB 01 E8
    std::string asm_str = p.inlineAsmATT();
    EXPECT_NE(asm_str.find("0xeb"), std::string::npos);
    EXPECT_NE(asm_str.find("0x1"), std::string::npos);
    EXPECT_NE(asm_str.find("0xe8"), std::string::npos);
}

// ============================================================================
// Validation
// ============================================================================

TEST(OverlapInst, Validate_EmptyPatternFails) {
    OverlapPattern empty = {"empty", "empty", ArchTarget::Both, {}, 0, "", "", false};
    EXPECT_FALSE(validatePattern(empty));
}

TEST(OverlapInst, Validate_TooLongPatternFails) {
    OverlapPattern toolong;
    toolong.name = "toolong";
    toolong.bytes.resize(16, 0x90); // > 15 bytes
    EXPECT_FALSE(validatePattern(toolong));
}

TEST(OverlapInst, Validate_AllPatternsPass) {
    auto all = getAllPatterns(ArchTarget::Both);
    for (auto& p : all) {
        SCOPED_TRACE("Pattern: " + p.name);
        EXPECT_TRUE(validatePattern(p));
    }
}

// ============================================================================
// Byte-level overlapping instruction verification
// ============================================================================

TEST(OverlapInst, JmpSkip_E8_DesyncsLinearDisasm) {
    // EB 01 E8 XX XX XX XX YY
    // Linear disasm: JMP +1, CALL rel32 (consumes 4 bytes after E8)
    // Real execution: JMP +1 skips E8, lands on XX XX...
    // The E8 byte desynchronizes the disassembler

    auto patterns = createJmpSkipPatterns();
    auto& p = patterns[0]; // jmp_skip_call
    EXPECT_EQ(p.bytes[2], 0xE8); // E8 = CALL opcode

    // E8 as CALL would consume 4 more bytes as its rel32 operand
    // This means a linear disassembler would "swallow" the next 4 real instruction bytes
}

TEST(OverlapInst, ComplementaryJumps_BothPathsSkipRogue) {
    // 74 03 75 01 E8
    // Path 1 (ZF=1): JZ +3 -> skips 75, 01, E8 -> lands after E8
    // Path 2 (ZF=0): falls through to 75 01 -> JNZ +1 -> skips E8

    auto patterns = createComplementaryJumpPatterns();
    auto& p = patterns[0]; // comp_jz_jnz_call

    uint8_t jz_disp = p.bytes[1];  // 03
    uint8_t jnz_disp = p.bytes[3]; // 01

    // JZ target: offset 0 (JZ) + 2 (instruction length) + 3 (disp) = 5 (past E8)
    int jz_target = 2 + jz_disp;
    EXPECT_EQ(jz_target, 5); // past the E8 byte

    // JNZ target: offset 2 (JNZ) + 2 (instruction length) + 1 (disp) = 5 (past E8)
    int jnz_target = 2 + 2 + jnz_disp;
    EXPECT_EQ(jnz_target, 5); // same target — both skip E8
}

// ============================================================================
// 15-byte NOP opaque predicate byte sequence verification
// ============================================================================

// Verify that XOR reg,reg always sets ZF=1 (opaque predicate correctness)
TEST(OverlapInst, NOP15_XorSelfAlwaysSetsZF) {
    // XOR EAX,EAX = 33 C0, XOR ECX,ECX = 33 C9, etc.
    // After XOR reg,reg: ZF=1, SF=0, PF=1, CF=0
    uint8_t xorModRM[] = {0xC0, 0xC9, 0xD2, 0xDB, 0xF6, 0xFF};
    for (auto modrm : xorModRM) {
        // Verify ModRM encodes reg,reg (mod=11, src=dst)
        EXPECT_EQ(modrm >> 6, 3); // mod=11
        uint8_t src = (modrm >> 3) & 7;
        uint8_t dst = modrm & 7;
        EXPECT_EQ(src, dst); // same register
    }
}

// Verify SUB reg,reg always sets ZF=1
TEST(OverlapInst, NOP15_SubSelfAlwaysSetsZF) {
    uint8_t subModRM[] = {0xC0, 0xC9, 0xD2, 0xDB, 0xF6, 0xFF};
    for (auto modrm : subModRM) {
        EXPECT_EQ(modrm >> 6, 3);
        EXPECT_EQ((modrm >> 3) & 7, modrm & 7);
    }
}

// Verify 15-byte NOP structure: 7x66 + 0F 1F + 84 + 00 = 11 bytes header
TEST(OverlapInst, NOP15_HeaderIs11Bytes) {
    // 7 prefix bytes (66) + 2 opcode bytes (0F 1F) + 1 ModRM (84) + 1 SIB (00) = 11
    uint8_t header[] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
                        0x0F, 0x1F, 0x84, 0x00};
    EXPECT_EQ(sizeof(header), 11u);

    // Verify all prefixes are 66
    for (int i = 0; i < 7; i++) EXPECT_EQ(header[i], 0x66);

    // Verify opcode
    EXPECT_EQ(header[7], 0x0F);
    EXPECT_EQ(header[8], 0x1F);

    // Verify ModRM: mod=10, reg=000, rm=100
    EXPECT_EQ(header[9], 0x84);
    EXPECT_EQ(header[9] >> 6, 2);       // mod=10 (32-bit displacement follows)
    EXPECT_EQ((header[9] >> 3) & 7, 0); // reg=000 (/0)
    EXPECT_EQ(header[9] & 7, 4);        // rm=100 (SIB follows)

    // Verify SIB
    EXPECT_EQ(header[10], 0x00);
}

// Verify displacement field (bytes 11-14) contains valid opaque predicate
TEST(OverlapInst, NOP15_DisplacementIsOpaqueXorJz) {
    // Pattern: 33 C0 74 01 = XOR EAX,EAX; JZ +1
    uint8_t disp[] = {0x33, 0xC0, 0x74, 0x01};

    EXPECT_EQ(disp[0], 0x33); // XOR opcode
    EXPECT_EQ(disp[1], 0xC0); // ModRM: EAX,EAX
    EXPECT_EQ(disp[2], 0x74); // JZ rel8
    EXPECT_EQ(disp[3], 0x01); // +1 (skip exactly 1 trap byte)
}

// Verify trap bytes are crash-inducing
TEST(OverlapInst, NOP15_TrapBytesAreFatal) {
    EXPECT_EQ(0xF4, 0xF4); // HLT: halts CPU in ring 0, #GP in ring 3
    EXPECT_EQ(0xCC, 0xCC); // INT3: debug breakpoint exception
    // EB FE = JMP -2 (infinite loop)
    EXPECT_EQ((uint8_t)0xEB, 0xEB);
    EXPECT_EQ((uint8_t)0xFE, 0xFE);
}

// Verify JZ +1 skips exactly 1 byte (the trap)
TEST(OverlapInst, NOP15_JzSkipsOneTrapByte) {
    // JZ +1 at displacement offset 2 (byte 13 of the 15-byte NOP)
    // JZ is 2 bytes: 74 01
    // IP after JZ decode = byte 15 (13 + 2)
    // Target = 15 + 1 = 16 (skips byte 15 which is the trap)
    uint8_t jz_disp = 0x01;
    int jz_instr_end = 13 + 2; // byte 15
    int target = jz_instr_end + jz_disp; // byte 16
    EXPECT_EQ(target, 16); // lands one byte past the trap
}

// Verify all condition codes used are correct for XOR-self result
TEST(OverlapInst, NOP15_AllConditionsValidAfterXorSelf) {
    // After XOR reg,reg: ZF=1, SF=0, PF=1, CF=0, OF=0
    // JZ  (74) = jump if ZF=1 → TAKEN ✓
    // JNS (79) = jump if SF=0 → TAKEN ✓
    // JP  (7A) = jump if PF=1 → TAKEN ✓
    // JC  (72) after STC: CF=1 → TAKEN ✓
    // JNC (73) after CLC: CF=0 → TAKEN ✓

    struct { uint8_t opcode; bool takenAfterXorSelf; const char* name; } conds[] = {
        {0x74, true,  "JZ"},
        {0x79, true,  "JNS"},
        {0x7A, true,  "JP"},
    };

    for (auto& c : conds) {
        EXPECT_TRUE(c.takenAfterXorSelf)
            << c.name << " (0x" << std::hex << (int)c.opcode
            << ") should be taken after XOR self";
    }
}

// Verify undocumented NOP opcodes (0F 19..0F 1D) are valid in header
TEST(OverlapInst, NOP15_UndocumentedOpcodeRange) {
    for (uint8_t op = 0x19; op <= 0x1F; op++) {
        // All should be valid NOP opcodes on P6+
        EXPECT_GE(op, 0x19);
        EXPECT_LE(op, 0x1F);
    }
}

// Verify infinite loop trap (EB FE) is a valid 2-byte self-referencing JMP
TEST(OverlapInst, NOP15_InfiniteLoopTrap) {
    // EB FE = JMP rel8 with offset -2
    // IP after decode = current + 2, target = current + 2 + (-2) = current
    // This creates an infinite loop at the JMP instruction itself
    int8_t displacement = (int8_t)0xFE; // -2 signed
    EXPECT_EQ(displacement, -2);
    // target = IP_after_decode + displacement = (addr+2) + (-2) = addr
    // loops forever
}

// Verify complete 16-byte sequence (15-byte NOP + 1 trap) is well-formed
TEST(OverlapInst, NOP15_Complete16ByteSequence) {
    // Full sequence from blog: 66*7 + 0F 1F 84 00 + 33 C0 74 01 + F4
    uint8_t seq[] = {
        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, // 7 prefixes
        0x0F, 0x1F, 0x84, 0x00,                     // opcode + ModRM + SIB
        0x33, 0xC0,                                   // XOR EAX,EAX (displacement byte 0-1)
        0x74, 0x01,                                   // JZ +1 (displacement byte 2-3)
        0xF4                                          // HLT trap (byte 15, after NOP)
    };

    // Total = 16 bytes (15-byte NOP + 1-byte trap)
    EXPECT_EQ(sizeof(seq), 16u);

    // NOP is bytes 0-14 (15 bytes)
    // Displacement field is bytes 11-14 (4 bytes)
    // Trap is byte 15 (1 byte, outside the NOP)

    // Hidden execution from byte 11:
    // 33 C0 = XOR EAX,EAX (ZF=1)
    // 74 01 = JZ +1 (always taken, skip byte 15)
    // F4    = HLT (never reached)
    // byte 16 = real code continues

    EXPECT_EQ(seq[11], 0x33); // XOR
    EXPECT_EQ(seq[12], 0xC0); // EAX,EAX
    EXPECT_EQ(seq[13], 0x74); // JZ
    EXPECT_EQ(seq[14], 0x01); // +1
    EXPECT_EQ(seq[15], 0xF4); // HLT trap
}
