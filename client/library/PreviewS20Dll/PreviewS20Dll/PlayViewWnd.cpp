/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			PlayViewWnd.cpp
Description:  		Ԥ������ ����Ԥ�����Ŵ��ں�Ԥ��������
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-4-7       qmx      ����
3.0        2012-4-17     nobody    ����
*****************************************************/
//lint -library
#include "stdafx.h"
#include "PlayViewWnd.h"
#include "PreviewS20Dll.h"
#include "util/AreaDivision.h"


IMPLEMENT_DYNAMIC(CPlayViewWnd, CWnd)

CPlayViewWnd::CPlayViewWnd()
    : m_pPlayView(NULL)
    , m_pParentWnd(NULL)
    , m_pToolBarWnd(NULL)
    , m_pLayoutMoreWnd(NULL)
    , m_pFullScreenWnd(NULL)
    , m_bIsWndInit(FALSE)
    , m_bWndFullScreen(FALSE)
    , m_nWndNum(4)
    , m_wLayoutMode(PREVIEW_ARRANGE_TYPE_4_4_A)
{
	m_nResID = 0;
	m_nLastChgedResID = 0;
}

CPlayViewWnd::~CPlayViewWnd()
{
	m_stCamInfo.Clear();
}


BEGIN_MESSAGE_MAP(CPlayViewWnd, CWnd)
	ON_MESSAGE(WM_PV_TOOLBAR_BTN_CLK, OnToolBarBtnClk)
	ON_MESSAGE(WM_PV_TOOLBAR_EXECUTE, OnToolBarBtnExecute)
    ON_MESSAGE(WM_LAYOUTCLOSE, OnLayoutMoreWndClose)
	ON_MESSAGE(WM_PV_TOOLBAR_BTN_CLK_REF, OnToolBarBtnClkRef)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_PV_REALPLAY_STOPPREVIEW, &CPlayViewWnd::OnUpdateBtnState)
END_MESSAGE_MAP()


/*************************************************
Function:    	Fun_Create
Description:	��������
Input:			rect ���ڵ�λ��
pParent ������
unsigned int uiID ���ڵ�id
dwStyle ���ڵ�����
uiStartWndIndex  ��ʼ���
uiMaxWndNum      ���Ŵ�����
Output:      	
Return:			�Ƿ񴴽��ɹ�
*************************************************/
BOOL CPlayViewWnd::CreateWnd(const RECT & rect, CWnd * pParent, unsigned int uiID,DWORD dwSyle,DWORD dwFunStyle)
{
	//���游����ָ��
	m_pParentWnd = pParent;
	
	//��������
	CWnd* pWnd = this;
	pWnd->Create(NULL,_T("PlayViewWnd"),WS_VISIBLE|WS_CHILD|WS_TABSTOP,rect,pParent,uiID);

	//��������Դ
	CreateChildWnd();

	m_bIsWndInit = TRUE;

	return TRUE;
}

/*************************************************
Function:    	CreateChildWnd
Description:	��������Դ
Input:			
Output:      	
Return:			
*************************************************/
void CPlayViewWnd::CreateChildWnd()
{
	//����ȫ����������
	if (NULL == m_pFullScreenWnd)
	{
		m_pFullScreenWnd = new CWnd;
	}
	if (NULL != m_pFullScreenWnd)
	{
		m_pFullScreenWnd->CreateEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW, AfxRegisterWndClass(NULL), 
			NULL,WS_POPUP|WS_VISIBLE,CRect(0,0,0,0), NULL, 0);

		CWnd* pScreenWnd = GetDesktopWindow();
		CRect rc;
		pScreenWnd->GetClientRect(&rc);
		m_pFullScreenWnd->MoveWindow(&rc);
		m_pFullScreenWnd->ShowWindow(SW_HIDE);
	}

	//Ԥ������
	if (NULL == m_pPlayView)
	{
		m_pPlayView = new CPlayView;
	}
	if (NULL != m_pPlayView)
	{
		m_pPlayView->CreateWnd(CRect(0,0,800,600),this,ID_WND_PLAYVIEW,0,PREVIEW_WND_MAXNUM);
	}
	
	//������
	if (theApp.m_stPreviewParam.nToolBarRight)
	{
		if (NULL == m_pToolBarWnd)
		{
			m_pToolBarWnd = new CPlayViewToolbar;
		}
		if (NULL != m_pToolBarWnd)
		{
			m_pToolBarWnd->CreateWnd(CRect(0,300,300,24),this,ID_WND_PLAYVIEWTOOLBAR);
		}
	}

	m_pPlayView->SetFirstWndWave(&m_pToolBarWnd->m_scpVideoWav, &m_pToolBarWnd->m_scpAudioWav);
}

