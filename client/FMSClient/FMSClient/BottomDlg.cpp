// BottomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FMSClient.h"
#include "BottomDlg.h"


// CBottomDlg �Ի���

IMPLEMENT_DYNAMIC(CBottomDlg, CDialog)

CBottomDlg::CBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBottomDlg::IDD, pParent)
	, m_nAppState(-1)
{

}

CBottomDlg::~CBottomDlg()
{
}

void CBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COPYRIGHT, m_staticCopyright);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_staticVersion);
	DDX_Control(pDX, IDC_BTN_STATE, m_btnState);
}


BEGIN_MESSAGE_MAP(CBottomDlg, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CBottomDlg ��Ϣ�������

BOOL CBottomDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(43, 47, 70));  //���ñ���ɫ

	return TRUE;
}

BOOL CBottomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCtrl();

	MoveCtrl();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBottomDlg::InitCtrl(void)
{
	m_staticCopyright.SetTextFont(SONG_12_N);
	m_staticVersion.SetTextFont(SONG_12_N);
	m_staticCopyright.SetTextColor(RGB(0x78, 0x7F, 0x8C));
	m_staticVersion.SetTextColor(RGB(0x78, 0x7F, 0x8C));
	m_btnState.SetIcon(_T("normal"), 1);
	m_btnState.EnableWindow(FALSE);
	m_btnState.SetBgColor(RGB(43, 47, 70));
	m_btnState.SetTextFont(FUi::Normal | FUi::Hovered | FUi::Disabled | FUi::Focus | FUi::Error | FUi::Default, SONG_12_N);
	m_btnState.SetTextColor(FUi::Normal | FUi::Hovered | FUi::Disabled | FUi::Focus | FUi::Error | FUi::Default, RGB(0x78, 0x7F, 0x8C));
	m_btnState.SetTextAlign(DT_CENTER);
}

void CBottomDlg::MoveCtrl(void)
{
	m_staticCopyright.MoveWindow(10, 8, 300, 12);
	m_staticVersion.MoveWindow(775, 8, 120, 12);
	m_btnState.MoveWindow(600, 8, 175, 14);
}

void CBottomDlg::SetAppState(int nAppState)
{
	if (m_nAppState == nAppState)
	{
		return;
	}

	if (APP_NORMAL == nAppState)
	{
		m_btnState.SetIcon(_T("normal"), 1);
		m_btnState.SetWindowText(_T("�ѳɹ���������ƽ̨"));
	}
	else if (APP_WARN == nAppState)
	{
		m_btnState.SetIcon(_T("normal"), 1);
		m_btnState.SetWindowText(_T("�ѳɹ���������ƽ̨"));
	}
	else if (APP_ERROR == nAppState)
	{
		m_btnState.SetIcon(_T("error"), 1);
		m_btnState.SetWindowText(_T("����������ƽ̨ʧ��"));
	}
	else
	{
		return;
	}

	m_nAppState = nAppState;
}

int CBottomDlg::GetAppState()
{
	return m_nAppState;
}