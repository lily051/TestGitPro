#include "StdAfx.h"
#include "FMessageBox.h"
#include "MsgBoxDlg.h"

FMessageBox::FMessageBox(void)
{
}

FMessageBox::~FMessageBox(void)
{
}

UINT FMessageBox::Show(LPCTSTR lpszText, LPCTSTR lpszCaption /*= NULL*/, UINT uType /*= MB_OK*/)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CMsgBoxDlg d;
    d.SetText(lpszCaption, lpszText, uType);
    return d.DoModal();
}

UINT FMessageBox::Show(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT uType, CWnd* pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMsgBoxDlg d(pParent);
	d.SetText(lpszCaption, lpszText, uType);
	return d.DoModal();
}
