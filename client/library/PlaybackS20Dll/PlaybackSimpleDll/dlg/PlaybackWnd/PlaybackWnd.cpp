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
			// ���ųɹ�.
		case MSG_PLAYBACK_PLAY_SUCCESS:
			{
				// ���ûطž��.
				pWnd->SetPlayHandle((HANDLE)dwParam1);
			}
			break;

			// ����ʧ�ܻ�ֹͣ����.
		case MSG_PLAYBACK_PLAY_FAIL:
		case MSG_PLAYBACK_STOPPLAY:
		case MSG_PLAYBACK_LOCATEPLAY_FAIL:
			{
				// �ÿջطž��.
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

	//�����������
	if (NULL == m_pbtnClose)
	{
		m_pbtnClose = new CButtonST_Skin;
		m_pbtnClose->Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_CLIPSIBLINGS, CRect(0,0,0,0), this, BTN_STOP_ID);
		m_pbtnClose->LoadbakeImage( 
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_BTN_CLOSE_WND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE));
		m_pbtnClose->ShowWindow(SW_HIDE);
		m_pbtnClose->SetTooltipText(_T("�ر�"));
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

	//��ʱ��
	SetTimer(TIMER_GET_PLAYPOS, GET_PLAYPOS_TIME, NULL);
    SetTimer(TIMER_GET_PLAYOSDTIME, GET_PLAYOSDTIME_TIME, NULL);

	//����ʱĬ����Ϣ���վ��Ϊ�����ھ��
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
	// TODO: �ڴ˴������Ϣ����������
	
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

	//�رն�ʱ��
	KillTimer(TIMER_GET_PLAYPOS);
    KillTimer(TIMER_GET_PLAYOSDTIME);

	//���ٱ������
	if (m_pPlayTitleWnd)
	{
		m_pPlayTitleWnd->DestroyWindow();
		delete m_pPlayTitleWnd;
	    m_pPlayTitleWnd = NULL;
	}
	//������Ƶ����
	if (m_pPlayVedioWnd)
	{
		m_pPlayVedioWnd->DestroyWindow();
		delete m_pPlayVedioWnd;
		m_pPlayVedioWnd = NULL;
	}
	// ���ٹ���������.
	if (m_pToolBarWnd != NULL)
	{
		m_pToolBarWnd->DestroyWindow();
		delete m_pToolBarWnd;
		m_pToolBarWnd = NULL;
	}
	// ����ͼƬչʾ����.
	if (m_pCaptureTip != NULL)
	{
		m_pCaptureTip->DestroyWindow();
		delete m_pCaptureTip;
		m_pCaptureTip = NULL;
	}
	// ���ټ���·��չʾ�Ի���.
	if (m_pSaveData != NULL)
	{
		m_pSaveData->DestroyWindow();
		delete m_pSaveData;
		m_pSaveData = NULL;
	}
	//���ٹرհ�ť
	if(m_pbtnClose != NULL)
	{
		m_pbtnClose->DestroyWindow();
		delete m_pbtnClose;
		m_pbtnClose = NULL;
	}

}

//���ô���ID
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

//���������ͷ�
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

//�ƶ��ؼ�
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

//��ʾ�طſؼ���������
void CPlaybackWnd::SetTitleText(CString strTitleText)
{
	if (m_pPlayTitleWnd)
	{
// 		CFont fFont;
// 		fFont.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
// 			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_MODERN, "����");
// 		m_pPlayTitleWnd->SetTextFont(&fFont);
	    m_pPlayTitleWnd->SetText(strTitleText);
	}
}

//���ݻط�״̬��ʾ����
void CPlaybackWnd::SetTitleText()
{
	if (!m_pPlayTitleWnd)
	{
		return;
	}

	// �ط�����.
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
		strPlayType = _T("�ƴ洢");
		break;

    case PB_TYPE_KMS:
        strPlayType = _T("KMS");
        break;

	case PB_TYPE_OBJECTCLOUD:
		strPlayType = _T("����洢");
		break;

	default:
		strPlayType = _T("δ֪");
		break;
	}

	CString strText;
	strText.Format(_T("����%02d: %s[%s]  "), m_unWndID + 1, (LPCTSTR)m_strChanName, (LPCTSTR)strPlayType);  //���԰�ChannelName����
	//����״̬  ƴ���ַ���
	if (m_hPlayback)
	{
		if (m_bSynPlay)
		{
			strText.AppendFormat(_T("%s"), _T(" ͬ���ط�../"));
		}
		if (m_bPause)
		{
			strText.AppendFormat(_T("%s"), _T(" �ط���ͣ../"));
		}
		if (m_bSaveData)
		{
			strText.AppendFormat(_T("%s"), _T(" ������../"));
		}
		if (m_bIsSingleFrame)
		{
			strText.AppendFormat(_T("%s"), _T(" ��֡�ط�../"));
		}
	}

	// ��ʾ�ٶ�.
	if (m_emPlaySpeed != PB_CONTROL_TYPE_NOREMAL)
	{
		// ����.
		if (m_emPlaySpeed > 0)
		{
			strText.AppendFormat(_T(" X%d����../"), (int)pow(2, (double)m_emPlaySpeed));
		}
		// ����.
		else if (m_emPlaySpeed < 0)
		{
			strText.AppendFormat(_T(" X1/%d����../"), (int)pow(2, (double)-m_emPlaySpeed));
		}
	}
	
	//ȥ�����һ���ַ�'/'
	int nIndex = strText.ReverseFind(_T('/'));
	if (nIndex >= 0)
	{
		strText = strText.Left(nIndex);
	}

	//��ʾ����
	m_pPlayTitleWnd->SetText(strText);
}

//��ȡ�ؼ���������
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

//��ʾȡ��������
void CPlaybackWnd::ShowTitle(BOOL bShowTitle)
{
	m_bShowTitle = bShowTitle;
	MoveCtrl();

	//��ʾ/����
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

//��ʾ�����İ���Ϣ
void CPlaybackWnd::ShowWndInfo(CString strMsg)
{
	SetStopTextShow(strMsg);
}

//���ô��ڱ߿���ɫ
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

//�߿����
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
// 	//������ڱ�ѡ�У���ͼ����������
// 	if (m_bSelect)
// 	{
// 		rcWin.DeflateRect(1, 1, 1, 1);
// 	}
	dc.Rectangle(rcWin);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	pen.DeleteObject();
}

