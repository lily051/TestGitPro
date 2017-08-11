#include "DemoApp.h"

void test_addr_makeaddrstring();
void test_addr_makeaddrin4();

void test_addr_getport();
void test_addr_getipin4();
void test_addr_getaf();
void test_addr_setport();
void test_addr_getaddrinfo();

//void test_addr_makeaddrin6();
//void test_addr_makeaddr();
//void test_addr_getaddrstring();
//void test_addr_getaddrsocket();

void Test_Addr()
{
	printf("******** test_addr_makeaddrstring *************\n");
	test_addr_makeaddrstring();
	printf("******** test_addr_makeaddrin4*************\n");
	test_addr_makeaddrin4();
	printf("******** test_addr_getport *************\n");
	test_addr_getport();
	printf("******** test_addr_getipin4 *************\n");
	test_addr_getipin4();
	printf("******** test_addr_getaf *************\n");
	test_addr_getaf();
	printf("******** test_addr_setport *************\n");
	test_addr_setport();
	printf("******** test_addr_getaddrinfo *************\n");
	test_addr_getaddrinfo();

}

void test_addr_makeaddrstring()
{
	HPR_INT32 ret;
	HPR_ADDR_T struAddr;
	ret = HPR_MakeAddrByString(AF_INET, "172.7.97.97", 10000, &struAddr);
	if (!ret)
	{
		printf("test_addr_makeaddrstring success\n");
		printf("after make, string addr = %s\n", HPR_GetAddrString(&struAddr));
		if (HPR_GetAddrType(&struAddr) == AF_INET)
		{
			printf("test_addr_makeaddrstring getaddrtype = AF_INET\n");
		}
	}
	else
	{
		printf("test_addr_makeaddrstring error, may bugs!!!!!!!!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
	}
}

void test_addr_makeaddrin4()
{
	HPR_INT32 ret;
	HPR_UINT32 nAddr = 0;
	HPR_ADDR_T struAddr;
	nAddr = inet_addr("172.7.97.97");

	ret = HPR_MakeAddr4ByInt(nAddr, 10000, &struAddr);
	if (ret)
	{
		printf("test_addr_makeaddrin4 error, may bugs!!!!!!!!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
	}
	else
	{
		printf("test_addr_makeaddrin4 success\n");
		printf("after make, string addr = %s\n", HPR_GetAddrString(&struAddr));
	}
}

#if 0
void test_addr_makeaddrin6()
{
	HPR_UINT32 nAddr = 0;
	HPR_ADDR_T struAddr = {0};
	nAddr = inet_addr("172.7.97.100");

	ret = HPR_MakeAddr4ByInt(nAddr, 10000, &struAddr);
	if (ret)
	{
		printf("test_addr_makeaddrin4 error, may bugs!!!!!!!!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
	}
	else
	{
		printf("test_addr_makeaddrin4 success\n");
	}
}
#endif

void test_addr_getport()
{
	HPR_ADDR_T struAddr;
	HPR_INT32 iRet = HPR_ERROR;
	HPR_UINT16 nPort = 0;

	iRet = HPR_MakeAddrByString(AF_INET, "172.7.97.97", 10000, &struAddr);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_getport error, HPR_MakeAddrByString error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
		return;
	}

	nPort = HPR_GetAddrPort(&struAddr);
	printf("test_addr_getport, HPR_GetAddrPort port = %d\n", nPort);

	return;
}

void test_addr_getipin4()
{
	HPR_ADDR_T struAddr;
	HPR_INT32 iRet = HPR_ERROR;
	HPR_UINT32 nAddr = 0;

	iRet = HPR_MakeAddrByString(AF_INET, "172.7.97.97", 10000, &struAddr);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_getipin4 error, HPR_MakeAddrByString error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
		return;
	}

	iRet = HPR_GetAddr4Int(&struAddr, &nAddr);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_getipin4 error, HPR_GetAddr4Int error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
	}
	else
	{
		if (nAddr == inet_addr("172.7.97.97"))
		{
			printf("test_addr_getipin4, HPR_GetAddr4Int ok\n");
		}
		else
		{
			printf("test_addr_getipin4, HPR_GetAddr4Int error, may bugs!!!!!!!!!!!!!!\n");
		}
	}

	return;
}

void test_addr_getaf()
{
	HPR_ADDR_T struAddr;
	HPR_INT32 iRet = HPR_ERROR;
	HPR_INT32 iAf = 0;

	iRet = HPR_MakeAddrByString(AF_INET, "172.7.97.97", 10000, &struAddr);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_getaf error, HPR_MakeAddrByString error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
		return;
	}

	iAf = HPR_GetAddrType(&struAddr);
	if (iAf == AF_INET)
	{
		printf("test_addr_getaf, HPR_GetAddrType iAf = AF_INET\n");
	}
	else
	{
		printf("test_addr_getaf error, HPR_GetAddrType error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
	}

	return;
}

void test_addr_setport()
{
	HPR_ADDR_T struAddr;
	HPR_INT32 iRet = HPR_ERROR;
	HPR_UINT16 nPort = 0;

	iRet = HPR_MakeAddrByString(AF_INET, "172.7.97.97", 10000, &struAddr);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_setport error, HPR_MakeAddrByString error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
		return;
	}

	nPort = HPR_GetAddrPort(&struAddr);
	printf("test_addr_setport 1, HPR_GetAddrPort port = %d\n", nPort);

	iRet = HPR_SetAddrPort(&struAddr, 10001);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_setport error, HPR_SetAddrPort error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
		return;
	}

	nPort = HPR_GetAddrPort(&struAddr);
	printf("test_addr_setport 2, HPR_GetAddrPort port = %d\n", nPort);

	return;
}

void test_addr_getaddrinfo()
{
	HPR_ADDR_T struAddr;
	HPR_ADDR_EXP_T struAddrExp = {0};
	HPR_INT32 iRet = HPR_ERROR;

	iRet = HPR_MakeAddrByString(AF_INET, "172.7.97.97", 10000, &struAddr);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_getaddrinfo error, HPR_MakeAddrByString error, may bugs!!!!!!!!!, errno = %d,%s\n", errno, strerror(errno));
		return;
	}

	iRet = HPR_GetAddrInfo(&struAddr, &struAddrExp);
	if (iRet == HPR_ERROR)
	{
		printf("test_addr_getaddrinfo 1, HPR_GetAddrInfo error, may bugs!!!!!! errno = %d,%s\n", errno, strerror(errno));
	}
	else
	{
		printf("test_addr_getaddrinfo ok\n");
		printf("port = %d\n", struAddrExp.nPort);
		if (struAddrExp.iAf == AF_INET)
		{
			printf("test_addr_getaddrinfo af == AF_INET\n");
			if (struAddrExp.ADDR.nAddr4 == inet_addr("172.7.97.97"))
			{
				printf("test_addr_getaddrinfo ipin4 ok\n");
			}
			else
			{
				printf("test_addr_getaddrinfo ipin4 error\n");
			}
		}
		else if (struAddrExp.iAf == AF_INET6)
		{
			printf("test_addr_getaddrinfo af == AF_INET6\n");
		}
		else
		{
			printf("test_addr_getaddrinfo af == error!!!!!!!!!!\n");
		}
	}

	return;
}
