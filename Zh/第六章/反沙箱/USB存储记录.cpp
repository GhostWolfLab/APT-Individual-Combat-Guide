#include <windows.h>
#include <stdio.h>

// 检查USB存储记录
bool CheckUSBStorageRecords() {
    HKEY hKey;
    DWORD mountedUSBDevicesCount;

    // 打开注册表项
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\Enum\\USBSTOR", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        // 查询注册表信息
        if (RegQueryInfoKeyW(hKey, NULL, NULL, NULL, &mountedUSBDevicesCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            // 如果没有安装过USB存储设备，可能是虚拟环境
            if (mountedUSBDevicesCount < 1) {
                return true;
            }
        }
        else {
            RegCloseKey(hKey);
        }
    }

    return false;
}

int main() {
    if (CheckUSBStorageRecords()) {
        printf("Running inside a sandbox.\n");
    }
    else {
        printf("Not running inside a sandbox.\n");
        // 这里插入恶意代码，比如显示一个弹窗消息
        MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);
    }
    return 0;
}
