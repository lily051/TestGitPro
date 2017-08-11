//lint -library
#include "stdafx.h"
#include "../ui/uiCtrl/memdcex.h"
#include "VolumnSetWnd.h"


CVolumnSetWnd::CVolumnSetWnd()
{
	m_bMove = FALSE;

	m_pParentWnd = NULL;

	m_pSliderVolumn = NULL;
	m_bVoice = FALSE;

	m_uiCloseTimer = 0;
	m_uiIdleTime = 0;

	m_nBitmapWidth = 0;
	m_nBitmapHeight = 0;
	m_hBitmap = NULL;

	m_bkBrush.CreateSolidBrush(UI_WND_BKGND);

}

CVolumnSetWnd::~CVolumnSetWnd()
{
	m_pParentWnd = NULL;

	m_pSliderVolumn = NULL;
	m_hBitmap = NULL;

	try
	{
		m_bkBrush.DeleteObject();
	}
	catch (...)
	{

	}

}


BEGIN_MESSAGE_MAP(CVolumnSetWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BITMAPSLIDER_MOVED, OnMsgSliderClick)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

int CVolumnSetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	InitCtrl();

	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	m_bMove = TRUE;

	return 0;
}

void CVolumnSetWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	m_uiIdleTime = 0;

	CWnd::OnMouseMove(nFlags, point);
}

void CVolumnSetWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		//当窗口显示时，设置窗口焦点
		SetFocus();
		//设置窗口自动关闭时钟
		SetCloseTimer();
	}
	else
	{
		//取消窗口自动关闭时钟
		KillColseTimer();
		AnimateWindow(200, AW_HIDE|AW_BLEND);
		::PostMessage(m_pParentWnd->GetSafeHwnd(), MSG_FLOATWND_CLOST, NULL, NULL);
	}
}

void CVolumnSetWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}
}

void CVolumnSetWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == IDT_VOLUMNWNDCLOSE_TIMER)
	{
		m_uiIdleTime += 1;

		if(m_uiIdleTime >= FLOATWND_CLOSE_TIME)
		{
			POINT pt;
			GetCursorPos(&pt);

			CRect rectWindow(0,0,0,0);

			GetWindowRect(&rectWindow);

			if(rectWindow.PtInRect(pt))
			{
				m_uiIdleTime = 0;
			}
			else
			{
				ShowWindow(SW_HIDE);
				m_uiIdleTime = 0;
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}

LRESULT CVolumnSetWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if((message == WM_ACTIVATE)&&(wParam == WA_INACTIVE))
	{
		ShowWindow(SW_HIDE);
	}

	return CWnd::DefWindowProc(message, wParam, lParam);
}

void CVolumnSetWnd::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CWnd::PostNcDestroy();
}

//初始化控件
void CVolumnSetWnd::InitCtrl()
{
	m_DrawBakeFun.DrawStrip_Loadbmp(
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_TOOLBARBAKE,Color(0,0,0,255)),
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_TOOLBARBAKE,Color(0,0,0,255)),
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_TOOLBARBAKE,Color(0,0,0,255)));

	if (m_pSliderVolumn == NULL)
	{
		m_pSliderVolumn = new CBitmapSlider;
		m_pSliderVolumn->Create(NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP|SS_NOTIFY|SS_BITMAP, CRect(0,0,0,0), this, TOOL_SLIDER_VOLUMN);
		m_pSliderVolumn->LoadPngChannel_EX(100, 0, IDB_PNG_SCROLLBAR, IDB_PNG_SCROLLBARON, FALSE);
		m_pSliderVolumn->LoadPngThumb(IDB_PNG_SCROLLTHUMB, IDB_PNG_SCROLLTHUMBSEL, TRUE);
		m_pSliderVolumn->SetRange( 0, 100 );
		m_pSliderVolumn->SetPos( 0 );
		m_pSliderVolumn->SetMarginLeft( 1 );
		m_pSliderVolumn->SetMarginRight( 1 );
		m_pSliderVolumn->SetPageSize( 5 );
		m_pSliderVolumn->Enable(TRUE);
	}

	LOGFONT lf = {0};
	lf.lfHeight = 100;
	lf.lfWeight = FW_MEDIUM;
	::lstrcpy(lf.lfFaceName, _T("宋体"));

	m_font.CreatePointFontIndirect(&lf);

	Bitmap * pBitmap = ::LoadBitmapFromResource(IDR_PNG_TOOLBARBAKE);
	pBitmap->GetHBITMAP(Color(0, 255, 0, 255), &m_hBitmap);
	m_nBitmapWidth = pBitmap->GetWidth();
	m_nBitmapHeight = pBitmap->GetHeight();

}

