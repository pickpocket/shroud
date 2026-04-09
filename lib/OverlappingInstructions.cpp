#include "Shroud/OverlappingInstructions.h"
#include <sstream>

namespace shroud {

// ============================================================================
// OverlapPattern inline assembly generation
// ============================================================================

std::string OverlapPattern::inlineAsmATT() const {
    std::ostringstream ss;
    ss << ".byte ";
    for (size_t i = 0; i < bytes.size(); i++) {
        if (i > 0) ss << ", ";
        ss << "0x" << std::hex << (int)bytes[i];
    }
    return ss.str();
}

std::string OverlapPattern::inlineAsmIntel() const {
    // Same as ATT for raw bytes
    return inlineAsmATT();
}

// ============================================================================
// JMP +1 over rogue byte patterns
// ============================================================================

std::vector<OverlapPattern> createJmpSkipPatterns() {
    std::vector<OverlapPattern> patterns;

    // EB 01 E8: JMP +1 skips E8 (CALL opcode), linear disasm sees CALL
    patterns.push_back({
        "jmp_skip_call", "JMP +1 over rogue CALL byte E8",
        ArchTarget::Both,
        {0xEB, 0x01, 0xE8},
        0,
        "JMP +1 -> skip E8",
        "JMP +1; CALL rel32 (swallows 4 bytes)",
        false
    });

    // EB 01 E9: JMP +1 skips E9 (JMP rel32 opcode)
    patterns.push_back({
        "jmp_skip_jmp", "JMP +1 over rogue JMP byte E9",
        ArchTarget::Both,
        {0xEB, 0x01, 0xE9},
        0,
        "JMP +1 -> skip E9",
        "JMP +1; JMP rel32 (swallows 4 bytes)",
        false
    });

    // EB 01 68: JMP +1 skips 68 (PUSH imm32)
    patterns.push_back({
        "jmp_skip_push", "JMP +1 over rogue PUSH byte 68",
        ArchTarget::Both,
        {0xEB, 0x01, 0x68},
        0,
        "JMP +1 -> skip 68",
        "JMP +1; PUSH imm32 (swallows 4 bytes)",
        false
    });

    // EB 02 E8 xx: JMP +2 skips 2 bytes
    patterns.push_back({
        "jmp_skip2_call", "JMP +2 over 2 rogue bytes (E8 CD)",
        ArchTarget::Both,
        {0xEB, 0x02, 0xE8, 0xCD},
        0,
        "JMP +2 -> skip E8 CD",
        "JMP +2; CALL rel32 (E8 CD ...)",
        false
    });

    return patterns;
}

// ============================================================================
// JMP into own operand patterns
// ============================================================================

std::vector<OverlapPattern> createJmpIntoOperandPatterns() {
    std::vector<OverlapPattern> patterns;

    // EB FF C0 48: JMP -1 (into FF byte) -> decodes FF C0 = INC EAX (x86-32)
    // On x86-64: FF C0 = INC EAX, 48 = REX.W prefix (harmless before next instr)
    patterns.push_back({
        "jmp_into_ff_c0", "JMP into own displacement, decodes as INC EAX",
        ArchTarget::X86_32,
        {0xEB, 0xFF, 0xC0},
        0,
        "JMP -1 -> FF C0 (INC EAX)",
        "JMP -1 (appears to loop); INC AL?",
        true // modifies EAX
    });

    // EB FE: JMP -2 (infinite loop for dead code block)
    patterns.push_back({
        "jmp_infinite", "JMP -2 creates infinite loop (for dead code)",
        ArchTarget::Both,
        {0xEB, 0xFE},
        0,
        "JMP -2 -> infinite loop at EB",
        "JMP -2",
        false
    });

    return patterns;
}

// ============================================================================
// Complementary conditional jump patterns
// ============================================================================

std::vector<OverlapPattern> createComplementaryJumpPatterns() {
    std::vector<OverlapPattern> patterns;

    // 74 03 75 01 E8: JZ +3 / JNZ +1 — always jumps past E8 rogue byte
    // If ZF=1: JZ skips to after E8 (3 bytes forward: 75, 01, E8)
    // If ZF=0: JNZ skips E8 (1 byte forward)
    // Either way, E8 is never reached
    patterns.push_back({
        "comp_jz_jnz_call", "Complementary JZ/JNZ over rogue CALL",
        ArchTarget::Both,
        {0x74, 0x03, 0x75, 0x01, 0xE8},
        0,
        "JZ/JNZ pair always skips E8",
        "JZ +3; JNZ +1; CALL rel32",
        false
    });

    // 73 03 72 01 E9: JNC/JC pair over rogue JMP
    patterns.push_back({
        "comp_jnc_jc_jmp", "Complementary JNC/JC over rogue JMP",
        ArchTarget::Both,
        {0x73, 0x03, 0x72, 0x01, 0xE9},
        0,
        "JNC/JC pair always skips E9",
        "JNC +3; JC +1; JMP rel32",
        false
    });

    // 77 03 76 01 E8: JA/JBE pair
    patterns.push_back({
        "comp_ja_jbe_call", "Complementary JA/JBE over rogue CALL",
        ArchTarget::Both,
        {0x77, 0x03, 0x76, 0x01, 0xE8},
        0,
        "JA/JBE pair always skips E8",
        "JA +3; JBE +1; CALL rel32",
        false
    });

    // 7F 03 7E 01 E8: JG/JLE pair
    patterns.push_back({
        "comp_jg_jle_call", "Complementary JG/JLE over rogue CALL",
        ArchTarget::Both,
        {0x7F, 0x03, 0x7E, 0x01, 0xE8},
        0,
        "JG/JLE pair always skips E8",
        "JG +3; JLE +1; CALL rel32",
        false
    });

    return patterns;
}

// ============================================================================
// MOV with hidden JMP patterns
// ============================================================================

std::vector<OverlapPattern> createMovWithHiddenJmpPatterns() {
    std::vector<OverlapPattern> patterns;

    // 66 B8 EB 05: MOV AX, 0x05EB — the immediate contains EB 05 = JMP +5
    // After XOR EAX,EAX (31 C0) + JZ back to EB 05:
    // 66 B8 EB 05 31 C0 74 F9
    // Linear: MOV AX, 0x05EB; XOR EAX,EAX; JZ -7
    // Real path: MOV AX, 0x05EB; XOR EAX,EAX; JZ (taken, ZF=1) -> jumps to EB 05
    //   -> EB 05 = JMP +5 -> skips past remaining bytes
    patterns.push_back({
        "mov_hidden_jmp", "MOV AX with hidden JMP +5 in immediate",
        ArchTarget::Both,
        {0x66, 0xB8, 0xEB, 0x05, 0x31, 0xC0, 0x74, 0xF9},
        0,
        "MOV AX,imm; XOR EAX,EAX; JZ back to hidden JMP",
        "MOV AX, 0x05EB; XOR EAX,EAX; JZ -7",
        true // modifies EAX
    });

    // B8 EB 03 00 00: MOV EAX, 0x000003EB — contains EB 03 = JMP +3
    patterns.push_back({
        "mov_eax_hidden_jmp", "MOV EAX with hidden JMP in immediate bytes",
        ArchTarget::Both,
        {0xB8, 0xEB, 0x03, 0x00, 0x00},
        0,
        "MOV EAX, imm32 with EB 03 at offset 1",
        "MOV EAX, 0x000003EB",
        true // modifies EAX
    });

    return patterns;
}

// ============================================================================
// Multi-byte NOP wrapper patterns
// ============================================================================

std::vector<OverlapPattern> createNopWrapperPatterns() {
    std::vector<OverlapPattern> patterns;

    // 3-byte NOP: 0F 1F 00 (NOP DWORD PTR [EAX])
    // The bytes 1F 00 decode differently if jumped to
    patterns.push_back({
        "nop3_wrapper", "3-byte NOP with different inner interpretation",
        ArchTarget::Both,
        {0x0F, 0x1F, 0x00},
        0,
        "NOP (3 bytes, no effect)",
        "NOP DWORD PTR [EAX]",
        false
    });

    // 4-byte NOP: 0F 1F 40 00
    patterns.push_back({
        "nop4_wrapper", "4-byte NOP",
        ArchTarget::Both,
        {0x0F, 0x1F, 0x40, 0x00},
        0,
        "NOP (4 bytes)",
        "NOP DWORD PTR [EAX+0]",
        false
    });

    // 7-byte NOP: 0F 1F 80 00 00 00 00
    patterns.push_back({
        "nop7_wrapper", "7-byte NOP with large displacement",
        ArchTarget::Both,
        {0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00},
        0,
        "NOP (7 bytes)",
        "NOP DWORD PTR [EAX+0x00000000]",
        false
    });

    // 9-byte NOP: 66 0F 1F 84 00 00 00 00 00
    patterns.push_back({
        "nop9_wrapper", "9-byte NOP (maximum recommended)",
        ArchTarget::Both,
        {0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00},
        0,
        "NOP (9 bytes)",
        "66 NOP DWORD PTR [EAX+EAX*1+0x00000000]",
        false
    });

    return patterns;
}

// ============================================================================
// Rogue CALL byte patterns
// ============================================================================

std::vector<OverlapPattern> createRogueCallPatterns() {
    std::vector<OverlapPattern> patterns;

    // E8 followed by a jump over it: EB 05 E8 xx xx xx xx
    // JMP +5 skips the CALL and its 4-byte operand
    patterns.push_back({
        "rogue_call_skip", "JMP +5 over complete rogue CALL",
        ArchTarget::Both,
        {0xEB, 0x05, 0xE8, 0xDE, 0xAD, 0xBE, 0xEF},
        0,
        "JMP +5 skips fake CALL",
        "JMP +5; CALL 0xEFBEADDE",
        false
    });

    // Two consecutive rogue bytes with JMP
    // EB 06 E8 xx xx xx xx E9: skip CALL and JMP opcode
    patterns.push_back({
        "rogue_call_jmp", "JMP over rogue CALL + JMP opcode pair",
        ArchTarget::Both,
        {0xEB, 0x06, 0xE8, 0xAA, 0xBB, 0xCC, 0xDD, 0xE9},
        0,
        "JMP +6 skips CALL + start of JMP",
        "JMP +6; CALL ...; JMP rel32",
        false
    });

    return patterns;
}

// ============================================================================
// Prefix confusion patterns
// ============================================================================

std::vector<OverlapPattern> createPrefixConfusionPatterns() {
    std::vector<OverlapPattern> patterns;

    // 66 66 90: Redundant operand-size prefixes before NOP
    // Some disassemblers handle multiple prefixes differently
    patterns.push_back({
        "double_prefix_nop", "Double 66h prefix before NOP",
        ArchTarget::Both,
        {0x66, 0x66, 0x90},
        0,
        "NOP with redundant prefixes",
        "data16 data16 NOP",
        false
    });

    // F2 prefix before non-SSE instruction (ignored on some, errors on others)
    // F2 90: REPNE NOP — effectively NOP on modern processors
    patterns.push_back({
        "repne_nop", "REPNE prefix before NOP",
        ArchTarget::Both,
        {0xF2, 0x90},
        0,
        "REPNE NOP (harmless)",
        "REPNE NOP",
        false
    });

    return patterns;
}

// ============================================================================
// REX prefix tricks (x86-64 only)
// ============================================================================

std::vector<OverlapPattern> createRexPrefixPatterns() {
    std::vector<OverlapPattern> patterns;

    // 40-4F are REX prefixes on x86-64 but INC/DEC reg on x86-32
    // 48 90: REX.W NOP = XCHG RAX, RAX (NOP) on x64
    //        DEC EAX; NOP on x86-32
    patterns.push_back({
        "rex_nop", "REX.W NOP (x64) vs DEC EAX; NOP (x32)",
        ArchTarget::X86_64,
        {0x48, 0x90},
        0,
        "XCHG RAX, RAX (NOP on x64)",
        "REX.W NOP",
        false
    });

    // 41 EB 01 E8: on x64: REX.B prefix on JMP
    // Actually REX only affects ModRM-based instructions, not JMP rel8
    // So: 41 is INC ECX on x32, REX.B on x64 (ignored by JMP)
    patterns.push_back({
        "rex_jmp_skip", "REX.B prefix before JMP (benign on x64)",
        ArchTarget::X86_64,
        {0x41, 0xEB, 0x01, 0xE8},
        0,
        "REX.B JMP +1 -> skip E8",
        "INC ECX (x32) or REX JMP +1; CALL (x64)",
        false // REX.B is ignored before JMP rel8
    });

    return patterns;
}

// ============================================================================
// Exotic NOP patterns (undocumented/reserved NOP-like opcodes)
// ============================================================================

std::vector<OverlapPattern> createExoticNopPatterns() {
    std::vector<OverlapPattern> patterns;

    // -----------------------------------------------------------------
    // 1. 0F 1F with /1-/7 reg fields (undocumented hint NOPs)
    //    Official Intel NOP uses /0 (0F 1F /0). Reg fields /1-/7
    //    are reserved but execute as NOPs on P6+ CPUs.
    // -----------------------------------------------------------------

    patterns.push_back({
        "hint_nop_0f1f_r1", "Undocumented 0F 1F /1 hint NOP (reg=ECX)",
        ArchTarget::Both,
        {0x0F, 0x1F, 0xC8},
        0,
        "NOP (undocumented /1)",
        "NOP ECX (hint NOP /1)",
        false
    });

    patterns.push_back({
        "hint_nop_0f1f_r2", "Undocumented 0F 1F /2 hint NOP (reg=EDX)",
        ArchTarget::Both,
        {0x0F, 0x1F, 0xD0},
        0,
        "NOP (undocumented /2)",
        "NOP EDX (hint NOP /2)",
        false
    });

    patterns.push_back({
        "hint_nop_0f1f_r3", "Undocumented 0F 1F /3 hint NOP (reg=EBX)",
        ArchTarget::Both,
        {0x0F, 0x1F, 0xD8},
        0,
        "NOP (undocumented /3)",
        "NOP EBX (hint NOP /3)",
        false
    });

    patterns.push_back({
        "hint_nop_0f1f_r4", "Undocumented 0F 1F /4 hint NOP (reg=ESP)",
        ArchTarget::Both,
        {0x0F, 0x1F, 0xE0},
        0,
        "NOP (undocumented /4)",
        "NOP ESP (hint NOP /4)",
        false
    });

    patterns.push_back({
        "hint_nop_0f1f_r5", "Undocumented 0F 1F /5 hint NOP (reg=EBP)",
        ArchTarget::Both,
        {0x0F, 0x1F, 0xE8},
        0,
        "NOP (undocumented /5)",
        "NOP EBP (hint NOP /5)",
        false
    });

    patterns.push_back({
        "hint_nop_0f1f_r6", "Undocumented 0F 1F /6 hint NOP (reg=ESI)",
        ArchTarget::Both,
        {0x0F, 0x1F, 0xF0},
        0,
        "NOP (undocumented /6)",
        "NOP ESI (hint NOP /6)",
        false
    });

    patterns.push_back({
        "hint_nop_0f1f_r7", "Undocumented 0F 1F /7 hint NOP (reg=EDI)",
        ArchTarget::Both,
        {0x0F, 0x1F, 0xF8},
        0,
        "NOP (undocumented /7)",
        "NOP EDI (hint NOP /7)",
        false
    });

    // -----------------------------------------------------------------
    // 2. 0F 19 through 0F 1E hint NOPs (all /0 reg variants)
    //    These opcode map entries are reserved as hint NOPs on P6+.
    // -----------------------------------------------------------------

    patterns.push_back({
        "hint_nop_0f19", "Reserved hint NOP 0F 19 /0",
        ArchTarget::Both,
        {0x0F, 0x19, 0xC0},
        0,
        "NOP (0F 19 hint)",
        "hint NOP 0F 19 C0",
        false
    });

    patterns.push_back({
        "hint_nop_0f1a", "Reserved hint NOP 0F 1A /0",
        ArchTarget::Both,
        {0x0F, 0x1A, 0xC0},
        0,
        "NOP (0F 1A hint)",
        "hint NOP 0F 1A C0",
        false
    });

    patterns.push_back({
        "hint_nop_0f1b", "Reserved hint NOP 0F 1B /0",
        ArchTarget::Both,
        {0x0F, 0x1B, 0xC0},
        0,
        "NOP (0F 1B hint)",
        "hint NOP 0F 1B C0",
        false
    });

    patterns.push_back({
        "hint_nop_0f1c", "Reserved hint NOP 0F 1C /0",
        ArchTarget::Both,
        {0x0F, 0x1C, 0xC0},
        0,
        "NOP (0F 1C hint)",
        "hint NOP 0F 1C C0",
        false
    });

    patterns.push_back({
        "hint_nop_0f1d", "Reserved hint NOP 0F 1D /0",
        ArchTarget::Both,
        {0x0F, 0x1D, 0xC0},
        0,
        "NOP (0F 1D hint)",
        "hint NOP 0F 1D C0",
        false
    });

    patterns.push_back({
        "hint_nop_0f1e", "Reserved hint NOP 0F 1E /0",
        ArchTarget::Both,
        {0x0F, 0x1E, 0xC0},
        0,
        "NOP (0F 1E hint)",
        "hint NOP 0F 1E C0",
        false
    });

    // -----------------------------------------------------------------
    // 3. 0F 19-1E with SIB+displacement for longer encodings
    // -----------------------------------------------------------------

    patterns.push_back({
        "hint_nop_0f19_sib_disp32", "8-byte hint NOP using 0F 19 with SIB+disp32",
        ArchTarget::Both,
        {0x0F, 0x19, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00},
        0,
        "NOP (0F 19 SIB+disp32, 8 bytes)",
        "hint NOP [EAX+EAX*1+0x00000000]",
        false
    });

    patterns.push_back({
        "hint_nop_0f1a_sib", "5-byte hint NOP using 0F 1A with SIB",
        ArchTarget::Both,
        {0x0F, 0x1A, 0x44, 0x00, 0x00},
        0,
        "NOP (0F 1A SIB+disp8, 5 bytes)",
        "hint NOP [EAX+EAX*1+0x00]",
        false
    });

    patterns.push_back({
        "hint_nop_0f1d_exotic_sib", "8-byte hint NOP with unusual SIB (0F 1D)",
        ArchTarget::Both,
        {0x0F, 0x1D, 0x84, 0xDB, 0x78, 0x56, 0x34, 0x12},
        0,
        "NOP (0F 1D exotic SIB+disp32, 8 bytes)",
        "hint NOP [EBX+EBX*8+0x12345678]",
        false
    });

    // -----------------------------------------------------------------
    // 4. CET-era NOPs (ENDBR64/ENDBR32)
    //    Execute as NOP on non-CET-enabled CPUs.
    // -----------------------------------------------------------------

    patterns.push_back({
        "cet_endbr64", "ENDBR64 - NOP on non-CET CPUs (F3 0F 1E FA)",
        ArchTarget::Both,
        {0xF3, 0x0F, 0x1E, 0xFA},
        0,
        "ENDBR64 (NOP on non-CET)",
        "ENDBR64",
        false
    });

    patterns.push_back({
        "cet_endbr32", "ENDBR32 - NOP on non-CET CPUs (F3 0F 1E FB)",
        ArchTarget::Both,
        {0xF3, 0x0F, 0x1E, 0xFB},
        0,
        "ENDBR32 (NOP on non-CET)",
        "ENDBR32",
        false
    });

    // -----------------------------------------------------------------
    // 5. Prefix-padded NOPs (stacked redundant prefixes)
    // -----------------------------------------------------------------

    patterns.push_back({
        "prefix_double_66_nop", "3-byte double-prefix NOP (66 66 90)",
        ArchTarget::Both,
        {0x66, 0x66, 0x90},
        0,
        "NOP (double 66h prefix, 3 bytes)",
        "data16 data16 NOP",
        false
    });

    patterns.push_back({
        "prefix_triple_66_nop", "4-byte triple-prefix NOP (66 66 66 90)",
        ArchTarget::Both,
        {0x66, 0x66, 0x66, 0x90},
        0,
        "NOP (triple 66h prefix, 4 bytes)",
        "data16 data16 data16 NOP",
        false
    });

    patterns.push_back({
        "prefix_es_66_nop", "3-byte ES+66h NOP (26 66 90)",
        ArchTarget::Both,
        {0x26, 0x66, 0x90},
        0,
        "NOP (ES override + data16, 3 bytes)",
        "ES: data16 NOP",
        false
    });

    patterns.push_back({
        "prefix_cs_ss_66_nop", "4-byte CS+SS+66h NOP (2E 36 66 90)",
        ArchTarget::Both,
        {0x2E, 0x36, 0x66, 0x90},
        0,
        "NOP (CS+SS override + data16, 4 bytes)",
        "CS: SS: data16 NOP",
        false
    });

    patterns.push_back({
        "prefix_fs_gs_66_67_nop", "5-byte FS+GS+66h+67h NOP (64 65 66 67 90)",
        ArchTarget::Both,
        {0x64, 0x65, 0x66, 0x67, 0x90},
        0,
        "NOP (FS+GS+data16+addr16, 5 bytes)",
        "FS: GS: data16 addr16 NOP",
        false
    });

    // -----------------------------------------------------------------
    // 6. 0F 1F with unusual SIB bytes (non-standard addressing)
    // -----------------------------------------------------------------

    patterns.push_back({
        "nop_0f1f_sib_edi_edi", "0F 1F with SIB scale=3 index=EDI base=EDI",
        ArchTarget::Both,
        {0x0F, 0x1F, 0x04, 0xFF},
        0,
        "NOP [EDI+EDI*8] (unusual SIB)",
        "NOP DWORD PTR [EDI+EDI*8]",
        false
    });

    patterns.push_back({
        "nop_0f1f_sib_ebx_ebx_disp32", "0F 1F with SIB scale=3 index=EBX base=EBX disp32",
        ArchTarget::Both,
        {0x0F, 0x1F, 0x84, 0xDB, 0x78, 0x56, 0x34, 0x12},
        0,
        "NOP [EBX+EBX*8+0x12345678] (exotic SIB+disp32)",
        "NOP DWORD PTR [EBX+EBX*8+0x12345678]",
        false
    });

    patterns.push_back({
        "nop_0f1f_sib_noindex_disp32", "0F 1F with SIB no-index disp32",
        ArchTarget::Both,
        {0x0F, 0x1F, 0x04, 0xE5, 0x00, 0x00, 0x00, 0x00},
        0,
        "NOP [disp32] (SIB no-index encoding)",
        "NOP DWORD PTR [0x00000000]",
        false
    });

    // -----------------------------------------------------------------
    // 7. 0F 0D prefetch-NOPs
    //    NOP on Intel Cedar Mill+, prefetch hint on AMD.
    //    Safe as NOP on all modern CPUs.
    // -----------------------------------------------------------------

    patterns.push_back({
        "prefetch_nop_0f0d_r0", "Prefetch-NOP 0F 0D /0 (NOP on modern Intel)",
        ArchTarget::Both,
        {0x0F, 0x0D, 0x00},
        0,
        "NOP (prefetch hint /0)",
        "PREFETCH [EAX] /0",
        false
    });

    patterns.push_back({
        "prefetch_nop_0f0d_r1", "Prefetch-NOP 0F 0D /1 (PREFETCHW on AMD, NOP on Intel)",
        ArchTarget::Both,
        {0x0F, 0x0D, 0x08},
        0,
        "NOP (prefetch hint /1)",
        "PREFETCHW [EAX] /1",
        false
    });

    patterns.push_back({
        "prefetch_nop_0f0d_r3", "Undocumented prefetch-NOP 0F 0D /3 (NOP on all modern CPUs)",
        ArchTarget::Both,
        {0x0F, 0x0D, 0x18},
        0,
        "NOP (undocumented prefetch /3)",
        "PREFETCH [EAX] /3 (undocumented)",
        false
    });

    return patterns;
}

// ============================================================================
// Get all patterns / pick random
// ============================================================================

std::vector<OverlapPattern> getAllPatterns(ArchTarget arch) {
    std::vector<OverlapPattern> all;
    auto append = [&all, arch](std::vector<OverlapPattern>&& v) {
        for (auto& p : v) {
            if (arch == ArchTarget::Both || p.arch == arch || p.arch == ArchTarget::Both) {
                all.push_back(std::move(p));
            }
        }
    };

    append(createJmpSkipPatterns());
    append(createJmpIntoOperandPatterns());
    append(createComplementaryJumpPatterns());
    append(createMovWithHiddenJmpPatterns());
    append(createNopWrapperPatterns());
    append(createRogueCallPatterns());
    append(createPrefixConfusionPatterns());
    append(createRexPrefixPatterns());
    append(createExoticNopPatterns());

    return all;
}

std::vector<OverlapPattern> pickPatterns(ObfRNG& rng, ArchTarget arch, int count) {
    auto all = getAllPatterns(arch);
    std::vector<OverlapPattern> picked;

    if (all.empty()) return picked;

    for (int i = 0; i < count; i++) {
        picked.push_back(all[rng.nextInRange(0, (uint32_t)all.size() - 1)]);
    }

    return picked;
}

// ============================================================================
// Validation
// ============================================================================

bool validatePattern(const OverlapPattern& pattern) {
    // Basic validation: pattern must not be empty
    if (pattern.bytes.empty()) return false;

    // For JMP-based patterns, verify the first byte is a JMP or Jcc opcode
    uint8_t first = pattern.bytes[0];
    bool isJmpOrJcc = (first == 0xEB) || (first == 0xE9) ||
                       (first >= 0x70 && first <= 0x7F) || // Jcc short
                       (first == 0x0F) || // Multi-byte NOP or Jcc near
                       (first == 0x66) || // Operand size prefix
                       (first == 0x67) || // Address size prefix
                       (first == 0xF2) || // REPNE prefix
                       (first == 0xF3) || // REP/REPE prefix (CET NOPs)
                       (first == 0x26) || // ES segment override
                       (first == 0x2E) || // CS segment override
                       (first == 0x36) || // SS segment override
                       (first == 0x64) || // FS segment override
                       (first == 0x65) || // GS segment override
                       (first == 0xB8) || // MOV EAX, imm32
                       (first >= 0x40 && first <= 0x4F); // REX prefixes (x64)

    if (!isJmpOrJcc && first != 0x41) return false;

    // Pattern bytes must be representable
    if (pattern.bytes.size() > 15) return false; // x86 max instruction length

    return true;
}

} // namespace shroud
