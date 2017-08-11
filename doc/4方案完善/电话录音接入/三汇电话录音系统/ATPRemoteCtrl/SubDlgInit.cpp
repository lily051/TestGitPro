// SubDlgInit.cpp : implementation file
//

#include "stdafx.h"
#include "ATPRemoteCtrl.h"
#include "SubDlgInit.h"
#include "ATPRemoteCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgInit dialog

CSubDlgInit::CSubDlgInit(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgInit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgInit)
	m_szUser = _T("admin");
	m_szPassword = _T("admin");
	m_wLocalPort = 6660;
	m_szLocalIP = _T("201.123.112.131");
	m_szServerIP = _T("201.123.112.5");
	m_szSetParam1 = _T("");
	m_uWebPort = 80;
	//}}AFX_DATA_INIT
}


void CSubDlgInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgInit)
	DDX_Text(pDX, IDC_EDIT_USER, m_szUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_szPassword);
	DDX_Text(pDX, IDC_EDIT_LOCAL_PORT, m_wLocalPort);
	DDX_Text(pDX, IDC_EDIT_LOCAL_IP, m_szLocalIP);
	DDX_Text(pDX, IDC_EDIT_SERVER_IP, m_szServerIP);
	DDX_Text(pDX, IDC_EDIT_SET_PARAM1, m_szSetParam1);
	DDX_Text(pDX, IDC_EDIT_WEBPORT, m_uWebPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgInit, CDialog)
	//{{AFX_MSG_MAP(CSubDlgInit)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_SOAP_DEST, OnButtonSoapDest)
	ON_BN_CLICKED(IDC_BUTTON_SetNetTimeout, OnBUTTONSetNetTimeout)
	ON_BN_CLICKED(IDC_BUTTON_SETWEBPORT, OnButtonSetwebport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgInit message handlers
CString tmpMsg;

void CSubDlgInit::OnTargetChanged(int nTarget)
{
	if (nTarget==0) // dll
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SetNetTimeout)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON_SETWEBPORT)->EnableWindow(FALSE);
		//GetDlgItem(IDC_EDIT_WEBPORT)->EnableWindow(FALSE);
	}
	else //soap
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SetNetTimeout)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_SETWEBPORT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EDIT_WEBPORT)->EnableWindow(TRUE);
	}
}

void CSubDlgInit::OnButtonConnect() 
{
	if (!UpdateData(TRUE)) return;
	int ret;
	OnButtonSoapDest();
	ret = Connect(m_szServerIP, m_szUser, m_szPassword, m_szLocalIP, m_wLocalPort);
	if (ret)
	{
		tmpMsg.Format("result=%d",ret);
		MessageBox(tmpMsg);
	}
}

void CSubDlgInit::OnButtonDisconnect() 
{
	if (!UpdateData(TRUE)) return;
	int ret;
	ret = Disconnect(m_szServerIP);
	if (ret)
	{
		tmpMsg.Format("result=%d",ret);
		MessageBox(tmpMsg);
	}
}

void CSubDlgInit::OnButtonSoapDest() 
{
	if (!UpdateData(TRUE)) return;
	CString str;
	int i;
	CATPRemoteCtrlDlg * pParent =TopDlg(GetParent());
	for (i=0; i<pParent->m_arrServer.GetSize(); i++)
	{
		if (m_szServerIP==pParent->m_arrServer[i])
		{
			if (m_uWebPort!=pParent->m_arrPort[i])
			{
				OnButtonSetwebport();				
			}
			return;
		}
	}
	strcpy(g_szDefaultServer, m_szServerIP);
	sprintf(g_szDefaultEndPoint, "http://%s:%d/server.php", m_szServerIP,m_uWebPort);

	i = pParent->m_arrServer.Add(m_szServerIP);
	pParent->m_arrPort.Add(m_uWebPort);
	str.Format("%s:%d", m_szServerIP, m_uWebPort);
	pParent->m_cbServerList.AddString(str);
	pParent->m_cbServerList.SetCurSel(i);
	pParent->m_nServerIndex = i;
	pParent->m_arrEndPoint.Add(g_szDefaultEndPoint);
	str.Format("Server Index=%d, IP=%s, EndPoint=%s", i+1, m_szServerIP, g_szDefaultEndPoint);
	pParent->AppendResult(str);
	pParent->UpdateData(FALSE);
}


void CSubDlgInit::OnBUTTONSetNetTimeout() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = SetNetTimeout(g_szDefaultServer, atoi(m_szSetParam1));
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		MessageBox("Web Service(soap) has not this function!");
	}	
}



void CSubDlgInit::OnButtonSetwebport() 
{
	CATPRemoteCtrlDlg * pParent =TopDlg(GetParent());
	if (pParent->m_cbServerList.GetCount()==0 || pParent->m_nServerIndex<0)
	{
		AfxMessageBox("Please set server IP first.");
		return;
	}
	if (!UpdateData(TRUE))
		return;
	pParent->ChangeWebPort(m_uWebPort);
}
