#include <windows.h>
#include <stdio.h>
char* GetClipboardText()
{
	HANDLE hData;
	char* pData;
	char* pText{};
	OpenClipboard(NULL);
		hData = GetClipboardData(CF_TEXT);
	if (hData != NULL)
	{
		pData = (char*)GlobalLock(hData);
		if (pData != NULL)
		{
			pText = (char*)malloc(strlen(pData) + 1);
			if (pText != NULL)
			{
				strcpy(pText, pData);
			}
			GlobalUnlock(hData);
		}
		CloseClipboard();
		return pText;
	}
	else
	{
		return NULL;
	}
}
int main()
{
	char* pText;
	pText = GetClipboardText();
	if (pText != NULL)
	{
		FILE* fp = fopen("cliplog.txt", "a+");
		if (fp != NULL)
		{
			fprintf(fp, "%s\n", pText);
			fclose(fp);
		}
		free(pText);
	}
	return 0;
}
