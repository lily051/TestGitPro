#include "DemoApp.h"

#if 1
static HPR_MUTEX_T g_hMutex;
static HPR_HANDLE g_hThread;
static int g_i = 0;
static void* CALLBACK test_mutex_svc(void* pParam);

void test_default_muetx();//recursive mutex
void test_fast_mutex();//fast
void test_errorcheck_mutex();//errorcheck

void Test_Mutex()
{
	//printf("MACRO PTHREAD_MUTEX_FAST_NP = %d\n", PTHREAD_MUTEX_FAST_NP);
	//printf("MACRO PTHREAD_MUTEX_ERRORCHECK_NP = %d\n", PTHREAD_MUTEX_ERRORCHECK_NP);
	printf("Test_Mutex::test_default_mutex ************************************\n");
	test_default_muetx();
	printf("Test_Mutex::test_fast_mutex **************************************\n");
	test_fast_mutex();
	printf("Test_Mutex::test_errorcheck_muter ***************************************\n");
	test_errorcheck_mutex();
}

void* CALLBACK test_mutex_svc(void* pParam)
{
	HPR_INT32 ret;
	printf("svc ready to lock 1\n");
	ret = HPR_MutexLock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_mutex_svc lock 1 success\n");
	}
	else
	{
		printf("test_mutex_svc lock 1 error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return 0;
	}

	g_i++;
	printf("svc g_i = %d\n", g_i);

	ret = HPR_MutexUnlock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_mutex_svc unlock 1 success\n");
	}
	else
	{
		printf("test_mutex_svc unlock 1 error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}

	return 0;
}

//recursive mutex
void test_default_muetx()
{
	HPR_INT32 ret;

	ret = HPR_MutexCreate(&g_hMutex);
	if (ret == -1)
	{
		printf("test_default_muetx create mutex error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	g_hThread = HPR_Thread_Create(test_mutex_svc,0,0);
	if (g_hThread == NULL)
	{
		printf("test_default_muetx error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_DEFAULT_MUTEX_END;
	}

	ret = HPR_MutexLock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_default_muetx lock 1 success\n");
	}
	else
	{
		printf("test_default_muetx lock 1 error,may bugs, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_DEFAULT_MUTEX_END;
	}

	g_i++;
	printf("main 1 g_i = %d\n", g_i);

	ret = HPR_MutexLock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_default_muetx lock 2 success\n");
	}
	else
	{
		printf("test_default_muetx lock 2  error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_DEFAULT_MUTEX_END;
	}

	g_i++;
	printf("main 2 g_i = %d\n", g_i);

	ret = HPR_MutexUnlock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_default_muetx unlock 1 success\n");
	}
	else
	{
		printf("test_default_muetx unlock 1 error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_DEFAULT_MUTEX_END;
	}
	
	ret = HPR_MutexUnlock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_default_muetx unlock 2 success\n");
	}
	else
	{
		printf("test_default_muetx unlock 2 error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_DEFAULT_MUTEX_END;
	}

TEST_DEFAULT_MUTEX_END:
	if (g_hThread != NULL)
	{
		printf("test_default_muetx wait for thread\n");
		HPR_Thread_Wait(g_hThread);
		g_hThread = NULL;
	}

	printf("test_fast_mutex destroy mutex\n");
	HPR_MutexDestroy(&g_hMutex);

	return;
}

//fast mutex, the posix default mutex attr.
void test_fast_mutex()
{
	HPR_INT32 ret;

	ret = HPR_MutexCreate(&g_hMutex, HPR_MUTEX_FAST);
	if (ret != 0)
	{
		printf("test_fast_mutex create mutex  error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	ret = HPR_MutexLock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_fast_mutex lock 1 success\n");
	}
	else
	{
		printf("test_fast_mutex lock 1  error,may bugs!!!, errno = %d, %s\n",errno, strerror(errno));
		goto TEST_FAST_MUTEX_END;
	}

	g_hThread = HPR_Thread_Create(test_mutex_svc,0,0);
	if (g_hThread == NULL)
	{
		printf("test_fast_muetx create thread error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		HPR_MutexUnlock(&g_hMutex);
		goto TEST_FAST_MUTEX_END;
	}

	g_i++;
	printf("main 1 g_i = %d\n", g_i);

	ret = HPR_MutexTryLock(&g_hMutex);
	if (ret == 0)
	{
		g_i++;
		printf("main 2 g_i = %d\n", g_i);
#if defined (OS_POSIX)
		printf("test_fast_mutex trylock 1 success, but test function error, may bugs !!!\n");
#endif
		HPR_MutexUnlock(&g_hMutex);
	}
	else
	{
		printf("test_fast_mutex trylock 1  error, errno = %d, %s\n",errno, strerror(errno));
		printf("test_fast_mutex ok as mutex attr is fast and one thread can only lock one time\n");
	}

	HPR_MutexUnlock(&g_hMutex);

TEST_FAST_MUTEX_END:
	if (g_hThread != NULL)
	{
		printf("test_fast_mutex wait for thread\n");
		HPR_Thread_Wait(g_hThread);
		g_hThread = NULL;
	}

	printf("test_fast_mutex destroy mutex\n");
	HPR_MutexDestroy(&g_hMutex);

	return;
}

//errorcheck mutex
void test_errorcheck_mutex()
{
	HPR_INT32 ret;

	ret = HPR_MutexCreate(&g_hMutex,HPR_MUTEX_ERRORCHECK);
	if (ret == -1)
	{
		printf("test_errorcheck_muetx create mutex error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	g_hThread = HPR_Thread_Create(test_mutex_svc,0,0);
	if (g_hThread == NULL)
	{
		printf("test_errorcheck_muetx create thread error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_ERRORCHECK_MUTEX_END;
	}
	ret = HPR_MutexLock(&g_hMutex);
	if (ret == 0)
	{
#if defined (OS_POSIX)
		printf("test_errorcheck_muetx lock 1 success\n");
#endif
	}
	else
	{
		printf("test_errorcheck_muetx lock 1 error,may bugs, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_ERRORCHECK_MUTEX_END;
	}

	g_i++;
	printf("main 1 g_i = %d\n", g_i);

	ret = HPR_MutexLock(&g_hMutex);
	if (ret == 0)
	{
#if defined (OS_POSIX)
		printf("test_errorcheck_muetx lock 2 success, may bugs!!!\n");
#elif defined (OS_WINDOWS)
		HPR_MutexUnlock(&g_hMutex);
#endif
	}
	else
	{
		printf("test_errorcheck_muetx lock 2  error, errno = %d, %s\n", errno, strerror(errno));
	}

	HPR_Sleep(100);

	g_i++;
	printf("main 2 g_i = %d\n", g_i);

	ret = HPR_MutexUnlock(&g_hMutex);
	if (ret == 0)
	{
		printf("test_errorcheck_muetx unlock 1 success\n");
	}
	else
	{
		printf("test_errorcheck_muetx unlock 1 error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		goto TEST_ERRORCHECK_MUTEX_END;
	}
	
//	ret = HPR_MutexUnlock(&g_hMutex);
//	if (ret == 0)
//	{
//		printf("test_errorcheck_muetx unlock 2 success\n");
//	}
//	else
//	{
//		printf("test_errorcheck_muetx unlock 2 error,may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
//		goto TEST_ERRORCHECK_MUTEX_END;
//	}

TEST_ERRORCHECK_MUTEX_END:
	if (g_hThread != NULL)
	{
		printf("test_errorcheck_muetx wait for thread\n");
		HPR_Thread_Wait(g_hThread);
		g_hThread = NULL;
	}

	printf("test_errorcheck_muetx destroy mutex\n");
	HPR_MutexDestroy(&g_hMutex);
	
	return;
}

#endif