//��ʱ������  ��ȡOSDʱ�䣬�طŽ��ȣ����ؽ���
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
					//�ط���ɻ��߻طŽ��Ȼ�ȡ����
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
				//�ص���OSDʱ��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::PostMessage(m_hRecvMsg, MSG_SELECT_VEDIO, 0, m_unWndID);

	CWnd::OnLButtonDown(nFlags, point);
}


void CPlaybackWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		//��ʾ/���ر���ѡ��
		if (m_bShowTitle)
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_SHOWTITLE, _T("���ر���"), 11);
		}
		else
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_SHOWTITLE, _T("��ʾ����"), 11);
		}
		//��Ƶ�ȵ���
		menuPopup.AppendMenu(MF_SEPARATOR);

		BCMenu menuSub;
		menuSub.CreatePopupMenu();
		if (menuSub.GetSafeHmenu() != NULL)
		{
			menuSub.AppendMenu(MF_STRING, MENU_ID_ORIGINAL, _T("�����طŴ���"), 11);
			menuSub.AppendMenu(MF_STRING, MENU_ID_4_3, _T("4:3"), 11);
			menuSub.AppendMenu(MF_STRING, MENU_ID_16_9, _T("16:9"), 11);
			menuPopup.AppendMenu(MF_POPUP, (UINT)menuSub.GetSafeHmenu(), _T("��ʾ����"), 11);
		}
		menuPopup.AppendMenu(MF_SEPARATOR);
		if (m_hPlayback != NULL)
		{
			// �طſ���.
			if (m_bPause  ||  m_bIsSingleFrame)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("����"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_STOP, _T("ֹͣ"), 11);
				menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_SLOW, _T("����"), 11);
				menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_FAST, _T("����"), 11);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("��ͣ"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_STOP, _T("ֹͣ"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_SLOW, _T("����"), 11);
				menuPopup.AppendMenu(MF_STRING, MENU_ID_FAST, _T("����"), 11);
			}

			if (m_bPause  ||  m_bReversePlay)
			{
				menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_SINGLEFRAME, _T("��֡����"), 11);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_SINGLEFRAME, _T("��֡����"), 11);
			}

			menuPopup.AppendMenu(MF_SEPARATOR);

			if (theApp.HasDownloadModule())
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_DOWNLOAD_BY_TIME, _T("¼������"), 11);
				if (m_bSaveData)
				{
					menuPopup.AppendMenu(MF_STRING, MENU_ID_SAVE, _T("ֹͣ����"), 11);
				}
				else
				{
					menuPopup.AppendMenu(MF_STRING, MENU_ID_SAVE, _T("����"), 11);
				}
			}
			menuPopup.AppendMenu(MF_STRING, MENU_ID_CAPTURE, _T("ץͼ"), 11);
			
			if (m_bSoundOpen)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("�ر�����"), 11);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("������"), 11);
			}
			menuPopup.AppendMenu(MF_SEPARATOR);
		}	// if.

		if (m_hPlayback || !m_bUsed)
		{
			menuPopup.AppendMenu(MF_STRING | MF_GRAYED, MENU_ID_PLAYAGAIN, _T("���²���"), 11);
		}
		else
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAYAGAIN, _T("���²���"), 11);
		}

		if (m_bSingleSound)
		{
			menuPopup.DeleteMenu(_T("����"));
			menuPopup.DeleteMenu(_T("����"));
			menuPopup.DeleteMenu(_T("��֡����"));
			menuPopup.DeleteMenu(_T("ץͼ"));
		}
		//��ʾ�����˵�
		POINT curPos;
		GetCursorPos(&curPos);
		LONG nCmd = menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD ,curPos.x,curPos.y,this);

		//��Ӧ�˵���Ϣ
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
				//������Ƶ����
				SetVedioOriginal();
			}
			break;
		case MENU_ID_4_3:
			{
				m_emShowPro = SHOW_PROPORTION_4_3;
				//������Ƶ����
				SetVedioOriginal();
			}
			break;
		case MENU_ID_16_9:
			{
				m_emShowPro = SHOW_PROPORTION_16_9;
				//������Ƶ����
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
 *    @brief   	   �ظ��ط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::PlayAgain()
{
	StopPlayback();
	StartPlay(TRUE);
}

//��Ƶ���ڷŴ󵽸����� / ��ԭ
void CPlaybackWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    //������Ϣ
    ::PostMessage(m_hRecvMsg, MSG_DB_VEDIO, 0, m_unWndID);

	CWnd::OnLButtonDblClk(nFlags, point);
}

//�ı���Ϣ���վ��
BOOL CPlaybackWnd::SetMsgRecvHwnd(HWND hWnd)
{
	if (hWnd == NULL)
	{
		return FALSE;
	}
	m_hRecvMsg = hWnd;

	return TRUE;
}


//�ı���Ƶ���ڱ���
void CPlaybackWnd::SetVedioOriginal()
{
	MoveCtrl();
}

