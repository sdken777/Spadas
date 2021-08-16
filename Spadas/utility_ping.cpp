
#include "spadas.h"

using namespace spadas;

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>

Bool spadas::system::ping(String ip, UInt timeout)
{
	if (ip.isEmpty()) return FALSE;

	HANDLE iHwnd;
	iHwnd = IcmpCreateFile();

	IPAddr pAddr = (IPAddr)inet_addr((Char*)ip.bytes());
	icmp_echo_reply pData;
	IcmpSendEcho(iHwnd, pAddr, NULL, 0, NULL, (LPVOID)&pData, sizeof(icmp_echo_reply), timeout);

	IcmpCloseHandle(iHwnd);

	return pData.Status == 0;
}

#elif defined(SPADAS_ENV_LINUX)

#include <stdio.h>
#include <string.h>

Bool spadas::system::ping(String ip, UInt timeout)
{
	if (ip.isEmpty()) return FALSE;

	Int t = math::max(1, math::ceil((Double)timeout / 1000));
	String command = (String)"ping -c 1 -W " + t + " " + ip;

	FILE *pf = popen((Char*)command.bytes(), "r");
	if (pf == NULL) return FALSE;

	char result[2048] = {0};
	char buffer[1024] = {0};
	while(fgets(buffer, 1024, pf) != NULL)
	{
		strcat(result, buffer);
		if(strlen(result) > 1024) break;
	}

	pclose(pf);

	Array<String> comps = String(result).split("\n");
	for (UInt i = 0; i < comps.size(); i++)
	{
		if (!comps[i].search("bytes from").isEmpty()) return TRUE;
	}

	return FALSE;
}

#endif