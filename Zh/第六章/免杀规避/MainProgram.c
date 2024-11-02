#include <windows.h>
#include <stdio.h>

typedef void (*ExecuteModuleA_t)();

int main() {
    HMODULE hModuleA = LoadLibrary("ModuleA.dll");
    if (!hModuleA) {
        printf("Failed to load ModuleA.dll\n");
        return 1;
    }

    ExecuteModuleA_t ExecuteModuleA = (ExecuteModuleA_t)GetProcAddress(hModuleA, "ExecuteModuleA");
    if (!ExecuteModuleA) {
        printf("Failed to get function address\n");
        FreeLibrary(hModuleA);
        return 1;
    }

    ExecuteModuleA();  //动态调用模块A的功能

    FreeLibrary(hModuleA);
    return 0;
}
