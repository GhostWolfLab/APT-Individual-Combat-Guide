#include <windows.h>
#include <stdio.h>
#include <winternl.h>

typedef NTSTATUS(WINAPI* NtQueryInformationProcessPtr)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

int main() {
    HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
    NtQueryInformationProcessPtr NtQueryInformationProcess = (NtQueryInformationProcessPtr)GetProcAddress(hNtDll, "NtQueryInformationProcess");

    PROCESS_BASIC_INFORMATION pbi;
    ULONG returnLength;
    NtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &pbi, sizeof(pbi), &returnLength);

    if (pbi.PebBaseAddress->BeingDebugged) {
        printf("Debugger detected!\n");
    }
    else {
        printf("No debugger detected.\n");
    }
    return 0;
}
