#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "PlayMiniToolBarWnd.h"


// CRealWndToolBar �Ի���

IMPLEMENT_DYNAMIC(CPlayMiniToolBarWnd, CWnd)

CPlayMiniToolBarWnd::CPlayMiniToolBarWnd(CWnd* pParent /*=NULL*/)
: m_bReverse(FALSE)
, m_bPause(FALSE)
, m_bOpenSound(FALSE)
, m_bIsSingle(FALSE)
, m_bSave(FALSE)
{
   
}

CPlayMiniToolBarWnd::~CPlayMiniToolBarWnd()
{
   
}



BEGIN_MESSAGE_MAP(CPlayMiniToolBarWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(TOOL_PLAY_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkPlay)
	ON_BN_CLICKED(TOOL_STOP_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkStop)
	ON_BN_CLICKED(TOOL_MINI_SLOW_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkSlow)
	ON_BN_CLICKED(TOOL_MINI_FAST_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkFast)
	ON_BN_CLICKED(TOOL_FRAME_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkFrame)
    ON_BN_CLICKED(TOOL_CAPTURE_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkCapture)
    ON_BN_CLICKED(TOOL_SAVEDATA_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkSave)
    ON_BN_CLICKED(TOOL_SOUND_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkSound)
	ON_BN_CLICKED(TOOL_DOWNLOAD_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkDownload)
	ON_BN_CLICKED(TOOL_EXPAND_ID, &CPlayMiniToolBarWnd::OnToolBarBtnClkExpand)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_MESSAGE(MSG_MOUSEEVENT_MINITOOLBAR_LEAVE, &CPlayMiniToolBarWnd::OnMsgMouseLeaveMiniToolbar)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRealWndToolBar ��Ϣ�������

void CPlayMiniToolBarWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
}

void CPlayMiniToolBarWnd::PreSubclassWindow()
{
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
	//	GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);

	//HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	//if(hInst) 
	//{            
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);          
	//	MYFUNC fun = NULL;
	//	//ȡ��SetLayeredWindowAttributes����ָ��     
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	//	if(fun)fun(this->GetSafeHwnd(),0,128,2);     
	//	FreeLibrary(hInst); 
	//}

	CWnd::PreSubclassWindow();
}

int CPlayMiniToolBarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateChildWnd();
	InitCtrl();
	InitPos();

	return 0;
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkPlay()
{
	OnBtnClick(TOOL_PLAY_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkStop()
{
	OnBtnClick(TOOL_STOP_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkSlow()
{
	OnBtnClick(TOOL_MINI_SLOW_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkFast()
{
	OnBtnClick(TOOL_MINI_FAST_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkFrame()
{
	OnBtnClick(TOOL_FRAME_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkCapture()
{
    OnBtnClick(TOOL_CAPTURE_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkSave()
{
    OnBtnClick(TOOL_SAVEDATA_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkSound()
{
    OnBtnClick(TOOL_SOUND_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkDownload()
{
	OnBtnClick(TOOL_DOWNLOAD_ID);
}

void CPlayMiniToolBarWnd::OnToolBarBtnClkExpand()
{
	OnBtnClick(TOOL_EXPAND_ID);
}

void CPlayMiniToolBarWnd::CreateChildWnd()
{
	CRect rcBtn(0, 0, 36, 22);
	CRect rcSmallBtn(0, 0, 20, 28);
	m_btnPlay.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcBtn, this, TOOL_PLAY_ID);

	m_btnStop.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcBtn, this, TOOL_STOP_ID);

	m_btnSlow.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcBtn, this, TOOL_MINI_SLOW_ID);

	m_btnFast.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcBtn, this, TOOL_MINI_FAST_ID);

	m_btnFrame.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcBtn, this, TOOL_FRAME_ID);

	m_btnDownload.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcSmallBtn, this, TOOL_DOWNLOAD_ID);

	if (theApp.m_stRights.bCapture)
	{
		m_btnCapture.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
			rcSmallBtn, this, TOOL_CAPTURE_ID);
	}

	if (theApp.m_stRights.bMontage)
	{
		m_btnSave.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
			rcSmallBtn, this, TOOL_SAVEDATA_ID);
	}

	m_btnSound.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcSmallBtn, this, TOOL_SOUND_ID);

	m_btnExpand.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcSmallBtn, this, TOOL_EXPAND_ID);

}

void CPlayMiniToolBarWnd::InitCtrl()
{
	m_hBkImage.Attach(RESOURCELOAD_BMP(IDB_BITMAP_MINI_TOOLBAR_BAKE));
	m_dcBuffer.CreateCompatibleDC(NULL);
	m_dcBuffer.SelectObject(&m_hBkImage);

	m_btnPlay.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNPAUSE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_PAUSE_HOVER));

	m_btnStop.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNSTOP),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_STOP_HOVER));

	m_btnSlow.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNSLOW),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_SLOW_HOVER));

	m_btnFast.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFAST),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_FAST_HOVER));

	m_btnPlay.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNPLAY),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_PLAY_HOVER));

	if (m_bReverse)
	{
		m_btnFrame.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFRAME_DIS),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE));
		m_btnFrame.EnableWindow(FALSE);
	}
	else
	{
		m_btnFrame.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFRAME),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_FRAME_HOVER));
		m_btnFrame.EnableWindow(TRUE);
	}
	

	m_btnDownload.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_DOWNLOAD),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_DOWNLOAD_HOVER));

	m_btnCapture.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CAPTURE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CAP_HOVER));

	m_btnSave.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE_HOVER));

	m_btnSound.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN_HOVER));

	m_btnExpand.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_EXPAND),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE));

    SetToolBarBtnTips();
}

