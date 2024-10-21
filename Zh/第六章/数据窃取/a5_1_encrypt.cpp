#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <sstream>

class A5_1 {
public:
    A5_1(uint64_t key, uint32_t frame) {
        // 初始化寄存器
        R1 = key & 0x7FFFF; // 19位
        R2 = (key >> 19) & 0x3FFFFF; // 22位
        R3 = (key >> 41) & 0x7FFFFF; // 23位

        // 加载帧号
        for (int i = 0; i < 22; ++i) {
            clock_all();
            R1 ^= (frame >> i) & 1;
            R2 ^= (frame >> i) & 1;
            R3 ^= (frame >> i) & 1;
        }

        // 100次时钟控制
        for (int i = 0; i < 100; ++i) {
            clock_majority();
        }
    }

    uint8_t get_keystream_byte() {
        uint8_t keystream = 0;
        for (int i = 0; i < 8; ++i) {
            keystream |= (clock_majority() << i);
        }
        return keystream;
    }

private:
    uint32_t R1, R2, R3;

    void clock_all() {
        R1 = ((R1 << 1) | parity(R1 & 0x072000)) & 0x7FFFF;
        R2 = ((R2 << 1) | parity(R2 & 0x300000)) & 0x3FFFFF;
        R3 = ((R3 << 1) | parity(R3 & 0x700080)) & 0x7FFFFF;
    }

    uint8_t clock_majority() {
        uint8_t majority = (parity(R1 & 0x10000) + parity(R2 & 0x20000) + parity(R3 & 0x40000)) >= 2;
        if (parity(R1 & 0x10000) == majority) clock_all();
        if (parity(R2 & 0x20000) == majority) clock_all();
        if (parity(R3 & 0x40000) == majority) clock_all();
        return parity(R1 ^ R2 ^ R3);
    }

    uint8_t parity(uint32_t x) {
        x ^= x >> 16;
        x ^= x >> 8;
        x ^= x >> 4;
        x ^= x >> 2;
        x ^= x >> 1;
        return x & 1;
    }
};

uint64_t hex_to_uint64(const std::string& hex_str) {
    std::stringstream ss;
    ss << std::hex << hex_str;
    uint64_t result;
    ss >> result;
    return result;
}

uint32_t hex_to_uint32(const std::string& hex_str) {
    std::stringstream ss;
    ss << std::hex << hex_str;
    uint32_t result;
    ss >> result;
    return result;
}

void process_file(const std::string& input_filename, const std::string& output_filename, uint64_t key, uint32_t frame, bool encrypt) {
    std::ifstream infile(input_filename, std::ios::binary);
    std::ofstream outfile(output_filename, std::ios::binary);

    if (!infile || !outfile) {
        std::cerr << "无法打开文件。" << std::endl;
        return;
    }

    std::vector<char> buffer(std::istreambuf_iterator<char>(infile), {});

    A5_1 a5_1(key, frame);

    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] ^= a5_1.get_keystream_byte();
    }

    outfile.write(buffer.data(), buffer.size());

    std::cout << "文件已成功" << (encrypt ? "加密" : "解密") << "。" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "用法: " << argv[0] << " <加密/解密> <输入文件> <输出文件> <密钥> <帧号>" << std::endl;
        return 1;
    }

    bool encrypt = std::strcmp(argv[1], "加密") == 0;
    const std::string input_filename = argv[2];
    const std::string output_filename = argv[3];
    uint64_t key = hex_to_uint64(argv[4]);
    uint32_t frame = hex_to_uint32(argv[5]);

    process_file(input_filename, output_filename, key, frame, encrypt);

    return 0;
}
