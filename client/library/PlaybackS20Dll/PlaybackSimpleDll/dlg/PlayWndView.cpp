// PlayWndDlg.cpp : ʵ���ļ�
//
//lint -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "PlayWndView.h"


// CPlayWndDlg �Ի���

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

	// TODO:  �ڴ������ר�õĴ�������
	for (int i=0; i < theApp.m_emLayoutStyle; i++)
	{
		m_pPlayWnd[i] = new CPlaybackWnd(m_hDlg, this);
		m_pPlayWnd[i]->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PLAYBACK_WND_ID+i);
		m_pPlayWnd[i]->InitWnd(i, theApp.m_emLayoutStyle);
		m_pPlayWnd[i]->ShowTitle((BOOL)theApp.m_stPlaybackParam.nShowWndTitle);
		m_pPlayWnd[i]->ShowWindow(SW_HIDE);
	}

	//���ó�ʼ����Ϊ�ķ���
	SetWndLayout(theApp.m_emLayoutStyle >= LAYOUT_WND_4 ? LAYOUT_WND_4 : theApp.m_emLayoutStyle);

	m_bMove = TRUE;

	// ���ó�ʼ����ѡ��߿�.
	if (m_pPlayWnd[m_unSelectWnd])
	{
		m_pPlayWnd[m_unSelectWnd]->SetBorderClr(TRUE);
	}

	return 0;
}

void CPlayWndView::OnDestroy()
{
	CWnd::OnDestroy();

	//�ر�ͬ����ʱ��
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

//���ô��ڷ���
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

//���յ�������ѡ�е���Ϣ
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

//���յ������ڵ�˫����Ϣ
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

//��������ȫ����Ϣ
LRESULT CPlayWndView::OnMsgFullScreen(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

//�������ڻط�״̬�ı���Ϣ
LRESULT CPlayWndView::OnMsgPlayStateUpdate(WPARAM wParam, LPARAM lParam)
{
	if (m_unSelectWnd == (UINT)wParam)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_UPDATE, (WPARAM)m_pPlayWnd[m_unSelectWnd], 0);
	}

	return 0;
}

//ʱ��ط�
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

//�ļ��ط�
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

//ֹͣ�ط�
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

// ֹͣ�طż�ص�.
void CPlayWndView::StopPlayCam(int nCameraID)
{
	for (int i=0; i<MAX_PLAYWND_NUM; i++)
	{
		if (m_pPlayWnd[i])
		{
			// ����Ǹü�ص㲢�����ڽ��лط�.
			if (nCameraID == m_pPlayWnd[i]->GetCameraID()  &&  m_pPlayWnd[i]->IsPlayback())
			{
				PLAYBACKDLL_INFO(_T("StopPlayCam, nCameraID:%d"), nCameraID);
				m_pPlayWnd[i]->StopPlayback(FALSE, BWCONTROL_CROWDING);
				break;
			}
		}
	}
}

// ֹͣ�طż�ص�.
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
 *    @brief   	   ��ȡ�ط�״̬��Ϣ.
 *    @param[in]   nWndNo:���ں�.
 *    @param[out]  stData:״̬��Ϣ.   
 *    @return      TRUE;�ɹ�,FALSE:ʧ��.
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

//ֹͣȫ���ط�
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

