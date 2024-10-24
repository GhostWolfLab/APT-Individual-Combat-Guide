#include <windows.h>
#include <stdio.h>

// 回调函数
VOID CALLBACK MyCallback(DWORD errorCode, DWORD bytesTransferred, LPOVERLAPPED pOverlapped) {
    MessageBoxW(NULL, L"I am Snowwolf", L"Ghost Wolf Lab", 0);
}

int main() {
    // 打开文件
    HANDLE hFile = CreateFileW(L"C:\\Windows\\win.ini", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file.\n");
        return 1;
    }

    // 分配内存
    PVOID fileBuffer = VirtualAlloc(0, 64, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (fileBuffer == NULL) {
        printf("Failed to allocate memory.\n");
        CloseHandle(hFile);
        return 1;
    }

    // 重叠结构体
    OVERLAPPED overlapped = { 0 };

    // 异步读取文件
    if (!ReadFileEx(hFile, fileBuffer, 32, &overlapped, MyCallback)) {
        printf("Failed to read file asynchronously.\n");
        VirtualFree(fileBuffer, 0, MEM_RELEASE);
        CloseHandle(hFile);
        return 1;
    }

    // 等待异步操作完成
    WaitForSingleObjectEx(hFile, INFINITE, TRUE);
    wprintf_s(L"Already pwned...\n");

    // 释放资源
    VirtualFree(fileBuffer, 0, MEM_RELEASE);
    CloseHandle(hFile);

    return 0;
}
