#!/usr/bin/env python3

import sys
import subprocess
import platform
from pathlib import Path
import argparse

def run(cmd):
    print(f"[RUN] {cmd}")
    result = subprocess.run(cmd, shell=True)
    if result.returncode != 0:
        sys.exit(result.returncode)

def gen_executable():
    p = platform.system()
    if p == "Windows":
        return "testbed.exe"
    elif p in ("Darwin", "Linux"):
        return "testbed"
    else:
        print("Unsupported platform")
        sys.exit(1)

def dylib_name():
    p = platform.system()
    if p == "Windows":
        return "libengine.dll"
    elif p == "Darwin":
        return "libengine.dylib"
    elif p == "Linux":
        return "libengine.so"
    else:
        print("Unsupported platform")
        sys.exit(1)

def find_runtime_paths(executable):
    exe_candidates = [
        Path("bin") / executable,
        Path("build/testbed") / executable
    ]
    lib_candidates = [
        Path("bin") / dylib_name(),
        Path("build/engine") / dylib_name()
    ]

    exe_path = next((p for p in exe_candidates if p.is_file()), None)
    lib_path = next((p for p in lib_candidates if p.is_file()), None)
    return exe_path, lib_path

def main():
    parser = argparse.ArgumentParser(description="Supernova Run System")
    parser.parse_args()

    executable = gen_executable()
    exe_path, lib_path = find_runtime_paths(executable)

    if not exe_path:
        print(f"[ERROR] Executable not found: {executable}")
        print("[HINT] Build first with: python3 scripts/build.py")
        sys.exit(1)

    if not lib_path:
        print(f"[ERROR] Engine library not found: {dylib_name()}")
        print("[HINT] Build first with: python3 scripts/build.py")
        sys.exit(1)

    print(f"[INFO] Found executable: {exe_path}")
    print(f"[INFO] Found engine library: {lib_path}")
    run(str(exe_path))

if __name__ == "__main__":
    main()
