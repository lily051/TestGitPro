//lint -library
#include "stdafx.h"
#include "CToolTips.h"


CToolTips::CToolTips()
{
	m_hWnd = NULL;
	m_nToolID = 0;
	m_hDlg = NULL;
	m_bNormal = TRUE;
	m_pText = NULL;
}

CToolTips::~CToolTips()
{
	if (m_hWnd != NULL)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}

BOOL CToolTips::CreateToolTip(int toolID, HWND hDlg, TCHAR* pText, BOOL bNormal)
{
	if (!toolID || !hDlg || !pText)
	{
		return FALSE;
	}
	// Get the window of the tool.
	HWND hwndTool = ::GetDlgItem(hDlg, toolID);
	HINSTANCE hInst = GetModuleHandle(NULL);
	// Create the tooltip.
#ifndef TTS_BALLOON
#define TTS_BALLOON             0x40
#endif

	DWORD dwStyle;
	if (bNormal)
	{
		dwStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;  //普通方块型
	}
	else
	{
		dwStyle = WS_POPUP |TTS_ALWAYSTIP | TTS_BALLOON;//泡泡型
	}

	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, NULL, 
		hInst, NULL);

	if (!hwndTool || !hwndTip)
	{
		return FALSE;
	}                              

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hDlg;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pText;
	::SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
	::SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

	m_hWnd = hwndTip;
	m_nToolID = toolID;
	m_hDlg = hDlg;
	m_bNormal = bNormal;
	m_pText = pText;

	return TRUE;
}

BOOL CToolTips::UpdateToolTip(TCHAR* pText)
{
	if (m_nToolID == 0 || m_hDlg == NULL)
	{
		return FALSE;
	}

	if (NULL != m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	return CreateToolTip(m_nToolID, m_hDlg, pText, m_bNormal);
}

BOOL CToolTips::UpdateToolTip(HWND hDlg)
{
	if (NULL == m_pText)
	{
		return FALSE;
	}

	if (NULL != m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	return CreateToolTip(m_nToolID, hDlg, m_pText, m_bNormal);
}

void CToolTips::RelayEvent(MSG* pMsg)
{
	if (NULL == m_hWnd)
	{
		return;
	}
	
	::SendMessage(m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)pMsg);
}