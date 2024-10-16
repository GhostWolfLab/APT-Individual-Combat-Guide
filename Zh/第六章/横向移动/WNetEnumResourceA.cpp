#include <stdio.h>
#include <windows.h>
#include <winnetwk.h>
// 定义一个辅助函数，根据资源类型返回字符串
const char* GetResourceType(DWORD dwType)
{
	switch (dwType)
	{
	case RESOURCETYPE_ANY:
		return "Any";
	case RESOURCETYPE_DISK:
		return "Disk";
	case RESOURCETYPE_PRINT:
		return "Print";
	default:
		return "Unknown";
	}
}
int main()
{
	HMODULE hMpr = LoadLibraryA("mpr.dll");
	typedef DWORD(WINAPI* WNetOpenEnumA_t)(DWORD dwScope,DWORD dwType,DWORD dwUsage,LPNETRESOURCEA lpNetResource,LPHANDLE lphEnum);
	WNetOpenEnumA_t WNetOpenEnumA =(WNetOpenEnumA_t)GetProcAddress(hMpr, "WNetOpenEnumA");
	typedef DWORD(WINAPI* WNetEnumResourceA_t)(HANDLE hEnum,LPDWORD lpcCount,LPVOID lpBuffer,LPDWORD lpBufferSize);
	WNetEnumResourceA_t WNetEnumResourceA =(WNetEnumResourceA_t)GetProcAddress(hMpr,"WNetEnumResourceA");
	typedef DWORD(WINAPI* WNetCloseEnum_t)(HANDLE hEnum);
	WNetCloseEnum_t WNetCloseEnum =(WNetCloseEnum_t)GetProcAddress(hMpr, "WNetCloseEnum");
	HANDLE hEnum;
	DWORD dwResult = WNetOpenEnumA(RESOURCE_CONTEXT,RESOURCETYPE_ANY, 0, NULL, &hEnum);
	if (dwResult == NO_ERROR)
	{
		printf("Enumerating network resources...\n");
		DWORD dwBufferSize = 16384;
		LPVOID lpBuffer = malloc(dwBufferSize);
		if (lpBuffer != NULL)
		{
			BOOL bDone = FALSE;
			while (!bDone)
			{
				DWORD dwCount = -1;
					dwResult = WNetEnumResourceA(hEnum,&dwCount, lpBuffer, &dwBufferSize);
				if (dwResult == NO_ERROR)
				{
					LPNETRESOURCEA lpNetResource= (LPNETRESOURCEA)lpBuffer;
					for (DWORD i = 0; i <
						dwCount; i++)
					{
						printf("Name: %s\n",lpNetResource[i].lpRemoteName);
						printf("Type: %s\n",GetResourceType(lpNetResource[i].dwType));
						printf("\n");
					}
				}
				else if (dwResult ==ERROR_NO_MORE_ITEMS)
				{
					bDone = TRUE;
				}
				else
				{
					printf("Error: %lu\n",dwResult);
					bDone = TRUE;
				}
			}
			free(lpBuffer);
			printf("Enumeration done.\n");
			WNetCloseEnum(hEnum);
			return 0;
		}
	}
	return -1;
}
