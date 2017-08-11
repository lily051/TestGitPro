// ChildDlgOwerDraw.cpp : implementation file
//
//lint -library
#include "stdafx.h"
//#include "../CollectorClient.h"
#include "ChildDlgOwerDraw.h"
#include "memdcex.h"

// CChildDlgOwerDraw dialog

IMPLEMENT_DYNAMIC(CChildDlgOwerDraw, CDialog)
//CChildDlgOwerDraw::CChildDlgOwerDraw(CWnd* pParent /*=NULL*/)
//	: CDialog(CChildDlgOwerDraw::IDD, pParent)
//{
//	m_pTitle = NULL;
//	m_pMinImage = NULL;
//	m_pRestoreImage = NULL;
//	m_pBakeImage = NULL;
//	m_pExitImage = NULL;
//	m_pExitImageSel = NULL;
//
//	m_crCaption.SetRect(0,0,0,0);
//	m_crMinBtn.SetRect(0,0,0,0);
//	m_crMinBtn.SetRect(0,0,0,0);
//
//	m_bMinBtnDown = FALSE;
//	m_bExitBtnDown = FALSE;
//}
CChildDlgOwerDraw::CChildDlgOwerDraw(UINT nIDTemplate, CWnd* pParentWnd) : 
CDialog(nIDTemplate,pParentWnd)
{
	m_pTitle = NULL;
	m_pMinImage = NULL;
	m_pRestoreImage = NULL;
	m_pBakeImage = NULL;
	m_pExitImage = NULL;
	m_pExitImageSel = NULL;

	m_crMinBtn.SetRect(0,0,0,0);
	m_crMinBtn.SetRect(0,0,0,0);
	m_crCaption.SetRect(0,0,0,0);
	
	m_bMinBtnDown = FALSE;
    m_bExitBtnDown = FALSE;

	m_bMiniDlg = FALSE;

	m_bHoverExitBtn = FALSE;
	m_bHoverMiniBtn = FALSE;
	m_bDownExitBtn = FALSE;
	m_bDownMiniBtn = FALSE;

	m_bMouseTrack = FALSE;


	//解锁按钮
	m_pLockImage = NULL;
	m_pUnLockImage = NULL;
	m_bDownLockBtn = FALSE;
	m_bHoverLockBtn = FALSE;
	m_crLockBtn.SetRect(0,0,0,0);
	m_bLock = FALSE;
}

CChildDlgOwerDraw::~CChildDlgOwerDraw()
{

	__UnInitCtrl();
}

void CChildDlgOwerDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlgOwerDraw, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()


// CChildDlgOwerDraw message handlers


void CChildDlgOwerDraw::SetTitleBar_Image(Image * pMid)
{
	if(m_pTitle)
	{
		delete m_pTitle;
		m_pTitle = NULL;
	}

	m_pTitle = pMid;
}

void CChildDlgOwerDraw::SetTitleBar_MinBtnImage(Image * pMinImage , Image * pRestoreImage,Image * hBakeImage)
{
	if(m_pMinImage)
	{
		delete m_pMinImage;
		m_pMinImage = NULL;
	}

	if(m_pRestoreImage)
	{

		delete m_pRestoreImage;
		m_pRestoreImage = NULL;
	}

	if(m_pBakeImage)
	{

		delete m_pBakeImage;
		m_pBakeImage = NULL;
	}

	m_pMinImage = pMinImage;
	m_pRestoreImage = pRestoreImage;
	m_pBakeImage = hBakeImage;
}


void CChildDlgOwerDraw::SetTitleBar_ExitBtnImage(Image * pExitImage , Image * pExitImageSel)
{
	if(m_pExitImage)
	{

		delete m_pExitImage;
		m_pExitImage = NULL;
	}

	if(m_pExitImageSel)
	{

		delete m_pExitImageSel;
		m_pExitImageSel = NULL;
	}

	m_pExitImage = pExitImage;
	m_pExitImageSel = pExitImageSel;
}

