#include "DemoApp.h"

#include <stdio.h>
#include <iostream>
using namespace std;
#define LOOP_COUNT 100000
#define WORK_COUNT 10000000
static void* CALLBACK s_fWork(void* pParam);

extern int HPR_InitThreadPool_Inter();
extern void HPR_FiniThreadPool_Inter();

static int g_i[WORK_COUNT] = {0};

void Test_ThreadPool()
{
	//1.测试创建销毁功能
	HPR_HANDLE hHandle = NULL;
	int i = 0;

	for (i=0; i<WORK_COUNT; i++)
	{
		g_i[i] = i;
	}

	i = 0;
	for (; i<LOOP_COUNT; i++)
	{
		hHandle = HPR_ThreadPool_Create(20, 30);
		//int k = 0;
		//for (; k<WORK_COUNT; k++)
		//{
		//	HPR_ThreadPool_Work(hHandle, s_fWork, &g_i[k]);
		//}

		if (i%1000 == 0)
		{
			printf("loop count = %d \n", i);
		}

		HPR_ThreadPool_Destroy(hHandle);
	}
	
	printf("%d\n", i);
	cin.get();

	//2.测试work功能
	hHandle = HPR_ThreadPool_Create(10, 50);
	int j = 0;
	int k = 0;
	for (; k<WORK_COUNT; k++)
	{
		if (-1 == HPR_ThreadPool_Work(hHandle, s_fWork, &g_i[k]) )
		{

			printf("HPR_ThreadPool_Work Error %d\n", i);
			j++;
		}
	}

	printf("j = %d, i = %d\n", j, k);

	HPR_USleep(1000000);

	HPR_ThreadPool_Destroy(hHandle);

	cin.get();

	return;
};

static void* CALLBACK s_fWork(void* pParam)
{
	if (*(int*)pParam%10000 == 0)
	{
		printf("s_fWork %d\n", *(int*)pParam);
	}

	return NULL;
}

