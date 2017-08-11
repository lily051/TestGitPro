#include "DemoApp.h"

#if 0
static HPR_HANDLE g_iMsgQ;
static HPR_HANDLE g_hThread;
static void* test_msgq_svc(void* pParam);

void test_msgq();

void Test_MsgQ()
{
	printf("*****************************\n");
	printf("test_msgq ********************\n");
	test_msgq();
}

void test_msgq()
{
	HPR_INT32 ret;
	ret = HPR_CreateMsgQ(&g_iMsgQ);
	if (ret)
	{
		printf("test_msgq create msgq error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	g_hThread = HPR_Thread_Create(test_msgq_svc,0,0);
	if (g_hThread == NULL)
	{
		printf("test_msgq create thread error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		HPR_DestroyMsgQ(g_iMsgQ);
		return;
	}

	HPR_MSG_T struMsg = {0};
	struMsg.iMsgLen = 256;
	sprintf(struMsg.szMsg,"Hello World!");

	ret = HPR_MsgQSend(g_iMsgQ, &struMsg);
	if (ret)
	{
		printf("test_msgq send msgq error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}

	if (g_hThread != NULL)
	{
		printf("test_msgq wait for thread\n");
		HPR_Thread_Wait(g_hThread);
		g_hThread = NULL;
	}


	//HPR_Sleep(3*1000);

	printf("test_msgq destroy msgq\n");
	HPR_DestroyMsgQ(g_iMsgQ);	

}

void* test_msgq_svc(void* pParam)
{
	HPR_INT32 ret;
	HPR_MSG_T struMsg = {0};
	struMsg.iMsgLen = 256;

	printf("enter test_msgq_svc \n");
	
	ret = HPR_MsgQReceive(g_iMsgQ,&struMsg);
	if (!ret)
	{
		printf("test_msgq_svc receiev msgq success\n");
		printf("msgq.msg = %s\n", (char*)struMsg.szMsg);
	}
	else
	{
		printf("test_msgq_svc receive error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
	}

	printf("out of svc\n");

	return 0;
}

#endif
