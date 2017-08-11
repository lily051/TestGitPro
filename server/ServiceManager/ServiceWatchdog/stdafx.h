
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <gdiplus.h>
using namespace Gdiplus;  
#pragma comment(lib, "gdiplus.lib")

#include <atlbase.h>

#include <afxsock.h>            // MFC 套接字扩展
#pragma comment(lib, "ws2_32.lib")

#pragma comment(lib, _T("./Utilities/CRashAPI.lib"))

#include "fclib/fcTchar.h"

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
#ifdef _DEBUG
//     #pragma comment(lib, "../../../public/base/hpr/v1.0.0.0/lib/x86_vc60/hpr.lib")
//     #pragma comment(lib, "../../../public/base/hlog/v2.1.0.42965/win_32_debug/hlog.lib")
    //#pragma comment(lib, "../../../public/base/HPP/lib/win32/vs2008_debug/hppd.lib")
    //#pragma comment(lib, "../../../public/base/Google/protobuf/lib/vs2008_debug/libprotobuf.lib")
    //#pragma comment(lib, "../../../public/finance/cms_8100/proto/_cms_8100/lib_debug/cms_8100_d.lib")
#else
//     #pragma comment(lib, "../../../public/base/hpr/v1.0.0.0/lib/x86_vc60/hpr.lib")
//     #pragma comment(lib, "../../../public/base/hlog/v2.1.0.42965/win_32_release/hlog.lib")
    //#pragma comment(lib, "../../../public/base/HPP/lib/win32/vs2008_release/hpp.lib")
    //#pragma comment(lib, "../../../public/base/Google/protobuf/lib/vs2008_release/libprotobuf.lib")
    //#pragma comment(lib, "../../../public/finance/cms_8100/proto/_cms_8100/lib_release/cms_8100.lib")
#endif

// 加载资源库
#pragma comment(lib, "Lang/MultiLanRes.lib")

#include <Windows.h>

#include "log/hlog_category.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


