// DlgPic.cpp : 实现文件
//

#include "stdafx.h"
#include "Preview_HIK.h"
#include "DlgPic.h"
#include "ui/memdc.h"


// CDlgPic 对话框

IMPLEMENT_DYNAMIC(CDlgPic, CDialog)

CDlgPic::CDlgPic(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPic::IDD, pParent)
{

}

CDlgPic::~CDlgPic()
{
}

void CDlgPic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPic, CDialog)
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgPic 消息处理程序

BOOL CDlgPic::OnEraseBkgnd(CDC* pDC)
{
    CRect rcClient;
    GetClientRect(&rcClient);
    CMyMemDC memDC(pDC, NULL, TRUE);
    m_pic.DrawPicture(memDC->m_hDC,0, 0, rcClient.Width(), rcClient.Height());
    m_pic.DrawPicture(memDC->m_hDC,rcClient.Width(), 0, rcClient.Width()/2, rcClient.Height());

    return CDialog::OnEraseBkgnd(pDC);
}

void CDlgPic::SetPic(CString & sPicPath)
{
    m_pic.FreePicture();
    USES_CONVERSION;
    m_pic.LoadPicture(T2A(sPicPath));
    Invalidate();
}

void CDlgPic::OnDestroy()
{
    CDialog::OnDestroy();

     m_pic.FreePicture();
}
