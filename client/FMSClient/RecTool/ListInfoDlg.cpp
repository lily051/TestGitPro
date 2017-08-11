// ListInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RecTool.h"
#include "ListInfoDlg.h"


// CListInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CListInfoDlg, CDialog)

CListInfoDlg::CListInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListInfoDlg::IDD, pParent)
{
	
}

CListInfoDlg::~CListInfoDlg()
{
}

void CListInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FILELIST, m_staticListInfo);
	DDX_Control(pDX, IDC_BTN_OPENFOLDER, m_btnOpenFolder);
}


BEGIN_MESSAGE_MAP(CListInfoDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_OPENFOLDER, &CListInfoDlg::OnBnClickedBtnOpenfolder)
END_MESSAGE_MAP()


// CListInfoDlg ��Ϣ�������

BOOL CListInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(57, 57, 57));

	return TRUE;
}

void CListInfoDlg::InitCtrl()
{
	m_staticListInfo.SetTextColor(FUi::White);
	m_staticListInfo.SetTextFont(YAHEI_18_SB);

	m_btnOpenFolder.SetObjectName(_T("btn_black"));
	m_btnOpenFolder.SetHoverCursor(LoadCursor(NULL, IDC_HAND)); 
	m_btnOpenFolder.SetTextFont(FUi::Normal | FUi::Hovered | FUi::Pressed, YAHEI_18_SB);
	m_btnOpenFolder.SetTextColor(FUi::Normal, RGB(63, 133, 228));
	m_btnOpenFolder.SetTextColor(FUi::Hovered | FUi::Pressed, RGB(0x2D, 0x5F, 0xA4));
	m_btnOpenFolder.SetTextAlign(DT_RIGHT);
}

void CListInfoDlg::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcText(rcClient);
	rcText.left = rcClient.left + 10;
	rcText.top = rcClient.top + 8;
	rcText.right = rcClient.left + 100;
	rcText.bottom = rcText.top + 20;
	m_staticListInfo.MoveWindow(rcText);

	CRect rcBtnOpenFolder(rcClient);
	rcBtnOpenFolder.left = rcClient.right  -230;
	rcBtnOpenFolder.right = rcBtnOpenFolder.left + 100;
	rcBtnOpenFolder.bottom = rcBtnOpenFolder.top + 30;
	m_btnOpenFolder.MoveWindow(rcBtnOpenFolder);
}

BOOL CListInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrl();

	MoveCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CListInfoDlg::OnBnClickedBtnOpenfolder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFolder = theApp.m_strAppCapPath;

	//����Դ������
	HINSTANCE hInstance = ShellExecute(NULL, _T("open"), strFolder, NULL, NULL, SW_SHOW);
}
