#pragma once

#include "Utils.h"
#include <cstdint>
#include <string>
#include <vector>

namespace shroud {

enum class ArchTarget {
    X86_32,
    X86_64,
    Both // patterns valid on both
};

// An overlapping instruction pattern: raw bytes that create
// dual-interpretation when jumping into the middle
struct OverlapPattern {
    std::string name;
    std::string description;
    ArchTarget arch;

    // The raw bytes of the pattern
    std::vector<uint8_t> bytes;

    // Entry point offset (where execution enters)
    int entryOffset;

    // The "real" execution path bytes and their disassembly
    std::string realDisasm;

    // The "fake" linear disassembly (what a naive disassembler sees)
    std::string fakeDisasm;

    // Whether the pattern has any side effects on registers/flags
    // (patterns that are pure control flow have no side effects)
    bool hasSideEffects;

    // Inline assembly string (AT&T syntax) for insertion
    std::string inlineAsmATT() const;

    // Inline assembly string (Intel syntax) for insertion
    std::string inlineAsmIntel() const;
};

// ============================================================================
// Pattern library
// ============================================================================

// JMP +1 over rogue byte: EB 01 XX
// Linear sees: JMP +1, then XX (rogue byte)
// Real path: skips the rogue byte
std::vector<OverlapPattern> createJmpSkipPatterns();

// JMP into own operand: EB FF ...
// The JMP target overlaps with following instruction bytes
std::vector<OverlapPattern> createJmpIntoOperandPatterns();

// Complementary conditional jumps: JZ/JNZ pair covers all cases
// 74 XX 75 YY where XX+YY covers all paths
std::vector<OverlapPattern> createComplementaryJumpPatterns();

// MOV with hidden JMP: 66 B8 EB XX ...
// MOV AX, imm16 where the immediate contains a JMP opcode
std::vector<OverlapPattern> createMovWithHiddenJmpPatterns();

// Multi-byte NOP wrappers (Intel recommended NOPs with hidden instructions)
std::vector<OverlapPattern> createNopWrapperPatterns();

// Rogue byte patterns using CALL (E8) as confusion byte
std::vector<OverlapPattern> createRogueCallPatterns();

// Prefix-based confusion (66h, 67h operand/address size override)
std::vector<OverlapPattern> createPrefixConfusionPatterns();

// REX prefix tricks (x86-64 only)
std::vector<OverlapPattern> createRexPrefixPatterns();

// Exotic NOP patterns: undocumented/reserved NOP-like opcodes
// (hint NOPs, CET NOPs, prefix-padded NOPs, prefetch-NOPs)
std::vector<OverlapPattern> createExoticNopPatterns();

// Get all patterns for a given architecture
std::vector<OverlapPattern> getAllPatterns(ArchTarget arch = ArchTarget::Both);

// Pick random patterns for insertion
std::vector<OverlapPattern> pickPatterns(ObfRNG& rng, ArchTarget arch,
                                          int count = 3);

// ============================================================================
// Validation
// ============================================================================

// Verify that a pattern's bytes are valid and the real execution path
// is a no-op (or specified side-effect only)
bool validatePattern(const OverlapPattern& pattern);

} // namespace shroud
