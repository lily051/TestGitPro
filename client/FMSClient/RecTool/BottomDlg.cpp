// BottomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RecTool.h"
#include "BottomDlg.h"


// CBottomDlg 对话框

IMPLEMENT_DYNAMIC(CBottomDlg, CDialog)

CBottomDlg::CBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBottomDlg::IDD, pParent)
{

}

CBottomDlg::~CBottomDlg()
{
}

void CBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBottomDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBottomDlg 消息处理程序

BOOL CBottomDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(31, 31, 31));  //设置背景色

	return TRUE;
}

void CBottomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CBottomDlg::InitCtrl(void)
{

}
BOOL CBottomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
