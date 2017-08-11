//lint -library
#include "stdafx.h"
#include "PlaybackWnd.h"
//#include "../LabelAdd.h"
#include "../ui/uiCtrl/BCMenu.h"
#include "../PlaybackSimpleDll.h"
#include "../util/Markup.h"
#include <math.h>

void CALLBACK PlaybackWndMsgCB(UINT dwMsgID, DWORD dwParam1, DWORD dwParam2, LPVOID dwUserData)
{
	CPlaybackWnd* pWnd = (CPlaybackWnd*)dwUserData;
	if (pWnd != NULL)
	{
		switch (dwMsgID)
		{
			// 播放成功.
		case MSG_PLAYBACK_PLAY_SUCCESS:
			{
				// 设置回放句柄.
				pWnd->SetPlayHandle((HANDLE)dwParam1);
			}
			break;

			// 播放失败或停止播放.
		case MSG_PLAYBACK_PLAY_FAIL:
		case MSG_PLAYBACK_STOPPLAY:
		case MSG_PLAYBACK_LOCATEPLAY_FAIL:
			{
				// 置空回放句柄.
				pWnd->SetPlayHandle(NULL);
			}
			break;
		}
		pWnd->PostMessage(dwMsgID, (WPARAM)dwParam1, (WPARAM)dwParam2);
		TRACE("@z PlaybackWnd PostMessage, dwMsgID:%ld", dwMsgID);
	}
}


IMPLEMENT_DYNAMIC(CPlaybackWnd, CWnd)
CPlaybackWnd::CPlaybackWnd(HANDLE hDlg, void* pParent)
{
    m_pParent = pParent;
	m_clrBorder = COLOR_BORDER_NOSELECT;
	m_bIsMax = FALSE;
	m_bPause = FALSE;
	m_bSoundOpen = FALSE;
	m_bSaveData = FALSE;
	m_bSingleSound = FALSE;
	m_pPlayTitleWnd = NULL;
	m_pPlayVedioWnd = NULL;
	m_pToolBarWnd = NULL;
	m_pToolBarExpandWnd = NULL;
	m_bShowTitle = TRUE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_emSynPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_bUsed = FALSE;
	m_unWndID = 0;
	m_hPlayback = NULL;
	m_pSetOsdTime = NULL;
	m_lpSetOsdTime = NULL;
	m_nCameraID = -1;
	m_lChannel = -1;
	m_lOsdTime = 0;
	m_nVolumn = 0;
	m_bMove = FALSE;
	m_emShowPro = SHOW_PROPORTION_ORIGINAL;
	m_bFullScreen = FALSE;
	m_bSynPlay = FALSE;
	m_bSelect = FALSE;
	m_strFilePath = _T("");
	m_bIsSingleFrame = FALSE;
	m_hDlg = hDlg;
	m_dwRight = PBSD_RIGHT_ALL;
	m_dwRev = 0;
	m_strSaveDataPath = _T("");
	m_hSaveDataState = NULL;
	m_emPlayType = PB_TYPE_DVR;
	m_bIsInitTask = FALSE;
	m_pCaptureTip = NULL;
	m_pSaveData = NULL;
	m_pbtnClose = NULL;
	m_hCheckSpaceThread = NULL;
	m_hStopCheck = NULL;
	m_bReversePlay = FALSE;
	m_lBeginTime = 0;
	m_lEndTime = 0;
	m_bPlayNextFile = FALSE;
	m_hStopCheck = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hRecvMsg = NULL;
	m_lPlayPos = 0;
	m_emPbType = PLAY_TYPE_TIME;
	m_bOperating = FALSE;
}

CPlaybackWnd::~CPlaybackWnd()
{
	m_pPlayTitleWnd = NULL;
	m_pPlayVedioWnd = NULL;
	m_pToolBarWnd = NULL;
	m_pToolBarExpandWnd = NULL;
	m_hPlayback = NULL;
	m_pSetOsdTime = NULL;
	m_lpSetOsdTime = NULL;
	m_pCaptureTip = NULL;
	m_pSaveData = NULL;
	m_pbtnClose = NULL;
	if (m_hStopCheck != NULL)
	{
		CloseHandle(m_hStopCheck);
		m_hStopCheck = NULL;
	}
}

BEGIN_MESSAGE_MAP(CPlaybackWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_MESSAGE(WM_UPDATE_MSG, &CPlaybackWnd::OnUpdateMsg)
	ON_MESSAGE(MSG_PLAYBACK_FIND_FAIL, &CPlaybackWnd::OnMsgFindFail)
	ON_MESSAGE(MSG_PLAYBACK_FIND_SUCCESS, &CPlaybackWnd::OnMsgFindSuccess)
	ON_MESSAGE(MSG_PLAYBACK_PLAY_FAIL, &CPlaybackWnd::OnMsgPlayFail)
	ON_MESSAGE(MSG_PLAYBACK_PLAY_SUCCESS, &CPlaybackWnd::OnMsgPlaySuccess)
	ON_MESSAGE(MSG_PLAYBACK_STOPPLAY, &CPlaybackWnd::OnMsgStopPlay)
	ON_MESSAGE(MSG_PLAYBACK_PLAYING, &CPlaybackWnd::OnMsgPlaying)
	ON_MESSAGE(MSG_PLAYBACK_LOCATEPLAY_SUCCESS, &CPlaybackWnd::OnMsgLocatePlaySuccess)
	ON_MESSAGE(MSG_PLAYBACK_LOCATEPLAY_FAIL, &CPlaybackWnd::OnMsgLocatePlayFail)
	ON_MESSAGE(MSG_PLAYBACK_BWRECONNECT_STOPPLAY, &CPlaybackWnd::OnMsgBwStopPlay)
	ON_MESSAGE(MSG_PLAYBACK_BW_LIMIT, &CPlaybackWnd::OnMsgBWLimit)
	ON_MESSAGE(MSG_MOUSEEVENT_MINITOOLBAR_LEAVE, &CPlaybackWnd::OnMsgMouseLeaveMiniToolbar)
	ON_MESSAGE(MSG_MOUSEEVENT_MINITOOLBAR_MOVE, &CPlaybackWnd::OnMsgMouseMoveMiniToolbar)
	ON_MESSAGE(MSG_TOOLBAR_BTN_CLICK, &CPlaybackWnd::OnMsgToolButtonClick)
	ON_MESSAGE(MSG_MINITOOLBAR_EXPAND_CLOSE, &CPlaybackWnd::OnMsgToolBarExpandClose)
	ON_MESSAGE(MSG_CHECK_SPACE_NOTENOUGH, &CPlaybackWnd::OnMsgCheckSpaceNotEnough)
	ON_MESSAGE(MSG_PLAYBACK_FIND_NO_FILE, &CPlaybackWnd::OnMsgFindNoFile)
	ON_MESSAGE(MSG_INVALIDATE_WINDOW, &CPlaybackWnd::OnMsgInvalidateWindow)
	ON_BN_CLICKED(BTN_STOP_ID, &CPlaybackWnd::OnClosePlayWindow)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CPlaybackWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//创建标题对象
	if (NULL == m_pbtnClose)
	{
		m_pbtnClose = new CButtonST_Skin;
		m_pbtnClose->Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_CLIPSIBLINGS, CRect(0,0,0,0), this, BTN_STOP_ID);
		m_pbtnClose->LoadbakeImage( 
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_BTN_CLOSE_WND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE));
		m_pbtnClose->ShowWindow(SW_HIDE);
		m_pbtnClose->SetTooltipText(_T("关闭"));
	}

	if (m_pPlayTitleWnd == NULL)
	{
		m_pPlayTitleWnd = new CPlayTitleWnd;
		m_pPlayTitleWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0,0,0,0), this, PLAYBACK_TITLE_ID);
		m_pPlayTitleWnd->ShowWindow(SW_SHOW);
	}
	if (NULL == m_pToolBarWnd)
	{
		m_pToolBarWnd = new CPlayMiniToolBarWnd;
		m_pToolBarWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0,0,0,0), this, PLAYBACK_MINITOOL_ID);
		m_pToolBarWnd->ShowWindow(SW_HIDE);
	}
	if (m_pPlayVedioWnd == NULL)
	{
		m_pPlayVedioWnd = new CPlayVedioWnd;
		m_pPlayVedioWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0,0,0,0), this, PLAYBACK_VEDIO_ID);
		m_pPlayVedioWnd->ShowWindow(SW_SHOW);
	}

	m_bMove = TRUE;

	//定时器
	SetTimer(TIMER_GET_PLAYPOS, GET_PLAYPOS_TIME, NULL);
    SetTimer(TIMER_GET_PLAYOSDTIME, GET_PLAYOSDTIME_TIME, NULL);

	//创建时默认消息接收句柄为父窗口句柄
	if (lpCreateStruct->hwndParent != NULL)
	{
		m_hRecvMsg = lpCreateStruct->hwndParent;
	}

	return 0;
}

void CPlaybackWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	
}

void CPlaybackWnd::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	
	//DrawBorder(m_clrBorder);
// 	CDC * pDC = this->GetDC();
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 	pDC->FillSolidRect(rcClient, RGB(20, 20, 20));
// 
// 	if (m_pPlayTitleWnd)
// 	{
// 		m_pPlayTitleWnd->Invalidate();
// 	}
// 	if (m_pPlayVedioWnd)
// 	{
// 		m_pPlayVedioWnd->Invalidate();
// 	}

}

void CPlaybackWnd::OnDestroy()
{
	CWnd::OnDestroy();

	if (m_bIsInitTask)
	{
		TRACE("@z UnInitTask:%d", m_unWndID);
		m_opePB.UnInitTask();
		m_bIsInitTask = FALSE;
	}

	DestroyStopPlayback();

	//关闭定时器
	KillTimer(TIMER_GET_PLAYPOS);
    KillTimer(TIMER_GET_PLAYOSDTIME);

	//销毁标题对象
	if (m_pPlayTitleWnd)
	{
		m_pPlayTitleWnd->DestroyWindow();
		delete m_pPlayTitleWnd;
	    m_pPlayTitleWnd = NULL;
	}
	//销毁视频对象
	if (m_pPlayVedioWnd)
	{
		m_pPlayVedioWnd->DestroyWindow();
		delete m_pPlayVedioWnd;
		m_pPlayVedioWnd = NULL;
	}
	// 销毁工具栏对象.
	if (m_pToolBarWnd != NULL)
	{
		m_pToolBarWnd->DestroyWindow();
		delete m_pToolBarWnd;
		m_pToolBarWnd = NULL;
	}
	// 销毁图片展示对象.
	if (m_pCaptureTip != NULL)
	{
		m_pCaptureTip->DestroyWindow();
		delete m_pCaptureTip;
		m_pCaptureTip = NULL;
	}
	// 销毁剪辑路径展示对话框.
	if (m_pSaveData != NULL)
	{
		m_pSaveData->DestroyWindow();
		delete m_pSaveData;
		m_pSaveData = NULL;
	}
	//销毁关闭按钮
	if(m_pbtnClose != NULL)
	{
		m_pbtnClose->DestroyWindow();
		delete m_pbtnClose;
		m_pbtnClose = NULL;
	}

}

