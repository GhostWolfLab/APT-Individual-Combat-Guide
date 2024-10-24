#include <windows.h>
#include <stdio.h>
#define THRESHOLD 1000
int main() {
	ULONGLONG startTime, endTime, elapsedTime;
	ULONGLONG(WINAPI * getTickCount64)() = (ULONGLONG(WINAPI*)())GetProcAddress(GetModuleHandle(TEXT("kernel32")), "GetTickCount64");
	startTime = getTickCount64();
	printf("Executing malicious code...\n");

	// 调用GetTickCount64函数，获取程序结束时的时间
	endTime = getTickCount64();

	// 计算程序执行的时间间隔
	elapsedTime = endTime - startTime;

	// 判断时间间隔是否大于阈值，如果是，则认为是被调试或分析，并退出程序
		if (elapsedTime > THRESHOLD) {
			printf("Debugger or analyzer detected!\n");
			exit(0);
		}
	printf("No debugger or analyzer detected!\n");

	// 这里插入恶意代码，比如显示一个弹窗消息
	MessageBoxA(NULL, "You have been infected by a malware!","Malware Alert", MB_OK | MB_ICONWARNING);
	return 0;
}
