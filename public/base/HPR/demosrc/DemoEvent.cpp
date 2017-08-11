#include "DemoApp.h"

#include <iostream>
using namespace std;

static HPR_HANDLE g_hEvent;
static HPR_HANDLE g_hNetEvent;
static HPR_HANDLE g_hThreadId;
static HPR_SOCK_T g_nSockFd;

#if defined (OS_WINDOWS)
	static LARGE_INTEGER g_iStartTime;
	static LARGE_INTEGER g_iNowTime;
	static LARGE_INTEGER g_iFeq;
#else
	#include <sys/time.h>
	static timespec g_tpStart;
	static timespec g_tpNow;
#endif

void* CALLBACK test_event_svc(void*)
{
	//HPR_Sleep(1000);
	//HPR_SetEvent(g_hEvent);
	//HPR_CloseSocket(g_nSockFd);
	//g_nSockFd = HPR_INVALID_SOCKET;
	return 0;
}

void test_single_event();
void test_multi_event();
void test_max_event();

void Test_Event()
{
	//test_max_event();
	//printf("test single event\n");
	//test_single_event();
	printf("test multi event\n");
	//test_multi_event();
}

void test_single_event()
{
#if defined (OS_WINDOWS)
	QueryPerformanceFrequency( &g_iFeq );
#endif

	HPR_UINT32 nWaitRes = HPR_WAIT_TIMEOUT;
	g_hEvent = HPR_CreateEvent(HPR_FALSE);
	if (!g_hEvent)
	{
		return;
	}

	g_hThreadId = HPR_Thread_Create(test_event_svc, NULL, 0);
	if (g_hThreadId == (HPR_HANDLE)HPR_INVALID_THREAD)
	{
		printf("test_single_event HPR_Thread_Create create thread error\n");
		return ;
	}

#if defined (OS_WINDOWS)
	QueryPerformanceCounter(&g_iStartTime);
#else
	//gettimeofday(&g_tvStart,NULL);
	clock_gettime(CLOCK_MONOTONIC,&g_tpStart);
#endif
	nWaitRes = HPR_WaitForSingleObject(g_hEvent, 3000);
	if (nWaitRes == HPR_WAIT_OBJECT_0)
	{
#if defined (OS_WINDOWS)
		QueryPerformanceCounter(&g_iNowTime);
		printf("time span = %.06f us\n", ((double)(g_iNowTime.QuadPart-g_iStartTime.QuadPart)/(double)g_iFeq.QuadPart)*1000000);
#else
		//gettimeofday(&g_tvNow,NULL);
		//printf("time span1 = %.06f\n",(double)(((g_tvNow.tv_sec*1000000)+g_tvNow.tv_usec) - ((g_tvStart.tv_sec*1000000)+g_tvStart.tv_usec)));
		clock_gettime(CLOCK_MONOTONIC,&g_tpNow);

		HPR_INT32 timedif = (HPR_INT32)(1000000*(g_tpNow.tv_sec-g_tpStart.tv_sec)+(g_tpNow.tv_nsec-g_tpStart.tv_nsec)/1000);
		printf("time span2 = %d us\n",timedif);
#endif
		printf("wait for single object return object 0\n");
		HPR_ResetEvent(g_hEvent);
	}
	else if (nWaitRes == HPR_WAIT_TIMEOUT)
	{
		printf("wait for single object wait timeout\n");
	}
	else
	{
		printf("wait for single object return error\n");
	}

	HPR_Thread_Wait(g_hThreadId);
	g_hThreadId = NULL;

	HPR_CloseEvent(g_hEvent);
	g_hEvent = NULL;

	return;
}

