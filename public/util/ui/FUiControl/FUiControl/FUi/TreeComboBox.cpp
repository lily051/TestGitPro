// TreeComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TreeComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeComboBox
IMPLEMENT_DYNAMIC(CTreeComboBox, CComboBox)

CTreeComboBox::CTreeComboBox()
	:m_bControlActive(FALSE)
	,m_bAlertBkg(FALSE)
	,m_bAlertText(FALSE)
	,m_nDroppedHeight(150)
	,m_nDroppedWidth(100)
	,m_pImgArrow(NULL)
	,m_bError(FALSE)
	,m_bFocus(FALSE)
	,m_bMouseTrack(FALSE)
	,m_bHoverBtn(FALSE)
	,m_bPressBtn(FALSE)
	,m_nArrowWidth(16)
	,m_nArrowHeight(16)
{
	m_crAlertBkg = GetSysColor(COLOR_WINDOW);
	m_crAlertText = GetSysColor(COLOR_WINDOWTEXT);
	m_BrushAlert.CreateSolidBrush(m_crAlertBkg);
	m_mapBorderColor[FUi::Normal] = FUi::RGB_204;
	m_mapBorderColor[FUi::Focus] = FUi::EDIT_FOCUS;
	m_mapBorderColor[FUi::Error] = FUi::Red;
}

CTreeComboBox::~CTreeComboBox()
{
	m_BrushAlert.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTreeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTreeComboBox)
	//ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, &CTreeComboBox::OnCbnKillfocus)
	ON_CONTROL_REFLECT(CBN_SETFOCUS, &CTreeComboBox::OnCbnSetfocus)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WMU_CLOSE_CONTROL, OnCloseControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeComboBox message handlers

void CTreeComboBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	CComboBox::PreSubclassWindow();

	CRect rect(0, 0, 0, 0);
	DWORD dwStyle =  WS_POPUP | WS_BORDER;
	CWnd* pWnd = &m_Tree;
	pWnd->CreateEx(0, WC_TREEVIEW, NULL, dwStyle, rect, GetParent(), 0, NULL);
	m_Tree.Init(this);

	GetClientRect(rect);
	SetDroppedWidth(rect.Width());
	SetDroppedHeight(m_nDroppedHeight);

	COMBOBOXINFO comboBoxInfo;
	comboBoxInfo.cbSize=sizeof(comboBoxInfo);

	if (GetComboBoxInfo(&comboBoxInfo))
	{
		if (comboBoxInfo.hwndItem != NULL)
		{
			m_edit.SubclassWindow(comboBoxInfo.hwndItem);
			int nState = FUi::Normal | FUi::Hovered | FUi::Pressed | FUi::Disabled | FUi::Focus;
			m_edit.SetBorderColor(nState, FUi::White);
			m_edit.SetComboEdit(TRUE);
		}
	}


// 	dwStyle = CBS_DROPDOWNLIST & GetStyle();
// 	ASSERT(CBS_DROPDOWNLIST == dwStyle);
}

BOOL CTreeComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(WM_KEYDOWN == pMsg->message && (VK_DOWN == pMsg->wParam || VK_F4 == pMsg->wParam))
	{
		//DisplayTree();
		return TRUE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

// void CTreeComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
// {
// 	// TODO: Add your message handler code here and/or call default
// 
// 	m_bControlActive = ! m_bControlActive;
// 	if(m_bControlActive)
// 		DisplayTree();
// 
// //	CComboBox::OnLButtonDown(nFlags, point);
// }

void CTreeComboBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	OnLButtonDown(nFlags, point);

//	CComboBox::OnLButtonDblClk(nFlags, point);
}

void CTreeComboBox::TreeCtrlDone()
{
	CWnd* pParent = GetParent();

	if(pParent != NULL)
	{
		WPARAM wParam = MAKEWPARAM(GetDlgCtrlID(), CBN_CLOSEUP);
		pParent->SendMessage(WM_COMMAND, wParam, (LPARAM)m_hWnd);
	}
}

LRESULT CTreeComboBox::OnCloseControl(WPARAM wParam, LPARAM lParam)
{
	TreeCtrlDone();
	if(NULL != m_Tree.GetSafeHwnd())
	{
		m_Tree.ShowWindow(SW_HIDE);
		CToolTipCtrl* pTooltip = m_Tree.GetToolTips();
		if(NULL != pTooltip)
			pTooltip->Pop();
	}
	m_bControlActive = FALSE;
	if(GetCount())
		SetCurSel(0);
	SetFocus();

	return 1;
}

