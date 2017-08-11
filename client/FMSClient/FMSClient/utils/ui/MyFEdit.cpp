// FEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "MyFEdit.h"


// CMyFEdit

//IMPLEMENT_DYNAMIC(CMyFEdit, CEdit)

CMyFEdit::CMyFEdit()
: m_bFocus(FALSE)
, m_bError(FALSE)
, m_bIsEmpty(TRUE)
//, m_bInput(FALSE)
, m_bPassword(FALSE)
, m_bMultiLine(FALSE)
, m_bIsComboBox(FALSE)
, m_nClientOffsetX(2)
, m_nClientOffsetY(2)
, m_bCanGetText(FALSE)
{
	m_mapFont[FUi::Normal] = YAHEI_20_N;
	m_mapFont[FUi::Hovered] = YAHEI_20_N;
	m_mapFont[FUi::Pressed] = YAHEI_20_N;
	m_mapFont[FUi::Disabled] = YAHEI_20_N;

	m_mapTextColor[FUi::Normal] = FUi::Black;
	m_mapTextColor[FUi::Hovered] = FUi::Black;
	m_mapTextColor[FUi::Pressed] = FUi::Black;
	m_mapTextColor[FUi::Disabled] = FUi::Black;
	m_mapTextColor[FUi::Default] = FUi::RGB_204;


    m_mapBorderColor[FUi::Normal] = FUi::RGB_204;
    m_mapBorderColor[FUi::Focus] = FUi::EDIT_FOCUS;
    m_mapBorderColor[FUi::Error] = FUi::Red;

    m_brWhiteBkg.CreateSolidBrush(FUi::White);	//背景色
}

CMyFEdit::~CMyFEdit()
{
    try
    {
		SAFE_DELETE_BRUSH(m_brTextBkg);
		SAFE_DELETE_BRUSH(m_brWhiteBkg);
    }
    catch (...)
    {
    }
}

BEGIN_MESSAGE_MAP(CMyFEdit, CEdit)
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
   // ON_CONTROL_REFLECT(EN_CHANGE, &CMyFEdit::OnEnChange)
    ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMyFEdit 消息处理程序
void CMyFEdit::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
    VCenterEdit(m_bMultiLine, lpncsp);
}

/*在该处处理上下两条颜色刷新，未完成*/
void CMyFEdit::OnNcPaint()
{
    // 获取整个控件的DC，包括非客户区
    CDC *pDC = GetWindowDC();
    if (NULL == pDC)
    {
        return ;
    }

    // 控件区域
    CRect rcWnd;
    GetWindowRect(rcWnd);
    rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);

    int nBorder = GetSystemMetrics(SM_CXEDGE);
    CRect rcClient;
    GetClientRect(rcClient);
    if (!m_bMultiLine)
    {
        rcClient.OffsetRect(m_nClientOffsetX, m_nClientOffsetY);
    }
    else
    {
        //int nScrollW = GetSystemMetrics(SM_CXVSCROLL);
        rcClient.DeflateRect(nBorder, nBorder, nBorder, nBorder);

        if (GetScrollPos(SB_VERT))
        {
            rcClient.right += GetSystemMetrics(SM_CXVSCROLL);
        }
    }
    pDC->ExcludeClipRect(rcClient);

    // 输入区域背景色：默认为白色
    CRect rcBackground(rcWnd);
    rcBackground.DeflateRect(1, 1);
    RenderEngine->DrawColor(pDC, rcBackground, FUi::White);

	// 根据状态绘制边框
    DrawBorder(pDC, rcWnd);

    ReleaseDC(pDC);
}

// 获得焦点
void CMyFEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);

    m_bFocus = TRUE;

	// 只读情况下隐藏插入符
	if (GetStyle() & ES_READONLY)
	{
		HideCaret();
	}

    OnNcPaint();
}

// 失去焦点
void CMyFEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);

    m_bFocus = FALSE;

    OnNcPaint();
}