//设置窗口ID
void CPlaybackWnd::InitWnd(UINT unWndID, PLAY_LAYOUT_WND emLayoutWnd)
{
	m_unWndID = unWndID;
	if (m_unWndID < (unsigned int)emLayoutWnd)
	{
		if (m_opePB.InitTask(PlaybackWndMsgCB, this))
		{
			m_bIsInitTask = TRUE;
			TRACE("@z InitTask:%d", m_unWndID);
		}
	}
}

//窗口清理释放
void CPlaybackWnd::clear()
{
	StopPlayback();
}

void CPlaybackWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();

		CRect rcClient;
		GetClientRect(rcClient);

		m_pPlayTitleWnd->MoveWindow(rcClient.left+1, rcClient.top+1, rcClient.Width()-2, TITLE_WND_HEIGHT);

		rcClient.top = rcClient.bottom - 28;
		rcClient.left += 1;
		rcClient.right -= 1;
		rcClient.bottom -= 1;
		if (m_pToolBarWnd != NULL)
		{
			m_pToolBarWnd->MoveWindow(rcClient);
		}
	}
}

//移动控件
void CPlaybackWnd::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nWidth = 0;
	int nHeight = 0;

	if (rcClient.Height()>(int)TITLE_WND_HEIGHT && m_bShowTitle && m_pPlayTitleWnd)
	{
		if (m_pPlayVedioWnd)
		{	
			if (m_emShowPro != SHOW_PROPORTION_ORIGINAL)
			{
				if (SHOW_PROPORTION_4_3 == m_emShowPro)
				{
					nWidth = 4;
					nHeight = 3;
				}
				else
				{
					nWidth = 16;
					nHeight = 9;
				}
				CRect rcVedio(rcClient.left+1, rcClient.top+TITLE_WND_HEIGHT+1, rcClient.right-1, rcClient.bottom-1);
				if (rcVedio.Height()*nWidth < rcVedio.Width()*nHeight)
				{
					int nWidthTemp = rcVedio.Height()*nWidth/nHeight;
					rcVedio.left = rcVedio.left + (rcVedio.Width()-nWidthTemp)/2;
					rcVedio.right = rcVedio.left + nWidthTemp;
				}
				else if (rcVedio.Height()*nWidth > rcVedio.Width()*nHeight)
				{
					int nHeightTemp = rcVedio.Width()*nHeight/nWidth;
					rcVedio.top = rcVedio.top + (rcVedio.Height()-nHeightTemp)/2;
					rcVedio.bottom = rcVedio.top + nHeightTemp;
				}
				m_pPlayVedioWnd->MoveWindow(rcVedio);
			}
			else
			{
				m_pPlayVedioWnd->MoveWindow(rcClient.left+1, rcClient.top+TITLE_WND_HEIGHT+1, rcClient.Width()-2, rcClient.Height()-TITLE_WND_HEIGHT-2);
			}
		}
	}
	else
	{
		if (m_pPlayVedioWnd)
		{
			if (m_emShowPro != SHOW_PROPORTION_ORIGINAL)
			{
				if (SHOW_PROPORTION_4_3 == m_emShowPro)
				{
					nWidth = 4;
					nHeight = 3;
				}
				else
				{
					nWidth = 16;
					nHeight = 9;
				}

				CRect rcVedio(rcClient);
				rcVedio.DeflateRect(1, 1, 1, 1);
				if (rcVedio.Height()*nWidth < rcVedio.Width()*nHeight)
				{
					int nWidthTemp = rcVedio.Height()*nWidth/nHeight;
					rcVedio.left = rcVedio.left + (rcVedio.Width()-nWidthTemp)/2;
					rcVedio.right = rcVedio.left + nWidthTemp;
				}
				else if (rcVedio.Height()*nWidth > rcVedio.Width()*nHeight)
				{
					int nHeightTemp = rcVedio.Width()*nHeight/nWidth;
					rcVedio.top = rcVedio.top + (rcVedio.Height()-nHeightTemp)/2;
					rcVedio.bottom = rcVedio.top + nHeightTemp;
				}
				m_pPlayVedioWnd->MoveWindow(rcVedio);
			}
			else
			{
				rcClient.DeflateRect(1, 1, 1, 1);
				m_pPlayVedioWnd->MoveWindow(rcClient);
			}
		}
	}
	if (m_pbtnClose)
	{
		CRect rtCliseBtn(0,0,0,0);
		rtCliseBtn.right = rcClient.right - 1;
		rtCliseBtn.left = rtCliseBtn.right - 20;
		rtCliseBtn.top = rcClient.top + 2;
		rtCliseBtn.bottom = rtCliseBtn.top + 20;
		m_pbtnClose->MoveWindow(&rtCliseBtn);
	}

}

//显示回放控件标题文字
void CPlaybackWnd::SetTitleText(CString strTitleText)
{
	if (m_pPlayTitleWnd)
	{
// 		CFont fFont;
// 		fFont.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
// 			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_MODERN, "宋体");
// 		m_pPlayTitleWnd->SetTextFont(&fFont);
	    m_pPlayTitleWnd->SetText(strTitleText);
	}
}

//根据回放状态显示标题
void CPlaybackWnd::SetTitleText()
{
	if (!m_pPlayTitleWnd)
	{
		return;
	}

	// 回放类型.
	CString strPlayType;
	switch (m_stLoginInfo.emPlayType)
	{
	case PB_TYPE_DVR:
		strPlayType = _T("DVR");
		break;

	case PB_TYPE_CVR:
		strPlayType = _T("CVR");
		break;

	case PB_TYPE_CLOUD:
		strPlayType = _T("云存储");
		break;

    case PB_TYPE_KMS:
        strPlayType = _T("KMS");
        break;

	case PB_TYPE_OBJECTCLOUD:
		strPlayType = _T("对象存储");
		break;

	default:
		strPlayType = _T("未知");
		break;
	}

	CString strText;
	strText.Format(_T("窗口%02d: %s[%s]  "), m_unWndID + 1, (LPCTSTR)m_strChanName, (LPCTSTR)strPlayType);  //可以把ChannelName屏蔽
	//根据状态  拼接字符串
	if (m_hPlayback)
	{
		if (m_bSynPlay)
		{
			strText.AppendFormat(_T("%s"), _T(" 同步回放../"));
		}
		if (m_bPause)
		{
			strText.AppendFormat(_T("%s"), _T(" 回放暂停../"));
		}
		if (m_bSaveData)
		{
			strText.AppendFormat(_T("%s"), _T(" 剪辑中../"));
		}
		if (m_bIsSingleFrame)
		{
			strText.AppendFormat(_T("%s"), _T(" 单帧回放../"));
		}
	}

	// 显示速度.
	if (m_emPlaySpeed != PB_CONTROL_TYPE_NOREMAL)
	{
		// 快速.
		if (m_emPlaySpeed > 0)
		{
			strText.AppendFormat(_T(" X%d倍速../"), (int)pow(2, (double)m_emPlaySpeed));
		}
		// 慢速.
		else if (m_emPlaySpeed < 0)
		{
			strText.AppendFormat(_T(" X1/%d倍速../"), (int)pow(2, (double)-m_emPlaySpeed));
		}
	}
	
	//去掉最后一个字符'/'
	int nIndex = strText.ReverseFind(_T('/'));
	if (nIndex >= 0)
	{
		strText = strText.Left(nIndex);
	}

	//显示标题
	m_pPlayTitleWnd->SetText(strText);
}

//获取控件标题文字
CString CPlaybackWnd::GetTitleText()
{
	if (m_pPlayTitleWnd)
	{
		return m_pPlayTitleWnd->GetText();
	}
	else
	{
		return _T("");
	}
}

//显示取消标题栏
void CPlaybackWnd::ShowTitle(BOOL bShowTitle)
{
	m_bShowTitle = bShowTitle;
	MoveCtrl();

	//显示/隐藏
	if (m_bShowTitle)
	{
		if (m_pPlayTitleWnd)
		{	
			m_pPlayTitleWnd->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (m_pPlayTitleWnd)
		{
			m_pPlayTitleWnd->ShowWindow(SW_HIDE);
		}
	}
}

//显示窗口文案信息
void CPlaybackWnd::ShowWndInfo(CString strMsg)
{
	SetStopTextShow(strMsg);
}

//设置窗口边框颜色
void CPlaybackWnd::SetBorderClr(BOOL bSet)
{
	if (bSet)
	{
		m_clrBorder = COLOR_BORDER_SELECT;
		m_bSelect = TRUE;
	}
	else
	{
		m_clrBorder = COLOR_BORDER_NOSELECT;
		m_bSelect = FALSE;
	}

	DrawBorder(m_clrBorder);
}

//边框绘制
void CPlaybackWnd::DrawBorder(COLORREF clrBorder)
{
	CWindowDC dc(this);
	CRect rcWin;
	CPen pen, *pOldPen;
	CBrush *pOldBrush;

	GetClientRect(&rcWin);
	pen.CreatePen(PS_SOLID, 0, clrBorder);
	pOldPen = dc.SelectObject(&pen);
	pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
// 	//如果窗口被选中，画图的区域增大
// 	if (m_bSelect)
// 	{
// 		rcWin.DeflateRect(1, 1, 1, 1);
// 	}
	dc.Rectangle(rcWin);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	pen.DeleteObject();
}

//定时器函数  获取OSD时间，回放进度，下载进度
void CPlaybackWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_GET_PLAYPOS:
		{
			if (m_hPlayback)
			{
			    m_lPlayPos = GetPlayPos();
				if (m_lPlayPos >= 100)
				{
					//回放完成或者回放进度获取出错
					PLAYBACKDLL_ERROR(_T("GetPlayPos error, m_lPlayPos:%ld"), m_lPlayPos);
					StopPlayback(TRUE);
				}
			}
		}
		break;
	case TIMER_GET_PLAYOSDTIME:
		{
			if (m_hPlayback)
			{
				m_lOsdTime = GetOsdTime();
				//回调，OSD时间
                if (m_lOsdTime != 0 && m_pSetOsdTime)
                {
					m_pSetOsdTime(m_lOsdTime, m_unWndID, m_lpSetOsdTime);
                }
				if (!m_bReversePlay)
				{
					if (m_lOsdTime == m_lEndTime)
					{
						PLAYBACKDLL_ERROR(_T("GetPlayPos, m_lOsdTime == m_lEndTime :%ld"), m_lOsdTime);
						StopPlayback(TRUE);
					}
				}	
				else
				{
					if (m_lOsdTime == m_lBeginTime)
					{
						PLAYBACKDLL_ERROR(_T("GetPlayPos, m_lOsdTime == m_lBeginTime :%ld"), m_lOsdTime);
						StopPlayback(TRUE);
					}
				}
			}
		}
		break;

	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CPlaybackWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::PostMessage(m_hRecvMsg, MSG_SELECT_VEDIO, 0, m_unWndID);

	CWnd::OnLButtonDown(nFlags, point);
}


void CPlaybackWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pToolBarWnd != NULL)
	{
		m_pToolBarWnd->ShowWindow(SW_HIDE);
	}

	if (m_pPlayVedioWnd)
	{
		BCMenu menuPopup;
		menuPopup.CreatePopupMenu();
		if (menuPopup.GetSafeHmenu() == NULL)
		{
			return;
		}
		//显示/隐藏标题选项
		if (m_bShowTitle)
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_SHOWTITLE, _T("隐藏标题"), 11);
		}
		else
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_SHOWTITLE, _T("显示标题"), 11);
		}
		//视频比调整
		menuPopup.AppendMenu(MF_SEPARATOR);

		BCMenu menuSub;
		menuSub.CreatePopupMenu();
		if (menuSub.GetSafeHmenu() != NULL)
		{
			menuSub.AppendMenu(MF_STRING, MENU_ID_ORIGINAL, _T("铺满回放窗口"), 11);
			menuSub.AppendMenu(MF_STRING, MENU_ID_4_3, _T("4:3"), 11);
			menuSub.AppendMenu(MF_STRING, MENU_ID_16_9, _T("16:9"), 11);
			menuPopup.AppendMenu(MF_POPUP, (UINT)menuSub.GetSafeHmenu(), _T("显示比例"), 11);
		}
		menuPopup.AppendMenu(MF_SEPARATOR);
		if (m_hPlayback != NULL)
		{
			// 回放控制.
			if (m_bPause  ||  m_bIsSingleFrame)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("播放"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_STOP, _T("停止"), 11);
				menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_SLOW, _T("减速"), 11);
				menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_FAST, _T("加速"), 11);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("暂停"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_STOP, _T("停止"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_SLOW, _T("减速"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_FAST, _T("加速"), 11);
			}

			if (m_bPause  ||  m_bReversePlay)
			{
				menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_SINGLEFRAME, _T("单帧播放"), 11);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_SINGLEFRAME, _T("单帧播放"), 11);
			}

			menuPopup.AppendMenu(MF_SEPARATOR);

			if (theApp.HasDownloadModule())
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_DOWNLOAD_BY_TIME, _T("录像下载"), 11);
				if (m_bSaveData)
				{
					menuPopup.AppendMenu(MF_STRING, MENU_ID_SAVE, _T("停止剪辑"), 11);
				}
				else
				{
					menuPopup.AppendMenu(MF_STRING, MENU_ID_SAVE, _T("剪辑"), 11);
				}
			}
			menuPopup.AppendMenu(MF_STRING, MENU_ID_CAPTURE, _T("抓图"), 11);
			
			if (m_bSoundOpen)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("关闭声音"), 11);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("打开声音"), 11);
			}
			menuPopup.AppendMenu(MF_SEPARATOR);
		}	// if.

		if (m_hPlayback || !m_bUsed)
		{
			menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_PLAYAGAIN, _T("重新播放"), 11);
		}
		else
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAYAGAIN, _T("重新播放"), 11);
		}

		if (m_bSingleSound)
		{
			menuPopup.DeleteMenu(_T("减速"));
			menuPopup.DeleteMenu(_T("加速"));
			menuPopup.DeleteMenu(_T("单帧播放"));
			menuPopup.DeleteMenu(_T("抓图"));
		}
		//显示弹出菜单
		POINT curPos;
		GetCursorPos(&curPos);
		LONG nCmd = menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD ,curPos.x,curPos.y,this);

		//响应菜单消息
		switch (nCmd)
		{
		case MENU_ID_SHOWTITLE:
			{
				if (m_bShowTitle)
				{
					ShowTitle(FALSE);
				}
				else
				{
					ShowTitle(TRUE);
				}
			}
			break;
		case MENU_ID_ORIGINAL:
			{
				m_emShowPro = SHOW_PROPORTION_ORIGINAL;
				//设置视频比例
				SetVedioOriginal();
			}
			break;
		case MENU_ID_4_3:
			{
				m_emShowPro = SHOW_PROPORTION_4_3;
				//设置视频比例
				SetVedioOriginal();
			}
			break;
		case MENU_ID_16_9:
			{
				m_emShowPro = SHOW_PROPORTION_16_9;
				//设置视频比例
				SetVedioOriginal();
			}
			break;
		case MENU_ID_PLAYAGAIN:
			{
				PlayAgain();
			}
			break;
		case MENU_ID_PLAY:
			{
				OnMiniToolbarControlPlay();
			}
			break;

		case MENU_ID_STOP:
			{
				StopPlayback();
			}
			break;

		case MENU_ID_SLOW:
			{
				PlaySlow();
			}
			break;

		case MENU_ID_FAST:
			{
				PlayFast();
			}
			break;

		case MENU_ID_SINGLEFRAME:
			{
				PlaySingleFrame(TRUE);
			}
			break;

		case MENU_ID_DOWNLOAD_BY_TIME:
			{
				OnMiniToolbarDownload();
			}
			break;

		case MENU_ID_CAPTURE:
			{
				OnMiniToolbarControlCapture();
			}
			break;

		case MENU_ID_SAVE:
			{
				OnMiniToolbarControlSave();
			}
			break;

		case MENU_ID_OPENSOUND:
			{
				if (!m_bSoundOpen)
				{
					OpenPlaySound();
				}
				else
				{
					ClosePlaySound();
				}
			}
			break;
		default:
			break;
		}
	}

	//CWnd::OnRButtonDown(nFlags, point);
}

void CPlaybackWnd::OnClosePlayWindow()
{
	StopPlayback();
}
/**   @fn          PlayAgain
 *    @brief   	   重复回放.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::PlayAgain()
{
	StopPlayback();
	StartPlay(TRUE);
}

//视频窗口放大到父窗口 / 还原
void CPlaybackWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    //传出消息
    ::PostMessage(m_hRecvMsg, MSG_DB_VEDIO, 0, m_unWndID);

	CWnd::OnLButtonDblClk(nFlags, point);
}

//改变消息接收句柄
BOOL CPlaybackWnd::SetMsgRecvHwnd(HWND hWnd)
{
	if (hWnd == NULL)
	{
		return FALSE;
	}
	m_hRecvMsg = hWnd;

	return TRUE;
}


//改变视频窗口比例
void CPlaybackWnd::SetVedioOriginal()
{
	MoveCtrl();
}

/*******************回放处理**************************************/
/*************************************************
Function:    	PlaybackByTime
Description:	按时间回放
Input:			struPlaybackParamInfo回放参数, strChanName监控点名称（非必要参数，建议传入）
Output:      	
Return:			TRUE成功, FALSE失败
Time:			2011-2-11 13:49
*************************************************/
BOOL CPlaybackWnd::PlaybackByTime(const PB_LOGININFO& stLoginInfo, 
								  const PLAYBACK_TIME_INFO& stPlaybackParam, 
								  const FINDFILE_PARAM& stFindParam, 
								  CString strChanName, 
								  int nCameraID, 
								  DWORD dwRight,
								  DWORD dwRev)
{
	if (!m_pPlayVedioWnd)
	{
		return FALSE;
	}

	StopPlayback();
	PB_FILEINFO stFile;
	stFile.stStartTime = m_tStartTime;
	stFile.stEndTime = m_tEndTime;
	std::vector<PB_FILEINFO> vFileInfo;
	vFileInfo.push_back(stFile);
	SetFileInfo(vFileInfo);
	m_stLoginInfo = stLoginInfo;
	m_strChanName = strChanName;
	m_emPbType = PLAY_TYPE_TIME;
	m_stPlayTimeParam = stPlaybackParam;
	m_stFindParam = stFindParam;
	m_nCameraID = nCameraID;
	m_lChannel = stPlaybackParam.lChannel;
	m_dwRight = dwRight;
	m_dwRev = dwRev;
	m_emPlayType = stLoginInfo.emPlayType;

	StartPlay(TRUE);

	return TRUE;

}
 
/*************************************************
Function:    	PlaybackByFile
Description:	按文件回放
Input:			struPlaybackParamInfo回放参数(结构体中的时间参数虽非回放必要参数，但如果需要改变进度，需要传入), strChanName监控点名称（非必要参数，建议传入）
Output:      	
Return:			TRUE成功, FALSE失败
Time:			2011-2-11 13:49
*************************************************/
BOOL CPlaybackWnd::PlaybackByFile(const PB_LOGININFO& stLoginInfo, 
								  const PLAYBACK_FILE_INFO& stPlaybackParam, 
								  CString strChanName, 
								  int nCameraID,
								  DWORD dwRight,
								  DWORD dwRev)
{
	if (!m_pPlayVedioWnd)
	{
		return FALSE;
	}

	StopPlayback();
	PB_FILEINFO stFile;
	stFile.stStartTime = stPlaybackParam.stFileInfo.stStartTime;
	stFile.stEndTime = stPlaybackParam.stFileInfo.stEndTime;
	strcpy_s(stFile.szFileName, 128, stPlaybackParam.stFileInfo.szFileName);
	stFile.dwFileSize = stPlaybackParam.stFileInfo.dwFileSize;
	strcpy_s(stFile.szPlayUrl, 255, stPlaybackParam.stFileInfo.szPlayUrl);
	std::vector<PB_FILEINFO> vFileInfo;
	vFileInfo.push_back(stFile);
	SetFileInfo(vFileInfo);
	m_stLoginInfo = stLoginInfo;
	m_strChanName = strChanName;
	m_emPbType = PLAY_TYPE_FILE;
	m_stPlayFileParam = stPlaybackParam;
	m_nCameraID = nCameraID;
	m_lChannel = stPlaybackParam.lChannel;
	m_dwRight = dwRight;
	m_dwRev = dwRev;
	m_emPlayType = stLoginInfo.emPlayType;

	StartPlay(TRUE);

	return TRUE;
}

