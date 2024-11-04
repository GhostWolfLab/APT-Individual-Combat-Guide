#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

void InitializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        exit(1);
    }
}

SOCKET CreateSocket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }
    return sock;
}

void ConnectToServer(SOCKET sock, const char* serverAddress, int port) {
    sockaddr_in serverInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress, &serverInfo.sin_addr);
    int result = connect(sock, (sockaddr*)&serverInfo, sizeof(serverInfo));
    if (result == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        exit(1);
    }
}

void SendData(SOCKET sock, const char* data) {
    int result = send(sock, data, strlen(data), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        exit(1);
    }
}

void CloseSocket(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}

int main() {
    InitializeWinsock();
    SOCKET sock = CreateSocket();
    ConnectToServer(sock, "192.168.0.189", 8080);  // 替换为目标服务器的地址和端口
    const char* data = "Hello, server!";
    SendData(sock, data);
    CloseSocket(sock);
    return 0;
}
