// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//lint -library
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#define PBSD_API_EXPORT

#include <gdiplus.h>
using namespace Gdiplus; 

//����·��ʹ��
#include <imagehlp.h>
//imaghlp
#pragma comment(lib, "imagehlp.lib")

#include <string>
#include <map>
#include <set>
#include <vector>
#include <math.h>

#include "PBResource.h"
#include "utils.h"
#include "Guard.h"

// hlog.
#include "hlog.h"
#include "hlogex.h"
#include "PBLog.h"
// sdk
#include "PlaybackSDKApi.h"
#include "PlaybackSdkDef.h"
#include "PlaybackDef.h"
//msgbox
#include "MsgBox.h"
// gpapi.
#include "GeneralPlayerAPI.h"

#include "PlaybackSimpleAPI.h"

#include "ResourceLoader.h"
#include <afxdhtml.h>
extern CResourceLoader g_ResourceLoader;
#define RESOURCELOAD_PNG(id)               g_ResourceLoader.LoadPNG(id)
#define RESOURCELOAD_PNG_EX_HBMP(id,cr)    g_ResourceLoader.ResourceLoad_PNG_EX_HBMP(id,cr)
#define RESOURCELOAD_PNG_EX_HBMPE(id)    g_ResourceLoader.ResourceLoad_PNG_EX_HBMP(id)
#define RESOURCELOAD_PNG_EX_IMG(id)        g_ResourceLoader.LoadPNGToImage(id)
#define RESOURCELOAD_CURSOR(id)            g_ResourceLoader.ResourceLoad_Cursor(id)
#define RESOURCELOAD_BMP(id)               g_ResourceLoader.ResourceLoad_Bmp(id)
#define FROM_PNG(id)                       g_ResourceLoader.LoadPNG(id)
#define RESOURCELOAD_ICON(id)	           g_ResourceLoader.LoadIcon(id)
#define RESOURCELOAD_COLOR(id)			   g_ResourceLoader.ResourceLoad_Color(id)
