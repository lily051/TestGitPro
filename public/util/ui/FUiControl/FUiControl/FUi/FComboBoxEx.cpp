// FComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "FComboBoxEx.h"

// 编辑框左边距离外边框的距离
#define EDIT_LEFT_M 3

// CFComboBoxEx
IMPLEMENT_DYNAMIC(CFComboBoxEx, CComboBox)

CFComboBoxEx::CFComboBoxEx()
: m_pImgArrow(NULL)
, m_bError(FALSE)
, m_bFocus(FALSE)
, m_bMouseTrack(FALSE)
, m_bHoverBtn(FALSE)
, m_bPressBtn(FALSE)
, m_nArrowWidth(24)
, m_nArrowHeight(24)
{
    m_mapBorderColor[FUi::Normal] = FUi::RGB_204;
    m_mapBorderColor[FUi::Focus] = FUi::EDIT_FOCUS;
    m_mapBorderColor[FUi::Error] = FUi::Red;
}

CFComboBoxEx::~CFComboBoxEx()
{
}

BEGIN_MESSAGE_MAP(CFComboBoxEx, CComboBox)
    ON_CONTROL_REFLECT(CBN_KILLFOCUS, &CFComboBoxEx::OnCbnKillfocus)
    ON_CONTROL_REFLECT(CBN_SETFOCUS, &CFComboBoxEx::OnCbnSetfocus)
    ON_CBN_SELCHANGE(CBN_SELCHANGE, &CFComboBoxEx::OnCbnSelchange)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &CFComboBoxEx::OnCbnCloseup)
END_MESSAGE_MAP()


// CFComboBoxEx 消息处理程序
void CFComboBoxEx::PreSubclassWindow()
{
    COMBOBOXINFO comboBoxInfo;
    comboBoxInfo.cbSize=sizeof(comboBoxInfo);

    if (GetComboBoxInfo(&comboBoxInfo))
    {
        if (comboBoxInfo.hwndItem != NULL)
        {
            m_edit.SubclassWindow(comboBoxInfo.hwndItem);
            m_edit.SetBorderColor(FUi::White, FUi::White);
			m_edit.SetComboEdit(TRUE);
        }
    }

    CComboBox::PreSubclassWindow();
}

// 调整Edit控件、下拉按钮的位置
void CFComboBoxEx::OnSize(UINT nType, int cx, int cy)
{
    CComboBox::OnSize(nType, cx, cy);

    CRect rcClient;
    GetClientRect(rcClient);

    CRect rcEdit;
    m_edit.GetWindowRect(&rcEdit);
    ScreenToClient(&rcEdit);

	rcEdit.top = rcClient.top + 1;
	rcEdit.bottom = rcClient.bottom - 1;
    rcEdit.left = EDIT_LEFT_M;
    rcEdit.right = rcClient.right - 1 - m_nArrowWidth;
    m_edit.MoveWindow(rcEdit);
}

void CFComboBoxEx::OnPaint()
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
    dc.ExcludeClipRect(rcEdit);
    // 背景
    RenderEngine->DrawColor(&dc, rcClient, FUi::White);
    // 边框
    DrawBorder(&dc, rcClient);

    // dc 中恢复 arrow 部分的绘图
    HRGN hrgn = CreateRectRgn(m_rcArrow.left, m_rcArrow.top, m_rcArrow.right, m_rcArrow.bottom);
    ExtSelectClipRgn(dc.GetSafeHdc(), hrgn, RGN_OR);

    // 绘制按钮
    DrawArrowBtn(&dc, m_rcArrow);
}
// 画边框
void CFComboBoxEx::DrawBorder(CDC* pDC, CRect& rcWnd)
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
void CFComboBoxEx::DrawArrowBtn(CDC* pDC, CRect& rcArrow)
{
    if (NULL == pDC || NULL == m_pImgArrow)
    {
        return ;
    }

    int nOffset = 0;
	if (m_bPressBtn)
	{
		nOffset = 2;
	}
    else if (m_bHoverBtn)
    {
        nOffset = 1;
    }

    RectF rcfDst((REAL)rcArrow.left, (REAL)rcArrow.top, (REAL)rcArrow.Width(), (REAL)rcArrow.Height());

    RenderEngine->DrawImageX(pDC, m_pImgArrow, rcfDst, 0, (REAL)m_nArrowHeight * nOffset, (REAL)m_nArrowWidth, (REAL)m_nArrowHeight);
}

// 获得焦点
void CFComboBoxEx::OnCbnKillfocus()
{
    m_bFocus = FALSE;
	m_edit.PostMessage(WM_KILLFOCUS);
}

// 失去焦点
void CFComboBoxEx::OnCbnSetfocus()
{
    m_bFocus = TRUE;
}

