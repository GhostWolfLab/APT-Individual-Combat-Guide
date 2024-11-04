#include <windows.h>
#include <iostream>
#include <string>

#ifndef UNLEN
#define UNLEN 256
#endif

//提取机器名和用户名生成密钥
std::wstring GetEnvironmentKey() {
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    GetComputerNameW(computerName, &size);

    wchar_t userName[UNLEN + 1];
    size = sizeof(userName) / sizeof(userName[0]);
    GetUserNameW(userName, &size);

    std::wstring environmentKey = std::wstring(computerName) + L"_" + std::wstring(userName);
    return environmentKey;
}

void ExecutePayload(const std::wstring& key) {
    const std::wstring expectedKey = L"WIN-DBR3H49FQJ3_administrator"; // 预期的环境密钥
    if (key == expectedKey) {
        MessageBoxW(NULL, L"Payload executed", L"Alert", MB_OK | MB_ICONINFORMATION);
    } else {
        std::wcout << L"Environment key mismatch. Payload not executed." << std::endl;
    }
}

int main() {
    std::wstring envKey = GetEnvironmentKey();
    ExecutePayload(envKey);
    return 0;
}