/*******************�طŴ���**************************************/
/*************************************************
Function:    	PlaybackByTime
Description:	��ʱ��ط�
Input:			struPlaybackParamInfo�طŲ���, strChanName��ص����ƣ��Ǳ�Ҫ���������鴫�룩
Output:      	
Return:			TRUE�ɹ�, FALSEʧ��
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
Description:	���ļ��ط�
Input:			struPlaybackParamInfo�طŲ���(�ṹ���е�ʱ�������ǻطű�Ҫ�������������Ҫ�ı���ȣ���Ҫ����), strChanName��ص����ƣ��Ǳ�Ҫ���������鴫�룩
Output:      	
Return:			TRUE�ɹ�, FALSEʧ��
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

//�ڴ����ϻ�ͼ�ص�
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
Description:	��ʼ�ط�
Input:			bFindFile:�Ƿ���Ҫ�����ļ�.
Output:      	
Return:			TRUE�ɹ�, FALSEʧ��
Time:			2011-2-12 11:39
*************************************************/
void CPlaybackWnd::StartPlay(BOOL bFindFile)
{
	m_bPlayNextFile = FALSE;
	m_bUsed = TRUE;

	if (m_emPbType == PLAY_TYPE_TIME)
	{
		//���ò��Ŵ���
		m_stPlayTimeParam.hWnd = m_pPlayVedioWnd->GetSafeHwnd();
		m_stPlayTimeParam.pDrawFunc = &DrawOnWnd;
		m_stPlayTimeParam.nUser = (LONG)this;
		//���ûطź���
		PB_FILEINFO * lpPbFileInfo = NULL;
		if (bFindFile)
		{
			m_OrderPlay.ClearFile();
			lpPbFileInfo = new PB_FILEINFO[MAX_FILE_COUNT];
			// �Ƿ�����������͵�¼���ļ�,����ʱ��طţ��������ļ��ط�..
			BOOL bPlayTime = (RECORD_TYPE_ALL == m_stFindParam.emRecordType);
			// ���ļ��ط�ʱ��Ҫ�Զ��ط���һ���ļ�.
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
		//���ò��Ŵ���
		m_stPlayFileParam.hWnd = m_pPlayVedioWnd->GetSafeHwnd();
		m_stPlayFileParam.pDrawFunc = &DrawOnWnd;
		m_stPlayFileParam.nUser = (LONG)this;
		//���ûطź���
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
Description:	�ı�ط�λ��
Input:			dvrPlaybackTime�ط�λ�ã�  ���ļ��ط�ʱ�����ݴ˲�������λ��
Output:      	
Return:			TRUE�ɹ�, FALSEʧ��
Time:			2011-2-12 11:39
*************************************************/
BOOL CPlaybackWnd::PlaybackSetPos(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime)
{
	// ������Ԥ����.
	if (!m_hPlayback)
	{
		// ���δԤ����.
		if (-1 == m_nCameraID)
		{
			return FALSE;
		}
		// ֹͣԤ��.
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
		// ��־Ϊ��ʱ��ط�.
		m_emPbType = PLAY_TYPE_TIME;
		// �����µ�Ԥ��.
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
			// ���ý���.
			m_opePB.LocatePlayTime(stStartTime, stCurTime, m_nCameraID, m_bReversePlay);
		}
		else
		{
			// ���ý���.
			m_opePB.LocatePlayTime(stCurTime, stStopTime, m_nCameraID, m_bReversePlay);
		}
	}

	return TRUE;
}

//ֹͣ�ط�,bAutoStop�Ƿ����Զ�ֹͣ,���Ƿ��ǻط�ʱ���ط��ļ�������.
BOOL CPlaybackWnd::StopPlayback(BOOL bAutoStop, BWControlType emBwControl)
{
	//�������ͣ�У��Ȼָ�
	if (m_bPause)
	{
		RePlayback();
	}
	// ����ǵ�֡���ţ��Ȼָ�.
	if (m_bIsSingleFrame)
	{
		// ��֡�ָ�.
		PlaySingleFrame(FALSE);
	}
	// ������ڱ������ݣ�ֹͣ����.
	if (m_bSaveData)
	{
		StopSaveData();
	}

	//ֹͣ�ط�
	m_opePB.StopPlayback(m_hDlg, m_nCameraID, m_dwRev, bAutoStop, emBwControl);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), MSG_PLAYBACK_STATE, 0, 0);
	return TRUE;
}

// �����˳�ʱֹͣ�ط�.
BOOL CPlaybackWnd::DestroyStopPlayback()
{
	//�������ͣ�У��Ȼָ�
	if (m_bPause)
	{
		RePlayback();
	}
	// ������ڱ������ݣ�ֹͣ����.
	if (m_bSaveData)
	{
		StopSaveData();
	}

	//ֹͣ�ط�
	m_opePB.TaskStopPlayback(m_hDlg, m_nCameraID, m_dwRev, TRUE, BWCONTROL_NOCONTROL);

	return TRUE;
}
 
//��ͣ�ط�
BOOL CPlaybackWnd::PausePlayback()
{
	if (!m_hPlayback || m_bPause)
	{
		return FALSE;
	}

	//��ͣ
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_PAUSE, 0, lOutValue))
	{
		return FALSE;
	}

	m_bPause = TRUE;

	TRACE("@z PausePlayback m_bPause:%d", m_bPause);

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}
 
//�ָ��ط�
BOOL CPlaybackWnd::RePlayback()
{
	if (!m_hPlayback || !m_bPause)
	{
		return FALSE;
	}

	// �ָ��ط�.
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_RESTART, 0, lOutValue))
	{
		return FALSE;
	}

	m_bPause = FALSE;

	TRACE("@z RePlayback m_bPause:%d", m_bPause);

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}

//�ı�ط��ٶ�
BOOL CPlaybackWnd::SetPlaySpeed(ENUM_CONTROL_TYPE emPlaySpeed, BOOL bSynPlay)
{
	if (!m_hPlayback)
	{
		return FALSE;
	}

	// �Ƿ�����ͣ�У�ֻ��ͬ���طŻ��õ��˲�.
	if (m_bPause)
	{
		m_emSynPlaySpeed = emPlaySpeed;
		return FALSE;
	}

	//�ٶ�δ�ı�  
	if (m_emPlaySpeed == emPlaySpeed)
	{
		return TRUE;
	}

	// ������ڵ�֡�ط�.
	if (m_bIsSingleFrame)
	{
		PlaySingleFrame(FALSE);
	}

	//�ı�ط��ٶ�
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(emPlaySpeed, 0, lOutValue))
	{
		return FALSE;
	}

	m_emPlaySpeed = emPlaySpeed;
	m_emSynPlaySpeed = emPlaySpeed;

	//ˢ�¿ؼ�״̬
	PostMessage(WM_UPDATE_MSG, 0, 0);

	// �򸸴��ڷ�����Ϣ,�����ٶȸı�.
	if (!bSynPlay)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_SPEED_CHANGE, (WPARAM)m_unWndID, (LPARAM)m_emPlaySpeed);
	}

	return TRUE;
}

