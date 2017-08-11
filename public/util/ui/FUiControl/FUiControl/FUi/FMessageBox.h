#pragma once

class AFX_EXT_CLASS FMessageBox
{
public:
    FMessageBox(void);
    ~FMessageBox(void);

public:
    static UINT Show(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT uType = MB_OK);

	static UINT Show(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT uType, CWnd* pParent);

};
