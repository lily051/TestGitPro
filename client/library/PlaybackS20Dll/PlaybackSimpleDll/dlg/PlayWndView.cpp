// PlayWndDlg.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "PlayWndView.h"


// CPlayWndDlg 对话框

IMPLEMENT_DYNAMIC(CPlayWndView, CPlayWndView)

CPlayWndView::CPlayWndView(HANDLE hDlg, CWnd* pParent /*=NULL*/)
: m_hDlg(hDlg)
{
	for (int i=0; i<MAX_PLAYWND_NUM; i++)
	{
		m_pPlayWnd[i] = NULL;
	}
	m_curLayWnd = LAYOUT_WND_1;
	m_unSelectWnd = 0;
	m_unFullWndID = -1;
	m_bSynPlay = FALSE;
	m_bReversePlay = FALSE;
	m_bSynTimer = FALSE;
	m_bMove = TRUE;
}

CPlayWndView::~CPlayWndView()
{
	m_hDlg = NULL;
}

BEGIN_MESSAGE_MAP(CPlayWndView, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(MSG_DB_VEDIO, &CPlayWndView::OnMsgWndDbClick)
	ON_MESSAGE(MSG_SELECT_VEDIO, &CPlayWndView::OnMsgWndSelect)
	ON_MESSAGE(MSG_WND_FULLSCREEN, &CPlayWndView::OnMsgFullScreen)
	ON_MESSAGE(MSG_PLAYSTATE_UPDATE, &CPlayWndView::OnMsgPlayStateUpdate)
	ON_MESSAGE(MSG_PLAYBACK_NEXT_FILE, &CPlayWndView::OnMsgPlayNextFile)
	ON_MESSAGE(MSG_MENU_CALLBACK, OnMenuCallBack)
	ON_MESSAGE(MSG_PLAYBACK_FIND_SUCCESS, &CPlayWndView::OnMsgFindSuccess)
	ON_MESSAGE(MSG_PLAYBACK_FIND_FAIL, &CPlayWndView::OnMsgFindFail)
	//ON_MESSAGE(MSG_MINITOOLBAR_OPENSOUND, &CPlayWndView::OnMsgMiniToolbarOpenSound)
	ON_MESSAGE(MSG_MINITOOLBAR_DOWNLOAD, &CPlayWndView::OnMsgMiniToolbarDownload)
	ON_MESSAGE(MSG_PLAYBACK_SPEED_CHANGE, &CPlayWndView::OnMsgPlaybackSpeedChange)
	ON_MESSAGE(MSG_PLAYBACK_SINGLEFRAME, &CPlayWndView::OnMsgPlaybackSingleFrame)
	ON_WM_TIMER()
END_MESSAGE_MAP()


int CPlayWndView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	for (int i=0; i < theApp.m_emLayoutStyle; i++)
	{
		m_pPlayWnd[i] = new CPlaybackWnd(m_hDlg, this);
		m_pPlayWnd[i]->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PLAYBACK_WND_ID+i);
		m_pPlayWnd[i]->InitWnd(i, theApp.m_emLayoutStyle);
		m_pPlayWnd[i]->ShowTitle((BOOL)theApp.m_stPlaybackParam.nShowWndTitle);
		m_pPlayWnd[i]->ShowWindow(SW_HIDE);
	}

	//设置初始画面为四分屏
	SetWndLayout(theApp.m_emLayoutStyle >= LAYOUT_WND_4 ? LAYOUT_WND_4 : theApp.m_emLayoutStyle);

	m_bMove = TRUE;

	// 设置初始窗口选择边框.
	if (m_pPlayWnd[m_unSelectWnd])
	{
		m_pPlayWnd[m_unSelectWnd]->SetBorderClr(TRUE);
	}

	return 0;
}

