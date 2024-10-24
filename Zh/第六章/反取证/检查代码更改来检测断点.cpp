#include <windows.h>
#include <stdio.h>

// 使用链接器选项启用增量链接
#pragma comment(linker, "/INCREMENTAL:YES")

// 计算函数校验和
DWORD CalculateFunctionChecksum(PUCHAR functionStart, PUCHAR functionEnd) {
    DWORD checksum = 0;
    while (functionStart < functionEnd) {
        checksum += *functionStart;
        functionStart++;
    }
    return checksum;
}

// 防止编译器将函数编译为内联
#pragma auto_inline(off)

// 关键函数示例
VOID CrucialFunction() {
    int x = 0;
    x += 2;
}

// 函数结束标记
VOID AfterCrucialFunction() {};

#pragma auto_inline(on)

int main() {
    // 预期的校验和值
    DWORD originalChecksum = 3429;

    // 计算当前函数的校验和
    DWORD checksum = CalculateFunctionChecksum((PUCHAR)CrucialFunction, (PUCHAR)AfterCrucialFunction);

    // 比较校验和
    if (checksum != originalChecksum) {
        printf("Software breakpoint detected!\n");
        return 1;
    }

    // 如果校验和匹配，继续执行
    printf("Not Have Debug! \n");

    return 0;
}
