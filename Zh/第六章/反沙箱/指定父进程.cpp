#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

// 获取父进程ID
DWORD GetParentPID(DWORD pid) {
    DWORD ppid = 0;
    PROCESSENTRY32W processEntry = { 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32W);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32FirstW(hSnapshot, &processEntry)) {
        do {
            if (processEntry.th32ProcessID == pid) {
                ppid = processEntry.th32ParentProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &processEntry));
    }
    CloseHandle(hSnapshot);
    return ppid;
}

int main() {
    DWORD parentPid = GetParentPID(GetCurrentProcessId());
    WCHAR parentName[MAX_PATH + 1];
    DWORD dwParentName = MAX_PATH;
    HANDLE hParent = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, parentPid);
    if (hParent) {
        if (QueryFullProcessImageNameW(hParent, 0, parentName, &dwParentName)) {
            CharUpperW(parentName);
            if (wcsstr(parentName, L"WINDBG.EXE")) {
                CloseHandle(hParent);
                return 1;
            }
        }
        CloseHandle(hParent);
    }

    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK);
    return 0;
}
