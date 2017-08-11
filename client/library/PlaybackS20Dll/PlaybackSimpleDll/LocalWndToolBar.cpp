// LocalWndToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalWndToolBar.h"


// CLocalWndToolBar

IMPLEMENT_DYNAMIC(CLocalWndToolBar, CWnd)

CLocalWndToolBar::CLocalWndToolBar()
{
	m_bIsPlaying = FALSE;
	m_bIsFullScreening = FALSE;
	m_bIsSounding = FALSE;
	m_bIsPausing = FALSE;
	m_bIsPlaying = FALSE;
}

CLocalWndToolBar::~CLocalWndToolBar()
{
}


BEGIN_MESSAGE_MAP(CLocalWndToolBar, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(TOOL_STOP_ID, CLocalWndToolBar::OnToolBarBtnClk_Stop)
	ON_BN_CLICKED(TOOL_CAPTURE_ID, CLocalWndToolBar::OnToolBarBtnClk_Capture)
	ON_BN_CLICKED(TOOL_FULLSCREEN_ID, CLocalWndToolBar::OnToolBarBtnClk_FullScreen)
	ON_BN_CLICKED(TOOL_SOUND_ID, CLocalWndToolBar::OnToolBarBtnClk_OpenSound)
	ON_BN_CLICKED(TOOL_MINI_SLOW_ID, CLocalWndToolBar::OnToolBarBtnClk_Slow)
	ON_BN_CLICKED(TOOL_PLAY_ID, CLocalWndToolBar::OnToolBarBtnClk_Play)
	ON_BN_CLICKED(TOOL_MINI_FAST_ID, CLocalWndToolBar::OnToolBarBtnClk_Fast)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVED, &CLocalWndToolBar::OnMsgSliderClick)
END_MESSAGE_MAP()



// CLocalWndToolBar 消息处理程序



int CLocalWndToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CreateCtrl();
	InitCtrl();

	return 0;
}

void CLocalWndToolBar::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CLocalWndToolBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveCtrl();
}

BOOL CLocalWndToolBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(10, 10, 12));
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SelectObject(&m_fTextFont);
	pDC->DrawText(m_strTime, m_rcTime, DT_RIGHT);
	return TRUE;
}