//在窗口上画图回调
void CALLBACK CPlaybackWnd::DrawOnWnd(LONG nPort, HDC hDC, LONG nUser)
{
	HWND hPlayHwnd = (HWND)nUser;
	if (NULL == hPlayHwnd)
	{
		return;
	}

	CPlaybackWnd*pPlaybackWnd = (CPlaybackWnd*)LongToPtr(nUser);
	if (NULL == pPlaybackWnd)
	{
		return;
	}
	if (pPlaybackWnd->m_pPlayVedioWnd)
	{
		pPlaybackWnd->m_pPlayVedioWnd->DrawTextOnVedio(CDC::FromHandle(hDC));
	}
}
/*************************************************
Function:    	StartPlay
Description:	开始回放
Input:			bFindFile:是否需要查找文件.
Output:      	
Return:			TRUE成功, FALSE失败
Time:			2011-2-12 11:39
*************************************************/
void CPlaybackWnd::StartPlay(BOOL bFindFile)
{
	m_bPlayNextFile = FALSE;
	m_bUsed = TRUE;

	if (m_emPbType == PLAY_TYPE_TIME)
	{
		//设置播放窗口
		m_stPlayTimeParam.hWnd = m_pPlayVedioWnd->GetSafeHwnd();
		m_stPlayTimeParam.pDrawFunc = &DrawOnWnd;
		m_stPlayTimeParam.nUser = (LONG)this;
		//调用回放函数
		PB_FILEINFO * lpPbFileInfo = NULL;
		if (bFindFile)
		{
			m_OrderPlay.ClearFile();
			lpPbFileInfo = new PB_FILEINFO[MAX_FILE_COUNT];
			// 是否查找所有类型的录像文件,是则按时间回放，不是则按文件回放..
			BOOL bPlayTime = (RECORD_TYPE_ALL == m_stFindParam.emRecordType);
			// 按文件回放时需要自动回放下一个文件.
			m_bPlayNextFile = !bPlayTime;
			m_opePB.FindAndPlayback(
				m_hDlg,
				m_nCameraID, 
				m_dwRev,
				m_stLoginInfo, 
				m_stPlayTimeParam, 
				m_stFindParam, 
				lpPbFileInfo,
				m_bReversePlay,
				bPlayTime);
		}
		else
		{
			m_opePB.PlaybackTime(
				m_hDlg,
				m_nCameraID, 
				m_dwRev,
				m_stLoginInfo, 
				m_stPlayTimeParam, 
				m_bReversePlay);
		}
	}
	else
	{
		//设置播放窗口
		m_stPlayFileParam.hWnd = m_pPlayVedioWnd->GetSafeHwnd();
		m_stPlayFileParam.pDrawFunc = &DrawOnWnd;
		m_stPlayFileParam.nUser = (LONG)this;
		//调用回放函数
		m_opePB.PlaybackFile(
			m_hDlg,
			m_nCameraID, 
			m_dwRev,
			m_stLoginInfo, 
			m_stPlayFileParam,
			m_bReversePlay);
	}
}

/*************************************************
Function:    	PlaybackSetPos
Description:	改变回放位置
Input:			dvrPlaybackTime回放位置，  按文件回放时将根据此参数计算位置
Output:      	
Return:			TRUE成功, FALSE失败
Time:			2011-2-12 11:39
*************************************************/
BOOL CPlaybackWnd::PlaybackSetPos(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime)
{
	// 不是在预览中.
	if (!m_hPlayback)
	{
		// 如果未预览过.
		if (-1 == m_nCameraID)
		{
			return FALSE;
		}
		// 停止预览.
		StopPlayback();

		if (m_bReversePlay)
		{
			memcpy_s(&m_stPlayTimeParam.stStartTime, sizeof(PB_TIME), &stStartTime, sizeof(PB_TIME));
			memcpy_s(&m_stPlayTimeParam.stStopTime, sizeof(PB_TIME), &stCurTime, sizeof(PB_TIME));
		}
		else
		{
			memcpy_s(&m_stPlayTimeParam.stStartTime, sizeof(PB_TIME), &stCurTime, sizeof(PB_TIME));
			memcpy_s(&m_stPlayTimeParam.stStopTime, sizeof(PB_TIME), &stStopTime, sizeof(PB_TIME));
		}
		// 标志为按时间回放.
		m_emPbType = PLAY_TYPE_TIME;
		// 开启新的预览.
		StartPlay(FALSE);
	}
	else
	{
		if (m_bReversePlay)
		{
			memcpy_s(&m_stPlayTimeParam.stStartTime, sizeof(PB_TIME), &stStartTime, sizeof(PB_TIME));
			memcpy_s(&m_stPlayTimeParam.stStopTime, sizeof(PB_TIME), &stCurTime, sizeof(PB_TIME));
		}
		else
		{
			memcpy_s(&m_stPlayTimeParam.stStartTime, sizeof(PB_TIME), &stStartTime, sizeof(PB_TIME));
			memcpy_s(&m_stPlayTimeParam.stStopTime, sizeof(PB_TIME), &stStopTime, sizeof(PB_TIME));
		}

		if (m_bReversePlay)
		{
			// 设置进度.
			m_opePB.LocatePlayTime(stStartTime, stCurTime, m_nCameraID, m_bReversePlay);
		}
		else
		{
			// 设置进度.
			m_opePB.LocatePlayTime(stCurTime, stStopTime, m_nCameraID, m_bReversePlay);
		}
	}

	return TRUE;
}

//停止回放,bAutoStop是否是自动停止,即是否是回放时间或回放文件结束了.
BOOL CPlaybackWnd::StopPlayback(BOOL bAutoStop, BWControlType emBwControl)
{
	//如果是暂停中，先恢复
	if (m_bPause)
	{
		RePlayback();
	}
	// 如果是单帧播放，先恢复.
	if (m_bIsSingleFrame)
	{
		// 单帧恢复.
		PlaySingleFrame(FALSE);
	}
	// 如果正在保存数据，停止保存.
	if (m_bSaveData)
	{
		StopSaveData();
	}

	//停止回放
	m_opePB.StopPlayback(m_hDlg, m_nCameraID, m_dwRev, bAutoStop, emBwControl);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), MSG_PLAYBACK_STATE, 0, 0);
	return TRUE;
}

// 程序退出时停止回放.
BOOL CPlaybackWnd::DestroyStopPlayback()
{
	//如果是暂停中，先恢复
	if (m_bPause)
	{
		RePlayback();
	}
	// 如果正在保存数据，停止保存.
	if (m_bSaveData)
	{
		StopSaveData();
	}

	//停止回放
	m_opePB.TaskStopPlayback(m_hDlg, m_nCameraID, m_dwRev, TRUE, BWCONTROL_NOCONTROL);

	return TRUE;
}
 
//暂停回放
BOOL CPlaybackWnd::PausePlayback()
{
	if (!m_hPlayback || m_bPause)
	{
		return FALSE;
	}

	//暂停
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_PAUSE, 0, lOutValue))
	{
		return FALSE;
	}

	m_bPause = TRUE;

	TRACE("@z PausePlayback m_bPause:%d", m_bPause);

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}
 
//恢复回放
BOOL CPlaybackWnd::RePlayback()
{
	if (!m_hPlayback || !m_bPause)
	{
		return FALSE;
	}

	// 恢复回放.
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_RESTART, 0, lOutValue))
	{
		return FALSE;
	}

	m_bPause = FALSE;

	TRACE("@z RePlayback m_bPause:%d", m_bPause);

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}

//改变回放速度
BOOL CPlaybackWnd::SetPlaySpeed(ENUM_CONTROL_TYPE emPlaySpeed, BOOL bSynPlay)
{
	if (!m_hPlayback)
	{
		return FALSE;
	}

	// 是否是暂停中，只有同步回放会用到此步.
	if (m_bPause)
	{
		m_emSynPlaySpeed = emPlaySpeed;
		return FALSE;
	}

	//速度未改变  
	if (m_emPlaySpeed == emPlaySpeed)
	{
		return TRUE;
	}

	// 如果正在单帧回放.
	if (m_bIsSingleFrame)
	{
		PlaySingleFrame(FALSE);
	}

	//改变回放速度
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(emPlaySpeed, 0, lOutValue))
	{
		return FALSE;
	}

	m_emPlaySpeed = emPlaySpeed;
	m_emSynPlaySpeed = emPlaySpeed;

	//刷新控件状态
	PostMessage(WM_UPDATE_MSG, 0, 0);

	// 向父窗口发送消息,播放速度改变.
	if (!bSynPlay)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_SPEED_CHANGE, (WPARAM)m_unWndID, (LPARAM)m_emPlaySpeed);
	}

	return TRUE;
}

// 获取同步回放速度.
ENUM_CONTROL_TYPE CPlaybackWnd::GetSynPlaySpeed() const
{
	return m_emSynPlaySpeed;
}

//获取回放OSD时间
LONG CPlaybackWnd::GetOsdTime()
{
	if (!m_hPlayback)
	{
		return 0;
	}

	//恢复
	LONG lOsdTime = 0;
	if (!m_opePB.GetOsdTime(lOsdTime))
	{
		return 0;
	}

	return lOsdTime;
}

//获取回放进度
LONG CPlaybackWnd::GetPlayPos()
{
	if (!m_hPlayback)
	{
		return 0;
	}

	//恢复
	LONG lPos = 0;
	if (!m_opePB.GetPlayPos(lPos))
	{
		return 0;
	}

	return lPos;
}

//打开声音
BOOL CPlaybackWnd::OpenPlaySound(DWORD dwSound)
{
	if (!m_hPlayback || m_bSoundOpen)
	{
		return FALSE;
	}

    //关闭其他窗口的声音
    if (m_pParent != NULL)
    {
        CPlayWndView* m_pDlg = (CPlayWndView*)m_pParent;
        m_pDlg->OnMsgMiniToolbarOpenSound(m_unWndID);
    }

	//打开声音
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_OPEN_AUDIO, 0, lOutValue))
	{
		//return FALSE;
	}

	m_bSoundOpen = TRUE;

	//设置初始音量为30
	SetPlayVolume(dwSound);

	// 向父窗口发送消息,以关闭其他窗口的音频.
	//::PostMessage(GetParent()->GetSafeHwnd(), MSG_MINITOOLBAR_OPENSOUND, (WPARAM)m_unWndID, 0);

	// 更新控件.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}

//关闭声音
BOOL CPlaybackWnd::ClosePlaySound()
{
	if (!m_hPlayback || !m_bSoundOpen)
	{
		return FALSE;
	}

	//关闭声音
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_CLOSE_AUDIO, 0, lOutValue))
	{
		//return FALSE;
	}

	m_bSoundOpen = FALSE;

	//刷新控件状态
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}

