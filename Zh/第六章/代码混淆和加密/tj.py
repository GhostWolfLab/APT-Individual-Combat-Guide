import argparse

def add_irrelevant_conditions(code):
    # 示例：添加无关条件语句
    lines = code.split('\n')
    for i in range(len(lines)):
        if 'if' in lines[i]:
            indent = ' ' * (len(lines[i]) - len(lines[i].lstrip()))
            lines.insert(i + 1, f'{indent}    if (1 == 1): pass  # 无关条件')
            break
    return '\n'.join(lines)

def swap_condition_order(code):
    # 示例：交换条件判断顺序
    lines = code.split('\n')
    for i in range(len(lines)):
        if 'if' in lines[i] and 'else' in lines[i + 1]:
            lines[i], lines[i + 1] = lines[i + 1], lines[i]
            break
    return '\n'.join(lines)

def insert_redundant_conditions(code):
    # 示例：插入冗余条件语句
    lines = code.split('\n')
    for i in range(len(lines)):
        if 'if' in lines[i]:
            indent = ' ' * (len(lines[i]) - len(lines[i].lstrip()))
            # 使用已定义的变量 guess
            lines.insert(i + 1, f'{indent}    if (guess > 0 and guess < 100): pass  # 冗余条件')
            break
    return '\n'.join(lines)

def main():
    parser = argparse.ArgumentParser(description='代码混淆脚本')
    parser.add_argument('file', type=str, help='要混淆的文件')
    parser.add_argument('--irrelevant', action='store_true', help='添加无关条件语句')
    parser.add_argument('--swap', action='store_true', help='交换条件判断顺序')
    parser.add_argument('--redundant', action='store_true', help='插入冗余条件语句')
    parser.add_argument('--all', action='store_true', help='执行所有混淆操作')

    args = parser.parse_args()

    with open(args.file, 'r', encoding='utf-8') as f:
        code = f.read()

    if args.irrelevant or args.all:
        code = add_irrelevant_conditions(code)
    if args.swap or args.all:
        code = swap_condition_order(code)
    if args.redundant or args.all:
        code = insert_redundant_conditions(code)

    with open(args.file, 'w', encoding='utf-8') as f:
        f.write(code)

if __name__ == '__main__':
    main()
