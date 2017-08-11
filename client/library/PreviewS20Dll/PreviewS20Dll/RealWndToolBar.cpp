//lint -library
#include "stdafx.h"
#include "RealWndToolBar.h"
#include "RealPlayViewWnd.h"
#include "PreviewS20Dll.h"


// CRealWndToolBar ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(CRealWndToolBar, CWnd)

CRealWndToolBar::CRealWndToolBar(CWnd* pParent /*=NULL*/)
{

}

CRealWndToolBar::~CRealWndToolBar()
{
}



BEGIN_MESSAGE_MAP(CRealWndToolBar, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
    ON_BN_CLICKED(BTN_ID_CAPTURE, CRealWndToolBar::OnToolBarBtnClk_Capture)
    ON_BN_CLICKED(BTN_ID_RECORD, CRealWndToolBar::OnToolBarBtnClk_Record)
    ON_BN_CLICKED(BTN_ID_SOUND, CRealWndToolBar::OnToolBarBtnClk_Sound)
    ON_BN_CLICKED(BTN_ID_STREAM, CRealWndToolBar::OnToolBarBtnClk_Stream)
	ON_BN_CLICKED(BTN_ID_OSD, CRealWndToolBar::OnToolBarBtnClk_OSD)
    ON_BN_CLICKED(BTN_ID_STOP, CRealWndToolBar::OnToolBarBtnClk_Stop)
END_MESSAGE_MAP()


// CRealWndToolBar ÏûÏ¢´¦Àí³ÌÐò

void CRealWndToolBar::OnPaint()
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

void CRealWndToolBar::PreSubclassWindow()
{
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);

	HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	if(hInst) 
	{            
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);          
		MYFUNC fun = NULL;
		//È¡µÃSetLayeredWindowAttributesº¯ÊýÖ¸Õë     
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),0,128,2);     
		FreeLibrary(hInst); 
	}

	CWnd::PreSubclassWindow();
}

int CRealWndToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateChildWnd();
	InitCtrl();
	InitPos();

	return 0;
}

//×¥Í¼
void CRealWndToolBar::OnToolBarBtnClk_Capture()
{
    OnBtnClick(BTN_ID_CAPTURE);
}

//Â¼Ïñ
void CRealWndToolBar::OnToolBarBtnClk_Record()
{
    OnBtnClick(BTN_ID_RECORD);
}

//ÒôÆµ
void CRealWndToolBar::OnToolBarBtnClk_Sound()
{
    OnBtnClick(BTN_ID_SOUND);
}

//ÇåÎú¶È
void CRealWndToolBar::OnToolBarBtnClk_Stream()
{
    OnBtnClick(BTN_ID_STREAM);
}

//OSDÐÅÏ¢
void CRealWndToolBar::OnToolBarBtnClk_OSD()
{
	OnBtnClick(BTN_ID_OSD);
}

//Í£Ö¹Ô¤ÀÀ
void CRealWndToolBar::OnToolBarBtnClk_Stop()
{
    OnBtnClick(BTN_ID_STOP);
}

void CRealWndToolBar::CreateChildWnd()
{
	if (0 == theApp.m_nPreviewModel)
	{
		m_btnCapture.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(0,0,0,0), this, BTN_ID_CAPTURE);

		m_btnRecord.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(0,0,0,0), this, BTN_ID_RECORD);

		m_btnSound.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(0,0,0,0), this, BTN_ID_SOUND);

		m_btnStreamType.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(0,0,0,0), this, BTN_ID_STREAM);

		m_btnOsdInfo.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(0,0,0,0), this, BTN_ID_OSD);

		m_btnStop.Create(_T(""),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			CRect(0,0,0,0), this,BTN_ID_STOP);
	}
	else
	{	
		m_btnSound.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(0,0,0,0), this, BTN_ID_SOUND);
		m_btnStop.Create(_T(""),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			CRect(0,0,0,0), this,BTN_ID_STOP);
	}
}

void CRealWndToolBar::InitCtrl()
{
	if (0 == theApp.m_nPreviewModel)
	{
		//×¥Í¼
		m_btnCapture.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_CAPTURE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_TOOL_GRAB_HOVER));

		//Â¼Ïñ
		m_btnRecord.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_RECORD),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_TOOL_RECORD_HOVER));

		//ÉùÒô
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_TOOL_SOUND_HOVER));

		//ÇåÎú¶ÈÇÐ»»
		m_btnStreamType.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_MAINSTREAM),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_MAINSTREAM_HOVER));

		//OSDÐÅÏ¢
		m_btnOsdInfo.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_TOOLBAR_OSD),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_TOOLBAR_OSD_HOVER));

		//Í£Ö¹Ô¤ÀÀ
		m_btnStop.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_STOP),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
	}
	else
	{
		//Í£Ö¹Ô¤ÀÀ
		m_btnStop.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_STOP),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE));
		//ÉùÒô
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_RW_TOOL_SOUND_HOVER));
	}

    CheckShowing4Btns();

    SetToolBarBtnTips();
}

