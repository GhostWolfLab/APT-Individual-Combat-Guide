#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

std::mutex cout_mutex;

void scan_port(const std::string& ip, int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Port " << port << " is open on " << ip << std::endl;
    }
    closesocket(sock);
    WSACleanup();
}

void scan_ports(const std::string& ip, int start_port, int end_port, int thread_count) {
    std::vector<std::thread> threads;
    for (int port = start_port; port <= end_port; ++port) {
        if (threads.size() >= thread_count) {
            for (auto& t : threads) {
                t.join();
            }
            threads.clear();
        }
        threads.emplace_back(scan_port, ip, port);
    }
    for (auto& t : threads) {
        t.join();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <IP> <start_port> <end_port> <thread_count>" << std::endl;
        return 1;
    }

    std::string ip = argv[1];
    int start_port = std::stoi(argv[2]);
    int end_port = std::stoi(argv[3]);
    int thread_count = std::stoi(argv[4]);

    scan_ports(ip, start_port, end_port, thread_count);

    return 0;
}
