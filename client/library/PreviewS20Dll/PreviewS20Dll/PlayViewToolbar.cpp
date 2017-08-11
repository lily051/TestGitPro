//lint -library
#include "stdafx.h"
#include "PlayViewToolbar.h"
#include "./PreviewS20Dll.h"
#include "ui/memdcex.h"
#include "util/AreaDivision.h"

IMPLEMENT_DYNAMIC(CPlayViewToolbar, CWnd)

CPlayViewToolbar::CPlayViewToolbar()
: m_bIsInit(FALSE)
, m_pParrentWnd(NULL)
, m_bRecording(FALSE)
, m_bPreviewing(FALSE)
, m_bSounding(FALSE)
, m_bMainStreaming(FALSE)
, m_bShowOsding(FALSE)
, m_bVideoUser(TRUE)
{
}

CPlayViewToolbar::~CPlayViewToolbar()
{
}

BEGIN_MESSAGE_MAP(CPlayViewToolbar, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PT_FULLSCREEN, OnBnClickedPlayviewtoolbarFullScreen)
	ON_BN_CLICKED(IDC_BTN_PT_STARTRECORD, OnBnClickedPlayviewtoolbarRecord)
	ON_BN_CLICKED(IDC_BTN_PT_VIDEOWAVE, OnBnClickedPlayviewtoolbarPreview)
	ON_BN_CLICKED(IDC_BTN_PT_CAPTURE, OnBnClickedPlayviewtoolbarCapture)
	ON_BN_CLICKED(IDC_BTN_PT_SOUND, OnBnClickedPlayviewtoolbarSound)
	ON_BN_CLICKED(IDC_BTN_PT_STREAM, OnBnClickedPlayviewtoolbarStream)
	ON_BN_CLICKED(IDC_BTN_PT_OSD, OnBnClickedPlayviewtoolbarOSD)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/*************************************************
Function:    	CreateWnd
Description:	��������
Input:			rect ���ڵ�λ��
pParent			������
unsigned int uiID ���ڵ�id
dwStyle ���ڵ�����
uiStartWndIndex  ��ʼ���
uiMaxWndNum      ���Ŵ�����
Output:      	
Return:			�Ƿ񴴽��ɹ�
*************************************************/
BOOL CPlayViewToolbar::CreateWnd(const RECT & rect, CWnd * pParent, unsigned int uiID)
{
	//��������
	CWnd* pWnd = this;
	pWnd->Create(NULL, _T("PlayViewToolbar"),WS_VISIBLE|WS_CHILD|WS_TABSTOP,rect, pParent, uiID);

	CreateChildWnd();

	InitCtrl();
	
	m_pParrentWnd = pParent;
	m_bIsInit = TRUE;
		
	return TRUE;
}

void CPlayViewToolbar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(rcClient);

	CBitmap bmpBake;
	bmpBake.Attach(g_ResourceLoader.ResourceLoad_Bmp(IDB_BMP_REALWND_TOOLBAR));

	BITMAP bm;
	bmpBake.GetBitmap(&bm);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(bmpBake);

	dc.StretchBlt(0,0,rcClient.Width(),rcClient.Height(),
		&dcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
}

/*************************************************
Function:    	CreateChildWnd
Description:	��������Դ
Input:			
Output:      	
Return:			
*************************************************/
void CPlayViewToolbar::CreateChildWnd(void)
{
	m_btnRecord.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,0,0), this, IDC_BTN_PT_STARTRECORD);

	m_btnFullScreen.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(10,70,100,100), this, IDC_BTN_PT_FULLSCREEN);

	m_btnPreview.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,0,0), this, IDC_BTN_PT_VIDEOWAVE);

	m_btnCapture.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,0,0), this, IDC_BTN_PT_CAPTURE);

	m_btnSound.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,0,0), this, IDC_BTN_PT_SOUND);

	m_btnStream.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,0,0), this, IDC_BTN_PT_STREAM);

	if (0 != theApp.m_nBtnRights)
	{
		m_btnOSD.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(0,0,0,0), this, IDC_BTN_PT_OSD);
	}

    //m_scpVideoWav.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,\
		CRect(0,0,0,0), this, IDC_STC_PT_VIDEOWAV);

	m_scpAudioWav.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,0,0), this, IDC_STC_PT_AUDIOWAV);

	m_btnPhoneWave.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		CRect(0,0,0,0), this, IDC_BTN_PT_PHONEWAVE);
}


