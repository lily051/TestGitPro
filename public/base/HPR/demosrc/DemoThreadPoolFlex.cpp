#include "DemoApp.h"
#include <stdio.h>
#include <iostream>
using namespace std;
#define LOOP_COUNT 10000000
#define WORK_COUNT 1000
void* CALLBACK s_fWork(void* pParam);

static int g_i[WORK_COUNT] = {0};

void Test_ThreadPoolFlex()
{
	//1.测试创建销毁功能
	void* hHandle = NULL;
	int i = 0;
	HPR_UINT32 nTimeNow = 0;
	HPR_UINT32 iMaxThead = 0;
	HPR_UINT32 iIdleTime = 0;

	for (i=0; i<WORK_COUNT; i++)
	{
		g_i[i] = i;
	}

	i = 0;
	for (; i<LOOP_COUNT; i++)
	{
		nTimeNow = HPR_GetTimeTick();
		iMaxThead = (nTimeNow)%100;
		iIdleTime = (nTimeNow)%120;
		printf("iMaxThead = %d,  iIdleTime=%d, cout = %d\n", (int)iMaxThead, (int)iIdleTime, i);
		hHandle = HPR_ThreadPoolFlex_Create(iMaxThead+1, iIdleTime+1, s_fWork, 0);
		if (!hHandle)
		{
			printf("thrmgr_new error\n");
			return;
		}

		int k = 0;
		for (; k<WORK_COUNT; k++)
		{
			HPR_ThreadPoolFlex_Work(hHandle, &g_i[k]);
		}

		if (i%1000 == 0)
		{
			printf("loop count = %d \n", i);
		}

		HPR_ThreadPoolFlex_Destroy(hHandle);
		hHandle = NULL;
	}

	printf("%d\n", i);
	cin.get();

	return;
};

void* CALLBACK s_fWork(void* pParam)
{
	if (*(int*)pParam%10000 == 0)
	{
		//printf("s_fWork %d\n", *(int*)pParam);
	}

	return NULL;
}
