#include <windows.h>
#include <stdio.h>
#include <math.h>

bool WaitForMouseMovement() {
    POINT currentMousePosition;
    POINT previousMousePosition;
    double mouseDistance = 0;

    // 获取初始鼠标位置
    GetCursorPos(&previousMousePosition);

    while (true) {
        // 获取当前鼠标位置
        GetCursorPos(&currentMousePosition);

        // 计算移动距离
        mouseDistance += sqrt(pow(currentMousePosition.x - previousMousePosition.x, 2) +
            pow(currentMousePosition.y - previousMousePosition.y, 2));

        // 更新上一次鼠标位置
        previousMousePosition = currentMousePosition;

        // 延迟100毫秒
        Sleep(100);

        // 如果移动距离超过设定的阈值，返回真
        if (mouseDistance > 20000) {
            return true;  // 用户已移动鼠标
        }
    }

    return false;  // 不应到达此处
}

int main() {
    if (!WaitForMouseMovement()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK);

    return 0;
}
