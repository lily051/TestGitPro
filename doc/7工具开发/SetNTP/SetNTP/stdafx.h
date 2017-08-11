
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����

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
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









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