void CChildDlgOwerDraw::SetTitleBar_LockImage(Image * pLockImage , Image * pUnLockImage)
{
	if(m_pLockImage)
	{

		delete m_pLockImage;
		m_pLockImage = NULL;
	}

	if(m_pUnLockImage)
	{

		delete m_pUnLockImage;
		m_pUnLockImage = NULL;
	}

	m_pLockImage = pLockImage;
	m_pUnLockImage = pUnLockImage;
}

void CChildDlgOwerDraw::__UnInitCtrl(void)
{
	if(m_pTitle)
	{

		delete m_pTitle;
		m_pTitle = NULL;
	}

	if(m_pMinImage)
	{

		delete m_pMinImage;
		m_pMinImage = NULL;
	}

	if(m_pRestoreImage)
	{

		delete m_pRestoreImage;
		m_pRestoreImage = NULL;
	}

	if(m_pExitImage)
	{

		delete m_pExitImage;
		m_pExitImage = NULL;
	}

	if(m_pExitImageSel)
	{
		delete m_pExitImageSel;
		m_pExitImageSel = NULL;
	}

	if(m_pBakeImage)
	{

		delete m_pBakeImage;
		m_pBakeImage = NULL;
	}
}
//void CChildDlgOwerDraw::OnPaint()
//{
//	CPaintDC dc(this);
//	_DrawDialog(&dc);
//}

BOOL CChildDlgOwerDraw::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	_DrawDialog(pDC);

	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CChildDlgOwerDraw::SetBakeColor(COLORREF crBakeColor)
{

	if(m_bakeBrush.GetSafeHandle())
	{

		m_bakeBrush.DeleteObject();
		m_bakeBrush.m_hObject = NULL;
	}

	m_bakeBrush.CreateSolidBrush(crBakeColor);
}

void CChildDlgOwerDraw::_DrawDialog(CDC * pDC)
{
	CRect crClient(0,0,0,0);
	GetClientRect(&crClient);

	CMemDCEX memDC(pDC,crClient);

	if(m_bakeBrush.GetSafeHandle())
	{

		memDC.FillRect(&crClient,&m_bakeBrush);
	}


	__DrawTitle(&memDC);

	__DrawFrame(&memDC);
}

/*************************************************
函数名称：DrawFrame()
描述:	绘制窗口的边框
输入: pDC 窗口设备句柄
输出:	无
返回: 无
*************************************************/
void  CChildDlgOwerDraw::__DrawFrame(CDC *pDC)
{
	//if (!IsWindowVisible() || IsIconic())  
	//{
	//	return;  
	//}

	CRect rcWindow;
	CRect rcClient;
	GetClientRect(&rcWindow);
	//GetClientRect(&rcClient);
	//rcWindow.OffsetRect(-rcWindow.left,-rcWindow.top);

	int iBound = 3;//(rcWindow.Width() - rcClient.Width()) / 2;
// 	if(iBound == 0)  //jin ta ma de cha dan 
// 	{
// 		return;
// 	}

	//CBrush brush(theApp.GetUIColor(UI_COLOR_TYPE_DEEP));
	//CBrush brushInling(theApp.GetUIColor(UI_COLOR_TYPE_STATISTIC));
    CBrush brush(RGB(167,195,217));
    CBrush brushInling(RGB(186,209,220));

	iBound = 1;

	//左边框
	CRect rcLeft(rcWindow.left,rcWindow.top,rcWindow.left + iBound,rcWindow.bottom);
	//底框
	CRect rcBottom(rcWindow.left,rcWindow.bottom - iBound,rcWindow.right,rcWindow.bottom);
	//右边框
	CRect rcRight(rcWindow.right - iBound,rcWindow.top,rcWindow.right,rcWindow.bottom);

	pDC->FillRect(&rcLeft,&brush);
	pDC->FillRect(&rcBottom,&brush);
	pDC->FillRect(&rcRight,&brush);
	pDC->FrameRect(&rcWindow,&brush);

	iBound = 3;
	rcLeft = CRect(rcWindow.left + 1,rcWindow.top,rcWindow.left + iBound,rcWindow.bottom - 1);
	rcBottom = CRect(rcWindow.left + 1,rcWindow.bottom - iBound,rcWindow.right - 1,rcWindow.bottom - 1);
	rcRight = CRect(rcWindow.right - iBound,rcWindow.top,rcWindow.right - 1,rcWindow.bottom - 1);
	pDC->FillRect(&rcLeft,&brushInling);
	pDC->FillRect(&rcBottom,&brushInling);
	pDC->FillRect(&rcRight,&brushInling);

	brush.DeleteObject();
	brushInling.DeleteObject();

}

