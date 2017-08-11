// CustomDrawGrayWnd.cpp : 实现文件
//
/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			CustomDrawBlackWnd.cpp
Description:  		自画窗口
1.黑色背景
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-3-26     qmx    创建
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



// CCustomDrawGrayWnd 消息处理程序

void CCustomDrawBlackWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Wnd_DrawBK(pDC);
	return CWnd::OnEraseBkgnd(pDC);
}

/*************************************************
Function:    	Wnd_SetBakeColor
Description:	设置窗口的背景颜色
Input:			crBakeColor 设置背景颜色
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
	return CWnd::PreTranslateMessage(pMsg);
}
