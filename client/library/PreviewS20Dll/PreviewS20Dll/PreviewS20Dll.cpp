// PreviewS20Dll.cpp : 定义 DLL 的初始化例程。
//
//lint -library
#include "stdafx.h"
#include "PreviewS20Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CPreviewS20DllApp

BEGIN_MESSAGE_MAP(CPreviewS20DllApp, CWinApp)
END_MESSAGE_MAP()


// CPreviewS20DllApp 构造

CPreviewS20DllApp::CPreviewS20DllApp()
: m_nUIStyle(0)
, m_hResInstance(NULL)
, m_pParentWnd(NULL)
, m_nPreviewModel(0)
, m_nBtnRights(0)
, m_bUSBExit(FALSE)
, m_lUSBPort(-1)
{
    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPreviewS20DllApp 对象

CPreviewS20DllApp theApp;
CResourceLoader g_ResourceLoader;


// CPreviewS20DllApp 初始化

BOOL CPreviewS20DllApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

    OutputDebugString(_T("-----------------------------------CPreviewS20DllApp::InitInstance begin-----------------------------------"));

    m_strModulePath = AppFun_GetAppPath();

    AppFun_GetLocalIPAddr(m_strLocalIPAddr);

    OutputDebugString(_T("-----------------------------------CPreviewS20DllApp::InitInstance end-----------------------------------"));

	return TRUE;
}

int CPreviewS20DllApp::ExitInstance()
{
    OutputDebugString(_T("-----------------------------------CPreviewS20DllApp::ExitInstance begin-----------------------------------"));

    OutputDebugString(_T("...Free SkinRes start..."));
    if (theApp.m_hResInstance != NULL)
    {
        BOOL bRet = FALSE;
        bRet = ::FreeLibrary(theApp.m_hResInstance);
        DWORD dwError = GetLastError();
        if (!bRet)
        {
            CString csTemp;
            csTemp.Format(_T("...Free SkinRes failed: %u..."), dwError);
            PV_ERROR("%s", CStringW2A(csTemp));
            OutputDebugString(csTemp);
        }
        OutputDebugString(_T("...Free SkinRes successfully..."));
    }
    OutputDebugString(_T("...Free SkinRes stop..."));

    OutputDebugString(_T("-----------------------------------CPreviewS20DllApp::ExitInstance end-----------------------------------"));
    return CWinApp::ExitInstance();
}

