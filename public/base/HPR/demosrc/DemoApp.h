#ifndef __TEST_DEMOAPP_H__
#define __TEST_DEMOAPP_H__

#if defined(__linux__)
#define WSAGetLastError() errno
#endif

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Hpr.h"
#include "HPR_Mutex.h"
#include "HPR_Cond.h"
#include "HPR_Semaphore.h"
#include "HPR_Socket.h"
#include "HPR_FileSystem.h"
#include "HPR_Utils.h"
//#include "HPR_Addr.h"
#include "HPR_MsgQ.h"
#include "HPR_String.h"
#include "HPR_Time.h"
#include "HPR_Timer.h"
#include "HPR_Thread.h"
#include "HPR_ThreadPool.h"
#include "HPR_ThreadPoolFlex.h"
#include "HPR_Atomic.h"
#include "HPR_DllSo.h"
#include "HPR_Select.h"
#include "HPR_AsyncIO.h"
#include "HPR_Event.h"
#include "HPR_Netdb.h"
#include "HPR_Error.h"
#include "HPR_Sock_Utils.h"
#include "HPR_Net_Addr.h"
#include "HPR_Singleton.h"


#include <iostream>
using namespace std;

void Test_Mutex();
void Test_Cond();
void Test_Sema();
void Test_Socket();
void Test_FileSystem();
void Test_Utils();
void Test_Addr();
//void Test_MsgQ();
void Test_String();
void Test_Time();
void Test_Timer();
void Test_Thread_Detached();
void Test_Thread();
void Test_ThreadMaxThread();
void Test_ThreadPool();
void Test_ThreadPoolFlex();
void Test_DllSo();
void Test_Select();
void Test_AsyncIO();
void Test_Event();
void Test_Event_Wait();
void Test_MsqQEx();
void Test_Atomic();
void Test_NetDB();
int  Test_SimpleHash();
void Test_SockUtils();
void Test_NetAddr();
void Test_TimeTick();

#endif
