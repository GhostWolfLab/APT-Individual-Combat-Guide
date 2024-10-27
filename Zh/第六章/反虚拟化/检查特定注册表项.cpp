#include <windows.h>
#include <stdio.h>

bool CheckSandboxRegistryKey() {
    HKEY hkResult;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Sandboxie", 0, KEY_QUERY_VALUE, &hkResult) == ERROR_SUCCESS) {
        RegCloseKey(hkResult);
        return true;  // 检测到沙箱注册表项
    }
    return false;  // 未检测到沙箱注册表项
}

int main() {
    if (CheckSandboxRegistryKey()) {
        printf("Running inside a sandbox.\n");
    }
    else {
        printf("Not running inside a sandbox.\n");
        MessageBoxW(NULL, L"I am Snowwolf", L"Ghost Wolf Lab", 0);
    }
    return 0;
}
