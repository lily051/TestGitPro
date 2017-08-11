// TranslucentWnd.cpp : 实现文件
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



// CTranslucentWnd 消息处理程序



BOOL CTranslucentWnd::CreateWnd(CWnd* pParent)
{
    // 必须先注册窗口
    CString strWndClass = AfxRegisterWndClass(CS_DBLCLKS, AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
        (HBRUSH)(COLOR_3DFACE + 1), AfxGetApp()->LoadStandardIcon(IDI_WINLOGO));

    // 窗口支持半透明，不支持 WS_CHILD风格
    CRect rcWnd(0, 0, 50, 50);
    BOOL bRet = CreateEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, strWndClass, _T("TranslucentWnd"), 
        WS_POPUP| WS_EX_TOOLWINDOW | WS_VISIBLE, rcWnd, NULL, 0);

    // 设置半透明，需要窗口带有 WS_EX_LAYERED 风格
    SetLayeredWindowAttributes(0, 50, LWA_ALPHA);

    return bRet;
}

BOOL CTranslucentWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CRect rcWnd;
    GetClientRect(rcWnd);

    pDC->FillSolidRect(rcWnd, RGB(80,80,80));
    return TRUE;
}
