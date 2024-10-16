#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>

void SetConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
std::string ConvertWCharToString(const WCHAR* wchar) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
    std::string str(bufferSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, wchar, -1, &str[0], bufferSize, nullptr, nullptr);
    return str;
}

void ListProcesses(const std::string& targetProcess) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateToolhelp32Snapshot failed" << std::endl;
        return;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Process32First failed" << std::endl;
        CloseHandle(hProcessSnap);
        return;
    }

    do {
        std::string processName = ConvertWCharToString(pe32.szExeFile);
        if (processName == targetProcess) {
            SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "Process ID: " << pe32.th32ProcessID << "\tProcess Name: " << processName << std::endl;
            SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }
        else {
            std::cout << "Process ID: " << pe32.th32ProcessID << "\tProcess Name: " << processName << std::endl;
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <process_name>" << std::endl;
        return 1;
    }

    std::string targetProcess = argv[1];
    ListProcesses(targetProcess);
    return 0;
}
