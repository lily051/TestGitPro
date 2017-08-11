// PreviewS20Dll.cpp : ���� DLL �ĳ�ʼ�����̡�
//
//lint -library
#include "stdafx.h"
#include "PreviewS20Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CPreviewS20DllApp

BEGIN_MESSAGE_MAP(CPreviewS20DllApp, CWinApp)
END_MESSAGE_MAP()


// CPreviewS20DllApp ����

CPreviewS20DllApp::CPreviewS20DllApp()
: m_nUIStyle(0)
, m_hResInstance(NULL)
, m_pParentWnd(NULL)
, m_nPreviewModel(0)
, m_nBtnRights(0)
, m_bUSBExit(FALSE)
, m_lUSBPort(-1)
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPreviewS20DllApp ����

CPreviewS20DllApp theApp;
CResourceLoader g_ResourceLoader;


// CPreviewS20DllApp ��ʼ��

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
*  @brief      ��ȡָ��������ɫֵ
*  @param[IN]  nClrType: ��ɫ���ͣ��μ�_ENUM_UI_COLOR_TYPE
*  @return     ɫֵ
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
*  @brief      ��ʾ�Զ�����Ϣ
*  @param[IN]  strCaption: ��Ϣ����
*  @param[IN]  strContent: ��Ϣ����
*  @param[IN]  nMsgType: ��Ϣ����(0:��ͨ������Ϣ 1:��������Ϣ)
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
*  @brief      ��ȡ�û���Ӧ�Ĳ���Ȩ��
*  @param[IN]  nOperType: Ȩ������
*  @return     TRUE: �У�FALSE: ��
*/
BOOL CPreviewS20DllApp::GetOperateRight(int nOperType)
{
    return (nOperType & theApp.m_stPreviewParam.nOperRight);
}

/** @fn         GetLocalCfg
*  @brief      ��ȡԤ�����ز�������
*  @param[IN]  stLocalCfg: ����������Ϣ
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