void CPlayWndView::OnDestroy()
{
	CWnd::OnDestroy();

	//关闭同步定时器
	if (m_bSynTimer)
	{
		KillTimer(IDT_SYNPLAY_TIMER);
		m_bSynTimer = FALSE;
	}

	for (int i=0; i<MAX_PLAYWND_NUM; i++)
	{
		if (m_pPlayWnd[i])
		{
			m_pPlayWnd[i]->clear();
			m_pPlayWnd[i]->DestroyWindow();
			delete m_pPlayWnd[i];
			m_pPlayWnd[i] = NULL;
		}
	}

	m_bMove = TRUE;
}

void CPlayWndView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveWnd();
	}
}

void CPlayWndView::MoveWnd()
{
	CRect rcWnd;
	GetClientRect(&rcWnd);

	if (m_unFullWndID != -1)
	{
		m_pPlayWnd[m_unFullWndID]->MoveWindow(rcWnd);
		return;
	}

	int nNum = 1;
	switch (m_curLayWnd)
	{
	case LAYOUT_WND_1:
		{
			if (m_pPlayWnd[0])
			{
				m_pPlayWnd[0]->MoveWindow(rcWnd);
			}
		}
		break;
	case LAYOUT_WND_4:
		{
			nNum = 2;
		}
		break;
	case LAYOUT_WND_9:
		{
			nNum = 3;
		}
		break;
	case LAYOUT_WND_16:
		{
			nNum = 4;
		}
		break;
	default:
		break;
	}

	CRect rcPlay(rcWnd);
	int nWndLeft = 0;
	int nWndTop = 0;
	int nWndWidth = rcWnd.Width()/nNum;
	int nWndHeight = rcWnd.Height()/nNum;
	int nOrder = 0;
	for (int i=0; i<nNum; i++)
	{
		for (int j=0; j<nNum; j++)
		{
			if (m_pPlayWnd[nOrder])
			{
				//int nMoveWidth = nWndWidth;
				//int nMoveHeight = nWndHeight;
				//if (i == nNum-1)
				//{
				//	nMoveWidth += rcWnd.Width()%nNum;
				//}
				//if (j == nNum-1)
				//{
				//	nMoveHeight += rcWnd.Height()%nNum;
				//}
				m_pPlayWnd[nOrder]->MoveWindow(nWndLeft, nWndTop, nWndWidth, nWndHeight);
				nOrder++;
			}
			nWndLeft += nWndWidth;
		}
		nWndLeft = 0;
		nWndTop += nWndHeight;
	}

}

//设置窗口分屏
void CPlayWndView::SetWndLayout(PLAY_LAYOUT_WND emLayoutWnd)
{
	m_curLayWnd = emLayoutWnd;
	m_unFullWndID = -1;
	for (int i=0; i<(int)m_curLayWnd; i++)
	{
		if (m_pPlayWnd[i])
		{
			m_pPlayWnd[i]->ShowWindow(SW_SHOW);
		}
	}

	for (int i=(int)m_curLayWnd; i<16; i++)
	{
		if (m_pPlayWnd[i])
		{
			m_pPlayWnd[i]->ShowWindow(SW_HIDE);
		}
	}

	MoveWnd();
}

//接收单个窗口选中的消息
LRESULT CPlayWndView::OnMsgWndSelect(WPARAM wParam, LPARAM lParam)
{
	if (m_unSelectWnd == (UINT)lParam)
	{
		return 0;
	}

	if (m_pPlayWnd[m_unSelectWnd])
	{
		m_pPlayWnd[m_unSelectWnd]->SetBorderClr(FALSE);
	}
	m_unSelectWnd = (UINT)lParam;
	if (m_pPlayWnd[m_unSelectWnd])
	{
		m_pPlayWnd[m_unSelectWnd]->SetBorderClr(TRUE);
	}

	::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_UPDATE, (WPARAM)m_pPlayWnd[m_unSelectWnd], 0);
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_FILELIST_CHANGE, (WPARAM)m_pPlayWnd[m_unSelectWnd], (LPARAM)m_pPlayWnd[m_unSelectWnd]->GetCameraID());
	return 0;
}

