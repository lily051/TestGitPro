#include "DemoApp.h"

static HPR_HANDLE s_SvcWaitHandle;
static HPR_HANDLE s_MainWaitHandle;

static HPR_HANDLE s_ThreadHandle;

void test_event_wait();

void* CALLBACK test_event_wait_svc(void* pParam)
{
	HPR_HANDLE hEvents[1];
	hEvents[0] = s_SvcWaitHandle;
	//HPR_OutputDebug("schina  test_event_wait_svc before WaitForSingleObject 0\n");

	HPR_OutputDebug("schina ##############  call order 0\n");

	HPR_UINT32 nRet = HPR_WaitForMultipleObjects(1, hEvents, HPR_FALSE, HPR_INFINITE);
	
	HPR_OutputDebug("schina  test_event_wait_svc after WaitForSingleObject iRet = %u\n", nRet);

	//HPR_OutputDebug("schina  test_event_wait_svc before HPR_SetEvent 2\n");

	HPR_OutputDebug("schina ##############  call order 2\n");

	HPR_Sleep(200);

	HPR_OutputDebug("schina ##############  call order 3\n");

	HPR_SetEvent(s_MainWaitHandle);

	return NULL;
}

void Test_Event_Wait()
{
	s_SvcWaitHandle = HPR_CreateEvent(HPR_FALSE);
	s_MainWaitHandle = HPR_CreateEvent(HPR_FALSE);

	//HPR_OutputDebug("schina Test_Event_Wait before HPR_Thread_Create 0\n");

	s_ThreadHandle = HPR_Thread_Create(test_event_wait_svc, NULL, 0);

	HPR_Sleep(1000);

	//HPR_OutputDebug("schina Test_Event_Wait before HPR_SetEvent 1\n");

	HPR_OutputDebug("schina ##############  call order 1\n");

	HPR_SetEvent(s_SvcWaitHandle);

	//HPR_OutputDebug("schina Test_Event_Wait After HPR_SetEvent 2\n");

	HPR_WaitForSingleObject(s_MainWaitHandle, HPR_INFINITE);

	HPR_OutputDebug("schina ##############  call order 4\n");

	//HPR_OutputDebug("schina Test_Event_Wait return 3\n");

	return;
}
