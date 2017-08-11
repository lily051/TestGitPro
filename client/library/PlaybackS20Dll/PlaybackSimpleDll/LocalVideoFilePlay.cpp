// LocalVideoFilePlay.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalVideoFilePlay.h"

void CALLBACK FileEndCB(long nPort, void *pUser)
{
	CLocalVideoFilePlay *pLocalVideo = (CLocalVideoFilePlay*)pUser;
	if (NULL == pLocalVideo)
	{
		return;
	}
	if (nPort == pLocalVideo->m_lPlayPort)
	{
		pLocalVideo->m_bStopPlay = TRUE; //����Ҫֹͣ�ı�־������ΪTRUE����ʾ�ļ��Ѳ��꣬��Ҫֹͣ
		//pLocalVideo->StopPlay();   //�ڽ����ص������ֹͣ�ӿڣ����У�
	}
}

// CLocalVideoFilePlay �Ի���

IMPLEMENT_DYNAMIC(CLocalVideoFilePlay, CDialog)

CLocalVideoFilePlay::CLocalVideoFilePlay(CWnd* pParent /*=NULL*/)
	: CDialog(CLocalVideoFilePlay::IDD, pParent)
{
	m_pLocalWndTitle = NULL;
	m_pLocalWndView = NULL;
	m_pLocalWndToolBar = NULL;
	m_pFullScreenWnd = NULL;
	m_pFullScreenParentWnd = NULL;
	m_pCaptureTip = NULL;

	m_lPlayPort = -1;
	m_strTotalTime = _T("");
	m_dwTatalSeconds = 0;

	m_bIsPlaying = FALSE;
	m_bIsPausing = FALSE;
	m_bIsSounding = FALSE;
	m_bIsFullScreening = FALSE;
	m_nSpeed = PS_NORMAL;
	m_bStopPlay = FALSE;
}

CLocalVideoFilePlay::~CLocalVideoFilePlay()
{
	m_pLocalWndTitle = NULL;
	m_pLocalWndView = NULL;
	m_pLocalWndToolBar = NULL;
	m_pFullScreenWnd = NULL;
	m_pFullScreenParentWnd = NULL;
	m_pCaptureTip = NULL;
}

void CLocalVideoFilePlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLocalVideoFilePlay, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(MSG_TOOLBAR_OPERATOR, &CLocalVideoFilePlay::OnMsgToolBarOpera)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLocalVideoFilePlay ��Ϣ�������

BOOL CLocalVideoFilePlay::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreateWnd();
	m_pFullScreenParentWnd = GetParent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLocalVideoFilePlay::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pLocalWndTitle != NULL)
	{
		m_pLocalWndTitle->DestroyWindow();
		delete m_pLocalWndTitle;
		m_pLocalWndTitle = NULL;
	}
	if (m_pLocalWndView != NULL)
	{
		m_pLocalWndView->DestroyWindow();
		delete m_pLocalWndView;
		m_pLocalWndView = NULL;
	}
	if (m_pLocalWndToolBar != NULL)
	{
		m_pLocalWndToolBar->DestroyWindow();
		delete m_pLocalWndToolBar;
		m_pLocalWndToolBar = NULL;
	}
	if (m_pFullScreenWnd != NULL)
	{
		m_pFullScreenWnd->DestroyWindow();
		delete m_pFullScreenWnd;
		m_pFullScreenWnd = NULL;
	}
	if (m_pFullScreenParentWnd != NULL)
	{
		SetParent(m_pFullScreenParentWnd);
	}
}

void CLocalVideoFilePlay::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	MoveWnd();
}

