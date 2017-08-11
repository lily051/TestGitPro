// SubDlgStatus.cpp : implementation file
//

#include "stdafx.h"
#include "ATPRemoteCtrl.h"
#include "SubDlgStatus.h"
#include "ATPRemoteCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgStatus dialog


CSubDlgStatus::CSubDlgStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgStatus)
	m_nCh = 0;
	//}}AFX_DATA_INIT
}


void CSubDlgStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgStatus)
	DDX_Text(pDX, IDC_EDIT_CH, m_nCh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgStatus, CDialog)
	//{{AFX_MSG_MAP(CSubDlgStatus)
	ON_BN_CLICKED(IDC_BUTTON_GetHardwareInfo, OnBUTTONGetHardwareInfo)
	ON_BN_CLICKED(IDC_BUTTON_GetChannelStatus, OnBUTTONGetChannelStatus)
	ON_BN_CLICKED(IDC_BUTTON_SYGetDiskFreeSpace, OnBUTTONSYGetDiskFreeSpace)
	ON_BN_CLICKED(IDC_BUTTON_GetLineVol, OnBUTTONGetLineVol)
	ON_BN_CLICKED(IDC_BUTTON_GetChannelRunningStatus, OnBUTTONGetChannelRunningStatus)
	ON_BN_CLICKED(IDC_BUTTON_GetDeviceTime, OnBUTTONGetDeviceTime)
	ON_BN_CLICKED(IDC_BUTTON_GetDevID, OnBUTTONGetDevID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgStatus message handlers

void CSubDlgStatus::OnTargetChanged(int nTarget)
{

}

void CSubDlgStatus::OnBUTTONGetHardwareInfo() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetHardwareInfo(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GetHardwareInfoResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetHardwareInfo(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("TotalChannel=%d,Type=%s,IP=%s,Gateway=%s\r\n"
				"Macaddress=%s,Mask=%s,DNS=%s,DevID=%s\r\n"
				"WebVersion=%s,RecSvrVersion=%s,SqlVersion=%s\r\nCpldVersion=%s\r\n"
				"kernelVersion=%s",
				p1.GetHardwareInfoReturn->TotalChannels,p1.GetHardwareInfoReturn->BoardType,
				p1.GetHardwareInfoReturn->IpAddress,p1.GetHardwareInfoReturn->Gateway,
				p1.GetHardwareInfoReturn->Macaddress,p1.GetHardwareInfoReturn->Mask,
				p1.GetHardwareInfoReturn->Dns,p1.GetHardwareInfoReturn->DevID,
				p1.GetHardwareInfoReturn->WebVersion,p1.GetHardwareInfoReturn->RecSvrVersion,
				p1.GetHardwareInfoReturn->SqlVersion,p1.GetHardwareInfoReturn->CpldVersion,
				p1.GetHardwareInfoReturn->kernelVersion);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
}

void CSubDlgStatus::OnBUTTONGetChannelStatus() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetChannelStatus(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GetChannelStatusResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetChannelStatus(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("Total=%d,Start=%d,status=%s",
			   p1.GetChannelStatusReturn->cTotalCh,p1.GetChannelStatusReturn->iStartCh,
			   p1.GetChannelStatusReturn->arrChStatus);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}	
}

void CSubDlgStatus::OnBUTTONSYGetDiskFreeSpace() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetDiskFreeSpaceB(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GetDiskFreeSpaceResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetDiskFreeSpace(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("Disk1:%s,type:%s,total/free=%d/%d\r\n"
				"Disk2:%s,type:%s,total/free=%d/%d\r\n"
				"Disk3:%s,type:%s,total/free=%d/%d",
				p1.GetDiskFreeSpaceReturn->szDiskName1, p1.GetDiskFreeSpaceReturn->szDiskType1, 
				p1.GetDiskFreeSpaceReturn->iDiskTotalSpace1, p1.GetDiskFreeSpaceReturn->iDiskFreeSpace1,
				p1.GetDiskFreeSpaceReturn->szDiskName2, p1.GetDiskFreeSpaceReturn->szDiskType2, 
				p1.GetDiskFreeSpaceReturn->iDiskTotalSpace2, p1.GetDiskFreeSpaceReturn->iDiskFreeSpace2,
				p1.GetDiskFreeSpaceReturn->szDiskName3, p1.GetDiskFreeSpaceReturn->szDiskType3, 
				p1.GetDiskFreeSpaceReturn->iDiskTotalSpace3, p1.GetDiskFreeSpaceReturn->iDiskFreeSpace3
				);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}	
	
}

void CSubDlgStatus::OnBUTTONGetLineVol() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetLineVol(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GetLineVolResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetLineVol(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("Total=%d,Start=%d,status=%s",
				p1.GetLineVolReturn->iTotalCh,p1.GetLineVolReturn->iStartCh,
				p1.GetLineVolReturn->szChVol);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
}

void CSubDlgStatus::OnBUTTONGetChannelRunningStatus() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetChannelRunningStatus(g_szDefaultServer, m_nCh);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__GetChannelRunningStatusResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetChannelRunningStatus(m_nCh, p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("ch=%d, status=%d, rec dir=%d, dtmf=%s\r\n"
				"rec len=%d, ring time=%s, ring cnt=%d\r\ncaller=%s, callee=%s",
				p1.GetChannelRunningStatusReturn->cChID,p1.GetChannelRunningStatusReturn->cChStatus,
				p1.GetChannelRunningStatusReturn->cRecDir, p1.GetChannelRunningStatusReturn->szDtmf,
				p1.GetChannelRunningStatusReturn->iRecLen, p1.GetChannelRunningStatusReturn->szRingTime,
				p1.GetChannelRunningStatusReturn->cRingCnt,
				p1.GetChannelRunningStatusReturn->szCallerID, p1.GetChannelRunningStatusReturn->szCalleeID);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}	
}

void CSubDlgStatus::OnBUTTONGetDeviceTime() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetDeviceTime(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		char * pszTime;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetDeviceTime(pszTime);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("%s",	pszTime);
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgStatus::OnBUTTONGetDevID() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = GetDeviceID(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		char * p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->GetDevID(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("%s",	p1);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}	
}