void CLocalWndToolBar::CreateCtrl()
{
	m_sliderPos.Create(NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP|SS_NOTIFY|SS_BITMAP, 
		CRect(0,0,0,0), this, TOOL_SLIDER_POS);
	m_btnStop.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		CRect(0, 0, 22, 22), this, TOOL_STOP_ID);
	m_btnCapture.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		CRect(0, 0, 22, 22), this, TOOL_CAPTURE_ID);
	m_btnSlow.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		CRect(0, 0, 22, 22), this, TOOL_MINI_SLOW_ID);
	m_btnPlay.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		CRect(0, 0, 22, 22), this, TOOL_PLAY_ID);
	m_btnFast.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		CRect(0, 0, 22, 22), this, TOOL_MINI_FAST_ID);
	m_btnSound.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		CRect(0, 0, 22, 22), this, TOOL_SOUND_ID);
	m_btnFullScreen.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		CRect(0, 0, 22, 22), this, TOOL_FULLSCREEN_ID);
	m_valueSound.Create(NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP|SS_NOTIFY|SS_BITMAP, 
		CRect(0,0,0,0), this, TOOL_SLIDER_VOLUMN);
}
void CLocalWndToolBar::InitCtrl()
{
	m_btnStop.SetSkinBitmap(
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP_OVER), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP_OVER));
	m_btnCapture.SetSkinBitmap(
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_CAPTURE), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_CAPTURE_OVER), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_CAPTURE_OVER));
	m_btnSlow.SetSkinBitmap(
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SLOW), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SLOW_OVER), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SLOW_OVER),
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SLOW));
	m_btnPlay.SetSkinBitmap(
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY_OVER), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY_OVER));
	m_btnFast.SetSkinBitmap(
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_FAST), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_FAST_OVER), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_FAST_OVER),
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_FAST));
	m_btnSound.SetSkinBitmap(
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE_OVER), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE_OVER));
	m_btnFullScreen.SetSkinBitmap(
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_FULLSCREEN), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_FULLSCREEN_OVER), 
		RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_FULLSCREEN_OVER));


	m_sliderPos.SetRange(0, 100);
	m_sliderPos.SetPos(0);
	m_sliderPos.SetMarginLeft(0);
	m_sliderPos.SetMarginRight(0);
	m_sliderPos.SetPageSize(5);
	m_sliderPos.DrawFocusRect( FALSE, FALSE );

	m_valueSound.SetRange(0, 100);
	m_valueSound.SetPos(50);
	m_valueSound.SetMarginLeft(0);
	m_valueSound.SetMarginRight(0);
	m_valueSound.SetPageSize(5);
	m_valueSound.DrawFocusRect( FALSE, FALSE );

	SetToolBarBtnTips();

	m_sliderPos.EnableWindow(FALSE);
	m_valueSound.EnableWindow(FALSE);

}
void CLocalWndToolBar::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(rcClient);

	const int nSpace = 4;
	const int nBigBtnWidth = 22;
	const int nBigBtnHeight = 22;

	//停止
	CRect rcStop;
	rcStop.left = 2*nSpace;
	rcStop.top = (rcClient.Height() - nBigBtnHeight) / 2 + 4;
	rcStop.right = rcStop.left + nBigBtnWidth;
	rcStop.bottom = rcStop.top + nBigBtnHeight-1;
	m_btnStop.MoveWindow(rcStop);

	//抓图
	CRect rcCapture;
	rcCapture.left = rcStop.right + 2*nSpace;
	rcCapture.top = rcStop.top;
	rcCapture.right = rcCapture.left + nBigBtnWidth;
	rcCapture.bottom = rcStop.bottom;
	m_btnCapture.MoveWindow(rcCapture);

	//全屏
	CRect rcFullScreen;
	rcFullScreen.left = rcCapture.right + 2*nSpace;
	rcFullScreen.top = rcStop.top;
	rcFullScreen.right = rcFullScreen.left + nBigBtnWidth;
	rcFullScreen.bottom = rcStop.bottom;
	m_btnFullScreen.MoveWindow(rcFullScreen);

	//音频
	CRect rcSound;
	rcSound.left = rcFullScreen.right + 2*nSpace;
	rcSound.top = rcStop.top;
	rcSound.right = rcSound.left + nBigBtnWidth;
	rcSound.bottom = rcStop.bottom;
	m_btnSound.MoveWindow(rcSound);

	//音量调节
	CRect rcVaule;
	rcVaule.left = rcSound.right + 2*nSpace;
	rcVaule.top = rcStop.top + nBigBtnHeight/2 - 2;
	rcVaule.right = rcVaule.left + 2*nBigBtnWidth;
	rcVaule.bottom = rcVaule.top + 4;
	m_valueSound.MoveWindow(rcVaule);
	m_valueSound.SetBitmapChannel_EX(rcVaule.Width(), 4, IDB_BITMAP_FILESLICED_SLIDER, IDB_BITMAP_FILESLICED_SLIDER_ON, FALSE );

	//播放
	CRect rcPlay;
	rcPlay.left = rcClient.Width()/2 -  nBigBtnWidth/2;
	rcPlay.top = rcStop.top;
	rcPlay.right = rcPlay.left + nBigBtnWidth;
	rcPlay.bottom = rcStop.bottom;
	m_btnPlay.MoveWindow(rcPlay);

	//慢放
	CRect rcSlow;
	rcSlow.left = rcPlay.left - nBigBtnWidth - nSpace;
	rcSlow.top = rcStop.top;
	rcSlow.right = rcSlow.left + nBigBtnWidth;
	rcSlow.bottom = rcStop.bottom;
	m_btnSlow.MoveWindow(rcSlow);

	//快放
	CRect rcFast;
	rcFast.left = rcPlay.right + nSpace;
	rcFast.top = rcStop.top;
	rcFast.right = rcFast.left + nBigBtnWidth;
	rcFast.bottom = rcStop.bottom;
	m_btnFast.MoveWindow(rcFast);

	//进度条
	const int nLeft = 0;
	const int nSliderWidth = rcClient.Width();
	if (nSliderWidth > 0)
	{
		const int nTop = 2;
		const int nSliderHeight = 6;
		m_sliderPos.MoveWindow(nLeft, nTop, nSliderWidth, nSliderHeight, TRUE);
		m_sliderPos.SetBitmapChannel_EX(nSliderWidth, nSliderHeight, IDB_BITMAP_FILESLICED_SLIDER, IDB_BITMAP_FILESLICED_SLIDER_ON, FALSE );
	}

	//时间显示
	m_rcTime.right = rcClient.right - nSpace*2;
	m_rcTime.left = m_rcTime.right - 116;
	m_rcTime.top = rcStop.top + nBigBtnHeight/2 - 7;
	m_rcTime.bottom = m_rcTime.top + 14;
}


