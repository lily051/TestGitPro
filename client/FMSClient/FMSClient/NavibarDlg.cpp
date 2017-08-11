// NavibarDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FMSClient.h"
#include "NavibarDlg.h"
#include "AppDef.h"
#include "MultiLang/lClient.h"

// CNavibarDlg �Ի���
#define  TIMER_BLINK 1000
#define BLINK_INTERVAL 500

#define CLOSE_BTN_ID 1000

#define HOME_BTN_WIDTH  112  // �϶���ͼƬ���
#define HOME_BTN_HEIGHT 40   // �϶���ͼƬ�߶�
#define BTN_WIDTH  140  // �϶���ͼƬ���
#define BTN_HEIGHT 40   // �϶���ͼƬ�߶�



IMPLEMENT_DYNAMIC(CNavibarDlg, CDialog)

CNavibarDlg::CNavibarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNavibarDlg::IDD, pParent)
	,m_pBtnHome(NULL)
	,m_pBtnMore(NULL)
	,m_pImgBkg(NULL)
{

}

CNavibarDlg::~CNavibarDlg()
{
}

void CNavibarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNavibarDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_MESSAGE(WM_ADD_MODULE, &CNavibarDlg::OnMsgAddModule)
END_MESSAGE_MAP()


// CNavibarDlg ��Ϣ�������

BOOL CNavibarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pImgBkg = FUiGetRes(_T("navi_bkg"));

	ModuleOper(MODULE_ADD, MENUID_MORE);

	LOGFONT lfFont = {0};
	_tcscpy_s(lfFont.lfFaceName, _T("΢���ź�"));
	lfFont.lfHeight = 18;
	lfFont.lfWeight = FW_NORMAL;
	RenderEngine->AddFont(YAHEI_18_N, lfFont);

	lfFont.lfWeight = 20;
	RenderEngine->AddFont(YAHEI_20_N, lfFont);

	ModuleOper(MODULE_ADD, MENUID_HOME);

	m_imgListDrag.Create(BTN_WIDTH - 40, BTN_HEIGHT - 5, ILC_COLOR32 | ILC_MASK, 1, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNavibarDlg::ModuleOper(int nOper, int nComponentID)
{
	switch(nOper)
	{
	case MODULE_ADD:
		{
			PostMessage(WM_ADD_MODULE, nComponentID, 0);
		}
		break;
	case MODULE_DEL:
		{
			PostMessage(WM_DEL_MODULE, nComponentID, 0);
		}
		break;
	case MODULE_HEAD:
		{
			PostMessage(WM_HEAD_MODULE, nComponentID, 0);
		}
		break;
	default:
		break;
	}
}

LRESULT CNavibarDlg::OnMsgAddModule(WPARAM wParam, LPARAM lParam)
{
	int nComponentID = (int)wParam;
	bool bSecondWnd = false;
	if (NULL != lParam)
	{
		bSecondWnd = true;
	}
	_AddModule(nComponentID, bSecondWnd);

	return 0;
}

void CNavibarDlg::_AddModule(int nComponentID, bool bSecondWnd)
{
	int nBtnState = FUi::Normal | FUi::Hovered | FUi::Pressed | FUi::Disabled;
	if (MENUID_HOME == nComponentID)
	{
		if (m_pBtnHome)
		{
			return;
		}
		m_pBtnHome = new CFButton;
		if (NULL == m_pBtnHome)
		{
			DebugMsg("Create Home Btn Fail:%d\n", GetLastError());
			return;
		}
		m_pBtnHome->Create(HOME_BTN_HOMEPAGE, WS_CHILD | WS_VISIBLE, CRect(0, 0, HOME_BTN_WIDTH, HOME_BTN_HEIGHT), this, MENUID_HOME);
		m_pBtnHome->SetObjectName(_T("navi_btn"));
		m_pBtnHome->SetIcon(_T("navi_home_icon"));
		m_pBtnHome->SetTextFont(nBtnState, YAHEI_20_N);
		m_pBtnHome->SetTextColor(nBtnState, FUi::White);
		m_pBtnHome->SetTextAlign(DT_CENTER);
	}
	else if (MENUID_MORE == nComponentID)
	{
		if (m_pBtnMore)
		{
			return;
		}
		m_pBtnMore = new CFButton;
		if (NULL == m_pBtnMore)
		{
			FMSCLIENT_ERROR("Create More Btn fail:%d\n", GetLastError());
			return;
		}
		m_pBtnMore->Create(_T(""), WS_CHILD, CRect(0, 0, 50, 40), this, MENUID_MORE);
		m_pBtnMore->SetObjectName(_T("navi_more"));
		m_pBtnMore->SetIcon(_T("navi_more_icon"));
	}
}


BOOL CNavibarDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::PreTranslateMessage(pMsg);
}

void CNavibarDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

BOOL CNavibarDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialog::OnEraseBkgnd(pDC);
}

void CNavibarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnTimer(nIDEvent);
}

void CNavibarDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}
