#include "DemoApp.h"

#if 0
#define MAX_SEND_COUNT 10000
#define MAX_SEND_COUNT 10000 
struct ASYNC_USRDATA
{
	HPR_INT32 iType;
};


static char szRecvBuf[1500] = {0};
static ASYNC_USRDATA s_struAsyncUsrDataSend = {0};
static ASYNC_USRDATA s_struAsyncUsrDataRecv = {0};
static int s_iTotalSendLen = 0;

static HPR_VOID Async_CBF(HPR_ULONG ErrorCode, HPR_ULONG NumberOfBytes, HPR_VOIDPTR SelfDefineData);
static void* CALLBACK Async_Send_Svc(void* pParam);
static void* CALLBACK Async_Create_Svc(void* pParam);


static HPR_HANDLE s_hIO = NULL;
static HPR_HANDLE s_hAioQ = NULL;

struct ASYNC_T
{
	HPR_SOCK_T iSockFd;
	HPR_BOOL bQuit;
};
static ASYNC_T s_struSockFds[MAX_SEND_COUNT];

void Test_AsyncIO()
{
	HPR_INT32 iRet = -1;
	//HPR_HANDLE hAioQ;
	//HPR_HANDLE hIO;
	HPR_ADDR_T struAddr = {0};
	char szSendBuf[1460] = {0};
	s_struAsyncUsrDataSend.iType = 0;
	s_struAsyncUsrDataRecv.iType = 1;
	
 	struct IO_BUF
	{
		int iError;
	};

	s_hAioQ = HPR_AsyncIO_CreateQueue();
	if (s_hAioQ == HPR_INVALID_ASYNCIO)
	{
		printf("HPR_AsyncIO_CreateQueue Error, errno = %d\n", HPR_GetSystemLastError());
		return;
	}

	HPR_ThreadDetached_Create(Async_Create_Svc, NULL, 0);

	cin.get();

	//HPR_AsyncIO_PostQueuedCompleteStatus(hAioQ, hIO, 0, 5, &ioBuf);


	HPR_AsyncIO_DestroyQueue(s_hAioQ);

	printf("HPR_CloseSocket\n");
}

static bool g_bSend1 = true;
static bool g_bSend2 = true;
static HPR_TIME_T s_StartTime = HPR_GetTimeTick();
static HPR_TIME_T s_StartRecvTime = HPR_GetTimeTick();

static HPR_VOID Async_CBF(HPR_ULONG ErrorCode, HPR_ULONG NumberOfBytes, HPR_VOIDPTR SelfDefineData)
{
	ASYNC_USRDATA* pUsrData = (ASYNC_USRDATA*)(SelfDefineData);
	if ((ErrorCode == 0) && (pUsrData->iType == 0))
	{
		s_iTotalSendLen += NumberOfBytes;
		if ((HPR_GetTimeTick() - s_StartTime) > 1000)
		{
			s_StartTime = HPR_GetTimeTick();
			HPR_OutputDebug("send data ok %d\n", NumberOfBytes);
		}	
	}
	else if ((ErrorCode == 0) && (pUsrData->iType == 1))
	{
		if ((HPR_GetTimeTick() - s_StartRecvTime) > 1000)
		{
			s_StartRecvTime = HPR_GetTimeTick();
			HPR_OutputDebug("recv data ok %d\n", NumberOfBytes);
		}
	}
	else if (ErrorCode != 0)
	{
		if (ErrorCode == 1234)
		{
			//g_bSend2 = false;
		}
		
		HPR_OutputDebug("ErrnoCode = %d\n", ErrorCode);
	}

	return;
}




