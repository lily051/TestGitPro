
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



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include "file_info.h"
#include "StringConverter.h"
#include "StringChange.h"
#include "../public/HLOG/lib/include/hlog.h"
#pragma comment(lib,"Version.lib") 

//升级文件信息
typedef struct FileInfo
{
	CString strName;     //本地文件名
	CString strVersion;  
	CString strPath;     //安装路径
	int nFlag;           //升级标志

	int nFileCount;      //文件序号
	ULONGLONG size;      //URL 文件大小
	CString Hash;        //文件MD5值
	CString Subcatalog;  //文件子目录
	CString Url;         //文件URL

}FILEINFO;

// 计算文件的MD5摘要码
extern BOOL CalculateMD5(const char* sFilename, unsigned char acMD5Digest[]);
extern CString MD5toString(unsigned char acMD5Digest[]);


#define UPDATER_LOG_ERROR(fmt, ...)		         hlog_format(HLOG_LEVEL_ERROR, "UPDATER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define UPDATER_LOG_WARN(fmt, ...)               hlog_format(HLOG_LEVEL_WARN, "UPDATER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define UPDATER_LOG_INFO(fmt, ...)               hlog_format(HLOG_LEVEL_INFO, "UPDATER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define UPDATER_LOG_DEBUG(fmt, ...)              hlog_format(HLOG_LEVEL_DEBUG, "UPDATER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define UPDATER_LOG_TRACE(fmt, ...)              hlog_format(HLOG_LEVEL_TRACE, "UPDATER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  

#define MAKEINI_LOG_ERROR(fmt, ...)		         hlog_format(HLOG_LEVEL_ERROR, "MakeInI", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define MAKEINI_LOG_WARN(fmt, ...)               hlog_format(HLOG_LEVEL_WARN, "MakeInI", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MAKEINI_LOG_INFO(fmt, ...)               hlog_format(HLOG_LEVEL_INFO, "MakeInI", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MAKEINI_LOG_DEBUG(fmt, ...)              hlog_format(HLOG_LEVEL_DEBUG, "MakeInI", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MAKEINI_LOG_TRACE(fmt, ...)              hlog_format(HLOG_LEVEL_TRACE, "MakeInI", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  


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


