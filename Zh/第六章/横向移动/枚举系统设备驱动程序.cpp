#include <windows.h>
#include <psapi.h>
#include <stdio.h>
int main()
{
	LPVOID drivers[1024];
	DWORD cbNeeded;
	int cDrivers, i;
		BOOL result = EnumDeviceDrivers(drivers, sizeof(drivers),
			&cbNeeded);
	if (result && cbNeeded < sizeof(drivers))
	{
		char szDriver[1024];
		cDrivers = cbNeeded / sizeof(drivers[0]);
		// 输出设备驱动程序的数量
		printf("There are %d drivers:\n", cDrivers);
		// 遍历每个设备驱动程序
		for (i = 0; i < cDrivers; i++)
		{
				if (GetDeviceDriverBaseNameA(drivers[i], szDriver,
					sizeof(szDriver) / sizeof(szDriver[0])))
				{
					printf("%d: %p - %s\n", i + 1, drivers[i],
						szDriver);
				}
		}
	}
	else
	{
		printf("EnumDeviceDrivers failed; array size needed is % d\n", cbNeeded / sizeof(LPVOID));
			return 1;
	}
	return 0;
}
