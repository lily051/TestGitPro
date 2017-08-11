// IntelDlgEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RPConfig.h"
#include "IntelDlgEx.h"
#include "IntelDlg.h" //ʹ����Դ

// CIntelDlgEx �Ի���

IMPLEMENT_DYNAMIC(CIntelDlgEx, CDialog)

CIntelDlgEx::CIntelDlgEx(CWnd* pParent /*=NULL*/)
	: CDialog(CIntelDlgEx::IDD, pParent)
{

}

CIntelDlgEx::~CIntelDlgEx()
{

}

void CIntelDlgEx::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_VCA_CHAN, m_comDevChan);
    DDX_Control(pDX, IDC_VCACFGOCX1, m_vcaCfgDll);
}

BOOL CIntelDlgEx::PreTranslateMessage(MSG* pMsg)
{
    switch (pMsg->message)
    {
    case WM_KEYDOWN:
        {
            //����Enter����
            if (VK_RETURN == pMsg->wParam)
            {
                return TRUE;
            }
            //����Esc��
            if (VK_ESCAPE == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    case WM_SYSKEYDOWN:
        {
            //����Alt + F4
            if (VK_F4  == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    default:
        break;
    }

    return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CIntelDlgEx, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_VCA_DEV, &CIntelDlgEx::OnBnClickedButtonVcaDev)
    ON_BN_CLICKED(IDC_BUTTON_VCA_CHAN, &CIntelDlgEx::OnBnClickedButtonVcaChan)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CIntelDlgEx ��Ϣ�������

void CIntelDlgEx::OnBnClickedButtonVcaDev()
{
    m_vcaCfgDll.VcaCfgInitEx(0,m_struFunParam.sDevIP,m_struFunParam.nDevPort,0,m_struFunParam.sDevUser,m_struFunParam.sDevPWD);
    m_vcaCfgDll.VcaCfgDevRes2UiEx();
    m_vcaCfgDll.VcaCfgFiniEx();
}

void CIntelDlgEx::OnBnClickedButtonVcaChan()
{
    int nDevChanNo = m_comDevChan.GetCurSel() + 1;
    if(nDevChanNo>0)
    {
        m_vcaCfgDll.VcaCfgInitEx(0,m_struFunParam.sDevIP,m_struFunParam.nDevPort,nDevChanNo,m_struFunParam.sDevUser,m_struFunParam.sDevPWD);
        m_vcaCfgDll.VcaCfgChan2UiEx();
        m_vcaCfgDll.VcaCfgFiniEx();
    }
}

void CIntelDlgEx::SetDevInfo( FUN_PARAM struFunParam )
{
     m_struFunParam = struFunParam;
     SetDlgItemText(IDC_EDIT_DEVIP,m_struFunParam.sDevIP);
     CString strPort;
     strPort.Format(_T("%d"),m_struFunParam.nDevPort);
     SetDlgItemText(IDC_EDIT_DEVPORT,strPort);
     SetDlgItemText(IDC_EDIT_DEVUSER,m_struFunParam.sDevUser);
     SetDlgItemText(IDC_EDIT_DEVPWD,m_struFunParam.sDevPWD);
     m_comDevChan.ResetContent();
     for (int i =0;i<m_struFunParam.nChannel;i++)
     {
         CString strChanName;
         strChanName.Format(_T("ͨ��%d"),i+1);
         m_comDevChan.AddString(strChanName);
     }
     if (m_struFunParam.nChannel>0)
     {
         m_comDevChan.SetCurSel(0);
     }
}