// ��ȡͬ���ط��ٶ�.
ENUM_CONTROL_TYPE CPlaybackWnd::GetSynPlaySpeed() const
{
	return m_emSynPlaySpeed;
}

//��ȡ�ط�OSDʱ��
LONG CPlaybackWnd::GetOsdTime()
{
	if (!m_hPlayback)
	{
		return 0;
	}

	//�ָ�
	LONG lOsdTime = 0;
	if (!m_opePB.GetOsdTime(lOsdTime))
	{
		return 0;
	}

	return lOsdTime;
}

//��ȡ�طŽ���
LONG CPlaybackWnd::GetPlayPos()
{
	if (!m_hPlayback)
	{
		return 0;
	}

	//�ָ�
	LONG lPos = 0;
	if (!m_opePB.GetPlayPos(lPos))
	{
		return 0;
	}

	return lPos;
}

//������
BOOL CPlaybackWnd::OpenPlaySound(DWORD dwSound)
{
	if (!m_hPlayback || m_bSoundOpen)
	{
		return FALSE;
	}

    //�ر��������ڵ�����
    if (m_pParent != NULL)
    {
        CPlayWndView* m_pDlg = (CPlayWndView*)m_pParent;
        m_pDlg->OnMsgMiniToolbarOpenSound(m_unWndID);
    }

	//������
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_OPEN_AUDIO, 0, lOutValue))
	{
		//return FALSE;
	}

	m_bSoundOpen = TRUE;

	//���ó�ʼ����Ϊ30
	SetPlayVolume(dwSound);

	// �򸸴��ڷ�����Ϣ,�Թر��������ڵ���Ƶ.
	//::PostMessage(GetParent()->GetSafeHwnd(), MSG_MINITOOLBAR_OPENSOUND, (WPARAM)m_unWndID, 0);

	// ���¿ؼ�.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}

//�ر�����
BOOL CPlaybackWnd::ClosePlaySound()
{
	if (!m_hPlayback || !m_bSoundOpen)
	{
		return FALSE;
	}

	//�ر�����
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_CLOSE_AUDIO, 0, lOutValue))
	{
		//return FALSE;
	}

	m_bSoundOpen = FALSE;

	//ˢ�¿ؼ�״̬
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;
}

//�ı�������С
BOOL CPlaybackWnd::SetPlayVolume(DWORD dwSound)
{
	if (!m_hPlayback || !m_bSoundOpen)
	{
		return FALSE;
	}

	//��������
	LONG lOutValue;
	if (!m_opePB.PlaybackControl(PB_CONTROL_TYPE_SET_VOLUME, (LONG)dwSound, lOutValue))
	{
		return FALSE;
	}

	m_nVolumn = dwSound;

	return TRUE;
}

//��ͼ
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
	strPicFile = strPicFile + _T("Զ�̻ط�\\") + strCurDate + _T("\\");

	//��ȡosdʱ����������
	LONG lOsdTime = GetOsdTime();
	if (lOsdTime <= 0)
	{
		strFileName.Format(_T("%s_%02u%02u%02u"), 
			(LPCTSTR)m_strChanName, t.wHour, t.wMinute, t.wSecond);
		strPicFile += strFileName;
	}
	else
	{
		//ʹ�ý�ͼʱ������
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

	// ץͼ.
	if (!m_opePB.Capture(m_hDlg, strFileA.GetBuffer(), emPicType, m_nCameraID, m_dwRev))
	{
		return FALSE;
	}
	strPic = strPicFile;

	return TRUE;
}

//��������
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

	//��ȡosdʱ����������
	LONG lOsdTime = GetOsdTime();
	if (lOsdTime <= 0)
	{
		strFileName.Format(_T("save_%s_%02d%02d%02d"),  (LPCTSTR)m_strChanName, tempTime.GetHour(), tempTime.GetMinute(), tempTime.GetSecond());
		strPicFile += strFileName;
	}
	else
	{
		//ʹ�ü���ʱ������
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

	// �����̿ռ��Ƿ��㹻.
	if (!CheckFilePathSpace(strPicFile))
	{
		return -1;
	}

	//��������  �ļ�·���ڵײ㴴��
	if (!m_opePB.SaveData(m_hDlg, strFileA.GetBuffer(), m_nCameraID, m_dwRev))
	{
		// �����IE����ģʽ.
		if (/*theApp.m_bIsIEProtected*/ 0)
		{
			CString strSelectPath = _T("");
			if (!theApp.GetIESafePath(strFileName, m_strSaveDataPath, strSelectPath, m_hSaveDataState, SAVE_FILETYPE_REC))
			{
				return 0;
			}

			// ץͼ��IE����.
			strFileA = CStringW2A(m_strSaveDataPath);
			if (!m_opePB.SaveData(m_hDlg, strFileA.GetBuffer(), m_nCameraID, m_dwRev))
			{
				theApp.CancelIEFile(m_hSaveDataState);
				return 0;
			}

			m_bSaveData = TRUE;
			// ���´��ڱ���.
			PostMessage(WM_UPDATE_MSG, 0, 0);
			strPath = strSelectPath;
			return 1;
		}	// if.

		return 0;
	}

	m_bSaveData = TRUE;

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	strPath = strPicFile;

	// ���������̿ռ��߳�.
	m_stCheckData.strFilePath = strPath;
	m_stCheckData.pParam = (LPVOID)this;

	ResetEvent(m_hStopCheck);
	m_hCheckSpaceThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFuncCheckSpace), &m_stCheckData, 0, NULL);

	return 1;
}

