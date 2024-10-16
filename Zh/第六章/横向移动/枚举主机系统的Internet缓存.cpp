#include <windows.h>
#include <stdio.h>
#include <wininet.h>
#include <ctime>
#pragma comment (lib, "wininet")
#pragma warning(disable : 4996)
typedef HANDLE(WINAPI* PFN_FINDFIRSTURLCACHEENTRYA)(LPCSTR, LPINTERNET_CACHE_ENTRY_INFOA, LPDWORD);
typedef BOOL(WINAPI* PFN_FINDNEXTURLCACHEENTRYA)(HANDLE, LPINTERNET_CACHE_ENTRY_INFOA, LPDWORD);
int main()
{
	LPCSTR lpszUrlSearchPattern = NULL;
	LPINTERNET_CACHE_ENTRY_INFOA lpCacheEntryInfo = NULL;
	DWORD dwSize = 0;
	HANDLE hEnumHandle = NULL;
	BOOL bResult = FALSE;
	PFN_FINDFIRSTURLCACHEENTRYA pfnFindFirstUrlCacheEntryA;
	PFN_FINDNEXTURLCACHEENTRYA pfnFindNextUrlCacheEntryA;
	HMODULE hModule;
	hModule = LoadLibraryA("wininet.dll");
	pfnFindFirstUrlCacheEntryA =(PFN_FINDFIRSTURLCACHEENTRYA)GetProcAddress(hModule, "FindFirstUrlCacheEntryA");
	pfnFindNextUrlCacheEntryA =(PFN_FINDNEXTURLCACHEENTRYA)GetProcAddress(hModule, "FindNextUrlCacheEntryA");
	hEnumHandle = pfnFindFirstUrlCacheEntryA(lpszUrlSearchPattern, NULL, &dwSize);
	if (hEnumHandle == NULL && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		lpCacheEntryInfo = (LPINTERNET_CACHE_ENTRY_INFOA)malloc(dwSize);
		if (lpCacheEntryInfo != NULL)
		{
			hEnumHandle = pfnFindFirstUrlCacheEntryA(lpszUrlSearchPattern, lpCacheEntryInfo, &dwSize);
			if (hEnumHandle != NULL)
			{
				printf("The cache entry URL is: %s\n", lpCacheEntryInfo->lpszSourceUrlName);
				printf("The cache entry file name is: %s\n", lpCacheEntryInfo->lpszLocalFileName);
				printf("The cache entry size is: %lu\n", lpCacheEntryInfo->dwSizeLow);
				printf("The cache entry last modified time is: % s\n", std::ctime((time_t*)&lpCacheEntryInfo-> LastModifiedTime)); free(lpCacheEntryInfo);
			}
		}
	}
	do
	{
		bResult = pfnFindNextUrlCacheEntryA(hEnumHandle, NULL, &dwSize);
		if (bResult == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			lpCacheEntryInfo = (LPINTERNET_CACHE_ENTRY_INFOA)malloc(dwSize);
			if (lpCacheEntryInfo != NULL)
			{
				bResult = pfnFindNextUrlCacheEntryA(hEnumHandle, lpCacheEntryInfo, &dwSize);
				if (bResult == TRUE)
				{
					printf("The cache entry URL is: %s\n", lpCacheEntryInfo->lpszSourceUrlName);
					printf("The cache entry file name is: % s\n", lpCacheEntryInfo->lpszLocalFileName);
					printf("The cache entry size is: %lu\n", lpCacheEntryInfo->dwSizeLow);
					printf("The cache entry last modified time is : % s\n", std::ctime((time_t*)&lpCacheEntryInfo-> LastModifiedTime));
					free(lpCacheEntryInfo);
				}
			}
		}
	} while (bResult == TRUE && GetLastError() != ERROR_NO_MORE_ITEMS);
	FindCloseUrlCache(hEnumHandle);
	return 0;
}
