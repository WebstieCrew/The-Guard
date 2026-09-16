import os
import subprocess
import sys

def main():
    src_dir = "src"
    build_dir = "build"
    output_bin = os.path.join(build_dir, "guard")

    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    c_files = []
    for root, _, files in os.walk(src_dir):
        for file in files:
            if file.endswith(".c"):
                c_files.append(os.path.join(root, file))

    if not c_files:
        sys.exit(1)

    compiler = "gcc"
    flags = ["-Wall", "-Wextra", "-O2"]
    cmd = [compiler] + flags + c_files + ["-o", output_bin]

    try:
        result = subprocess.run(cmd, check=True, capture_output=True, text=True)
        if result.stdout:
            print(result.stdout)
    except subprocess.CalledProcessError as e:
        if e.stderr:
            print(e.stderr, file=sys.stderr)
        sys.exit(e.returncode)

if __name__ == "__main__":
    main()