void CRealWndToolBar::InitPos()
{
	CRect rcClient;
	GetClientRect(rcClient);

    BOOL bFirst = FALSE;
	CRect rcBtn(rcClient);
	rcBtn.right -= 20;
	rcBtn.left = rcBtn.right - 20;
	if (0 == theApp.m_nPreviewModel)
	{
		//Í£Ö¹Ô¤ÀÀ
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STOP)
		{
			m_btnStop.MoveWindow(rcBtn);
			bFirst = TRUE;
		}

		//OSDÐÅÏ¢
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_OSDINFO)
		{
			if (bFirst)
			{
				rcBtn.OffsetRect(-35, 0);
			}
			else
			{
				bFirst = TRUE;
			}
			m_btnOsdInfo.MoveWindow(rcBtn);
		}

		//ÇåÎú¶ÈÇÐ»»
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STREAM_TYPE)
		{
			if (bFirst)
			{
				rcBtn.OffsetRect(-35, 0);
			}
			else
			{
				bFirst = TRUE;
			}
			m_btnStreamType.MoveWindow(rcBtn);
		}

		//ÒôÆµ
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_AUDIO)
		{
			if (bFirst)
			{
				rcBtn.OffsetRect(-35, 0);
			}
			else
			{
				bFirst = TRUE;
			}
			m_btnSound.MoveWindow(rcBtn);
		}

		//Â¼Ïñ
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_RECORD)
		{
			if (bFirst)
			{
				rcBtn.OffsetRect(-35, 0);
			}
			else
			{
				bFirst = TRUE;
			}
			m_btnRecord.MoveWindow(rcBtn);
		}

		//×¥Í¼
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_CAPTURE)
		{
			if (bFirst)
			{
				rcBtn.OffsetRect(-35, 0);
			}
			else
			{
				bFirst = TRUE;
			}
			m_btnCapture.MoveWindow(rcBtn);
		}
	}
	else
	{
		//Í£Ö¹Ô¤ÀÀ
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STOP)
		{
			m_btnStop.MoveWindow(rcBtn);
			bFirst = TRUE;
		}
		//ÒôÆµ
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_AUDIO)
		{
			if (bFirst)
			{
				rcBtn.OffsetRect(-35, 0);
			}
			else
			{
				bFirst = TRUE;
			}
			m_btnSound.MoveWindow(rcBtn);
		}
	}
}

void CRealWndToolBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	InitPos();
}

void CRealWndToolBar::OnBtnClick(UINT nID)
{
    ::PostMessage(GetParent()->GetSafeHwnd(), WM_PV_REALWND_TOOLBAR_CLK, nID, NULL);
}

void CRealWndToolBar::SetToolBarBtnTips()
{
    int nToolIndex = 0;
	// ×¥Í¼
	m_btnCapture.SetTooltipText(_T("×¥Í¼"));
    // Â¼Ïñ
    m_btnRecord.SetTooltipText(_T("Â¼Ïñ"));
    // ÒôÆµ
    m_btnSound.SetTooltipText(_T("ÒôÆµ"));
    // ÇåÎú¶ÈÇÐ»»
    m_btnStreamType.SetTooltipText(_T("ÇåÎú¶È"));
	// OSDÐÅÏ¢
	m_btnOsdInfo.SetTooltipText(_T("OSDÐÅÏ¢"));
    // Í£Ö¹Ô¤ÀÀ
    m_btnStop.SetTooltipText(_T("Í£Ö¹Ô¤ÀÀ"));
}

void CRealWndToolBar::CheckShowing4Btns()
{
	// ×¥Í¼
	if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_CAPTURE)
	{
		m_btnCapture.ShowWindow(SW_SHOW);
	} 
	else
	{
		m_btnCapture.ShowWindow(SW_HIDE);
	}
    // Â¼Ïñ
    if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_RECORD)
    {
        m_btnRecord.ShowWindow(SW_SHOW);
    } 
    else
    {
        m_btnRecord.ShowWindow(SW_HIDE);
    }
	// ÒôÆµ
	if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_AUDIO)
	{
		m_btnSound.ShowWindow(SW_SHOW);
	} 
	else
	{
		m_btnSound.ShowWindow(SW_HIDE);
	}
	// ÇåÎú¶È
	if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STREAM_TYPE)
	{
		m_btnStreamType.ShowWindow(SW_SHOW);
	} 
	else
	{
		m_btnStreamType.ShowWindow(SW_HIDE);
	}
	// OSDÐÅÏ¢
	if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STREAM_TYPE)
	{
		m_btnOsdInfo.ShowWindow(SW_SHOW);
	} 
	else
	{
		m_btnOsdInfo.ShowWindow(SW_HIDE);
	}
	// Í£Ö¹Ô¤ÀÀ
	if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STOP)
	{
		m_btnStop.ShowWindow(SW_SHOW);
	} 
	else
	{
		m_btnStop.ShowWindow(SW_HIDE);
	}
}