void CPlayViewWnd::OnDestroy()
{
	if (m_pPlayView)
	{
		m_pPlayView->DestroyWindow();
		delete m_pPlayView;
		m_pPlayView = NULL;
	}

	if (m_pToolBarWnd)
	{
		m_pToolBarWnd->DestroyWindow();
		delete m_pToolBarWnd;
		m_pToolBarWnd = NULL;
	}

	if (m_pFullScreenWnd)
	{
		m_pFullScreenWnd->DestroyWindow();
		delete m_pFullScreenWnd;
		m_pFullScreenWnd = NULL;
	}

	if(m_pLayoutMoreWnd)
	{
		m_pLayoutMoreWnd->DestroyWindow();
		delete m_pLayoutMoreWnd;
		m_pLayoutMoreWnd = NULL;
	}
}

void CPlayViewWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bIsWndInit)
	{
		InitPose();
	}
}

/*************************************************
Function:    	InitPose
Description:	��ʼ��λ��
Input:			
Output:      	
Return:			
*************************************************/
void CPlayViewWnd::InitPose()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	
	//���Ŵ���
	CRect rcPlayView(rcClient);
    if (theApp.m_stPreviewParam.nToolBarRight)
    {
        rcPlayView.bottom = rcClient.bottom - CTRL_PREVIEW_TOOLBAR_HEIGHT;
    }
	if(m_pPlayView)
	{
		m_pPlayView->MoveWindow(rcPlayView);
		m_pPlayView->ShowWindow(SW_SHOW);
	}

	//������λ��
	CRect rcPlayViewToolBar(rcClient);
	rcPlayViewToolBar.top = rcPlayViewToolBar.bottom - CTRL_PREVIEW_TOOLBAR_HEIGHT;
	rcPlayViewToolBar.left = rcPlayView.left;
	rcPlayViewToolBar.right = rcPlayView.right;
	if (NULL != m_pToolBarWnd)
	{
		m_pToolBarWnd->MoveWindow(&rcPlayViewToolBar);
		m_pToolBarWnd->ShowWindow(SW_SHOW);
	}
}

/*******************************************************************************
*    Function:      SetLayoutMode
*    Description:   ���ò��Ŵ��ڷָ�ģʽ
*    Input:         WORD wMode,�ָ�ģʽ
*    Output:        
*    Return:	     
*******************************************************************************/
BOOL CPlayViewWnd::SetLayoutMode(WORD wMode)
{
	//���ģʽû�ı�
	if (m_wLayoutMode == wMode)
	{
		return FALSE;
	}
	
	//�޸Ļ���ָ�
	if (NULL != m_pPlayView)
	{
		m_pPlayView->SetLayoutMode(wMode);
		m_wLayoutMode = wMode;	
	}
	
	//���õ�ǰ�������
	m_nWndNum = AppFun_GetLayoutWndNum(m_wLayoutMode);
	
	if (m_pLayoutMoreWnd)
	{
		m_pLayoutMoreWnd->SetFocusBtn(wMode);
	}

	return TRUE;
}

