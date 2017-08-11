#include "DemoApp.h"

typedef int (*Proc_Init)();

void Test_DllSo()
{
	printf("*********test_dllso*******************8\n");

	HPR_HANDLE hDso;
	HPR_PROC hProc;

#if defined (OS_WINDOWS)
	hDso = HPR_LoadDSo("hpr.dll");
#elif defined (OS_POSIX)
	hDso = HPR_LoadDSo("libhpr.so");
#else
#endif

	if (hDso == HPR_INVALID_HANDLE)
	{
		printf("test_dllso errno, may bugs!!!!!!!!!!!!!!!,errno = %d, %s\n", errno, strerror(errno));
		return;
	}
	else
	{
		printf("test_dllso HPR_LoadDSo success\n");
	}

	hProc = HPR_GetDsoSym(hDso, "HPR_Init");
	if (hProc)
	{
		printf("test_dllso success\n");
		Proc_Init pFunc = (Proc_Init)hProc;
		if(0 == pFunc())
		{
			printf("test_dllso call func by hproc success\n");
		}
		else
		{
			printf("test_dllso call func by hproc error, may bugs!!!!!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
		}
	}
	else
	{
		printf("test_dllso HPR_GetDsoSym error, may bugs!!!!!!!!!!, errno = %d, %s\n", errno, strerror(errno));
	}

	HPR_UnloadDSo(hDso);

	return;
}
