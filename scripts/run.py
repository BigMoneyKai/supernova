#!usr/bin/env python3

import os
import sys
import shutil
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
    elif p == "Darwin" or "Linux":
        return "testbed"
    else:
        print("Unsupported platform")
        sys.exit(1)

def dllibs_exist():
    p = platform.system()
    if p == "Windows":
        if Path("bin/libengine.dll").is_file():
            print("[INFO] Found bin/libengine.dll")
            return True
    elif p == "Darwin" or "Linux":
        if Path("bin/libengine.so").is_file():
            print("[INFO] Found bin/libengine.so")
            :wa
            return True
    else:
        print("Unsupported platform")
        sys.exit(1)

def main():
    parser = argparse.ArgumentParser(description="Supernova Run System");

    executable = gen_executable()
    run_cmd = f"bin/{executable}"
    if dllibs_exist():
        run(run_cmd)

if __name__ == "__main__":
    main()