void CChildDlgOwerDraw::__InitPose(void)
{
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);
	int iRight = rectClient.right;

	if(m_pExitImage)
	{
		m_crExitBtn.right = iRight - 4;
		m_crExitBtn.left = m_crExitBtn.right - m_pExitImage->GetWidth();
		m_crExitBtn.top = rectClient.top + 3;
		m_crExitBtn.bottom = m_crExitBtn.top + m_pExitImage->GetHeight();
		iRight -= m_crExitBtn.Width();
	}

	if(m_pBakeImage)
	{
		m_crMinBtn.right = iRight - 4;
		m_crMinBtn.left = m_crMinBtn.right - m_pBakeImage->GetWidth();
		m_crMinBtn.top = rectClient.top + 3;
		m_crMinBtn.bottom = m_crMinBtn.top + m_pBakeImage->GetHeight();
		iRight -= m_crMinBtn.Width();
	}
	
	if (m_pLockImage)
	{
		m_crLockBtn.right = iRight - 4;
		m_crLockBtn.left = m_crLockBtn.right - m_pBakeImage->GetWidth();
		m_crLockBtn.top = rectClient.top + 3;
		m_crLockBtn.bottom = m_crLockBtn.top + m_pBakeImage->GetHeight();
	}
	if(m_pTitle)
	{

		m_crCaption = rectClient;
		m_crCaption.bottom = m_crCaption.top + m_pTitle->GetHeight();
	}
}

void CChildDlgOwerDraw::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	__InitPose();

	// TODO: Add your message handler code here
}

void CChildDlgOwerDraw::SetMiniSize(BOOL bMini)
{
	m_bMiniDlg = bMini;

	m_bDownExitBtn = m_bDownMiniBtn = m_bHoverExitBtn = m_bHoverMiniBtn = FALSE;

	CDC * pDC = GetWindowDC();

	if(pDC)
	{
		__DrawTitle(pDC);

		ReleaseDC(pDC);
		pDC = NULL;
	}
}

void CChildDlgOwerDraw::Fun_SetLock(BOOL bLock)
{
	CDC * pDC = GetWindowDC();
	m_bLock = bLock;
	if (pDC)
	{
		__DrawTitle(pDC);

		ReleaseDC(pDC);
		pDC = NULL;
	}
}

