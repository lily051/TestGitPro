// ButtonST_Msg.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "ButtonST_Msg.h"


// CButtonST_Msg

IMPLEMENT_DYNAMIC(CButtonST_Msg, CButtonST)
CButtonST_Msg::CButtonST_Msg()
{
}

CButtonST_Msg::~CButtonST_Msg()
{
}


BEGIN_MESSAGE_MAP(CButtonST_Msg, CButtonST)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	//	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CButtonST_Msg message handlers

BOOL CButtonST_Msg::OnClicked() 
{
	//把按钮的点击消息传给父窗口，包括按钮的id号
	::PostMessage(GetParent()->m_hWnd, WM_BTNMSG_CLICKED, (WPARAM)GetDlgCtrlID(),NULL);

	return TRUE;
}

//void CButtonPlayback::OnMove(int x, int y)
//{
//	CButtonST::OnMove(x, y);
//
//	//把按钮的点击消息传给父窗口，包括按钮的id号
//	::PostMessage(GetParent()->m_hWnd,WM_MOUSEMOVE,(WPARAM)GetDlgCtrlID(),NULL);
//
//
//	// TODO: Add your message handler code here
//}

void CButtonST_Msg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	::PostMessage(GetParent()->m_hWnd,WM_MOUSEMOVE,(WPARAM)GetDlgCtrlID(),NULL);

	CButtonST::OnMouseMove(nFlags, point);
}

void CButtonST_Msg::OnLButtonDown(UINT nFlags,CPoint point )
{
	CWnd *pParent = GetParent();
	int nctrlid = GetDlgCtrlID();
	::PostMessage(GetParent()->m_hWnd,WM_LBUTTONDOWN,(WPARAM)GetDlgCtrlID(),NULL);
	CButtonST::OnLButtonDown(nFlags,point);

}
void CButtonST_Msg::OnLButtonUp(UINT nFlags,CPoint point )
{
	::PostMessage(GetParent()->m_hWnd,WM_LBUTTONUP,(WPARAM)GetDlgCtrlID(),NULL);
	CButtonST::OnLButtonUp(nFlags,point);
}