#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

void XOR_encrypt_decrypt(std::vector<char>& data, const std::string& key) {
    size_t key_length = key.size();
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= key[i % key_length];
    }
}

void process_file(const std::string& input_filename, const std::string& output_filename, const std::string& key, bool encrypt) {
    std::ifstream infile(input_filename, std::ios::binary);
    std::ofstream outfile(output_filename, std::ios::binary);

    if (!infile || !outfile) {
        std::cerr << "无法打开文件：" << input_filename << std::endl;
        return;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

    XOR_encrypt_decrypt(buffer, key);

    outfile.write(buffer.data(), buffer.size());

    std::cout << "文件已成功" << (encrypt ? "加密" : "解密") << "：" << input_filename << " -> " << output_filename << std::endl;
}

void encrypt_directory(const std::string& directory, const std::string& key) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string input_filename = entry.path().string();
            std::string output_filename = input_filename + ".enc";
            process_file(input_filename, output_filename, key, true);
        }
    }
}

void decrypt_directory(const std::string& directory, const std::string& key) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string input_filename = entry.path().string();
            if (input_filename.size() > 4 && input_filename.substr(input_filename.size() - 4) == ".enc") {
                std::string output_filename = input_filename.substr(0, input_filename.size() - 4);
                process_file(input_filename, output_filename, key, false);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "用法: " << argv[0] << " <加密/解密> <目录> <密钥>" << std::endl;
        return 1;
    }

    std::cout << "命令行参数数量: " << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "参数[" << i << "]: " << argv[i] << std::endl;
    }

    bool encrypt = std::strcmp(argv[1], "加密") == 0; // 去掉std::前缀
    const std::string directory = argv[2];
    const std::string key = argv[3];

    std::cout << "开始" << (encrypt ? "加密" : "解密") << "目录：" << directory << std::endl;

    if (encrypt) {
        encrypt_directory(directory, key);
    }
    else {
        decrypt_directory(directory, key);
    }

    std::cout << (encrypt ? "加密" : "解密") << "完成" << std::endl;

    return 0;
}
