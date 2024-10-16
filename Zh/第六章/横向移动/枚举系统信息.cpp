#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <iphlpapi.h>
#include <tchar.h>
#include <ntstatus.h>
#include <winternl.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ntdll.lib")

typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

void writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << content << std::endl;
        file.close();
    }
    else {
        std::cerr << "Unable to open file" << std::endl;
    }
}

std::string getOSInfo() {
    // 获取操作系统信息
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fnRtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fnRtlGetVersion != nullptr) {
            RTL_OSVERSIONINFOW osInfo = { 0 };
            osInfo.dwOSVersionInfoSize = sizeof(osInfo);
            if (fnRtlGetVersion(&osInfo) == 0) {
                std::string osInfoStr = "OS Name: Windows\nVersion: " + std::to_string(osInfo.dwMajorVersion) + "." + std::to_string(osInfo.dwMinorVersion) + "\n";
                return osInfoStr;
            }
        }
    }
    return "Failed to get OS version information\n";
}

std::string getCPUInfo() {
    // 获取CPU信息
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::string cpuInfo = "CPU Architecture: " + std::to_string(sysInfo.wProcessorArchitecture) + "\nNumber of Cores: " + std::to_string(sysInfo.dwNumberOfProcessors) + "\n";
    return cpuInfo;
}

std::string getNetworkInfo() {
    // 获取网络适配器信息
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        std::cerr << "Error allocating memory" << std::endl;
        return "";
    }
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            std::cerr << "Error allocating memory" << std::endl;
            return "";
        }
    }
    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        std::string networkInfo = "";
        while (pAdapter) {
            networkInfo += "Adapter Name: " + std::string(pAdapter->AdapterName) + "\n";
            networkInfo += "IP Address: " + std::string(pAdapter->IpAddressList.IpAddress.String) + "\n";
            networkInfo += "MAC Address: ";
            for (UINT i = 0; i < pAdapter->AddressLength; i++) {
                if (i == (pAdapter->AddressLength - 1))
                    networkInfo += std::to_string((int)pAdapter->Address[i]);
                else
                    networkInfo += std::to_string((int)pAdapter->Address[i]) + "-";
            }
            networkInfo += "\n";
            pAdapter = pAdapter->Next;
        }
        free(pAdapterInfo);
        return networkInfo;
    }
    else {
        std::cerr << "GetAdaptersInfo failed with error: " << dwRetVal << std::endl;
        return "";
    }
}

int main() {
    std::string filename = "system_info.txt";
    std::vector<std::string> options = { "OS", "CPU", "Network", "All" };
    std::cout << "Choose information to retrieve (OS, CPU, Network, All): ";
    std::string choice;
    std::cin >> choice;

    if (choice == "OS" || choice == "All") {
        std::string osInfo = getOSInfo();
        writeToFile(filename, osInfo);
    }
    if (choice == "CPU" || choice == "All") {
        std::string cpuInfo = getCPUInfo();
        writeToFile(filename, cpuInfo);
    }
    if (choice == "Network" || choice == "All") {
        std::string networkInfo = getNetworkInfo();
        writeToFile(filename, networkInfo);
    }

    std::cout << "Information written to " << filename << std::endl;
    return 0;
}