//接收单个窗口的双击消息
LRESULT CPlayWndView::OnMsgWndDbClick(WPARAM wParam, LPARAM lParam)
{
	UINT nWndID = (UINT)lParam;

	if (m_unFullWndID != -1)
	{
		m_unFullWndID = -1;
		for (int i=0; i<(int)m_curLayWnd; i++)
		{
			m_pPlayWnd[i]->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_unFullWndID = nWndID;
		for (int i=0; i<(int)m_curLayWnd; i++)
		{  
			m_pPlayWnd[i]->ShowWindow(SW_HIDE);
		}
		m_pPlayWnd[m_unFullWndID]->ShowWindow(SW_SHOW);
	}

	MoveWnd();

	return 0;
}

//单个窗口全屏消息
LRESULT CPlayWndView::OnMsgFullScreen(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

//单个窗口回放状态改变消息
LRESULT CPlayWndView::OnMsgPlayStateUpdate(WPARAM wParam, LPARAM lParam)
{
	if (m_unSelectWnd == (UINT)wParam)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_UPDATE, (WPARAM)m_pPlayWnd[m_unSelectWnd], 0);
	}

	return 0;
}

//时间回放
BOOL CPlayWndView::PlaybackTime(
								const PB_LOGININFO& stLoginInfo, 
								const PLAYBACK_TIME_INFO& stPlaybackParam, 
								const FINDFILE_PARAM& stFindParam,
								CString strChanName, 
								int nCameraID, 
								int nWndID,
								DWORD dwRight,
								DWORD dwRev)
{
	int nPlayWnd = nWndID==-1?m_unSelectWnd:nWndID;
	if (nPlayWnd < 0 || nPlayWnd >= MAX_PLAYWND_NUM)
	{
		return FALSE;
	}

	if (m_pPlayWnd[nPlayWnd])
	{
		if (!m_pPlayWnd[nPlayWnd]->PlaybackByTime(stLoginInfo, stPlaybackParam, stFindParam, strChanName, nCameraID, dwRight, dwRev))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//文件回放
BOOL CPlayWndView::PlaybackFile(const PB_LOGININFO& stLoginInfo, 
								const PLAYBACK_FILE_INFO& stPlaybackParam, 
								CString strChanName, 
								int nCameraID, 
								int nWndID,
								DWORD dwRight,
								DWORD dwRev)
{
	int nPlayWnd = nWndID==-1?m_unSelectWnd:nWndID;
	m_unSelectWnd = nWndID;
	if (nPlayWnd < 0 || nPlayWnd >= MAX_PLAYWND_NUM)
	{
		return FALSE;
	}

	if (m_pPlayWnd[nPlayWnd])
	{
		if (!m_pPlayWnd[nPlayWnd]->PlaybackByFile(stLoginInfo, stPlaybackParam, strChanName, nCameraID, dwRight, dwRev))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//停止回放
void CPlayWndView::StopPlay(int nWndID)
{
	int nPlayWnd = nWndID==-1?m_unSelectWnd:nWndID;
	if (nPlayWnd < 0 || nPlayWnd >= MAX_PLAYWND_NUM)
	{
		return;
	}

	if (m_pPlayWnd[nPlayWnd])
	{
		m_pPlayWnd[nPlayWnd]->StopPlayback();
	}

}

// 停止回放监控点.
void CPlayWndView::StopPlayCam(int nCameraID)
{
	for (int i=0; i<MAX_PLAYWND_NUM; i++)
	{
		if (m_pPlayWnd[i])
		{
			// 如果是该监控点并且正在进行回放.
			if (nCameraID == m_pPlayWnd[i]->GetCameraID()  &&  m_pPlayWnd[i]->IsPlayback())
			{
				PLAYBACKDLL_INFO(_T("StopPlayCam, nCameraID:%d"), nCameraID);
				m_pPlayWnd[i]->StopPlayback(FALSE, BWCONTROL_CROWDING);
				break;
			}
		}
	}
}

// 停止回放监控点.
void CPlayWndView::StopPlayCam()
{
	for (int i=0; i<MAX_PLAYWND_NUM; i++)
	{
		if (m_pPlayWnd[i])
		{
             m_pPlayWnd[i]->IsPlayback();
		}
	}
}

/**   @fn          GetPlaybackInfo
 *    @brief   	   获取回放状态信息.
 *    @param[in]   nWndNo:窗口号.
 *    @param[out]  stData:状态信息.   
 *    @return      TRUE;成功,FALSE:失败.
 */
BOOL CPlayWndView::GetPlaybackInfo(int nWndNo, PlaybackStatData& stData)
{
	int nPlayWnd = (nWndNo == -1) ? m_unSelectWnd : nWndNo;
	if (nPlayWnd < 0 || nPlayWnd >= MAX_PLAYWND_NUM)
	{
		return FALSE;
	}

	if (NULL == m_pPlayWnd[nPlayWnd])
	{
		return FALSE;
	}

	return m_pPlayWnd[nPlayWnd]->GetPlaybackInfo(stData);
}

//停止全部回放
void CPlayWndView::StopAll()
{
	for (int i=0; i<MAX_PLAYWND_NUM; i++)
	{
		if (m_pPlayWnd[i])
		{
			m_pPlayWnd[i]->StopPlayback();
		}
	}
}

void CPlayWndView::CaptureAll()
{
    for (int i=0; i<MAX_PLAYWND_NUM; i++)
    {
        if (m_pPlayWnd[i])
        {
            m_pPlayWnd[i]->OnMiniToolbarControlCapture();
        }
    }
}

//回放定位
BOOL CPlayWndView::SetPlayTime(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nWndID)
{
    //TODO:(Machieal) 2016-11-1
    //窗口号如果改成nWndID < 1则web传入0是无法定位
	if (nWndID < 0 || nWndID > MAX_PLAYWND_NUM)  
	{
		return FALSE;
	}

	//同步调整
	if (m_bSynPlay)
	{
		for (int i=0; i<(int)m_curLayWnd; i++)
		{
			if (m_pPlayWnd[i] && m_pPlayWnd[i]->m_hPlayback)
			{
				m_pPlayWnd[i]->PlaybackSetPos(stCurTime, stStartTime, stStopTime);
			}
		}
	}
	else
	{
		if (m_pPlayWnd[nWndID])
		{
			m_pPlayWnd[nWndID]->PlaybackSetPos(stCurTime, stStartTime, stStopTime);
		}
	}

	return TRUE;
}

//按钮点击事件
void CPlayWndView::OnButtonClick(int nBtnID, LONG lValue)
{
	if (m_unSelectWnd < 0UL || m_unSelectWnd >= MAX_PLAYWND_NUM)
	{
		return;
	}

	if (m_pPlayWnd[m_unSelectWnd] == NULL)
	{
		return;
	}

	switch ((TOOLBAR_CTRL_ID)nBtnID)
	{
	case TOOL_STOPALL_ID://停止全部
		{
			StopAll();
		}
		break;  
	case TOOL_SLIDER_VOLUMN://音量
		{
			m_pPlayWnd[m_unSelectWnd]->SetPlayVolume((DWORD)lValue);
		}
		break;
	case TOOL_SLIDER_SPEED://回放速度
		{
		    ENUM_CONTROL_TYPE emPlaySpeed = (ENUM_CONTROL_TYPE)lValue;
			m_pPlayWnd[m_unSelectWnd]->SetPlaySpeed(emPlaySpeed);
		}
		break;
	case TOOL_PLAY_ID:	// 重复回放.
		{
			if (m_pPlayWnd[m_unSelectWnd] != NULL)
			{
				m_pPlayWnd[m_unSelectWnd]->PlayAgain();
			}
		}
		break;
    case TOOL_CAPTURE_ID:
        {
            CaptureAll();
        }
        break;
	default:
		break;
	}

}

//窗口提示字符
void CPlayWndView::ShowWndInfo(CString strInfo, int nWndID)
{
	int nPlayWnd = nWndID==-1?m_unSelectWnd:nWndID;
	if (nPlayWnd < 0 || nPlayWnd >= MAX_PLAYWND_NUM)
	{
		return;
	}

	if (m_pPlayWnd[nPlayWnd])
	{
		m_pPlayWnd[nPlayWnd]->ShowWndInfo(strInfo);
	}
}

//设置OSD回调函数
void CPlayWndView::SetOsdCallback(pFunMsgOsdTime pCallbackFun, LPVOID lpvoid)
{
	for (int i=0; i<MAX_PLAYWND_NUM; i++)
	{
		if (m_pPlayWnd[i])
		{
			m_pPlayWnd[i]->SetOsdCallback(pCallbackFun, lpvoid);
		}
	}
}

void CPlayWndView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDT_SYNPLAY_TIMER)
	{
		LONG lMinOsdTime = 0;
		LONG lMaxOsdTime = 0;
		//获取当前最小osd
		for (int i=0; i<(int)m_curLayWnd; i++)
		{
			if (lMinOsdTime == 0)
			{
				lMinOsdTime = m_pPlayWnd[i]->m_lOsdTime;
				lMaxOsdTime = m_pPlayWnd[i]->m_lOsdTime;
			}
			else
			{
				if (m_pPlayWnd[i]->m_lOsdTime == 0)
				{
					continue;
				}
				lMinOsdTime = lMinOsdTime < m_pPlayWnd[i]->m_lOsdTime ? lMinOsdTime : m_pPlayWnd[i]->m_lOsdTime;
				lMaxOsdTime = lMaxOsdTime > m_pPlayWnd[i]->m_lOsdTime ? lMaxOsdTime : m_pPlayWnd[i]->m_lOsdTime;
			}
		}

		if (lMinOsdTime == 0)
		{
			return;
		}

		//同步
		for (int i=0; i<(int)m_curLayWnd; i++)
		{
			if (m_pPlayWnd[i]->m_hPlayback && m_pPlayWnd[i]->m_bPause)
			{
				// 如果正在倒放.
				if (m_bReversePlay)
				{
					if (m_pPlayWnd[i]->m_lOsdTime <= lMaxOsdTime+3 && m_pPlayWnd[i]->m_lOsdTime >= lMaxOsdTime-3)
					{
						m_pPlayWnd[i]->RePlayback();
					}
				}
				// 正放.
				else
				{
					if (m_pPlayWnd[i]->m_lOsdTime <= lMinOsdTime+3 && m_pPlayWnd[i]->m_lOsdTime >= lMinOsdTime-3)
					{
						m_pPlayWnd[i]->RePlayback();
					}
				}
				
				m_pPlayWnd[i]->SetPlaySpeed(m_pPlayWnd[i]->GetSynPlaySpeed(), TRUE);
			}
			else if (m_pPlayWnd[i]->m_hPlayback && !m_pPlayWnd[i]->m_bPause)
			{
				// 如果正在倒放.
				if (m_bReversePlay)
				{
					if (m_pPlayWnd[i]->m_lOsdTime < lMaxOsdTime - 3)
					{
						m_pPlayWnd[i]->PausePlayback();
					}
				}
				// 正放.
				else
				{
					if (m_pPlayWnd[i]->m_lOsdTime > lMinOsdTime+3)
					{
						m_pPlayWnd[i]->PausePlayback();
					}
				}
			}
		}
	}
	

	CWnd::OnTimer(nIDEvent);
}

//获取当前选中窗口音量大小
BOOL CPlayWndView::GetCurSelWndVolumn(BOOL & bVoice, int& nVolumn)
{
	if (m_pPlayWnd[m_unSelectWnd])
	{
		bVoice = m_pPlayWnd[m_unSelectWnd]->m_bSoundOpen;
		nVolumn = m_pPlayWnd[m_unSelectWnd]->m_nVolumn;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//获取当前鼠标所在分屏
int CPlayWndView::GetCursorWnd()
{
	CPoint point;
	GetCursorPos(&point);

	for (int i=0; i<m_curLayWnd; i++)
	{
		if (m_pPlayWnd[i])
		{
			CRect rcWnd;
			m_pPlayWnd[i]->GetWindowRect(&rcWnd);
			if (rcWnd.PtInRect(point))
			{
				return i;
			}
		}
	}

	return -1;
}

/** @fn			GetCurSelectedWnd
*	@brief		获取当前选中的分屏.
*	@param
*	@param
*	@return
*/
int CPlayWndView::GetCurSelectedWnd()
{
	return m_unSelectWnd;
}

// 回放下一个文件.
LRESULT CPlayWndView::OnMsgPlayNextFile(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_NEXT_FILE, wParam, lParam);

	return 0;
}

LRESULT CPlayWndView::OnMenuCallBack(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MENU_CALLBACK, wParam, lParam);

	return 0;
}

// 查找文件成功.
LRESULT CPlayWndView::OnMsgFindSuccess(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_SUCCESS, wParam, lParam);
	return 0;
}

// 查找文件失败.
LRESULT CPlayWndView::OnMsgFindFail(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_FAIL, wParam, lParam);
	return 0;
}

