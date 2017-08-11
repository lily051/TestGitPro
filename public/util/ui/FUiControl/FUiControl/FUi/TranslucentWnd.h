#pragma once


// CTranslucentWnd

class CTranslucentWnd : public CWnd
{
	DECLARE_DYNAMIC(CTranslucentWnd)

public:
	CTranslucentWnd();
	virtual ~CTranslucentWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
    BOOL CreateWnd(CWnd* pParent);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


