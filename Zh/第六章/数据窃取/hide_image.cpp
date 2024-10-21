#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

void hide_file_in_image(const std::string& input_filename, std::ofstream& image_file) {
    std::ifstream file(input_filename, std::ios::binary);
    if (!file) {
        std::cerr << "无法打开文件：" << input_filename << std::endl;
        return;
    }

    std::vector<char> file_data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    size_t file_size = file_data.size();

    // 写入文件名长度、文件名和文件大小
    uint16_t filename_length = input_filename.size();
    image_file.write(reinterpret_cast<const char*>(&filename_length), sizeof(filename_length));
    image_file.write(input_filename.c_str(), filename_length);
    image_file.write(reinterpret_cast<const char*>(&file_size), sizeof(file_size));

    // 写入文件内容
    image_file.write(file_data.data(), file_data.size());
}

void hide_directory_in_image(const std::string& directory, const std::string& image_filename) {
    std::ofstream image_file(image_filename, std::ios::binary | std::ios::app); // 使用app模式以追加内容
    if (!image_file) {
        std::cerr << "无法打开图像文件：" << image_filename << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string input_filename = entry.path().string();
            std::cout << "隐藏文件：" << input_filename << " 到图像中" << std::endl;
            hide_file_in_image(input_filename, image_file);
        }
    }

    std::cout << "成功将目录文件隐藏到图像中，并保存为：" << image_filename << std::endl;
}

void extract_files_from_image(const std::string& image_filename, const std::string& output_directory) {
    std::ifstream image_file(image_filename, std::ios::binary);
    if (!image_file) {
        std::cerr << "无法打开图像文件：" << image_filename << std::endl;
        return;
    }

    while (true) {
        // 读取文件名长度
        uint16_t filename_length;
        image_file.read(reinterpret_cast<char*>(&filename_length), sizeof(filename_length));
        if (image_file.eof()) break;

        // 读取文件名
        std::string filename(filename_length, '\0');
        image_file.read(&filename[0], filename_length);

        // 读取文件大小
        size_t file_size;
        image_file.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));

        // 读取文件内容
        std::vector<char> file_data(file_size);
        image_file.read(file_data.data(), file_size);

        // 写入输出文件
        std::string output_filename = output_directory + "/" + filename;
        std::ofstream output_file(output_filename, std::ios::binary);
        if (!output_file) {
            std::cerr << "无法写入文件：" << output_filename << std::endl;
            continue;
        }
        output_file.write(file_data.data(), file_data.size());
        std::cout << "提取文件：" << output_filename << std::endl;
    }

    std::cout << "成功从图像中提取所有文件到目录：" << output_directory << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "用法: " << argv[0] << " <隐藏/提取> <目录/图像文件> <图像文件/输出目录>" << std::endl;
        return 1;
    }

    std::cout << "命令行参数数量: " << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "参数[" << i << "]: " << argv[i] << std::endl;
    }

    bool hide = std::strcmp(argv[1], "隐藏") == 0;
    const std::string input_path = argv[2];
    const std::string output_path = argv[3];

    std::cout << "操作类型: " << (hide ? "隐藏" : "提取") << std::endl;
    std::cout << "输入路径: " << input_path << std::endl;
    std::cout << "输出路径: " << output_path << std::endl;

    if (hide) {
        hide_directory_in_image(input_path, output_path);
    }
    else {
        extract_files_from_image(input_path, output_path);
    }

    return 0;
}