/**   @fn          GetFileInfo
 *    @brief   	   获取文件列表的引用.
 *    @param[in]   nWndID:文件窗口ID.
 *    @param[in]   
 *    @return      文件列表引用.
 */
std::vector<PB_FILEINFO>& CPlayWndView::GetFileInfo(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetFileInfo();
}

/**   @fn          GetFileInfo
 *    @brief   	   获取单个文件信息.
 *    @param[in]   nIndex:文件索引.
 *    @param[in]   nWndID:窗口ID.
 *    @param[out]  stFileInfo;文件信息.
 *    @return      文件信息引用.
 */
BOOL CPlayWndView::GetFileInfo(int nWndID, int nIndex, PB_FILEINFO& stFileInfo)
{
	return m_pPlayWnd[nWndID]->GetFileInfo(nIndex, stFileInfo);
}


/**   @fn          GetCameraID
 *    @brief   	   获取监控点ID.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      监控点ID.
 */
int CPlayWndView::GetCameraID(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetCameraID();
}

/**   @fn          GetRevInfo
 *    @brief   	   获取保留字段信息.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      保留信息.
 */
DWORD CPlayWndView::GetRevInfo(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetRevInfo();
}

/**   @fn          GetCamName
 *    @brief   	   获取监控点名称.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      监控点名称.
 */