//ֹͣ��������
BOOL CPlaybackWnd::StopSaveData()
{
	if (!m_hPlayback || !m_bSaveData)
	{
		return FALSE;
	}

	// ֹͣ���
	if (m_hCheckSpaceThread != NULL)
	{
		SetEvent(m_hStopCheck);
		WaitForWindowThread(m_hCheckSpaceThread);
		CloseHandle(m_hCheckSpaceThread);
		m_hCheckSpaceThread = NULL;
	}

	//ֹͣ��������
	m_opePB.StopSave(m_hDlg, m_nCameraID, m_dwRev);

	// �����IE����ģʽ.
	if (theApp.m_bIsIEProtected  &&  !m_strSaveDataPath.IsEmpty()  &&  m_hSaveDataState != NULL)
	{
		// ���Ƶ�ָ��Ŀ¼.
		if (!theApp.SaveIEFile(m_strSaveDataPath, m_hSaveDataState))
		{
			return FALSE;
		}
	}

	m_bSaveData = FALSE;
	m_strSaveDataPath.Empty();
	m_hSaveDataState = NULL;

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return TRUE;

}

//���Ͱ�ť״̬ˢ����Ϣ
void CPlaybackWnd::PostUpdateMsg()
{
	//����
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYSTATE_UPDATE, (WPARAM)m_unWndID, 0);
}

//����osd�ص���Ϣ
void CPlaybackWnd::SetOsdCallback(pFunMsgOsdTime pSetOsdTime, LPVOID lpvoid)
{
	m_pSetOsdTime = pSetOsdTime;
	m_lpSetOsdTime = lpvoid;
}

//�����Ƿ�ͬ��
void CPlaybackWnd::SetSynPlay(BOOL bSynPlay)
{
	m_bSynPlay = bSynPlay;
	//��ͬ���н⿪
	if (!m_bSynPlay)
	{
		RePlayback();
	}
	else
	{
		// �ָ��ط��ٶ�.
		SetPlaySpeed(PB_CONTROL_TYPE_NOREMAL, TRUE);
		// ֹͣ��֡�ط�.
		if (IsSingleFrame())
		{
			PlaySingleFrame(FALSE, TRUE);
		}
	}
}

