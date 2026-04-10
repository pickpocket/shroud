#!/usr/bin/env python3
"""
Experiment 6: Diversity analysis.
Compiles the same source N times with different seeds, measures binary differences.
"""

import subprocess
import sys
import os
import json
import hashlib
import struct

def hamming_distance(a, b):
    """Byte-level hamming distance between two byte sequences."""
    min_len = min(len(a), len(b))
    diff = sum(1 for i in range(min_len) if a[i] != b[i])
    diff += abs(len(a) - len(b))
    return diff

def count_unique_constants(binary_path):
    """Count unique 32-bit constants in the .text section."""
    try:
        import lief
        binary = lief.parse(binary_path)
        if binary is None:
            return 0

        text = None
        for section in binary.sections:
            if section.name == ".text":
                text = section
                break

        if text is None:
            return 0

        data = bytes(text.content)
        constants = set()
        for i in range(len(data) - 3):
            val = struct.unpack_from('<I', data, i)[0]
            if val > 0xFF and val != 0xFFFFFFFF and val != 0xCCCCCCCC:
                constants.add(val)

        return len(constants)
    except Exception:
        return 0

def has_ffffffff_pattern(binary_path):
    """Check for XOR reg, 0xFFFFFFFF patterns (NOT-free MBA should avoid these)."""
    try:
        with open(binary_path, 'rb') as f:
            data = f.read()
        # XOR reg, imm32 where imm32 = 0xFFFFFFFF
        # 81 F0 FF FF FF FF = XOR EAX, 0xFFFFFFFF (6 bytes)
        # 83 F0 FF = XOR EAX, -1 (3 bytes, sign-extended)
        count = 0
        for i in range(len(data) - 5):
            if data[i] == 0x81 and (data[i+1] & 0xF8) == 0xF0:
                if data[i+2:i+6] == b'\xFF\xFF\xFF\xFF':
                    count += 1
        return count
    except Exception:
        return -1


def main():
    if len(sys.argv) < 4:
        print(f"Usage: {sys.argv[0]} <source.c> <num_compilations> <obf_command>")
        print(f"  Example: {sys.argv[0]} crackme.c 10 'opt -load-pass-plugin=ShroudPasses.dll -passes=mba-obf'")
        sys.exit(1)

    source = sys.argv[1]
    n = int(sys.argv[2])
    obf_cmd = sys.argv[3] if len(sys.argv) > 3 else ""

    print(f"[*] Compiling {source} {n} times...")

    binaries = []
    hashes = []

    for i in range(n):
        outfile = f"/tmp/diversity_build_{i}.exe"
        ll_file = f"/tmp/diversity_{i}.ll"
        obf_file = f"/tmp/diversity_obf_{i}.ll"

        # Compile to IR
        subprocess.run(
            ["clang", "-S", "-emit-llvm", "-O2", "-o", ll_file, source],
            capture_output=True
        )

        if obf_cmd:
            # Apply obfuscation (each run produces different output due to RNG)
            subprocess.run(
                obf_cmd.split() + ["-S", ll_file, "-o", obf_file],
                capture_output=True
            )
            # Compile obfuscated IR
            subprocess.run(
                ["clang", "-O0", "-o", outfile, obf_file],
                capture_output=True
            )
        else:
            subprocess.run(
                ["clang", "-O0", "-o", outfile, ll_file],
                capture_output=True
            )

        if os.path.exists(outfile):
            with open(outfile, 'rb') as f:
                data = f.read()
            binaries.append(data)
            hashes.append(hashlib.sha256(data).hexdigest())
        else:
            binaries.append(b'')
            hashes.append('')

    # Compute pairwise hamming distances
    distances = []
    for i in range(n):
        for j in range(i + 1, n):
            if binaries[i] and binaries[j]:
                dist = hamming_distance(binaries[i], binaries[j])
                distances.append(dist)

    # Count unique hashes
    unique_hashes = len(set(h for h in hashes if h))

    # Count unique constants in each binary
    const_counts = []
    ffffffff_counts = []
    for i in range(n):
        outfile = f"/tmp/diversity_build_{i}.exe"
        if os.path.exists(outfile):
            const_counts.append(count_unique_constants(outfile))
            ffffffff_counts.append(has_ffffffff_pattern(outfile))

    result = {
        "source": source,
        "num_compilations": n,
        "unique_hashes": unique_hashes,
        "all_different": unique_hashes == n,
        "avg_hamming_distance": sum(distances) / len(distances) if distances else 0,
        "min_hamming_distance": min(distances) if distances else 0,
        "max_hamming_distance": max(distances) if distances else 0,
        "avg_unique_constants": sum(const_counts) / len(const_counts) if const_counts else 0,
        "avg_ffffffff_patterns": sum(ffffffff_counts) / len(ffffffff_counts) if ffffffff_counts else 0,
        "binary_sizes": [len(b) for b in binaries],
    }

    print(f"[*] Unique binaries: {unique_hashes}/{n}")
    print(f"[*] Avg hamming distance: {result['avg_hamming_distance']:.0f} bytes")
    print(f"[*] Avg unique constants: {result['avg_unique_constants']:.0f}")
    print(f"[*] Avg 0xFFFFFFFF patterns: {result['avg_ffffffff_patterns']:.1f}")

    outfile = source + ".diversity_result.json"
    with open(outfile, 'w') as f:
        json.dump(result, f, indent=2)
    print(f"[*] Results written to {outfile}")


if __name__ == "__main__":
    main()
