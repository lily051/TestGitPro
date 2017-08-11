
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



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#include "FtpHttpUpload.h"
#include "UploadThread.h"
#include "StringChange.h"
#include "zconf.h"

#include "../HLOG/lib/include/hlog.h"
#pragma comment(lib, "../Release/hlog.lib")

#define PARA_KEY_CHECKURL _T("CheckURL=")        //URL
#define PARA_KEY_CHECLOGPATH _T("CheckPath=")    //��־·��
#define PARA_KEY_CHECFLAG _T("CheckFlag=")       //�ϴ���ʽ
#define PARA_KEY_CHECAPPNAME _T("CheckAppName=") //���øó��������


enum {
	WM_FILE_EXIST = WM_USER + 1,  //�ļ��Ѿ�����
	WM_UPLOAD_OK,                 //�ϴ��ļ��ɹ�
	WM_UPLOAD_PROCESS,            //�ϴ�����
	WM_FTPUPLOAD,                 //FTP�ϴ�
	WM_HTTPUPLOAD,                //HTTP�ϴ�
	WM_FTPUPLOAD_OK,              //FTP��ʽ�ϴ��ɹ�
	WM_HTTPLOAD_OK,               //HTTP��ʽ�ϴ��ɹ�
	WM_FTPUOLOAD_FAILD,            //FTP��ʽ�ϴ�ʧ��
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
