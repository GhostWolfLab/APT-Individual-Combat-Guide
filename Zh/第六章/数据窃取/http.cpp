#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <winhttp.h>
#include <algorithm>

#pragma comment(lib, "winhttp.lib")

void send_data(const std::string& server, const std::string& data) {
    std::wstring wServer(server.begin(), server.end());
    std::wstring wData(data.begin(), data.end());

    HINTERNET hSession = WinHttpOpen(L"A HTTP Client/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        std::cerr << "Error in opening session: " << GetLastError() << std::endl;
        return;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, wServer.c_str(), INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnect) {
        std::cerr << "Error in connecting: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hSession);
        return;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wData.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (!hRequest) {
        std::cerr << "Error in opening request: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return;
    }

    BOOL bResult = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, NULL, 0, 0, 0);
    if (!bResult) {
        std::cerr << "Error in sending request: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return;
    }

    bResult = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResult) {
        std::cerr << "Error in receiving response: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return;
    }

    std::cout << "Data sent successfully" << std::endl;

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server> <file>" << std::endl;
        return 1;
    }

    std::string server = argv[1];
    std::string filename = argv[2];
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // 移除换行符和回车符
    data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
    data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());

    std::cout << "Data to be sent: " << data << std::endl;
    send_data(server, data);
    return 0;
}
