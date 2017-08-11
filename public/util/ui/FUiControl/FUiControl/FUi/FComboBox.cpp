// FComboBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FComboBox.h"

// �༭����߾�����߿�ľ���
#define EDIT_LEFT_M 3

// CFComboBox
IMPLEMENT_DYNAMIC(CFComboBox, CComboBox)

CFComboBox::CFComboBox()
: m_pImgArrow(NULL)
, m_bError(FALSE)
, m_bFocus(FALSE)
, m_bMouseTrack(FALSE)
, m_bHoverBtn(FALSE)
, m_bPressBtn(FALSE)
, m_nArrowWidth(16)
, m_nArrowHeight(16)
,m_bPopupDlg(FALSE)
{
    m_mapBorderColor[FUi::Normal] = FUi::RGB_204;
    m_mapBorderColor[FUi::Focus] = FUi::EDIT_FOCUS;
    m_mapBorderColor[FUi::Error] = FUi::Red;
}

CFComboBox::~CFComboBox()
{
}

BEGIN_MESSAGE_MAP(CFComboBox, CComboBox)
    ON_CONTROL_REFLECT(CBN_KILLFOCUS, &CFComboBox::OnCbnKillfocus)
    ON_CONTROL_REFLECT(CBN_SETFOCUS, &CFComboBox::OnCbnSetfocus)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CFComboBox ��Ϣ�������
void CFComboBox::PreSubclassWindow()
{
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

    CComboBox::PreSubclassWindow();
}

// ����Edit�ؼ���������ť��λ��
void CFComboBox::OnSize(UINT nType, int cx, int cy)
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

void CFComboBox::OnPaint()
{
    CPaintDC dc(this);

    CRect rcClient;
    GetClientRect(rcClient);

    // edit ��λ��
    CRect rcEdit(rcClient);
    m_edit.GetWindowRect(rcEdit);
    ScreenToClient(&rcEdit);

    // ������ť��λ��
    m_rcArrow.CopyRect(rcClient);
    m_rcArrow.top += rcClient.CenterPoint().y - m_nArrowHeight / 2;
    m_rcArrow.bottom = m_rcArrow.top + m_nArrowHeight;
    m_rcArrow.right -= 1;
    m_rcArrow.left = m_rcArrow.right - m_nArrowWidth;

    // edit �� arrow ���ֵ������ų���
    CRect rc0;
    rc0.UnionRect(rcEdit, m_rcArrow);
    // �ų�edit
    dc.ExcludeClipRect(rc0);
    // ����
    CRect rcBackground(rcClient);
    rcBackground.DeflateRect(1, 1);
    RenderEngine->DrawColor(&dc, rcBackground, FUi::White);
    // �߿�
    DrawBorder(&dc, rcClient);

    // dc �лָ� arrow ���ֵĻ�ͼ
    HRGN hrgn = CreateRectRgn(m_rcArrow.left, m_rcArrow.top, m_rcArrow.right, m_rcArrow.bottom);
    ExtSelectClipRgn(dc.GetSafeHdc(), hrgn, RGN_OR);

    // ���ư�ť
    DrawArrowBtn(&dc, m_rcArrow);
}
// ���߿�
void CFComboBox::DrawBorder(CDC* pDC, CRect& rcWnd)
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

// ����ť
void CFComboBox::DrawArrowBtn(CDC* pDC, CRect& rcArrow)
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

// ʧȥ����
void CFComboBox::OnCbnKillfocus()
{
	m_bFocus = FALSE;
	Invalidate();
	UpdateWindow();
}

// ��ý���
void CFComboBox::OnCbnSetfocus()
{
    m_bFocus = TRUE;
	Invalidate();
	UpdateWindow();
}

// ���ý�������
void CFComboBox::SetTextFont(int nState, const CString& strFontName)
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

void CFComboBox::SetBoderColor(int nState, COLORREF color)
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

// ������ʾ����
void CFComboBox::SetDefaultText(LPCTSTR szTipText)
{
    m_edit.SetDefaultText(szTipText);
}

// ����������ɫ
void CFComboBox::SetTextColor(int nState, COLORREF color)
{
    m_edit.SetTextColor(nState, color);
}

// �����Ƿ��������
void CFComboBox::SetState(BOOL bError /*= FALSE*/)
{
    m_bError = bError;
}

// ��������������
void CFComboBox::SetText(LPCTSTR szText)
{
    m_edit.SetWindowText(szText);
}

// ��ȡ����������
void CFComboBox::GetText(LPTSTR szText)
{
    //m_edit.GetWindowText(szText);
}

void CFComboBox::ClearText()
{
    ResetContent();
    m_edit.SetWindowText(_T(""));
}

void CFComboBox::SetReadOnly(BOOL bReadOnly /*= TRUE*/)
{
	m_edit.SetReadOnly(bReadOnly);
}

void CFComboBox::SetObjectName(LPCTSTR szObjectName)
{
    // ����������ͷ��ͼƬ��normal��hover��pressed
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

void CFComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_rcArrow.PtInRect(point))
    {
		m_bPressBtn = TRUE;
		Invalidate();
		// �������ť����ʾ����
		ShowDropDown(TRUE);
		if (m_bPopupDlg)
		{
			ShowDropDown(FALSE);
		}
		
    }

    //CComboBox::OnLButtonDown(nFlags, point);
}

void CFComboBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_rcArrow.PtInRect(point))
	{
		m_bPressBtn = FALSE;
		InvalidateRect(m_rcArrow);
	}

    CComboBox::OnLButtonUp(nFlags, point);
}

void CFComboBox::OnMouseHover(UINT nFlags, CPoint point)
{
	if (m_rcArrow.PtInRect(point))
	{
		m_bHoverBtn = TRUE;
		InvalidateRect(m_rcArrow);
	}
	else	//���������ͣtips��ʾ
	{
		CString strText;
		m_edit.GetWindowText(strText);
		SetTipText(strText);
	}

    CComboBox::OnMouseHover(nFlags, point);
}

void CFComboBox::OnMouseLeave()
{
    // ����뿪
    m_bMouseTrack = FALSE;
    m_bHoverBtn = FALSE;
	InvalidateRect(m_rcArrow);

    CComboBox::OnMouseLeave();
}

void CFComboBox::OnMouseMove(UINT nFlags, CPoint point)
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

/*
	��ʼ��ʱ�������PreTranslateMessage�������WindowProc

	���ѡ��ʱ�������

	���������ʱ�������

	�����¼������簴ť�����SetWindowText �� SetCurSel �����PreTranslateMessage ���������WindowProc
*/
BOOL CFComboBox::PreTranslateMessage(MSG* pMsg)
{
	// ComboBox�������ֻ���SetSel��Edit����ˢ�£�
	// ���ص�ÿ�ζ�����Ϣ15�������������ֶ�ˢ��һ��Edit
	if (pMsg->message == 15)
	{
		m_edit.UpdateCtrl();
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

LRESULT CFComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == 15)
	{
		m_edit.UpdateCtrl();
	}

	return CComboBox::WindowProc(message, wParam, lParam);
}

void CFComboBox::SetPopupDlg(BOOL bPopup)
{
	m_bPopupDlg = bPopup;
}

void CFComboBox::SetTipText(LPCTSTR szTipText)
{
	_tcscpy_s(m_szTipText, MAX_LEN_TOOLTIP, szTipText);

	::PostMessage(GetParent()->GetSafeHwnd(), WM_CTRL_TOOLTIP, (WPARAM)this, (LPARAM)m_szTipText);
}