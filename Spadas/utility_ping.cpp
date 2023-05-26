
#if defined(SPADAS_ENV_WINDOWS)

#include "spadas.h"
#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>

using namespace spadas;

Bool spadas::system::ping(String ip, UInt timeout)
{
	if (ip.isEmpty()) return FALSE;

	HANDLE iHwnd;
	iHwnd = IcmpCreateFile();

	IPAddr pAddr = (IPAddr)inet_addr(ip.chars().data());
	icmp_echo_reply pData;
	IcmpSendEcho(iHwnd, pAddr, NULL, 0, NULL, (LPVOID)&pData, sizeof(icmp_echo_reply), timeout);

	IcmpCloseHandle(iHwnd);

	return pData.Status == 0;
}

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_NILRT)

#include "spadas.h"
#include <stdio.h>
#include <string.h>
#undef NULL
#define NULL 0

using namespace spadas;

Bool spadas::system::ping(String ip, UInt timeout)
{
	if (ip.isEmpty()) return FALSE;

	Int t = math::max(1, (Int)math::ceil((Double)timeout / 1000));
	String command = (String)"ping -c 1 -W " + t + " " + ip;

	FILE *pf = popen(command.chars().data(), "r");
	if (pf == NULL) return FALSE;

	char result[2048] = {0};
	char buffer[1024] = {0};
	while(fgets(buffer, 1024, pf) != NULL)
	{
		strcat(result, buffer);
		if(strlen(result) > 1024) break;
	}

	pclose(pf);

	Array<StringSpan> comps = String(result).split("\n");
	for (UInt i = 0; i < comps.size(); i++)
	{
		if (!comps[i].search("bytes from").isEmpty()) return TRUE;
	}

	return FALSE;
}

#elif defined(SPADAS_ENV_MACOS)

#include "spadas.h"
#include <stdio.h>
#include <string.h>

using namespace spadas;

Bool spadas::system::ping(String ip, UInt timeout)
{
	if (ip.isEmpty()) return FALSE;

	String command = (String)"ping -c 1 -W " + timeout + " " + ip;

	FILE *pf = popen(command.chars().data(), "r");
	if (pf == NULL) return FALSE;

	char result[2048] = {0};
	char buffer[1024] = {0};
	while(fgets(buffer, 1024, pf) != NULL)
	{
		strcat(result, buffer);
		if(strlen(result) > 1024) break;
	}

	pclose(pf);

	Array<StringSpan> comps = String(result).split("\n");
	for (UInt i = 0; i < comps.size(); i++)
	{
		if (!comps[i].search(" 0.0\% packet loss").isEmpty()) return TRUE;
	}

	return FALSE;
}

#endif