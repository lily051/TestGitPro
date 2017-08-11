// SubDlgOp.cpp : implementation file
//

#include "stdafx.h"
#include "ATPRemoteCtrl.h"
#include "SubDlgOp.h"
#include "ATPRemoteCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgOp dialog


CSubDlgOp::CSubDlgOp(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgOp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgOp)
	m_nCh = 0;
	m_szMonitor = _T("");
	m_nRebootMode = 0;
	m_szDelFile = _T("");
	m_szSaveFile = _T("");
	m_szUDFile = _T("");
	m_nStart = 0;
	m_nEnd = 0;
	m_szStartDate = _T("");
	m_szStopDate = _T("");
	m_szStartTime = _T("");
	m_szStopTime = _T("");
	//}}AFX_DATA_INIT
}


void CSubDlgOp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgOp)
	DDX_Text(pDX, IDC_EDIT_CH, m_nCh);
	DDX_Text(pDX, IDC_EDIT_MONITORIP, m_szMonitor);
	DDX_CBIndex(pDX, IDC_COMBO_RebootDevice, m_nRebootMode);
	DDX_Text(pDX, IDC_EDIT_DELFILE, m_szDelFile);
	DDX_Text(pDX, IDC_EDIT_SaveFile, m_szSaveFile);
	DDX_Text(pDX, IDC_EDIT_UploadFile, m_szUDFile);
	DDX_Text(pDX, IDC_EDIT_CH_START, m_nStart);
	DDX_Text(pDX, IDC_EDIT_CH_END, m_nEnd);
	DDX_Text(pDX, IDC_EDIT_DATE_BEGIN, m_szStartDate);
	DDX_Text(pDX, IDC_EDIT_DATE_END, m_szStopDate);
	DDX_Text(pDX, IDC_EDIT_TIME_BEGIN, m_szStartTime);
	DDX_Text(pDX, IDC_EDIT_TIME_END, m_szStopTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgOp, CDialog)
	//{{AFX_MSG_MAP(CSubDlgOp)
	ON_BN_CLICKED(IDC_BUTTON_StartMonitor, OnBUTTONStartMonitor)
	ON_BN_CLICKED(IDC_BUTTON_StopMonitor, OnBUTTONStopMonitor)
	ON_BN_CLICKED(IDC_BUTTON_RebootDevice, OnBUTTONRebootDevice)
	ON_BN_CLICKED(IDC_BUTTON_DelFile, OnBUTTONDelFile)
	ON_BN_CLICKED(IDC_BUTTON_DelFiles, OnBUTTONDelFiles)
	ON_BN_CLICKED(IDC_BUTTON_StopDelFile, OnBUTTONStopDelFile)
	ON_BN_CLICKED(IDC_BUTTON_UploadFile, OnBUTTONUploadFile)
	ON_BN_CLICKED(IDC_BUTTON_DownloadFile, OnBUTTONDownloadFile)
	ON_BN_CLICKED(IDC_BUTTON_StartUploadFile, OnBUTTONStartUploadFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgOp message handlers
void CSubDlgOp::OnTargetChanged(int nTarget)
{
	if (nTarget==0) // dll
	{
		//GetDlgItem(IDC_BUTTON_DelFiles)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_StopDelFile)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_UploadFile)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_StartUploadFile)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DownloadFile)->EnableWindow(TRUE);
	}
	else //soap
	{
		//GetDlgItem(IDC_BUTTON_DelFiles)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_StopDelFile)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UploadFile)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_StartUploadFile)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DownloadFile)->EnableWindow(FALSE);
	}
}

void CSubDlgOp::OnBUTTONStartMonitor() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = StartMonitor(g_szDefaultServer, m_nCh, m_szMonitor);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__MonitorInfo p1;
		bool bRet;
		p1.ChID=m_nCh;
		p1.szRemoteIP = m_szMonitor.GetBuffer(0);
		ret = TopDlg(GetParent())->m_pSynapiProxy->StartMonitor(&p1,bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("StartMonitor Success");
			else
				tmpMsg.Format("StartMonitor Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgOp::OnBUTTONStopMonitor() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = StopMonitor(g_szDefaultServer, m_szMonitor);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__MonitorInfo p1;
		bool bRet;
		p1.ChID=m_nCh;
		p1.szRemoteIP = m_szMonitor.GetBuffer(0);
		ret = TopDlg(GetParent())->m_pSynapiProxy->StopMonitor(&p1,bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("StopMonitor Success");
			else
				tmpMsg.Format("StopMonitor Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgOp::OnBUTTONRebootDevice() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = RebootDevice(g_szDefaultServer, eRebootCmd(m_nRebootMode));
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		bool bRet;
		ret = TopDlg(GetParent())->m_pSynapiProxy->RebootDevice(m_nRebootMode,bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("RebootDevice Success");
			else
				tmpMsg.Format("RebootDevice Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgOp::OnBUTTONDelFile() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = DelFile(g_szDefaultServer, m_szDelFile);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		bool bRet;
		ret = TopDlg(GetParent())->m_pSynapiProxy->DeleteFile(m_szDelFile.GetBuffer(0),bRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			if (bRet)
				tmpMsg.Format("DeleteFile Success");
			else
				tmpMsg.Format("DeleteFile Failed");
			MessageBox(tmpMsg);
		}
	}	
}

void CSubDlgOp::OnBUTTONDelFiles() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = DelFiles(g_szDefaultServer, m_nStart, m_nEnd, m_szStartDate, m_szStopDate,
			m_szStartTime, m_szStopTime);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		int nRet;
		ret = TopDlg(GetParent())->m_pSynapiProxy->DelRecordFile(
			m_szStartDate.GetBuffer(0),m_szStartTime.GetBuffer(0),
			m_szStopDate.GetBuffer(0), m_szStopTime.GetBuffer(0),
			m_nStart, m_nEnd, nRet);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
		}
		else
		{
			tmpMsg.Format("DeleteFile %d", nRet);
		}
		MessageBox(tmpMsg);
	}	
}

void CSubDlgOp::OnBUTTONStopDelFile() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = StopDelFile(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		MessageBox("soap has not this function");
	}	
}

void CSubDlgOp::OnBUTTONUploadFile() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = UploadFile(g_szDefaultServer, m_szUDFile);
		//if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		MessageBox("soap has not this function");
	}	
}

void CSubDlgOp::OnBUTTONDownloadFile() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = DownloadFile(g_szDefaultServer, m_szUDFile, m_szSaveFile);
		//if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		MessageBox("soap has not this function");
	}	
}

void CSubDlgOp::OnBUTTONStartUploadFile() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = StartUploadFile(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		MessageBox("soap has not this function");
	}	
}