void test_multi_event()
{
	HPR_INT32 iRet;
	g_nSockFd = HPR_INVALID_SOCKET;
	HPR_UINT32 nWaitRes = HPR_WAIT_TIMEOUT;
	HPR_UINT32 nStartTime;

	g_nSockFd = HPR_CreateSocket(AF_INET, SOCK_STREAM, 0);

	g_hEvent = HPR_CreateEvent(HPR_FALSE);
	g_hNetEvent = HPR_CreateEvent(HPR_FALSE);
	if (!g_hEvent || !g_hNetEvent)
	{
		printf("test_multi_event HPR_CreateEvent error, may bugs!!!!!, errno = %d\n", errno);
		HPR_CloseSocket(g_nSockFd);
		return;
	}

	HPR_HANDLE hEvents[2];
	hEvents[0] = g_hEvent;
	hEvents[1] = g_hNetEvent;

	HPR_ADDR_T struAddr;
	HPR_MakeAddrByString(AF_INET,"172.7.97.95",10001,&struAddr);
	printf("test_multi_event connect \n");

	nStartTime = HPR_GetTimeTick();
	iRet = HPR_ConnectWithTimeOut(g_nSockFd, &struAddr, HPR_INFINITE);
	printf("connect default timeout = %d\n", ((HPR_GetTimeTick()-nStartTime)/1000));
	printf("test_multi_event connect fail\n");
	if ( HPR_EventSelect(g_nSockFd,g_hNetEvent,HPR_FD_CLOSE|HPR_FD_READ) != HPR_OK)
	{
		printf("test_multi_event HPR_EnumNetworkEvents error, may bugs!!!!!, errno = %d\n", errno);
		return;
	}

	g_hThreadId = HPR_Thread_Create(test_event_svc, NULL, 0);
	if (g_hThreadId == (HPR_HANDLE)HPR_INVALID_THREAD)
	{
		printf("test_multi_event HPR_Thread_Create create thread error\n");
		return ;
	}

	while (1)
	{
		nWaitRes = HPR_WaitForMultipleObjects(2, hEvents, HPR_FALSE, HPR_INFINITE);
		if (nWaitRes == HPR_WAIT_OBJECT_0)
		{
			printf("test_multi_event wait for multi object return wait (object 0)\n");
			break;
		}
		else if (nWaitRes == (HPR_WAIT_OBJECT_0 + 1))
		{
			printf("test_multi_event wait for multi object return wait (object 0 + 1)\n");
			HPR_NETWORKEVENTS_T struNetworkEvents;
			HPR_EnumNetworkEvents(g_nSockFd, g_hNetEvent,&struNetworkEvents);
			if (struNetworkEvents.lNetworkEvents & HPR_FD_CLOSE)
			{
				printf("close event\n");
				break;
			}
			else if (struNetworkEvents.lNetworkEvents & HPR_FD_READ)
			{
				printf("read event\n");
				char szRcvBuf[256];
				int iRcvLen = 0;
				iRcvLen = HPR_Recv(g_nSockFd,szRcvBuf,256); 
				if (iRcvLen > 0)
				{
					szRcvBuf[iRcvLen] = 0;
					printf("szRcvBuf = %s\n", szRcvBuf);
				}
				HPR_ResetEvent(g_hNetEvent);
			}
			else if (struNetworkEvents.lNetworkEvents & HPR_FD_WRITE)
			{
				printf("write event\n");
				HPR_Send(g_nSockFd,(void*)"3",1);
				HPR_ResetEvent(g_hNetEvent);
			}
		}
		else if (nWaitRes == HPR_WAIT_TIMEOUT)
		{
			printf("test_multi_event wait timeout\n");
			break;
		}
		else
		{
			printf("test_multi_event wait for multi object return wait object error\n");
			break ;
		}
	}

	HPR_ResetEvent(g_hEvent);

	HPR_Thread_Wait(g_hThreadId);
	g_hThreadId = NULL;

	HPR_CloseEvent(g_hEvent);
	HPR_CloseEvent(g_hNetEvent);
	HPR_CloseSocket(g_nSockFd);
	g_hNetEvent = NULL;
	g_hEvent = NULL;
	g_nSockFd = HPR_INVALID_SOCKET;

	return;
}

void test_max_event()
{
	HPR_HANDLE hEvent;
	int i=0;
	for (;;)
	{
		hEvent = HPR_CreateEvent(HPR_FALSE);
		if (hEvent != NULL)
		{
			i++;
			printf("now count is %d\n", i);
		}
	}
}