static void* CALLBACK Async_Send_Svc(void* pParam)
{
	HPR_INT32 iRet = HPR_ERROR;
	ASYNC_T* pAsync = (ASYNC_T*)(pParam);
	HPR_SOCK_T iSockFd = pAsync->iSockFd;
	HPR_ADDR_T struAddr = {0};
	char szSendBuf[1460] = {0};

	if ((HPR_SOCK_T)iSockFd == HPR_INVALID_SOCKET)
	{
		printf("HPR_CreateSocket Error, errno = %d\n", HPR_GetSystemLastError());
		return NULL;
	}

	//HPR_MakeAddrByString(AF_INET, "172.7.153.153", 10000, &struAddr);

	//iRet = HPR_ConnectWithTimeOut((HPR_SOCK_T)iSockFd,  &struAddr, 8000);
	//if (iRet == HPR_ERROR)
	//{
	//	printf("HPR_ConnectWithTimeOut Error, errno = %d\n", HPR_GetSystemLastError());
	//	HPR_CloseSocket((HPR_SOCK_T)(iSockFd));
	//	return NULL;
	//}

	//printf("socket = %d, %x\n", iSockFd, pParam);

	//if (HPR_AsyncIO_BindIOHandleToQueue((HPR_HANDLE)iSockFd, s_hAioQ) == HPR_ERROR)
	//{
	//	printf("HPR_AsyncIO_BindIOHandleToQueue Error, errno = %d\n", HPR_GetSystemLastError());
	//	HPR_CloseSocket((HPR_SOCK_T)iSockFd);
	//	iSockFd = (HPR_INVALID_SOCKET);
	//	return NULL;
	//}

	//HPR_AsyncIO_BindCallBackToIOHandle((HPR_HANDLE)iSockFd, Async_CBF);

	//printf("Ready To Send\n");

	HPR_TIME_T nStartTime;
	HPR_TIME_T nNowTime;
	HPR_ADDR_T struDstAddr1 = {0};
	HPR_ADDR_T struDstAddr2 = {0};
	HPR_MakeAddrByString(AF_INET, "172.7.153.153", 10000, &struDstAddr1);
	HPR_MakeAddrByString(AF_INET, "172.7.99.3", 10000, &struDstAddr2);

	nStartTime = nNowTime = HPR_GetTimeTick();

	int i = 0;
	i ++;

	bool bSend1 = true;
	bool bSend2 = true;

	for (;;)
	{
		//if (pAsync->bQuit)
		//{
		//	break;
		//}


		nNowTime = HPR_GetTimeTick();

		if (nNowTime - nStartTime > 10000*1000)
		{
			//HPR_CloseSocket(pAsync->iSockFd);
			//pAsync->iSockFd = HPR_INVALID_SOCKET;
			//WSACloseEvent((HANDLE)pAsync->iSockFd);
			//HPR_CloseSocket(pAsync->iSockFd);
			//break;
		}
		
		//if (send(iSockFd, szSendBuf, sizeof(szSendBuf), 0) <= 0)
		if (g_bSend1)
		{
			s_struAsyncUsrDataSend.iType = 0;
			if (HPR_AsyncIO_Send((HPR_HANDLE)iSockFd, szSendBuf, sizeof(szSendBuf), &s_struAsyncUsrDataSend) == HPR_ERROR)
			{
				printf("HPR_AsyncIO_SendTo Error\n, errno = %d\n", HPR_GetSystemLastError());
				break;
			}

			//HPR_Sleep(1000);

			s_struAsyncUsrDataRecv.iType = 1;
			if (HPR_AsyncIO_Recv((HPR_HANDLE)iSockFd, szRecvBuf, sizeof(szRecvBuf),&s_struAsyncUsrDataRecv) == HPR_ERROR)
			{
				int iii = WSAGetLastError();
				HPR_OutputDebug("iii2 = %d\n", iii);
			}
			closesocket(iSockFd);
			HPR_AsyncIO_UnBindIOHandle((HPR_HANDLE)iSockFd, s_hAioQ);
			
		}
		
		//if (g_bSend2)
		//{
		//	s_struAsyncUsrDataSend.iType = 0;
		//	if (HPR_AsyncIO_SendTo((HPR_HANDLE)iSockFd, szSendBuf, sizeof(szSendBuf)/2, &s_struAsyncUsrDataSend, &struDstAddr2) == HPR_ERROR)
		//	{
		//		printf("HPR_AsyncIO_SendTo Error\n, errno = %d\n", HPR_GetSystemLastError());
		//		//g_bSend2 = false;
		//		continue;
		//	}
		//	s_struAsyncUsrDataRecv.iType = 1;
		//	if (HPR_ERROR == HPR_AsyncIO_RecvFrom((HPR_HANDLE)iSockFd, szRecvBuf, sizeof(szRecvBuf),&s_struAsyncUsrDataRecv,NULL))
		//	{
		//		int ii = WSAGetLastError();
		//		HPR_OutputDebug("ii1 = %d\n", ii);
		//	}
		//}
		//
		HPR_Sleep(200);
	}

	//HPR_AsyncIO_UnBindIOHandle((HPR_HANDLE)iSockFd, s_hAioQ);
	//printf("HPR_AsyncIO_DestroyQueue ##################3\n");


	return NULL;
}

