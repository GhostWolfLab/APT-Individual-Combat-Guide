import sys

def replace_content_in_file(file_path, old_content, new_content):
    try:
        # 读取文件内容
        with open(file_path, 'r', encoding='utf-8') as file:
            file_data = file.read()

        # 替换指定内容
        new_data = file_data.replace(old_content, new_content)

        # 将新的内容写回文件
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_data)

        print(f"Successfully replaced '{old_content}' with '{new_content}' in {file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python script.py <file_path> <old_content> <new_content>")
    else:
        file_path = sys.argv[1]
        old_content = sys.argv[2]
        new_content = sys.argv[3]
        replace_content_in_file(file_path, old_content, new_content)
