#include <stdio.h>
#include <windows.h>
#include <winnetwk.h>
typedef struct {
	DWORD dwNetType;
	LPCSTR lpNetName;
} NET_TYPE;
// 定义一个数组，包含所有已知的网络类型和名称
NET_TYPE net_types[] = {
 { WNNC_NET_MSNET, "MSNet" },
 { WNNC_NET_LANMAN, "Lanman" },
 { WNNC_NET_NETWARE, "Netware" },
 { WNNC_NET_VINES, "Vines" },
 { WNNC_NET_10NET, "10Net" },
 { WNNC_NET_LOCUS, "Locus" },
 { WNNC_NET_SUN_PC_NFS, "Sun PC NFS" },
 { WNNC_NET_LANSTEP, "Lanstep" },
 { WNNC_NET_9TILES, "9Tiles" },
 { WNNC_NET_LANTASTIC, "Lantastic" },
 { WNNC_NET_AS400, "AS/400" },
 { WNNC_NET_FTP_NFS, "FTP NFS" },
 { WNNC_NET_PATHWORKS, "Pathworks" },
 { WNNC_NET_LIFENET, "Lifenet" },
 { WNNC_NET_POWERLAN, "Powerlan" },
 { WNNC_NET_BWNFS, "BWNFS" },
 { WNNC_NET_COGENT, "Cogent" },
 { WNNC_NET_FARALLON, "Farallon" },
 { WNNC_NET_APPLETALK, "Appletalk" },
 { WNNC_NET_INTERGRAPH, "Intergraph" },
 { WNNC_NET_SYMFONET, "Symfonet" },
 { WNNC_NET_CLEARCASE, "ClearCase" },
 { WNNC_NET_FRONTIER, "Frontier" },
 { WNNC_NET_BMC, "BMC" },
 { WNNC_NET_DCE, "DCE" },
 { WNNC_NET_AVID, "Avid" },
 { WNNC_NET_DOCUSPACE, "DocuSpace" },
 { WNNC_NET_MANGOSOFT, "MangoSoft" },
 { WNNC_NET_SERNET, "Sernet" },
 { WNNC_NET_RIVERFRONT1, "Riverfront1" },
 { WNNC_NET_RIVERFRONT2, "Riverfront2" },
 { WNNC_NET_DECORB, "Decorb" },
 { WNNC_NET_PROTSTOR, "Protstor" },
 { WNNC_NET_FJ_REDIR, "FJ Redir" },
 { WNNC_NET_DISTINCT, "Distinct" },
 { WNNC_NET_TWINS, "Twins" },
 { WNNC_NET_RDR2SAMPLE, "RDR2 Sample" },
 { WNNC_NET_CSC, "Windows Offline Files (Client SideCaching)" },
 { WNNC_NET_3IN1, "3In1 Net"},
 { WNNC_NET_EXTENDNET,"ExtendNet"},
 {WNNC_CRED_MANAGER,"Credential Manager"}
};

#define NET_TYPE_COUNT (sizeof(net_types) / sizeof(NET_TYPE))

int main()
{
	HMODULE hMpr = LoadLibraryA("mpr.dll");
	typedef DWORD(WINAPI* WNetGetProviderNameA_t)(DWORD
		dwNetType,
		LPSTR lpProviderName,
		LPDWORD lpBufferSize);
	WNetGetProviderNameA_t WNetGetProviderNameA =(WNetGetProviderNameA_t)GetProcAddress(hMpr,"WNetGetProviderNameA");
	for (int i = 0; i < NET_TYPE_COUNT; i++)
	{
		DWORD dwNetType = net_types[i].dwNetType;
		LPCSTR lpNetName = net_types[i].lpNetName;
		DWORD dwBufferSize = 256;
		LPSTR lpProviderName = (LPSTR)malloc(dwBufferSize);
		DWORD dwResult = WNetGetProviderNameA(dwNetType,
			lpProviderName, &dwBufferSize);
		if (dwResult == NO_ERROR)
		{
			printf("%s: %s\n", lpNetName, lpProviderName);
		}
		free(lpProviderName);
	}
	return 0;
}
