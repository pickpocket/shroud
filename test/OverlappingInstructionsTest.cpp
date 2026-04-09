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