//改变音量大小
BOOL CPlaybackWnd::SetPlayVolume(DWORD dwSound)
{
	if (!m_hPlayback || !m_bSoundOpen)
	{
		return FALSE;
	}

	//设置音量
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_SET_VOLUME, (LONG)dwSound, lOutValue))
	{
		return FALSE;
	}

	m_nVolumn = dwSound;

	return TRUE;
}

//截图
BOOL CPlaybackWnd::CapturePic(CString& strPic)
{
	if (!m_hPlayback)
	{
		return FALSE;
	}

	SYSTEMTIME t;
	GetLocalTime(&t);

	ENUM_PIC_TYPE emPicType;
	TCHAR szPicPath[MAX_PATH];
	MByteToWChar(theApp.m_stPlaybackParam.szPicPath, szPicPath, MAX_PATH);
	CString strPicFile;
	strPicFile.Format(_T("%s"), szPicPath);

    if (strPicFile.IsEmpty())
    {
        CString strPath;
        AppFun_GetAppPath(strPicFile);
    }

	CString strCurDate;
	CString strFileName;
	strCurDate.Format(_T("%04u%02u%02u"), t.wYear, t.wMonth, t.wDay);
	strPicFile = strPicFile + _T("远程回放\\") + strCurDate + _T("\\");

	//获取osd时间用于命名
	LONG lOsdTime = GetOsdTime();
	if (lOsdTime <= 0)
	{
		strFileName.Format(_T("%s_%02u%02u%02u"), 
			(LPCTSTR)m_strChanName, t.wHour, t.wMinute, t.wSecond);
		strPicFile += strFileName;
	}
	else
	{
		//使用截图时间命名
		CTime tempTime(lOsdTime);
		strFileName.Format(_T("%s_%02d%02d%02d"), 
			(LPCTSTR)m_strChanName, tempTime.GetHour(), tempTime.GetMinute(), tempTime.GetSecond());
		strPicFile += strFileName;
	}

	if (theApp.m_stPlaybackParam.nPicType == 0)
	{
	    emPicType = PIC_TYPE_BMP;
		if (strPicFile.GetLength() > MAX_PATH-4)
		{
			strPicFile = strPicFile.Left(MAX_PATH - 4);
		}
		strPicFile.AppendFormat(_T(".bmp"));
		strFileName.AppendFormat(_T(".bmp"));
	}
	else
	{
		emPicType = PIC_TYPE_JPG;
		if (strPicFile.GetLength() > MAX_PATH-5)
		{
			strPicFile = strPicFile.Left(MAX_PATH - 4);
		}
		strPicFile.AppendFormat(_T(".jpeg"));
		strFileName.AppendFormat(_T(".jpeg"));
	}
	CStringA strFileA;
	strFileA = CStringW2A(strPicFile);

	// 抓图.
	if (!m_opePB.Capture(m_hDlg, strFileA.GetBuffer(), emPicType, m_nCameraID, m_dwRev))
	{
		return FALSE;
	}
	strPic = strPicFile;

	return TRUE;
}

//保存数据
int CPlaybackWnd::SaveData(CString& strPath)
{
	if (!m_hPlayback || m_bSaveData)
	{
		return 0;
	}

	CTime tempTime = CTime::GetCurrentTime();

	CString strCurDate;
	CString strFileName;
	strCurDate.Format(_T("%04d%02d%02d"), tempTime.GetYear(), tempTime.GetMonth(), tempTime.GetDay());
	TCHAR szDownloadPath[MAX_PATH];
	MByteToWChar(theApp.m_stPlaybackParam.szDownloadPath, szDownloadPath, MAX_PATH);
	CString strPicFile;
	strPicFile.Format(_T("%s"), szDownloadPath);
	strPicFile = strPicFile + _T("Clip\\") + strCurDate + _T("\\");

	//获取osd时间用于命名
	LONG lOsdTime = GetOsdTime();
	if (lOsdTime <= 0)
	{
		strFileName.Format(_T("save_%s_%02d%02d%02d"),  (LPCTSTR)m_strChanName, tempTime.GetHour(), tempTime.GetMinute(), tempTime.GetSecond());
		strPicFile += strFileName;
	}
	else
	{
		//使用剪辑时间命名
		CTime tempTime(lOsdTime);
		strFileName.Format(_T("osd_%s_%02d%02d%02d"), (LPCTSTR)m_strChanName, tempTime.GetHour(), tempTime.GetMinute(), tempTime.GetSecond());
		strPicFile += strFileName;
	}
	if (strPicFile.GetLength() > MAX_PATH-4)
	{
		strPicFile = strPicFile.Left(MAX_PATH - 4);
	}

	strPicFile.AppendFormat(_T(".mp4"));
	strFileName.AppendFormat(_T(".mp4"));

	CStringA strFileA;
	strFileA = CStringW2A(strPicFile);

	// 检查磁盘空间是否足够.
	if (!CheckFilePathSpace(strPicFile))
	{
		return -1;
	}

	//保存数据  文件路径在底层创建
	if (!m_opePB.SaveData(m_hDlg, strFileA.GetBuffer(), m_nCameraID, m_dwRev))
	{
		// 如果是IE保护模式.
		if (/*theApp.m_bIsIEProtected*/ 0)
		{
			CString strSelectPath = _T("");
			if (!theApp.GetIESafePath(strFileName, m_strSaveDataPath, strSelectPath, m_hSaveDataState, SAVE_FILETYPE_REC))
			{
				return 0;
			}

			// 抓图到IE缓存.
			strFileA = CStringW2A(m_strSaveDataPath);
			if (!m_opePB.SaveData(m_hDlg, strFileA.GetBuffer(), m_nCameraID, m_dwRev))
			{
				theApp.CancelIEFile(m_hSaveDataState);
				return 0;
			}

			m_bSaveData = TRUE;
			// 更新窗口标题.
			PostMessage(WM_UPDATE_MSG, 0, 0);
			strPath = strSelectPath;
			return 1;
		}	// if.

		return 0;
	}

	m_bSaveData = TRUE;

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	strPath = strPicFile;

	// 开启检测磁盘空间线程.
	m_stCheckData.strFilePath = strPath;
	m_stCheckData.pParam = (LPVOID)this;

	ResetEvent(m_hStopCheck);
	m_hCheckSpaceThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFuncCheckSpace), &m_stCheckData, 0, NULL);

	return 1;
}

//停止保存数据
BOOL CPlaybackWnd::StopSaveData()
{
	if (!m_hPlayback || !m_bSaveData)
	{
		return FALSE;
	}

	// 停止检测
	if (m_hCheckSpaceThread != NULL)
	{
		SetEvent(m_hStopCheck);
		WaitForWindowThread(m_hCheckSpaceThread);
		CloseHandle(m_hCheckSpaceThread);
		m_hCheckSpaceThread = NULL;
	}

	//停止保存数据
	m_opePB.StopSave(m_hDlg, m_nCameraID, m_dwRev);

	// 如果是IE保护模式.
	if (theApp.m_bIsIEProtected  &&  !m_strSaveDataPath.IsEmpty()  &&  m_hSaveDataState != NULL)
	{
		// 复制到指定目录.
		if (!theApp.SaveIEFile(m_strSaveDataPath, m_hSaveDataState))
		{
			return FALSE;
		}
	}

	m_bSaveData = FALSE;
	m_strSaveDataPath.Empty();
	m_hSaveDataState = NULL;

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;

}

//发送按钮状态刷新消息
void CPlaybackWnd::PostUpdateMsg()
{
	//发送
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYSTATE_UPDATE, (WPARAM)m_unWndID, 0);
}

//设置osd回调信息
void CPlaybackWnd::SetOsdCallback(pFunMsgOsdTime pSetOsdTime, LPVOID lpvoid)
{
	m_pSetOsdTime = pSetOsdTime;
	m_lpSetOsdTime = lpvoid;
}

//设置是否同步
void CPlaybackWnd::SetSynPlay(BOOL bSynPlay)
{
	m_bSynPlay = bSynPlay;
	//从同步中解开
	if (!m_bSynPlay)
	{
		RePlayback();
	}
	else
	{
		// 恢复回放速度.
		SetPlaySpeed(PB_CONTROL_TYPE_NOREMAL, TRUE);
		// 停止单帧回放.
		if (IsSingleFrame())
		{
			PlaySingleFrame(FALSE, TRUE);
		}
	}
}

/*******************回放处理**************************************/
// 响应界面更新消息.
LRESULT CPlaybackWnd::OnUpdateMsg(WPARAM wParam, LPARAM lParam)
{
	TRACE("@z OnUpdateMsg m_bPause:%d", m_bPause);
	//设置标题
	SetTitleText();
	//刷新控件状态
	PostUpdateMsg();
	// 刷新工具栏状态.
	if (m_pToolBarWnd != NULL)
	{
		m_pToolBarWnd->UpdateBtnState(m_bPause, m_bSoundOpen, m_bSaveData, m_bIsSingleFrame, m_bSingleSound);
	}
	if (m_pToolBarExpandWnd != NULL)
	{
		m_pToolBarExpandWnd->UpdateBtnState(m_bSoundOpen, m_bSaveData);
	}

	return 0;
}

/**   @fn      void CPlaybackWnd::SetStopTextShow(CString& strTitle, CString& strVideo)
 *    @brief   设置停止文字.
 *    @param   
 *    @param   strVidel:视频窗口文字.
 *    @return  
 */
void CPlaybackWnd::SetStopTextShow(CString strVideo, BOOL bErase)
{
 	// 视频窗口.
	if (m_pPlayVedioWnd != NULL)
	{
 		m_pPlayVedioWnd->SetTextShow(strVideo, bErase);
	}
}

void CPlaybackWnd::SetTextOnWnd( CString strVideo )
{
	// 视频窗口.
	if (m_pPlayVedioWnd != NULL)
	{
		m_pPlayVedioWnd->SetTextOnWnd(strVideo);
	}
}
/**   @fn      PlaySingleFrame
 *    @brief   单帧播放.
 *    @param   bPlay:TRUE-单帧播放,FALSE-停止单帧播放.
 *    @param   
 *    @return  
 */
