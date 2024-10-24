#include <windows.h>
#include <stdio.h>
#include <psapi.h>

// 检查内存页面权限的函数
bool CheckMemoryPages() {
    HANDLE hProcess = GetCurrentProcess();
    MEMORY_BASIC_INFORMATION mbi;
    unsigned char* addr = 0;

    while (VirtualQueryEx(hProcess, addr, &mbi, sizeof(mbi)) == sizeof(mbi)) {
        if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_EXECUTE_READWRITE)) {
            // 检查是否存在写时复制（Copy-on-Write）
            if (!(mbi.Protect & PAGE_WRITECOPY)) {
                printf("Memory page with execute and write permissions found!\n");
                return true;
            }
        }
        addr += mbi.RegionSize;
    }

    return false;
}

int main() {
    if (CheckMemoryPages()) {
        printf("Possible breakpoint detected based on memory page permissions.\n");
    }
    else {
        printf("No breakpoints detected based on memory page permissions.\n");
    }
    return 0;
}
