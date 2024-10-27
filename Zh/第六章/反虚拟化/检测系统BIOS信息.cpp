#include <windows.h>
#include <stdio.h>

bool IsVirtualMachine() {
    char biosVersion[128];
    DWORD size = sizeof(biosVersion);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", "SystemBiosVersion", RRF_RT_REG_SZ, NULL, &biosVersion, &size) == ERROR_SUCCESS) {
        if (strstr(biosVersion, "VirtualBox") || strstr(biosVersion, "VMware")) {
            return true;
        }
    }
    return false;
}

int main() {
    if (IsVirtualMachine()) {
        printf("Running inside a virtual machine.\n");
    }
    else {
        printf("Not running inside a virtual machine.\n");
    }
    return 0;
}