void CPlaybackWnd::PlaySingleFrame(BOOL bPlay, BOOL bSynPlay)
{
	if (!m_hPlayback)
	{
		return;
	}

	// 如果是暂停状态.
	if (m_bPause)
	{
		m_bPause = FALSE;
	}

	LONG lOutValue;
	if (bPlay)
	{
		// 如果正在多倍速回放.
		if (m_emPlaySpeed != PB_CONTROL_TYPE_NOREMAL)
		{
			if (!SetPlaySpeed(PB_CONTROL_TYPE_NOREMAL))
			{
				return;
			}
		}
		// 单帧回放.
		if (m_opePB.PlaybackControl(PB_CONTROL_TYPE_SINGLE_FRAME, 0, lOutValue))
		{
			m_bIsSingleFrame = TRUE;
		}
		else
		{
			//说明下，第一次调用单帧播放时，可能会返回失败(特别是CVR 云存储，pcnvr回放时)，
			//但是实际已经操作成功了，而播放库那边反馈暂时无法修改此问题因此做如下操作。
			//Begin
			Sleep(100);
			if (m_opePB.PlaybackControl(PB_CONTROL_TYPE_SINGLE_FRAME, 0, lOutValue))
			{
				m_bIsSingleFrame = TRUE;
			}
			//End
		}
	}
	else
	{
		if (m_opePB.PlaybackControl(PB_CONTROL_TYPE_SINGLE_FRAME_RESTORE, 0, lOutValue))
		{
			m_bIsSingleFrame = FALSE;
		}
	}

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	// 向父窗口发送单帧回放消息.
	if (!bSynPlay)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_SINGLEFRAME, (WPARAM)m_unWndID, (LPARAM)bPlay);
	}
}

/**   @fn          IsSingleFrame
 *    @brief   	   是否正在单帧回放.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:是,FALSE:否.
 */
BOOL CPlaybackWnd::IsSingleFrame() const
{
	return m_bIsSingleFrame;
}

/**   @fn          IsPlayback
 *    @brief   	   是否正在进行
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
BOOL CPlaybackWnd::IsPlayback()
{
	if (m_hPlayback != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

/**   @fn          IsUsed
 *    @brief   	   是否已经使用过该窗口.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-是,FALSE-否.
 */
BOOL CPlaybackWnd::IsUsed()
{
	return m_bUsed;
}

/**   @fn          Invalidate
 *    @brief   	   重置回放.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::Invalidate()
{
	m_opePB.InvalidateWindow();
}

/**   @fn          GetCameraID
 *    @brief   	   获取监控点ID.
 *    @param[in]   
 *    @param[in]   
 *    @return      监控点ID.
 */
int CPlaybackWnd::GetCameraID()
{
	return m_nCameraID;
}

/**   @fn          GetRevInfo
 *    @brief   	   获取保留字段信息.
 *    @param[in]   
 *    @param[in]   
 *    @return      保留字段.
 */
DWORD CPlaybackWnd::GetRevInfo()
{
	return m_dwRev;
}

/**   @fn          GetRight
 *    @brief   	   获取权限.
 *    @param[in]   
 *    @param[in]   
 *    @return      权限值.
 */
DWORD CPlaybackWnd::GetRight()
{
	CString str;
	str.Format(_T("@z GetRight:%ld"), m_dwRight);
	OutputDebugString(str);
	return m_dwRight;
}

BOOL CPlaybackWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, UI_PLAYWND_SPACE);

	DrawBorder(m_clrBorder);
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

// 查找文件失败.
LRESULT CPlaybackWnd::OnMsgFindFail(WPARAM wParam, LPARAM lParam)
{
	// 文件列表指针.
	LPB_FILEINFO pFileList = (LPB_FILEINFO)wParam;

	if (pFileList != NULL)
	{
		delete[] pFileList;
		pFileList = NULL;
	}

	SetTitleText(_T(""));

	ShowErrorMsg(_T("查找文件失败"));

	// 向父窗口发送消息.
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_FAIL, (WPARAM)m_unWndID, (LPARAM)m_nCameraID);
	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);
	return 0;
}

// 查找文件成功.
LRESULT CPlaybackWnd::OnMsgFindSuccess(WPARAM wParam, LPARAM lParam)
{
	// 文件个数.
	int nFileCount = (int)lParam;
	// 文件列表指针.
	LPB_FILEINFO pFileList = (LPB_FILEINFO)wParam;

	// 加入文件列表.
	for (int i=0; i < nFileCount; i++)
	{
		if (pFileList != NULL)
		{
			pFileList[i].emPlayType = m_stLoginInfo.emPlayType;
			pFileList[i].emDevType = m_stLoginInfo.emDevType;
			if (PB_TYPE_KMS == m_stLoginInfo.emPlayType)
			{
				pFileList[i].stStartTime = m_stPlayTimeParam.stStartTime;
				pFileList[i].stEndTime = m_stPlayTimeParam.stStopTime;
			}
			m_OrderPlay.AddFile(pFileList[i]);
		}
	}
	// 设置当前播放序号.
	int nPlayIndex = m_bReversePlay ? (nFileCount > 0 ? nFileCount - 1 : 0) : 0;
	m_OrderPlay.SetCurPlayIndex(nPlayIndex);
	if (pFileList != NULL)
	{   //获取URL后将指针删除；这里需要注意的是指针的作用。（Machieal 2016-11-1）
        strcpy_s(m_stPlayTimeParam.szPlayUrl, sizeof(m_stPlayTimeParam.szPlayUrl), pFileList[nPlayIndex].szPlayUrl);
		delete[] pFileList;
		pFileList = NULL;

		// 向父窗口发送消息.
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_SUCCESS, (WPARAM)m_unWndID, (LPARAM)m_nCameraID);
	}
	return 0;
}

// 回放成功.
LRESULT CPlaybackWnd::OnMsgPlaySuccess(WPARAM wParam, LPARAM lParam)
{
	// 播放句柄.
	m_hPlayback = (HANDLE)wParam;

	//保存回放参数
	m_bPause = FALSE;
	m_bSoundOpen = FALSE;
	m_bSaveData = FALSE;
	m_bSingleSound = FALSE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_emSynPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_tStartTime = m_emPbType==PLAY_TYPE_TIME?m_stPlayTimeParam.stStartTime:m_stPlayFileParam.stFileInfo.stStartTime;
	m_tEndTime = m_emPbType==PLAY_TYPE_TIME?m_stPlayTimeParam.stStopTime:m_stPlayFileParam.stFileInfo.stEndTime;
	CTime tBeginTime(m_tStartTime.dwYear, m_tStartTime.dwMonth, m_tStartTime.dwDay, m_tStartTime.dwHour, m_tStartTime.dwMinute, m_tStartTime.dwSecond);
	CTime tEndTime(m_tEndTime.dwYear, m_tEndTime.dwMonth, m_tEndTime.dwDay, m_tEndTime.dwHour, m_tEndTime.dwMinute, m_tEndTime.dwSecond);
	m_lEndTime = (LONG)tEndTime.GetTime();
	m_lBeginTime = (LONG)tBeginTime.GetTime();

    //获取设备录像相关能力
    if (m_stLoginInfo.emDevType == PB_DEVICE_HIKVISION && m_stLoginInfo.emPlayType == PB_TYPE_DVR)
    {
        GetDeviceRecordAbility();
    }

    // 更新窗口标题.
    PostMessage(WM_UPDATE_MSG, 0, 0);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), MSG_PLAYBACK_STATE, 1, 0);
	OpenPlaySound(50);
	return 0;
}

// 回放失败.
LRESULT CPlaybackWnd::OnMsgPlayFail(WPARAM wParam, LPARAM lParam)
{
	// 播放句柄.
	m_hPlayback = NULL;

	ShowErrorMsg(_T("回放失败"));

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), MSG_PLAYBACK_STATE, 0, 0);
	OutputDebugString(_T("@z Playback fail"));
	return 0;
}

/**   @fn          ShowErrorMsg
 *    @brief   	   显示错误信息.
 *    @param[in]   strDefMsg:默认信息.
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::ShowErrorMsg(const CString& strDefMsg)
{
	DWORD dwError = m_opePB.GetSDKLastError();

	switch (dwError)
	{
		// 不支持的操作.
	case PB_UNSUPPORT_OPERATION:
		{
			SetStopTextShow(_T("该操作不支持"));
		}
		break;

		// 不支持的倒放类型.
	case PB_UNSUPPORT_REVPLAY_TYPE:
		{
			SetStopTextShow(_T("该设备类型或存储类型不支持倒放"));
		}
		break;

		// 不支持按卡号查询.
	case PB_UNSUPPORT_CARDNUM:
		{
			SetStopTextShow(_T("不支持按卡号查询"));
		}
		break;

	default:
		{
			SetStopTextShow(strDefMsg);
		}
		break;
	}
}

// 带宽连接控制.
LRESULT CPlaybackWnd::OnMsgBWLimit(WPARAM wParam, LPARAM lParam)
{
	SetStopTextShow(_T("带宽连接数达到上限,无权限回放"));
	return 0;
}

// 停止回放.
LRESULT CPlaybackWnd::OnMsgStopPlay(WPARAM wParam, LPARAM lParam)
{
	TRACE("@z OnMsgStopPlay");
	// 带宽控制类型.
	BWControlType emBwControl = (BWControlType)wParam;
	// 是否是自动停止.
	//BOOL bAutoStop = (BOOL)lParam;

	//恢复参数
	m_bPause = FALSE;
	m_bIsSingleFrame = FALSE;
	m_bSoundOpen = FALSE;
	m_bSaveData = FALSE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_emSynPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_hPlayback = NULL;
	m_lOsdTime = 0;

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	// 带宽控制停止.
	if (BWCONTROL_CROWDING == emBwControl)
	{
		SetStopTextShow(_T("带宽连接数达到上限,被高级别用户挤占"));
	}
	else if (BWCONTROL_RECONNECT == emBwControl)
	{
		SetStopTextShow(_T("带宽连接数达到上限"));
	}
	else if (SHOW_ERROR_MSG == emBwControl)
	{
		ShowErrorMsg(_T("回放失败"));
	}
	else
	{
		//刷新视频界面
		if (m_pPlayVedioWnd != NULL)
		{
			m_pPlayVedioWnd->SetStatePicHide();
		}
	}

	// 如果是文件播放模式,则自动播放下一个文件.
	/*if ((bAutoStop  &&  PLAY_TYPE_FILE == m_emPbType)  ||  (bAutoStop && m_bPlayNextFile)) 
	{
		LPOrderPlayData pData = new OrderPlayData;
		pData->nCameraID = m_nCameraID;
		pData->strCamName = m_strChanName;
		pData->nChannel = m_lChannel;
		pData->nWndID = m_unWndID;
		pData->dwRight = m_dwRight;
		pData->dwRev = m_dwRev;
		memcpy_s(&pData->stLoginInfo, sizeof(PB_LOGININFO), &m_stLoginInfo, sizeof(PB_LOGININFO));
		int nCurIndex = 0;
		if (m_OrderPlay.GetCurPlayIndex(nCurIndex))
		{
			int nIndex = m_bReversePlay ? nCurIndex - 1 : nCurIndex + 1;
			PB_FILEINFO stFileInfo;
			if (m_OrderPlay.GetFileInfo(nIndex, stFileInfo))
			{
				memcpy_s(&pData->stFileInfo, sizeof(PB_FILEINFO), &stFileInfo, sizeof(PB_FILEINFO));
				::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_NEXT_FILE, (WPARAM)pData, (LPARAM)nIndex);
			}
		}
	}*/

	// 设置回放停止.
	if (m_pToolBarWnd != NULL)
	{
		m_pToolBarWnd->ShowWindow(SW_HIDE);
	}

	if (m_pbtnClose != NULL)
	{
		m_pbtnClose->ShowWindow(SW_HIDE);
	}

	return 0;
}