BOOL CTreeComboBox::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	if((! m_bAlertText && ! m_bAlertBkg) || WM_CTLCOLOREDIT != message)
		return CComboBox::OnChildNotify(message, wParam, lParam, pLResult);

	HDC hdcChild = (HDC)wParam;
	if(NULL != hdcChild)
	{
// 		if(m_bAlertText)
// 			SetTextColor(hdcChild, m_crAlertText);
// 		if(m_bAlertBkg)
// 			SetBkColor(hdcChild, m_crAlertBkg);
		*pLResult = (LRESULT)(m_BrushAlert.GetSafeHandle());
	}

	return TRUE;
//	return CComboBox::OnChildNotify(message, wParam, lParam, pLResult);
}

void CTreeComboBox::DisplayTree(CString strText)
{
	CRect rect;
	GetWindowRect(rect);

	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int nDesiredDroppedHeight, nDroppedHeight = m_nDroppedHeight;

	if(NULL != m_Tree.GetSafeHwnd())
	{
		int nCount = 0;
		HTREEITEM hItem = m_Tree.GetRootItem();
		while(NULL != hItem)
		{
			hItem = m_Tree.GetNextSiblingItem(hItem);
			nCount++;
		}
		if(nCount > 1)
		{
			nDesiredDroppedHeight = nCount * (m_Tree.GetItemHeight() + GetSystemMetrics(SM_CXEDGE) / 2);
			if(nDesiredDroppedHeight > nScreenHeight / 2)
				nDesiredDroppedHeight = nScreenHeight / 2;
			if(nDroppedHeight < nDesiredDroppedHeight)
				nDroppedHeight = nDesiredDroppedHeight;
		}
	}

	if(rect.bottom + nDroppedHeight + GetSystemMetrics(SM_CYCAPTION) > nScreenHeight)
	{
		rect.top = rect.top - GetSystemMetrics(SM_CXEDGE) - nDroppedHeight;
		rect.bottom = nDroppedHeight + 1;
	}
	else
	{
		rect.top = rect.bottom;
		rect.bottom = nDroppedHeight;
	}
	rect.right = m_nDroppedWidth;

   // m_Tree.SetScrollRange(SB_VERT, 0, 50);
    int n = m_Tree.GetScrollPos(SB_VERT);
    //选择默认节点
    if (strText == _T(""))
    {
        m_Tree.SelectItem(m_Tree.GetRootItem());
        m_Tree.SetScrollPos(SB_VERT, 0, FALSE);
    }
    else
    {
        HTREEITEM hSelectItem = FindItem(m_Tree.GetRootItem(), strText);
        if (hSelectItem != NULL)
        {
            m_Tree.SelectItem(hSelectItem);
        }
        else
        {
            m_Tree.SelectItem(m_Tree.GetRootItem());
            m_Tree.SetScrollPos(SB_VERT, 0, FALSE);
        }
    }

	m_Tree.Display(rect);
}

void CTreeComboBox::SetTitle(CString sTitle)
{
	if(GetCount())
	{
		SetCurSel(0);
		return;
	}

	ResetContent();
	AddString(sTitle);
	SetCurSel(0);
}

void CTreeComboBox::SetAlertColorBkg(const COLORREF crColor)
{
	m_crAlertBkg = crColor;
	m_BrushAlert.DeleteObject();
	m_BrushAlert.CreateSolidBrush(m_crAlertBkg);
}

// 调整Edit控件、下拉按钮的位置
void CTreeComboBox::OnSize(UINT nType, int cx, int cy)
{
	CComboBox::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcEdit;
	m_edit.GetWindowRect(&rcEdit);
	ScreenToClient(&rcEdit);

	rcEdit.left = EDIT_LEFT_M;
	rcEdit.right = rcClient.right - 1 - m_nArrowWidth;
	m_edit.MoveWindow(rcEdit);
}


