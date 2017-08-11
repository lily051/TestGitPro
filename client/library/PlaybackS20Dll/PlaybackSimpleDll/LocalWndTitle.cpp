// LocalWndTitle.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalWndTitle.h"


// CLocalWndTitle

IMPLEMENT_DYNAMIC(CLocalWndTitle, CWnd)

CLocalWndTitle::CLocalWndTitle()
{
	m_clrBk = UI_PLAYWND_TITLE_COLOR;
	m_clrText = RGB(255, 255, 255);
	m_strText = _T("");

	//���ñ�������
	m_fTextFont.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_MODERN, _T("Arial"));
}

CLocalWndTitle::~CLocalWndTitle()
{
	try
	{
		m_fTextFont.DeleteObject();
	}
	catch (...)
	{

	}
}


BEGIN_MESSAGE_MAP(CLocalWndTitle, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CLocalWndTitle ��Ϣ�������



int CLocalWndTitle::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

BOOL CLocalWndTitle::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrBk);

	pDC->SetTextColor(m_clrText);
	pDC->SelectObject(&m_fTextFont);
	pDC->DrawText(m_strText, rcClient, NULL);

	return TRUE;
}

//�����ı�
void CLocalWndTitle::SetText(CString strText)
{
	m_strText = strText;
	Update();
}

//ˢ��
void CLocalWndTitle::Update()
{
	CRect rc;
	GetWindowRect(rc);
	RedrawWindow();

	Invalidate(TRUE);
	UpdateWindow();
}
