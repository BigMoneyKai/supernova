#!usr/bin/env python3

import os
import sys
import shutil
import subprocess
import platform
import argparse

BUILD_DIR = "build"
SRC_DIR = "."
BIN_DIR = "bin"
TOOLCHAIN_DIR = "cmake/toolchains"

def run(cmd):
    print(f"[RUN] {cmd}")
    result = subprocess.run(cmd, shell=True)
    if result.returncode != 0:
        sys.exit(result.returncode)

def exists(cmd):
    return shutil.which(cmd) is not None

def detect_platform():
    p = platform.system()
    if p == "Windows":
        return "windows"
    elif p == "Linux":
        return "linux"
    elif p == "Darwin":
        return "macos"
    else:
        print("Unsupported platform")
        sys.exit(1)

def detect_compiler(platform):
    if platform == "windows":
        if exists("cl"):
            return "msvc"
        elif exists("gcc"):
            return "mingw"
        else:
            print("No suitable compiler found")
    else:
        if exists("clang"):
            return "clang"
        elif exists("gcc"): 
            return "gcc"
        else:
            print("No suitable compiler found")

    sys.exit(1)

def config(args, platform, compiler):
    cmd = f"cmake -S {SRC_DIR} -B {BUILD_DIR}"
    cmd += f" -DCMAKE_BUILD_TYPE={args.build_type}"

    if platform == "windows":
        if compiler == "msvc":
            cmd += ' -G "Visual studio 17 2022"'
        elif compiler == "gcc":
            cmd += f" -DCMAKE_BUILD_TYPE={TOOLCHAIN_DIR}/windows-mingw-w64.cmake"
            
    elif platform == "linux":
        if compiler == "clang":
            cmd += f" -DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN_DIR}/linux-clang.cmake"
        elif compiler == "gcc":
            cmd += f" -DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN_DIR}/linux-gcc.cmake"

    elif platform == "macos":
        cmd += f" -DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN_DIR}/apple-clang.cmake"

    if args.cmake_args:
        cmd += " " + " ".join(args.cmake_args)

    cmd += " -G Ninja"

    return cmd

def main():
    parser = argparse.ArgumentParser(description="Supernova Build System")

    parser.add_argument('-p', "--platform", choices=["auto", "windows", "linux", "macos"], default="auto")
    parser.add_argument('-c', "--compiler", choices=["auto", "msvc", "mingw", "gcc", "clang"], default="auto")
    parser.add_argument('-bt', "--build-type", choices=["Debug", "Release"], default="Debug")
    parser.add_argument('-cl', "--clean", action="store_true")
    parser.add_argument('-ca', "--cmake-args", nargs="*", help="extra cmake args")
    parser.add_argument('-V', "--verbose", action="store_true")
    parser.add_argument('-cmt', "--close-multi-threads", action="store_true")

    args = parser.parse_args()

    platform = detect_platform() if args.platform == "auto" else args.platform
    compiler = detect_compiler(platform) if args.compiler == "auto" else args.compiler

    print(f"[INFO] Platform : {platform}")
    print(f"[INFO] Compiler : {compiler}")

    # clean
    if args.clean and os.path.exists(BUILD_DIR):
        print("[INFO] Cleaning build directory")
        shutil.rmtree(BUILD_DIR)

    # configure
    config_cmd = config(args, platform, compiler)
    run(config_cmd)

    # build
    build_cmd = f"cmake --build {BUILD_DIR}"
    if args.verbose:
        build_cmd += " " + "--verbose"

    run(build_cmd)

if __name__ == '__main__':
    main()
