// SubDlgCfg.cpp : implementation file
//

#include "stdafx.h"
#include "ATPRemoteCtrl.h"
#include "SubDlgCfg.h"
#include "ATPRemoteCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgCfg dialog

CSubDlgCfg::CSubDlgCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgCfg)
	m_nCompress = 0;
	m_nFileFormat = 0;
	m_nStartCh = 0;
	m_nTotalCh = 0;
	m_szRebootTime = _T("");
	m_szRecPath = _T("");
	m_nCh = 0;
	m_nRecType = 0;
	m_nRecDir = 0;
	m_nMaxRecLen = 0;
	m_nRcvDTMFTime = 0;
	m_szDNS = _T("");
	m_szGateway = _T("");
	m_szIPAddr = _T("");
	m_szMask = _T("");
	m_szDateTime = _T("");
	m_szNewPwd = _T("");
	m_szOldPwd = _T("");
	m_nLogLevel = 0;
	m_nRebootHour = 0;
	m_nRebootMinute = 0;
	m_bAutoReboot = FALSE;
	m_nRecInt = 0;
	m_szStartDtmf = _T("");
	m_szStopDtmf = _T("");
	m_szBindPhone = _T("");
	m_uPort = 0;
	//}}AFX_DATA_INIT
	m_szDateTime = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
}


void CSubDlgCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgCfg)
	DDX_Text(pDX, IDC_EDIT_COMPRESS, m_nCompress);
	DDX_Text(pDX, IDC_EDIT_FILE_FORMAT, m_nFileFormat);
	DDX_Text(pDX, IDC_EDIT_START_CH, m_nStartCh);
	DDX_Text(pDX, IDC_EDIT_TOTAL_CH, m_nTotalCh);
	DDX_Text(pDX, IDC_EDIT_REBOOT_TIME, m_szRebootTime);
	DDX_Text(pDX, IDC_EDIT_REC_PATH, m_szRecPath);
	DDX_Text(pDX, IDC_EDIT_CH, m_nCh);
	DDX_Text(pDX, IDC_EDIT_REC_TYPE, m_nRecType);
	DDX_Text(pDX, IDC_EDIT_REC_DIR, m_nRecDir);
	DDX_Text(pDX, IDC_EDIT_MAXRECLEN, m_nMaxRecLen);
	DDX_Text(pDX, IDC_EDIT_RCV_DTMF_TIME, m_nRcvDTMFTime);
	DDX_Text(pDX, IDC_EDIT_DNS, m_szDNS);
	DDX_Text(pDX, IDC_EDIT_GATEWAY, m_szGateway);
	DDX_Text(pDX, IDC_EDIT_IP_ADDR, m_szIPAddr);
	DDX_Text(pDX, IDC_EDIT_MASK, m_szMask);
	DDX_Text(pDX, IDC_EDIT_DATETIME, m_szDateTime);
	DDX_Text(pDX, IDC_EDIT_NEW_PWD, m_szNewPwd);
	DDX_Text(pDX, IDC_EDIT_OLD_PWD, m_szOldPwd);
	DDX_Text(pDX, IDC_EDIT_LOG_LEVEL, m_nLogLevel);
	DDX_Text(pDX, IDC_EDIT_REBOOT_HOUR, m_nRebootHour);
	DDX_Text(pDX, IDC_EDIT_REBOOT_MIN, m_nRebootMinute);
	DDX_Check(pDX, IDC_CHECK_AUTO_REBOOT, m_bAutoReboot);
	DDX_Text(pDX, IDC_EDIT_REC_INT, m_nRecInt);
	DDX_Text(pDX, IDC_EDIT_START_DTMF, m_szStartDtmf);
	DDX_Text(pDX, IDC_EDIT_STOP_DTMF, m_szStopDtmf);
	DDX_Text(pDX, IDC_EDIT_BIND_PHONE, m_szBindPhone);
	DDX_Text(pDX, IDC_EDIT_WEBPORT, m_uPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgCfg, CDialog)
	//{{AFX_MSG_MAP(CSubDlgCfg)
	ON_BN_CLICKED(IDC_BUTTON_GetGlobalCfg, OnBUTTONGetGlobalCfg)
	ON_BN_CLICKED(IDC_BUTTON_SetGlobalCfg, OnBUTTONSetGlobalCfg)
	ON_BN_CLICKED(IDC_BUTTON_GetChannelCfg, OnBUTTONGetChannelCfg)
	ON_BN_CLICKED(IDC_BUTTON_SetChannelCfg, OnBUTTONSetChannelCfg)
	ON_BN_CLICKED(IDC_BUTTON_GetNetCfg, OnBUTTONGetNetCfg)
	ON_BN_CLICKED(IDC_BUTTON_SetNetCfg, OnBUTTONSetNetCfg)
	ON_BN_CLICKED(IDC_BUTTON_SetDateTime, OnBUTTONSetDateTime)
	ON_BN_CLICKED(IDC_BUTTON_SetAdminPassword, OnBUTTONSetAdminPassword)
	ON_BN_CLICKED(IDC_BUTTON_EnableDebugMsg, OnBUTTONEnableDebugMsg)
	ON_BN_CLICKED(IDC_BUTTON_SET_WEBPORT, OnButtonSetWebport)
	ON_BN_CLICKED(IDC_BUTTON_GET_WEBPORT, OnButtonGetWebport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgCfg message handlers
void CSubDlgCfg::OnTargetChanged(int nTarget)
{
	if (nTarget==0) // dll
	{
		GetDlgItem(IDC_BUTTON_EnableDebugMsg)->EnableWindow(TRUE);
	}
	else //soap
	{
		GetDlgItem(IDC_BUTTON_EnableDebugMsg)->EnableWindow(FALSE);
	}
}

void CSubDlgCfg::OnBUTTONGetGlobalCfg() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetGlobalCfg(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GetGlobalCfgResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetGlobalCfg(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			m_nCompress=p1.GetGlobalCfgReturn->icompress;
			m_nFileFormat=p1.GetGlobalCfgReturn->ifileformat;
			m_nStartCh = p1.GetGlobalCfgReturn->istartch;
			m_nTotalCh = p1.GetGlobalCfgReturn->itotalch;
			m_szRecPath = p1.GetGlobalCfgReturn->szrecpath;
			m_szRebootTime = p1.GetGlobalCfgReturn->szdevreboottime;
			m_bAutoReboot = p1.GetGlobalCfgReturn->cSetReboot;
			m_nRebootHour = p1.GetGlobalCfgReturn->iRebootHour;
			m_nRebootMinute = p1.GetGlobalCfgReturn->iRebootMin;

			UpdateData(FALSE);
		}
	}	
}