/*******************�طŴ���**************************************/
// ��Ӧ���������Ϣ.
LRESULT CPlaybackWnd::OnUpdateMsg(WPARAM wParam, LPARAM lParam)
{
	TRACE("@z OnUpdateMsg m_bPause:%d", m_bPause);
	//���ñ���
	SetTitleText();
	//ˢ�¿ؼ�״̬
	PostUpdateMsg();
	// ˢ�¹�����״̬.
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
 *    @brief   ����ֹͣ����.
 *    @param   
 *    @param   strVidel:��Ƶ��������.
 *    @return  
 */
void CPlaybackWnd::SetStopTextShow(CString strVideo, BOOL bErase)
{
 	// ��Ƶ����.
	if (m_pPlayVedioWnd != NULL)
	{
 		m_pPlayVedioWnd->SetTextShow(strVideo, bErase);
	}
}

void CPlaybackWnd::SetTextOnWnd( CString strVideo )
{
	// ��Ƶ����.
	if (m_pPlayVedioWnd != NULL)
	{
		m_pPlayVedioWnd->SetTextOnWnd(strVideo);
	}
}
/**   @fn      PlaySingleFrame
 *    @brief   ��֡����.
 *    @param   bPlay:TRUE-��֡����,FALSE-ֹͣ��֡����.
 *    @param   
 *    @return  
 */
void CPlaybackWnd::PlaySingleFrame(BOOL bPlay, BOOL bSynPlay)
{
	if (!m_hPlayback)
	{
		return;
	}

	// �������ͣ״̬.
	if (m_bPause)
	{
		m_bPause = FALSE;
	}

	LONG lOutValue;
	if (bPlay)
	{
		// ������ڶ౶�ٻط�.
		if (m_emPlaySpeed != PB_CONTROL_TYPE_NOREMAL)
		{
			if (!SetPlaySpeed(PB_CONTROL_TYPE_NOREMAL))
			{
				return;
			}
		}
		// ��֡�ط�.
		if (m_opePB.PlaybackControl(PB_CONTROL_TYPE_SINGLE_FRAME, 0, lOutValue))
		{
			m_bIsSingleFrame = TRUE;
		}
		else
		{
			//˵���£���һ�ε��õ�֡����ʱ�����ܻ᷵��ʧ��(�ر���CVR �ƴ洢��pcnvr�ط�ʱ)��
			//����ʵ���Ѿ������ɹ��ˣ������ſ��Ǳ߷�����ʱ�޷��޸Ĵ�������������²�����
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

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	// �򸸴��ڷ��͵�֡�ط���Ϣ.
	if (!bSynPlay)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_SINGLEFRAME, (WPARAM)m_unWndID, (LPARAM)bPlay);
	}
}

/**   @fn          IsSingleFrame
 *    @brief   	   �Ƿ����ڵ�֡�ط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:��,FALSE:��.
 */
BOOL CPlaybackWnd::IsSingleFrame() const
{
	return m_bIsSingleFrame;
}

/**   @fn          IsPlayback
 *    @brief   	   �Ƿ����ڽ���
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
 *    @brief   	   �Ƿ��Ѿ�ʹ�ù��ô���.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-��,FALSE-��.
 */
BOOL CPlaybackWnd::IsUsed()
{
	return m_bUsed;
}

/**   @fn          Invalidate
 *    @brief   	   ���ûط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::Invalidate()
{
	m_opePB.InvalidateWindow();
}

/**   @fn          GetCameraID
 *    @brief   	   ��ȡ��ص�ID.
 *    @param[in]   
 *    @param[in]   
 *    @return      ��ص�ID.
 */
int CPlaybackWnd::GetCameraID()
{
	return m_nCameraID;
}

/**   @fn          GetRevInfo
 *    @brief   	   ��ȡ�����ֶ���Ϣ.
 *    @param[in]   
 *    @param[in]   
 *    @return      �����ֶ�.
 */
DWORD CPlaybackWnd::GetRevInfo()
{
	return m_dwRev;
}

/**   @fn          GetRight
 *    @brief   	   ��ȡȨ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      Ȩ��ֵ.
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, UI_PLAYWND_SPACE);

	DrawBorder(m_clrBorder);
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

// �����ļ�ʧ��.
LRESULT CPlaybackWnd::OnMsgFindFail(WPARAM wParam, LPARAM lParam)
{
	// �ļ��б�ָ��.
	LPB_FILEINFO pFileList = (LPB_FILEINFO)wParam;

	if (pFileList != NULL)
	{
		delete[] pFileList;
		pFileList = NULL;
	}

	SetTitleText(_T(""));

	ShowErrorMsg(_T("�����ļ�ʧ��"));

	// �򸸴��ڷ�����Ϣ.
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_FAIL, (WPARAM)m_unWndID, (LPARAM)m_nCameraID);
	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);
	return 0;
}

// �����ļ��ɹ�.
LRESULT CPlaybackWnd::OnMsgFindSuccess(WPARAM wParam, LPARAM lParam)
{
	// �ļ�����.
	int nFileCount = (int)lParam;
	// �ļ��б�ָ��.
	LPB_FILEINFO pFileList = (LPB_FILEINFO)wParam;

	// �����ļ��б�.
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
	// ���õ�ǰ�������.
	int nPlayIndex = m_bReversePlay ? (nFileCount > 0 ? nFileCount - 1 : 0) : 0;
	m_OrderPlay.SetCurPlayIndex(nPlayIndex);
	if (pFileList != NULL)
	{   //��ȡURL��ָ��ɾ����������Ҫע�����ָ������á���Machieal 2016-11-1��
        strcpy_s(m_stPlayTimeParam.szPlayUrl, sizeof(m_stPlayTimeParam.szPlayUrl), pFileList[nPlayIndex].szPlayUrl);
		delete[] pFileList;
		pFileList = NULL;

		// �򸸴��ڷ�����Ϣ.
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_SUCCESS, (WPARAM)m_unWndID, (LPARAM)m_nCameraID);
	}
	return 0;
}

// �طųɹ�.
LRESULT CPlaybackWnd::OnMsgPlaySuccess(WPARAM wParam, LPARAM lParam)
{
	// ���ž��.
	m_hPlayback = (HANDLE)wParam;

	//����طŲ���
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

    //��ȡ�豸¼���������
    if (m_stLoginInfo.emDevType == PB_DEVICE_HIKVISION && m_stLoginInfo.emPlayType == PB_TYPE_DVR)
    {
        GetDeviceRecordAbility();
    }

    // ���´��ڱ���.
    PostMessage(WM_UPDATE_MSG, 0, 0);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), MSG_PLAYBACK_STATE, 1, 0);
	OpenPlaySound(50);
	return 0;
}

// �ط�ʧ��.
LRESULT CPlaybackWnd::OnMsgPlayFail(WPARAM wParam, LPARAM lParam)
{
	// ���ž��.
	m_hPlayback = NULL;

	ShowErrorMsg(_T("�ط�ʧ��"));

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), MSG_PLAYBACK_STATE, 0, 0);
	OutputDebugString(_T("@z Playback fail"));
	return 0;
}

/**   @fn          ShowErrorMsg
 *    @brief   	   ��ʾ������Ϣ.
 *    @param[in]   strDefMsg:Ĭ����Ϣ.
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::ShowErrorMsg(const CString& strDefMsg)
{
	DWORD dwError = m_opePB.GetSDKLastError();

	switch (dwError)
	{
		// ��֧�ֵĲ���.
	case PB_UNSUPPORT_OPERATION:
		{
			SetStopTextShow(_T("�ò�����֧��"));
		}
		break;

		// ��֧�ֵĵ�������.
	case PB_UNSUPPORT_REVPLAY_TYPE:
		{
			SetStopTextShow(_T("���豸���ͻ�洢���Ͳ�֧�ֵ���"));
		}
		break;

		// ��֧�ְ����Ų�ѯ.
	case PB_UNSUPPORT_CARDNUM:
		{
			SetStopTextShow(_T("��֧�ְ����Ų�ѯ"));
		}
		break;

	default:
		{
			SetStopTextShow(strDefMsg);
		}
		break;
	}
}

// �������ӿ���.
LRESULT CPlaybackWnd::OnMsgBWLimit(WPARAM wParam, LPARAM lParam)
{
	SetStopTextShow(_T("�����������ﵽ����,��Ȩ�޻ط�"));
	return 0;
}

// ֹͣ�ط�.
LRESULT CPlaybackWnd::OnMsgStopPlay(WPARAM wParam, LPARAM lParam)
{
	TRACE("@z OnMsgStopPlay");
	// �����������.
	BWControlType emBwControl = (BWControlType)wParam;
	// �Ƿ����Զ�ֹͣ.
	//BOOL bAutoStop = (BOOL)lParam;

	//�ָ�����
	m_bPause = FALSE;
	m_bIsSingleFrame = FALSE;
	m_bSoundOpen = FALSE;
	m_bSaveData = FALSE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_emSynPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_hPlayback = NULL;
	m_lOsdTime = 0;

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	// �������ֹͣ.
	if (BWCONTROL_CROWDING == emBwControl)
	{
		SetStopTextShow(_T("�����������ﵽ����,���߼����û���ռ"));
	}
	else if (BWCONTROL_RECONNECT == emBwControl)
	{
		SetStopTextShow(_T("�����������ﵽ����"));
	}
	else if (SHOW_ERROR_MSG == emBwControl)
	{
		ShowErrorMsg(_T("�ط�ʧ��"));
	}
	else
	{
		//ˢ����Ƶ����
		if (m_pPlayVedioWnd != NULL)
		{
			m_pPlayVedioWnd->SetStatePicHide();
		}
	}

	// ������ļ�����ģʽ,���Զ�������һ���ļ�.
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

	// ���ûط�ֹͣ.
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

// ���������豸.
LRESULT CPlaybackWnd::OnMsgPlaying(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;
	if (PB_TYPE_DVR == m_stLoginInfo.emPlayType)
	{
		strMsg = _T("���������豸...");
	}
	else
	{
		strMsg = _T("�������ӷ�����...");
	}
	SetStopTextShow(strMsg);
	return 0;
}

/**   @fn          GetFileInfo
 *    @brief   	   ��ȡ�ļ��б�����.
 *    @param[in]   
 *    @param[in]   
 *    @return      �ļ��б������.
 */
std::vector<PB_FILEINFO>& CPlaybackWnd::GetFileInfo()
{
	return m_OrderPlay.GetFileInfo();
}

/**   @fn          SetFileInfo
 *    @brief   	   �����ļ��б�.
 *    @param[in]  
 *    @param[in]   vFileInfo:�ļ��б�����.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackWnd::SetFileInfo(const std::vector<PB_FILEINFO>& vFileInfo)
{
	return m_OrderPlay.SetFileInfo(vFileInfo);
}

/**   @fn          GetFileInfo
 *    @brief   	   ��ȡ�����ļ���Ϣ.
 *    @param[in]   nIndex:�ļ�����.
 *    @param[out]  stFileInfo:�ļ���Ϣ. 
 *    @return      �ļ���Ϣ����.
 */
BOOL CPlaybackWnd::GetFileInfo(int nIndex, PB_FILEINFO& stFileInfo)
{
	return m_OrderPlay.GetFileInfo(nIndex, stFileInfo);
}

// ���ûطŽ��ȳɹ�.
LRESULT CPlaybackWnd::OnMsgLocatePlaySuccess(WPARAM wParam, LPARAM lParam)
{
	// �ָ�����ͣ״̬.
	m_bPause = FALSE;
	m_bIsSingleFrame = FALSE;

	m_tStartTime = m_emPbType==PLAY_TYPE_TIME?m_stPlayTimeParam.stStartTime:m_stPlayFileParam.stFileInfo.stStartTime;
	m_tEndTime = m_emPbType==PLAY_TYPE_TIME?m_stPlayTimeParam.stStopTime:m_stPlayFileParam.stFileInfo.stEndTime;
	CTime tBeginTime(m_tStartTime.dwYear, m_tStartTime.dwMonth, m_tStartTime.dwDay, m_tStartTime.dwHour, m_tStartTime.dwMinute, m_tStartTime.dwSecond);
	CTime tEndTime(m_tEndTime.dwYear, m_tEndTime.dwMonth, m_tEndTime.dwDay, m_tEndTime.dwHour, m_tEndTime.dwMinute, m_tEndTime.dwSecond);
	m_lEndTime = (LONG)tEndTime.GetTime();
	m_lBeginTime = (LONG)tBeginTime.GetTime();

	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return 0;
}

// ���ûطŽ���ʧ��.
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

	ShowErrorMsg(_T("�ط�ʧ��"));
	
	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);

	return 0;
}