void CPlayMiniToolBarWnd::InitPos()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcBtn(rcClient);
	rcBtn.left += 4;
	rcBtn.right = rcBtn.left + 20;
	m_btnPlay.MoveWindow(rcBtn);

	rcBtn.OffsetRect(24, 0);
	if (rcBtn.right <= rcClient.right)
	{
		m_btnStop.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnStop.ShowWindow(SW_HIDE);
	}
	m_btnStop.MoveWindow(rcBtn);

	rcBtn.OffsetRect(24, 0);
	if (rcBtn.right <= rcClient.right)
	{
		m_btnSlow.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnSlow.ShowWindow(SW_HIDE);
	}
	m_btnSlow.MoveWindow(rcBtn);

	rcBtn.OffsetRect(24, 0);
	if (rcBtn.right <= rcClient.right)
	{
		m_btnFast.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnFast.ShowWindow(SW_HIDE);
	}
	m_btnFast.MoveWindow(rcBtn);

	rcBtn.OffsetRect(24, 0);
	if (rcBtn.right <= rcClient.right)
	{
		m_btnFrame.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnFrame.ShowWindow(SW_HIDE);
	}
	m_btnFrame.MoveWindow(rcBtn);


	// �Ҳఴť����.
	int nSmallBtnNum = 5;
	// �Ҳఴť���.
	int nSmallBtnWidth = 20;
	// ��ť���.
	int nSpace = 4;

	CRect rcBtnSamll;
	rcBtnSamll.CopyRect(rcClient);
	rcBtnSamll.right -= nSpace;
	rcBtnSamll.left = rcBtnSamll.right - nSmallBtnWidth;

	// ����������ұ������޷��������а�ť.
	if ((nSmallBtnNum * (nSmallBtnWidth + nSpace) ) > (rcClient.right - rcBtn.right - (5 * nSpace)))
	{
		m_btnDownload.ShowWindow(SW_HIDE);
		m_btnCapture.ShowWindow(SW_HIDE);
		m_btnSave.ShowWindow(SW_HIDE);
		m_btnSound.ShowWindow(SW_HIDE);
		m_btnExpand.ShowWindow(SW_SHOW);
		
		m_btnExpand.MoveWindow(rcBtnSamll);
	}
	else
	{
		if (theApp.HasDownloadModule())
		{
			m_btnDownload.ShowWindow(SW_SHOW);
		}
		
		m_btnCapture.ShowWindow(SW_SHOW);
		m_btnSave.ShowWindow(SW_SHOW);
		m_btnSound.ShowWindow(SW_SHOW);
		m_btnExpand.ShowWindow(SW_HIDE);
		m_btnSound.MoveWindow(rcBtnSamll);

		if (theApp.m_stRights.bMontage)
		{
			rcBtnSamll.OffsetRect(-24, 0);
			m_btnSave.MoveWindow(rcBtnSamll);
		}

		if (theApp.m_stRights.bCapture)
		{
			rcBtnSamll.OffsetRect(-24, 0);
			m_btnCapture.MoveWindow(rcBtnSamll);
		}

		rcBtnSamll.OffsetRect(-24, 0);
		m_btnDownload.MoveWindow(rcBtnSamll);
	}
}

void CPlayMiniToolBarWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	InitPos();
}

