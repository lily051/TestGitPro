// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <WinSock2.h>


#pragma warning(disable:4005)
// TODO: reference additional headers your program requires here
#include "fchlogConfig.h"
#include "Msger.h"

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
#include "HPR_Guard.h"

namespace {
// 有时候，true/false并不够用
typedef int mybool_t;
mybool_t const MYBOOL_EXCEPTION = -1;
mybool_t const MYBOOL_FALSE = 0;
mybool_t const MYBOOL_TRUE = 1;
}

