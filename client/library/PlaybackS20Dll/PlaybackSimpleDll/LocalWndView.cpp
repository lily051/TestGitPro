// LocalWndView.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalWndView.h"


// CLocalWndView

IMPLEMENT_DYNAMIC(CLocalWndView, CWnd)

CLocalWndView::CLocalWndView()
{
	m_clrBk = UI_PLAYWND_COLOR;
	m_bIsPlaying = FALSE;
	m_bIsPausing = FALSE;
	m_bIsSounding = FALSE;
	m_bIsFullScreening = FALSE;
}

CLocalWndView::~CLocalWndView()
{
}


BEGIN_MESSAGE_MAP(CLocalWndView, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CLocalWndView 消息处理程序



BOOL CLocalWndView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrBk);

	return TRUE;
}

int CLocalWndView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CLocalWndView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CLocalWndView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CLocalWndView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PopupMenu();

	CWnd::OnRButtonDown(nFlags, point);
}

void CLocalWndView::PopupMenu(BOOL bJustClose/* = FALSE*/)
{

	BCMenu menuPopup;
	menuPopup.CreatePopupMenu();

	if (!bJustClose)
	{
		if (m_bIsPlaying)
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_CAPTURE, _T("抓图"));
			menuPopup.AppendMenu(MF_STRING, MENU_ID_SLOW, _T("减速"));
			if (!m_bIsPausing)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("暂停"));
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("播放"));
			}
			
			menuPopup.AppendMenu(MF_STRING, MENU_ID_FAST, _T("加速"));
			if (!m_bIsSounding)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("开启音频"));
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("关闭音频"));
			}
			menuPopup.AppendMenu(MF_SEPARATOR);
			if (!m_bIsFullScreening)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_FULLSCREEN, _T("全屏显示"));
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_FULLSCREEN, _T("退出全屏"));
			}
			menuPopup.AppendMenu(MF_STRING, MENU_ID_STOP, _T("停止"));
		}
	}

	CPoint point;
	GetCursorPos(&point);
	int nCmd = menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, point.x, point.y, this);

	ExcuteSelectOption(nCmd);
}

void CLocalWndView::ExcuteSelectOption(int nCmd)
{
	switch(nCmd)
	{
	case MENU_ID_CAPTURE:       //抓图
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_CAPTURE, 0);
		}
		break;
	case MENU_ID_SLOW:          //减速
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SLOW, 0);
		}
		break;
	case MENU_ID_PLAY:          //播放、暂停
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_PLAY, 0);
		}
		break;
	case MENU_ID_FAST:          //加速
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_FAST, 0);
		}
		break;
	case MENU_ID_OPENSOUND:     //音频
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SOUND, 0);
		}
		break;
	case MENU_ID_FULLSCREEN:    //全屏
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_FULLSCREEN, 0);
		}
		break;
	case MENU_ID_STOP:          //停止
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_STOP, 0);
		}
		break;
	}
}

void CLocalWndView::UpdatePlayStatus(BOOL bPlay)
{
	m_bIsPlaying = bPlay;
}

void CLocalWndView::UpdateMenuStatus(int nBtnType, BOOL bStatus)
{
	switch(nBtnType)
	{
	case OPERA_ID_PLAY:
		m_bIsPausing = bStatus;
		break;
	case OPERA_ID_SOUND:
		m_bIsSounding = bStatus;
		break;
	case OPERA_ID_FULLSCREEN:
		m_bIsFullScreening = bStatus;
		break;
	default:
		break;
	}
}
