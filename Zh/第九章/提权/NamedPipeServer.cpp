#include <iostream>
#include <windows.h>
#include <aclapi.h>

int main() {
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;

    SECURITY_ATTRIBUTES sa;
    PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(pSD, TRUE, NULL, FALSE);

    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = pSD;
    sa.bInheritHandle = FALSE;

    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\mypipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        &sa);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateNamedPipe failed: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Waiting for connection..." << std::endl;
    if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
        std::cout << "Client connected." << std::endl;

        while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
            buffer[dwRead] = '\0';
            std::cout << "Received message: " << buffer << std::endl;
            std::string response = "Hello from the server!\n";
            DWORD dwWritten;
            WriteFile(hPipe, response.c_str(), response.size(), &dwWritten, NULL);
        }
    }

    CloseHandle(hPipe);
    LocalFree(pSD);
    return 0;
}
