#include <windows.h>
#include <stdio.h>
#include <strsafe.h>

// 检查时区
bool CheckTimeZone() {
    // 设置线程区域
    SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));

    DYNAMIC_TIME_ZONE_INFORMATION dynamicTimeZoneInfo;
    if (GetDynamicTimeZoneInformation(&dynamicTimeZoneInfo) == TIME_ZONE_ID_INVALID) {
        return false;  // 无法获取时区信息
    }

    wchar_t timeZoneName[129];
    StringCchCopyW(timeZoneName, 128, dynamicTimeZoneInfo.TimeZoneKeyName);
    CharUpperW(timeZoneName);

    // 检查时区名称是否为目标时区
    if (!wcsstr(timeZoneName, L"CENTRAL EUROPEAN STANDARD TIME")) {
        return false;  // 不在目标时区
    }

    return true;  // 在目标时区
}

int main() {
    if (!CheckTimeZone()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
