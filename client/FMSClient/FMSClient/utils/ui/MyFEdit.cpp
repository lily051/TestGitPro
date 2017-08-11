// FEdit.cpp : ʵ���ļ�
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

    m_brWhiteBkg.CreateSolidBrush(FUi::White);	//����ɫ
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


// CMyFEdit ��Ϣ�������
void CMyFEdit::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
    VCenterEdit(m_bMultiLine, lpncsp);
}

/*�ڸô���������������ɫˢ�£�δ���*/
void CMyFEdit::OnNcPaint()
{
    // ��ȡ�����ؼ���DC�������ǿͻ���
    CDC *pDC = GetWindowDC();
    if (NULL == pDC)
    {
        return ;
    }

    // �ؼ�����
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

    // �������򱳾�ɫ��Ĭ��Ϊ��ɫ
    CRect rcBackground(rcWnd);
    rcBackground.DeflateRect(1, 1);
    RenderEngine->DrawColor(pDC, rcBackground, FUi::White);

	// ����״̬���Ʊ߿�
    DrawBorder(pDC, rcWnd);

    ReleaseDC(pDC);
}

// ��ý���
void CMyFEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);

    m_bFocus = TRUE;

	// ֻ����������ز����
	if (GetStyle() & ES_READONLY)
	{
		HideCaret();
	}

    OnNcPaint();
}

// ʧȥ����
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

// ������ʾ����
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

// ������ʾ������ɫ
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
// ������ʾ����
void CMyFEdit::SetDefaultText(LPCTSTR szTipText)
{
    m_strDefaultText = szTipText;
    
	CRect rcClient;
	GetClientRect(&rcClient);

	// ��ȡDC
	CDC *pDC = GetDC();
	if (NULL == pDC)
	{
		return ;
	}

	// ����ĻDC �����ڴ�DC
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// ����ĻDC����λͼ
	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height()); 
	memDC.SelectObject(&bitMap);

	// ��������
	CFont *pFont = RenderEngine->GetFont(m_mapFont[FUi::Normal]);
	memDC.SelectObject(pFont);
	RenderEngine->DrawColor(&memDC, rcClient, FUi::White);
	rcClient.left += 5;// ΢��������߾�
	UINT uFormt = DT_SINGLELINE;
	if (!m_bMultiLine)
	{
		rcClient.top -= 5;
		uFormt |= DT_VCENTER;	// �����ı�����ʾ���־�����ʾ
	}
	else
	{
		rcClient.top += 3;	// ΢�������ı�����ʾ���ֵ��ϱ߾�
	}
	if (m_bIsComboBox)
	{
		rcClient.top -= 0;	// ΢��ComboBox��edit��ʾ���ֵ�λ��
	}
	RenderEngine->DrawTextX(&memDC, m_strDefaultText, rcClient, m_mapTextColor[FUi::Default], uFormt);

	// ����������ˢ
	m_brTextBkg.Detach();
	m_brTextBkg.CreatePatternBrush(&bitMap);

	ReleaseDC(pDC);
}
// ���ñ߿���ɫ
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
// �����Ƿ������ʾ״̬
void CMyFEdit::SetErrorState(BOOL bError /*= FALSE*/)
{
    m_bError = bError;
    SetFocus();
}
// ���߿�
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

// �����иı�
void CMyFEdit::OnEnChange()
{
    // ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CEdit::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// �û����룬�������״̬
    if (m_bError)
    {
        SetErrorState(FALSE);
        SendMessage(WM_NCPAINT, 0, 0);
    }

	// ��ǰ������Ƿ�Ϊ��
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

	SetTipText(strText); //������ʾ����

	Invalidate();
}

//lint -e569
void CMyFEdit::SetPasswordMode(BOOL bSet)
{
    if (bSet)
	{
		m_bPassword = TRUE;
        SetPasswordChar(_T('��'));
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
    // ��������ı�����ô�Ͳ�������ֱ����
    if (bFlag)
    {
        lpncsp->rgrc[0].top += nBorderW;
        lpncsp->rgrc[0].bottom -= nBorderW;
        lpncsp->rgrc[0].left += nBorderW;
        lpncsp->rgrc[0].right -= nBorderW;

        return ;
    }
    // �������ָ߶�
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
            // ֻ������²���ӦEM_SETSEL��Ϣ
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
                    // �������ȡ�����ı������ؿ�
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
