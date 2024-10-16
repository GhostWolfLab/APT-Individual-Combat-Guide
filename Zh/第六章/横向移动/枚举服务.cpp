#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

std::mutex cout_mutex;

void identify_service(const std::string& ip, int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
        char buffer[1024];
        int bytes_received;

        //发送简单的HTTP GET请求来识别HTTP服务
        std::string http_request = "GET / HTTP/1.1\r\nHost: " + ip + "\r\n\r\n";
        send(sock, http_request.c_str(), http_request.size(), 0);
        bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            if (strstr(buffer, "HTTP/")) {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Port " << port << " is open on " << ip << " (HTTP)" << std::endl;
                closesocket(sock);
                WSACleanup();
                return;
            }
        }

        //在这里添加更多服务识别逻辑（例如FTP、SSH等）

        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Port " << port << " is open on " << ip << " (Unknown service)" << std::endl;
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
        threads.emplace_back(identify_service, ip, port);
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
