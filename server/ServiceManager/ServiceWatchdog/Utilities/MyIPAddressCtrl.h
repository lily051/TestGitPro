#pragma once


// CMyIPAddressCtrl

class CMyIPAddressCtrl : public CIPAddressCtrl
{
	DECLARE_DYNAMIC(CMyIPAddressCtrl)

public:
	CMyIPAddressCtrl();
	virtual ~CMyIPAddressCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


