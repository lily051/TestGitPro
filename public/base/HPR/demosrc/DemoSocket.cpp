#include "DemoApp.h"

void test_socket_udp();
void test_socket_tcp();
void test_socket_mcast();

void Test_Socket()
{
	//HPR_InitNetwork();

	//printf("*************test_socket_udp********************\n");
	//test_socket_udp();

	//printf("*************test_socket_tcp********************\n");
	test_socket_tcp();

	//printf("*************test_socket_mcast********************\n");
	//test_socket_mcast();

	//HPR_FiniNetwork();
}

void test_socket_udp()
{
	HPR_SOCK_T iSockFd;
	HPR_INT32 ret;
	HPR_BOOL bNonBlock = HPR_TRUE;
	HPR_INT32 iTTL = 16;
	HPR_INT32 iTOS = 1;
	HPR_INT32 iSndBufSize = 50*1024;
	HPR_INT32 iRcvBufSize = 50*1024;
	HPR_INT32 iTimeOut = 3000;
	HPR_ADDR_T struToAddr;
	HPR_ADDR_T struLocalAddr;
	char szRcvBuf[1024] = {0};
	int iRcvBufLen = 1024;
	iSockFd = HPR_CreateSocket(AF_INET, SOCK_DGRAM, 0);
	if (iSockFd == HPR_INVALID_SOCKET)
	{
		printf("******test_socket_udp create socket error, may bugs!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	printf("------------------test non block\n");
	ret = HPR_SetNonBlock(iSockFd, bNonBlock);
	if (ret == HPR_ERROR)
	{
		printf("test_socket_udp setnonblock error, may bugs!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_SOCKET_UDP_END;
	}
	
	printf("---------------------test ttl\n");
	ret = HPR_SetTTL(iSockFd,iTTL);
	if (ret)
	{
		printf("test_socket_udp set ttl error, may bugs!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_SOCKET_UDP_END;
	}
	else
	{
		ret = HPR_GetTTL(iSockFd,&iTTL);
		if (ret)
		{
			printf("test_socket_udp get ttl error, may bugs!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
			goto TEST_SOCKET_UDP_END;
		}

		printf("test_socket_udp get ttl success, iTTL = %d\n", iTTL);
	}

	printf("---------------test tos\n");
	iTOS = 20;
	ret = HPR_SetTOS(iSockFd, iTOS);
	if (ret)
	{
		printf("test_socket_udp set tos error, may bugs!!!!!!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
		goto TEST_SOCKET_UDP_END;
	}
	else
	{
		ret = HPR_GetTOS(iSockFd,&iTOS);
		if (ret)
		{
			printf("test_socket_udp get tos error, may bugs!!!!!!!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
			goto TEST_SOCKET_UDP_END;
		}
		printf("test_socket_udp get tos success, iTOS = %d\n", iTOS);
	}

	printf("----------------test reuse addr\n");
	ret = HPR_SetReuseAddr(iSockFd,HPR_TRUE);
	if (ret)
	{
		printf("test_socket_udp set reuse addr error, may bugs!!!!!!!!!!!!, errno = %d, %s\n",errno, strerror(errno));
		goto TEST_SOCKET_UDP_END;
	}

	printf("-------------------test settimeout\n");
	ret = HPR_SetTimeOut(iSockFd, iTimeOut, iTimeOut);
	if (ret)
	{
		printf("test_socket_udp set timeout error, may bugs!!!!!!!!!!!,errno = %d, %s\n",errno ,strerror(errno));
	}

	printf("---------------test set buffer size\n");
	ret = HPR_SetBuffSize(iSockFd,iSndBufSize,iRcvBufSize);
	if (ret)
	{
		printf("test_socket_udp set bufsize error, may bugs!!!!!!!!!!!!!!,errno = %d, %s\n",errno, strerror(errno));
		goto TEST_SOCKET_UDP_END;
	}
	
	iSndBufSize = iRcvBufSize = 0;
	ret = HPR_GetBuffSize(iSockFd, &iSndBufSize,&iRcvBufSize);
	if (ret)
	{
		printf("test_socket_udp get bufsize error, mqy bugs!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
		goto TEST_SOCKET_UDP_END;
	}
	printf("test_socket_udp iSndBufSize = %dk, iRcvBufSize = %dk\n", iSndBufSize/1024, iRcvBufSize/1024);

	printf("--------------test sendto\n");
#if defined (OS_WINDOWS)
	_snprintf(szRcvBuf,1024,"%d-%s", 1024,"hello test_socket_udp!");
#else
	snprintf(szRcvBuf,1024,"%d-%s", 1024,"hello test_socket_udp!");
#endif
	printf("format string\n");
	HPR_MakeAddrByString(AF_INET,"172.7.97.97", 10001, &struToAddr);
	HPR_MakeAddr4ByInt(0, 10000, &struLocalAddr);
	ret = HPR_Bind(iSockFd,&struLocalAddr);
	if (ret)
	{
		printf("test_socket_udp bind error, may bugs!!!!!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
	}

	printf("make addr\n");
	ret = HPR_SendTo(iSockFd,(HPR_VOIDPTR)szRcvBuf,(HPR_INT32)strlen(szRcvBuf),&struToAddr);
	printf("send to\n");
	if (ret > 0)
	{
		memset(&struToAddr,0,sizeof(struToAddr));
		iRcvBufLen = HPR_RecvFrom(iSockFd,(HPR_VOIDPTR)szRcvBuf,1024,&struToAddr);	
		if (iRcvBufLen > 0)
		{
			szRcvBuf[iRcvBufLen] = 0;
			printf("test_socket_udp recv buf is %s\n", szRcvBuf);
			printf("remote addr is %s,%d\n", HPR_GetAddrString(&struToAddr),ntohs(struToAddr.SA.sin4.sin_port));
		}
		else
		{
			printf("test_socket_udp recvform error, may bugs!!!!!!!!!!!!,errno = %d, %s\n",errno, strerror(errno));
		}
	}
	else
	{
			printf("test_socket_udp sento error, may bugs!!!!!!!!!!!!,errno = %d, %s\n",errno, strerror(errno));
	}

TEST_SOCKET_UDP_END:

	HPR_CloseSocket(iSockFd);
}

void test_socket_tcp()
{
	int ret;
	HPR_SOCK_T iSockFd = 0;
	HPR_ADDR_T struLocalAddr;
	HPR_ADDR_T struRemoteAddr;

	HPR_MakeAddr4ByInt(0,10000,&struLocalAddr);
	HPR_MakeAddrByString(AF_INET,"10.64.51.238",7302,&struRemoteAddr);

	iSockFd = HPR_CreateSocket(AF_INET,SOCK_STREAM,0);
	if (iSockFd == HPR_INVALID_SOCKET)
	{
		printf("test_socket_tcp CreateSocket error, may bugs!!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

#if 0
	ret = HPR_Bind(iSockFd,&struLocalAddr);
	if (ret)
	{
		printf("test_socket_tcp bind error, may bugs!!!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
		return ;
	}

	ret = HPR_Listen(iSockFd, 5);
	if (ret)
	{
		printf("test_socket_tcp listen error, may bugs!!!!!!!!!!,errno = %d, %s\n", errno ,strerror(errno));
		return ;
	}

	printf("accept\n");
	
	HPR_SOCK_T iSock = HPR_Accept(iSockFd,NULL);
	if (iSock > 0)
	{
		char* pRcvBuf = new char[100*1024*1024 + 4];
		int iRecvLen = 256;

		printf("accept ok, before recvn\n");

		iRecvLen = HPR_Recvn(iSock,pRcvBuf,100*1024*1024,8000);
		if (iRecvLen > 0)
		{
			pRcvBuf[100*1024*1024] = 0;
			printf("test_socket_tcp recv success, %d\n", iRecvLen);
			HPR_Send(iSock, pRcvBuf,100);
		}
		else
		{
			printf("test_socket_tcp recv error, may bugs!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
		}

		cin.get();
	}
	else
	{
		printf("test_socket_tcp accept error, may bug!!!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
	}

#endif	
#if 1
	ret = HPR_SetNoDelay(iSockFd,HPR_TRUE);
	if (ret)
	{
		printf("test_socket_tcp setnodelay error, may bugs!!!!!!!,errno = %d, %s\n",errno, strerror(errno));
	}
	printf("test_socket_tcp set no nagle algorithm ok\n");

	ret = HPR_LingerOff(iSockFd);
	if (ret)
	{
		printf("test_socket_tcp set linger on error, may bugs!!!!!!!,errno = %d, %s\n",errno, strerror(errno));
	}
	printf("test_socket_tcp set linger on ok\n");

	ret = HPR_ConnectWithTimeOut(iSockFd, &struRemoteAddr, 5000);	
	if (ret)
	{
		printf("test_socket_tcp connect with timeout error, may bugs!!!!!!!!!!!, errno = %d, %s\n", HPR_GetSystemLastError(),strerror(errno));
		return;
	}

	char szSndBuf[256] = {0};
	char szRcvBuf[400*1024] = {0};
	int iRecvLen = 256;
	//sprintf(szSndBuf, "%s", "hello, test_socket_tcp");
	//ret = HPR_Send(iSockFd,szSndBuf,(HPR_INT32)strlen(szSndBuf));
	//if (ret > 0)
	//{
		//printf("test_socket_tcp test_socket_tcp send success\n");
		iRecvLen = HPR_Recvn(iSockFd,szRcvBuf,400*1024,5000);
		if (iRecvLen > 0)
		{
			szRcvBuf[iRecvLen] = 0;
			printf("test_socket_tcp HPR_Recv success, iRecvLen = %d,\"%s\"\n",iRecvLen,szRcvBuf);
		}
		else
		{
			printf("test_socket_tcp HPR_Recv error, may bugs!!!!!!!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
		}
	//}
	//else
	//{
	//	printf("test_socket_tcp send error, may bugs!!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
	//	return;
	//}
#endif
	HPR_CloseSocket(iSockFd);
}




void test_socket_mcast()
{
	HPR_SOCK_T iSockFd = HPR_INVALID_SOCKET;
	iSockFd = HPR_CreateSocket(AF_INET, SOCK_DGRAM, 0);

	HPR_ADDR_T struMCastAddr;
	HPR_ADDR_T struLocalAddr;
	HPR_ADDR_T struRemoteAddr;

	HPR_INT32 iRet = HPR_ERROR;

	fprintf(stderr, "enter test_socket_mcast()\n");

	HPR_MakeAddrByString(AF_INET, "224.0.1.99", 10000, &struMCastAddr);
	HPR_MakeAddrByString(AF_INET ,"172.7.163.200", 10000, &struLocalAddr);

	iRet = HPR_Bind(iSockFd, &struLocalAddr);
	if (iRet == HPR_ERROR)
	{
		fprintf(stderr, "bind error, errno = %d\n", HPR_GetSystemLastError());
		HPR_CloseSocket(iSockFd);
		iSockFd = HPR_INVALID_SOCKET;
		return;
	}

	iRet = HPR_JoinMultiCastGroup(iSockFd, &struLocalAddr, &struMCastAddr);
	if (iRet == HPR_ERROR)
	{
		fprintf(stderr, "HPR_JoinMultiCastGroup error, errno = %d\n", HPR_GetSystemLastError());
		HPR_CloseSocket(iSockFd);
		iSockFd = HPR_INVALID_SOCKET;
		return;
	}

	fprintf(stderr, "ready to receive data\n");

	char szRecvBuf[50*1024];
	HPR_INT32 iRecvLen = 0;
	for (int i=0; i<100; i++)
	{
		iRecvLen = HPR_RecvFrom(iSockFd, szRecvBuf, sizeof(szRecvBuf), &struRemoteAddr);
		if (iRecvLen > 0)
		{
			fprintf(stderr, "HPR_RecvFrom From %s:%d len = %d\n", HPR_GetAddrString(&struRemoteAddr), HPR_GetAddrPort(&struRemoteAddr), iRecvLen);
		}
		else
		{
			fprintf(stderr, "HPR_RecvFrom len = %d, errno = %d\n", iRecvLen, HPR_GetSystemLastError());
		}
	}

	cin.get();
	cin.get();

	fprintf(stderr, "break for()\n");

	return;
}

