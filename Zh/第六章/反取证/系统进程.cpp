#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
// 定义要检测的调试器或分析工具的进程名
#define OLLYDBG L"ollydbg.exe"
#define PROCEXP L"procexp.exe"
// 定义要检测的调试器或分析工具的窗口标题
#define OLLYDBG_TITLE L"OllyDbg"
#define PROCEXP_TITLE L"Process Explorer"
// 定义要检测的调试器或分析工具的窗口类名
#define OLLYDBG_CLASS L"OLLYDBG"
#define PROCEXP_CLASS L"PROCEXPL"

void MaliciousCode()
{
	MessageBoxA(NULL, "You have been infected by a malware!","Malware Alert", MB_OK | MB_ICONWARNING);
}

void AntiDebugCode()
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pe;
	DWORD dwCurrentProcessId;
	BOOL bFound = FALSE;
	dwCurrentProcessId = GetCurrentProcessId();
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot failed with error code % d\n", GetLastError());
		ExitProcess(0);
	}
	pe.dwSize = sizeof(PROCESSENTRY32);

	// 遍历快照中的进程信息
	if (Process32First(hSnapshot, &pe))
	{
		do
		{
			if (_wcsicmp(pe.szExeFile, OLLYDBG) == 0 || _wcsicmp(pe.szExeFile, PROCEXP) == 0)
			{
				HWND hWnd = FindWindow(NULL, pe.szExeFile);
				if (hWnd != NULL)
				{
					WCHAR szTitle[MAX_PATH];
					WCHAR szClass[MAX_PATH];
					GetWindowText(hWnd, szTitle, MAX_PATH);
					GetClassName(hWnd, szClass, MAX_PATH);
						if ((_wcsicmp(szTitle, OLLYDBG_TITLE) == 0 && _wcsicmp(szClass, OLLYDBG_CLASS) == 0) || (_wcsicmp(szTitle, PROCEXP_TITLE) == 0 && _wcsicmp(szClass, PROCEXP_CLASS) == 0))
						{
							bFound = TRUE;
							break;
						}
				}
			}
		} while (Process32Next(hSnapshot, &pe));
	}
	CloseHandle(hSnapshot);
	if (bFound)
	{
		ExitProcess(0);
	}
}
int main()
{
	AntiDebugCode();
	MaliciousCode();
	return 0;
}
