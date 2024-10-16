#include <windows.h>
#include <stdio.h>
#include <tchar.h>
int _cdecl _tmain()
{
	for (int i = 0; i < 26; i++)
	{
		TCHAR szDrive[4];
		wsprintf(szDrive, TEXT("%c:\\"), TEXT('A') + i);
			ULARGE_INTEGER qwFreeBytes, qwFreeBytesToCaller,
			qwTotalBytes;
		BOOL bResult = GetDiskFreeSpaceEx(szDrive,
			&qwFreeBytesToCaller, &qwTotalBytes, &qwFreeBytes);
			if (bResult)
			{
				_tprintf(TEXT("%s: Total bytes: %I64u, Free bytes: % I64u, Free bytes to caller : % I64u\n"),
					szDrive, qwTotalBytes.QuadPart,
					qwFreeBytes.QuadPart, qwFreeBytesToCaller.QuadPart);
			}
	}
	return 0;
}
