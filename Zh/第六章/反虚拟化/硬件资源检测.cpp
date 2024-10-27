#include <windows.h>
#include <stdio.h>

bool IsVirtualMachine() {
    // 检查CPU核心数
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    DWORD numberOfProcessors = systemInfo.dwNumberOfProcessors;
    if (numberOfProcessors < 2) {
        return true;  // 可能是虚拟机
    }

    // 检查RAM大小
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    DWORD RAMMB = memoryStatus.ullTotalPhys / 1024 / 1024;
    if (RAMMB < 2048) {
        return true;  // 可能是虚拟机
    }

    // 检查硬盘容量
    HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        return true;  // 无法打开硬盘，可能是虚拟机
    }

    DISK_GEOMETRY pDiskGeometry;
    DWORD bytesReturned;
    if (!DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &pDiskGeometry, sizeof(pDiskGeometry), &bytesReturned, NULL)) {
        CloseHandle(hDevice);
        return true;  // 无法获取硬盘信息，可能是虚拟机
    }

    CloseHandle(hDevice);

    DWORD diskSizeGB = pDiskGeometry.Cylinders.QuadPart * (ULONG)pDiskGeometry.TracksPerCylinder * (ULONG)pDiskGeometry.SectorsPerTrack * (ULONG)pDiskGeometry.BytesPerSector / 1024 / 1024 / 1024;
    if (diskSizeGB < 100) {
        return true;  // 可能是虚拟机
    }

    return false;  // 不是虚拟机
}

int main() {
    if (IsVirtualMachine()) {
        printf("Running inside a virtual machine.\n");
    } else {
        printf("Not running inside a virtual machine.\n");
        MessageBoxW(NULL, L"I am Snowwolf", L"Ghost Wolf Lab", 0);
    }
    return 0;
}
