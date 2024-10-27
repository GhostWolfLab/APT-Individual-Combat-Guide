#include <windows.h>
#include <stdio.h>

// 检查特定文件
bool CheckForVirtualBoxFiles() {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(L"C:\\Windows\\System32\\VBox*.dll", &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        return true;  // 检测到VirtualBox文件
    }
    return false;  // 未检测到VirtualBox文件
}

// 检查特定注册表项
bool CheckForVirtualBoxRegistryKey() {
    HKEY hkResult;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\Services\\VBoxSF", 0, KEY_QUERY_VALUE, &hkResult) == ERROR_SUCCESS) {
        RegCloseKey(hkResult);
        return true;  // 检测到VirtualBox注册表项
    }
    return false;  // 未检测到VirtualBox注册表项
}

int main() {
    if (CheckForVirtualBoxFiles() || CheckForVirtualBoxRegistryKey()) {
        printf("Running inside VirtualBox.\n");
    }
    else {
        printf("Not running inside VirtualBox.\n");
    }

    return 0;
}
