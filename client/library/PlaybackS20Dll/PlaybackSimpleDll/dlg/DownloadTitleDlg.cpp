// DownloadTitleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadTitleDlg.h"


// CDownloadTitleDlg �Ի���

IMPLEMENT_DYNAMIC(CDownloadTitleDlg, CDialog)

CDownloadTitleDlg::CDownloadTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadTitleDlg::IDD, pParent)
{
	m_bMove = FALSE;
	m_bkBrush.CreateSolidBrush(UI_TREE_BKCOLOR);

	m_nDowningCount = 0;
	m_nDownSuccessCount = 0;
	m_nDownFailedCount = 0;
}

CDownloadTitleDlg::~CDownloadTitleDlg()
{
	try
	{
		m_bkBrush.DeleteObject();
	}
	catch (...)
	{

	}

}

void CDownloadTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DOWNTITLE, m_btnTitle);
	DDX_Control(pDX, IDC_STATIC_DOWNLOADING, m_staDownloading);
	DDX_Control(pDX, IDC_STATIC_DOWNSUCCESS, m_staDownSuccess);
	DDX_Control(pDX, IDC_STATIC_DOWNFAILED, m_staDownFailed);
}


BEGIN_MESSAGE_MAP(CDownloadTitleDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_DOWNLOADING, &CDownloadTitleDlg::OnStnClickedStaticDownloading)
	ON_STN_CLICKED(IDC_STATIC_DOWNSUCCESS, &CDownloadTitleDlg::OnStnClickedStaticDownsuccess)
	ON_STN_CLICKED(IDC_STATIC_DOWNFAILED, &CDownloadTitleDlg::OnStnClickedStaticDownfailed)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDownloadTitleDlg ��Ϣ�������

BOOL CDownloadTitleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrl();

	m_bMove = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDownloadTitleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}

}

HBRUSH CDownloadTitleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_bkBrush;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

/** @fn InitCtrl
*   @brief ��ʼ���ؼ�
*   @param[in]  NULL
*   @param[out] NULL
*   @return NULL
*/
void CDownloadTitleDlg::InitCtrl()
{
	m_btnTitle.SetWindowText(_T("�����б�"));
	m_btnTitle.Load(IDB_PNG_TITLE);

	m_staDownloading.Load(IDB_PNG_ONBTN_DOWNLOAD);
	m_staDownSuccess.Load(IDB_PNG_DOWNSUCCESS);
	m_staDownFailed.Load(IDB_PNG_ONBTN_DELETE);

	m_staDownloading.SetStaticSel(TRUE);

	SetStaText();

}

/** @fn MoveCtrl
*   @brief �ƶ��ؼ�
*   @param[in]  NULL
*   @param[out] NULL
*   @return NULL
*/
void CDownloadTitleDlg::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nSpace = 1;
	CRect rcBtnTitle;
	rcBtnTitle.left = rcClient.left + nSpace;
	rcBtnTitle.top = rcClient.top + nSpace;
	rcBtnTitle.right = rcClient.right - 1;
	rcBtnTitle.bottom = rcBtnTitle.top + 26;
	m_btnTitle.MoveWindow(rcBtnTitle);

	int nStaticHeight = 32;

	CRect rcStatic(rcBtnTitle);
	rcStatic.top = rcBtnTitle.bottom;
	rcStatic.bottom = rcStatic.top + nStaticHeight;
	m_staDownloading.MoveWindow(rcStatic);

	rcStatic.top = rcStatic.bottom;
	rcStatic.bottom = rcStatic.top + nStaticHeight;
	m_staDownSuccess.MoveWindow(rcStatic);

	rcStatic.top = rcStatic.bottom;
	rcStatic.bottom = rcStatic.top + nStaticHeight;
	m_staDownFailed.MoveWindow(rcStatic);

}

/** @fn SetStaText 
*   @brief ����static�ؼ�����
*   @param[in]  NULL
*   @param[out] NULL
*   @return NULL
*/
void CDownloadTitleDlg::SetStaText()
{
	CString staText;

	staText.Format(_T("��������(%d)"), m_nDowningCount);
	m_staDownloading.SetText(staText);

	staText.Format(_T("������(%d)"), m_nDownSuccessCount);
	m_staDownSuccess.SetText(staText);

	staText.Format(_T("����ʧ��(%d)"), m_nDownFailedCount);
	m_staDownFailed.SetText(staText);

	Invalidate();
	UpdateWindow();
}

void CDownloadTitleDlg::OnStnClickedStaticDownloading()
{
	m_staDownloading.SetStaticSel(TRUE);
	m_staDownSuccess.SetStaticSel(FALSE);
	m_staDownFailed.SetStaticSel(FALSE);

	::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLIST_SELECT, IDC_STATIC_DOWNLOADING, 0);
}

void CDownloadTitleDlg::OnStnClickedStaticDownsuccess()
{
	m_staDownloading.SetStaticSel(FALSE);
	m_staDownSuccess.SetStaticSel(TRUE);
	m_staDownFailed.SetStaticSel(FALSE);

	::PostMessage(this->GetParent()->GetSafeHwnd(), MSG_DOWNLIST_SELECT, IDC_STATIC_DOWNSUCCESS, 0);
}

void CDownloadTitleDlg::OnStnClickedStaticDownfailed()
{
	m_staDownloading.SetStaticSel(FALSE);
	m_staDownSuccess.SetStaticSel(FALSE);
	m_staDownFailed.SetStaticSel(TRUE);

	::PostMessage(this->GetParent()->GetSafeHwnd(), MSG_DOWNLIST_SELECT, IDC_STATIC_DOWNFAILED, 0);
}


void CDownloadTitleDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, UI_TREE_BKCOLOR);

}
