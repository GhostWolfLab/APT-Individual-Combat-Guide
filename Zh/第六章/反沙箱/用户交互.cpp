#include <windows.h>
#include <stdio.h>

bool CheckUserInteraction() {
    // 显示消息框并等待用户点击
    int result = MessageBoxW(NULL, L"If you are a real person,please click OK", L"Hello", MB_OK);
    if (result == IDOK) {
        return true;  // 用户点击了OK按钮
    }
    return false;  // 用户没有点击OK按钮，可能是沙箱环境
}

int main() {
    if (!CheckUserInteraction()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
