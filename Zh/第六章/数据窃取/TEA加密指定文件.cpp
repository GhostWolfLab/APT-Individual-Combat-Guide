#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

void TEA_encrypt(uint32_t* v, const uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = 0x9e3779b9;
    for (uint32_t i = 0; i < 32; i++) {
        sum += delta;
        v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
    }
    v[0] = v0;
    v[1] = v1;
}

void TEA_decrypt(uint32_t* v, const uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720, delta = 0x9e3779b9;
    for (uint32_t i = 0; i < 32; i++) {
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
        v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        sum -= delta;
    }
    v[0] = v0;
    v[1] = v1;
}

void process_file(const std::string& input_filename, const std::string& output_filename, const uint32_t* key, bool encrypt) {
    std::ifstream infile(input_filename, std::ios::binary);
    std::ofstream outfile(output_filename, std::ios::binary);

    if (!infile || !outfile) {
        std::cerr << "无法打开文件。" << std::endl;
        return;
    }

    std::vector<char> buffer(std::istreambuf_iterator<char>(infile), {});
    size_t original_size = buffer.size();

    // 填充到8字节的倍数
    while (buffer.size() % 8 != 0) {
        buffer.push_back('\0');
    }

    for (size_t i = 0; i < buffer.size(); i += 8) {
        uint32_t* v = reinterpret_cast<uint32_t*>(&buffer[i]);
        if (encrypt) {
            TEA_encrypt(v, key);
        }
        else {
            TEA_decrypt(v, key);
        }
    }

    // 写入加密/解密后的数据
    outfile.write(buffer.data(), buffer.size());

    std::cout << "文件已成功" << (encrypt ? "加密" : "解密") << "。" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "用法: " << argv[0] << " <加密/解密> <输入文件> <输出文件> <密钥>" << std::endl;
        return 1;
    }

    bool encrypt = std::strcmp(argv[1], "加密") == 0;
    const std::string input_filename = argv[2];
    const std::string output_filename = argv[3];

    uint32_t key[4];
    std::memcpy(key, argv[4], std::min(strlen(argv[4]), sizeof(key)));

    process_file(input_filename, output_filename, key, encrypt);

    return 0;
}
