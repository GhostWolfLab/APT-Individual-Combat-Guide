import sys

//XOR加密
def xor(data, key):
    key = str(key)
    l = len(key)
    output_str = ""

    for i in range(len(data)):
        current = data[i]
        current_key = key[i % len(key)]
        ordd = lambda x: x if isinstance(x, int) else ord(x)
        output_str += chr(ordd(current) ^ ord(current_key))
    return output_str

def xor_encrypt(data, key):
    ciphertext = xor(data, key)
    ciphertext = '{ 0x' + ', 0x'.join(hex(ord(x))[2:] for x in ciphertext) + ' };'
    print(ciphertext)
    return ciphertext, key

//加密/解密的秘钥
my_secret_key = "xor_encode_secret"

//指定可执行程序
plaintext = open("./evil.bin", "rb").read()

ciphertext, p_key = xor_encrypt(plaintext, my_secret_key)

//输出加密后的shellcode
print("Encrypted shellcode:")
print(ciphertext)