void CPlayViewToolbar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bIsInit)
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
void CPlayViewToolbar::InitPose(void)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	
	int nBtnWid = 45;			//ͨ�ð�ť���
	int nBtnHeit = 45;			//��ť�߶�
	int nBtnSpace = 6;			//��ť���
	int nBtnSmallWid = 24;

	if (0 == theApp.m_nPreviewModel)
	{
		//���¼��ť
		int nTop = 2;
		int nLLeft = rcClient.left;

		m_btnRecord.MoveWindow(nLLeft,nTop,nBtnWid,nBtnHeit);
		if (1 == theApp.m_nPreviewModel)
		{
			m_btnRecord.EnableWindow(FALSE);
		}

		//ȫ����ť
		nLLeft += nBtnWid;
		m_btnFullScreen.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//��ʼԤ����ť
		nLLeft += nBtnWid;
		m_btnPreview.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//ץͼ
		nLLeft += nBtnWid;
		m_btnCapture.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//��������
		nLLeft += nBtnWid;
		m_btnSound.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//��������
		nLLeft += nBtnWid;
		m_btnStream.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		if (0 != theApp.m_nBtnRights)
		{
			//OSD��Ϣ��ʾ
			nLLeft += nBtnWid;
			m_btnOSD.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);
		}

		//�������ΰ�ť
// 		nLLeft += nBtnWid;
// 		m_scpVideoWav.MoveWindow(nLLeft,nTop-2,rcClient.right-nLLeft-2,nBtnHeit);
	}
	else
	{
		//��ʼԤ����ť
		int nTop = 2;
		int nLLeft = rcClient.left;
		m_btnPreview.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//��������
		nLLeft += nBtnWid;
		m_btnSound.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//��������
		nLLeft += nBtnWid;
		m_btnStream.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//�������ΰ�ť
// 		nLLeft += nBtnWid;
// 		m_scpVideoWav.MoveWindow(nLLeft,nTop-2,rcClient.right-nLLeft-2,nBtnHeit);
	}
}



void CPlayViewToolbar::InitCtrl(void)
{
	m_btnFullScreen.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_PLAYVIEW_FULLSCREEN),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));

	m_btnRecord.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD_HOVER));

	m_btnPreview.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_PREVIEW),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));

    m_btnCapture.LoadbakeImage(
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CAPTURE),
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));

    m_btnSound.LoadbakeImage(
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_SOUND),
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));

    m_btnStream.LoadbakeImage(
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MAIN_STREAM),
        RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));

	if (0 != theApp.m_nBtnRights)
	{
		m_btnOSD.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SHOW_OSD_INFO),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
	}

	m_btnPhoneWave.LoadbakeImage( \
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),\
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_PHONE_WAVE),\
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));

	//��Ӱ�ť��ʾ
	SetButtonToolTip();

    CheckShowingBtns();
}
// ¼����ͣ
void CPlayViewToolbar::OnBnClickedPlayviewtoolbarRecord()
{
	if (!m_bPreviewing)
	{
		MsgBoxApi_Show(_T("���ȿ���Ԥ��."), MSGBOX_TYPE_WARNING);
		return;
	}
	if (!m_bVideoUser)
	{
		MessageBox(m_strVideoMsg, _T("��ʾ"), MB_ICONINFORMATION);
		return;
	}

	if (m_bRecording)
	{
		OnClkToolBarBtn(IDC_BTN_PT_STOPRECORD,NULL);
		m_btnRecord.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD_HOVER));
		m_bRecording = FALSE;
	}
	else
	{
		OnClkToolBarBtn(IDC_BTN_PT_STARTRECORD,NULL);
		m_btnRecord.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_STOP_RECORD),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_STOP_RECORD_HOVER));
		m_bRecording = TRUE;
	}

	UpdateRecordTips(m_bRecording);
}
// Ԥ����ͣ
void CPlayViewToolbar::OnBnClickedPlayviewtoolbarPreview()
{
	if (m_bRecording)
	{
		MsgBoxApi_Show(_T("¼���У���ֹֹͣԤ��."), MSGBOX_TYPE_WARNING);
		return;
	}
	if (m_bPreviewing)
	{
		OnClkToolBarBtn(IDC_BTN_PT_VIDEOWAVE_STOP,NULL);
		m_btnPreview.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_PREVIEW),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bPreviewing = FALSE;
	}
	else
	{
		OnClkToolBarBtn(IDC_BTN_PT_VIDEOWAVE,NULL);
		m_btnPreview.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_STOP_PREVIEW),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bPreviewing = TRUE;
	}

	UpdatePreviewTips(m_bPreviewing);
}

void CPlayViewToolbar::OnBnClickedPlayviewtoolbarCapture()
{
	if (!m_bPreviewing)
	{
		return;
	}
	::PostMessage(GetParent()->m_hWnd,WM_PV_TOOLBAR_EXECUTE,IDC_BTN_PT_CAPTURE,0);
}