void CTreeComboBox::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(rcClient);

	// edit 的位置
	CRect rcEdit(rcClient);
	m_edit.GetWindowRect(rcEdit);
	ScreenToClient(&rcEdit);

	// 下拉按钮的位置
	m_rcArrow.CopyRect(rcClient);
	m_rcArrow.top += rcClient.CenterPoint().y - m_nArrowHeight / 2;
	m_rcArrow.bottom = m_rcArrow.top + m_nArrowHeight;
	m_rcArrow.right -= 1;
	m_rcArrow.left = m_rcArrow.right - m_nArrowWidth;

	// edit 和 arrow 部分的区域排除掉
	CRect rc0;
	rc0.UnionRect(rcEdit, m_rcArrow);
	// 排除edit
	dc.ExcludeClipRect(rc0);
	// 背景
	CRect rcBackground(rcClient);
	rcBackground.DeflateRect(1, 1);
	RenderEngine->DrawColor(&dc, rcBackground, FUi::White);
	// 边框
	DrawBorder(&dc, rcClient);

	// dc 中恢复 arrow 部分的绘图
	HRGN hrgn = CreateRectRgn(m_rcArrow.left, m_rcArrow.top, m_rcArrow.right, m_rcArrow.bottom);
	ExtSelectClipRgn(dc.GetSafeHdc(), hrgn, RGN_OR);

	// 绘制按钮
	DrawArrowBtn(&dc, m_rcArrow);
}
// 画边框
void CTreeComboBox::DrawBorder(CDC* pDC, CRect& rcWnd)
{
	if (NULL == pDC)
	{
		return ;
	}

	int nCtrlState = FUi::Normal;
	if (m_bError)
	{
		nCtrlState = FUi::Error;
	}
	else
	{
		if (m_bFocus)
		{
			nCtrlState = FUi::Focus;
		}
	}

	RenderEngine->DrawRect(pDC, rcWnd, 1, m_mapBorderColor[nCtrlState]);
}

// 画按钮
void CTreeComboBox::DrawArrowBtn(CDC* pDC, CRect& rcArrow)
{
	if (NULL == pDC || NULL == m_pImgArrow)
	{
		return ;
	}

	int nOffset = 0;
	if (m_bHoverBtn)
	{
		nOffset = 1;
	}
	if (m_bPressBtn)
	{
		nOffset = 2;
	}

	RectF rcfDst((REAL)rcArrow.left, (REAL)rcArrow.top, (REAL)rcArrow.Width(), (REAL)rcArrow.Height());

	RenderEngine->DrawImageX(pDC, m_pImgArrow, rcfDst, 0, (REAL)m_nArrowHeight * nOffset, (REAL)m_nArrowWidth, (REAL)m_nArrowHeight);
}

// 失去焦点
void CTreeComboBox::OnCbnKillfocus()
{
	m_bFocus = FALSE;
	Invalidate();
	UpdateWindow();
}

// 获得焦点
void CTreeComboBox::OnCbnSetfocus()
{
	m_bFocus = TRUE;
	Invalidate();
	UpdateWindow();
}

// 设置界面字体
void CTreeComboBox::SetTextFont(int nState, const CString& strFontName)
{
	m_edit.SetTextFont(nState, strFontName);

	if (nState & FUi::Normal)
	{
		m_mapFont[FUi::Normal] = strFontName;
	}
	if (nState & FUi::Hovered)
	{
		m_mapFont[FUi::Hovered] = strFontName;
	}
	if (nState & FUi::Pressed)
	{
		m_mapFont[FUi::Pressed] = strFontName;
	}
	if (nState & FUi::Disabled)
	{
		m_mapFont[FUi::Disabled] = strFontName;
	}
	CFont* pFont = RenderEngine->GetFont(strFontName);
	SetFont(pFont);
}

void CTreeComboBox::SetBoderColor(int nState, COLORREF color)
{
	if (nState & FUi::Normal)
	{
		m_mapBorderColor[FUi::Normal] = color;
	}
	if (nState & FUi::Hovered)
	{
		m_mapBorderColor[FUi::Hovered] = color;
	}
	if (nState & FUi::Pressed)
	{
		m_mapBorderColor[FUi::Pressed] = color;
	}
	if (nState & FUi::Disabled)
	{
		m_mapBorderColor[FUi::Disabled] = color;
	}
	if (nState & FUi::Error)
	{
		m_mapBorderColor[FUi::Error] = color;
	}
	if (nState & FUi::Focus)
	{
		m_mapBorderColor[FUi::Focus] = color;
	}
	Invalidate();
}

// 设置提示文字
void CTreeComboBox::SetDefaultText(LPCTSTR szTipText)
{
	m_edit.SetDefaultText(szTipText);
}

// 设置字体颜色
void CTreeComboBox::SetTextColor(int nState, COLORREF color)
{
	m_edit.SetTextColor(nState, color);
}

// 设置是否输入错误
void CTreeComboBox::SetState(BOOL bError /*= FALSE*/)
{
	m_bError = bError;
}

