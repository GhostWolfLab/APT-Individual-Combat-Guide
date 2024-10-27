#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>

void CheckLoadedModules() {
    DWORD runningProcessesIDs[1024], runningProcessesBytes;
    if (!EnumProcesses(runningProcessesIDs, sizeof(runningProcessesIDs), &runningProcessesBytes)) {
        return;
    }

    for (int i = 0; i < runningProcessesBytes / sizeof(DWORD); i++) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, runningProcessesIDs[i]);
        if (!hProcess) continue;

        HMODULE processModules[1024];
        DWORD processModulesBytes;
        if (EnumProcessModules(hProcess, processModules, sizeof(processModules), &processModulesBytes)) {
            for (int j = 0; j < processModulesBytes / sizeof(HMODULE); j++) {
                WCHAR moduleName[MAX_PATH + 1];
                if (GetModuleFileNameExW(hProcess, processModules[j], moduleName, MAX_PATH)) {
                    CharUpperW(moduleName);
                    if (wcsstr(moduleName, L"DBGHELP.DLL")) {
                        CloseHandle(hProcess);
                        exit(0);  // 检测到调试模块，退出
                    }
                }
            }
        }
        CloseHandle(hProcess);
    }
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK);
}

int main() {
    CheckLoadedModules();
    return 0;
}
