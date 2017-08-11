
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


#include "file_info.h"
#include "StringConverter.h"
#include "StringChange.h"
#include "../public/HLOG/lib/include/hlog.h"
#pragma comment(lib,"Version.lib") 

//�����ļ���Ϣ
typedef struct FileInfo
{
	CString strName;     //�����ļ���
	CString strVersion;  
	CString strPath;     //��װ·��
	int nFlag;           //������־

	int nFileCount;      //�ļ����
	ULONGLONG size;      //URL �ļ���С
	CString Hash;        //�ļ�MD5ֵ
	CString Subcatalog;  //�ļ���Ŀ¼
	CString Url;         //�ļ�URL

}FILEINFO;

// �����ļ���MD5ժҪ��
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


