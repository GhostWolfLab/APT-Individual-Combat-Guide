#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
// 定义要检查的MAC地址
#define MAC_COUNT 2
BYTE MAC_ADDRESSES[MAC_COUNT][6] = { {0x00, 0x0C, 0x29}, // VMware
									 {0x08, 0x00, 0x27} }; // VirtualBox
// 检查MAC地址是否特定
BOOL CheckMacAddress()
{
	ULONG ulOutBufLen = 15 * 1024;
	PIP_ADAPTER_INFO pAdapterInfo = (PIP_ADAPTER_INFO)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
	DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	if (dwRetVal != ERROR_SUCCESS)
	{
		printf("GetAdaptersInfo failed: %d\n", dwRetVal);
		return FALSE;
	}
	PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
	while (pAdapter)
	{
		BYTE MacAddress[6];
		memcpy(MacAddress, pAdapter->Address, sizeof(MacAddress));
		for (int i = 0; i < MAC_COUNT; i++)
		{
			// 如果当前适配器的MAC地址与要检查的MAC地址匹配，则返回真
			if (memcmp(MacAddress, MAC_ADDRESSES[i], sizeof(MacAddress)) == 0)
			{
				return TRUE;
			}
		}
		// 获取下一个适配器信息
		pAdapter = pAdapter->Next;
	}
	// 没有找到特定的MAC地址，返回假
	return FALSE;
}
int main()
{
	BOOL bFound = CheckMacAddress();
	if (bFound)
	{
		printf("Found specific MAC address, exit\n");
		return 1;
	}
	printf("No specific MAC address found, continue\n");
	// 这里插入恶意代码，比如显示一个弹窗消息
	MessageBoxA(NULL, "You have been infected by a malware!","Malware Alert", MB_OK | MB_ICONWARNING);
	return 0;
}
