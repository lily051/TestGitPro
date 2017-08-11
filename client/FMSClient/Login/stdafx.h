// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows 头文件:
#include <windows.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include "utils.h"
#include "MultiLang/lClient.h"
#include "FUiControl.h"
#include "hak.h"

#ifdef _DEBUG
#pragma comment(lib, "../../../../public/util/ui/FUiControl/Debug/FUiControl.lib")
#pragma comment(lib, "../../../../public/base/libhak/libhak3/lib32/debug/libhak.lib")
#else
#pragma comment(lib, "../../../public/util/ui/FUiControl/Release/FUiControl.lib")
#pragma comment(lib, "../../../public/base/libhak3/lib32/release/libhak.lib")
#endif

#pragma comment(lib, "../../../public/dev_sdk/HCNetSDK/lib/HCNetSDK.lib")
#pragma comment(lib, "../../../public/base/HLOG/v2.0/win_32_release/hlog.lib")


// TODO: 在此处引用程序需要的其他头文件