// �����Ӵ���
void CLocalVideoFilePlay::CreateWnd(void)
{
	if (m_pLocalWndTitle == NULL)
	{
		m_pLocalWndTitle = new CLocalWndTitle;
		if (NULL == m_pLocalWndTitle)
		{
			PLAYBACKDLL_ERROR(_T("�����Ӵ��ڱ���ʧ�ܣ�"));
			return;
		}
		m_pLocalWndTitle->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0,0,0,0), this, PLAYBACK_TITLE_ID);
		m_pLocalWndTitle->ShowWindow(SW_SHOW);
		m_pLocalWndTitle->SetText(_T(""));
	}
	if (m_pLocalWndView == NULL)
	{
		m_pLocalWndView = new CLocalWndView;
		if (NULL == m_pLocalWndView)
		{
			PLAYBACKDLL_ERROR(_T("�����Ӵ�����ͼʧ�ܣ�"));
			return;
		}
		m_pLocalWndView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0,0,0,0), this, PLAYBACK_VEDIO_ID);
		m_pLocalWndView->ShowWindow(SW_SHOW);
	}
	if (m_pLocalWndToolBar == NULL)
	{
		m_pLocalWndToolBar = new CLocalWndToolBar;
		if (NULL == m_pLocalWndView)
		{
			PLAYBACKDLL_ERROR(_T("�����Ӵ��ڹ�����ʧ�ܣ�"));
			return;
		}
		m_pLocalWndToolBar->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0,0,0,0), this, PLAYBACK_TITLE_ID);
		m_pLocalWndToolBar->ShowWindow(SW_SHOW);
	}
	if (NULL == m_pFullScreenWnd)
	{
		m_pFullScreenWnd = new CWnd;
		if (NULL == m_pLocalWndView)
		{
			PLAYBACKDLL_ERROR(_T("�����Ӵ���ȫ��ʧ�ܣ�"));
			return;
		}
		m_pFullScreenWnd->CreateEx(WS_EX_TOPMOST, AfxRegisterWndClass(NULL), NULL,WS_POPUP,CRect(0,0,0,0), NULL, 0);
		CWnd* pScreenWnd = GetDesktopWindow();
		CRect rc;
		pScreenWnd->GetClientRect(&rc);
		m_pFullScreenWnd->MoveWindow(&rc);
	}
}

// ��ʼ���ؼ�λ��
void CLocalVideoFilePlay::MoveWnd(void)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nTitleHeight = 18;
	int nToorBar = 45;
	CRect rcLocalVideoFileView(rcClient);
	rcLocalVideoFileView.top = rcClient.top + nTitleHeight;
	rcLocalVideoFileView.bottom = rcClient.bottom - nToorBar;
	if (NULL != m_pLocalWndView)
	{
		m_pLocalWndView->MoveWindow(rcLocalVideoFileView);
	}

	CRect rcLocalVideoFileTitle(rcClient);
	rcLocalVideoFileTitle.bottom = nTitleHeight;
	if (NULL != m_pLocalWndTitle)
	{
		m_pLocalWndTitle->MoveWindow(rcLocalVideoFileTitle);
	}

	CRect rcLocalVideoFileToolBar(rcClient);
	rcLocalVideoFileToolBar.top = rcLocalVideoFileToolBar.bottom - nToorBar;
	if (NULL != m_pLocalWndToolBar)
	{
		m_pLocalWndToolBar->MoveWindow(rcLocalVideoFileToolBar);
	}
}

void CLocalVideoFilePlay::StartPlay(const int nVideoType, const CString csFilePath)
{
	if (NULL == m_pLocalWndView)
	{
		return ;
	}
	StopPlay();
	if (!GPAPI_Startup(PT_HIK, NULL, 0, 0))
	{
		PLAYBACKDLL_ERROR(_T("local GPAPI_Startup falid"));
		return;
	}
	if (!GPAPI_GetPort(PT_HIK, m_lPlayPort, m_pLocalWndView->GetSafeHwnd()))
	{
		PLAYBACKDLL_ERROR(_T("local GPAPI_GetPort falid"));
		return;
	}
	//�����ļ����Ž����ص�����
	GPAPI_SetFileEndCallback(PT_HIK, m_lPlayPort, FileEndCB, this);

	if (!GPAPI_FileStart(PT_HIK, m_lPlayPort, (CStringW2A(csFilePath)).GetBuffer(), m_pLocalWndView->GetSafeHwnd()))
	{
		PLAYBACKDLL_ERROR(_T("local GPAPI_FileStart falid"));
		return;
	}
	m_csFilePath = csFilePath;

	SetTimer(0, 1000, NULL);
	SetTimer(1, 500, NULL);

	m_dwTatalSeconds = GPAPI_GetFileTime(PT_HIK, m_lPlayPort);
	m_strTotalTime = SecondToStrTime(m_dwTatalSeconds);
	m_bIsPlaying = TRUE;

	m_csFileName = csFilePath.Right(csFilePath.GetLength() - 1 - csFilePath.ReverseFind(_T('\\')));
	if (m_pLocalWndTitle != NULL)
	{
		m_pLocalWndTitle->SetText(m_csFileName);
	}
	if (m_pLocalWndView != NULL)
	{
		m_pLocalWndView->UpdatePlayStatus(TRUE);
	}
	if (m_pLocalWndToolBar != NULL)
	{
		m_pLocalWndToolBar->UpdateBtnStatus(TRUE, OPERA_ID_NOTICE);
	}
	PostMessage(MSG_TOOLBAR_OPERATOR, OPERA_ID_SOUND, 0);
}

