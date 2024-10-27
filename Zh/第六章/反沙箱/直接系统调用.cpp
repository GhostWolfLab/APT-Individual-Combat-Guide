#include <windows.h>
#include <iostream>

// 系统调用函数指针
typedef NTSTATUS(NTAPI* PNtCreateThreadEx)(
    PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    PVOID ObjectAttributes,
    HANDLE ProcessHandle,
    LPTHREAD_START_ROUTINE StartAddress,
    PVOID Parameter,
    ULONG CreateFlags,
    ULONG_PTR ZeroBits,
    SIZE_T StackSize,
    SIZE_T MaximumStackSize,
    PVOID AttributeList
    );

void shellcode_exec() {
    MessageBoxA(NULL, "Shellcode executed!", "Alert", MB_OK | MB_ICONINFORMATION);
}

int main() {
    HANDLE hThread;
    HANDLE hProcess = GetCurrentProcess();

    // 获取系统调用的地址
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) {
        std::cerr << "Failed to get handle for ntdll.dll" << std::endl;
        return 1;
    }

    PNtCreateThreadEx pNtCreateThreadEx = (PNtCreateThreadEx)GetProcAddress(hNtdll, "NtCreateThreadEx");
    if (!pNtCreateThreadEx) {
        std::cerr << "Failed to get address for NtCreateThreadEx" << std::endl;
        return 1;
    }

    // 调用系统调用
    NTSTATUS status = pNtCreateThreadEx(&hThread, GENERIC_ALL, NULL, hProcess, (LPTHREAD_START_ROUTINE)shellcode_exec, NULL, FALSE, 0, 0, 0, NULL);
    if (status != 0) {
        std::cerr << "NtCreateThreadEx failed with status: " << status << std::endl;
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}