// 正在连接设备.
LRESULT CPlaybackWnd::OnMsgPlaying(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;
	if (PB_TYPE_DVR == m_stLoginInfo.emPlayType)
	{
		strMsg = _T("正在连接设备...");
	}
	else
	{
		strMsg = _T("正在连接服务器...");
	}
	SetStopTextShow(strMsg);
	return 0;
}

/**   @fn          GetFileInfo
 *    @brief   	   获取文件列表引用.
 *    @param[in]   
 *    @param[in]   
 *    @return      文件列表的引用.
 */
std::vector<PB_FILEINFO>& CPlaybackWnd::GetFileInfo()
{
	return m_OrderPlay.GetFileInfo();
}

/**   @fn          SetFileInfo
 *    @brief   	   设置文件列表.
 *    @param[in]  
 *    @param[in]   vFileInfo:文件列表引用.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPlaybackWnd::SetFileInfo(const std::vector<PB_FILEINFO>& vFileInfo)
{
	return m_OrderPlay.SetFileInfo(vFileInfo);
}

/**   @fn          GetFileInfo
 *    @brief   	   获取单个文件信息.
 *    @param[in]   nIndex:文件索引.
 *    @param[out]  stFileInfo:文件信息. 
 *    @return      文件信息引用.
 */
BOOL CPlaybackWnd::GetFileInfo(int nIndex, PB_FILEINFO& stFileInfo)
{
	return m_OrderPlay.GetFileInfo(nIndex, stFileInfo);
}

// 设置回放进度成功.
LRESULT CPlaybackWnd::OnMsgLocatePlaySuccess(WPARAM wParam, LPARAM lParam)
{
	// 恢复非暂停状态.
	m_bPause = FALSE;
	m_bIsSingleFrame = FALSE;

	m_tStartTime = m_emPbType==PLAY_TYPE_TIME?m_stPlayTimeParam.stStartTime:m_stPlayFileParam.stFileInfo.stStartTime;
	m_tEndTime = m_emPbType==PLAY_TYPE_TIME?m_stPlayTimeParam.stStopTime:m_stPlayFileParam.stFileInfo.stEndTime;
	CTime tBeginTime(m_tStartTime.dwYear, m_tStartTime.dwMonth, m_tStartTime.dwDay, m_tStartTime.dwHour, m_tStartTime.dwMinute, m_tStartTime.dwSecond);
	CTime tEndTime(m_tEndTime.dwYear, m_tEndTime.dwMonth, m_tEndTime.dwDay, m_tEndTime.dwHour, m_tEndTime.dwMinute, m_tEndTime.dwSecond);
	m_lEndTime = (LONG)tEndTime.GetTime();
	m_lBeginTime = (LONG)tBeginTime.GetTime();

	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return 0;
}

// 设置回放进度失败.
LRESULT CPlaybackWnd::OnMsgLocatePlayFail(WPARAM wParam, LPARAM lParam)
{
	TRACE("@z LocatPlayTime false");

	m_bPause = FALSE;
	m_bIsSingleFrame = FALSE;
	m_bSoundOpen = FALSE;
	m_bSaveData = FALSE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_emSynPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_hPlayback = NULL;
	m_lOsdTime = 0;

	ShowErrorMsg(_T("回放失败"));
	
	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return 0;
}

// 带宽控制服务器请求停止回放.
LRESULT CPlaybackWnd::OnMsgBwStopPlay(WPARAM wParam, LPARAM lParam)
{
	PLAYBACKDLL_INFO(_T("StopPlayCam"));
	StopPlayback(FALSE, BWCONTROL_RECONNECT);

	return 0;
}

/**   @fn           GetPlayTimeInfo
 *    @brief   	   	获取时间信息
 *    @param[in]   
 *    @param[in]   
 *    @return       时间信息引用.
 */
PLAYBACK_TIME_INFO& CPlaybackWnd::GetPlayTimeInfo()
{
	return m_stPlayTimeParam;
}

/**   @fn           GetPlayFileInfo
 *    @brief   	   	获取文件信息
 *    @param[in]   
 *    @param[in]   
 *    @return       文件信息引用.
 */
PLAYBACK_FILE_INFO& CPlaybackWnd::GetPlayFileInfo()
{
	return m_stPlayFileParam;
}

/**   @fn           GetFindInf
 *    @brief   	   	获取文件查找信息
 *    @param[in]   
 *    @param[in]   
 *    @return       文件查找信息引用.
 */
FINDFILE_PARAM& CPlaybackWnd::GetFindInf()
{
	return m_stFindParam;
}

/**   @fn          GetCamName
 *    @brief   	   获取监控点名称.
 *    @param[in]   
 *    @param[in]   
 *    @return      监控点名称.
 */
CString& CPlaybackWnd::GetCamName()
{
	return m_strChanName;
}

/**   @fn          GetChannel
 *    @brief   	   获取通道号.
 *    @param[in]   
 *    @param[in]   
 *    @return      通道号.
 */
LONG CPlaybackWnd::GetChannel()
{
	return m_lChannel;
}

/**   @fn          GetPlayType
 *    @brief   	   获取回放类型.
 *    @param[in]   
 *    @param[in]   
 *    @return      回放类型.
 */
ENUM_PLAY_TYPE CPlaybackWnd::GetPlayType()
{
	return m_emPlayType;
}

/**   @fn          GetLoginInfo
 *    @brief   	   获取登录信息.
 *    @param[in]   
 *    @param[in]   
 *    @return      登录信息引用.
 */
 PB_LOGININFO& CPlaybackWnd::GetLoginInfo()
{
	return m_stLoginInfo;
}

