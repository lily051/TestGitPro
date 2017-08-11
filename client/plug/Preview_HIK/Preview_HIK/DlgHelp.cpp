// DlgHelp.cpp : 实现文件
//

#include "stdafx.h"
#include "Preview_HIK.h"
#include "DlgHelp.h"


// CDlgHelp 对话框

IMPLEMENT_DYNAMIC(CDlgHelp, CDialog)

CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHelp::IDD, pParent)
{

}

CDlgHelp::~CDlgHelp()
{
}

void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHelp, CDialog)
END_MESSAGE_MAP()


// CDlgHelp 消息处理程序
