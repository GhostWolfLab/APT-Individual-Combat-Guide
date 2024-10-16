#include <iostream>
#include <windows.h>
#include <string>

void QueryRegistryKey(HKEY hKeyRoot, const std::string& subKey) {
    HKEY hKey;
    if (RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key: " << subKey << std::endl;
        return;
    }

    char valueName[256];
    BYTE data[1024];
    DWORD valueNameSize, dataSize, type;
    DWORD index = 0;

    while (true) {
        valueNameSize = sizeof(valueName);
        dataSize = sizeof(data);
        LONG result = RegEnumValueA(hKey, index, valueName, &valueNameSize, nullptr, &type, data, &dataSize);
        if (result == ERROR_NO_MORE_ITEMS) {
            break;
        }
        else if (result != ERROR_SUCCESS) {
            std::cerr << "Failed to enumerate registry values." << std::endl;
            break;
        }

        std::cout << "Value Name: " << valueName << std::endl;
        if (type == REG_SZ) {
            std::cout << "Value Data: " << reinterpret_cast<char*>(data) << std::endl;
        }
        else if (type == REG_DWORD) {
            std::cout << "Value Data: " << *reinterpret_cast<DWORD*>(data) << std::endl;
        }
        else {
            std::cout << "Value Data: (unsupported type)" << std::endl;
        }
        std::cout << std::endl;
        ++index;
    }

    RegCloseKey(hKey);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <root_key> <sub_key>" << std::endl;
        std::cerr << "Example: " << argv[0] << " HKEY_LOCAL_MACHINE SOFTWARE\\Microsoft\\Windows\\CurrentVersion" << std::endl;
        return 1;
    }

    std::string rootKeyStr = argv[1];
    std::string subKey = argv[2];
    HKEY hKeyRoot;

    if (rootKeyStr == "HKEY_LOCAL_MACHINE") {
        hKeyRoot = HKEY_LOCAL_MACHINE;
    }
    else if (rootKeyStr == "HKEY_CURRENT_USER") {
        hKeyRoot = HKEY_CURRENT_USER;
    }
    else if (rootKeyStr == "HKEY_CLASSES_ROOT") {
        hKeyRoot = HKEY_CLASSES_ROOT;
    }
    else if (rootKeyStr == "HKEY_USERS") {
        hKeyRoot = HKEY_USERS;
    }
    else if (rootKeyStr == "HKEY_CURRENT_CONFIG") {
        hKeyRoot = HKEY_CURRENT_CONFIG;
    }
    else {
        std::cerr << "Invalid root key specified." << std::endl;
        return 1;
    }

    QueryRegistryKey(hKeyRoot, subKey);

    return 0;
}