/*******************************************************************************
*    Function:      SetPlayViewWndFullScreen
*    Description:   ���ò��Ŵ���ȫ��/��ȫ��
*    Input:         
*    Output:        
*    Return:	     
*******************************************************************************/
BOOL CPlayViewWnd::SetPlayViewWndFullScreen()
{
	m_bWndFullScreen = !m_bWndFullScreen;
	
	if (m_bWndFullScreen)
	{
		//����ȫ��ǰ��λ��
		GetWindowRect(&m_rcWndPos);
		GetParent()->ScreenToClient(&m_rcWndPos);

		//����ȫ��
		if (NULL != m_pFullScreenWnd)
		{
			CWnd* pScreenWnd = GetDesktopWindow();
			CRect rc;
			pScreenWnd->GetClientRect(&rc);
			SetParent(m_pFullScreenWnd);
            m_pToolBarWnd->UpdateToolTips();
			MoveWindow(&rc);
			m_pFullScreenWnd->ShowWindow(SW_SHOW);
			ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (NULL != m_pFullScreenWnd)
		{
            SetParent(m_pParentWnd);
            m_pToolBarWnd->UpdateToolTips();
			m_pFullScreenWnd->ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
			MoveWindow(&m_rcWndPos);
		}
	}

	//����ȫ����ť
	if(m_bWndFullScreen)
	{
		if (NULL != m_pToolBarWnd)
		{
			m_pToolBarWnd->SetFullScreenON();
		}
	}
	else
	{
		if (NULL != m_pToolBarWnd)
		{
			m_pToolBarWnd->SetFullScreenOff();
		}	
    }

    // ����ȫ��״̬
    if (m_pPlayView != NULL)
    {
        m_pPlayView->SetFullScreenStatus(m_bWndFullScreen);
    }

	return TRUE;
}

/*******************************************************************************
*    Function:      OnBtnClick
*    Description:   ��Ӧ������btn�¼�
*    Input:         
*    Output:        
*    Return:	     
*******************************************************************************/
LRESULT CPlayViewWnd::OnToolBarBtnClk(WPARAM wParam, LPARAM lParam)
{
	int nCmd = (int)wParam;
	switch(nCmd)
	{
		case IDC_BTN_PT_ARRANGE:
			OnClkLayoutMoreBtn();
			break;
		case IDC_BTN_PT_ARRANGE_1:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_1_1_A);	
			break;
		case IDC_BTN_PT_ARRANGE_4:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_2_2_A);
			break;
		case IDC_BTN_PT_ARRANGE_9:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_3_3_A);
			break;
		case IDC_BTN_PT_ARRANGE_16:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_4_4_A);
			break;	
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_6:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_3_3_B);
			break;
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_8:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_4_4_D);
			break;	
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_10:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_4_4_C);
			break;	
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_13:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_4_4_B);
			break;	
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_14:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_5_5_D);
			break;
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_17:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_5_5_C);
			break;
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_22:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_5_5_B);
			break;
		case ID_WND_PLAYVIEWTOOLBAR_ARRANGE_25:
			SetLayoutMode(PREVIEW_ARRANGE_TYPE_5_5_A);
			break;
		case IDC_BTN_PT_FULLSCREEN:
			SetPlayViewWndFullScreen();
			break;
		case IDC_BTN_PT_STOPALL:
			m_veCamID.clear();
			StopPreview();
			break;
		case IDC_BTN_PT_STARTRECORD:
			{
				BOOL bStart = StartRecord();
				if (!bStart)
				{
					m_pToolBarWnd->ResetVideo();
				}
			}
			break;
		case IDC_BTN_PT_STOPRECORD:
			StopRecord();
			break;
		case IDC_BTN_PT_VIDEOWAVE:
			StartPreview(m_stCamInfo);
			break;
		case IDC_BTN_PT_VIDEOWAVE_STOP:
			StopPreview();
			break;
		case IDC_BTN_PT_COMMERROR:
			{
				if (m_pToolBarWnd != NULL)
				{
					m_pToolBarWnd->ResetVideo();
				}
			}
			break;
		default:
			break;
	}

	return 0;
}

