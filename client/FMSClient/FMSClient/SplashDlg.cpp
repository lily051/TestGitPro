// SplashDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FMSClient.h"
#include "SplashDlg.h"


// CSplashDlg 对话框

DWORD _stdcall ThrobberThreadProc(void * pParam)
{
	CSplashDlg * pSplashDlg = (CSplashDlg*)pParam;
	if (NULL == pSplashDlg)
	{
		return 0;
	}
	pSplashDlg->ThrobberProc();

	return 0;
}

IMPLEMENT_DYNAMIC(CSplashDlg, CDialog)

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
	, m_pImgBkg(NULL)
	, m_strTitle(_T(""))
	, m_strText(_T(""))
	, m_strVer(_T(""))
	, m_hExitThread(0)
    , m_hThread(0)
{
	LOGFONT lfFont = {0};
	_tcscpy_s(lfFont.lfFaceName, _T("微软雅黑"));
	lfFont.lfHeight = 42;
	lfFont.lfWeight = FW_NORMAL;
	RenderEngine->AddFont(YAHEI_42_N, lfFont);
}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSplashDlg 消息处理程序

BOOL CSplashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	//加载图片资源
	m_pImgBkg = FUiGetRes(_T("splash"));
	if (m_pImgBkg->GetLastStatus() != Ok)
	{
		return FALSE;
	}

	//调整窗口大小
	SetWindowPos(&CWnd::wndTop, 0, 0, m_pImgBkg->GetWidth(), m_pImgBkg->GetHeight(), SWP_NOMOVE);
	CenterWindow();
	GetClientRect(m_rcWnd);

	if (NULL == m_hExitThread)
	{
		m_hExitThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	if (NULL == m_hThread)
	{
		m_hThread = CreateThread(NULL, 0, ThrobberThreadProc, this, 0, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CSplashDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CSplashDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	FUi::CMemDCEX memDC(&dc, m_rcWnd);
	SplashStep(&memDC);
}

void CSplashDlg::SetText(const CString& strTitle, const CString& strVer, const CString& strText)
{
	m_strTitle = strTitle;
	m_strVer.Format(_T("Copyright(C) 2016 All rights reserved(v%s)"), strVer);
	m_strText = strText;
}

void CSplashDlg::SetText(const CString& strText)
{
	m_strText = strText;
}

void CSplashDlg::Show()
{
	Create(IDD);
	ShowWindow(SW_SHOW);
	UpdateWindow();

	SetForegroundWindow();
}

void CSplashDlg::Hide()
{
	if (m_hExitThread)
	{
		SetEvent(m_hExitThread);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hExitThread);
		CloseHandle(m_hThread);

		m_hExitThread = NULL;
		m_hThread = NULL;
	}

	DestroyWindow();
}

void CSplashDlg::SplashStep(CDC* pDC)
{
	if (NULL == pDC)
	{
		return;
	}

	//画背景
	DrawBkg(pDC);

	CString strPath = theApp.GetAppPath() + _T("\\Config\\Client.ini");
	int nTemp = GetPrivateProfileInt(_T("CLIENT"), _T("SplashColor"), RGB(237,237,237), strPath);

	//画文字
	CRect rcTmp(m_rcWnd);
	rcTmp.left = 20;
	rcTmp.right = m_rcWnd.right - 20;
	rcTmp.bottom = m_rcWnd.bottom - 36;
	pDC->SelectObject(RenderEngine->GetFont(YAHEI_42_N));

	CRect rcCal(0, 0, rcTmp.Width(), rcTmp.Height());
	//计算多行文字所需高度
	int nHeight = pDC->DrawText(m_strTitle, rcCal, DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
	if (nHeight > 41)
	{
		LOGFONT lf1 = {0};
		_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
		lf1.lfHeight = 32;
		lf1.lfWeight = FW_NORMAL;

		CFont font;
		font.CreateFontIndirect(&lf1);
		pDC->SelectObject(&font);
	}

	RenderEngine->DrawTextX(pDC, m_strTitle, rcTmp, nTemp, DT_CENTER | DT_SINGLELINE |DT_END_ELLIPSIS | DT_VCENTER);
	
	rcTmp = m_rcWnd;
	rcTmp.top = m_rcWnd.bottom / 2;
	rcTmp.bottom = rcTmp.top + 48;
	pDC->SelectObject(RenderEngine->GetFont(YAHEI_18_N));
	RenderEngine->DrawTextX(pDC, m_strText, rcTmp, nTemp, DT_CENTER | DT_SINGLELINE | DT_BOTTOM);

	rcTmp.top = m_rcWnd.bottom - 26;
	rcTmp.bottom = rcTmp.top + 20;
	rcTmp.right = m_rcWnd.right - 16;
	pDC->SelectObject(RenderEngine->GetFont(YAHEI_18_N));
	RenderEngine->DrawTextX(pDC, m_strVer, rcTmp, nTemp, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);

	return;
}

void CSplashDlg::DrawBkg(CDC* pDC)
{
	RenderEngine->DrawImageX(pDC, m_pImgBkg, m_rcWnd, 0);
}

void CSplashDlg::ThrobberProc()
{
	while(WAIT_OBJECT_0 != WaitForSingleObject(m_hExitThread, 60))
	{
		CDC * pDC = GetDC();
		FUi::CMemDCEX memDC(pDC, m_rcWnd);
		SplashStep(&memDC);
		ReleaseDC(pDC);
	}
}