// 设置输入框的文字
void CTreeComboBox::SetText(LPCTSTR szText)
{
	m_edit.SetWindowText(szText);
}

// 获取输入框的文字
void CTreeComboBox::GetText(CString& szText)
{
	m_edit.GetWindowText(szText);
}

void CTreeComboBox::ClearText()
{
	ResetContent();
	m_edit.SetWindowText(_T(""));
}

void CTreeComboBox::SetReadOnly(BOOL bReadOnly /*= TRUE*/)
{
	m_edit.SetReadOnly(bReadOnly);
}

void CTreeComboBox::SetObjectName(LPCTSTR szObjectName)
{
	// 设置下拉箭头的图片，normal、hover、pressed
	CString strTemp(szObjectName);
	Image *pImg = CResMgr::GetInstance()->GetRes(strTemp + _T("_arrow"));
	if (pImg)
	{
		m_pImgArrow = pImg;
		m_nArrowWidth = m_pImgArrow->GetWidth();
		m_nArrowHeight = m_pImgArrow->GetHeight() / 3;

		m_strObjectName = szObjectName;
	}
}

void CTreeComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
// 	if (m_rcArrow.PtInRect(point))
// 	{
// 		m_bPressBtn = TRUE;
// 		Invalidate();
// 		// 鼠标点击按钮，显示下拉
// 		ShowDropDown();
// 	}

    CString strText;
    GetText(strText);
	m_bControlActive = ! m_bControlActive;
	if(m_bControlActive && m_rcArrow.PtInRect(point))
	{
        SetState(FALSE);
		DisplayTree(strText);
	}
	

	//ShowDropDown(FALSE);

	//CComboBox::OnLButtonDown(nFlags, point);
}

void CTreeComboBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_rcArrow.PtInRect(point))
	{
		m_bPressBtn = FALSE;
		InvalidateRect(m_rcArrow);
	}

	CComboBox::OnLButtonUp(nFlags, point);
}

void CTreeComboBox::OnMouseHover(UINT nFlags, CPoint point)
{
	if (m_rcArrow.PtInRect(point))
	{
		m_bHoverBtn = TRUE;
		InvalidateRect(m_rcArrow);
	}
	else	//设置鼠标悬停tips提示
	{
		CString strText;
		m_edit.GetWindowText(strText);
		SetTipText(strText);
	}

	CComboBox::OnMouseHover(nFlags, point);
}

void CTreeComboBox::OnMouseLeave()
{
	// 鼠标离开
	m_bMouseTrack = FALSE;
	m_bHoverBtn = FALSE;
	InvalidateRect(m_rcArrow);

	CComboBox::OnMouseLeave();
}

void CTreeComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTrack)
	{
		TRACKMOUSEEVENT eventTrack;
		eventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		eventTrack.hwndTrack = m_hWnd;
		eventTrack.dwFlags = TME_LEAVE | TME_HOVER;
		eventTrack.dwHoverTime = 1;
		m_bMouseTrack = ::_TrackMouseEvent(&eventTrack);
	}

	CComboBox::OnMouseMove(nFlags, point);
}

void CTreeComboBox::SetActrueParent(CWnd* pParent)
{
	m_Tree.SetActrueParent(pParent);
}

void CTreeComboBox::SetTipText(LPCTSTR szTipText)
{
	_tcscpy_s(m_szTipText, MAX_LEN_TOOLTIP, szTipText);

	::PostMessage(GetParent()->GetSafeHwnd(), WM_CTRL_TOOLTIP, (WPARAM)this, (LPARAM)m_szTipText);
}

HTREEITEM CTreeComboBox::FindItem(HTREEITEM hRoot, CString strText)
{
    HTREEITEM hFind;

    //空树，直接返回
    if (hRoot == NULL)
    {
        return NULL;
    }

    //遍历查找
    while(hRoot != NULL)
    {
        if (m_Tree.GetItemText(hRoot) == strText)
        {
            return hRoot;
        }

        //查找当前节点的子节点
        if (m_Tree.ItemHasChildren(hRoot))
        {
            hRoot = m_Tree.GetChildItem(hRoot);
            hFind = FindItem(hRoot, strText);
            if (hFind)
            {
                return hFind;
            }
            else
            {
                hRoot = m_Tree.GetNextSiblingItem(m_Tree.GetParentItem(hRoot));
            }
        }
        else
        {
            hRoot = m_Tree.GetNextSiblingItem(hRoot);
        }
    }

    return hRoot;
}
