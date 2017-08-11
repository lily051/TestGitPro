// CenterProcessorCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "CenterProcessorCfg.h"
#include "./Markup/Markup.h"
#include "Folder.h"
#include "XMLFunction.h"


// return 1 or 0
int is_valid_ip(const char *ip_str) 
{
	char temp[31];
	int a,b,c,d; 
	if(sscanf(ip_str, "%d.%d.%d.%d ",&a,&b,&c,&d)==4 
		&&   a>=0   &&   a<=255 
		&&   b>=0   &&   b<=255 
		&&   c>=0   &&   c<=255   
		&&   d>=0   &&   d<=255)    
	{    
		sprintf(temp, "%d.%d.%d.%d",a,b,c,d);    //把格式化的数据写入字符串temp     
		if(strcmp(temp,ip_str)==0)     
		{    
			return 1;     
		}     
		else    
		{    
			return 0;
		}    
	}    
	else     
	{    
		return 0; 
	}    
}

// CCenterProcessorCfg 对话框

IMPLEMENT_DYNAMIC(CCenterProcessorCfg, CDialog)

CCenterProcessorCfg::CCenterProcessorCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CCenterProcessorCfg::IDD, pParent)
	, m_bMove(FALSE)
{
	m_brushBkgnd.CreateSolidBrush(RGB(/*203*/227, /*203*/227, /*207*/227));
}

CCenterProcessorCfg::~CCenterProcessorCfg()
{
}

void CCenterProcessorCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_ADDR_ALARM_IP, m_alramIPCtrl);
	DDX_Control(pDX, IDC_IP_ADDR_ALARM_PORT, m_editAlarmPort);
	DDX_Control(pDX, IDC_IP_ADDR_LOCAL_IP, m_localIPCtrl);
	DDX_Control(pDX, IDC_IP_ADDR_LOCAL_PORT, m_editLocalPort);
}


BEGIN_MESSAGE_MAP(CCenterProcessorCfg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CENTER_CFG_SAVE, &CCenterProcessorCfg::OnBnClickedBtnCenterCfgSave)
	ON_BN_CLICKED(IDC_BTN_CENTER_CFG_RESET, &CCenterProcessorCfg::OnBnClickedBtnCenterCfgReset)
END_MESSAGE_MAP()


// CCenterProcessorCfg 消息处理程序

BOOL CCenterProcessorCfg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ReadConfig();
	InitDlg();
	m_bMove = TRUE;

	return TRUE;
}

// 背景画刷
HBRUSH CCenterProcessorCfg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor != CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brushBkgnd;
	}

	return hbr;
}

// 初始化配置界面
BOOL CCenterProcessorCfg::InitDlg(void)
{
	m_alramIPCtrl.SetWindowText(m_sOldAlramIP);
	m_editAlarmPort.SetWindowText(m_sOldAlarmPort);
	m_localIPCtrl.SetWindowText(m_sOldlocalIP);
	m_editLocalPort.SetWindowText(m_sOldLocalPort);
	UpdateData();
	
	return TRUE;
}

//读取配置文件
BOOL CCenterProcessorCfg::ReadConfig()
{
	CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + SVC_CFG_FILE);
	if (strcmp(fileContent, _T("")) == 0)
	{
		BuildDefaultFile();
		fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + SVC_CFG_FILE);
	}

	CMarkup xmlInfo;
	xmlInfo.SetDoc(fileContent);

	if (xmlInfo.FindElem(XML_CENTER_PROCESSOR_ROOT) && xmlInfo.IntoElem())
	{
		// ALARM IP
		if (xmlInfo.FindElem(XML_ALARM_SERVER))
		{
			m_sOldAlramIP = xmlInfo.GetData();
		}

		// ALARM PORT
		if (xmlInfo.FindElem(XML_ALARM_PORT))
		{
			m_sOldAlarmPort = xmlInfo.GetData();
		}

		// LOCAL IP
		if (xmlInfo.FindElem(XML_LOCAL_IP))
		{
			m_sOldlocalIP = xmlInfo.GetData();
		}

		// LOCAL PORT
		if (xmlInfo.FindElem(XML_LOCAL_PORT))
		{
			m_sOldLocalPort = xmlInfo.GetData();
		}

		InitDlg();

		return TRUE;
	}

	return FALSE;
}

