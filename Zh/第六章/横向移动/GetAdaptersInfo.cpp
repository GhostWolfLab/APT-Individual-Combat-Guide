#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>
typedef DWORD(WINAPI* PFN_GETADAPTERSINFO)(
	PIP_ADAPTER_INFO pAdapterInfo,
	PULONG pOutBufLen
	);
int main()
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
		ULONG ulOutBufLen = 0;
		DWORD dwRetVal = 0;
	PFN_GETADAPTERSINFO pfnGetAdaptersInfo;
		HMODULE hModule;
	hModule = LoadLibraryA("iphlpapi.dll");
		pfnGetAdaptersInfo =(PFN_GETADAPTERSINFO)GetProcAddress(hModule,"GetAdaptersInfo");
		dwRetVal = pfnGetAdaptersInfo(NULL, &ulOutBufLen);
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulOutBufLen);
		dwRetVal = pfnGetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	if (dwRetVal == NO_ERROR)
	{
		printf("Successfully got the adapters information\n");
			PIP_ADAPTER_INFO pTemp = pAdapterInfo;
		while (pTemp)
		{
			printf("The adapter name is: %s\n", pTemp->AdapterName);
			printf("The adapter description is: %s\n", pTemp -> Description);
			printf("The adapter type is: %u\n", pTemp->Type);
			printf("The adapter IP address is: %s\n", pTemp -> IpAddressList.IpAddress.String);
			printf("The adapter MAC address is: %02X-%02X-%02X-% 02X - % 02X - % 02X\n", pTemp->Address[0], pTemp->Address[1],
				pTemp->Address[2], pTemp->Address[3], pTemp->Address[4],
				pTemp->Address[5]);
			pTemp = pTemp->Next;
		}
	}
	else
	{
		printf("Failed to get the adapters information\n");
		return 5;
	}
	free(pAdapterInfo);
	return 0;
}
