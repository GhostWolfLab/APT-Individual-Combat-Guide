#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

std::mutex print_mutex;

void scanHostICMP(const std::string& ip) {
    HANDLE hIcmpFile;
    unsigned long ipaddr = inet_addr(ip.c_str());
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Unable to open handle.\n";
        return;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*)malloc(ReplySize);
    if (ReplyBuffer == NULL) {
        std::cerr << "Unable to allocate memory\n";
        return;
    }

    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
    if (dwRetVal != 0) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Reply from " << ip << ": bytes=" << pEchoReply->DataSize << " time=" << pEchoReply->RoundTripTime << "ms TTL=" << (int)pEchoReply->Options.Ttl << "\n";
    }

    free(ReplyBuffer);
    IcmpCloseHandle(hIcmpFile);
}

void scanHostTCP(const std::string& ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(sock);
        return;
    }

    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "Host " << ip << " is up (TCP)\n";
    closesocket(sock);
}

void scanHostUDP(const std::string& ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    char sendData[] = "Hello";
    if (sendto(sock, sendData, sizeof(sendData), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(sock);
        return;
    }

    char recvData[32];
    int serverLen = sizeof(server);
    if (recvfrom(sock, recvData, sizeof(recvData), 0, (sockaddr*)&server, &serverLen) != SOCKET_ERROR) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Host " << ip << " is up (UDP)\n";
    }

    closesocket(sock);
}

void scanNetwork(const std::string& subnet, int start, int end, int threads, const std::string& protocol, int port) {
    std::vector<std::thread> threadPool;
    for (int i = start; i <= end; ++i) {
        std::string ip = subnet + std::to_string(i);
        if (protocol == "icmp") {
            threadPool.emplace_back(scanHostICMP, ip);
        }
        else if (protocol == "tcp") {
            threadPool.emplace_back(scanHostTCP, ip, port);
        }
        else if (protocol == "udp") {
            threadPool.emplace_back(scanHostUDP, ip, port);
        }
        else {
            std::cerr << "Unsupported protocol: " << protocol << "\n";
            return;
        }

        if (threadPool.size() >= threads) {
            for (auto& t : threadPool) {
                if (t.joinable()) {
                    t.join();
                }
            }
            threadPool.clear();
        }
    }
    for (auto& t : threadPool) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int main() {
    std::string subnet, protocol;
    int start, end, threads, port = 0;

    std::cout << "Enter the subnet (e.g., 192.168.1.): ";
    std::cin >> subnet;
    std::cout << "Enter the start of the IP range (e.g., 1): ";
    std::cin >> start;
    std::cout << "Enter the end of the IP range (e.g., 254): ";
    std::cin >> end;
    std::cout << "Enter the number of threads: ";
    std::cin >> threads;
    std::cout << "Enter the protocol (icmp, tcp, udp): ";
    std::cin >> protocol;
    if (protocol == "tcp" || protocol == "udp") {
        std::cout << "Enter the port number: ";
        std::cin >> port;
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    scanNetwork(subnet, start, end, threads, protocol, port);

    WSACleanup();
    return 0;
}