void CSubDlgCfg::OnBUTTONSetGlobalCfg() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = SetGlobalCfg(g_szDefaultServer, m_nCompress, m_nFileFormat, m_nStartCh,
			m_bAutoReboot, m_nRebootHour, m_nRebootMinute);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GlobalCfg p1;
		bool bRet;

		p1.icompress = m_nCompress;
		p1.ifileformat = m_nFileFormat;
		p1.istartch = m_nStartCh;
		p1.itotalch = m_nTotalCh;
		p1.szrecpath = m_szRecPath.GetBuffer(0);
		p1.szdevreboottime = m_szRebootTime.GetBuffer(0);
		p1.cSetReboot = m_bAutoReboot;
		p1.iRebootHour = m_nRebootHour;
		p1.iRebootMin = m_nRebootMinute;

		ret = TopDlg(GetParent())->m_pSynapiProxy->SetGlobalCfg(&p1, bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("SetGlobalCfg Success");
			else
				tmpMsg.Format("SetGlobalCfg Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgCfg::OnBUTTONGetChannelCfg() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetChannelCfg(g_szDefaultServer, m_nCh);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GetChannelCfgResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetChannelCfg(m_nCh, p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			m_nCh=p1.GetChannelCfgReturn->iChID;
			m_nRecType=p1.GetChannelCfgReturn->iRecType;
			m_nRecDir = p1.GetChannelCfgReturn->iRecDir;
			m_nMaxRecLen = p1.GetChannelCfgReturn->iMaxReclen;
			m_nRcvDTMFTime = p1.GetChannelCfgReturn->iRecvDtmfTime;
			m_nRecInt = p1.GetChannelCfgReturn->iLongRecInterval;
			m_szBindPhone = p1.GetChannelCfgReturn->szBindPhoneNum;
			m_szStartDtmf = p1.GetChannelCfgReturn->szStartDtmf;
			m_szStopDtmf = p1.GetChannelCfgReturn->szStopDtmf;
			UpdateData(FALSE);
		}
	}	
	
}

