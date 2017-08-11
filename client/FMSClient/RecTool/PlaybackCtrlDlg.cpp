// PlaybackCtrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RecTool.h"
#include "PlaybackCtrlDlg.h"


// CPlaybackCtrlDlg 对话框

IMPLEMENT_DYNAMIC(CPlaybackCtrlDlg, CFDialog)

CPlaybackCtrlDlg::CPlaybackCtrlDlg(CWnd* pParent /*=NULL*/)
: CFDialog(CPlaybackCtrlDlg::IDD, pParent)
,m_hPlayback(0)
{
	PBSD_Init();
}

CPlaybackCtrlDlg::~CPlaybackCtrlDlg()
{
}

void CPlaybackCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlaybackCtrlDlg, CFDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPlaybackCtrlDlg 消息处理程序

BOOL CPlaybackCtrlDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();

	MoveCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPlaybackCtrlDlg::InitCtrl(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT_DLG);
	vecMargin.push_back(MARGIN_TOP_DLG);
	vecMargin.push_back(MARGIN_RIGHT_DLG);
	vecMargin.push_back(MARGIN_BOTTOM_DLG);
	SetMargin(vecMargin);

	SetObjectName(_T("dialog"));

	m_hPlayback = PBSD_Create(this->GetSafeHwnd());
	if (NULL == m_hPlayback)
	{
		return;
	}
	PLAYBACK_PARAM playbackParam;
	playbackParam.nPicType = 1;
	playbackParam.nShowFileWnd = 1;
	PBSD_SetParameter(playbackParam);

	PBSD_ShowWindow(m_hPlayback, TRUE);
}

void CPlaybackCtrlDlg::MoveCtrl(void)
{
	CRect rcClient(PLAYBACK_WND_SIZE);
	MoveWindow(rcClient);

	CenterWindow();

	CRect rcCtrl(rcClient);
	rcCtrl.bottom = rcCtrl.bottom - PLAYBACK_WND_TOOLBAR_HEIGHT;
	PBSD_MoveWindow(m_hPlayback, rcCtrl);
}

void CPlaybackCtrlDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (!PBSD_StopPlayByWndNo(m_hPlayback, 0))
	{
		OutputDebugString(_T("PBSD_StopPlayByWndNo Failed!"));
	}

	if (!PBSD_Destroy(m_hPlayback))
	{
		OutputDebugString(_T("PBSD_Destroy Failed!"));
	}

	if (PBSD_UnInit())
	{
		OutputDebugString(_T("PBSD_UnInit Failed!"));
	}

	return;
}
//0:无文件列表；1：有文件列表
BOOL CPlaybackCtrlDlg::StartPlayBack(int nFlag, CString strRecName)
{
	BOOL bRet = PBSD_StopPlayByWndNo(m_hPlayback, 0);
	bRet = PBSD_LocalPlayback(m_hPlayback, nFlag, strRecName);
	return bRet;
}

void CPlaybackCtrlDlg::OnCancel()
{
	CDialog::OnCancel();

	if (!PBSD_StopPlayByWndNo(m_hPlayback, 0))
	{
		OutputDebugString(_T("PBSD_StopPlayByWndNo Failed!"));
	}
}