void CPlayViewToolbar::OnBnClickedPlayviewtoolbarSound()
{
	if (!m_bPreviewing)
	{
		return;
	}
	if (m_bSounding)
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_OPEN_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bSounding = TRUE;
	}
	else
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bSounding = FALSE;
	}
	UpdateSoundTips(m_bSounding);

	::PostMessage(GetParent()->m_hWnd,WM_PV_TOOLBAR_EXECUTE,IDC_BTN_PT_SOUND,0);
}

void CPlayViewToolbar::OnBnClickedPlayviewtoolbarStream()
{
	if (!m_bPreviewing)
	{
		return;
	}
	if (m_bRecording)
	{
		MsgBoxApi_Show(_T("����¼���У���֧�������л�."), MSGBOX_TYPE_WARNING);
		return;
	}
	::PostMessage(GetParent()->m_hWnd,WM_PV_TOOLBAR_EXECUTE,IDC_BTN_PT_STREAM,0);
}

void CPlayViewToolbar::OnBnClickedPlayviewtoolbarOSD()
{
	if (!m_bPreviewing)
	{
		return;
	}
	::PostMessage(GetParent()->m_hWnd,WM_PV_TOOLBAR_EXECUTE,IDC_BTN_PT_OSD,0);
}

void CPlayViewToolbar::OnBnClickedPlayviewtoolbarFullScreen()
{
	OnClkToolBarBtn(IDC_BTN_PT_FULLSCREEN,NULL);
}

LRESULT CPlayViewToolbar::OnClkToolBarBtn(WPARAM wparam, LPARAM lparam) 
{
	::PostMessage(GetParent()->m_hWnd,WM_PV_TOOLBAR_BTN_CLK,wparam,lparam);

	return S_OK;
}

void CPlayViewToolbar::SetFullScreenON(void)
{
	m_btnFullScreen.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_PLAYVIEW_ESCFULLSCREEN),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_PLAYVIEW_ESCFULLSCREEN));
}

void CPlayViewToolbar::SetFullScreenOff(void)
{
	m_btnFullScreen.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_PLAYVIEW_FULLSCREEN),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_PLAYVIEW_FULLSCREEN));
	
	//m_btnPreCfg.ShowWindow(SW_SHOW);
	//m_btnPreGroupCfg.ShowWindow(SW_SHOW);

#ifdef MODULE_BROADCAST
    m_btnBroadcast.ShowWindow(SW_SHOW);
#endif

#ifdef MODULE_VERIFICATION
	m_btnClusterShock.ShowWindow(SW_SHOW);
#endif
}

void CPlayViewToolbar::CheckShowingBtns()
{
    if (theApp.m_stPreviewParam.nToolBarRight & PV_TOOLBAR_FULLSCREEN)
    {
        m_btnFullScreen.ShowWindow(SW_SHOW);
    } 
    else
    {
        m_btnFullScreen.ShowWindow(SW_HIDE);
    }

	m_btnRecord.ShowWindow(SW_SHOW);
	m_scpVideoWav.ShowWindow(SW_SHOW);
	m_scpAudioWav.ShowWindow(SW_SHOW);
}

void CPlayViewToolbar::SetButtonToolTip()
{
    // �໭��ȫ��
    m_btnFullScreen.SetTooltipText(STR_PREVIEW_TOOLBAR_FULLSCREEN);
	// ¼��
	m_btnRecord.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTRECORD);
	// Ԥ��
	m_btnPreview.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTPREVIEW);
	m_btnCapture.SetTooltipText(_T("ץͼ"));
    m_btnSound.SetTooltipText(_T("������Ƶ"));
	m_btnStream.SetTooltipText(_T("������"));
	m_btnOSD.SetTooltipText(_T("��ʾˮӡ"));
}

void CPlayViewToolbar::UpdateToolTips()
{
    // �໭��ȫ��
    m_btnFullScreen.UpdateTooltip();
	m_btnRecord.UpdateTooltip();
}

BOOL CPlayViewToolbar::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);

	//CMemDCEX memDC(pDC,rectClient);
	//m_DrawBakeFun.DrawStrip_DrawFun(&memDC,&rectClient);
	
	return TRUE;
}

void CPlayViewToolbar::ResetVideo(void)
{
	KillTimer(RECORD_TIMER);
	m_btnRecord.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD_HOVER));
	m_bRecording = FALSE;
}

void CPlayViewToolbar::SetPreviewStatus(BOOL bPreviewing,  int nStreamType)
{
	if (bPreviewing)		// Ԥ����
	{
		m_btnPreview.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_STOP_PREVIEW));
	}
	else						
	{
		m_btnPreview.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_PREVIEW));
	}

	m_bPreviewing = bPreviewing;
	UpdatePreviewTips(bPreviewing);
}

