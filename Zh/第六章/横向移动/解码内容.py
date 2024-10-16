def hex_to_readable(hex_string):
    # 过滤掉非十六进制字符
    hex_string = ''.join(filter(lambda x: x in '0123456789abcdefABCDEF', hex_string))

    # 将十六进制字符串转换为字节
    bytes_data = bytes.fromhex(hex_string)

    # 尝试逐字节解码
    readable_string = ''
    for byte in bytes_data:
        try:
            readable_string += chr(byte)
        except UnicodeDecodeError:
            readable_string += '�'  # 使用替代字符

    return readable_string

def main():
    # 从文件读取十六进制字符串
    with open('memory_dump.txt', 'r') as input_file:
        hex_string = input_file.read().strip()

    # 转换为可读字符串
    readable_string = hex_to_readable(hex_string)

    # 将可读字符串写入输出文件
    with open('readable_output.txt', 'w', encoding='utf-8') as output_file:
        output_file.write(readable_string)

    print("已将可读字符串写入 'readable_output.txt' 文件。")

if __name__ == '__main__':
    main()
