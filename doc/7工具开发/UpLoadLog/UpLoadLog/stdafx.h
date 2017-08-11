
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

#include "FtpHttpUpload.h"
#include "UploadThread.h"
#include "StringChange.h"
#include "zconf.h"

#include "../HLOG/lib/include/hlog.h"
#pragma comment(lib, "../Release/hlog.lib")

#define PARA_KEY_CHECKURL _T("CheckURL=")        //URL
#define PARA_KEY_CHECLOGPATH _T("CheckPath=")    //日志路径
#define PARA_KEY_CHECFLAG _T("CheckFlag=")       //上传方式
#define PARA_KEY_CHECAPPNAME _T("CheckAppName=") //调用该程序的名字


enum {
	WM_FILE_EXIST = WM_USER + 1,  //文件已经存在
	WM_UPLOAD_OK,                 //上传文件成功
	WM_UPLOAD_PROCESS,            //上传进度
	WM_FTPUPLOAD,                 //FTP上传
	WM_HTTPUPLOAD,                //HTTP上传
	WM_FTPUPLOAD_OK,              //FTP方式上传成功
	WM_HTTPLOAD_OK,               //HTTP方式上传成功
	WM_FTPUOLOAD_FAILD,            //FTP方式上传失败
	WM_HTTPUOLOAD_FAILD
};

#define UPLOAD_LOG_ERROR(fmt, ...)		       hlog_format(HLOG_LEVEL_ERROR, "upload", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define UPLOAD_LOG_WARN(fmt, ...)               hlog_format(HLOG_LEVEL_WARN, "upload", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define UPLOAD_LOG_INFO(fmt, ...)               hlog_format(HLOG_LEVEL_INFO, "upload", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define UPLOAD_LOG_DEBUG(fmt, ...)              hlog_format(HLOG_LEVEL_DEBUG, "upload", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define UPLOAD_LOG_TRACE(fmt, ...)              hlog_format(HLOG_LEVEL_TRACE, "upload", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  



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