void CPlayViewToolbar::SetRecordStatus(BOOL bRecording)
{
	if (bRecording)		// ¼����
	{
		m_btnRecord.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_STOP_RECORD),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_STOP_RECORD_HOVER));
	}
	else						
	{
		m_btnRecord.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_TOOLBAR_START_RECORD_HOVER));
	}

	m_bRecording = bRecording;
	UpdateRecordTips(bRecording);
}

void CPlayViewToolbar::UpdateRecordTips(BOOL bRecord)
{
	if (bRecord == FALSE)
	{
		// ��ʼ¼��
		m_btnRecord.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTRECORD);
	}
	else
	{
		// ֹͣ¼��
		m_btnRecord.SetTooltipText(STR_PREVIEW_TOOLBAR_STOPRECORD);
	}

	m_btnRecord.UpdateTooltip();
}

void CPlayViewToolbar::SetSoundStatus(BOOL bSounding)
{
	if (bSounding)
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_OPEN_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bSounding = TRUE;
	}
	else
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bSounding = FALSE;
	}

	UpdateSoundTips(m_bSounding);
}

void CPlayViewToolbar::SetStreamStatus(BOOL bMainStreaming)
{
	if (bMainStreaming)
	{
		m_btnStream.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MAIN_STREAM),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bMainStreaming = TRUE;
	}
	else
	{
		m_btnStream.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SUB_STREAM),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bMainStreaming = FALSE;
	}
	UpdateStreamTips(m_bMainStreaming);
}

void CPlayViewToolbar::SetOsdStatus(BOOL bShowOsding)
{
	if (bShowOsding)
	{
		m_btnOSD.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_HIDE_OSD_INFO),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bShowOsding = TRUE;
	}
	else
	{
		m_btnOSD.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SHOW_OSD_INFO),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		m_bShowOsding = FALSE;
	}
	UpdateOsdTips(m_bShowOsding);
}

void CPlayViewToolbar::UpdatePreviewTips(BOOL bPreviewing)
{
	if (bPreviewing == FALSE)
	{
		// ��ʼԤ��
		m_btnPreview.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTPREVIEW);
	}
	else
	{
		// ֹͣԤ��
		m_btnPreview.SetTooltipText(STR_PREVIEW_TOOLBAR_STOPPREVIEW);
	}

	m_btnPreview.UpdateTooltip();
}

void CPlayViewToolbar::UpdateSoundTips(BOOL bSounding)
{
    if (bSounding == FALSE)
    {
        // ��ʼ����
        m_btnSound.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTSOUND);
    }
    else
    {
        // �ر�����
        m_btnSound.SetTooltipText(STR_PREVIEW_TOOLBAR_STOPSOUND);
    }

    m_btnSound.UpdateTooltip();
}

void CPlayViewToolbar::UpdateStreamTips(BOOL bStream)
{
	if (bStream == FALSE)
	{
		// ������
		m_btnStream.SetTooltipText(STR_POPUPMENU_SUB_STREAM);
	}
	else
	{
		// ������
		m_btnStream.SetTooltipText(STR_POPUPMENU_MAIN_STREAM);
	}

	m_btnStream.UpdateTooltip();
}

void CPlayViewToolbar::UpdateOsdTips(BOOL bOsd)
{
	if (bOsd == FALSE)
	{
		// ��ʾOSD
		m_btnOSD.SetTooltipText(STR_POPUPMENU_SHOWOSD);
	}
	else
	{
		// ����OSD
		m_btnOSD.SetTooltipText(STR_POPUPMENU_HIDEOSD);
	}

	m_btnOSD.UpdateTooltip();
}

void CPlayViewToolbar::OnBnClickedPlayviewtoolbarRecordRef()
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_PT_STARTRECORD, BN_CLICKED), 0);
}

void CPlayViewToolbar::OnBnRecordPlayviewtoolbarEnableRef(BOOL bEnale, const CString& strMsg)
{
	m_bVideoUser = bEnale;
	m_strVideoMsg = strMsg;
}
void CPlayViewToolbar::UpdateBtnState(int nMenuID, int nState)
{
	switch(nMenuID)
	{
	case MENU_ID_STOP_PREVIEW:
		{
			BOOL bState = (BOOL)nState;
			SetPreviewStatus(bState,0);
		}
		break;
	case MENU_ID_RECORD:
		{
			BOOL bState = (BOOL)nState;
			SetRecordStatus(bState);
		}
		break;
	case MENU_ID_SOUND:
		{
			BOOL bState = (BOOL)nState;
			SetSoundStatus(bState);
		}
		break;
	case MENU_ID_STREAMTYPE:
		{
			BOOL bState = (BOOL)nState;
			SetStreamStatus(bState);
		}
		break;
	case MENU_ID_OSD:
		{
			BOOL bState = (BOOL)nState;
			SetOsdStatus(bState);
		}
		break;
	default:
		break;
	}
}