HBRUSH CMyFEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	if(m_bIsEmpty)
	{
		return m_brTextBkg;
	}
	return m_brWhiteBkg;
}

// 设置显示字体
void CMyFEdit::SetTextFont(int nState, const CString& strFontName)
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

// 设置显示文字颜色
void CMyFEdit::SetTextColor(int nState, COLORREF color)
{
    if (nState & FUi::Normal)
    {
        m_mapTextColor[FUi::Normal] = color;
    }
    if (nState & FUi::Hovered)
    {
        m_mapTextColor[FUi::Hovered] = color;
    }
    if (nState & FUi::Pressed)
    {
        m_mapTextColor[FUi::Pressed] = color;
    }
    if (nState & FUi::Disabled)
    {
        m_mapTextColor[FUi::Disabled] = color;
    }
    if (nState & FUi::Error)
    {
        m_mapTextColor[FUi::Error] = color;
    }
    if (nState & FUi::Focus)
    {
        m_mapTextColor[FUi::Focus] = color;
    }
	if (nState & FUi::Default)
	{
		m_mapTextColor[FUi::Default] = color;
	}
    Invalidate();
}
// 设置提示文字
void CMyFEdit::SetDefaultText(LPCTSTR szTipText)
{
    m_strDefaultText = szTipText;
    
	CRect rcClient;
	GetClientRect(&rcClient);

	// 获取DC
	CDC *pDC = GetDC();
	if (NULL == pDC)
	{
		return ;
	}

	// 以屏幕DC 建立内存DC
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// 以屏幕DC建立位图
	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height()); 
	memDC.SelectObject(&bitMap);

	// 绘制文字
	CFont *pFont = RenderEngine->GetFont(m_mapFont[FUi::Normal]);
	memDC.SelectObject(pFont);
	RenderEngine->DrawColor(&memDC, rcClient, FUi::White);
	rcClient.left += 5;// 微调文字左边距
	UINT uFormt = DT_SINGLELINE;
	if (!m_bMultiLine)
	{
		rcClient.top -= 5;
		uFormt |= DT_VCENTER;	// 单行文本下提示文字居中显示
	}
	else
	{
		rcClient.top += 3;	// 微调多行文本下提示文字的上边距
	}
	if (m_bIsComboBox)
	{
		rcClient.top -= 0;	// 微调ComboBox中edit提示文字的位置
	}
	RenderEngine->DrawTextX(&memDC, m_strDefaultText, rcClient, m_mapTextColor[FUi::Default], uFormt);

	// 创建背景画刷
	m_brTextBkg.Detach();
	m_brTextBkg.CreatePatternBrush(&bitMap);

	ReleaseDC(pDC);
}
// 设置边框颜色
void CMyFEdit::SetBorderColor(int nState, COLORREF color)
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
// 设置是否错误提示状态
void CMyFEdit::SetErrorState(BOOL bError /*= FALSE*/)
{
    m_bError = bError;
    SetFocus();
}
// 画边框
void CMyFEdit::DrawBorder(CDC* pDC, CRect& rcWnd)
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

// 内容有改变
void CMyFEdit::OnEnChange()
{
    // 如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CEdit::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// 用户输入，清除错误状态
    if (m_bError)
    {
        SetErrorState(FALSE);
        SendMessage(WM_NCPAINT, 0, 0);
    }

	// 当前输入框是否为空
	CString strText;
	if (m_bPassword)
	{
        GetPasswordText(strText);
	}
    else
    {
        GetWindowText(strText);
    }
	m_bIsEmpty = strText.IsEmpty();

	SetTipText(strText); //设置提示文字

	Invalidate();
}

//lint -e569
void CMyFEdit::SetPasswordMode(BOOL bSet)
{
    if (bSet)
	{
		m_bPassword = TRUE;
        SetPasswordChar(_T('●'));
    }
    else
	{
		m_bPassword = FALSE;
        SetPasswordChar(0);
    }
}