CString& CPlayWndView::GetCamName(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetCamName();
}

/**   @fn          GetChannel
 *    @brief   	   获取通道号.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      通道号.
 */
LONG CPlayWndView::GetChannel(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetChannel();
}

/**   @fn          GetRight
 *    @brief   	   获取权限.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      权限.
 */
DWORD CPlayWndView::GetRight(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetRight();
}

/**   @fn          GetPlayType
 *    @brief   	   获取回放类型.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      回放类型.
 */
ENUM_PLAY_TYPE CPlayWndView::GetPlayType(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetPlayType();
}

/**   @fn          GetLoginInfo
 *    @brief   	   获取登录信息.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      登录信息引用.
 */
PB_LOGININFO& CPlayWndView::GetLoginInfo(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetLoginInfo();
}

/**   @fn          OnMsgMiniToolbarOpenSound
 *    @brief   	   关闭其他窗口的音频.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayWndView::OnMsgMiniToolbarOpenSound(int nWndID)
{
	for (int i=0; i<m_curLayWnd; i++)
	{
		if (i != nWndID  &&  m_pPlayWnd[i] != NULL)
		{
			m_pPlayWnd[i]->ClosePlaySound();
		}
	}
}

/**   @fn          OnMsgMiniToolbarDownload
 *    @brief   	   按时间下载.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlayWndView::OnMsgMiniToolbarDownload(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MINITOOLBAR_DOWNLOAD, wParam, lParam);
	return 0;
}

/**   @fn          UpdateButtonTips
 *    @brief   	   更新按钮tips.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayWndView::UpdateButtonTips()
{
	for (int i = 0; i < m_curLayWnd; i++)
	{
		if (m_pPlayWnd[i] != NULL)
		{
			m_pPlayWnd[i]->UpdateButtonTips();
		}
	}
}
/**   @fn          OnMsgPlaybackSpeedChange
 *    @brief   	   回放速度改变.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlayWndView::OnMsgPlaybackSpeedChange(WPARAM wParam, LPARAM lParam)
{
	// 窗口号.
	int nWndID = (int)wParam;
	// 回放速度.
	ENUM_CONTROL_TYPE emSpeed = (ENUM_CONTROL_TYPE)lParam;

	// 如果是同步回放，则其他窗口同步改变速率.
	if (m_bSynPlay)
	{
		for (int i = 0; i < (int)m_curLayWnd; i++)
		{
			if (i == nWndID)
			{
				continue;
			}

			if (m_pPlayWnd[i] != NULL  &&  m_pPlayWnd[i]->m_hPlayback)
			{
				m_pPlayWnd[i]->SetPlaySpeed(emSpeed, TRUE);
			}
		}
	}

	return 0;
}

/**   @fn          OnMsgPlaybackSingleFrame
 *    @brief   	   单帧回放.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlayWndView::OnMsgPlaybackSingleFrame(WPARAM wParam, LPARAM lParam)
{
	// 窗口号.
	int nWndID = (int)wParam;
	// 是否进行单帧回放.
	BOOL bPlay = (BOOL)lParam;

	// 如果是同步回放，则其他窗口设置单帧回放.
	if (m_bSynPlay)
	{
		for (int i = 0; i < (int)m_curLayWnd; i++)
		{
			if (i == nWndID)
			{
				continue;
			}

			if (m_pPlayWnd[i] != NULL  &&  m_pPlayWnd[i]->m_hPlayback)
			{
				m_pPlayWnd[i]->PlaySingleFrame(bPlay, TRUE);
			}
		}
	}

	return 0;
}

/**   @fn          Invalidate
 *    @brief   	   重置回放.
 *    @param[in]   nWndID:窗口ID.
 *    @param[in]   
 *    @return      
 */