/**   @fn          HideToolBar
 *    @brief   	   隐藏工具栏.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::HideToolBar()
{
	if (m_pToolBarWnd != NULL  &&  m_pToolBarWnd->IsWindowVisible())
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		GetClientRect(m_rcClient);
		m_rcClient.left += 1;
		m_rcClient.right -= 1;
		m_rcClient.bottom -= 1;
		if (m_rcClient.PtInRect(pt))
		{
			return;
		}

		m_pToolBarWnd->ShowWindow(SW_HIDE);

		if (m_pbtnClose)
		{
			m_pbtnClose->ShowWindow(SW_HIDE);
		}
	}
}

/**   @fn          OnMsgMouseLeaveMiniToolbar
 *    @brief   	   鼠标离开工具栏消息.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgMouseLeaveMiniToolbar(WPARAM wParam, LPARAM lParam)
{
	HideToolBar();
	return 0;
}

/**   @fn          OnMsgMouseMoveMiniToolbar
 *    @brief   	   鼠标进入工具栏消息.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgMouseMoveMiniToolbar(WPARAM wParam, LPARAM lParam)
{
	if (m_hPlayback != NULL)
	{
		if (m_pToolBarWnd != NULL  &&  !m_pToolBarWnd->IsWindowVisible())
		{
			m_pToolBarWnd->ShowWindow(SW_SHOW);
		}

		if (m_pbtnClose && !m_pbtnClose->IsWindowVisible())
		{
			m_pbtnClose->ShowWindow(SW_SHOW);
		}
	}
	return 0;
}

/**   @fn          OnMsgToolButtonClick
 *    @brief   	   工具栏按钮点击消息.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgToolButtonClick(WPARAM wParam, LPARAM lParam)
{
	switch ((TOOLBAR_CTRL_ID)wParam)
	{
		// 扩展按钮.
	case TOOL_EXPAND_ID:
		{
			OnMiniToolbarControlExpand();
		}
		break;
		// 抓图.
	case TOOL_CAPTURE_ID:
		{
			OnMiniToolbarControlCapture();
		}
		break;
		// 保存.
	case TOOL_SAVEDATA_ID:
		{
			OnMiniToolbarControlSave();
		}
		break;  
		// 停止.
	case TOOL_STOP_ID:
		{
			StopPlayback();
		}
		break;  
		// 减速.
	case TOOL_MINI_SLOW_ID:
		{
			PlaySlow();
		}
		break;
		// 快进.
	case TOOL_MINI_FAST_ID:
		{
			PlayFast();
		}
		break;
		// 单帧播放.
	case TOOL_FRAME_ID:
		{
			PlaySingleFrame(TRUE);
		}
		break; 
		// 播放.
	case TOOL_PLAY_ID:
		{
			OnMiniToolbarControlPlay();
		}
		break;
		// 按时间下载.
	case TOOL_DOWNLOAD_ID:
		{
			OnMiniToolbarDownload();
		}
		break;
		// 音频.
	case TOOL_SOUND_ID:
		{	
			if (!m_bSoundOpen)
			{
				OpenPlaySound();
			}
			else
			{
				ClosePlaySound();
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

/**   @fn          PlaySlow
 *    @brief   	   减速播放.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::PlaySlow()
{
	int nSpeed = m_emPlaySpeed;
	nSpeed--;
	nSpeed = (nSpeed < PB_CONTROL_TYPE_SLOW16) ? PB_CONTROL_TYPE_SLOW16 : nSpeed;
	SetPlaySpeed((ENUM_CONTROL_TYPE)nSpeed);
}

/**   @fn          PlayFast
 *    @brief   	   加速播放.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::PlayFast()
{
	int nSpeed = m_emPlaySpeed;
	nSpeed++;
	nSpeed = (nSpeed > PB_CONTROL_TYPE_FAST16) ? PB_CONTROL_TYPE_FAST16 : nSpeed;
	SetPlaySpeed((ENUM_CONTROL_TYPE)nSpeed);
}

/**   @fn          OnMsgToolBarExpandClose
 *    @brief   	   响应扩展工具栏关闭消息.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgToolBarExpandClose(WPARAM wParam, LPARAM lParam)
{
	if (m_pToolBarExpandWnd)
	{
		m_pToolBarExpandWnd->DestroyWindow();
		delete m_pToolBarExpandWnd;
		m_pToolBarExpandWnd = NULL;
	}

	return 0;
}

/**   @fn          OnMiniToolbarControlPlay
 *    @brief   	   工具栏操作-回放.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::OnMiniToolbarControlPlay()
{
	// 如果正在进行单帧回放.
	if (m_bIsSingleFrame)
	{
		// 单帧恢复.
		PlaySingleFrame(FALSE);
	}
	else
	{
		if (m_hPlayback)
		{
			if (m_bPause)
			{
				RePlayback();
			}
			else
			{
				PausePlayback();
			}
		}
	}
}

/**   @fn          OnMiniToolbarDownload
 *    @brief   	   工具栏操作-按时间下载.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::OnMiniToolbarDownload()
{

	if (m_pToolBarWnd != NULL)
	{
		m_pToolBarWnd->ShowWindow(SW_HIDE);
	}
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MINITOOLBAR_DOWNLOAD, (WPARAM)m_unWndID, 0);
}

/**   @fn          OnMiniToolbarControlSave
 *    @brief   	   工具栏操作保存数据.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::OnMiniToolbarControlSave()
{
	if (!theApp.IsCamSaveRight(m_dwRight))
	{
		MsgBoxApi_Show(_T("当前用户不具有剪辑权限!"), MSGBOX_TYPE_WARNING);
		return;
	}

	if (!m_bSaveData)
	{
		m_strFilePath.Empty();
		int nRet = SaveData(m_strFilePath);
		if (-1 == nRet)
		{
			MsgBoxApi_Show(_T("磁盘空间不足,剪辑失败!"), MSGBOX_TYPE_WARNING);
		}
		else if (0 == nRet)
		{
			if (theApp.m_bIsIEProtected)
			{
				MsgBoxApi_Show(_T("剪辑失败,请使用管理员身份运行IE,或降低权限等级"), MSGBOX_TYPE_ERROR);
			}
			else
			{
				MsgBoxApi_Show(_T("剪辑失败!"), MSGBOX_TYPE_ERROR);
			}
		}
	}
	else
	{
		if (!StopSaveData())
		{
			MsgBoxApi_Show(_T("停止剪辑失败!"), MSGBOX_TYPE_ERROR);
		}
		else
		{
			if (!m_strFilePath.IsEmpty())
			{
				if (m_pSaveData != NULL)
				{
					m_pSaveData->DestroyWindow();
					delete m_pSaveData;
					m_pSaveData = NULL;
				}

				if (NULL == m_pSaveData)
				{
					m_pSaveData = new CSaveDataDlg;
					m_pSaveData->Create(CSaveDataDlg::IDD, this);
					m_pSaveData->LoadFile(m_strFilePath.GetBuffer());
					m_pSaveData->ShowWindow(SW_SHOW);
				}
			}
		}
	}
}

/**   @fn          OnMiniToolbarControlCapture
 *    @brief   	   工具栏操作-截图.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::OnMiniToolbarControlCapture()
{
    if (!IsPlayback())
    {
        return;
    }

	if (!theApp.IsCamCaptureRight(m_dwRight))
	{
		MsgBoxApi_Show(_T("当前用户不具有抓图权限!"), MSGBOX_TYPE_WARNING);
		return;
	}

	CString strPic;
	if (CapturePic(strPic))
	{
		if (m_pCaptureTip)
		{
			m_pCaptureTip->DestroyWindow();
			delete m_pCaptureTip;
			m_pCaptureTip = NULL;
		}

		//创建图片展示窗口
		if (NULL == m_pCaptureTip)
		{
			m_pCaptureTip = new CCaptureTipDlg;
			m_pCaptureTip->Create(CCaptureTipDlg::IDD, this);
			m_pCaptureTip->LoadPic(strPic.GetBuffer());
			m_pCaptureTip->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		CString strMsg;
		strMsg.Format(_T("窗口%02d:抓图失败"), m_unWndID + 1);
		MsgBoxApi_Show(strMsg.GetBuffer(), MSGBOX_TYPE_ERROR);
	}
}

/**   @fn          OnMiniToolbarControlExpand
 *    @brief   	   工具栏操作-扩展.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::OnMiniToolbarControlExpand()
{
	if (NULL == m_pToolBarExpandWnd)
	{
		m_pToolBarExpandWnd = new CPlayMiniToolBarExpandWnd(m_bSoundOpen, m_bSaveData);
        if (NULL != m_pToolBarExpandWnd )
        {
            m_pToolBarExpandWnd->CreateFloatWindow(this);
        }
		CRect rcVoice;
		if (m_pToolBarWnd != NULL  &&  m_pToolBarExpandWnd != NULL)
		{
			int nWidth = m_pToolBarExpandWnd->GetWndWidth();
			m_pToolBarWnd->GetWindowRect(rcVoice);
			m_pToolBarExpandWnd->MoveWindow(
				rcVoice.right - nWidth, 
				rcVoice.bottom + 2, 
				m_pToolBarExpandWnd->GetWndWidth(),
				m_pToolBarExpandWnd->GetWndHeight());				
		}
	}
}

/**   @fn          UpdateButtonTips
 *    @brief   	   更新按钮tips.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::UpdateButtonTips()
{
	if (m_pToolBarWnd != NULL)
	{
		m_pToolBarWnd->UpdateButtonTips();
	}
	if (m_pToolBarExpandWnd != NULL)
	{
		m_pToolBarExpandWnd->UpdateButtonTips();
	}
}

/**   @fn          GetPlaybackInfo
 *    @brief   	   获取回放状态信息.
 *    @param[in]   stData:状态信息.
 *    @param[in]   
 *    @return      
 */
BOOL CPlaybackWnd::GetPlaybackInfo(PlaybackStatData& stData)
{
	stData.bIsPlaying = (NULL == m_hPlayback) ? FALSE : TRUE;
	stData.bIsSaving = m_bSaveData;
	stData.nCamID = m_nCameraID;
	stData.dwRev = m_dwRev;

	return TRUE;
}

/**   @fn          SetPlayHandle
 *    @brief   	   设置回放句柄.
 *    @param[in]   hPlayHandle:回放句柄.
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::SetPlayHandle(HANDLE hPlayHandle)
{
	m_opePB.SetPlayHandle(hPlayHandle);
}

/**   @fn          ThreadFuncCheckSpace
 *    @brief   	   检查磁盘空间大小.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
DWORD WINAPI CPlaybackWnd::ThreadFuncCheckSpace(LPVOID pParam)
{
	LPThreadDataCheckSpace pData = static_cast<LPThreadDataCheckSpace>(pParam);
	if (NULL == pData)
	{
		return 0;
	}

	CPlaybackWnd* pWnd = static_cast<CPlaybackWnd*>(pData->pParam);
	if (NULL == pWnd)
	{
		return 0;
	}

	while (TRUE)
	{
		TRACE("@z checking space");
		if (!CheckFilePathSpace(pData->strFilePath))
		{
			// 磁盘空间不足.
			pWnd->PostMessage(MSG_CHECK_SPACE_NOTENOUGH, 0, 0);
			PLAYBACKDLL_ERROR(_T("@z 磁盘空间不足!"));
			break;
		}

		if (WAIT_OBJECT_0 == WaitForSingleObject(pWnd->m_hStopCheck, 3000))
		{
			TRACE("@z stop checking space");
			break;
		}
	}

	return 0;
}

/**   @fn          OnMsgCheckSpaceNotEnough
 *    @brief   	   磁盘空间不足响应消息.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgCheckSpaceNotEnough(WPARAM wParam, LPARAM lParam)
{
	CloseHandle(m_hCheckSpaceThread);
	m_hCheckSpaceThread = NULL;

	StopSaveData();

	MsgBoxApi_Show(_T("磁盘空间不足,剪辑失败!"), MSGBOX_TYPE_WARNING);

	return 0;
}

/**   @fn          OnMsgFindNoFile
 *    @brief   	   响应没有找到符合条件的文件.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgFindNoFile(WPARAM wParam, LPARAM lParam)
{
	// 文件列表指针.
	LPB_FILEINFO pFileList = (LPB_FILEINFO)wParam;

	if (pFileList != NULL)
	{
		delete[] pFileList;
		pFileList = NULL;
	}

	SetTitleText(_T(""));
	SetStopTextShow(_T("没有找到符合条件的文件"));
	// 更新窗口标题.
	PostMessage(WM_UPDATE_MSG, 0, 0);
	
		// 向父窗口发送消息.
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_FAIL, (WPARAM)m_unWndID, (LPARAM)m_nCameraID);

	return 0;
}

/**   @fn          OnMsgInvalidateWindow
 *    @brief   	   刷新窗口响应消息.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgInvalidateWindow(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("@z OnMsgInvalidateWindow"));
	SetTitleText(_T(""));
	m_bUsed = FALSE;

	return 0;
}

/**   @fn          IsReversePlay
 *    @brief   	   是否是倒放状态.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-是,FALSE-否.
 */
BOOL CPlaybackWnd::IsReversePlay()
{
	return m_bReversePlay;
}

/**   @fn          GetCurPlayFileIndex
 *    @brief   	   nPlayIndex:播放序号.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
BOOL CPlaybackWnd::GetCurPlayFileIndex(int& nPlayIndex)
{
	return m_OrderPlay.GetCurPlayIndex(nPlayIndex);
}

/**   @fn          SetCurPlayFileIndex
 *    @brief   	   设置当前回放序号.
 *    @param[in]   nPlayIndex:回放序号.
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::SetCurPlayFileIndex(int nPlayIndex)
{
	m_OrderPlay.SetCurPlayIndex(nPlayIndex);
}

void CPlaybackWnd::ParseDeviceRecordAbility()
{
    CMarkup xml;
    if(!xml.SetDoc(m_strAbilityXml.GetBuffer(0)))
    {
        return;
    }

    if (!xml.FindElem(_T("RecordAbility version=\"2.0\"")))
    {
        return;
    }
    xml.IntoElem();

    if (!xml.FindElem(_T("PlayBack")))
    {
        PLAYBACKDLL_ERROR(_T("not Find Elem Playback"));
        return;
    }
}

void CPlaybackWnd::GetDeviceRecordAbility()
{
    char* pOutBuf = new char[XML_ABILITY_OUT_LEN];
    memset(pOutBuf, 0, XML_ABILITY_OUT_LEN);
    if (!m_opePB.GetCompressCfgAbility(m_stLoginInfo, m_stFindParam.lChannel, pOutBuf))
    {
        m_strAbilityXml.Empty();
        delete[] pOutBuf;
        PLAYBACKDLL_ERROR(_T("获取设备录像能力失败!"));
        return;
    }

    TCHAR *chTmp = new TCHAR[XML_ABILITY_OUT_LEN];
    MByteToWChar(pOutBuf,chTmp,XML_ABILITY_OUT_LEN);
    m_strAbilityXml.Format(_T("%s"),chTmp);
    delete []pOutBuf;
    delete []chTmp;

    ParseDeviceRecordAbility();
}
