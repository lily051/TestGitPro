#include "DemoApp.h"
#include "DemoMutexClass.h"
#include "DemoSemClass.h"
#include "DemoSockUtilsClass.h"
#include "DemoMsgQClass.h"
#include "DemoStringClass.h"
#include "DemoTimeClass.h"

//Test All Function
void Test_All();

int main()
{
	if (HPR_Init())
	{
		printf("HPR_Init Error\n");
		return 0;
	}

	//const char* szFormat = "%s-%d";
	//char *szString = "12345";
	//char szRcvBuf[1024] = {0};
	//va_list ap
	//va
	//HPR_VSnprintf(szRcvBuf,1024,"%s-%d", 1024,szString);
	//Test_All();
	//Test_Mutex();		//OK
	//Test_Sema();		//OK
	//Test_Cond();		//OK
	//Test_FileSystem();
	//Test_MsgQ();		//OK
	//Test_Timer();		//OK
	//Test_Addr();		//OK
	//Test_DllSo();		//OK
	//Test_String();	//OK
	//Test_Socket();	//OK
	//Test_ThreadPool(); 
	//Test_Thread();
	//Test_Event();
	//Test_Event_Wait();
	//Test_MsqQEx();
	//Test_Atomic();
	//Test_Utils();
	//Test_NetDB();
	Test_Time();
	//Test_AsyncIO();
	//Test_ThreadMaxThread();
	//Test_ThreadPool();
	//Test_ThreadPoolFlex();

	//Mutex Class Test
	//i_test::i_mutex mutex;
	//mutex.Test();

	//Sem Class Test
	//i_test::i_sem sem;
	//sem.Test();

	//i_test::i_msgq msgq;
	//msgq.Test();

	//i_test::i_string str;
	//str.Test();

	//i_test::i_time t;
	//t.Test();

	//////////////////////////////////////////////////////////////////////////
	//add your test case here.

	//////////////////////////////////////////////////////////////////////////
	

	//cout << "+++ Test hpr::hpr_timetick" << endl;
	//Test_TimeTick();

	//cout << "+++ Test hpr::hpr_net_addr" << endl;
	//Test_NetAddr();

	//SockUtils TCPServer Class Test
	//i_test::i_tcp_server tcp_server;
	//tcp_server.Test();

	//cout << "+++ hpr_sock_utils (1)" << endl;
	//Test_SockUtils();

	//SockUtils TCPClient Class Test
	//i_test::i_tcp_client tcp_client;
	//tcp_client.Test();
	
	HPR_Fini();

	return 0;
}

void Test_All()
{
	printf("########## Test Mutex #############\n");	
	Test_Mutex();
	printf("########## Test Sema #############\n");	
	Test_Sema();
	printf("########## Test Cond #############\n");	
	Test_Cond();
	printf("########## Test Socket #############\n");	
	//Test_Socket();
	printf("########## Test FileSystem #############\n");	
	//Test_FileSystem();
	printf("########## Test Utils #############\n");	
	//Test_Utils();
	printf("########## Test Addr #############\n");	
	Test_Addr();
	printf("########## Test MsgQ#############\n");	
	//Test_MsgQ();
	printf("########## Test String #############\n");	
	Test_String();
	printf("########## Test Time #############\n");	
	//Test_Time();
	printf("########## Test Timer #############\n");	
	Test_Timer();
	printf("########## Test Thread #############\n");	
	//Test_Thread();
	printf("########## Test ThreadPool #############\n");	
	//Test_ThreadPool();
	printf("########## Test DllSo #############\n");	
	Test_DllSo();
	printf("########## Test Select #############\n");	
	//Test_Select();
	printf("########## Test AsynIO #############\n");	
	//Test_AsynIO();
	printf("########## Test Event #############\n");	
	//Test_Event();
	printf("########## Test MsgQEx #############\n");	
	//Test_MsqQEx();
	printf("########## Test Atomic #############\n");	
	//Test_Atomic();
	printf("########## Test SimpleHash #############\n");	
    //Test_SimpleHash();
	printf("########## Test ThreadPoolFlex #############\n");	
	//Test_ThreadPoolFlex();
	Test_ThreadPoolFlex();
}

