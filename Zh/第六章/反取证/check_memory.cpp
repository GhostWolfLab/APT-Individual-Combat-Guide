#include <windows.h>
#include <stdio.h>
#include <string.h>

// 原始代码段示例
char original_code[] = "\x90\x90\x90\x90";  // NOP指令

void detect_memory_modification() {
    char current_code[sizeof(original_code)];

    // 复制当前代码段数据
    memcpy(current_code, original_code, sizeof(original_code));

    // 检查代码段是否被修改
    if (memcmp(original_code, current_code, sizeof(original_code)) != 0) {
        printf("Code modification detected!\n");
    }
    else {
        printf("No code modification.\n");
    }
}

int main() {
    printf("Starting memory modification detection...\n");
    detect_memory_modification();
    printf("Detection completed.\n");
    return 0;
}