void CLocalWndToolBar::SetToolBarBtnTips()
{
	m_btnStop.SetToolTipText(_T("停止播放"));
	m_btnCapture.SetToolTipText(_T("抓图"));
	m_btnFullScreen.SetToolTipText(_T("全屏显示"));
	m_btnSound.SetToolTipText(_T("打开音频"));
	m_btnSlow.SetToolTipText(_T("减速"));
	m_btnPlay.SetToolTipText(_T("暂停"));
	m_btnFast.SetToolTipText(_T("加速"));
}

void CLocalWndToolBar::OnToolBarBtnClk_Stop()
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_STOP, 0);
}

void CLocalWndToolBar::OnToolBarBtnClk_Capture()
{
	if (m_bIsPlaying)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_CAPTURE, 0);
	}
}

void CLocalWndToolBar::OnToolBarBtnClk_FullScreen()
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_FULLSCREEN, 0);
}

void CLocalWndToolBar::OnToolBarBtnClk_OpenSound()
{
	if (m_bIsPlaying)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SOUND, 0);
	}
}

void CLocalWndToolBar::OnToolBarBtnClk_Slow()
{
	if (m_bIsPlaying)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SLOW, 0);
	}
}

void CLocalWndToolBar::OnToolBarBtnClk_Play()
{
	if (m_bIsPlaying)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_PLAY, 0);
	}
}

void CLocalWndToolBar::OnToolBarBtnClk_Fast()
{
	if (m_bIsPlaying)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_FAST, 0);
	}
}

void CLocalWndToolBar::SetPlayPos(int nPos)
{
	m_sliderPos.SetPos(nPos);
}

void CLocalWndToolBar::SetTimeInfo(CString strTime)
{
	m_strTime = strTime;
	RedrawWindow(&m_rcTime);
}

