// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows ͷ�ļ�:
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


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
