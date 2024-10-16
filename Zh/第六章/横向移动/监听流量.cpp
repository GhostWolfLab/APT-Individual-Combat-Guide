#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

void handle_tcp_client(SOCKET client_socket) {
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }
    closesocket(client_socket);
}

void start_tcp_server(const std::string& ip, int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, SOMAXCONN);

    std::cout << "TCP server listening on " << ip << ":" << port << std::endl;

    while (true) {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
        std::thread(handle_tcp_client, client_socket).detach();
    }

    closesocket(server_socket);
    WSACleanup();
}

void start_udp_server(const std::string& ip, int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));

    std::cout << "UDP server listening on " << ip << ":" << port << std::endl;

    char buffer[1024];
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
    while (true) {
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &client_addr_size);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    closesocket(server_socket);
    WSACleanup();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <protocol> <IP> <port>" << std::endl;
        return 1;
    }

    std::string protocol = argv[1];
    std::string ip = argv[2];
    int port = std::stoi(argv[3]);

    if (protocol == "tcp") {
        start_tcp_server(ip, port);
    }
    else if (protocol == "udp") {
        start_udp_server(ip, port);
    }
    else {
        std::cerr << "Unsupported protocol: " << protocol << std::endl;
        return 1;
    }

    return 0;
}
