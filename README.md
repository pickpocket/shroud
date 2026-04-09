# Shroud

**Shroud** is an LLVM-based code obfuscation framework that transforms clean C/C++ source into binaries that resist static analysis, decompilation, and reverse engineering. It operates as a set of out-of-tree LLVM passes that slot into any standard clang/opt toolchain — no compiler fork required.

Shroud layers three complementary obfuscation strategies that target different levels of analysis: **dynamic MBA expressions** corrupt data flow with randomly-generated arithmetic identities, **opaque predicates** shatter control flow with mathematically-proven always-true conditions backed by deep bogus block chains, and **overlapping instruction patterns** exploit x86 variable-length encoding to break disassembler assumptions at the byte level. Each technique is independently useful; combined, they produce binaries that IDA Pro's Hex-Rays decompiler refuses to process entirely.

The framework is built for extensibility. The core obfuscation algorithms live in a pure C++ library with no LLVM dependency, fully covered by 98 unit tests. The LLVM integration is a thin pass plugin layer. This architecture makes it straightforward to add new predicate families, MBA generation strategies, or anti-disassembly patterns — and to eventually layer a VM-based obfuscator on top.

## What it does

Takes clean C/C++ source code and produces a binary that:
- **Defeats Hex-Rays decompiler** (IDA Pro returns no result)
- Replaces every arithmetic/bitwise operation with **randomly-coefficiented MBA expressions** unique per operation site (711+ unique constants, zero `0xFFFFFFFF` patterns)
- Injects **opaque predicate branches** with 2-5 deep bogus block chains using 55+ predicate types from number theory, lattice cryptography, elliptic curves, cellular automata, game theory, and more
- Scatters **anti-disassembly byte patterns** including undocumented x86 hint NOPs, CET ENDBR NOPs, prefix-padded NOPs, and JMP-over-rogue-byte sequences
- With LTO, inlines CRT functions (`strlen`, etc.) so they get obfuscated too

### Example output (crackme, password checker)

| Metric | Plain | Obfuscated | LTO Obfuscated |
|--------|-------|------------|----------------|
| Binary size | 28KB | 3.5MB | 6.9MB |
| `main()` instructions | 36 | 873,226 | ~1.5M |
| Unique hex constants | ~5 | 711 | 1000+ |
| Hex-Rays decompile | Clean | **FAILS** | **FAILS** |
| Basic blocks in `main` | 8 | 532 | 800+ |

## Project Structure

```
llvm-obfuscator/
├── CMakeLists.txt                          # Top-level build
├── msvc-toolchain.cmake                    # MSVC/clang-cl toolchain file
├── include/Shroud/
│   ├── Utils.h                             # RNG, modular arithmetic, CRC32
│   ├── OpaquePredicates.h                  # 55+ predicate types
│   ├── MBA.h                               # Dynamic truth-table MBA generation
│   └── OverlappingInstructions.h           # 48+ anti-disassembly patterns
├── lib/                                    # Core library (pure C++, no LLVM dependency)
│   ├── Utils.cpp
│   ├── OpaquePredicates.cpp
│   ├── MBA.cpp
│   └── OverlappingInstructions.cpp
├── pass/                                   # LLVM pass plugin (requires LLVM dev headers)
│   ├── PassPlugin.cpp                      # Plugin entry point, registers all passes
│   ├── MBAPass.{h,cpp}                     # MBA substitution pass
│   ├── OpaquePredicatesPass.{h,cpp}        # Opaque predicate insertion pass
│   └── OverlappingInstructionsPass.{h,cpp} # Anti-disassembly pass
├── test/                                   # GoogleTest unit tests (98 tests)
│   ├── OpaquePredicatesTest.cpp
│   ├── MBATest.cpp
│   ├── OverlappingInstructionsTest.cpp
│   └── IntegrationTest.cpp
└── crackme/                                # Demo target
    ├── crackme.c                           # Clean password checker
    └── crackme_obfuscated.cpp              # Same code, compiled through plugin
```

## Prerequisites

### Windows