void CChildDlgOwerDraw::__DrawTitle(CDC * pDC)
{

	CRect crClient(0,0,0,0);
	GetClientRect(&crClient);

	if(m_pTitle)
	{
		Graphics graph(pDC->GetSafeHdc());
		CRect rectBtn(0,0,0,0);

		int iLeftTemp = 0;
        
		graph.DrawImage(m_pTitle,crClient.left,crClient.top,crClient.Width()*2,m_pTitle->GetHeight());

		Image * pImage = NULL;
		Image * pBakeImage = NULL;

		if(m_pExitImage)
		{

			rectBtn = m_crExitBtn;
			rectBtn.top += 2;

			if((m_bDownExitBtn || m_bHoverExitBtn) && m_pExitImageSel)
			{
				pImage = m_pExitImageSel;

				if(m_bDownExitBtn)
				{

					rectBtn.OffsetRect(1,1);
				}
			}else
			{

				pImage = m_pExitImage;
			}

			if(pImage)
			{

				// 画背景图片
				graph.DrawImage(pImage, rectBtn.left, rectBtn.top, pImage->GetWidth(), pImage->GetHeight());
			}
		}

		//如果存在最小化按钮,贴上最小化按钮图片
		if(m_pMinImage)
		{
			rectBtn = m_crMinBtn;

			if(m_bMiniDlg == FALSE)
			{
				rectBtn.top -= 1;
				if((m_bDownMiniBtn || m_bHoverMiniBtn ) && m_pBakeImage)
				{
					pBakeImage = m_pBakeImage;

					if(m_bDownMiniBtn)
					{

						rectBtn.OffsetRect(1,1);
					}
				}else
				{
					pBakeImage = NULL;
				}

				pImage = m_pMinImage;
			}else
			{
				//rectBtn.OffsetRect(0,-1);
				rectBtn.top += 1;

				if((m_bDownMiniBtn || m_bHoverMiniBtn ) && m_pBakeImage)
				{
					pBakeImage = m_pBakeImage;

					if(m_bDownMiniBtn)
					{

						rectBtn.OffsetRect(1,1);
					}
				}else
				{

					pBakeImage = NULL;
				}

				pImage = m_pRestoreImage;
			}
			
			

			if(pBakeImage)
			{

				// 画背景图片
				graph.DrawImage(pBakeImage, rectBtn.left, rectBtn.top, pBakeImage->GetWidth(), pBakeImage->GetHeight());

			}
			if(pImage)
			{

				graph.DrawImage(pImage, rectBtn.left, rectBtn.top, pImage->GetWidth(), pImage->GetHeight());
			}

			
		}

		//解锁按钮
		if(m_pLockImage)
		{
			rectBtn = m_crLockBtn;

			//图片位置
			if (m_bDownLockBtn || m_bHoverLockBtn)
			{
				rectBtn.top -= 1;
				if (m_bDownLockBtn)
				{
					rectBtn.OffsetRect(1,1);
				}

			}

			//图片
			pImage = m_bLock?m_pLockImage:m_pUnLockImage;
			

			if(pBakeImage)
			{

				// 画背景图片
				graph.DrawImage(pBakeImage, rectBtn.left, rectBtn.top, pBakeImage->GetWidth(), pBakeImage->GetHeight());

			}
			if(pImage)
			{

				graph.DrawImage(pImage, rectBtn.left, rectBtn.top, pImage->GetWidth(), pImage->GetHeight());
			}
		}
	}

	CRect rectTitleText = crClient;
	rectTitleText.left += 10;
	rectTitleText.bottom = rectTitleText.top + 24;


	//绘制标题栏中的标题
	CString szCaption;
	GetWindowText(szCaption);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));

	CFont *pFont = GetFont();
	//CFont font;
	//LOGFONT lf = {0};
	//lf.lfHeight = 92;
	//lf.lfWeight = FW_BOLD;
	//::lstrcpy(lf.lfFaceName, "Tahoma");
	//font.CreatePointFontIndirect(&lf);	

	CFont *pOldFont = pDC->SelectObject(pFont);

	pDC->DrawText(szCaption,&rectTitleText,DT_SINGLELINE |  DT_VCENTER |DT_LEFT/*|DT_CENTER */);
	//font.DeleteObject();

	if(pOldFont)
	{
		pDC->SelectObject(pOldFont);
		pOldFont = NULL;
	}
}

