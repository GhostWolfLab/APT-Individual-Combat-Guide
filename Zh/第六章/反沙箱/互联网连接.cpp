#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#pragma comment(lib, "winhttp.lib")

bool CheckInternetConnection() {
    // 打开HTTP会话
    HINTERNET hSession = WinHttpOpen(L"Mozilla/5.0", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        return false;
    }

    // 连接到服务器
    HINTERNET hConnection = WinHttpConnect(hSession, L"www.baidu.com", INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnection) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 创建HTTP请求
    HINTERNET hRequest = WinHttpOpenRequest(hConnection, L"GET", L"test", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnection);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 发送HTTP请求
    BOOL status = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (!status) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnection);
        WinHttpCloseHandle(hSession);
        return false;  // 没有有效的互联网连接
    }

    // 清理资源
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnection);
    WinHttpCloseHandle(hSession);

    return true;  // 有效的互联网连接
}

int main() {
    if (!CheckInternetConnection()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