1. **LLVM** (clang + opt + dev headers):
   ```
   winget install LLVM.LLVM
   ```
   For the pass plugin, you also need the full dev distribution:
   ```
   # Download from GitHub releases:
   # https://github.com/llvm/llvm-project/releases
   # Get: clang+llvm-XX.X.X-x86_64-pc-windows-msvc.tar.xz
   # Extract to e.g. C:\llvm-dev\clang+llvm-XX.X.X-x86_64-pc-windows-msvc
   ```

2. **Visual Studio Build Tools** (for MSVC libraries):
   ```
   winget install Microsoft.VisualStudio.2022.BuildTools --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621"
   ```

3. **CMake** and **Ninja**:
   ```
   winget install Kitware.CMake
   winget install Ninja-build.Ninja
   ```

4. **LLVM-MinGW** (for building/testing the core library without MSVC):
   ```
   winget install MartinStorsjo.LLVM-MinGW.UCRT
   ```

### Linux

```bash
sudo apt install llvm-18-dev clang-18 lld-18 cmake ninja-build
```

## Building

### Option A: Core library + tests only (no LLVM plugin)

Uses LLVM-MinGW or any C++17 compiler. No LLVM dev headers needed.

```bash
mkdir build && cd build
cmake .. -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DBUILD_TESTS=ON \
    -DBUILD_LLVM_PLUGIN=OFF
ninja
ctest --output-on-failure
```

### Option B: Full build with LLVM pass plugin (Windows/MSVC)

Requires LLVM dev distribution + Visual Studio Build Tools.

```bash
# Set paths (adjust versions as needed)
LLVM_DEV="C:/llvm-dev/clang+llvm-22.1.2-x86_64-pc-windows-msvc"

mkdir build && cd build
cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="../msvc-toolchain.cmake" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded \
    -DBUILD_LLVM_PLUGIN=ON \
    -DBUILD_TESTS=OFF \
    -DLLVM_DIR="$LLVM_DEV/lib/cmake/llvm" \
    ..
ninja
```

This produces `pass/ShroudPasses.dll`.

### Option B: Full build with LLVM pass plugin (Linux)

```bash
mkdir build && cd build
cmake -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_LLVM_PLUGIN=ON \
    -DBUILD_TESTS=ON \
    ..
ninja
ctest --output-on-failure
```

This produces `pass/ShroudPasses.so`.

## Usage

### Obfuscating a binary (3-step pipeline)

```bash
# Step 1: Compile to LLVM IR
#   -O1 or -O2 recommended (inlines helper functions)
#   -O2 inlines CRT functions like strlen for maximum obfuscation
clang -S -emit-llvm -O2 -o program.ll program.c

# Step 2: Apply obfuscation passes
opt -load-pass-plugin=./ShroudPasses.dll \
    -passes="mba-obf,opaque-pred,overlap-inst" \
    -S program.ll -o program_obf.ll

# Step 3: Compile to native binary
#   -O0 prevents the optimizer from simplifying MBA expressions back
clang -O0 -o program.exe program_obf.ll
```

### Available passes

| Pass name | What it does |
|-----------|-------------|
| `mba-obf` | Replaces ADD/SUB/XOR/AND/OR with dynamic MBA expressions (3 rounds, random coefficients per site) |
| `opaque-pred` | Inserts always-true conditional branches with 2-5 deep bogus block chains |
| `overlap-inst` | Injects anti-disassembly inline asm (JMP-over-rogue, exotic NOPs, prefix confusion) |
| `obfuscate-all` | Runs all three in sequence |

### Individual pass usage

```bash
# MBA only
opt -load-pass-plugin=./ShroudPasses.dll -passes="mba-obf" -S in.ll -o out.ll

# Opaque predicates only
opt -load-pass-plugin=./ShroudPasses.dll -passes="opaque-pred" -S in.ll -o out.ll

# Anti-disassembly only
opt -load-pass-plugin=./ShroudPasses.dll -passes="overlap-inst" -S in.ll -o out.ll

# All passes
opt -load-pass-plugin=./ShroudPasses.dll -passes="obfuscate-all" -S in.ll -o out.ll
```

### LTO mode (obfuscates inlined CRT functions)

Using `-O2` in step 1 causes the compiler to inline `strlen`, `memcpy`, and other CRT functions into your code before the obfuscator runs. This means CRT code gets obfuscated too:

```bash
# -O2 inlines CRT functions, then obfuscate, then compile at -O0
clang -S -emit-llvm -O2 -o program.ll program.c
opt -load-pass-plugin=./ShroudPasses.dll -passes="obfuscate-all" -S program.ll -o program_obf.ll
clang -O0 -o program.exe program_obf.ll
```

## How it works

### MBA (Mixed Boolean-Arithmetic)

Every `x + y` becomes something like:
```
2793041*(~x&~y) + 1847293*(~x&y) + 3901827*(x&~y) + 892716*(x&y) + ...
```
The coefficients are generated by solving a linear system over the 16 bitwise basis functions of 2 variables, then adding random null-space perturbations. Each operation site gets unique coefficients. The implementation avoids `NOT` operations entirely — complements are computed via `x - (x & y)` instead of `~x`, eliminating the telltale `XOR reg, 0xFFFFFFFF` pattern.

Three rounds of substitution: round 1 replaces original operations, rounds 2-3 replace operations introduced by previous rounds, creating deeply nested expressions.

### Opaque Predicates

55+ predicate types across 28 mathematical categories:
- **Number theory**: Fermat's little theorem, quadratic residues, consecutive integer products
- **Post-quantum**: Lattice SIS/LWE, multivariate quadratic systems, error-correcting codes
- **Algebraic geometry**: Elliptic curve point verification, Weil pairing
- **Combinatorics**: Fibonacci identities, Catalan numbers, Lucas sequences
- **Analysis**: Continued fraction convergents, Parseval/Hadamard theorem
- **Game theory**: Nash equilibrium, Sprague-Grundy (Nim)
- **Automata theory**: DFA acceptance, pumping lemma
- **Polynomial algebra**: Bezout's identity, irreducibility witnesses, LFSR periods
- And more: Hensel lifting, Cayley-Hamilton, Euler characteristic, Shannon entropy bounds...

Each predicate spawns a chain of 2-5 bogus blocks with junk computation, anti-disassembly patterns, and their own nested opaque predicates.

### Overlapping Instructions

48+ anti-disassembly patterns:
- **JMP-over-rogue**: `EB 01 E8` (JMP +1 over CALL opcode), confuses linear sweep
- **Complementary jumps**: `74 03 75 01 E8` (JZ/JNZ pair, both skip rogue byte)
- **Exotic NOPs**: Undocumented `0F 19`-`0F 1E` hint NOPs, `0F 1F /1-/7` reserved reg fields
- **CET NOPs**: `F3 0F 1E FA` (ENDBR64), NOP on non-CET CPUs
- **Prefix padding**: Stacked `66 66 66 90`, segment override + NOP combinations
- **Unusual SIB**: `0F 1F 84 DB 78 56 34 12` (NOP with exotic addressing mode)

## Tests

98 GoogleTest cases covering:
- Exhaustive 8-bit verification of all MBA substitution rules
- Random 32-bit verification (100K+ samples)
- All 55+ opaque predicates verified for correctness
- All 48+ anti-disassembly patterns validated
- Integration tests combining all three techniques
- Constant obfuscation for specific values (0, 1, 42, 255, 0xDEADBEEF)
- Multi-round MBA chaining correctness
- Dynamic MBA uniqueness across seeds

```bash
cd build && ctest --output-on-failure
# 98/98 tests passed
```

## Crackme

The `crackme/crackme.c` is a simple password checker (password: `s3cr3t_k3y`) used to demonstrate the obfuscation. Build and test:

```bash
# Build plain version
clang -O1 -o crackme crackme/crackme.c

# Build obfuscated version
clang -S -emit-llvm -O2 -o crackme.ll crackme/crackme.c
opt -load-pass-plugin=./ShroudPasses.dll -passes="obfuscate-all" -S crackme.ll -o crackme_obf.ll
clang -O0 -o crackme_obf crackme_obf.ll

# Test both
./crackme "wrong"          # Access Denied.
./crackme "s3cr3t_k3y"     # Access Granted!
./crackme_obf "wrong"      # Access Denied.
./crackme_obf "s3cr3t_k3y" # Access Granted!
```

## License

This project is for educational and research purposes.
