#include "DemoApp.h"

typedef struct msgex_s
{
	int iMsgType;		/**< 消息类型			*/
	int iPriority;		/**< 消息优先级			*/
	int iMsgLen;		/**< 消息长度(pMsg长度)	*/
	char pMsg[128];		/**< 消息内容			*/
}msgex_t;

static HPR_HANDLE g_hMsgQId;
static HPR_HANDLE g_hThreadId;
static void test_msqqex();

static void* CALLBACK test_msgqex_svc(void* pParam)
{
	msgex_t struMsg;
	HPR_INT32 iRet = 0;

	//cin.get();

	while (1)
	{
		memset(&struMsg,0,sizeof(struMsg));
		//HPR_Sleep(2000);
		iRet = HPR_MsgQReceiveEx(g_hMsgQId,(HPR_MSG_BLOCK_T*)&struMsg,5000);
		if (iRet == HPR_OK)
		{
			printf("receive msgq = %s, len = %d, pri = %d\n", struMsg.pMsg,struMsg.iMsgLen,struMsg.iPriority);
			if (struMsg.iMsgType == 1)
			{
				break;
			}
		}
		else
		{
			printf("HPR_MsgQReceiveEx timeout\n");
		}
	}

	return NULL;
}

void Test_MsqQEx()
{
	test_msqqex();
}

void test_msqqex()
{
	HPR_INT32 iRet = HPR_ERROR;
	msgex_t struMsg;
	iRet = HPR_MsgQCreateEx(&g_hMsgQId);
	if (iRet < 0)
	{
		printf("test_msqqex create msgq error!!!!!!!\n");
		return;
	}

	struMsg.iMsgType = 0;

	g_hThreadId = HPR_Thread_Create(test_msgqex_svc,NULL,0);
	
	for (int i=0; i<30; i++)
	{
		struMsg.iPriority = i%3;
		if (i%3 == 0)//模拟超时
		{
			//HPR_Sleep(3000);
		}

		struMsg.iMsgLen = HPR_Snprintf(struMsg.pMsg,128,"%d",i);
		if (i == 9)//模拟数据越界
		{
			struMsg.iMsgLen = 10000;
		}

		if (i == 19)//模拟数据越界
		{
			struMsg.iMsgLen = 3;
		}

		if ( HPR_MsgQSendEx(g_hMsgQId,(HPR_MSG_BLOCK_T*)&struMsg,1000) == HPR_ERROR )
		{
			printf("HPR_MsgQSendEx error, %d\n", i);
		}
		else
		{
			printf("HPR_MsgQSendEx succ, %d\n", i);
		}
	}

	HPR_Sleep(1000);
	struMsg.iMsgType = 1;
	struMsg.iMsgLen = 1;
	struMsg.pMsg[0] = 'A';
	HPR_MsgQSendEx(g_hMsgQId,(HPR_MSG_BLOCK_T*)&struMsg,1000);

	//printf("send msg end\n");

	HPR_Thread_Wait(g_hThreadId);

	HPR_MsgQDestroyEx(g_hMsgQId);
	g_hMsgQId = NULL;
}

