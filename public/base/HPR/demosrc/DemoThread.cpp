#include "DemoApp.h"
#include <iostream>
using namespace std;
//HPR_ThreadDetached_Create

#define MAX_THREAD_COUNT 10000
static HPR_HANDLE s_hThreadId[MAX_THREAD_COUNT];
static HPR_BOOL s_bQuitThead = 0;
//static HPR_HANDLE s_hThreadId;

void* CALLBACK test_thread_svc(void* pParam)
{
	printf("test_thread_svc detached svc\n");

	for (;;)
	{
		if (s_bQuitThead)
		{
			break;
		}

		HPR_Sleep(200);
	}

	printf("test_thread_svc return\n");
	
	return 0;
}

void Test_Thread_Detached()
{
	HPR_BOOL bRet;
	for (int i=0; i<10; i++)
	{
		bRet = HPR_ThreadDetached_Create(test_thread_svc,NULL,0);
		if (!bRet)
		{
			printf("create failed");
		}
		HPR_Sleep(3000);
	}

	HPR_Sleep(5000);

	cin.get();
}

void Test_Thread()
{
	s_bQuitThead = HPR_FALSE;
	s_hThreadId[0] = HPR_Thread_Create(test_thread_svc,NULL,0);
	if (s_hThreadId[0] != HPR_INVALID_THREAD)
	{
#if defined (OS_WINDOWS)
		DWORD dwExitCode = 0;
		if (GetExitCodeThread(s_hThreadId[0], &dwExitCode))
		{
			if (dwExitCode == STILL_ACTIVE)
			{
				fprintf(stderr, "still active\n");
			}
		}
#endif
		s_bQuitThead = HPR_TRUE;

#if defined (OS_WINDOWS)
		HPR_Sleep(1000);
		if (GetExitCodeThread(s_hThreadId[0], &dwExitCode))
		{
			fprintf(stderr, "dwExitCode = %d\n", dwExitCode);
		}
		else 
		{
			fprintf(stderr, "GetExitCodeThread error %d\n", HPR_GetSystemLastError());
		}
#endif
		HPR_Thread_Wait(s_hThreadId[0]);
#if defined (OS_WINDOWS)
		if (GetExitCodeThread(s_hThreadId[0], &dwExitCode))
		{
			fprintf(stderr, "dwExitCode = %d\n", dwExitCode);
		}
		else 
		{
			fprintf(stderr, "GetExitCodeThread error %d\n", HPR_GetSystemLastError());
		}
#endif
		
		s_hThreadId[0] = HPR_INVALID_THREAD;
	}
}

void Test_ThreadMaxThread()
{
	int iThreadCount = 0;

	HPR_ZeroMemory(s_hThreadId, sizeof(HPR_HANDLE)*MAX_THREAD_COUNT);

	for (;;)
	{
		s_hThreadId[iThreadCount] = HPR_Thread_Create(test_thread_svc,NULL,128*1024);
		if (s_hThreadId[iThreadCount] != (HPR_HANDLE)HPR_INVALID_THREAD)
		{
			printf("thread count = %d\n", ++iThreadCount);
		}
		else
		{
			printf("create thread failed = %d\n", ++iThreadCount);
			break;
		}
	}

	s_bQuitThead = 1;

	for (;;)
	{
		if (s_hThreadId[iThreadCount] != (HPR_HANDLE)HPR_INVALID_THREAD)
		{
			HPR_Thread_Wait(s_hThreadId[iThreadCount]);
			s_hThreadId[iThreadCount] = (HPR_HANDLE)HPR_INVALID_THREAD;
		}
		else
		{
			break;
		}
	}
}

