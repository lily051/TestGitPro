// MyEdit.cpp : ʵ���ļ�
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



// CIntEdit ��Ϣ�������
void CIntEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CIntEdit::OnEnChange()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CEdit::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
