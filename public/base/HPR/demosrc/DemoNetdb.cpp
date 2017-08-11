#include "DemoApp.h"

void Test_NetDB()
{
	HPR_INT32 iRet;
	printf("test netdb\n");
#if 1
	printf("get host name \n");
	char szHostName[128] = {0};
	int iHostNameLen = 128;
	HPR_GetHostName(szHostName, iHostNameLen);

	printf("mac addr\n");
	unsigned char szMac[6] = {0};
	HPR_INT32 iMacLen = sizeof(szMac);

	iRet = HPR_GetMacAddr("172.7.153.153", szMac, &iMacLen);
	if (iRet == HPR_OK)
	{
		for (int i=0; i<5; i++)
		{
			fprintf(stderr,"%02x-", szMac[i]);
		}
		fprintf(stderr,"%02x\n", szMac[5]);
	}
	else
	{
		printf("HPR_GetMacAddr HPR_ERROR\n");
	}


	HPR_ADAPTER_INFO* pAdapterInfo = NULL;
	iRet = HPR_GetAdapterInfo(&pAdapterInfo);
	if (iRet == HPR_OK)
	{
		HPR_ADAPTER_INFO* pTmp = pAdapterInfo;
		fprintf(stderr, "HPR_GetAdapterInfo ok\n");
		while (pTmp)
		{
			//char szMac[8] = {0};
			for (int i=0; i<5; i++)
			{
				fprintf(stderr,"%02x-", pTmp->szMacAddr[i]);
			}
			fprintf(stderr,"%02x\n", pTmp->szMacAddr[5]);

			printf("%s-%s-%s-%d\n", pTmp->szAdapterName, \
				pTmp->pIpAddrList->struIpAddrString.szIPAddr,\
				pTmp->pIpAddrList->struIpAddrString.szNetMask,\
				pTmp->nMacAddrLen);
			pTmp = pTmp->pNext;
		}
		HPR_FreeAdapterInfo(pAdapterInfo);
	}
	else
	{
		fprintf(stderr, "HPR_GetAdapterInfo error, %d\n", errno);
	}

	fprintf(stderr, "\n\n");
#endif

	HPR_ADDRESS_INFO *addrinfo = NULL;
	//char *addrs = "www.google.com.us";
	char *addrs = "ipv6.dlut.edu.cn";
	HPR_ADDRESS_INFO hints = {0};
	hints.iFamily = AF_INET6;
	iRet = HPR_GetAddressInfo(addrs, NULL, &hints, &addrinfo);
	if (iRet == HPR_OK)
	{
		HPR_ADDRESS_INFO *tmp = addrinfo;
		char ip[16] = {0};
		HPR_UINT32 dwIP = 0;
		while (tmp)
		{
			if (tmp->iFamily == PF_INET)
			{
				dwIP = tmp->Address.SA.sin4.sin_addr.s_addr;
				sprintf(ip, "%d.%d.%d.%d", (dwIP >> 0) & 255, (dwIP >> 8) & 255, (dwIP >> 16) & 255, (dwIP >> 24) & 255);
				fprintf(stderr, "IPv4: %s\n", ip);
			}
			else if (tmp->iFamily == PF_INET6)
			{
				fprintf(stderr, "IPv6: ");
				for (int i = 0; i < 16; ++ i)
				{
					fprintf(stderr, "%02x", tmp->Address.SA.sin6.sin6_addr.s6_addr[i]);
					if (i % 2 != 0 && i != 15)
					{
						fprintf(stderr, ":");
					}
				}
				fprintf(stderr, "\n");
			}

			tmp = tmp->pNext;
		}

		HPR_FreeAddressInfo(addrinfo);
	}

	//HPR_Sleep(1000);
}
