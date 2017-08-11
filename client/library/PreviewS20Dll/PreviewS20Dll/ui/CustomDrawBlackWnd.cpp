// CustomDrawGrayWnd.cpp : ʵ���ļ�
//
/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			CustomDrawBlackWnd.cpp
Description:  		�Ի�����
1.��ɫ����
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-3-26     qmx    ����
*****************************************************/
//lint -library
#include "stdafx.h"
#include "../PreviewS20Dll.h"
#include "CustomDrawBlackWnd.h"


// CCustomDrawGrayWnd

IMPLEMENT_DYNAMIC(CCustomDrawBlackWnd, CWnd)

CCustomDrawBlackWnd::CCustomDrawBlackWnd()
{
	m_bakeBrush.m_hObject = NULL;
	m_bakeBrush.CreateSolidBrush(UI_COLOR_BKGND_175);
}

CCustomDrawBlackWnd::~CCustomDrawBlackWnd()
{
	if( m_bakeBrush.m_hObject )
	{
		if( DeleteObject(m_bakeBrush.m_hObject) )
		{
			m_bakeBrush.m_hObject = NULL;
		}
	}
}


BEGIN_MESSAGE_MAP(CCustomDrawBlackWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CCustomDrawGrayWnd ��Ϣ�������

void CCustomDrawBlackWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	Wnd_DrawBK(&dc);
}

void CCustomDrawBlackWnd::Wnd_DrawBK(CDC * pDC)
{
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);

	//CMemDCEX memDC(pDC,rectClient);

	pDC->FillRect(rectClient,&m_bakeBrush);
}

BOOL CCustomDrawBlackWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Wnd_DrawBK(pDC);
	return CWnd::OnEraseBkgnd(pDC);
}

/*************************************************
Function:    	Wnd_SetBakeColor
Description:	���ô��ڵı�����ɫ
Input:			crBakeColor ���ñ�����ɫ
Output:      	
Return:			
*************************************************/
void CCustomDrawBlackWnd::Wnd_SetBakeColor(COLORREF crBakeColor)
{
	if( m_bakeBrush.m_hObject )
	{
		if( DeleteObject(m_bakeBrush.m_hObject) )
		{
			m_bakeBrush.m_hObject = NULL;
		}
	}

	m_bakeBrush.CreateSolidBrush(crBakeColor);
}

BOOL CCustomDrawBlackWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
			{
				//����Enter��
				if (pMsg->wParam == VK_RETURN)
				{
					return TRUE;
				}
				//����Esc��
				if (pMsg->wParam == VK_ESCAPE)
				{
					return TRUE;
				}
			}
			break;
		case WM_SYSKEYDOWN:
			{
				//����Alt + F4
				if (pMsg->wParam == VK_F4)
				{
					return TRUE;
				}
			}
			break;
		default:
			break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}