void CMyFEdit::GetPasswordText(CString& strText)
{
    m_bCanGetText = TRUE;
    return GetWindowText(strText);
}

BOOL CMyFEdit::IsPassWord()
{
	return m_bPassword;
}

void CMyFEdit::SetIsEmpty(BOOL bEmpty)
{
	m_bIsEmpty = bEmpty;
}

BOOL CMyFEdit::IsError()
{
	return m_bError;
}

//lint +e569

void CMyFEdit::PreSubclassWindow()
{
    m_bMultiLine = ES_MULTILINE & GetStyle();
    TCHAR ch = GetPasswordChar();
    if (NULL != ch)
    {
        SetPasswordMode(TRUE);
    }
    SendMessage(WM_SIZE);

    CEdit::PreSubclassWindow();
}

void CMyFEdit::VCenterEdit(BOOL bFlag, NCCALCSIZE_PARAMS* lpncsp)
{
    int nBorderW = GetSystemMetrics(SM_CXEDGE);
    // 如果多行文本，那么就不设置竖直居中
    if (bFlag)
    {
        lpncsp->rgrc[0].top += nBorderW;
        lpncsp->rgrc[0].bottom -= nBorderW;
        lpncsp->rgrc[0].left += nBorderW;
        lpncsp->rgrc[0].right -= nBorderW;

        return ;
    }
    // 计算文字高度
    CDC* pDC = GetDC();
    if (NULL == pDC)
    {
        return ;
    }
    CFont *pFont = RenderEngine->GetFont(m_mapFont[FUi::Normal]);
    CFont *pOld = pDC->SelectObject(pFont);
    CRect rcText;
    pDC->DrawText(_T("Height"), rcText, DT_CALCRECT | DT_LEFT);
    int nTextHeight = rcText.Height();
    pDC->SelectObject(pOld);
    ReleaseDC(pDC);

    CRect rcNewClient(lpncsp->rgrc[0]);
    m_nClientOffsetY = (rcNewClient.Height() - nTextHeight) / 2;// - nBorderW;
    
    /*
    lpncsp->rgrc:
    The first contains the new coordinates of a window that has been moved or resized. 
    The second contains the coordinates of the window before it was moved or resized. 
    The third contains the coordinates of the client area of a window before it was moved or resized. 
    If the window is a child window, the coordinates are relative to the client area of the parent window. 
    If the window is a top-level window, the coordinates are relative to the screen. 
    */
    int nCX = lpncsp->rgrc[2].left - lpncsp->rgrc[1].left;
    int nCY = lpncsp->rgrc[2].top - lpncsp->rgrc[1].top + 1;
    lpncsp->rgrc[0].top += m_nClientOffsetY;
    lpncsp->rgrc[0].bottom -= /*nCenterOffset*/2;
    lpncsp->rgrc[0].left += nCX;
    lpncsp->rgrc[0].right -= nCY;

    m_nClientOffsetX = nCX;
}

void CMyFEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);

	SetDefaultText(m_strDefaultText);
}


LRESULT CMyFEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case EM_SETSEL:
        {
            // 只读情况下不响应EM_SETSEL消息
            if (GetStyle() & ES_READONLY)
            {
                return 0;
            }
            break;
        }
    case WM_GETTEXT:
        {
            if (m_bPassword)
            {
                if (m_bCanGetText)
                {
                    m_bCanGetText = FALSE;
                }
                else
                {
                    // 不允许获取密码文本，返回空
                    return 0;
                }
            }
            break;
        }
    }

	return CEdit::WindowProc(message, wParam, lParam);
}

#define WM_CTRL_TOOLTIP	(WM_USER + 2011)

void CMyFEdit::SetTipText(LPCTSTR szTipText)
{
	_tcscpy_s(m_szTipText, MAX_LEN_TOOLTIP, szTipText);

	::PostMessage(GetParent()->GetSafeHwnd(), WM_CTRL_TOOLTIP, (WPARAM)this, (LPARAM)m_szTipText);
}
