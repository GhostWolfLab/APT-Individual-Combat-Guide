#include <windows.h>
#include <stdio.h>

int main() {
    // 尝试阻止用户输入
    if (BlockInput(TRUE)) {
        printf("User input blocked successfully.\n");

        // 等待5秒钟，演示阻止效果
        Sleep(5000);

        // 恢复用户输入
        BlockInput(FALSE);
        printf("User input unblocked.\n");
    }
    else {
        printf("Failed to block user input. Make sure to run as administrator.\n");
    }

    return 0;
}
