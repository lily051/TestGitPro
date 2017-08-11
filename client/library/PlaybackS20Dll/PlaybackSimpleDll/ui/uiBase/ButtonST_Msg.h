#pragma once
#include "ButtonST.h"


//鼠标单击消息
#define WM_BTNMSG_CLICKED								(WM_USER + 1350)

class CButtonST_Msg : public CButtonST
{
	DECLARE_DYNAMIC(CButtonST_Msg)

public:
	CButtonST_Msg();
	virtual ~CButtonST_Msg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnClicked();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point ); 
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point ); 
};


