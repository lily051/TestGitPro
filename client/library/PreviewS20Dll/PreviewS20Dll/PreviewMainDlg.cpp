// PreviewMainDlg.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "PreviewS20Dll.h"
#include "PreviewMainDlg.h"


// CPreviewMainDlg 对话框

IMPLEMENT_DYNAMIC(CPreviewMainDlg, CDialog)

CPreviewMainDlg::CPreviewMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewMainDlg::IDD, pParent)
    , m_bIsWndInit(FALSE)
    , m_pPlayviewWnd(NULL)
{

}

CPreviewMainDlg::~CPreviewMainDlg()
{
    m_pPlayviewWnd = NULL;
}

void CPreviewMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewMainDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPreviewMainDlg 消息处理程序

BOOL CPreviewMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CreateChildWnd();

    SetWinPos();

    m_bIsWndInit = TRUE;

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CPreviewMainDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (m_bIsWndInit)
    {
        SetWinPos();
    }
}

void CPreviewMainDlg::OnDestroy()
{
    // TODO: 在此处添加消息处理程序代码
    DeleteChildWnd();

    CDialog::OnDestroy();
}

/** @fn         CreateChildWnd
*  @brief      创建预览子窗口
*  @return     void
*/
void CPreviewMainDlg::CreateChildWnd()
{
    if (m_pPlayviewWnd == NULL)
    {
        m_pPlayviewWnd = new CPlayViewWnd;
    }
    if (m_pPlayviewWnd != NULL)
    {
        BOOL bRet = FALSE;
        bRet = m_pPlayviewWnd->CreateWnd(CRect(0,0,20,20),this,ID_WND_PLAYVIEWWND,0,0);
        //if (bRet && theApp.m_pBWCtrlManage != NULL)
        //{
        //    theApp.m_pBWCtrlManage->RegisterWnd(m_pPlayviewWnd->GetSafeHwnd());
        //}
    }
    //if (m_pLogManage == NULL)
    //{
    //    m_pLogManage = new CLogManage;
    //}
    //if (m_pLogManage != NULL)
    //{
    //    m_pLogManage->Init();
    //}
}

/** @fn         DeleteChildWnd
*  @brief      销毁预览子窗口
*  @return     void
*/
void CPreviewMainDlg::DeleteChildWnd()
{
    if (m_pPlayviewWnd != NULL)
    {
        m_pPlayviewWnd->DestroyWindow();
        delete m_pPlayviewWnd;
        m_pPlayviewWnd = NULL;
    }
    //if (m_pLogManage != NULL)
    //{
    //    m_pLogManage->UnInit();
    //    delete m_pLogManage;
    //    m_pLogManage = NULL;
    //}
}

/** @fn         SetPreviewLayoutMode
*  @brief      设置预览窗口画面分割模式
*  @param[IN]  nLayoutMode: 画面分割模式
*  @return     TRUE: 成功 FALSE: 失败
*/
BOOL CPreviewMainDlg::SetPreviewLayoutMode(int nLayoutMode)
{
    if (m_pPlayviewWnd != NULL)
    {
        return m_pPlayviewWnd->SetLayoutMode(nLayoutMode);
    }

    return FALSE;
}

/** @fn         StartPreview
*  @brief      在选中的窗口中开始预览
*  @param[IN]  struPreviewInfo: 预览参数信息
*  @return     TRUE: 成功 FALSE: 失败
*/
BOOL CPreviewMainDlg::StartPreview(const PREVIEW_CAMINFO &struPreviewInfo)
{
    if (m_pPlayviewWnd != NULL)
    {
        return m_pPlayviewWnd->StartPreview(struPreviewInfo);
    }

    return FALSE;
}

/** @fn         SetWinPos
*  @brief      设置预览窗口位置
*  @return     void
*/
void CPreviewMainDlg::SetWinPos()
{
    CRect rcWnd;
    GetClientRect(&rcWnd);

    // 预览窗口位置
    if (m_pPlayviewWnd != NULL)
    {
        m_pPlayviewWnd->MoveWindow(rcWnd);
    }
}

/** @fn         StopPreview
*  @brief      停止预览
*  @return     TRUE: 成功 FALSE: 失败
*/
BOOL CPreviewMainDlg::StopPreview()
{
    if (m_pPlayviewWnd != NULL)
    {
        m_pPlayviewWnd->StopPreview();

        return TRUE;
    }

    return FALSE;
}

/** @fn         StartRecord
*  @brief      开始录像
*  @return     摄像机ID
*/
BOOL CPreviewMainDlg::StartRecord()
{
	if (m_pPlayviewWnd != NULL)
	{
		return m_pPlayviewWnd->StartRecord();
	}
;
	return FALSE;
}

BOOL CPreviewMainDlg::StopRecord()
{
	if (m_pPlayviewWnd != NULL)
	{
		return m_pPlayviewWnd->StopRecord();
	}

	return FALSE;
}

BOOL CPreviewMainDlg::GetRecordFile(RECOED_FILE_INFO& recordFile)
{
	if (m_pPlayviewWnd != NULL)
	{
		return m_pPlayviewWnd->GetRecordFile(recordFile);
	}

	return FALSE;
}

BOOL CPreviewMainDlg::SetEventFunc(PV_EVENT_FUNC Func)
{
	if (m_pPlayviewWnd != NULL)
	{
		return m_pPlayviewWnd->SetEventFunc(Func);
	}

	return FALSE;
}

BOOL CPreviewMainDlg::SetShowString( const CString &strBusinessInfo )
{
	if (m_pPlayviewWnd != NULL)
	{
		return m_pPlayviewWnd->SetShowString(strBusinessInfo);
	}
	return FALSE;
}

BOOL CPreviewMainDlg::OpenSound()
{
	if (m_pPlayviewWnd != NULL)
	{
		return m_pPlayviewWnd->OpenSound();
	}

	return FALSE;
}
