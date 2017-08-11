// SubDlgDvd.cpp : implementation file
//

#include "stdafx.h"
#include "ATPRemoteCtrl.h"
#include "SubDlgDvd.h"
#include "ATPRemoteCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgDvd dialog


CSubDlgDvd::CSubDlgDvd(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgDvd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgDvd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSubDlgDvd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgDvd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgDvd, CDialog)
	//{{AFX_MSG_MAP(CSubDlgDvd)
	ON_BN_CLICKED(IDC_BUTTON_CheckMediaConnected, OnBUTTONCheckMediaConnected)
	ON_BN_CLICKED(IDC_BUTTON_CheckMediaAvailable, OnBUTTONCheckMediaAvailable)
	ON_BN_CLICKED(IDC_BUTTON_WriteDVD, OnBUTTONWriteDVD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgDvd message handlers
void CSubDlgDvd::OnTargetChanged(int nTarget)
{
	if (nTarget==0) // dll
	{

	}
	else //soap
	{

	}
}

void CSubDlgDvd::OnBUTTONCheckMediaConnected() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = CheckMediaConnected(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		int p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->CheckMediaConnected(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("CheckMediaConnected response=%d", p1);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
}

void CSubDlgDvd::OnBUTTONCheckMediaAvailable() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = CheckMediaAvailable(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		int p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->CheckMediaAvailable(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("CheckMediaAvailable response=%d", p1);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
}

void CSubDlgDvd::OnBUTTONWriteDVD() 
{
	int ret;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		ret = WriteDVD(g_szDefaultServer);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		int p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->WriteDVD(p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("WriteDVD response=%d", p1);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
}
