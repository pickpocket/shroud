#!/usr/bin/env python3
"""
Experiment 4: Symbolic execution via angr.
Attempts to find the crackme password via symbolic argv[1].
Measures: time to solve, states explored, outcome.
"""

import angr
import claripy
import sys
import time
import json

def solve_crackme(binary_path, timeout=300):
    """Try to find the password that makes the crackme print 'Access Granted'."""
    result = {
        "binary": binary_path,
        "timeout_sec": timeout,
        "outcome": "unknown",
        "time_sec": 0,
        "states_explored": 0,
        "password_found": None,
        "error": None
    }

    try:
        start = time.time()

        proj = angr.Project(binary_path, auto_load_libs=False)

        # Symbolic argv[1]: up to 20 bytes
        password_len = 20
        password = claripy.BVS("password", password_len * 8)

        # Set up initial state with symbolic argv
        state = proj.factory.entry_state(
            args=[binary_path, password],
            add_options=angr.options.unicorn
        )

        # Constrain password to printable ASCII
        for i in range(password_len):
            byte = password.get_byte(i)
            state.solver.add(byte >= 0x20)
            state.solver.add(byte <= 0x7e)

        simgr = proj.factory.simulation_manager(state)

        # Look for "Access Granted" in stdout
        def is_granted(s):
            return b"Access Granted" in s.posix.dumps(1)

        def is_denied(s):
            return b"Access Denied" in s.posix.dumps(1)

        simgr.explore(
            find=is_granted,
            avoid=is_denied,
            timeout=timeout
        )

        elapsed = time.time() - start
        result["time_sec"] = round(elapsed, 2)
        result["states_explored"] = simgr.completion_count if hasattr(simgr, 'completion_count') else len(simgr.deadended) + len(simgr.found) + len(simgr.active)

        if simgr.found:
            found_state = simgr.found[0]
            solution = found_state.solver.eval(password, cast_to=bytes)
            # Strip null bytes
            solution = solution.split(b'\x00')[0]
            result["outcome"] = "solved"
            result["password_found"] = solution.decode('ascii', errors='replace')
        elif elapsed >= timeout:
            result["outcome"] = "timeout"
        else:
            result["outcome"] = "no_solution"

    except Exception as e:
        result["outcome"] = "error"
        result["error"] = str(e)
        result["time_sec"] = round(time.time() - start, 2)

    return result


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <binary> [timeout_sec]")
        sys.exit(1)

    binary = sys.argv[1]
    timeout = int(sys.argv[2]) if len(sys.argv) > 2 else 300

    print(f"[*] Solving {binary} with {timeout}s timeout...")
    result = solve_crackme(binary, timeout)

    print(f"[*] Outcome: {result['outcome']}")
    print(f"[*] Time: {result['time_sec']}s")
    print(f"[*] States: {result['states_explored']}")
    if result['password_found']:
        print(f"[*] Password: {result['password_found']}")
    if result['error']:
        print(f"[!] Error: {result['error']}")

    # Write JSON result
    outfile = binary + ".angr_result.json"
    with open(outfile, 'w') as f:
        json.dump(result, f, indent=2)
    print(f"[*] Results written to {outfile}")


if __name__ == "__main__":
    main()