void CSubDlgCfg::OnBUTTONSetChannelCfg() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = SetChannelCfg(g_szDefaultServer, m_nCh, m_nRecInt, m_nMaxRecLen, 
			m_nRecDir, m_nRecType, m_nRcvDTMFTime, m_szBindPhone, m_szStartDtmf, m_szStopDtmf);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__ChannelCfg p1;
		bool bRet;
		
		p1.iChID = m_nCh;
		p1.iRecType = m_nRecType;
		p1.iRecDir = m_nRecDir;
		p1.iMaxReclen = m_nMaxRecLen;
		p1.iRecvDtmfTime = m_nRcvDTMFTime;
		p1.iLongRecInterval = m_nRecInt;
		p1.szBindPhoneNum = m_szBindPhone.GetBuffer(0);
		p1.szStartDtmf = m_szStartDtmf.GetBuffer(0);
		p1.szStopDtmf = m_szStopDtmf.GetBuffer(0);
		
		ret = TopDlg(GetParent())->m_pSynapiProxy->SetChannelCfg(&p1, bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("SetChannelCfg Success");
			else
				tmpMsg.Format("SetChannelCfg Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgCfg::OnBUTTONGetNetCfg() 
{
	MessageBox("N/A");
}

void CSubDlgCfg::OnBUTTONSetNetCfg() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = SetNetworkCfg(g_szDefaultServer, m_szIPAddr, m_szMask, m_szGateway, m_szDNS);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__NetCfg p1;
		bool bRet;
		
		p1.szIPAddr = m_szIPAddr.GetBuffer(0);
		p1.szMask = m_szMask.GetBuffer(0);
		p1.szGateway = m_szGateway.GetBuffer(0);
		p1.szDns = m_szDNS.GetBuffer(0);
		
		ret = TopDlg(GetParent())->m_pSynapiProxy->SetNetCfg(&p1, bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("SetNetCfg Success");
			else
				tmpMsg.Format("SetNetCfg Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgCfg::OnBUTTONSetDateTime() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = SetDateTime(g_szDefaultServer, m_szDateTime);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		bool bRet;
		ret = TopDlg(GetParent())->m_pSynapiProxy->SetDateTime(m_szDateTime.GetBuffer(0), bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("SetDateTime Success");
			else
				tmpMsg.Format("SetDateTime Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgCfg::OnBUTTONSetAdminPassword() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = SetAdminPassword(g_szDefaultServer, m_szOldPwd, m_szNewPwd);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		bool bRet;
		ret = TopDlg(GetParent())->m_pSynapiProxy->SetAdminPassword(m_szOldPwd.GetBuffer(0),m_szNewPwd.GetBuffer(0),bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("SetAdminPassword Success");
			else
				tmpMsg.Format("SetAdminPassword Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgCfg::OnBUTTONEnableDebugMsg() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = EnableDebugMsg(g_szDefaultServer, m_nLogLevel);
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

void CSubDlgCfg::OnButtonSetWebport() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = SetWebPort(g_szDefaultServer, m_uPort);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		bool bRet;
		ret = TopDlg(GetParent())->m_pSynapiProxy->SetWebPort(m_uPort,bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
			{
				tmpMsg.Format("SetWebPort Success");
				MessageBox(tmpMsg);
				CATPRemoteCtrlDlg * pParent =TopDlg(GetParent());
				pParent->ChangeWebPort(m_uPort);
			}
			else
			{
				tmpMsg.Format("SetWebPort Failed");
				MessageBox(tmpMsg);
			}
		}
	}	
	
}

void CSubDlgCfg::OnButtonGetWebport() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetWebPort(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		int nRet;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetWebPort(nRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			m_uPort=nRet;
			UpdateData(FALSE);
		}
	}		
}