/** @fn         GetUIColor
*  @brief      获取指定类型颜色值
*  @param[IN]  nClrType: 颜色类型，参见_ENUM_UI_COLOR_TYPE
*  @return     色值
*/
COLORREF CPreviewS20DllApp::GetUIColor(int nClrType)
{
    if (m_nUIStyle == PV_SKIN_STYLE_BLUE)
    {
        switch (nClrType)
        {
        case UI_COLOR_TYPE_LIGHT:
            {
                return UI_BLUE_BKGND_LIGHT;
            }
        case UI_COLOR_TYPE_DEEP:
            {
                return UI_BLUE_BKGND_DEEP;
            }
        case UI_COLOR_TYPE_PV:
            {
                return UI_BLUE_BKGND_PV;
            }
        case UI_COLOR_TYPE_RW:
            {
                return UI_BLUE_BKGND_RW;
            }
        case UI_COLOR_TYPE_RW_BORDER:
            {
                return UI_BLUE_RW_BORDER;
            }
        case UI_COLOR_TYPE_UIDIALOG:
            {
                return UI_BLUE_UIDIALOG_BKGND;
            }
        case UI_COLOR_TYPE_UIDIALOG_TITLE:
            {
                return UI_BLUE_UIDIALOG_TITLE;
            }
        case UI_COLOR_TYPE_STATISTIC:
            {
                return UI_BLUE_STATISTIC_BKGND;
            }
        case UI_COLOR_TYPE_TREE_BKG:
            {
                return UI_BLUE_TREE_BKGND;
            }
        case UI_COLOR_TYPE_LIST_BKG:
            {
                return UI_BLUE_LIST_BKGND;
            }
        case UI_COLOR_TYPE_LIST_LIGHT:
            {
                return UI_BLUE_LIST_ITEM_LIGHT;
            }
        case UI_COLOR_TYPE_LIST_DEEP:
            {
                return UI_BLUE_LIST_ITEM_DEEP;
            }
        default:
            break;
        }
    }
    else
    {
        switch (nClrType)
        {
        case UI_COLOR_TYPE_LIGHT:
            {
                return UI_RED_BKGND_LIGHT;
            }
        case UI_COLOR_TYPE_DEEP:
            {
                return UI_RED_BKGND_DEEP;
            }
        case UI_COLOR_TYPE_PV:
            {
                return UI_RED_BKGND_PV;
            }
        case UI_COLOR_TYPE_RW:
            {
                return UI_RED_BKGND_RW;
            }
        case UI_COLOR_TYPE_RW_BORDER:
            {
                return UI_RED_RW_BORDER;
            }
        case UI_COLOR_TYPE_UIDIALOG:
            {
                return UI_RED_UIDIALOG_BKGND;
            }
        case UI_COLOR_TYPE_UIDIALOG_TITLE:
            {
                return UI_RED_UIDIALOG_TITLE;
            }
        case UI_COLOR_TYPE_STATISTIC:
            {
                return UI_RED_STATISTIC_BKGND;
            }
        case UI_COLOR_TYPE_TREE_BKG:
            {
                return UI_RED_TREE_BKGND;
            }
        case UI_COLOR_TYPE_LIST_BKG:
            {
                return UI_RED_LIST_BKGND;
            }
        case UI_COLOR_TYPE_LIST_LIGHT:
            {
                return UI_RED_LIST_ITEM_LIGHT;
            }
        case UI_COLOR_TYPE_LIST_DEEP:
            {
                return UI_RED_LIST_ITEM_DEEP;
            }
        default:
            break;
        }
    }
    return 0;
}

/** @fn         ShowCustomMsg
*  @brief      显示自定义消息
*  @param[IN]  strCaption: 消息标题
*  @param[IN]  strContent: 消息内容
*  @param[IN]  nMsgType: 消息类型(0:普通文字消息 1:超链接消息)
*  @return     void
*/
void CPreviewS20DllApp::ShowCustomMsg(const CString &strCaption, const CString &strContent, int nMsgType/* = 0*/)
{
    MSG_DETAIL md;
    md.strCaption = strCaption;
    md.strMsg = strContent;
    md.tdp.byShowMode = 1;
    md.tdp.uBlinkTime = 0;
    md.tdp.uMsgType = nMsgType;
    ShowTipMsg(md);
}

/** @fn         GetOperateRight
*  @brief      获取用户对应的操作权限
*  @param[IN]  nOperType: 权限类型
*  @return     TRUE: 有，FALSE: 无
*/
BOOL CPreviewS20DllApp::GetOperateRight(int nOperType)
{
    return (nOperType & theApp.m_stPreviewParam.nOperRight);
}

/** @fn         GetLocalCfg
*  @brief      获取预览本地参数配置
*  @param[IN]  stLocalCfg: 参数配置信息
*  @return     void
*/
void CPreviewS20DllApp::GetLocalCfg(LOCAL_CFG &stLocalCfg)
{
    LONG nMaxRecSize = m_stLocalCfg.stRecordInfo.nRecordFileSize;
	LONG nCapType = m_stLocalCfg.stSnapInfo.nCappicMode;
	if (nMaxRecSize < 0L || nMaxRecSize > 8L)
	{
		nMaxRecSize = PREVIEW_RECORDFILESIZE_512M;
		m_stLocalCfg.stRecordInfo.nRecordFileSize = nMaxRecSize;
	}
	if (nCapType < 0L || nCapType > 1L)
	{
		nCapType = PREVIEW_SNAPMODE_JPG;
		m_stLocalCfg.stSnapInfo.nCappicMode = nCapType;
	}
    stLocalCfg = m_stLocalCfg;
}
