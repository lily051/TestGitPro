// MyEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "IntEdit.h"

// CIntEdit

IMPLEMENT_DYNAMIC(CIntEdit, CEdit)

CIntEdit::CIntEdit() : m_bIsInformed(FALSE)
{

}

CIntEdit::~CIntEdit()
{
}


BEGIN_MESSAGE_MAP(CIntEdit, CEdit)
    ON_WM_CHAR()
    ON_CONTROL_REFLECT(EN_CHANGE, &CIntEdit::OnEnChange)
END_MESSAGE_MAP()



// CIntEdit 消息处理程序
void CIntEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CIntEdit::OnEnChange()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CEdit::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    CString csTemp;
    GetWindowText(csTemp);
    int intNo = _tstoi(csTemp);
    CString sNo;
    sNo.Format(_T("%d"),intNo);
    if (sNo != csTemp)
    {
        SetWindowText(sNo);
    }
}