void CPlayMiniToolBarWnd::OnBtnClick(UINT nID)
{
    ::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_BTN_CLICK, nID, NULL);
}

void CPlayMiniToolBarWnd::SetToolBarBtnTips()
{
	m_btnPlay.SetTooltipText(_T("��ͣ"));
	m_btnStop.SetTooltipText(_T("ֹͣ"));
	m_btnSlow.SetTooltipText(_T("����"));
	m_btnFast.SetTooltipText(_T("����"));
	m_btnFrame.SetTooltipText(_T("��֡����"));
	m_btnDownload.SetTooltipText(_T("����¼��"));
    m_btnCapture.SetTooltipText(_T("ץͼ"));
	m_btnSave.SetTooltipText(_T("����"));
	m_btnSound.SetTooltipText(_T("������"));
	m_btnExpand.SetTooltipText(_T("��ʾ����"));
}

/** @fn         DrawBkImage
 *  @brief      ���Ʊ���ͼƬ����OnPaint����
 *  @return     void
 */
void CPlayMiniToolBarWnd::DrawBkImage()
{
    CDC *pDC = GetDC();
    if (NULL == pDC)
    {
        return;
    }

    CRect rcWnd;
    GetClientRect(&rcWnd);

    //˫�����ͼ
    //Create������dcҪ��DeleteDC���ͷ�,Get����dcҪ��ReleaseDC�ͷ�

    //1������һ���ڴ����dc(CreateCompatibleDC)
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    //2������һ���ڴ����bitmap(CreateCompatibleBitmap)
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(pDC, rcWnd.Width(), rcWnd.Height());
    //3�������������ڴ����dc��bitmap(SelectObject)
    CBitmap *pOldBitmap = memDC.SelectObject(&memBitmap);
    //�ñ���ɫ��λͼ����ɾ�
    memDC.FillSolidRect(rcWnd, RGB(0, 0, 0));
    //4�����Ʊ���ͼƬ
    BITMAP btBkg;
    ZeroMemory(&btBkg, sizeof(BITMAP));
    m_hBkImage.GetBitmap(&btBkg);

    //memDC.BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), &m_dcBuffer, 0, 0, SRCCOPY);
    memDC.StretchBlt(0,0,rcWnd.Width(),rcWnd.Height(),&m_dcBuffer,0,0,btBkg.bmWidth,btBkg.bmHeight,SRCCOPY);

    //5�����ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
    //pDC->BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), &memDC, 0, 0, SRCCOPY);
    pDC->StretchBlt(0,0,rcWnd.Width(),rcWnd.Height(),&memDC,0,0,btBkg.bmWidth,btBkg.bmHeight,SRCCOPY);
    //6������
    memDC.SelectObject(pOldBitmap);
    memDC.DeleteDC();
    memBitmap.DeleteObject();
    ReleaseDC(pDC);

	m_btnPlay.Invalidate(TRUE);
	m_btnStop.Invalidate(TRUE);
}

BOOL CPlayMiniToolBarWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ���Ʊ���ͼƬ
	DrawBkImage();
	return TRUE;

	//return CWnd::OnEraseBkgnd(pDC);
}

void CPlayMiniToolBarWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TRACKMOUSEEVENT       tme;       
	tme.cbSize=sizeof(tme);       
	tme.dwFlags=TME_LEAVE;             
	tme.hwndTrack=m_hWnd;       
	if(!_TrackMouseEvent(&tme))       
	{
		
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CPlayMiniToolBarWnd::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MOUSEEVENT_MINITOOLBAR_LEAVE, 0, 0);
	TRACE("@z onleave");
	CWnd::OnMouseLeave();
}

/**   @fn          OnMsgMouseLeaveMiniToolbar
 *    @brief   	   ����뿪��������Ϣ.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
HRESULT CPlayMiniToolBarWnd::OnMsgMouseLeaveMiniToolbar(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MOUSEEVENT_MINITOOLBAR_LEAVE, 0, 0);
	return 0;
}

void CPlayMiniToolBarWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	try
	{
		m_dcBuffer.DeleteDC();
		m_hBkImage.DeleteObject();
	}
	catch (...)
	{
	}
}


/**   @fn          GetSoundBtnRect
 *    @brief   	   ��ȡ��Ƶ��ť�Ĵ���λ��.
 *    @param[out]  rcSound:����λ��. 
 *    @param[in]   
 *    @return      
 */
void CPlayMiniToolBarWnd::GetSoundBtnRect(CRect& rcSound)
{
	m_btnSound.GetWindowRect(rcSound);
}

