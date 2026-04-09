<p align="center">
  <h1 align="center">Shroud</h1>
  <p align="center">
    LLVM-based binary obfuscation framework
    <br />
    <em>No compiler fork. No runtime dependency. Just passes.</em>
  </p>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/tests-98%2F98-brightgreen" alt="Tests">
  <img src="https://img.shields.io/badge/LLVM-18--22-blue" alt="LLVM">
  <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey" alt="Platform">
  <img src="https://img.shields.io/badge/arch-x86__64%20%7C%20x86-orange" alt="Architecture">
  <img src="https://img.shields.io/badge/license-research-yellow" alt="License">
</p>

---

Shroud transforms clean C/C++ source into binaries that resist static analysis, decompilation, and reverse engineering. It operates as a set of out-of-tree LLVM passes that slot into any standard `clang`/`opt` toolchain.

Three complementary strategies target different layers of analysis:

- **Dynamic MBA** corrupts data flow with randomly-generated arithmetic identities
- **Opaque predicates** shatter control flow with mathematically-proven conditions and deep bogus block chains
- **Overlapping instructions** exploit x86 variable-length encoding to break disassemblers at the byte level
- **String hiding** replaces string constants with per-character MBA expressions computed at runtime

Combined, they produce binaries that IDA Pro's Hex-Rays decompiler refuses to process entirely.

## Highlights

| | Plain | Shroud | Shroud + LTO |
|---|---|---|---|
| Binary size | 28 KB | 3.5 MB | 6.9 MB |
| `main()` instructions | 36 | 873,226 | ~1.5M |
| Unique constants | ~5 | 711+ | 1,000+ |
| Hex-Rays | Readable | **FAILS** | **FAILS** |
| `strings` output | Visible | Visible | **Hidden** |

## Quick Start

```bash
# 1. Compile to LLVM IR (O2 inlines CRT functions)
clang -S -emit-llvm -O2 -o program.ll program.c

# 2. Obfuscate
opt -load-pass-plugin=./ShroudPasses.dll \
    -passes="mba-obf,opaque-pred,overlap-inst" \
    -S program.ll -o obf.ll

# 3. Hide strings
opt -load-pass-plugin=./ShroudPasses.dll \
    -passes="string-hide" -S obf.ll -o final.ll

# 4. Compile (O0 prevents optimizer from undoing MBA)
clang -O0 -o program.exe final.ll
```

## Features

### Dynamic MBA (Mixed Boolean-Arithmetic)

Every arithmetic operation is replaced with a unique expression using random coefficients generated via truth-table linear algebra and null-space perturbation. No two sites in the binary look the same.

```
x + y  →  2793041*(~x&~y) + 1847293*(~x&y) + 3901827*(x&~y) + 892716*(x&y) + ...
```

- 3 rounds of substitution (each round transforms operations from previous rounds)
- NOT-free implementation (no `XOR reg, 0xFFFFFFFF` patterns)
- Random minterm computation algebra (3 forms per minterm, shuffled per site)
- Random noise injection `(expr - expr = 0)` between terms
- Constant obfuscation: integer literals become MBA expressions

### Opaque Predicates

55+ predicate types across 28 mathematical categories, randomly mixed as always-true or always-false:

| Category | Examples |
|----------|---------|
| Number theory | Fermat's little theorem, quadratic residues, consecutive products |
| Post-quantum | Lattice SIS/LWE, multivariate quadratic, error-correcting codes |
| Algebraic geometry | Elliptic curve point verification, Weil pairing |
| Combinatorics | Fibonacci identities, Catalan numbers, Lucas sequences |
| Analysis | Continued fractions, Parseval/Hadamard theorem |
| Game theory | Nash equilibrium, Sprague-Grundy (Nim) |
| Automata theory | DFA acceptance, pumping lemma |
| Polynomial algebra | Bezout's identity, irreducibility witnesses, LFSR periods |

Each block gets 1-4 chained predicate gates. Each gate has 2-5 deep bogus block chains. Predicates are randomly negated so the analyst can't assume "true branch = real path."

### Overlapping Instructions

Fully dynamic anti-disassembly — every instance is uniquely generated:

- **10 opaque condition strategies** (XOR/SUB/STC/CLC/MOV+TEST/AND/CMP)
- **Random register** per instance (EAX/EBX/ECX/EDX/ESI/EDI)
- **Random rogue bytes** (E8/E9/68/48B8/FF15/0F0B/CC with random operands)
- **Exotic NOP overlap**: anti-disasm hidden inside displacement fields of undocumented `0F 19`-`0F 1E` hint NOPs
- **NOP as control flow**: exotic NOPs become load-bearing CFG edges (stripping them crashes the binary)
- **Undocumented encodings**: `0F 1F /1-/7`, CET ENDBR, prefix stacking, exotic SIB

### String Hiding

String constants are replaced with `zeroinitializer` and rebuilt at runtime via a global constructor that computes each character through MBA expressions. No key, no decryption loop, `strings` finds nothing.

## Available Passes

| Pass | Description |
|------|------------|
| `mba-obf` | Dynamic MBA substitution (3 rounds, random coefficients) |
| `opaque-pred` | Chained opaque predicate gates with bogus block chains |
| `overlap-inst` | Dynamic anti-disassembly with exotic NOP overlap |
| `string-hide` | MBA-based string constant hiding |

Passes can be combined freely:

```bash
# Individual
opt -load-pass-plugin=./ShroudPasses.dll -passes="mba-obf" -S in.ll -o out.ll

# Combined
opt -load-pass-plugin=./ShroudPasses.dll -passes="mba-obf,opaque-pred,overlap-inst" -S in.ll -o out.ll

# String hiding (separate step, runs as module pass)
opt -load-pass-plugin=./ShroudPasses.dll -passes="string-hide" -S in.ll -o out.ll
```

## Building

### Prerequisites

<details>
<summary><strong>Windows</strong></summary>

```bash
winget install LLVM.LLVM
winget install Kitware.CMake
winget install Ninja-build.Ninja
winget install Microsoft.VisualStudio.2022.BuildTools --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621"
```

Download the LLVM dev distribution from [GitHub releases](https://github.com/llvm/llvm-project/releases) (`clang+llvm-*-x86_64-pc-windows-msvc.tar.xz`).

</details>

<details>
<summary><strong>Linux</strong></summary>

```bash
sudo apt install llvm-18-dev clang-18 lld-18 cmake ninja-build
```

</details>

### Core Library Only (no LLVM dependency)

```bash
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
ninja && ctest
```

### Full Build with Pass Plugin

<details>
<summary><strong>Windows (MSVC)</strong></summary>

```bash
LLVM_DEV="C:/llvm-dev/clang+llvm-22.1.2-x86_64-pc-windows-msvc"

mkdir build && cd build
cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="../msvc-toolchain.cmake" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded \
    -DBUILD_LLVM_PLUGIN=ON \
    -DLLVM_DIR="$LLVM_DEV/lib/cmake/llvm" \
    ..
ninja
```

Output: `pass/ShroudPasses.dll`

</details>

<details>
<summary><strong>Linux</strong></summary>

```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_LLVM_PLUGIN=ON ..
ninja
```

Output: `pass/ShroudPasses.so`

</details>

## Architecture

```
                    ┌─────────────────────────────┐
                    │   Core Library (pure C++17)  │
                    │   No LLVM dependency         │
                    │   98 unit tests              │
                    ├──────────┬──────────┬────────┤
                    │   MBA    │  Opaque  │Overlap │
                    │Generator │Predicates│Patterns│
                    └────┬─────┴────┬─────┴───┬────┘
                         │          │         │
                    ┌────▼──────────▼─────────▼────┐
                    │   LLVM Pass Plugin (thin)    │
                    │   ShroudPasses.dll/.so        │
                    ├──────────┬──────────┬────────┤
                    │ MBAPass  │OpaquePass│Overlap │
                    │          │          │  Pass  │
                    └────┬─────┴────┬─────┴───┬────┘
                         │          │         │
              ┌──────────▼──────────▼─────────▼──────────┐
              │        clang / opt toolchain              │
              │   clang -emit-llvm → opt → clang -O0     │
              └──────────────────────────────────────────┘
```

## Testing

98 GoogleTest cases across 4 test suites:

- **MBA**: Exhaustive 8-bit + random 32-bit verification of all substitution rules, dynamic generation, null-space vectors, constant obfuscation, multi-round chaining
- **Opaque Predicates**: All 55+ predicates verified for correctness across input ranges
- **Overlapping Instructions**: All 48+ patterns validated, architecture filtering, diversity checks
- **Integration**: Combined techniques, full pipeline semantics, large-scale random 32-bit (100K+ samples)

```bash
cd build && ctest --output-on-failure
# 98/98 tests passed
```

## Demo

The included crackme (`crackme/crackme.c`) is a password checker that demonstrates the full pipeline:

```bash
# Plain
clang -O1 -o crackme crackme/crackme.c
./crackme "s3cr3t_k3y"   # Access Granted!

# Obfuscated
clang -S -emit-llvm -O2 -o crackme.ll crackme/crackme.c
opt -load-pass-plugin=./ShroudPasses.dll -passes="mba-obf,opaque-pred,overlap-inst" -S crackme.ll -o step1.ll
opt -load-pass-plugin=./ShroudPasses.dll -passes="string-hide" -S step1.ll -o step2.ll
clang -O0 -o crackme_obf step2.ll

./crackme_obf "s3cr3t_k3y" # Access Granted!
strings crackme_obf | grep -i access  # (nothing)
```

## Project Structure

```
shroud/
├── include/Shroud/          # Public headers
│   ├── MBA.h                # Truth-table MBA generation
│   ├── OpaquePredicates.h   # 55+ predicate types
│   ├── OverlappingInstructions.h  # 48+ anti-disasm patterns
│   └── Utils.h              # RNG, modular arithmetic
├── lib/                     # Core library (pure C++, no LLVM)
├── pass/                    # LLVM pass plugin
│   ├── MBAPass.cpp          # Dynamic MBA substitution
│   ├── OpaquePredicatesPass.cpp   # Chained predicate gates
│   ├── OverlappingInstructionsPass.cpp  # Dynamic anti-disasm
│   ├── StringHidePass.cpp   # MBA string hiding
│   └── PassPlugin.cpp       # Plugin registration
├── test/                    # 98 GoogleTest cases
└── crackme/                 # Demo target
```

## Limitations

- **x86/x86-64 only** — anti-disassembly patterns are x86-specific
- **C/C++ via clang** — requires LLVM IR as input
- **No debug info preservation** — obfuscation strips debuggability by design
- **Binary size increase** — expect 10-100x depending on pass configuration
- **Not for hot loops** — MBA overhead is significant; intended for cold protection code

## Contributing

PRs welcome for:
- New opaque predicate families (number theory, algebra, topology)
- Additional anti-disassembly patterns (ARM, RISC-V)
- Deobfuscation resistance testing and bypass reports
- Platform support (macOS, cross-compilation)

## License

This project is for educational and security research purposes.
