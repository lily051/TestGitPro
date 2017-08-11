// PreviewMainDlg.cpp : ʵ���ļ�
//
//lint -library
#include "stdafx.h"
#include "PreviewS20Dll.h"
#include "PreviewMainDlg.h"


// CPreviewMainDlg �Ի���

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


// CPreviewMainDlg ��Ϣ�������

BOOL CPreviewMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CreateChildWnd();

    SetWinPos();

    m_bIsWndInit = TRUE;

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CPreviewMainDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: �ڴ˴������Ϣ����������
    if (m_bIsWndInit)
    {
        SetWinPos();
    }
}

void CPreviewMainDlg::OnDestroy()
{
    // TODO: �ڴ˴������Ϣ����������
    DeleteChildWnd();

    CDialog::OnDestroy();
}

/** @fn         CreateChildWnd
*  @brief      ����Ԥ���Ӵ���
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
*  @brief      ����Ԥ���Ӵ���
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
*  @brief      ����Ԥ�����ڻ���ָ�ģʽ
*  @param[IN]  nLayoutMode: ����ָ�ģʽ
*  @return     TRUE: �ɹ� FALSE: ʧ��
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
*  @brief      ��ѡ�еĴ����п�ʼԤ��
*  @param[IN]  struPreviewInfo: Ԥ��������Ϣ
*  @return     TRUE: �ɹ� FALSE: ʧ��
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
*  @brief      ����Ԥ������λ��
*  @return     void
*/
void CPreviewMainDlg::SetWinPos()
{
    CRect rcWnd;
    GetClientRect(&rcWnd);

    // Ԥ������λ��
    if (m_pPlayviewWnd != NULL)
    {
        m_pPlayviewWnd->MoveWindow(rcWnd);
    }
}

/** @fn         StopPreview
*  @brief      ֹͣԤ��
*  @return     TRUE: �ɹ� FALSE: ʧ��
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
*  @brief      ��ʼ¼��
*  @return     �����ID
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
