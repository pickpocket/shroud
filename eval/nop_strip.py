#!/usr/bin/env python3
"""
Experiment 3: NOP removal correctness.
Strips all multi-byte NOP patterns from a binary and checks if it still runs.
Plain binaries should survive. Obfuscated binaries with load-bearing NOPs should crash.
"""

import lief
import sys
import os
import subprocess
import json
import struct

# Multi-byte NOP patterns to detect and strip
NOP_PATTERNS = [
    # Documented NOPs (Intel SDM Table 4-12)
    bytes([0x0F, 0x1F, 0x00]),                          # 3-byte NOP
    bytes([0x0F, 0x1F, 0x40, 0x00]),                    # 4-byte NOP
    bytes([0x0F, 0x1F, 0x44, 0x00, 0x00]),              # 5-byte NOP
    bytes([0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00]),        # 6-byte NOP
    bytes([0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00]),  # 7-byte NOP
    bytes([0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00]),  # 8-byte NOP
    bytes([0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00]),  # 9-byte NOP
]

def is_exotic_nop_opcode(b1, b2):
    """Check if two bytes form an undocumented hint NOP opcode."""
    return b1 == 0x0F and 0x19 <= b2 <= 0x1F

def is_prefix_66(b):
    return b == 0x66

def find_and_strip_nops(data, text_offset, text_size):
    """Find all multi-byte NOP sequences in the .text section and replace with 0x90 (single-byte NOP)."""
    patched = bytearray(data)
    nops_found = 0
    nops_patched = 0

    i = text_offset
    end = text_offset + text_size

    while i < end - 2:
        # Check for 0F 1F (documented NOP) or 0F 19-1E (undocumented hint NOP)
        if i < end - 1 and patched[i] == 0x0F and 0x19 <= patched[i+1] <= 0x1F:
            # Parse the full instruction length
            nop_start = i
            i += 2  # skip opcode

            # Check for ModRM
            if i < end:
                modrm = patched[i]
                mod = (modrm >> 6) & 3
                rm = modrm & 7
                i += 1

                # SIB byte?
                if rm == 4 and mod != 3:
                    i += 1

                # Displacement
                if mod == 1:
                    i += 1
                elif mod == 2:
                    i += 4
                elif mod == 0 and rm == 5:
                    i += 4

            nop_len = i - nop_start
            nops_found += 1

            # Patch to single-byte NOPs
            for j in range(nop_start, i):
                patched[j] = 0x90
            nops_patched += 1
            continue

        # Check for prefix-stacked NOPs (66 66 ... 90)
        if patched[i] == 0x66:
            j = i
            while j < end and patched[j] == 0x66:
                j += 1
            if j < end and patched[j] == 0x90 and j - i >= 2:
                nop_len = j - i + 1
                nops_found += 1
                for k in range(i, j + 1):
                    patched[k] = 0x90
                nops_patched += 1
                i = j + 1
                continue

        # Check for prefix-stacked multi-byte NOPs (66 66 ... 0F 1F ...)
        if patched[i] == 0x66:
            j = i
            while j < end and patched[j] == 0x66:
                j += 1
            if j < end - 1 and patched[j] == 0x0F and 0x19 <= patched[j+1] <= 0x1F:
                # This is a prefix-stacked exotic NOP; will be caught on next iteration
                pass

        # Check for ENDBR64/ENDBR32 (F3 0F 1E FA/FB)
        if i < end - 3 and patched[i] == 0xF3 and patched[i+1] == 0x0F and patched[i+2] == 0x1E:
            if patched[i+3] in (0xFA, 0xFB):
                nops_found += 1
                for j in range(i, i + 4):
                    patched[j] = 0x90
                nops_patched += 1
                i += 4
                continue

        i += 1

    return bytes(patched), nops_found, nops_patched


def strip_nops(input_path, output_path):
    """Strip all NOP-like instructions from a PE/ELF binary."""
    binary = lief.parse(input_path)
    if binary is None:
        return {"error": f"Failed to parse {input_path}"}

    # Find .text section
    text_section = None
    for section in binary.sections:
        if section.name == ".text" or (hasattr(section, 'characteristics') and section.characteristics & 0x20000000):
            text_section = section
            break

    if text_section is None:
        return {"error": "No .text section found"}

    with open(input_path, 'rb') as f:
        data = f.read()

    text_offset = text_section.offset
    text_size = text_section.size

    patched_data, nops_found, nops_patched = find_and_strip_nops(data, text_offset, text_size)

    with open(output_path, 'wb') as f:
        f.write(patched_data)

    # Make executable on Unix
    if os.name != 'nt':
        os.chmod(output_path, 0o755)

    return {
        "nops_found": nops_found,
        "nops_patched": nops_patched,
        "text_offset": text_offset,
        "text_size": text_size
    }


def test_binary(binary_path, test_args, expected_exit=0):
    """Run a binary and check if it works."""
    try:
        result = subprocess.run(
            [binary_path] + test_args,
            capture_output=True, timeout=10, text=True
        )
        return {
            "ran": True,
            "exit_code": result.returncode,
            "stdout": result.stdout[:200],
            "stderr": result.stderr[:200],
            "correct": result.returncode == expected_exit
        }
    except subprocess.TimeoutExpired:
        return {"ran": False, "error": "timeout"}
    except Exception as e:
        return {"ran": False, "error": str(e)}


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <binary> <test_arg1> [test_arg2 ...]")
        print(f"  Strips NOPs from binary, tests if it still works.")
        sys.exit(1)

    input_path = sys.argv[1]
    test_args = sys.argv[2:]
    output_path = input_path + ".nop_stripped"

    print(f"[*] Stripping NOPs from {input_path}...")
    strip_result = strip_nops(input_path, output_path)

    if "error" in strip_result:
        print(f"[!] {strip_result['error']}")
        sys.exit(1)

    print(f"[*] Found {strip_result['nops_found']} NOP sequences, patched {strip_result['nops_patched']}")

    # Test original
    print(f"[*] Testing original binary...")
    orig_result = test_binary(input_path, test_args)
    print(f"    Exit: {orig_result.get('exit_code', 'N/A')}, Correct: {orig_result.get('correct', False)}")

    # Test stripped
    print(f"[*] Testing NOP-stripped binary...")
    stripped_result = test_binary(output_path, test_args)
    print(f"    Exit: {stripped_result.get('exit_code', 'N/A')}, Correct: {stripped_result.get('correct', False)}")

    # Summary
    result = {
        "binary": input_path,
        "strip_stats": strip_result,
        "original": orig_result,
        "stripped": stripped_result,
        "load_bearing": orig_result.get("correct", False) and not stripped_result.get("correct", False)
    }

    if result["load_bearing"]:
        print(f"[+] LOAD-BEARING: Original works, stripped crashes!")
    elif not orig_result.get("correct", False):
        print(f"[?] Original binary doesn't work with these args")
    else:
        print(f"[-] NOT load-bearing: stripped binary still works")

    outfile = input_path + ".nop_strip_result.json"
    with open(outfile, 'w') as f:
        json.dump(result, f, indent=2)
    print(f"[*] Results written to {outfile}")


if __name__ == "__main__":
    main()
