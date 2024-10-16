#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

void DumpMemory(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processID);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process." << std::endl;
        return;
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    MEMORY_BASIC_INFORMATION memInfo;
    LPVOID address = sysInfo.lpMinimumApplicationAddress;

    while (address < sysInfo.lpMaximumApplicationAddress) {
        if (VirtualQueryEx(hProcess, address, &memInfo, sizeof(memInfo)) == sizeof(memInfo)) {
            if (memInfo.State == MEM_COMMIT && (memInfo.Type == MEM_PRIVATE || memInfo.Type == MEM_IMAGE)) {
                BYTE* buffer = new BYTE[memInfo.RegionSize];
                SIZE_T bytesRead;
                if (ReadProcessMemory(hProcess, address, buffer, memInfo.RegionSize, &bytesRead)) {
                    // Process the memory data here
                    std::cout << "Read " << bytesRead << " bytes from address " << address << std::endl;
                }
                delete[] buffer;
            }
        }
        address = (LPVOID)((SIZE_T)address + memInfo.RegionSize);
    }

    CloseHandle(hProcess);
}

int main() {
    DWORD processID;
    std::cout << "Enter the process ID: ";
    std::cin >> processID;
    DumpMemory(processID);
    return 0;
}
