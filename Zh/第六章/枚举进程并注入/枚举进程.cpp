#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

void GetProcessSnapshot(DWORD dwProcessID)
{
	HANDLE hSnapshot =
		CreateToolhelp32Snapshot(TH32CS_SNAPALL, dwProcessID);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			printf("The snapshot handle of the process %u is 0x % 08X\n", dwProcessID, hSnapshot);
				PROCESSENTRY32 pe;
			pe.dwSize = sizeof(pe);
			BOOL bProcess = Process32First(hSnapshot, &pe);
				while (bProcess)
				{
					printf("Process ID: %u\n", pe.th32ProcessID);
						printf("Process Name: %s\n", pe.szExeFile);
						printf("Parent Process ID: %u\n",pe.th32ParentProcessID);
					printf("Thread Count: %u\n", pe.cntThreads);
						printf("\n");
					bProcess = Process32Next(hSnapshot, &pe);
				}
				MODULEENTRY32 me;me.dwSize = sizeof(me);
			BOOL bModule = Module32First(hSnapshot, &me);
				while (bModule)
				{
					printf("Module Name: %s\n", me.szModule);
						printf("Module Path: %s\n", me.szExePath);
						printf("Module Handle: 0x%08X\n", me.hModule);
						printf("Module Base Address: 0x%08X\n",
							me.modBaseAddr);
					printf("Module Base Size: %u\n", me.modBaseSize);
					printf("\n");
					bModule = Module32Next(hSnapshot, &me);
				}
			THREADENTRY32 te;
			te.dwSize = sizeof(te);
			BOOL bThread = Thread32First(hSnapshot, &te);
				while (bThread)
				{
					printf("Thread ID: %u\n", te.th32ThreadID);
						printf("Owner Process ID: %u\n",te.th32OwnerProcessID);
					printf("Thread Priority: %d\n", te.tpBasePri);
						printf("\n");
					bThread = Thread32Next(hSnapshot, &te);
				}
				CloseHandle(hSnapshot);
		}
		else
		{
			printf("CreateToolhelp32Snapshot failed with error code % u\n", GetLastError());
		}
}
int main(void)
{
	GetProcessSnapshot(0);
	return 0;
}
