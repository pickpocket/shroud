#!/usr/bin/env python3
"""
Experiment 5: Binary size and runtime overhead.
Compiles benchmarks plain and obfuscated, measures size/time/compilation differences.
"""

import subprocess
import sys
import os
import time
import json

def compile_plain(source, output):
    """Compile without obfuscation."""
    start = time.time()
    result = subprocess.run(
        ["clang", "-O1", "-o", output, source],
        capture_output=True, text=True
    )
    elapsed = time.time() - start
    return {"success": result.returncode == 0, "time": round(elapsed, 2), "stderr": result.stderr[:200]}

def compile_obfuscated(source, output, shroud_dll):
    """Compile with full Shroud obfuscation pipeline."""
    ll_file = output + ".ll"
    step1 = output + ".step1.ll"
    step2 = output + ".step2.ll"

    start = time.time()

    # Step 1: compile to IR
    r1 = subprocess.run(
        ["clang", "-S", "-emit-llvm", "-O2", "-o", ll_file, source],
        capture_output=True, text=True
    )
    if r1.returncode != 0:
        return {"success": False, "error": "clang -emit-llvm failed"}

    # Step 2: apply obfuscation passes
    r2 = subprocess.run(
        ["opt", "-load-pass-plugin=" + shroud_dll,
         "-passes=opaque-pred,mba-obf,overlap-inst",
         "-S", ll_file, "-o", step1],
        capture_output=True, text=True
    )
    if r2.returncode != 0:
        return {"success": False, "error": "opt failed"}

    # Step 3: string hiding
    r3 = subprocess.run(
        ["opt", "-load-pass-plugin=" + shroud_dll,
         "-passes=string-hide",
         "-S", step1, "-o", step2],
        capture_output=True, text=True
    )
    if r3.returncode != 0:
        return {"success": False, "error": "string-hide failed"}

    # Step 4: compile to binary
    r4 = subprocess.run(
        ["clang", "-O0", "-o", output, step2],
        capture_output=True, text=True
    )

    elapsed = time.time() - start
    return {"success": r4.returncode == 0, "time": round(elapsed, 2)}

def measure_runtime(binary, args, runs=10):
    """Measure average runtime over N runs."""
    times = []
    for _ in range(runs):
        start = time.time()
        result = subprocess.run(
            [binary] + args,
            capture_output=True, timeout=60
        )
        elapsed = time.time() - start
        if result.returncode == 0:
            times.append(elapsed)

    if not times:
        return {"avg": 0, "min": 0, "max": 0, "runs": 0}

    return {
        "avg": round(sum(times) / len(times), 4),
        "min": round(min(times), 4),
        "max": round(max(times), 4),
        "runs": len(times)
    }

def file_size(path):
    try:
        return os.path.getsize(path)
    except:
        return 0

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <source.c> <test_args...> [--shroud-dll=path]")
        sys.exit(1)

    source = sys.argv[1]
    shroud_dll = ""
    test_args = []

    for arg in sys.argv[2:]:
        if arg.startswith("--shroud-dll="):
            shroud_dll = arg.split("=", 1)[1]
        else:
            test_args.append(arg)

    basename = os.path.splitext(os.path.basename(source))[0]
    plain_bin = f"/tmp/{basename}_plain"
    obf_bin = f"/tmp/{basename}_obf"

    results = {"source": source, "test_args": test_args}

    # Compile plain
    print(f"[*] Compiling {source} (plain)...")
    plain_compile = compile_plain(source, plain_bin)
    results["plain_compile"] = plain_compile
    results["plain_size"] = file_size(plain_bin)
    print(f"    Size: {results['plain_size']} bytes, Time: {plain_compile['time']}s")

    # Compile obfuscated
    if shroud_dll:
        print(f"[*] Compiling {source} (obfuscated)...")
        obf_compile = compile_obfuscated(source, obf_bin, shroud_dll)
        results["obf_compile"] = obf_compile
        results["obf_size"] = file_size(obf_bin)
        results["size_ratio"] = round(results["obf_size"] / max(results["plain_size"], 1), 1)
        print(f"    Size: {results['obf_size']} bytes ({results['size_ratio']}x), Time: {obf_compile['time']}s")

    # Measure runtime
    if os.path.exists(plain_bin):
        print(f"[*] Measuring runtime (plain)...")
        results["plain_runtime"] = measure_runtime(plain_bin, test_args)
        print(f"    Avg: {results['plain_runtime']['avg']}s")

    if shroud_dll and os.path.exists(obf_bin):
        print(f"[*] Measuring runtime (obfuscated)...")
        results["obf_runtime"] = measure_runtime(obf_bin, test_args)
        print(f"    Avg: {results['obf_runtime']['avg']}s")

        if results["plain_runtime"]["avg"] > 0:
            results["runtime_ratio"] = round(
                results["obf_runtime"]["avg"] / results["plain_runtime"]["avg"], 1)
            print(f"    Slowdown: {results['runtime_ratio']}x")

    outfile = source + ".overhead_result.json"
    with open(outfile, 'w') as f:
        json.dump(results, f, indent=2)
    print(f"[*] Results written to {outfile}")


if __name__ == "__main__":
    main()
