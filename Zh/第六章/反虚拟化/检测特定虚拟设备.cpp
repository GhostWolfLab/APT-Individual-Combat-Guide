#include <windows.h>
#include <stdio.h>
#include <winternl.h>

typedef NTSTATUS(NTAPI* NtCreateFilePtr)(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength
    );

typedef VOID(WINAPI* RtlInitUnicodeStringPtr)(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

bool IsVirtualMachine() {
    HMODULE hNtDll = LoadLibraryA("ntdll.dll");
    if (hNtDll == NULL) {
        return false;
    }

    NtCreateFilePtr NtCreateFile = (NtCreateFilePtr)GetProcAddress(hNtDll, "NtCreateFile");
    RtlInitUnicodeStringPtr RtlInitUnicodeString = (RtlInitUnicodeStringPtr)GetProcAddress(hNtDll, "RtlInitUnicodeString");
    if (NtCreateFile == NULL || RtlInitUnicodeString == NULL) {
        FreeLibrary(hNtDll);
        return false;
    }

    UNICODE_STRING uDeviceName;
    OBJECT_ATTRIBUTES objectAttributes;
    HANDLE hDevice = NULL;
    IO_STATUS_BLOCK ioStatusBlock;

    // 检查VBoxGuest设备
    RtlSecureZeroMemory(&uDeviceName, sizeof(uDeviceName));
    RtlInitUnicodeString(&uDeviceName, L"\\Device\\VBoxGuest");
    InitializeObjectAttributes(&objectAttributes, &uDeviceName, OBJ_CASE_INSENSITIVE, NULL, NULL);

    NTSTATUS status = NtCreateFile(
        &hDevice,
        GENERIC_READ,
        &objectAttributes,
        &ioStatusBlock,
        NULL,
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        FILE_OPEN,
        0,
        NULL,
        0
    );

    FreeLibrary(hNtDll);

    if (NT_SUCCESS(status)) {
        return true;  // 检测到虚拟机
    }

    return false;  // 未检测到虚拟机
}

int main() {
    if (IsVirtualMachine()) {
        printf("Running inside a virtual machine.\n");
    }
    else {
        printf("Not running inside a virtual machine.\n");
        MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);
    }
    return 0;
}
