#pragma once


// CWnd_AP

class CWnd_AP : public CWnd
{
	DECLARE_DYNAMIC(CWnd_AP)

public:
	CWnd_AP();
	virtual ~CWnd_AP();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

protected:
	// Draw Window Background
	virtual void Wnd_DrawBK(CDC * pDC);
};


