#include <windows.h>
#include <stdio.h>

bool CheckAndUnhookFunction() {
    // 手动加载DLL
    HANDLE dllFile = CreateFileW(L"C:\\Windows\\System32\\ntdll.dll", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (dllFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD dllFileSize = GetFileSize(dllFile, NULL);
    HANDLE hDllFileMapping = CreateFileMappingW(dllFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, NULL);
    if (!hDllFileMapping) {
        CloseHandle(dllFile);
        return false;
    }

    PVOID pDllFileMappingBase = MapViewOfFile(hDllFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (!pDllFileMappingBase) {
        CloseHandle(hDllFileMapping);
        CloseHandle(dllFile);
        return false;
    }

    CloseHandle(dllFile);

    // 解析DLL头信息
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pDllFileMappingBase;
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDllFileMappingBase + pDosHeader->e_lfanew);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)&(pNtHeader->OptionalHeader);
    PIMAGE_EXPORT_DIRECTORY pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)pDllFileMappingBase + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    PULONG pAddressOfFunctions = (PULONG)((PBYTE)pDllFileMappingBase + pExportDirectory->AddressOfFunctions);
    PULONG pAddressOfNames = (PULONG)((PBYTE)pDllFileMappingBase + pExportDirectory->AddressOfNames);
    PUSHORT pAddressOfNameOrdinals = (PUSHORT)((PBYTE)pDllFileMappingBase + pExportDirectory->AddressOfNameOrdinals);

    // 查找原始函数代码位置
    PVOID pNtCreateThreadExOriginal = NULL;
    for (DWORD i = 0; i < pExportDirectory->NumberOfNames; ++i) {
        PCSTR pFunctionName = (PSTR)((PBYTE)pDllFileMappingBase + pAddressOfNames[i]);
        if (!strcmp(pFunctionName, "NtCreateThreadEx")) {
            pNtCreateThreadExOriginal = (PVOID)((PBYTE)pDllFileMappingBase + pAddressOfFunctions[pAddressOfNameOrdinals[i]]);
            break;
        }
    }

    if (!pNtCreateThreadExOriginal) {
        UnmapViewOfFile(pDllFileMappingBase);
        CloseHandle(hDllFileMapping);
        return false;
    }

    // 比较函数代码
    PVOID pNtCreateThreadEx = GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtCreateThreadEx");
    if (memcmp(pNtCreateThreadEx, pNtCreateThreadExOriginal, 16) != 0) {
        UnmapViewOfFile(pDllFileMappingBase);
        CloseHandle(hDllFileMapping);
        return false;  // 检测到挂钩
    }

    UnmapViewOfFile(pDllFileMappingBase);
    CloseHandle(hDllFileMapping);

    return true;  // 没有检测到挂钩
}

int main() {
    if (!CheckAndUnhookFunction()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
