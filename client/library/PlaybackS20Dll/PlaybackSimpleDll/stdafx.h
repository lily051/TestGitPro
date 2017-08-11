// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//lint -library
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#define PBSD_API_EXPORT

#include <gdiplus.h>
using namespace Gdiplus; 

//创建路径使用
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
