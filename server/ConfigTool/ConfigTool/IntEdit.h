#pragma once


// CIntEdit

class CIntEdit : public CEdit
{
	DECLARE_DYNAMIC(CIntEdit)

public:
	CIntEdit();
	virtual ~CIntEdit();

    BOOL m_bIsInformed;

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnEnChange();
};


