#include "stdafx.h"
#include "PlayTitleWnd.h"

//lint -library
IMPLEMENT_DYNAMIC(CPlayTitleWnd, CWnd)
CPlayTitleWnd::CPlayTitleWnd()
{
	m_clrBk = UI_PLAYWND_TITLE_COLOR;
	m_clrText = RGB(212, 212, 212);
	m_strText = _T("");

	//���ñ�������
	m_fTextFont.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_MODERN, _T("Arial"));
}

CPlayTitleWnd::~CPlayTitleWnd()
{
	try
	{
		m_fTextFont.DeleteObject();
	}
	catch (...)
	{
		
	}
}


BEGIN_MESSAGE_MAP(CPlayTitleWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CPlayTitleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CPlayTitleWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
}

//�����ı�
void CPlayTitleWnd::SetText(CString strText)
{
	m_strText = strText;
	UpdateSurface();
}

//���ñ����ı���ɫ
void CPlayTitleWnd::SetTextColour(COLORREF clrText)
{
	m_clrText = clrText;
}

//�����ı�����
void CPlayTitleWnd::SetTextFont(CFont* fTextFont)
{
	memcpy(&m_fTextFont, fTextFont, sizeof(CFont));
}

//��ȡ��������
CString CPlayTitleWnd::GetText()
{
	return m_strText;
}

//ˢ��
void CPlayTitleWnd::UpdateSurface()
{
	CRect rc;
	GetWindowRect(rc);
	RedrawWindow();

	Invalidate(TRUE);
	UpdateWindow();
}

BOOL CPlayTitleWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrBk);

	pDC->SetTextColor(m_clrText);
	pDC->SelectObject(&m_fTextFont);
	pDC->DrawText(m_strText, rcClient, NULL);
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}