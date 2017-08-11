
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
#include <afxdhtml.h>

using namespace std;

#include <list>
#include <vector>
#include <map>
#include <string>
#include "resource.h"
#include "hlogconfig.h"
#include "FUiControl.h"
#include "hak.h"
#include "utils/Guard.h"
#include "utils/utils.h"
#include "MultiLang/fcTchar.h"
#include "../CommuniMidLayer/CommuniMidLayer.h"


#ifdef _DEBUG
#pragma comment(lib, "../Debug/Login.lib")
#pragma comment(lib, "../Debug/CommuniMidLayer.lib")
#pragma comment(lib, "../../../../public/util/ui/FUiControl/Debug/FUiControl.lib")
#pragma comment(lib, "../../../../public/base/libhak/libhak3/lib32/debug/libhak.lib")
#pragma comment(lib, "../../../../public/base/Google/protobuf/lib/vs2008_debug/libprotobuf.lib")
#pragma comment(lib, "../../../../public/finance/cms_8100/proto/_cms_8100/lib_debug/cms_8100_d.lib")
#else
#pragma comment(lib, "../Release/Login.lib")
#pragma comment(lib, "../Release/CommuniMidLayer.lib")
#pragma comment(lib, "../../../public/util/ui/FUiControl/Release/FUiControl.lib")
#pragma comment(lib, "../../../public/base/libhak3/lib32/release/libhak.lib")
#pragma comment(lib, "../../../public/base/Google/protobuf/lib/vs2008_release/libprotobuf.lib")
#pragma comment(lib, "../../../public/finance/cms_8120/proto/_cms_8120/lib_release/cms_8120.lib")
#endif

#pragma comment(lib, "../../../public/base/HLOG/v2.0/win_32_release/hlog.lib")
#pragma comment(lib, "../../../public/base/HPR/lib/hpr.lib")
#pragma comment(lib, "../../../public/base/HPP/lib/win32/vs2008_release/hpp.lib")
#pragma comment(lib, "../../../public/dev_sdk/HCNetSDK/lib/HCNetSDK.lib")

//Ԥ��
#include "PreviewS20API.h"

//�ط�
#include "PlaybackSimpleAPI.h"



//util����
#include "./utils/utils.h"
#include "utils/html/HtmlParser.h"






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