void CCenterProcessorCfg::BuildDefaultFile()
{
	CMarkup xmlInfo;
	{
		// 构建XML报文
		xmlInfo.SetDoc(XML_HEAD);

		xmlInfo.AddElem(XML_CENTER_PROCESSOR_ROOT);
		xmlInfo.IntoElem();

		// ALARM PORT
		xmlInfo.AddElem(XML_ALARM_SERVER);
		xmlInfo.SetData(XML_DEFAULT_ALARM_SERVER);

		// ALARM PORT
		xmlInfo.AddElem(XML_ALARM_PORT);
		xmlInfo.SetData(XML_DEFAULT_ALARM_PORT);

		// LOCAL IP
		xmlInfo.AddElem(XML_LOCAL_IP);
		xmlInfo.SetData(XML_DEFAULT_LOCAL_IP);

		// LOCAL PORT
		xmlInfo.AddElem(XML_LOCAL_PORT);
		xmlInfo.SetData(XML_DEFAULT_LOCAL_PORT);
	}

	// XML报文保存到文件中
	CStringA strCfgFilePath(CFolder::GetExecutionRoot() + SVC_CFG_FILE);
	CStringA strContent(xmlInfo.GetDoc());
	if(!CFolder::SaveToUTF8File(strContent.GetBuffer(0), strCfgFilePath.GetBuffer(0)))
	{
		VIEWMSGLOG_ERROR("服务配置保存失败");
	}
	strCfgFilePath.ReleaseBuffer();
	strContent.ReleaseBuffer();
}

BOOL CCenterProcessorCfg::SaveConfig()
{
	CString csAlarmIP,csAlarmPort,csLocalIP,csLocalPort;
	m_alramIPCtrl.GetWindowText(csAlarmIP);
	m_editAlarmPort.GetWindowText(csAlarmPort);
	m_localIPCtrl.GetWindowText(csLocalIP);
	m_editLocalPort.GetWindowText(csLocalPort);

	CMarkup xmlInfo;
	{
		// 构建XML报文
		xmlInfo.SetDoc(XML_HEAD);

		xmlInfo.AddElem(XML_CENTER_PROCESSOR_ROOT);
		xmlInfo.IntoElem();

		// ALARM PORT
		xmlInfo.AddElem(XML_ALARM_SERVER);
		xmlInfo.SetData(csAlarmIP);

		// ALARM PORT
		xmlInfo.AddElem(XML_ALARM_PORT);
		xmlInfo.SetData(csAlarmPort);

		// LOCAL IP
		xmlInfo.AddElem(XML_LOCAL_IP);
		xmlInfo.SetData(csLocalIP);

		// LOCAL PORT
		xmlInfo.AddElem(XML_LOCAL_PORT);
		xmlInfo.SetData(csLocalPort);
	}

	BOOL bRet = FALSE;
	{
		// 将IP和PORT的XML报文保存到文件中
		CStringA strCfgFilePath(CFolder::GetExecutionRoot() + SVC_CFG_FILE);
		CStringA strContent(xmlInfo.GetDoc());
		if( CFolder::SaveToUTF8File(strContent.GetBuffer(0), strCfgFilePath.GetBuffer(0)) )
		{
			bRet = TRUE;

			m_sOldAlramIP = csAlarmIP;
			m_sOldAlarmPort = csAlarmPort;
			m_sOldlocalIP = csLocalIP;
			m_sOldLocalPort = csLocalPort;

			AfxMessageBox(_T("服务配置保存成功"));
		}
		else
		{
			MessageBox(_T("服务配置保存失败"), _T("错误"), MB_ICONERROR|MB_OK);
		}
		strCfgFilePath.ReleaseBuffer();
		strContent.ReleaseBuffer();
	}

	return bRet;
}

