#include "DemoApp.h"

#if defined OS_WINDOWS
	#if (_MSC_VER < 1310)
		#define U64_2_DB(x) double(double(HPR_UINT32((x) >> 32)) * double(0x100000000) + double(HPR_UINT32(x)))
	#else
		#define U64_2_DB(x) (x)
	#endif
#else
	#define U64_2_DB(x) (x)
#endif

static HPR_SEM_T ExitPerform;
HPR_VOIDPTR CALLBACK Performance(HPR_VOIDPTR)
{
	HPR_CPU_PERFORMANCE *cpu_per = NULL;
	HPR_CPU_PERFORMANCE *o_cpu_per = NULL;
	HPR_MEMORY_STATUS mem_sta;;
	HPR_NETWORK_FLOWDATA net_flo;
	HPR_NETWORK_FLOWDATA o_net_flo[8];

	HPR_UINT32 tick = 0;

	int cpu_num = HPR_GetCPUNumber();
	HPR_UINT32 size = cpu_num * sizeof(HPR_CPU_PERFORMANCE);
	cpu_per = new HPR_CPU_PERFORMANCE[cpu_num * 2];
	memset(cpu_per, 0, 2 * size);
	o_cpu_per = cpu_per + cpu_num;
	double cpu_usage = 0;
	double one_usage = 0;
	HPR_UINT64 cpu_sum = 0;
	HPR_UINT64 o_cpu_sum = 0;

	HPR_ADAPTER_INFO *pAdapter = NULL;
	HPR_GetAdapterInfo(&pAdapter);
	HPR_ADAPTER_INFO *pTmp = NULL;

	fprintf(stderr, "start performance monitor\n");
	while (HPR_SemTimedWait(&ExitPerform, 1000) != HPR_OK)
	{
		if (HPR_GetCPUPerformance(cpu_per, &size) == HPR_OK)
		{
			fprintf(stderr, "cpu [");
			for (int i = 0; i < cpu_num; ++ i)
			{
				cpu_sum = cpu_per[i].u64KernelTime + cpu_per[i].u64UserTime + cpu_per[i].u64Reserve1[0] + cpu_per[i].u64Reserve1[1];
				o_cpu_sum = o_cpu_per[i].u64KernelTime + o_cpu_per[i].u64UserTime + o_cpu_per[i].u64Reserve1[0] + o_cpu_per[i].u64Reserve1[1];
				one_usage = (double)(U64_2_DB(cpu_sum) - U64_2_DB(o_cpu_sum) - U64_2_DB(cpu_per[i].u64IdleTime) + U64_2_DB(o_cpu_per[i].u64IdleTime)) * 100 / (double)(U64_2_DB(cpu_sum - o_cpu_sum));
				cpu_usage += one_usage;
				fprintf(stderr, "%2.2lf %%  ", one_usage);
			}
			fprintf(stderr, "= %2.2lf %%]\n", cpu_usage / cpu_num);

			cpu_usage = 0;
			memcpy(o_cpu_per, cpu_per, size);
		}

		if (HPR_GetMemoryStatus(&mem_sta) == HPR_OK)
		{
#if defined (OS_WINDOWS)
#if defined OS_WINDOWS
			fprintf(stderr, "memory[phys: %I64d/%I64d KB, page: %I64d/%I64d KB]\n"
#else
			fprintf(stderr, "memory[phys: %lld/%lld KB, page: %lld/%lld KB]\n"
#endif
				, (mem_sta.u64TotalPhys - mem_sta.u64AvailPhys) / 1024
				, mem_sta.u64TotalPhys / 1024
				, (mem_sta.u64TotalPageFile - mem_sta.u64AvailPageFile) / 1024
				, mem_sta.u64TotalPageFile / 1024);

#else
			cout << "memory[phys:" << ((mem_sta.u64TotalPhys - mem_sta.u64AvailPhys) / 1024) \
				 << "/" << (mem_sta.u64TotalPhys / 1024) << " KB, " \
				 << "page: " <<  ((mem_sta.u64TotalPageFile - mem_sta.u64AvailPageFile) / 1024)\
				 << "/" << ((mem_sta.u64TotalPageFile / 1024))<< "KB" << endl;
#endif
		}

		pTmp = pAdapter;
		for (int i = 0; pTmp != NULL; pTmp = pTmp->pNext, ++ i)
		{
			net_flo.iIndex = pTmp->iIndex;
			strncpy(net_flo.szAdapterName, pTmp->szAdapterName, sizeof(pTmp->szAdapterName));
			if (HPR_GetNetWorkFlowData(&net_flo) == HPR_OK)
			{
				fprintf(stderr, "network[index: %d, flow: %2.2lf %%]\n", net_flo.iIndex\
					, double(net_flo.nInBytes + net_flo.nOutBytes - o_net_flo[i].nInBytes - o_net_flo[i].nOutBytes) * 100 / (net_flo.nSpeed / 8) / double((HPR_GetTimeTick() - tick) / 1000));
				memcpy(&o_net_flo[i], &net_flo, sizeof(net_flo));
			}
		}

		tick = HPR_GetTimeTick();
		fprintf(stderr, "\n");
	}

	HPR_FreeAdapterInfo(pAdapter);
	delete[] cpu_per;
	fprintf(stderr, "stop performance monitor\n");
	return NULL;
}

void Test_Utils()
{

	printf("test zeromemory\n");
	char szBuf[] = "123456";
	printf("szBuf = %s\n", szBuf);
	HPR_ZeroMemory(szBuf, sizeof(szBuf));
	printf("after HPR_ZeroMemory, szBuf = %s\n", szBuf);

	printf("test random\n");
	HPR_UINT32 nRandom = 0;
	printf("nRandom = %u\n", nRandom);
	nRandom = HPR_Rand();
	printf("after HPR_Rand nRandom = %u\n", nRandom);

	HPR_SemCreate(&ExitPerform, 0);
	HPR_HANDLE hThread = HPR_Thread_Create(Performance, NULL, 1024 * 1024);
	if (hThread != HPR_INVALID_THREAD)
	{
		cin.get();
		HPR_SemPost(&ExitPerform);
		HPR_Thread_Wait(hThread);
	}
	HPR_SemDestroy(&ExitPerform);
	return;
}