void CChildDlgOwerDraw::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rectDlg(0,0,0,0);

	if(GetCapture() == this)
	{
		ReleaseCapture();
		CRect rectBtn(0,0,0,0);

		if(m_bDownExitBtn)
		{
			m_bDownExitBtn = FALSE;
			m_bHoverExitBtn = FALSE;

			CDC * pDC = GetWindowDC();

			if(pDC)
			{
				__DrawTitle(pDC);

				ReleaseDC(pDC);
				pDC = NULL;
			}

			rectBtn = m_crExitBtn;		
			//ScreenToClient(&point);

			if (rectBtn.PtInRect(point)) 
			{

				::PostMessage(GetParent()->m_hWnd,CUSTOMDLG_V1_CLOSE,(WPARAM)this,NULL);
				m_bDownExitBtn = FALSE;
				m_bHoverExitBtn = FALSE;
			}	
		}

		if(m_bDownMiniBtn)
		{
			m_bDownMiniBtn = FALSE;

			CDC * pDC = GetWindowDC();

			if(pDC)
			{
				__DrawTitle(pDC);

				ReleaseDC(pDC);
				pDC = NULL;
			}

			rectBtn = m_crMinBtn;
			
			if(rectBtn.PtInRect(point))
			{
				
				if(m_bMiniDlg)
				{
					::PostMessage(GetParent()->m_hWnd,CUSTOMDLG_V1_RESORE,(WPARAM)this,NULL);
				}
				else
				{
					::PostMessage(GetParent()->m_hWnd,CUSTOMDLG_V1_MINI,(WPARAM)this,NULL);
				}
			}
		}

		if (m_bDownLockBtn)
		{
			m_bDownLockBtn = FALSE;
			
			int iLock = m_bLock?0:1;             //TRUE,发送解锁命令 FALSE,发送锁定命令
			::PostMessage(GetParent()->m_hWnd,CUSTOMDLG_V1_LOCK,(WPARAM)this,iLock);
		}
		
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CChildDlgOwerDraw::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CRect rectDlg(0,0,0,0);
	CRect rectBtnTmp(0,0,0,0);

	// 添加了对退出按钮的消息响应 [9/13/2009 GuoLinlin]
	BOOL bDownExitBtn = FALSE;
	BOOL bDownMiniBtn = FALSE;
	BOOL bDownLockBtn = FALSE;

	if(m_crExitBtn.PtInRect(point))
	{
		bDownExitBtn = TRUE;
		SetCapture();
	}
	else
	{
		bDownExitBtn = FALSE;
	}	

	if(m_crMinBtn.PtInRect(point))
	{
		bDownMiniBtn = TRUE;
		SetCapture();

	}else
	{
		bDownMiniBtn = FALSE;
	}

	if (m_crLockBtn.PtInRect(point))
	{
		SetCapture();
		bDownLockBtn = TRUE;
	}
	else
	{
		bDownLockBtn = FALSE;
	}

	if(bDownMiniBtn != m_bDownMiniBtn || bDownExitBtn != m_bDownExitBtn || bDownLockBtn != m_bDownLockBtn)
	{

		m_bDownMiniBtn = bDownMiniBtn;
		m_bDownExitBtn = bDownExitBtn;
		m_bDownLockBtn = bDownLockBtn;

		CDC * pDC = GetWindowDC();

		if(pDC)
		{
			__DrawTitle(pDC);

			ReleaseDC(pDC);
			pDC = NULL;
		}	
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CChildDlgOwerDraw::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if( m_crCaption.PtInRect(point))
	{
		if (m_bMiniDlg)
		{	
			::PostMessage(GetParent()->m_hWnd, CUSTOMDLG_V1_RESORE, (WPARAM)this,NULL);
		}
		else
		{	
			::PostMessage(GetParent()->m_hWnd, CUSTOMDLG_V1_MINI, (WPARAM)this,NULL);
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CChildDlgOwerDraw::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CChildDlgOwerDraw::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CChildDlgOwerDraw::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


BOOL CChildDlgOwerDraw::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
			{
				//屏蔽Enter键
				if (pMsg->wParam == VK_RETURN)
				{
					return TRUE;
				}
				//屏蔽Esc键
				if (pMsg->wParam == VK_ESCAPE)
				{
					return TRUE;
				}
			}
			break;
		case WM_SYSKEYDOWN:
			{
				//屏蔽Alt + F4
				if (pMsg->wParam == VK_F4)
				{
					return TRUE;
				}
			}
			break;
		default:
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