void CLocalWndToolBar::UpdateBtnStatus(BOOL bIsPlaying, int nBtnType)
{
	switch(nBtnType)
	{
	case OPERA_ID_STOP:
		{
			m_sliderPos.SetPos(0);
			m_sliderPos.EnableWindow(FALSE);
			m_valueSound.SetPos(50);
			m_valueSound.EnableWindow(FALSE);
			m_strTime = _T("");
			RedrawWindow(&m_rcTime);
			if (m_bIsPlaying)
			{
				m_btnStop.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_REPLAY), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_REPLAY_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_REPLAY_OVER));
				m_btnStop.SetToolTipText(_T("重新播放"));
				m_btnPlay.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY_OVER),
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY));
				m_btnPlay.SetToolTipText(_T("播放"));
				m_btnPlay.EnableWindow(FALSE);
				m_btnSlow.EnableWindow(FALSE);
				m_btnFast.EnableWindow(FALSE);
				m_btnSound.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE_OVER));
				m_btnSound.SetToolTipText(_T("打开音频"));
			}
			else
			{
				m_btnStop.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP_OVER));
				m_btnStop.SetToolTipText(_T("停止播放"));
				m_btnPlay.EnableWindow(TRUE);
				m_btnSlow.EnableWindow(TRUE);
				m_btnFast.EnableWindow(TRUE);
			}
			m_bIsPausing = FALSE;
			m_bIsSounding = FALSE;
			m_bIsPlaying = bIsPlaying;
		}
		break;
	case OPERA_ID_FULLSCREEN:
		{
			if (!m_bIsFullScreening)
			{
				m_btnFullScreen.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_EXITFULL), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_EXITFULL_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_EXITFULL_OVER));
				m_btnFullScreen.SetToolTipText(_T("退出全屏"));
				m_bIsFullScreening = TRUE;
			}
			else
			{
				m_btnFullScreen.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_FULLSCREEN), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_FULLSCREEN_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TOOLBAR_FULLSCREEN_OVER));
				m_btnFullScreen.SetToolTipText(_T("全屏显示"));
				m_bIsFullScreening = FALSE;
			}
		}
		break;
	case OPERA_ID_SOUND:
		{
			if (!m_bIsSounding)
			{
				m_btnSound.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_OVER));
					m_btnSound.SetToolTipText(_T("关闭音频"));
				m_bIsSounding = TRUE;
				m_valueSound.EnableWindow(TRUE);
				m_valueSound.SetPos(50);
			}
			else
			{
				m_btnSound.SetSkinBitmap(
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_SOUND_CLOSE_OVER));
				m_btnSound.SetToolTipText(_T("打开音频"));
				m_bIsSounding = FALSE;
				m_valueSound.EnableWindow(FALSE);
			}
		}
		break;
	case OPERA_ID_PLAY:
		{
			if (!m_bIsPausing)
			{
				m_btnPlay.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY_OVER),
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PLAY));
				m_btnPlay.SetToolTipText(_T("播放"));
				m_btnSlow.EnableWindow(FALSE);	//暂停时禁止快进快退
				m_btnFast.EnableWindow(FALSE);
				m_bIsPausing = TRUE;
			}
			else
			{
				m_btnPlay.SetSkinBitmap(
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE_OVER), 
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE_OVER),
					RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE));
				m_btnPlay.SetToolTipText(_T("暂停"));
				m_btnSlow.EnableWindow(TRUE);	//恢复快进快退
				m_btnFast.EnableWindow(TRUE);
				m_bIsPausing = FALSE;
			}
		}
		break;
	case OPERA_ID_NOTICE:
		{
			m_bIsPlaying = bIsPlaying;
			m_sliderPos.EnableWindow(TRUE);
			m_btnPlay.SetSkinBitmap(
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE), 
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE_OVER), 
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE_OVER),
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_PAUSE));
			m_btnPlay.SetToolTipText(_T("暂停"));
			m_btnPlay.EnableWindow(TRUE);
			m_btnSlow.EnableWindow(TRUE);
			m_btnFast.EnableWindow(TRUE);
			m_btnStop.SetSkinBitmap(
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP), 
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP_OVER), 
				RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_FILESLICED_STOP_OVER));
			m_btnStop.SetToolTipText(_T("停止播放"));
			m_bIsPausing = FALSE;
		}
		break;
	default:
		break;
	}
	Invalidate(FALSE);
}

LRESULT CLocalWndToolBar::OnMsgSliderClick(WPARAM wParam, LPARAM lParam)
{
	int nSlider = m_sliderPos.GetDlgCtrlID();
	int nValueSound = m_valueSound.GetDlgCtrlID();
	if (nSlider == wParam)
	{
		if (!m_bIsPlaying)
		{
			return 0;
		}
		int iPos = 0;
		iPos = m_sliderPos.GetPos();
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SLIDER, iPos);
	}
	if (nValueSound == wParam)
	{
		if (!m_bIsPlaying)
		{
			return 0;
		}
		int iPos = 0;
		iPos = m_valueSound.GetPos();
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SOUNDVALUE, iPos);
	}
	return 0;
}