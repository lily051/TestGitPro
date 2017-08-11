#pragma once


// CLocalWndTitle

class CLocalWndTitle : public CWnd
{
	DECLARE_DYNAMIC(CLocalWndTitle)

public:
	CLocalWndTitle();
	virtual ~CLocalWndTitle();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	//设置文本
	void SetText(CString strText);
	//刷新
	void Update();
private:
	COLORREF m_clrBk;    //背景色
	CString m_strText;   //标题文字
	COLORREF m_clrText;  //标题文字颜色
	CFont m_fTextFont;   //标题文字字体
};


