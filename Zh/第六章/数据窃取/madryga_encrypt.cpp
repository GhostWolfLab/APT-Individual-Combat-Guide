#include <iostream>
#include <fstream>
#include <string>

// Madryga加密函数
void madryga_encrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext) {
    for (size_t i = 0; i < plaintext.size(); i++) {
        ciphertext += (char)(plaintext[i] ^ key[i % key.size()]);
    }
}

// Madryga解密函数
void madryga_decrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext) {
    for (size_t i = 0; i < ciphertext.size(); i++) {
        plaintext += (char)(ciphertext[i] ^ key[i % key.size()]);
    }
}

int main() {
    // 指定文件路径和秘钥
    std::string filePath, encryptedFilePath, decryptedFilePath, key;

    // 读取文件路径和秘钥
    std::cout << "请输入文件路径：";
    std::cin >> filePath;
    std::cout << "请输入加密后文件路径：";
    std::cin >> encryptedFilePath;
    std::cout << "请输入解密后文件路径：";
    std::cin >> decryptedFilePath;
    std::cout << "请输入秘钥：";
    std::cin >> key;

    // 读取文件内容
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件。" << std::endl;
        return 1;
    }

    std::string plaintext((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();

    // 加密文件内容
    std::string ciphertext;
    madryga_encrypt(plaintext, key, ciphertext);

    // 保存加密后的文件
    std::ofstream encryptedFile(encryptedFilePath, std::ios::binary);
    if (!encryptedFile.is_open()) {
        std::cerr << "无法保存加密后的文件。" << std::endl;
        return 1;
    }

    encryptedFile.write(ciphertext.c_str(), ciphertext.size());
    encryptedFile.close();

    // 解密文件内容
    std::string decryptedText;
    madryga_decrypt(ciphertext, key, decryptedText);

    // 保存解密后的文件
    std::ofstream decryptedFile(decryptedFilePath, std::ios::binary);
    if (!decryptedFile.is_open()) {
        std::cerr << "无法保存解密后的文件。" << std::endl;
        return 1;
    }

    decryptedFile.write(decryptedText.c_str(), decryptedText.size());
    decryptedFile.close();

    return 0;
}
