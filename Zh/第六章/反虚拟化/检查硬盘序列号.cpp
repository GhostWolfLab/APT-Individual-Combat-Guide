#include <windows.h>
#include <stdio.h>

bool IsVirtualMachine() {
    char serialNumber[128];
    DWORD size = sizeof(serialNumber);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum", "0", RRF_RT_REG_SZ, NULL, &serialNumber, &size) == ERROR_SUCCESS) {
        if (strstr(serialNumber, "VMware") || strstr(serialNumber, "VBOX")) {
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
