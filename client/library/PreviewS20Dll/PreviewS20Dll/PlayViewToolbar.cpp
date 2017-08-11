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
Description:	创建窗口
Input:			rect 窗口的位置
pParent			父窗口
unsigned int uiID 窗口的id
dwStyle 窗口的属性
uiStartWndIndex  起始序号
uiMaxWndNum      播放窗口数
Output:      	
Return:			是否创建成功
*************************************************/
BOOL CPlayViewToolbar::CreateWnd(const RECT & rect, CWnd * pParent, unsigned int uiID)
{
	//创建窗口
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
Description:	创建子资源
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
Description:	初始化位置
Input:			
Output:      	
Return:			
*************************************************/
void CPlayViewToolbar::InitPose(void)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	
	int nBtnWid = 45;			//通用按钮宽度
	int nBtnHeit = 45;			//按钮高度
	int nBtnSpace = 6;			//按钮间距
	int nBtnSmallWid = 24;

	if (0 == theApp.m_nPreviewModel)
	{
		//左边录像按钮
		int nTop = 2;
		int nLLeft = rcClient.left;

		m_btnRecord.MoveWindow(nLLeft,nTop,nBtnWid,nBtnHeit);
		if (1 == theApp.m_nPreviewModel)
		{
			m_btnRecord.EnableWindow(FALSE);
		}

		//全屏按钮
		nLLeft += nBtnWid;
		m_btnFullScreen.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//开始预览按钮
		nLLeft += nBtnWid;
		m_btnPreview.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//抓图
		nLLeft += nBtnWid;
		m_btnCapture.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//声音开关
		nLLeft += nBtnWid;
		m_btnSound.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//主子码流
		nLLeft += nBtnWid;
		m_btnStream.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		if (0 != theApp.m_nBtnRights)
		{
			//OSD信息显示
			nLLeft += nBtnWid;
			m_btnOSD.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);
		}

		//声音波形按钮
// 		nLLeft += nBtnWid;
// 		m_scpVideoWav.MoveWindow(nLLeft,nTop-2,rcClient.right-nLLeft-2,nBtnHeit);
	}
	else
	{
		//开始预览按钮
		int nTop = 2;
		int nLLeft = rcClient.left;
		m_btnPreview.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//声音开关
		nLLeft += nBtnWid;
		m_btnSound.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//主子码流
		nLLeft += nBtnWid;
		m_btnStream.MoveWindow(nLLeft,nTop-2,nBtnWid,nBtnHeit);

		//声音波形按钮
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

	//添加按钮提示
	SetButtonToolTip();

    CheckShowingBtns();
}
// 录像启停
void CPlayViewToolbar::OnBnClickedPlayviewtoolbarRecord()
{
	if (!m_bPreviewing)
	{
		MsgBoxApi_Show(_T("请先开启预览."), MSGBOX_TYPE_WARNING);
		return;
	}
	if (!m_bVideoUser)
	{
		MessageBox(m_strVideoMsg, _T("提示"), MB_ICONINFORMATION);
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
// 预览启停
void CPlayViewToolbar::OnBnClickedPlayviewtoolbarPreview()
{
	if (m_bRecording)
	{
		MsgBoxApi_Show(_T("录像中，禁止停止预览."), MSGBOX_TYPE_WARNING);
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
		MsgBoxApi_Show(_T("正在录像中，不支持码流切换."), MSGBOX_TYPE_WARNING);
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
    // 多画面全屏
    m_btnFullScreen.SetTooltipText(STR_PREVIEW_TOOLBAR_FULLSCREEN);
	// 录像
	m_btnRecord.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTRECORD);
	// 预览
	m_btnPreview.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTPREVIEW);
	m_btnCapture.SetTooltipText(_T("抓图"));
    m_btnSound.SetTooltipText(_T("开启音频"));
	m_btnStream.SetTooltipText(_T("主码流"));
	m_btnOSD.SetTooltipText(_T("显示水印"));
}

void CPlayViewToolbar::UpdateToolTips()
{
    // 多画面全屏
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
	if (bPreviewing)		// 预览中
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
	if (bRecording)		// 录像中
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
		// 开始录像
		m_btnRecord.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTRECORD);
	}
	else
	{
		// 停止录像
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
		// 开始预览
		m_btnPreview.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTPREVIEW);
	}
	else
	{
		// 停止预览
		m_btnPreview.SetTooltipText(STR_PREVIEW_TOOLBAR_STOPPREVIEW);
	}

	m_btnPreview.UpdateTooltip();
}

void CPlayViewToolbar::UpdateSoundTips(BOOL bSounding)
{
    if (bSounding == FALSE)
    {
        // 开始声音
        m_btnSound.SetTooltipText(STR_PREVIEW_TOOLBAR_STARTSOUND);
    }
    else
    {
        // 关闭声音
        m_btnSound.SetTooltipText(STR_PREVIEW_TOOLBAR_STOPSOUND);
    }

    m_btnSound.UpdateTooltip();
}

void CPlayViewToolbar::UpdateStreamTips(BOOL bStream)
{
	if (bStream == FALSE)
	{
		// 主码流
		m_btnStream.SetTooltipText(STR_POPUPMENU_SUB_STREAM);
	}
	else
	{
		// 子码流
		m_btnStream.SetTooltipText(STR_POPUPMENU_MAIN_STREAM);
	}

	m_btnStream.UpdateTooltip();
}

void CPlayViewToolbar::UpdateOsdTips(BOOL bOsd)
{
	if (bOsd == FALSE)
	{
		// 显示OSD
		m_btnOSD.SetTooltipText(STR_POPUPMENU_SHOWOSD);
	}
	else
	{
		// 隐藏OSD
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