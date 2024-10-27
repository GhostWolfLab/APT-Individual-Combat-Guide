#include <windows.h>
#include <stdio.h>

// 模拟函数挂钩
void HookFunction() {
    // 获取MessageBoxW函数地址
    PVOID pMessageBoxW = GetProcAddress(GetModuleHandleW(L"user32.dll"), "MessageBoxW");

    // 修改函数保护属性以进行写操作
    DWORD oldProtect;
    VirtualProtect(pMessageBoxW, 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    // 模拟挂钩，使用RET指令
    char hook[] = { 0xC3 };  // RET
    memcpy(pMessageBoxW, hook, 1);

    // 恢复函数保护属性
    VirtualProtect(pMessageBoxW, 1, oldProtect, &oldProtect);
}

// 从磁盘加载DLL并找到原始函数代码（示例代码）
PVOID LoadDllFromDiskAndFindFunctionCode() {
    HANDLE dllFile = CreateFileW(L"C:\\Windows\\System32\\user32.dll", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (dllFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    HANDLE hDllFileMapping = CreateFileMappingW(dllFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, NULL);
    if (!hDllFileMapping) {
        CloseHandle(dllFile);
        return NULL;
    }

    PVOID pDllFileMappingBase = MapViewOfFile(hDllFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (!pDllFileMappingBase) {
        CloseHandle(hDllFileMapping);
        CloseHandle(dllFile);
        return NULL;
    }

    CloseHandle(dllFile);

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pDllFileMappingBase;
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDllFileMappingBase + pDosHeader->e_lfanew);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)&(pNtHeader->OptionalHeader);
    PIMAGE_EXPORT_DIRECTORY pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)pDllFileMappingBase + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    PULONG pAddressOfFunctions = (PULONG)((PBYTE)pDllFileMappingBase + pExportDirectory->AddressOfFunctions);
    PULONG pAddressOfNames = (PULONG)((PBYTE)pDllFileMappingBase + pExportDirectory->AddressOfNames);
    PUSHORT pAddressOfNameOrdinals = (PUSHORT)((PBYTE)pDllFileMappingBase + pExportDirectory->AddressOfNameOrdinals);

    PVOID pFunctionOriginal = NULL;
    for (DWORD i = 0; i < pExportDirectory->NumberOfNames; ++i) {
        PCSTR pFunctionName = (PSTR)((PBYTE)pDllFileMappingBase + pAddressOfNames[i]);
        if (!strcmp(pFunctionName, "MessageBoxW")) {
            pFunctionOriginal = (PVOID)((PBYTE)pDllFileMappingBase + pAddressOfFunctions[pAddressOfNameOrdinals[i]]);
            break;
        }
    }

    UnmapViewOfFile(pDllFileMappingBase);
    CloseHandle(hDllFileMapping);

    return pFunctionOriginal;
}

// 检测并修复挂钩函数
void DetectAndFixHook() {
    PVOID pMessageBoxWOriginal = LoadDllFromDiskAndFindFunctionCode();
    if (!pMessageBoxWOriginal) {
        return;
    }

    PVOID pMessageBoxWHooked = GetProcAddress(GetModuleHandleW(L"user32.dll"), "MessageBoxW");

    if (memcmp(pMessageBoxWHooked, pMessageBoxWOriginal, 16)) {
        DWORD oldProtection, tempProtection;
        VirtualProtect(pMessageBoxWHooked, 16, PAGE_EXECUTE_READWRITE, &oldProtection);
        memcpy(pMessageBoxWHooked, pMessageBoxWOriginal, 16);
        VirtualProtect(pMessageBoxWHooked, 16, oldProtection, &tempProtection);
    }
}

int main() {
    // 模拟函数挂钩
    HookFunction();

    // 检测挂钩并修复函数
    DetectAndFixHook();

    // 测试修复后的函数
    MessageBoxW(NULL, L"Fixed", L"Fixed", 0);

    return 0;
}