static void* CALLBACK Async_Create_Svc(void* pParam)
{
	int i = 0;
	HPR_SOCK_T iSockFd;
	int iRet = HPR_ERROR;
	for (;;)
	{
		i %= MAX_SEND_COUNT;
		s_struSockFds[i].bQuit = HPR_FALSE;
		s_struSockFds[i].iSockFd = HPR_CreateSocket(AF_INET, SOCK_STREAM, 0);
		//s_struSockFds[i].iSockFd = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
		printf("socket create = %d\n", s_struSockFds[i].iSockFd);

		iSockFd = s_struSockFds[i].iSockFd;

		HPR_HANDLE hThreadId;
		HPR_ADDR_T struAddr = {0};
		struct sockaddr_in struPeer = {0};
		struPeer.sin_family = AF_INET;
		struPeer.sin_addr.s_addr = inet_addr("172.7.153.153");
		struPeer.sin_port = htons(10000);
		HPR_MakeAddrByString(AF_INET, "172.7.153.153", 10000, &struAddr);

		iRet = WSAConnect(iSockFd, (sockaddr*)&struPeer, sizeof(struPeer), NULL, NULL, NULL, NULL);
		//iRet = HPR_ConnectWithTimeOut((HPR_SOCK_T)iSockFd,  &struAddr, 8000);
		iRet = HPR_OK;
		if (iRet == HPR_ERROR)
		{
			printf("HPR_ConnectWithTimeOut Error, errno = %d\n", HPR_GetSystemLastError());
			HPR_CloseSocket((HPR_SOCK_T)(iSockFd));
			return NULL;
		}

		if (HPR_AsyncIO_BindIOHandleToQueue((HPR_HANDLE)iSockFd, s_hAioQ) == HPR_ERROR)
		{
			printf("HPR_AsyncIO_BindIOHandleToQueue Error, errno = %d\n", HPR_GetSystemLastError());
			HPR_CloseSocket((HPR_SOCK_T)iSockFd);
			iSockFd = (HPR_INVALID_SOCKET);
			return NULL;
		}

		HPR_AsyncIO_BindCallBackToIOHandle((HPR_HANDLE)iSockFd, Async_CBF);

		printf("Ready To Send\n");

		hThreadId = HPR_Thread_Create(Async_Send_Svc, &s_struSockFds[i], 0);
		if (hThreadId == (HPR_HANDLE)HPR_INVALID_THREAD)
		{
			printf("HPR_Thread_Create error\n");
		}

		HPR_Thread_Wait(hThreadId);

		//HPR_CloseSocket(iSockFd);

		//HPR_AsyncIO_UnBindIOHandle((HPR_HANDLE)iSockFd, s_hAioQ);

		//s_struSockFds[i].bQuit = HPR_TRUE;
		//HPR_CloseSocket((HPR_SOCK_T)(s_struSockFds[i].iSockFd));
		//s_struSockFds[i].iSockFd = HPR_INVALID_SOCKET;
		//cin.get();

		HPR_Sleep(1000);

		i++;
	}

	return NULL;
}

#endif
