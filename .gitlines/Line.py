import os
import sys

def count_lines_and_files(root_dir="."):
    total_lines = 0
    total_files = 0
    extensions = (".c", ".h", ".js", ".html", ".css", ".py", ".xml")

    for root, dirs, files in os.walk(root_dir):
        if ".git" in dirs:
            dirs.remove(".git")
        if "build" in dirs:
            dirs.remove("build")

        for file in files:
            if file.endswith(extensions) or file == "project.c":
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
                        lines = len(f.readlines())
                        total_lines += lines
                        total_files += 1
                        print(f"{file_path}: {lines} lines")
                except Exception as e:
                    print(f"Error reading {file_path}: {e}")

    print(f"\nTotal Files: {total_files}")
    print(f"Total Lines: {total_lines}")

if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else "."
    count_lines_and_files(target)
