#include <windows.h>
#include <stdio.h>

bool IsDebugging() {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    wchar_t szPath[MAX_PATH];

    if (!GetModuleFileNameW(NULL, szPath, MAX_PATH)) {
        return false;
    }

    if (!CreateProcessW(szPath, GetCommandLineW(), NULL, NULL, FALSE, CREATE_SUSPENDED | DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &si, &pi)) {
        return false;
    }

    bool isDebugging = DebugActiveProcess(GetProcessId(pi.hProcess));
    if (isDebugging) {
        DebugActiveProcessStop(GetProcessId(pi.hProcess));
    }

    TerminateProcess(pi.hProcess, 0);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return isDebugging;
}

int main() {
    if (IsDebugging()) {
        printf("Debugger detected!\n");
    }
    else {
        printf("No debugger detected.\n");
    }

    return 0;
}