void CPlayWndView::Invalidate(int nWndID)
{
	if (m_pPlayWnd[nWndID] != NULL)
	{
		m_pPlayWnd[nWndID]->Invalidate();
	}
}

/**   @fn          IsReversePlay
 *    @brief   	   是否是倒放状态.
 *    @param[in]   nWndID:窗口ID.
 *    @param[in]   
 *    @return      TRUE-是,FALSE-否.
 */
BOOL CPlayWndView::IsReversePlay(int nWndID)
{
	if (NULL == m_pPlayWnd[nWndID])
	{
		return FALSE;
	}

	return m_pPlayWnd[nWndID]->IsReversePlay();
}

/**   @fn          GetCurPlayFileIndex
 *    @brief   	   获取当前播放文件序号.
 *    @param[in]   nWndID:窗口ID.
 *    @param[in]   nPlayIndex:文件序号.
 *    @return      TRUE-成功,FALSE-失败.
 */
BOOL CPlayWndView::GetCurPlayFileIndex(int nWndID, int& nPlayIndex)
{
	if (NULL == m_pPlayWnd[nWndID])
	{
		return FALSE;
	}

	return m_pPlayWnd[nWndID]->GetCurPlayFileIndex(nPlayIndex);
}

/**   @fn          SetCurPlayFileIndex
 *    @brief   	   设置当前播放文件序号.
 *    @param[in]   nWndID:窗口ID.
 *    @param[in]   nPlayIndex:文件序号.
 *    @return      TRUE-成功,FALSE-失败.
 */
void CPlayWndView::SetCurPlayFileIndex(int nWndID, int nPlayIndex)
{
	if (NULL == m_pPlayWnd[nWndID])
	{
		return;
	}
	
	m_pPlayWnd[nWndID]->SetCurPlayFileIndex(nPlayIndex);
}