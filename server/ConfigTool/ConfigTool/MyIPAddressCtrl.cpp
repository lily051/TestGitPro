// MyIPAddressCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MyIPAddressCtrl.h"


// CMyIPAddressCtrl

IMPLEMENT_DYNAMIC(CMyIPAddressCtrl, CIPAddressCtrl)

CMyIPAddressCtrl::CMyIPAddressCtrl()
{

}

CMyIPAddressCtrl::~CMyIPAddressCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyIPAddressCtrl, CIPAddressCtrl)    
END_MESSAGE_MAP()

BOOL CMyIPAddressCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
    if (HIWORD(wParam) == EN_CHANGE)
    {
        CEdit* pWnd = (CEdit*) CWnd::FromHandle((HWND)lParam);
        if (pWnd != NULL)
        {
            CString csTemp;
            pWnd->GetWindowText(csTemp);
            if (csTemp == _T(""))
            {
                return TRUE;
            }
            int nFiled = _tstoi(csTemp);
            if (nFiled > 255)
            {
                nFiled = 255;
            }
            if (nFiled < 0)
            {
                nFiled = 0;
            }
            CString csField;
            csField.Format(_T("%d"), nFiled);
            if (csField != csTemp)
            {
                pWnd->SetWindowText(csField);
                int nLength = csField.GetLength();
                pWnd->SetSel(nLength, nLength);
            }            
        }
        return TRUE;
    }
    return CIPAddressCtrl::OnCommand(wParam, lParam);
}