void CLocalVideoFilePlay::StopPlay()
{
	if (m_pLocalWndTitle != NULL)
	{
		m_pLocalWndTitle->SetText(_T(""));
	}
	if (!GPAPI_StopSound(PT_HIK, m_lPlayPort))
	{
		PLAYBACKDLL_ERROR(_T("local GPAPI_StopSound falid, port = %d"), m_lPlayPort);
	}
	if (!GPAPI_FileStop(PT_HIK, m_lPlayPort))
	{
		PLAYBACKDLL_ERROR(_T("local GPAPI_FileStop falid, port = %d"), m_lPlayPort);
	}
	
	if (!GPAPI_FreePort(PT_HIK, m_lPlayPort))
	{
		PLAYBACKDLL_ERROR(_T("local GPAPI_FreePort falid, port = %d"), m_lPlayPort);
	}
	KillTimer(0);
	KillTimer(1);
	if (m_pLocalWndView != NULL)
	{
		m_pLocalWndView->Invalidate(TRUE);
	}
	m_lPlayPort = -1;
	m_bIsPlaying = FALSE;
	if (m_pLocalWndToolBar != NULL)
	{
		m_pLocalWndToolBar->UpdateBtnStatus(FALSE, OPERA_ID_STOP);
	}
	if (m_pLocalWndView != NULL)
	{
		m_pLocalWndView->UpdatePlayStatus(FALSE);
	}
	m_bIsPausing = FALSE;
	m_bIsSounding = FALSE;
	m_nSpeed = PS_NORMAL;
	m_bStopPlay = FALSE;
}