// ������Ʒ���������ֹͣ�ط�.
LRESULT CPlaybackWnd::OnMsgBwStopPlay(WPARAM wParam, LPARAM lParam)
{
	PLAYBACKDLL_INFO(_T("StopPlayCam"));
	StopPlayback(FALSE, BWCONTROL_RECONNECT);

	return 0;
}

/**   @fn           GetPlayTimeInfo
 *    @brief   	   	��ȡʱ����Ϣ
 *    @param[in]   
 *    @param[in]   
 *    @return       ʱ����Ϣ����.
 */
PLAYBACK_TIME_INFO& CPlaybackWnd::GetPlayTimeInfo()
{
	return m_stPlayTimeParam;
}

/**   @fn           GetPlayFileInfo
 *    @brief   	   	��ȡ�ļ���Ϣ
 *    @param[in]   
 *    @param[in]   
 *    @return       �ļ���Ϣ����.
 */
PLAYBACK_FILE_INFO& CPlaybackWnd::GetPlayFileInfo()
{
	return m_stPlayFileParam;
}

/**   @fn           GetFindInf
 *    @brief   	   	��ȡ�ļ�������Ϣ
 *    @param[in]   
 *    @param[in]   
 *    @return       �ļ�������Ϣ����.
 */
FINDFILE_PARAM& CPlaybackWnd::GetFindInf()
{
	return m_stFindParam;
}

/**   @fn          GetCamName
 *    @brief   	   ��ȡ��ص�����.
 *    @param[in]   
 *    @param[in]   
 *    @return      ��ص�����.
 */
CString& CPlaybackWnd::GetCamName()
{
	return m_strChanName;
}

/**   @fn          GetChannel
 *    @brief   	   ��ȡͨ����.
 *    @param[in]   
 *    @param[in]   
 *    @return      ͨ����.
 */
LONG CPlaybackWnd::GetChannel()
{
	return m_lChannel;
}

/**   @fn          GetPlayType
 *    @brief   	   ��ȡ�ط�����.
 *    @param[in]   
 *    @param[in]   
 *    @return      �ط�����.
 */
ENUM_PLAY_TYPE CPlaybackWnd::GetPlayType()
{
	return m_emPlayType;
}

/**   @fn          GetLoginInfo
 *    @brief   	   ��ȡ��¼��Ϣ.
 *    @param[in]   
 *    @param[in]   
 *    @return      ��¼��Ϣ����.
 */
 PB_LOGININFO& CPlaybackWnd::GetLoginInfo()
{
	return m_stLoginInfo;
}