//�طŶ�λ
BOOL CPlayWndView::SetPlayTime(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nWndID)
{
    //TODO:(Machieal) 2016-11-1
    //���ں�����ĳ�nWndID < 1��web����0���޷���λ
	if (nWndID < 0 || nWndID > MAX_PLAYWND_NUM)  
	{
		return FALSE;
	}

	//ͬ������
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

//��ť����¼�
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
	case TOOL_STOPALL_ID://ֹͣȫ��
		{
			StopAll();
		}
		break;  
	case TOOL_SLIDER_VOLUMN://����
		{
			m_pPlayWnd[m_unSelectWnd]->SetPlayVolume((DWORD)lValue);
		}
		break;
	case TOOL_SLIDER_SPEED://�ط��ٶ�
		{
		    ENUM_CONTROL_TYPE emPlaySpeed = (ENUM_CONTROL_TYPE)lValue;
			m_pPlayWnd[m_unSelectWnd]->SetPlaySpeed(emPlaySpeed);
		}
		break;
	case TOOL_PLAY_ID:	// �ظ��ط�.
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

//������ʾ�ַ�
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

//����OSD�ص�����
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == IDT_SYNPLAY_TIMER)
	{
		LONG lMinOsdTime = 0;
		LONG lMaxOsdTime = 0;
		//��ȡ��ǰ��Сosd
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

		//ͬ��
		for (int i=0; i<(int)m_curLayWnd; i++)
		{
			if (m_pPlayWnd[i]->m_hPlayback && m_pPlayWnd[i]->m_bPause)
			{
				// ������ڵ���.
				if (m_bReversePlay)
				{
					if (m_pPlayWnd[i]->m_lOsdTime <= lMaxOsdTime+3 && m_pPlayWnd[i]->m_lOsdTime >= lMaxOsdTime-3)
					{
						m_pPlayWnd[i]->RePlayback();
					}
				}
				// ����.
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
				// ������ڵ���.
				if (m_bReversePlay)
				{
					if (m_pPlayWnd[i]->m_lOsdTime < lMaxOsdTime - 3)
					{
						m_pPlayWnd[i]->PausePlayback();
					}
				}
				// ����.
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

//��ȡ��ǰѡ�д���������С
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

//��ȡ��ǰ������ڷ���
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
*	@brief		��ȡ��ǰѡ�еķ���.
*	@param
*	@param
*	@return
*/
int CPlayWndView::GetCurSelectedWnd()
{
	return m_unSelectWnd;
}

// �ط���һ���ļ�.
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

// �����ļ��ɹ�.
LRESULT CPlayWndView::OnMsgFindSuccess(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_SUCCESS, wParam, lParam);
	return 0;
}

// �����ļ�ʧ��.
LRESULT CPlayWndView::OnMsgFindFail(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_FAIL, wParam, lParam);
	return 0;
}

/**   @fn          GetFileInfo
 *    @brief   	   ��ȡ�ļ��б������.
 *    @param[in]   nWndID:�ļ�����ID.
 *    @param[in]   
 *    @return      �ļ��б�����.
 */
std::vector<PB_FILEINFO>& CPlayWndView::GetFileInfo(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetFileInfo();
}

/**   @fn          GetFileInfo
 *    @brief   	   ��ȡ�����ļ���Ϣ.
 *    @param[in]   nIndex:�ļ�����.
 *    @param[in]   nWndID:����ID.
 *    @param[out]  stFileInfo;�ļ���Ϣ.
 *    @return      �ļ���Ϣ����.
 */
BOOL CPlayWndView::GetFileInfo(int nWndID, int nIndex, PB_FILEINFO& stFileInfo)
{
	return m_pPlayWnd[nWndID]->GetFileInfo(nIndex, stFileInfo);
}


/**   @fn          GetCameraID
 *    @brief   	   ��ȡ��ص�ID.
 *    @param[in]   nWndID:���ں�.
 *    @param[in]   
 *    @return      ��ص�ID.
 */
int CPlayWndView::GetCameraID(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetCameraID();
}

/**   @fn          GetRevInfo
 *    @brief   	   ��ȡ�����ֶ���Ϣ.
 *    @param[in]   nWndID:���ں�.
 *    @param[in]   
 *    @return      ������Ϣ.
 */
DWORD CPlayWndView::GetRevInfo(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetRevInfo();
}

/**   @fn          GetCamName
 *    @brief   	   ��ȡ��ص�����.
 *    @param[in]   nWndID:���ں�.
 *    @param[in]   
 *    @return      ��ص�����.
 */
CString& CPlayWndView::GetCamName(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetCamName();
}

/**   @fn          GetChannel
 *    @brief   	   ��ȡͨ����.
 *    @param[in]   nWndID:���ں�.
 *    @param[in]   
 *    @return      ͨ����.
 */
LONG CPlayWndView::GetChannel(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetChannel();
}

/**   @fn          GetRight
 *    @brief   	   ��ȡȨ��.
 *    @param[in]   nWndID:���ں�.
 *    @param[in]   
 *    @return      Ȩ��.
 */
DWORD CPlayWndView::GetRight(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetRight();
}

/**   @fn          GetPlayType
 *    @brief   	   ��ȡ�ط�����.
 *    @param[in]   nWndID:���ں�.
 *    @param[in]   
 *    @return      �ط�����.
 */
ENUM_PLAY_TYPE CPlayWndView::GetPlayType(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetPlayType();
}

/**   @fn          GetLoginInfo
 *    @brief   	   ��ȡ��¼��Ϣ.
 *    @param[in]   nWndID:���ں�.
 *    @param[in]   
 *    @return      ��¼��Ϣ����.
 */
PB_LOGININFO& CPlayWndView::GetLoginInfo(int nWndID)
{
	return m_pPlayWnd[nWndID]->GetLoginInfo();
}

/**   @fn          OnMsgMiniToolbarOpenSound
 *    @brief   	   �ر��������ڵ���Ƶ.
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
 *    @brief   	   ��ʱ������.
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
 *    @brief   	   ���°�ťtips.
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
 *    @brief   	   �ط��ٶȸı�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlayWndView::OnMsgPlaybackSpeedChange(WPARAM wParam, LPARAM lParam)
{
	// ���ں�.
	int nWndID = (int)wParam;
	// �ط��ٶ�.
	ENUM_CONTROL_TYPE emSpeed = (ENUM_CONTROL_TYPE)lParam;

	// �����ͬ���طţ�����������ͬ���ı�����.
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
 *    @brief   	   ��֡�ط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlayWndView::OnMsgPlaybackSingleFrame(WPARAM wParam, LPARAM lParam)
{
	// ���ں�.
	int nWndID = (int)wParam;
	// �Ƿ���е�֡�ط�.
	BOOL bPlay = (BOOL)lParam;

	// �����ͬ���طţ��������������õ�֡�ط�.
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
 *    @brief   	   ���ûط�.
 *    @param[in]   nWndID:����ID.
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
 *    @brief   	   �Ƿ��ǵ���״̬.
 *    @param[in]   nWndID:����ID.
 *    @param[in]   
 *    @return      TRUE-��,FALSE-��.
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
 *    @brief   	   ��ȡ��ǰ�����ļ����.
 *    @param[in]   nWndID:����ID.
 *    @param[in]   nPlayIndex:�ļ����.
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
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
 *    @brief   	   ���õ�ǰ�����ļ����.
 *    @param[in]   nWndID:����ID.
 *    @param[in]   nPlayIndex:�ļ����.
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
void CPlayWndView::SetCurPlayFileIndex(int nWndID, int nPlayIndex)
{
	if (NULL == m_pPlayWnd[nWndID])
	{
		return;
	}
	
	m_pPlayWnd[nWndID]->SetCurPlayFileIndex(nPlayIndex);
}