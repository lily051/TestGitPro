#include "DemoApp.h"
#include <iostream>
using namespace std;


static int g_sem_i = 0;
static HPR_SEM_T g_hSem; 
static HPR_HANDLE g_hThread;
static void* CALLBACK test_sem_svc(void* pParam);
void test_sem_0();
void test_sem_1();

void Test_Sema()
{
	HPR_TIME_T tNow;
	HPR_INT32 iRet = -1;
	printf("enter Test_Sema ****************************\n");

	HPR_SemCreate(&g_hSem, 0);
	tNow = HPR_GetTimeTick();
	//printf("tNow = %llu, iRet = %d\n", tNow, iRet);
	cout << "tNow = " << (hpr_int32)tNow;
	cout << ", iRet = " << iRet;
	iRet = HPR_SemTimedWait(&g_hSem, 130*1000);
	tNow = HPR_GetTimeTick();
	//printf("tNow = %llu, %d\n", tNow, iRet);
	cout << "tNow = " << (hpr_int32)tNow;
	cout << ", iRet = " << iRet;
	HPR_SemDestroy(&g_hSem);

	printf("test_sem_0 ******************************\n");
	//test_sem_0();
	printf("test_sem_1 ******************************\n");
	//test_sem_1();

	return;
}

void* CALLBACK test_sem_svc(void* pParam)
{
	printf("enter test_sema_svc \n");
	HPR_Sleep(1000);

	printf("test_sem_svc sem post\n");
	g_sem_i++;
	printf("svc g_sem_i = %d\n", g_sem_i);
	if (HPR_SemPost(&g_hSem))
	{
		printf("test_sem_svc sem post error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}

	return 0;
}

void test_sem_0()
{
	HPR_INT32 ret;
	
	ret = HPR_SemCreate(&g_hSem,0);
	if (ret != 0)
	{
		printf("test_sem_0 create semaphore error, may bugs!!!, %d, %s\n", errno, strerror(errno));
		return;
	}

	g_hThread = HPR_Thread_Create(test_sem_svc,0,0);
	if (g_hThread == NULL)
	{
		HPR_SemDestroy(&g_hSem);
		printf("test_sem_0 create thread error, may bugs!!!, %d, %s\n", errno, strerror(errno));
		return;
	}

	printf("test_sem_0 ready to wait sem\n");
	ret = HPR_SemTimedWait(&g_hSem, 3*1000);
	if (ret == 0)
	{
		g_sem_i++;
		printf("test_sem_0 main g_sem_i = %d\n", g_sem_i);
	}
	else
	{
		printf("test_sem_0 wait sem error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}

	if (g_hThread != NULL)
	{
		printf("test_sem_0 wait thread svc\n");
		HPR_Thread_Wait(g_hThread);
		g_hThread = NULL;
	}

	printf("test_sem_0 destroy sem\n");
	HPR_SemDestroy(&g_hSem);

	return;
}

void test_sem_1()
{
	HPR_INT32 ret;
	
	ret = HPR_SemCreate(&g_hSem,1);
	if (ret != 0)
	{
		printf("test_sem_1 create semaphore error, may bugs!!!, %d, %s\n", errno, strerror(errno));
		return;
	}

	g_hThread = HPR_Thread_Create(test_sem_svc,0,0);
	if (g_hThread == NULL)
	{
		HPR_SemDestroy(&g_hSem);
		printf("test_sem_1 create thread error, may bugs!!!, %d, %s\n", errno, strerror(errno));
		return;
	}

	printf("test_sem_1 ready to wait sem\n");
	ret = HPR_SemWait(&g_hSem);
	if (ret == 0)
	{
		g_sem_i++;
		printf("test_sem_1 main g_sem_i = %d\n", g_sem_i);
	}
	else
	{
		printf("test_sem_1 wait sem error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}

	if (g_hThread != NULL)
	{
		printf("test_sem_1 wait thread svc\n");
		HPR_Thread_Wait(g_hThread);
		g_hThread = NULL;
	}

	printf("test_sem_1 destroy sem\n");
	HPR_SemDestroy(&g_hSem);
}