//检查参数合法性
BOOL CCenterProcessorCfg::CheckParamInvalid()
{
	CString csAlarmIP,csAlarmPort,csLocalIP,csLocalPort;
	m_alramIPCtrl.GetWindowText(csAlarmIP);
	csAlarmIP.Trim();
	m_editAlarmPort.GetWindowText(csAlarmPort);
	csAlarmPort.Trim();
	m_localIPCtrl.GetWindowText(csLocalIP);
	csLocalIP.Trim();
	m_editLocalPort.GetWindowText(csLocalPort);
	csLocalPort.Trim();

	//检查参数的合法性
	DWORD dwAlarmAddress;
	int nRet = m_alramIPCtrl.GetAddress(dwAlarmAddress);
	if (4 != nRet) 
	{
		MessageBox(_T("数据中心服务IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_alramIPCtrl.SetFocus();
		return FALSE;
	}
	if (0 == dwAlarmAddress)
	{
		MessageBox(_T("数据中心服务IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_alramIPCtrl.SetFocus();
		return FALSE;
	}

	DWORD dwLocalAddress;
	nRet = m_localIPCtrl.GetAddress(dwLocalAddress);
	if (4 != nRet) 
	{
		MessageBox(_T("本机IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_localIPCtrl.SetFocus();
		return FALSE;
	}
	if (0 == dwLocalAddress)
	{
		MessageBox(_T("本机IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_localIPCtrl.SetFocus();
		return FALSE;
	}

	if (!is_valid_ip(CStringA(csAlarmIP)))
	{
		MessageBox(_T("中心服务IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_alramIPCtrl.SetFocus();
		return FALSE;
	}

	if (csAlarmPort.IsEmpty())
	{
		MessageBox(_T("中心服务端口不能为空!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_editAlarmPort.SetFocus();
		m_editAlarmPort.SetSel(0, -1);
		return FALSE;
	}

	int nAlarmPort = _tstoi(csAlarmPort);
	if (nAlarmPort<=0 || nAlarmPort >=65535)
	{
		MessageBox(_T("中心服务端口范围为0~65535!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_editAlarmPort.SetFocus();
		m_editAlarmPort.SetSel(0, -1);
		return FALSE;
	}

	if (!is_valid_ip(CStringA(csLocalIP)))
	{
		MessageBox(_T("本机IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_localIPCtrl.SetFocus();
		return FALSE;
	}

	int nLocalPort = _tstoi(csLocalPort);
	if (nLocalPort<=0 || nLocalPort >=65535)
	{
		MessageBox(_T("本机端口范围为0~65535!"),_T("警告"),MB_ICONWARNING|MB_OK);
		m_editLocalPort.SetFocus();
		m_editLocalPort.SetSel(0, -1);
		return FALSE;
	}

	return TRUE;
}

void CCenterProcessorCfg::OnBnClickedBtnCenterCfgSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FALSE == CheckParamInvalid())
	{
		return;
	}

	SaveConfig();
}

void CCenterProcessorCfg::OnBnClickedBtnCenterCfgReset()
{
	// TODO: 在此添加控件通知处理程序代码
	InitDlg();
}

// 检测服务配置是否改变
BOOL CCenterProcessorCfg::CheckIsChanged(void)
{
	CString csAlarmIP,csAlarmPort,csLocalIP,csLocalPort;
	m_alramIPCtrl.GetWindowText(csAlarmIP);
	csAlarmIP.Trim();
	m_editAlarmPort.GetWindowText(csAlarmPort);
	csAlarmPort.Trim();
	m_localIPCtrl.GetWindowText(csLocalIP);
	csLocalIP.Trim();
	m_editLocalPort.GetWindowText(csLocalPort);
	csLocalPort.Trim();

	if ((strcmp(csAlarmIP, m_sOldAlramIP) != 0) 
		|| (strcmp(csAlarmPort, m_sOldAlarmPort) != 0)
		|| (strcmp(csLocalIP, m_sOldlocalIP) != 0) 
		|| (strcmp(csLocalPort, m_sOldLocalPort) != 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