LRESULT CLocalVideoFilePlay::OnMsgToolBarOpera(WPARAM wParam, LPARAM lParam)
{
	int nOperType = (int)wParam;
	switch(nOperType)
	{
	case OPERA_ID_CAPTURE:
		OnCapture();
		break;
	case OPERA_ID_SLOW:
		{
			PLAYSPEED emSpeed = ((m_nSpeed - 1) < PS_SLOW16) ? PS_SLOW16 : (PLAYSPEED)((int)m_nSpeed - 1);
			if (m_pLocalWndView != NULL)
			{
				if (!GPAPI_SetPlaySpeed(PT_HIK, m_lPlayPort, emSpeed, m_pLocalWndView->GetSafeHwnd()))
				{
					PLAYBACKDLL_ERROR(_T("local GPAPI_SetPlaySpeed slow falid, port = %d"), m_lPlayPort);
				}
				m_nSpeed = emSpeed;
				OnUpdateWndTitle();
			}
		}
		break;
	case OPERA_ID_PLAY:
		if (!m_bIsPausing)
		{
			if (!GPAPI_Pause(PT_HIK, m_lPlayPort, TRUE))
			{
				PLAYBACKDLL_ERROR(_T("local GPAPI_Pause pause falid, port = %d"), m_lPlayPort);
			}
			m_bIsPausing = TRUE;
			OnUpdateWndTitle();
			if (m_pLocalWndToolBar != NULL)
			{
				m_pLocalWndToolBar->UpdateBtnStatus(TRUE, OPERA_ID_PLAY);
			}
			if (m_pLocalWndView != NULL)
			{
				m_pLocalWndView->UpdateMenuStatus(OPERA_ID_PLAY, TRUE);
			}
		}
		else
		{
			if (!GPAPI_Pause(PT_HIK, m_lPlayPort, FALSE))
			{
				PLAYBACKDLL_ERROR(_T("local GPAPI_Pause play falid, port = %d"), m_lPlayPort);
			}
			m_bIsPausing = FALSE;
			OnUpdateWndTitle();
			if (m_pLocalWndToolBar != NULL)
			{
				m_pLocalWndToolBar->UpdateBtnStatus(TRUE, OPERA_ID_PLAY);
			}
			if (m_pLocalWndView != NULL)
			{
				m_pLocalWndView->UpdateMenuStatus(OPERA_ID_PLAY, FALSE);
			}
		}
		break;
	case OPERA_ID_FAST:
		{
			PLAYSPEED emSpeed = ((m_nSpeed + 1) > PS_Fast16) ? PS_Fast16 : (PLAYSPEED)((int)m_nSpeed + 1);
			if (m_pLocalWndView != NULL)
			{
				if (!GPAPI_SetPlaySpeed(PT_HIK, m_lPlayPort, emSpeed, m_pLocalWndView->GetSafeHwnd()))
				{
					PLAYBACKDLL_ERROR(_T("local GPAPI_SetPlaySpeed fast falid, port = %d"), m_lPlayPort);
				}
				m_nSpeed = emSpeed;
				OnUpdateWndTitle();
			}
		}
		break;
	case OPERA_ID_SOUND:
		if (!m_bIsSounding)
		{
			if (!m_bIsPlaying)
			{
				return 0;
			}
			if (!GPAPI_PlaySound(PT_HIK, m_lPlayPort))
			{
				PLAYBACKDLL_ERROR(_T("local GPAPI_PlaySound open falid, port = %d"), m_lPlayPort);
			}
			if (!GPAPI_SetVolume(PT_HIK, m_lPlayPort, 50))
			{
				PLAYBACKDLL_ERROR(_T("local GPAPI_SetVolume falid, port = %d"), m_lPlayPort);
			}
			m_bIsSounding = TRUE;
			if (m_pLocalWndToolBar != NULL)
			{
				m_pLocalWndToolBar->UpdateBtnStatus(TRUE, OPERA_ID_SOUND);
			}
			if (m_pLocalWndView != NULL)
			{
				m_pLocalWndView->UpdateMenuStatus(OPERA_ID_SOUND, TRUE);
			}
		}
		else
		{
			if (!GPAPI_StopSound(PT_HIK, m_lPlayPort))
			{
				PLAYBACKDLL_ERROR(_T("local GPAPI_StopSound falid, port = %d"), m_lPlayPort);
			}
			m_bIsSounding = FALSE;
			if (m_pLocalWndToolBar != NULL)
			{
				m_pLocalWndToolBar->UpdateBtnStatus(TRUE, OPERA_ID_SOUND);
			}
			if (m_pLocalWndView != NULL)
			{
				m_pLocalWndView->UpdateMenuStatus(OPERA_ID_SOUND, FALSE);
			}
		}
		break;
	case OPERA_ID_SOUNDVALUE:
		if (m_bIsSounding)
		{
			int nPos = (int)lParam;
			if (!GPAPI_SetVolume(PT_HIK, m_lPlayPort, nPos))
			{
				PLAYBACKDLL_ERROR(_T("local GPAPI_SetVolume falid, port = %d"), m_lPlayPort);
			}
		}
		break;
	case OPERA_ID_FULLSCREEN:
		if (!m_bIsFullScreening)
		{
			OnFullScreen(TRUE);
			m_bIsFullScreening = TRUE;
			if (m_pLocalWndToolBar != NULL)
			{
				m_pLocalWndToolBar->UpdateBtnStatus(TRUE, OPERA_ID_FULLSCREEN);
			}
			if (m_pLocalWndView != NULL)
			{
				m_pLocalWndView->UpdateMenuStatus(OPERA_ID_FULLSCREEN, TRUE);
			}
		}
		else
		{
			OnFullScreen(FALSE);
			m_bIsFullScreening = FALSE;
			if (m_pLocalWndToolBar != NULL)
			{
				m_pLocalWndToolBar->UpdateBtnStatus(TRUE, OPERA_ID_FULLSCREEN);
			}
			if (m_pLocalWndView != NULL)
			{
				m_pLocalWndView->UpdateMenuStatus(OPERA_ID_FULLSCREEN, FALSE);
			}
		}
		break;
	case OPERA_ID_STOP:
		{
			if (!m_bIsPlaying)
			{
				StartPlay(0, m_csFilePath);
			}
			else
			{
				StopPlay();
				OnUpdateWndTitle();
			}
		}
		break;
	case OPERA_ID_SLIDER:
		{
			int nPos = (int)lParam;
			if (!GPAPI_SetPlayPos(PT_HIK, m_lPlayPort, nPos * 0.01f))
			{
				PLAYBACKDLL_ERROR(_T("local GPAPI_SetPlayPos falid, port = %d"), m_lPlayPort);
			}
		}
		break;
	default:
		break;
	}
	return 0;
}


void CLocalVideoFilePlay::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 0:  //���ò��Ž���
		{
			int nPos = (int)(GPAPI_GetPlayPos(PT_HIK, m_lPlayPort) * 100);
			if (m_pLocalWndToolBar != NULL)
			{
				m_pLocalWndToolBar->SetPlayPos(nPos);
			}
			if (nPos == 100)
			{
				StopPlay();
			}
			if (m_bStopPlay )   //�Ƿ��Ѿ����Ž������ɽ����ص�����(��ֹ���ŵ����1��ʱͣ��)
			{
				StopPlay();
			}
		}
		break;
	case 1:  //���ò���ʱ��
		{
			DWORD dwCurTime = GPAPI_GetPlayedTime(PT_HIK, m_lPlayPort);
			CString strCurTime = SecondToStrTime(dwCurTime);
			if (m_pLocalWndToolBar)
			{
				strCurTime = strCurTime + _T("/") + m_strTotalTime;
				m_pLocalWndToolBar->SetTimeInfo(strCurTime);
			}
		}
		break;

	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

