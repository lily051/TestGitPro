// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

//lint -library
//lint -e322
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

#include <afxsock.h>            // MFC �׽�����չ

#include <windows.h>
using namespace std;

//GDI+��ͷ�ļ�
#include <gdiplus.h>
using namespace Gdiplus;

#include "resource.h"

#include <hlog.h>

#include <StreamLayerAPI.h>
#include <GeneralPlayerAPI.h>
#include <CameraDSAPI.h>
#include "USBCamera.h"
#include "MsgBox.h"

#include "util/utils.h"
#include "util/Guard.h"
#include "util/fchlogconfig.h"
#include "util/ResourceLoader.h"
#include "util/StringConverter.h"

#include "include/PVSkinRes.h"
#include "include/PreviewDef.h"

#include "AppFun.h"

#include "PreviewS20API.h"
#include "./include/VideoSvcClientAPI.h"
#include "./include/PhoneSvcClientAPI.h"

#include <vector>
#include <list>
#include <map>

#include "HCNetSDK.h"
//lint +e322