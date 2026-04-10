#!/usr/bin/env python3
"""
Experiment 1 & 2: IDA Pro analysis.
This script is designed to be run via IDA's scripting interface (IDAPython).
It exports: function count, basic block count, decompilation success/failure per function.

Run inside IDA:
  File -> Script File -> ida_analyze.py
Or headless:
  idat64 -A -S"ida_analyze.py" binary.exe
"""

import json
import os

try:
    import idautils
    import idaapi
    import idc
    IN_IDA = True
except ImportError:
    IN_IDA = False
    print("[!] Not running inside IDA. This script requires IDAPython.")
    print("    Run via: idat64 -A -S\"ida_analyze.py\" <binary>")

def analyze():
    if not IN_IDA:
        return

    idaapi.auto_wait()

    binary_path = idc.get_input_file_path()
    result = {
        "binary": binary_path,
        "functions": [],
        "total_functions": 0,
        "total_basic_blocks": 0,
        "decompile_success": 0,
        "decompile_fail": 0,
    }

    for func_ea in idautils.Functions():
        func = idaapi.get_func(func_ea)
        if func is None:
            continue

        func_info = {
            "name": idc.get_func_name(func_ea),
            "address": hex(func_ea),
            "size": func.size(),
            "basic_blocks": 0,
            "instructions": 0,
            "decompiled": False,
            "decompile_error": None,
        }

        # Count basic blocks
        flowchart = idaapi.FlowChart(func)
        bb_count = 0
        inst_count = 0
        for block in flowchart:
            bb_count += 1
            ea = block.start_ea
            while ea < block.end_ea:
                inst_count += 1
                ea = idc.next_head(ea)
        func_info["basic_blocks"] = bb_count
        func_info["instructions"] = inst_count

        # Try decompilation
        try:
            cfunc = idaapi.decompile(func_ea)
            if cfunc:
                func_info["decompiled"] = True
                func_info["decompile_lines"] = len(str(cfunc).split('\n'))
                result["decompile_success"] += 1
            else:
                func_info["decompile_error"] = "returned None"
                result["decompile_fail"] += 1
        except Exception as e:
            func_info["decompile_error"] = str(e)
            result["decompile_fail"] += 1

        result["functions"].append(func_info)
        result["total_basic_blocks"] += bb_count

    result["total_functions"] = len(result["functions"])

    # Write results
    outfile = binary_path + ".ida_result.json"
    with open(outfile, 'w') as f:
        json.dump(result, f, indent=2)

    print(f"[*] Functions: {result['total_functions']}")
    print(f"[*] Basic blocks: {result['total_basic_blocks']}")
    print(f"[*] Decompile success: {result['decompile_success']}")
    print(f"[*] Decompile fail: {result['decompile_fail']}")
    print(f"[*] Results written to {outfile}")

    # Exit IDA if running headless
    if idaapi.cvar.batch:
        idc.qexit(0)

if IN_IDA:
    analyze()