// ץͼ
BOOL CLocalVideoFilePlay::OnCapture()
{
	if (m_lPlayPort != -1)
	{
		CString csPicType;
		PICTUREFORMAT picFmt;
		if (theApp.m_stPlaybackParam.nPicType == 0)
		{
			csPicType = _T(".bmp");
			picFmt = PF_BMP;
		}
		else
		{
			csPicType = _T(".jpeg");
			picFmt = PF_JPEG;
		}

		ENUM_PIC_TYPE emPicType;
		TCHAR szPicPath[MAX_PATH];
		MByteToWChar(theApp.m_stPlaybackParam.szPicPath, szPicPath, MAX_PATH);
		CString strPicFile;
		strPicFile.Format(_T("%s"), szPicPath);

		if (strPicFile.IsEmpty())
		{
			AppFun_GetAppPath(strPicFile);
		}
		CTime tmCurrent = CTime::GetCurrentTime();
		CString strFileName;
		strFileName = strPicFile + _T("���ػط�\\");
		strFileName.AppendFormat(_T("%04d%02d%02d\\%02d%02d%02d%s"), 
			tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 
			tmCurrent.GetHour(), tmCurrent.GetMinute(), tmCurrent.GetSecond(), (LPCTSTR)csPicType);

		//����·��
		MakeSureDirectoryPathExists(CStringW2A(strFileName));

		if (GPAPI_Capture(PT_HIK, m_lPlayPort, (CStringW2A(strFileName)).GetBuffer(), picFmt))
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
				strFileName.ReleaseBuffer();
				m_pCaptureTip->LoadPic(strFileName.GetBuffer());
				strFileName.ReleaseBuffer();
				m_pCaptureTip->ShowWindow(SW_SHOW);
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

// ȫ����ʾ
void CLocalVideoFilePlay::OnFullScreen(BOOL bFull)
{
	if (bFull)
	{
		GetWindowRect(&m_rcWndSize);
		GetParent()->ScreenToClient(&m_rcWndSize);

		//��ȡ��ǰ�����ĸ���Ļ
		HMONITOR hMonitor = MonitorFromWindow(this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);
		MONITORINFOEX mi;
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(hMonitor, &mi);
		//����ȫ��
		SetParent(m_pFullScreenWnd);
		if (m_pFullScreenWnd != NULL)
		{
			m_pFullScreenWnd->MoveWindow(&(mi.rcMonitor));
			CRect rcFullScreen;
			m_pFullScreenWnd->GetClientRect(&rcFullScreen);
			MoveWindow(rcFullScreen);
			m_pFullScreenWnd->ShowWindow(SW_SHOW);
			ShowWindow(SW_SHOW);
		}
	}
	else
	{
		SetParent(m_pFullScreenParentWnd);
		m_pFullScreenWnd->ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
		MoveWindow(&m_rcWndSize);
	}
}

//ˢ�±���
void CLocalVideoFilePlay::OnUpdateWndTitle()
{
	CString strText(m_csFileName);
	if (m_bIsPlaying)
	{
		if (m_bIsPausing)
		{
			strText.AppendFormat(_T("%s"), _T("  �ط���ͣ..."));
		}

		// ��ʾ�ٶ�.
		if (m_nSpeed != PS_NORMAL)
		{
			// ����.
			if (m_nSpeed > 0)
			{
				strText.AppendFormat(_T("  X%d����../"), (int)pow(2, (double)m_nSpeed));
			}
			// ����.
			else if (m_nSpeed < 0)
			{
				strText.AppendFormat(_T("  X1/%d����../"), (int)pow(2, (double)-m_nSpeed));
			}
		}

		//ȥ�����һ���ַ�'/'
		int nIndex = strText.ReverseFind(_T('/'));
		if (nIndex >= 0)
		{
			strText = strText.Left(nIndex);
		}
	}
	else
	{
		strText = _T("");
	}

	if (m_pLocalWndTitle != NULL)
	{
		m_pLocalWndTitle->SetText(strText);
	}
}

// ʱ��ת��
CString CLocalVideoFilePlay::SecondToStrTime( DWORD seconds )
{
	DWORD dwTime = seconds;
	int hour = dwTime/(60*60);
	int minute = (dwTime%(60*60))/60;
	int s = dwTime%60;
	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d"), hour, minute,s);

	return strTime;   
}