/**   @fn          HideToolBar
 *    @brief   	   ���ع�����.
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
 *    @brief   	   ����뿪��������Ϣ.
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
 *    @brief   	   �����빤������Ϣ.
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
 *    @brief   	   ��������ť�����Ϣ.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgToolButtonClick(WPARAM wParam, LPARAM lParam)
{
	switch ((TOOLBAR_CTRL_ID)wParam)
	{
		// ��չ��ť.
	case TOOL_EXPAND_ID:
		{
			OnMiniToolbarControlExpand();
		}
		break;
		// ץͼ.
	case TOOL_CAPTURE_ID:
		{
			OnMiniToolbarControlCapture();
		}
		break;
		// ����.
	case TOOL_SAVEDATA_ID:
		{
			OnMiniToolbarControlSave();
		}
		break;  
		// ֹͣ.
	case TOOL_STOP_ID:
		{
			StopPlayback();
		}
		break;  
		// ����.
	case TOOL_MINI_SLOW_ID:
		{
			PlaySlow();
		}
		break;
		// ���.
	case TOOL_MINI_FAST_ID:
		{
			PlayFast();
		}
		break;
		// ��֡����.
	case TOOL_FRAME_ID:
		{
			PlaySingleFrame(TRUE);
		}
		break; 
		// ����.
	case TOOL_PLAY_ID:
		{
			OnMiniToolbarControlPlay();
		}
		break;
		// ��ʱ������.
	case TOOL_DOWNLOAD_ID:
		{
			OnMiniToolbarDownload();
		}
		break;
		// ��Ƶ.
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
 *    @brief   	   ���ٲ���.
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
 *    @brief   	   ���ٲ���.
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
 *    @brief   	   ��Ӧ��չ�������ر���Ϣ.
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
 *    @brief   	   ����������-�ط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::OnMiniToolbarControlPlay()
{
	// ������ڽ��е�֡�ط�.
	if (m_bIsSingleFrame)
	{
		// ��֡�ָ�.
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
 *    @brief   	   ����������-��ʱ������.
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
 *    @brief   	   ������������������.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::OnMiniToolbarControlSave()
{
	if (!theApp.IsCamSaveRight(m_dwRight))
	{
		MsgBoxApi_Show(_T("��ǰ�û������м���Ȩ��!"), MSGBOX_TYPE_WARNING);
		return;
	}

	if (!m_bSaveData)
	{
		m_strFilePath.Empty();
		int nRet = SaveData(m_strFilePath);
		if (-1 == nRet)
		{
			MsgBoxApi_Show(_T("���̿ռ䲻��,����ʧ��!"), MSGBOX_TYPE_WARNING);
		}
		else if (0 == nRet)
		{
			if (theApp.m_bIsIEProtected)
			{
				MsgBoxApi_Show(_T("����ʧ��,��ʹ�ù���Ա�������IE,�򽵵�Ȩ�޵ȼ�"), MSGBOX_TYPE_ERROR);
			}
			else
			{
				MsgBoxApi_Show(_T("����ʧ��!"), MSGBOX_TYPE_ERROR);
			}
		}
	}
	else
	{
		if (!StopSaveData())
		{
			MsgBoxApi_Show(_T("ֹͣ����ʧ��!"), MSGBOX_TYPE_ERROR);
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
 *    @brief   	   ����������-��ͼ.
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
		MsgBoxApi_Show(_T("��ǰ�û�������ץͼȨ��!"), MSGBOX_TYPE_WARNING);
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

		//����ͼƬչʾ����
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
		strMsg.Format(_T("����%02d:ץͼʧ��"), m_unWndID + 1);
		MsgBoxApi_Show(strMsg.GetBuffer(), MSGBOX_TYPE_ERROR);
	}
}

/**   @fn          OnMiniToolbarControlExpand
 *    @brief   	   ����������-��չ.
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
 *    @brief   	   ���°�ťtips.
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
 *    @brief   	   ��ȡ�ط�״̬��Ϣ.
 *    @param[in]   stData:״̬��Ϣ.
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
 *    @brief   	   ���ûطž��.
 *    @param[in]   hPlayHandle:�طž��.
 *    @param[in]   
 *    @return      
 */
void CPlaybackWnd::SetPlayHandle(HANDLE hPlayHandle)
{
	m_opePB.SetPlayHandle(hPlayHandle);
}

/**   @fn          ThreadFuncCheckSpace
 *    @brief   	   �����̿ռ��С.
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
			// ���̿ռ䲻��.
			pWnd->PostMessage(MSG_CHECK_SPACE_NOTENOUGH, 0, 0);
			PLAYBACKDLL_ERROR(_T("@z ���̿ռ䲻��!"));
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
 *    @brief   	   ���̿ռ䲻����Ӧ��Ϣ.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgCheckSpaceNotEnough(WPARAM wParam, LPARAM lParam)
{
	CloseHandle(m_hCheckSpaceThread);
	m_hCheckSpaceThread = NULL;

	StopSaveData();

	MsgBoxApi_Show(_T("���̿ռ䲻��,����ʧ��!"), MSGBOX_TYPE_WARNING);

	return 0;
}

/**   @fn          OnMsgFindNoFile
 *    @brief   	   ��Ӧû���ҵ������������ļ�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackWnd::OnMsgFindNoFile(WPARAM wParam, LPARAM lParam)
{
	// �ļ��б�ָ��.
	LPB_FILEINFO pFileList = (LPB_FILEINFO)wParam;

	if (pFileList != NULL)
	{
		delete[] pFileList;
		pFileList = NULL;
	}

	SetTitleText(_T(""));
	SetStopTextShow(_T("û���ҵ������������ļ�"));
	// ���´��ڱ���.
	PostMessage(WM_UPDATE_MSG, 0, 0);
	
		// �򸸴��ڷ�����Ϣ.
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_FAIL, (WPARAM)m_unWndID, (LPARAM)m_nCameraID);

	return 0;
}

/**   @fn          OnMsgInvalidateWindow
 *    @brief   	   ˢ�´�����Ӧ��Ϣ.
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
 *    @brief   	   �Ƿ��ǵ���״̬.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-��,FALSE-��.
 */
BOOL CPlaybackWnd::IsReversePlay()
{
	return m_bReversePlay;
}

/**   @fn          GetCurPlayFileIndex
 *    @brief   	   nPlayIndex:�������.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
BOOL CPlaybackWnd::GetCurPlayFileIndex(int& nPlayIndex)
{
	return m_OrderPlay.GetCurPlayIndex(nPlayIndex);
}

/**   @fn          SetCurPlayFileIndex
 *    @brief   	   ���õ�ǰ�ط����.
 *    @param[in]   nPlayIndex:�ط����.
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
        PLAYBACKDLL_ERROR(_T("��ȡ�豸¼������ʧ��!"));
        return;
    }

    TCHAR *chTmp = new TCHAR[XML_ABILITY_OUT_LEN];
    MByteToWChar(pOutBuf,chTmp,XML_ABILITY_OUT_LEN);
    m_strAbilityXml.Format(_T("%s"),chTmp);
    delete []pOutBuf;
    delete []chTmp;

    ParseDeviceRecordAbility();
}
