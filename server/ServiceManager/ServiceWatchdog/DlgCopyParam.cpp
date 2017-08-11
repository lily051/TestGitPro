// DlgCopyParam.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCopyParam.h"
#include "ServiceWatchdogConfig.h"
#include "XMLFunction.h"
#include "./Markup/Markup.h"
#include "utils.h"

// CDlgCopyParam 对话框

IMPLEMENT_DYNAMIC(CDlgCopyParam, CDialog)

CDlgCopyParam::CDlgCopyParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCopyParam::IDD, pParent)
{
    m_bSelect = TRUE;
    m_nDCPort = 8849;
}

CDlgCopyParam::~CDlgCopyParam()
{
}

void CDlgCopyParam::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_listCtrlServerModule);
}


BEGIN_MESSAGE_MAP(CDlgCopyParam, CDialog)
    ON_BN_CLICKED(IDC_BTN_OK, &CDlgCopyParam::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgCopyParam::OnBnClickedBtnCancel)
    ON_BN_CLICKED(IDC_BTN_ALL_SEL, &CDlgCopyParam::OnBnClickedBtnAllSel)
END_MESSAGE_MAP()


// CDlgCopyParam 消息处理程序

BOOL CDlgCopyParam::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetDlgItemText(IDC_EDIT_IP, m_strDCIp);
    SetDlgItemInt(IDC_EDIT_PORT, m_nDCPort);
    SetDlgItemText(IDC_EDIT_LOCAL_IP, m_strLocalIP);

    m_listCtrlServerModule.SetColumnWidth(0,110);
    m_listCtrlServerModule.SetExtendedStyle(LVS_EX_CHECKBOXES);
    CString strCopyParam = XML_GetCopyParam();
    std::list<ServiceConfig>& listAllService = ServiceWatchdogConfig::Instance().m_lstAllService;
    for (std::list<ServiceConfig>::iterator itor = listAllService.begin();
        itor != listAllService.end(); itor++)
    {
        int nItem = m_listCtrlServerModule.GetItemCount();
        std::string strConfigToolExe = itor->m_svcPath + "\\" + itor->m_svcConfigToolName;
        CModuleVerInfo verInfo(strConfigToolExe.c_str());
        CString strFileVersion;
        verInfo.GetVerValue(_T("FileVersion"), strFileVersion);
        if (strFileVersion == strCopyParam)
        {
            m_listCtrlServerModule.InsertItem(nItem, itor->m_svcDisplayName.c_str());
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgCopyParam::OnBnClickedBtnOk()
{
    CString csDCIP;
    GetDlgItemText(IDC_EDIT_IP, csDCIP);
    int nDCPort = GetDlgItemInt(IDC_EDIT_PORT);
    CString csLocalIP;
    GetDlgItemText(IDC_EDIT_LOCAL_IP, csLocalIP);

    std::list<ServiceConfig>& listAllService = ServiceWatchdogConfig::Instance().m_lstAllService;

    BOOL m_bSaveData = FALSE;
    int nItem = m_listCtrlServerModule.GetItemCount();
    for (int iItem=0;iItem<nItem;++iItem)
    {
        if (m_listCtrlServerModule.GetCheck(iItem))
        {
            CString strText = m_listCtrlServerModule.GetItemText(iItem,0);
            for (std::list<ServiceConfig>::iterator itor = listAllService.begin();
                itor != listAllService.end(); itor++)
            {
                CString strModuleName(itor->m_svcDisplayName.c_str());
                if (strText == strModuleName)
                {
                    std::string strServerConfig = itor->m_svcPath + "\\ServiceConfig.xml";
                    CString strFile(strServerConfig.c_str());
                    SvaeCfg(strFile,csDCIP,nDCPort,csLocalIP);
                    m_bSaveData = TRUE;
                }
            }
        }
    }

    if (m_bSaveData)
    {
        MessageBox(_T("保存成功!"), _T("提示"), MB_ICONINFORMATION|MB_OK);
    }
}

void CDlgCopyParam::OnBnClickedBtnCancel()
{
    CDialog::OnCancel();
}

void CDlgCopyParam::OnBnClickedBtnAllSel()
{
    int nItem = m_listCtrlServerModule.GetItemCount();
    for (int i=0;i<nItem;++i)
    {
        m_listCtrlServerModule.SetCheck(i,m_bSelect);
    }
    m_bSelect = !m_bSelect;
}

void CDlgCopyParam::SvaeCfg(const CString& strFile,const CString& sIP, int nPort,const CString& sLocalIP)
{
    CMarkup xmlInfo;
    // 构建XML报文
    xmlInfo.SetDoc(XML_HEAD);

    xmlInfo.AddElem(_T("ROOT"));
    xmlInfo.IntoElem();
  
    xmlInfo.AddElem(_T("LOCAL_PARAM"));
    xmlInfo.AddAttrib(_T("ip"),sLocalIP);
    xmlInfo.AddAttrib(_T("loginname"),_T("admin"));
    xmlInfo.AddAttrib(_T("loginpwd"),_T("888"));

    xmlInfo.AddElem(_T("DATA_CENTER"));
    xmlInfo.AddAttrib(_T("ip"),sIP);
    xmlInfo.AddAttrib(_T("port"),nPort);

    xmlInfo.Save(strFile);
}