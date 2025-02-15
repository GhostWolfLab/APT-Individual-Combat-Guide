#include <iostream>
#include <windows.h>

int main() {
    HANDLE hPipe;
    DWORD dwWritten;
    char buffer[1024];
    DWORD dwRead;

    hPipe = CreateFile(TEXT("\\\\.\\pipe\\mypipe"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateFile failed: " << GetLastError() << std::endl;
        return 1;
    }

    std::string message = "Hello from the client!\n";
    WriteFile(hPipe, message.c_str(), message.size(), &dwWritten, NULL);

    while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
        buffer[dwRead] = '\0';
        std::cout << "Received response: " << buffer << std::endl;
    }

    CloseHandle(hPipe);
    return 0;
}