//移动控件
void CVolumnSetWnd::MoveCtrl()
{
	CRect rcWindow;
	GetClientRect(&rcWindow);

	int nSliderHeight = 11;
	int nSliderWidth = 100;
	int nTop = (rcWindow.Height() - nSliderHeight) / 2;
	int nSpace = 4;

	if (m_pSliderVolumn)
	{
		m_pSliderVolumn->MoveWindow(rcWindow.left + nSpace, nTop, nSliderWidth, nSliderHeight);
	}
}

//开启定时器
BOOL CVolumnSetWnd::SetCloseTimer(void)
{
	if(m_uiCloseTimer != 0)
	{
		KillColseTimer();
	}

	m_uiIdleTime = 0;
	m_uiCloseTimer = SetTimer(IDT_VOLUMNWNDCLOSE_TIMER, 1000, NULL);

	return true;
}

//停止时钟
void CVolumnSetWnd::KillColseTimer(void)
{
	if(m_uiCloseTimer != 0)
	{
		KillTimer(IDT_VOLUMNWNDCLOSE_TIMER);
		m_uiCloseTimer = 0;
	}

	m_uiIdleTime = 0;
}

//创建
BOOL CVolumnSetWnd::CreateFloatWindow(CWnd *pParentWnd)
{
	m_pParentWnd = pParentWnd;

	BOOL bRet =  CreateEx(WS_EX_TOPMOST,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW),(HBRUSH)GetStockObject(BLACK_BRUSH)),
		NULL, WS_POPUP|WS_VISIBLE, CRect(0,0,0,0), 0, 0, NULL);

	return bRet;
}

void CVolumnSetWnd::OnDestroy()
{
	CWnd::OnDestroy();

	if (m_pSliderVolumn)
	{
		m_pSliderVolumn->DestroyWindow();
		delete m_pSliderVolumn;
		m_pSliderVolumn = NULL;
	}

}

//进度条点击
LRESULT CVolumnSetWnd::OnMsgSliderClick(WPARAM wParam, LPARAM lParam)
{
	if (TOOL_SLIDER_VOLUMN == wParam)
	{
		if (m_pSliderVolumn)
		{
			int nPos = m_pSliderVolumn->GetPos();
			if (m_pParentWnd)
			{
				::PostMessage(m_pParentWnd->GetSafeHwnd(), MSG_TOOLBAR_BTN_CLICK, TOOL_SLIDER_VOLUMN, nPos);
			}
		}
	}

	return 0;
}

//设置初始化进度
void CVolumnSetWnd::SetInitPos(int nVolumn)
{
	if (m_pSliderVolumn)
	{
		m_pSliderVolumn->SetPos(nVolumn);
	}

}

void CVolumnSetWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC * pDC(&dc);
	CMemDCEX memDC(pDC,rcClient);
	m_DrawBakeFun.DrawStrip_DrawFun(&memDC,&rcClient);

	// 不为绘图消息调用 CWnd::OnPaint()
}

BOOL CVolumnSetWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);

	CMemDCEX memDC(pDC,rectClient);
	m_DrawBakeFun.DrawStrip_DrawFun(&memDC,&rectClient);

	if (m_dcMem.m_hDC == NULL)
	{
		m_dcMem.CreateCompatibleDC(pDC);
		m_dcMem.SelectObject(m_hBitmap);
	}

	return TRUE;

	//return CWnd::OnEraseBkgnd(pDC);
}

HBRUSH CVolumnSetWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (pWnd->GetDlgCtrlID() == TOOL_CHECKBOX_VOICE)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//透明绘图
void CVolumnSetWnd::TransparentBk(CDC * pDc, CWnd * pWnd)
{
	CRect rcWnd;
	pWnd->GetClientRect(&rcWnd);
	pDc->SetStretchBltMode(HALFTONE);
	pDc->SetBrushOrg(0,0);
	pDc->StretchBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &m_dcMem, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);

}