/**   @fn          UpdateBtnState
 *    @brief   	   ���°�ť״̬.
 *    @param[in]   bPause:�Ƿ�����ͣ״̬.
 *    @param[in]   bOpenSound:�Ƿ������Ƶ.
 *    @param[in]   bSave:�Ƿ��Ǳ�������״̬.
 *    @param[in]   bIsSingleFrame:�Ƿ��ǵ�֡����.
 *    @return      
 */
void CPlayMiniToolBarWnd::UpdateBtnState(BOOL bPause, BOOL bOpenSound, BOOL bSave, BOOL bIsSingleFrame, BOOL bSingleSound)
{
	// ���š���ͣ.
	m_bPause = bPause;
	m_bOpenSound = bOpenSound;
	m_bSave = bSave;
	m_bIsSingle = bIsSingleFrame;

	if (bPause  ||  bIsSingleFrame)
	{
		m_btnPlay.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNPLAY),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_PLAY_HOVER));
		m_btnPlay.SetTooltipText(_T("����"));

		m_btnFast.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFAST_DIS),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE));
		m_btnSlow.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNSLOW_DIS),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE));
		m_btnFast.EnableWindow(FALSE);
		m_btnSlow.EnableWindow(FALSE);
	}
	else
	{
		m_btnPlay.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNPAUSE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_PAUSE_HOVER));
		m_btnPlay.SetTooltipText(_T("��ͣ"));

		m_btnSlow.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNSLOW),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_SLOW_HOVER));

		m_btnFast.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFAST),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_FAST_HOVER));
		m_btnFast.EnableWindow(TRUE);
		m_btnSlow.EnableWindow(TRUE);
	}

	if (bPause  ||  m_bReverse)
	{
		m_btnFrame.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFRAME_DIS),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE));
		m_btnFrame.EnableWindow(FALSE);
	}
	else
	{
		m_btnFrame.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFRAME),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_FRAME_HOVER));
		m_btnFrame.EnableWindow(TRUE);
	}

	// ��������. ͼ����ʾ��һ���Ĳ��������ǵ�ǰ����״̬
	if (bOpenSound)
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_VOLUMN), 
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSEVOLUMN_HOVER));
		m_btnSound.SetTooltipText(_T("�ر�����"));
	}
	// û�д���������.
	else
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN_HOVER));
		m_btnSound.SetTooltipText(_T("������"));
	}

	// ���ڼ�����.
	if (bSave)
	{
		m_btnSave.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE_HOVER),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE));
		m_btnSave.SetTooltipText(_T("ֹͣ����"));
	}
	// û�м���.
	else
	{
		m_btnSave.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE_HOVER));
		m_btnSave.SetTooltipText(_T("����"));
	}
	if (bSingleSound)
	{
		m_btnFast.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFAST_DIS),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFAST_DIS));

		m_btnSlow.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNSLOW_DIS),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNSLOW_DIS));


		m_btnFrame.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFRAME_DIS),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_TOOLBAR_BAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINI_BTNFRAME_DIS));
		
		m_btnFast.EnableWindow(FALSE);
		m_btnSlow.EnableWindow(FALSE);
		m_btnFrame.EnableWindow(FALSE);
		m_btnCapture.EableClickButton(FALSE);
	}
	else
	{
		m_btnCapture.EableClickButton(TRUE);
	}
}

/**   @fn          UpdateButtonTips
 *    @brief   	   ���°�ťtips.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayMiniToolBarWnd::UpdateButtonTips()
{
	m_btnPlay.UpdateTooltip();
	m_btnStop.UpdateTooltip();
	m_btnSlow.UpdateTooltip();
	m_btnFast.UpdateTooltip();
	m_btnFrame.UpdateTooltip();
	m_btnDownload.UpdateTooltip();
	m_btnSound.UpdateTooltip();
	m_btnSave.UpdateTooltip();
	m_btnCapture.UpdateTooltip();
	m_btnExpand.UpdateTooltip();
}

/**   @fn          SetReverseState
 *    @brief   	   ���õ���״̬.
 *    @param[in]   bReverse:�Ƿ��ǵ���״̬.
 *    @param[in]   
 *    @return      
 */
void CPlayMiniToolBarWnd::SetReverseState(BOOL bReverse)
{
	m_bReverse = bReverse;
	UpdateBtnState(m_bPause, m_bOpenSound, m_bSave, m_bIsSingle);
}