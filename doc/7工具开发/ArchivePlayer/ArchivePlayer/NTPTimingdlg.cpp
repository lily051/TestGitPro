// NTPTimingdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ArchivePlayer.h"
#include "NTPTimingdlg.h"


// NTPTimingdlg 对话框

IMPLEMENT_DYNAMIC(NTPTimingdlg, CDialog)

NTPTimingdlg::NTPTimingdlg(CWnd* pParent /*=NULL*/)
	: CDialog(NTPTimingdlg::IDD, pParent)
{

}

NTPTimingdlg::~NTPTimingdlg()
{
}

void NTPTimingdlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS_NTP, m_editNTPip);
    DDX_Control(pDX, IDC_EDIT_NTP_PORT, m_editNTPport);
    DDX_Control(pDX, IDC_EDIT_NTP_INTERVAL, m_editNTPinterval);
}


BEGIN_MESSAGE_MAP(NTPTimingdlg, CDialog)
    ON_BN_CLICKED(IDOK, &NTPTimingdlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &NTPTimingdlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// NTPTimingdlg 消息处理程序

void NTPTimingdlg::setNTPedit(BOOL flag)
{
    m_editNTPinterval.EnableWindow(flag);
    m_editNTPip.EnableWindow(flag);
    m_editNTPport.EnableWindow(flag);
}

void NTPTimingdlg::OnBnClickedCheck()
{
    // TODO: 在此添加控件通知处理程序代码
/*    switch (this->IsDlgButtonChecked(IDC_CHECK__NTP_SWITCH))
    {
    case BST_CHECKED:
        setNTPedit(FALSE);
        break;
    case BST_UNCHECKED:
       setNTPedit(TRUE);
        break;
    case BST_INDETERMINATE:
        
        break;
    }*/
}

void NTPTimingdlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码

    BYTE nField0,nField1,nField2,nField3; 
    m_editNTPip.GetAddress(nField0,nField1,nField2,nField3);
    m_DevIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);

    m_editNTPport.GetWindowText(m_DevPort);
    m_editNTPinterval.GetWindowText(m_Interval);
    OnOK();
}

void NTPTimingdlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    OnCancel();
}
