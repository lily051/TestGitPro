#include "StdAfx.h"
#include "Splash.h"
#include "FMSClient.h"

CSplash::CSplash(CSplashDlg * pDlg)
:m_pSplashDlg(pDlg)
{
	//CString strVer = fcU2T(theApp.m_pbm)
	CString strVer = _T("1.0.0.1");
	m_pSplashDlg->SetText(_T("理财客户端"), strVer, _T("Loading..."));
	m_pSplashDlg->Show();
}

CSplash::~CSplash(void)
{
	try
	{
		m_pSplashDlg->Hide();
		m_pSplashDlg = NULL;
	}
	catch (...)
	{
	}
}
