#include <windows.h>
#include <stdio.h>

typedef NTSTATUS(NTAPI* NtSetInformationThreadPtr)(
    HANDLE ThreadHandle,
    THREAD_INFORMATION_CLASS ThreadInformationClass,
    PVOID ThreadInformation,
    ULONG ThreadInformationLength
    );

#define ThreadHideFromDebugger 0x11
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

bool HideThreadFromDebugger() {
    HMODULE hNtDll = LoadLibraryA("ntdll.dll");
    if (hNtDll == NULL) {
        return false;
    }

    NtSetInformationThreadPtr NtSetInformationThread =
        (NtSetInformationThreadPtr)GetProcAddress(hNtDll, "NtSetInformationThread");
    if (NtSetInformationThread == NULL) {
        FreeLibrary(hNtDll);
        return false;
    }

    // 使用真实参数调用 NtSetInformationThread
    NTSTATUS status = NtSetInformationThread(GetCurrentThread(), (THREAD_INFORMATION_CLASS)ThreadHideFromDebugger, NULL, 0);

    // 使用虚假参数调用 NtSetInformationThread，检查返回状态
    NTSTATUS fake_status = NtSetInformationThread(GetCurrentThread(), (THREAD_INFORMATION_CLASS)0x12345678, (PVOID)1, 0);
    if (fake_status == STATUS_SUCCESS) {
        printf("NtSetInformationThread function might be hooked!\n");
    }
    else {
        printf("NtSetInformationThread function is not hooked.\n");
    }

    FreeLibrary(hNtDll);
    return status == STATUS_SUCCESS;
}

int main() {
    if (HideThreadFromDebugger()) {
        printf("Thread hidden from debugger successfully.\n");
    }
    else {
        printf("Failed to hide thread from debugger.\n");
    }

    return 0;
}