// 设置界面字体
void CFComboBoxEx::SetTextFont(int nState, const CString& strFontName)
{
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

void CFComboBoxEx::SetBoderColor(int nState, COLORREF color)
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
void CFComboBoxEx::SetDefaultText(LPCTSTR szTipText)
{
    m_edit.SetDefaultText(szTipText);
}

// 设置字体颜色
void CFComboBoxEx::SetTextColor(int nState, COLORREF color)
{
    m_edit.SetTextColor(nState, color);
}

// 设置是否输入错误
void CFComboBoxEx::SetState(BOOL bError /*= FALSE*/)
{
    m_bError = bError;
}

// 设置输入框的文字
void CFComboBoxEx::SetText(LPCTSTR szText)
{
    m_edit.SetWindowText(szText);
}

// 获取输入框的文字
void CFComboBoxEx::GetText(LPTSTR szText)
{
    //m_edit.GetWindowText(szText);
}

void CFComboBoxEx::ClearText()
{
    ResetContent();
    m_edit.SetWindowText(_T(""));
}

void CFComboBoxEx::SetReadOnly(BOOL bReadOnly /*= TRUE*/)
{
	m_edit.SetReadOnly(bReadOnly);
}

void CFComboBoxEx::OnCbnSelchange()
{
    ExecSelChange();
}

void CFComboBoxEx::SetObjectName(LPCTSTR szObjectName)
{
    // 设置下拉箭头的图片，normal、hover、pressed
    CString strTemp(szObjectName);
    Image *pImg = CResMgr::GetInstance()->GetRes(strTemp);
    if (pImg)
    {
        m_pImgArrow = pImg;
        m_nArrowWidth = m_pImgArrow->GetWidth();
        m_nArrowHeight = m_pImgArrow->GetHeight() / 3;

        m_strObjectName = szObjectName;
    }
}

void CFComboBoxEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_rcArrow.PtInRect(point))
    {
		m_bPressBtn = TRUE;
		Invalidate();
		ExecClickDropBtn();
		// 鼠标点击按钮，显示下拉
    }
    //CComboBox::OnLButtonDown(nFlags, point);
}

void CFComboBoxEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bPressBtn = FALSE;
	Invalidate();

	CComboBox::OnLButtonUp(nFlags, point);
}

void CFComboBoxEx::OnMouseHover(UINT nFlags, CPoint point)
{
    Invalidate();
    CComboBox::OnMouseHover(nFlags, point);
}

void CFComboBoxEx::OnMouseLeave()
{
    // 鼠标离开
    m_bMouseTrack = FALSE;
	m_bHoverBtn = FALSE;
    Invalidate();
    CComboBox::OnMouseLeave();
}

void CFComboBoxEx::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bMouseTrack)
    {
        TRACKMOUSEEVENT eventTrack;
        eventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
        eventTrack.hwndTrack = m_hWnd;
        eventTrack.dwFlags = TME_LEAVE | TME_HOVER;
        eventTrack.dwHoverTime = 10;
        m_bMouseTrack = ::_TrackMouseEvent(&eventTrack);
    }

	CPoint ptCurrent(0,0);
	GetCursorPos(&ptCurrent);
	ScreenToClient(&ptCurrent);

	if (m_rcArrow.PtInRect(point))
	{
		if (!m_bHoverBtn)
		{
			m_bHoverBtn = TRUE;
			Invalidate();
		}
	}
	else
	{
		if (m_bHoverBtn)
		{
			m_bHoverBtn = FALSE;
			Invalidate();
		}
	}


    CComboBox::OnMouseMove(nFlags, point);
}

/*
	初始化时不会进入PreTranslateMessage，会进入WindowProc

	鼠标选择时都会进入

	方向键上下时都会进入

	其他事件（比如按钮点击）SetWindowText 或 SetCurSel 会进入PreTranslateMessage ，不会进入WindowProc
*/
BOOL CFComboBoxEx::PreTranslateMessage(MSG* pMsg)
{
	// ComboBox设置文字或者SetSel后，Edit不会刷新，
	// 拦截到每次都有消息15，所以这里先手动刷新一下Edit
	if (pMsg->message == 15)
	{
		m_edit.UpdateCtrl();
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

LRESULT CFComboBoxEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == 15)
	{
		m_edit.UpdateCtrl();
	}

	if (message == WM_MOUSEMOVE)
	{
		OnMouseMove(wParam, lParam);
	}

	return CComboBox::WindowProc(message, wParam, lParam);
}

void CFComboBoxEx::ExecClickDropBtn(void)
{
	ShowDropDown();
}

void CFComboBoxEx::ExecSelChange()
{
	CString strItem;
	GetLBText(GetCurSel(), strItem);
	SetText(strItem);
}

void CFComboBoxEx::OnCbnCloseup()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bPressBtn = FALSE;
	Invalidate();
}
