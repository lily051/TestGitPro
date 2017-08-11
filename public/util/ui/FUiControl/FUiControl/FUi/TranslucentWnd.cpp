// TranslucentWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TranslucentWnd.h"


// CTranslucentWnd

IMPLEMENT_DYNAMIC(CTranslucentWnd, CWnd)

CTranslucentWnd::CTranslucentWnd()
{

}

CTranslucentWnd::~CTranslucentWnd()
{
}


BEGIN_MESSAGE_MAP(CTranslucentWnd, CWnd)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CTranslucentWnd ��Ϣ�������



BOOL CTranslucentWnd::CreateWnd(CWnd* pParent)
{
    // ������ע�ᴰ��
    CString strWndClass = AfxRegisterWndClass(CS_DBLCLKS, AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
        (HBRUSH)(COLOR_3DFACE + 1), AfxGetApp()->LoadStandardIcon(IDI_WINLOGO));

    // ����֧�ְ�͸������֧�� WS_CHILD���
    CRect rcWnd(0, 0, 50, 50);
    BOOL bRet = CreateEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, strWndClass, _T("TranslucentWnd"), 
        WS_POPUP| WS_EX_TOOLWINDOW | WS_VISIBLE, rcWnd, NULL, 0);

    // ���ð�͸������Ҫ���ڴ��� WS_EX_LAYERED ���
    SetLayeredWindowAttributes(0, 50, LWA_ALPHA);

    return bRet;
}

BOOL CTranslucentWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CRect rcWnd;
    GetClientRect(rcWnd);

    pDC->FillSolidRect(rcWnd, RGB(80,80,80));
    return TRUE;
}
