#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void find_and_display_file(const std::string& filename, const fs::path& directory) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && entry.path().filename() == filename) {
                std::cout << "File found: " << entry.path() << std::endl;

                std::ifstream file(entry.path());
                if (file.is_open()) {
                    std::cout << "File content:" << std::endl;
                    std::string line;
                    while (std::getline(file, line)) {
                        std::cout << line << std::endl;
                    }
                    file.close();
                }
                else {
                    std::cerr << "Unable to open file: " << entry.path() << std::endl;
                }
                return;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
    std::cout << "File not found: " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <directory>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    fs::path directory = argv[2];

    find_and_display_file(filename, directory);

    return 0;
}
