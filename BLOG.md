# Jumping Into NOPs: How Exotic x86 Encodings Break Every Disassembler

I've been building [Shroud](https://github.com/pickpocket/shroud), an LLVM-based obfuscation framework, and stumbled onto a technique that's been unreasonably effective at breaking static analysis tools. The core idea is simple: encode real control flow inside the displacement field of undocumented x86 NOP instructions, making them load-bearing parts of the program's CFG. Strip the NOP, crash the binary. Keep the NOP, miss the hidden execution path.

## Background: x86 hint NOPs nobody talks about

Intel documents `0F 1F /0` as the official multi-byte NOP. What they don't document (but every P6+ CPU since 1995 silently executes) is that the entire `0F 19` through `0F 1E` range, with all ModRM reg field values `/0` through `/7`, behaves identically as hint NOPs. That's over a hundred undocumented NOP encodings hiding in plain sight.

The interesting part is their encoding format. An 8-byte hint NOP like `0F 1D 84 00 XX XX XX XX` breaks down as:

```
0F 1D       → opcode (reserved hint NOP)
84          → ModRM: mod=10, reg=000, rm=100 (SIB follows)
00          → SIB: scale=0, index=EAX, base=EAX
XX XX XX XX → 32-bit displacement (can be ANYTHING)
```

The CPU reads all 8 bytes as one instruction, does nothing, and moves on. The displacement field is completely ignored. But here's the thing: those 4 displacement bytes are still valid x86 machine code if you enter them at a different offset.

## The technique: NOPs as control flow bridges

The idea is to place an exotic NOP in the instruction stream with a JMP that lands inside its displacement field. Linear disassembly sees one 8-byte NOP instruction. Execution threads through the NOP's guts.

```nasm
    jmp .Ldisp              ; skip NOP header
    .byte 0x0F, 0x1D, 0x84, 0x00  ; NOP header (dead)
.Ldisp:                     ; byte 4 of the NOP = displacement field
    jmp .Lexit              ; assembler resolves this relative offset
    .byte 0xAB, 0xCD       ; padding (dead, but completes NOP displacement)
.Lexit:                     ; real code continues
```

IDA, Binary Ninja, and Ghidra all see `0F 1D 84 00 [EB xx AB CD]` as a single 8-byte NOP instruction with displacement `0xCDABxxEB`. They skip past it. But the CPU never executes the NOP — it enters at `.Ldisp` and runs the `JMP .Lexit` that's hiding in the displacement bytes.

## Why this actually breaks things

Most anti-disassembly tricks (the `EB 01 E8` pattern, complementary conditional jumps, etc.) are well-known idioms that modern disassemblers handle. IDA has recognized `JMP +1` over rogue bytes since version 5. These exotic NOP overlaps are different because:

**1. The NOP is load-bearing.** If an automated tool strips NOPs as a cleanup pass (which many deobfuscators do), the hidden JMP inside the displacement disappears and the program crashes. The NOP isn't decoration — it's the bridge between basic blocks.

**2. No disassembler represents overlapping instructions.** Every disassembly format assumes each byte belongs to exactly one instruction. The displacement bytes are simultaneously part of the NOP (from the NOP's start offset) and a valid JMP instruction (from the displacement offset). There's no way to show both interpretations in a standard listing.

**3. The hidden code is real computation, not just jumps.** In Shroud, the displacement bytes can contain actual register arithmetic:

```nasm
    jmp .Ldisp
    .byte 0x0F, 0x1A, 0x84, 0x00  ; exotic NOP header
.Ldisp:
    xor ecx, ecx            ; real computation (hidden in NOP displacement)
    add ecx, 42             ; ECX = 42
    jmp .Lexit               ; continue to real code
    .byte 0xDE, 0xAD, 0xBE  ; dead padding
.Lexit:
```

Linear disasm: one NOP. Execution: zeroes ECX, sets it to 42, continues. A decompiler analyzing the NOP sees no data flow. The value 42 materializes from nowhere.

**4. They chain.** Shroud can thread execution through 2-3 nested exotic NOPs before reaching real code:

```
entry → JMP into NOP₁ disp → JMP into NOP₂ disp → JMP into NOP₃ disp → exit
```

Linear disassembly sees three consecutive NOPs. Execution zigzags through all their displacement fields. Each NOP uses a different opcode (`0F 19`, `0F 1C`, `0F 1E`), different SIB bytes, different displacement values — no pattern to match.

**5. The opaque predicate decides which path enters the NOP.** The JMP into the displacement is guarded by an opaque predicate (XOR self → JZ, SUB self → JNZ, STC → JNC, etc.) that's randomly either always-true or always-false. The disassembler can't determine which branch is taken without symbolic execution, so it can't know whether the NOP is entered from the header or from the displacement.

## What IDA sees vs. what executes

Here's an actual snippet from a Shroud-obfuscated binary:

**IDA's view:**
```
.text:0x140355198  nop     dword ptr [rax+rax+12345678h]    ; 8-byte hint NOP
.text:0x1403551A0  xor     eax, eax
```

**What actually executes:**
```
; enters at byte 4 of the NOP (displacement field)
0x14035519C:  jmp     0x1403551A0     ; hidden in displacement bytes
; lands on:
0x1403551A0:  xor     eax, eax
```

The NOP is the only path to the XOR instruction. Without it, there's no control flow edge and the function is broken.

## Results

From a clean 28KB crackme to a 9MB binary where Hex-Rays refuses to decompile `main()` entirely. The function has 500+ basic blocks, 873,000+ instructions, and exotic NOPs woven throughout as structural elements of the control flow graph.

Every anti-disasm instance is uniquely generated: random NOP opcode (`0F 19`-`0F 1F`), random SIB byte, random register for the opaque predicate, random computation in the displacement. No signature matches more than one instance.

The full framework is at [github.com/pickpocket/shroud](https://github.com/pickpocket/shroud).

---

*pickpocket — AresX*