LRESULT CPlayViewWnd::OnToolBarBtnExecute(WPARAM wParam, LPARAM lParam)
{
	int nCmd = (int)wParam;
	switch(nCmd)
	{
	case IDC_BTN_PT_CAPTURE:
		m_pPlayView->OnToolBarExecute(MENU_ID_CAPTURE);
		break;
	case IDC_BTN_PT_SOUND:
		m_pPlayView->OnToolBarExecute(MENU_ID_SOUND);
		break;
	case IDC_BTN_PT_STREAM:
		m_pPlayView->OnToolBarExecute(MENU_ID_STREAMTYPE);
		break;
	case IDC_BTN_PT_OSD:
		m_pPlayView->OnToolBarExecute(MENU_ID_OSD);
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CPlayViewWnd::OnToolBarBtnClkRef(WPARAM wParam, LPARAM lParam)
{
	int nCmd = (int)wParam;
	switch(nCmd)
	{
	case PT_CMD_START_RECORD:
		{
			m_pToolBarWnd->OnBnClickedPlayviewtoolbarRecordRef();
		}
		break;
	case PT_CMD_STOP_RECORD:
		{
			m_pToolBarWnd->OnBnClickedPlayviewtoolbarRecordRef();
		}
		break;
	case PT_CMD_DISABLE_RECORD:
		{
			CString* lpStr = (CString*)lParam;
			CString strMsg = *lpStr;
			delete lpStr;
			m_pToolBarWnd->OnBnRecordPlayviewtoolbarEnableRef(FALSE, strMsg);
		}
		break;
	case PT_CMD_ENABLE_RECORD:
		{
			CString* lpStr = (CString*)lParam;
			CString strMsg = *lpStr;
			delete lpStr;
			m_pToolBarWnd->OnBnRecordPlayviewtoolbarEnableRef(TRUE, strMsg);
		}
		break;
	default:
		break;
	}

	return 0;
}

/*******************************************************************************
*    Function:      OnClkLayoutMoreBtn
*    Description:   ������չ�ָ��
*    Input:         
*    Output:        
*    Return:	     
*******************************************************************************/
void CPlayViewWnd::OnClkLayoutMoreBtn()
{
	if (NULL == m_pLayoutMoreWnd)
	{
		m_pLayoutMoreWnd = new CLayoutMoreWnd;
	}

	if (NULL == m_pLayoutMoreWnd)
	{
		return;
	}

	CRect rcToolbar(0,0,0,0);
	if (NULL != m_pToolBarWnd)
	{
		m_pToolBarWnd->GetWindowRect(&rcToolbar);
	}

	if(m_pLayoutMoreWnd->GetSafeHwnd() == NULL)
	{
		m_pLayoutMoreWnd->LayoutMoreSetWnd_Create(CRect(0,0,290,46),this,ID_WND_PLAYVIEW_LAYOUTMORE,MODE_NORMALSCREEN);
	}

	if(m_pLayoutMoreWnd && m_pLayoutMoreWnd->GetSafeHwnd())
	{
		CRect rectDlg;
		m_pLayoutMoreWnd->GetWindowRect(&rectDlg);
		m_pLayoutMoreWnd->SetFocusBtn(m_wLayoutMode);
		m_pLayoutMoreWnd->MoveWindow(rcToolbar.right - rectDlg.Width() - 25,rcToolbar.top - rectDlg.Height(),rectDlg.Width(),rectDlg.Height());
	}	
}

/*******************************************************************************
*    Function:      OnClkLayoutMoreBtn
*    Description:   ������չ�ָ��
*    Input:         
*    Output:        
*    Return:	     
*******************************************************************************/
LRESULT CPlayViewWnd::OnLayoutMoreWndClose(WPARAM wparam, LPARAM lparam) 
{
	if(m_pLayoutMoreWnd)
	{
		m_pLayoutMoreWnd->DestroyWindow();
		delete m_pLayoutMoreWnd;
		m_pLayoutMoreWnd = NULL;
	}

	return 0;
}

/*******************************************************************************
*    Function:      StartPreview
*    Description:   ��ѡ�еĴ�����Ԥ��
*    Input:			struCamInfo,��ص���Ϣ
*    Output:        
*    Return:	     
*******************************************************************************/
BOOL CPlayViewWnd::StartPreview(const PREVIEW_CAMINFO &struPreviewInfo)
{
    if (theApp.m_bUSBExit)
    {
        return FALSE;   //���û������� ëҲ����
    }

//     BOOL bRet = FALSE;
//     bRet = USBCamera_SetFilter(USBCAMERA_ALL);  //ע�������豸
//     if (!bRet)
//     {
//         PV_ERROR("USBCamera_SetFilter Failed![Error:%d]", USBCamera_GetLastError());
//         return FALSE;
//     }

    if (NULL != m_pPlayView)
    {
        BOOL bPreviewing = FALSE;
        m_stCamInfo = struPreviewInfo;
        bPreviewing = m_pPlayView->StartPreview(struPreviewInfo);
        return bPreviewing;
    }

	return FALSE;
}

/*******************************************************************************
*    Function:      StopPreview
*    Description:   ֹͣԤ��
*    Input:         
*    Output:        
*    Return:	     
*******************************************************************************/
void CPlayViewWnd::StopPreview()
{
	if (NULL != m_pPlayView)
	{
		m_pPlayView->StopPreview();	
	}
}

LRESULT CPlayViewWnd::OnUpdateBtnState(WPARAM wParam, LPARAM lParam)
{
	int nMenuID = (int)wParam;
	int nState = (int)lParam;
	if (NULL != m_pToolBarWnd)
	{
		m_pToolBarWnd->UpdateBtnState(nMenuID, nState);
	}
    return 0;
}

BOOL CPlayViewWnd::StartRecord()
{
	if (NULL != m_pPlayView)
	{
		BOOL bRecording = FALSE;
		bRecording = m_pPlayView->StartRecord();
		return bRecording;
	}

	return FALSE;
}

BOOL CPlayViewWnd::StopRecord()
{
	if (NULL != m_pPlayView)
	{
		return m_pPlayView->StopRecord();
	}

	return FALSE;
}

BOOL CPlayViewWnd::StartSound()
{
    if (m_pPlayView != NULL)
    {
        return m_pPlayView->OpenSound();
    }

    return FALSE;
}

BOOL CPlayViewWnd::StopSound()
{
    if (NULL != m_pPlayView)
    {	
        return m_pPlayView->CloseSound();
    }

    return FALSE;
}

BOOL CPlayViewWnd::GetRecordFile(RECOED_FILE_INFO& recordFile)
{
	if (NULL != m_pPlayView)
	{	
		return m_pPlayView->GetRecordFile(recordFile);
	}

	return FALSE;
}

BOOL CPlayViewWnd::SetEventFunc(PV_EVENT_FUNC Func)
{
	if (NULL != m_pPlayView)
	{	
		return m_pPlayView->SetEventFunc(Func);
	}

	return FALSE;
}

BOOL CPlayViewWnd::SetShowString( const CString &strBusinessInfo)
{
	if (NULL != m_pPlayView)
	{	
		BOOL bRet = m_pPlayView->SetShowString(strBusinessInfo);
		return bRet;
	}
	return FALSE;
}

BOOL CPlayViewWnd::OpenSound()
{
	if (NULL != m_pPlayView)
	{	
		m_pPlayView->OnToolBarExecute(MENU_ID_SOUND);
		return TRUE;
	}
	return FALSE;
}

