#include "DemoApp.h"

static HPR_COND_T g_hCond;
static HPR_MUTEX_T g_hMutex;
static HPR_HANDLE g_hThread1;
static HPR_HANDLE g_hThread2;

static int g_cond_i = 0;

static void* CALLBACK test_cond_svc(void* pParam);
void test_cond_single();
void test_cond_broadcast();
void sys_test_cond_single();

void Test_Cond()
{
	printf("***********************\n");
	printf("test_cond_single**********************\n");
	test_cond_single();
	printf("test_cond_broadcast**********************\n");
	test_cond_broadcast();
	printf("SYS_TEST cond_signal**********************\n");
	sys_test_cond_single();
}

void* CALLBACK test_cond_svc(void* pParam)
{
	HPR_INT32 ret;

	printf("enter test_cond_svc \n");
	HPR_MutexLock(&g_hMutex);
	printf("test_cond_svc mutex lock success\n");
	ret = HPR_CondWait(&g_hCond,&g_hMutex);
	printf("test_cond_svc 1  g_cond_i = %d\n", g_cond_i);
	HPR_MutexUnlock(&g_hMutex);

	//HPR_MutexLock(&g_hMutex);
	//ret = HPR_CondWait(&g_hCond,&g_hMutex);
	//printf("test_cond_svc 2  g_cond_i = %d\n", g_cond_i);
	//HPR_MutexUnlock(&g_hMutex);
	return 0;
}

void test_cond_single()
{
	HPR_INT32 ret;
	
	HPR_MutexCreate(&g_hMutex);
	ret = HPR_CondCreate(&g_hCond);
	if (ret)
	{
		printf("test_cond_single create cond error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	g_hThread1 = HPR_Thread_Create(test_cond_svc,0,0);
	if (g_hThread1 == NULL)
	{
		printf("test_cond_single create thread error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		HPR_CondDestroy(&g_hCond);
		return;
	}

	HPR_Sleep(5000); //signal after 1 second.

	HPR_MutexLock(&g_hMutex);
	if (HPR_CondSignal(&g_hCond))
	{
		printf("test_cond_single signal error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}
	g_cond_i++;
	printf("test_cond_single g_cond_i = %d\n", g_cond_i);
	HPR_MutexUnlock(&g_hMutex);

	if (g_hThread1 != NULL)
	{
		printf("test_cond_single wait for thread\n");
		HPR_Thread_Wait(g_hThread1);
		g_hThread1 = NULL;
	}

	HPR_CondDestroy(&g_hCond);
	HPR_MutexDestroy(&g_hMutex);

	return;
}

void test_cond_broadcast()
{
	HPR_INT32 ret;
	
	HPR_MutexCreate(&g_hMutex);
	ret = HPR_CondCreate(&g_hCond);
	if (ret)
	{
		printf("test_cond_broadcast create cond error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	g_hThread1 = HPR_Thread_Create(test_cond_svc,0,0);
	g_hThread2 = HPR_Thread_Create(test_cond_svc,0,0);
	if (g_hThread1 == NULL)
	{
		printf("test_cond_broadcast create thread error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		HPR_CondDestroy(&g_hCond);
		return;
	}

	HPR_Sleep(1000); //signal after 1 second.

	HPR_MutexLock(&g_hMutex);
	if (HPR_CondBroadCast(&g_hCond))
	{
		printf("test_cond_broadcast signal error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}
	g_cond_i++;
	printf("test_cond_broadcast g_cond_i = %d\n", g_cond_i);
	HPR_MutexUnlock(&g_hMutex);

	if (g_hThread1 != NULL)
	{
		printf("test_cond_broadcast wait for thread\n");
		HPR_Thread_Wait(g_hThread1);
		HPR_Thread_Wait(g_hThread2);
		
		g_hThread1 = NULL;
		g_hThread2 = NULL;
	}

	HPR_CondDestroy(&g_hCond);
	HPR_MutexDestroy(&g_hMutex);

	return;
}

void sys_test_cond_single()
{
	HPR_INT32 ret;
	
	HPR_MutexCreate(&g_hMutex);
	ret = HPR_CondCreate(&g_hCond);
	HPR_MutexLock(&g_hMutex);
	if (HPR_CondBroadCast(&g_hCond))
	{
		printf("test_cond_broadcast signal error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}
	HPR_MutexUnlock(&g_hMutex);

	HPR_CondWait(&g_hCond, &g_hMutex);

	HPR_CondDestroy(&g_hCond);
	HPR_MutexDestroy(&g_hMutex);

	return;
}

