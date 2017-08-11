
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类

#include <queue>
#include "StringChange.h"
#include "../public/mysql/include/mysql.h"
#include "../public/hak/include/hak.h"
#include "../public/sdk/inc/hcs_sdk.h"
#include "../public/sdk/inc/hcs_sdk_define.h"
#include "../public/HLOG/lib/include/hlog.h"
#include "../public/hak/include/hak.h"
#include "../public//HCNet/HCNetSDK.h"
#pragma comment(lib, "../Release/hcs_sdk.lib")
#pragma comment(lib, "../Release/hlog.lib")
#pragma comment(lib, "../Release/libmysql.lib")
#pragma comment(lib, "../Release/libmysql.lib")
#pragma comment(lib, "../Release/libhak.lib")
#pragma comment(lib, "../Release/HCCore.lib")
#pragma comment(lib, "../Release/HCNetSDK.lib")
using namespace std;
#define CHAR_LENGTH 30
typedef struct DVRStruct
{

	char cDvrIp[CHAR_LENGTH];
	char cDvrUser[CHAR_LENGTH];
	char cDvrPwd[MAX_PATH];
	int  iPort;
}DVRStruct;


#define NTP_LOG_ERROR(fmt, ...)		         hlog_format(HLOG_LEVEL_ERROR, "ntp", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define NTP_LOG_WARN(fmt, ...)               hlog_format(HLOG_LEVEL_WARN, "ntp", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define NTP_LOG_INFO(fmt, ...)               hlog_format(HLOG_LEVEL_INFO, "ntp", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define NTP_LOG_DEBUG(fmt, ...)              hlog_format(HLOG_LEVEL_DEBUG, "ntp", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define NTP_LOG_TRACE(fmt, ...)              hlog_format(HLOG_LEVEL_TRACE, "ntp", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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


