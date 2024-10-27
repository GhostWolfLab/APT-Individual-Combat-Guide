#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

bool CheckSandboxProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    bool found = false;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (_wcsicmp(pe32.szExeFile, L"vmtoolsd.exe") == 0) {
            found = true;  // 检测到沙箱进程
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return found;
}

int main() {
    if (CheckSandboxProcesses()) {
        printf("Running inside a sandbox.\n");
    }
    else {
        printf("Not running inside a sandbox.\n");
        MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);
    }
    return 0;
}
