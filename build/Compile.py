import argparse
import multiprocessing
import os
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

SRC_DIR = Path("src")
BUILD_DIR = Path("build")
BIN_DIR = Path("bin")
TARGET_NAME = "guard.exe" if os.name == "nt" else "guard"
TARGET_PATH = BIN_DIR / TARGET_NAME

DEFAULT_CC = "clang" if shutil.which("clang") else "gcc"
CFLAGS = [
    "-std=c11",
    "-O3",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wshadow",
    "-Conversion",
    "-Isrc",
]

def compile_object(c_file: Path, obj_file: Path, cc: str) -> bool:
    if obj_file.exists() and obj_file.stat().st_mtime > c_file.stat().st_mtime:
        return True

    cmd = [cc] + CFLAGS + ["-c", str(c_file), "-o", str(obj_file)]
    res = subprocess.run(cmd, capture_output=True, text=True)
    
    if res.returncode != 0:
        print(f"[ERROR] {c_file}\n{res.stderr}", file=sys.stderr)
        return False
    return True

def link_objects(obj_files: list[Path], cc: str) -> bool:
    cmd = [cc] + [str(obj) for obj in obj_files] + ["-o", str(TARGET_PATH)]
    res = subprocess.run(cmd, capture_output=True, text=True)
    
    if res.returncode != 0:
        print(f"[LINK ERROR]\n{res.stderr}", file=sys.stderr)
        return False
    return True

def main():
    parser = argparse.ArgumentParser(description="The Guard Engine Build System")
    parser.add_argument("--cc", default=DEFAULT_CC, help="C compiler driver")
    parser.add_argument("--clean", action="store_true", help="Clean build artifacts")
    args = parser.parse_args()

    if args.clean:
        shutil.rmtree(BUILD_DIR, ignore_errors=True)
        shutil.rmtree(BIN_DIR, ignore_errors=True)
        print("[CLEAN] Artifacts purged.")
        return

    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    BIN_DIR.mkdir(parents=True, exist_ok=True)

    c_files = list(SRC_DIR.rglob("*.c"))
    if not c_files:
        print("[ERROR] No C source files found in src/", file=sys.stderr)
        sys.exit(1)

    obj_files = []
    tasks = []

    with ThreadPoolExecutor(max_workers=multiprocessing.cpu_count()) as executor:
        for c_file in c_files:
            rel_path = c_file.relative_to(SRC_DIR)
            obj_file = BUILD_DIR / rel_path.with_suffix(".o")
            obj_file.parent.mkdir(parents=True, exist_ok=True)
            obj_files.append(obj_file)
            tasks.append(executor.submit(compile_object, c_file, obj_file, args.cc))

        if not all(future.result() for future in as_completed(tasks)):
            sys.exit(1)

    if link_objects(obj_files, args.cc):
        print(f"[SUCCESS] Binary built at: {TARGET_PATH}")
    else:
        sys.exit(1)

if __name__ == "__main__":
    main()
