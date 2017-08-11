/**
*   @file   SvcCfg.cpp
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  服务配置的实现
*  
*   @author mzt
*   @date   2013/07/01
*
*   @note   <create>    <2013/07/01>
*
*   @warning
*/

// SvcCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "SvcCfg.h"
#include "ServiceWatchdog.h"
#include "MainDialog.h"

#include "./Markup/Markup.h"
#include "Folder.h"

#include "XMLFunction.h"
#include "DlgCopyParam.h"

// CSvcCfg 对话框

IMPLEMENT_DYNAMIC(CSvcCfg, CDialog)

CSvcCfg::CSvcCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CSvcCfg::IDD, pParent)
	, m_bInit(FALSE)
{
    m_brushBkgnd.CreateSolidBrush(RGB(/*203*/227, /*203*/227, /*207*/227));
}

CSvcCfg::~CSvcCfg()
{
}

void CSvcCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSvcCfg, CDialog)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSvcCfg 消息处理程序

// 初始化Dialog
BOOL CSvcCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    //InitDlg();

    return TRUE;
}

// 屏蔽Esc、Enter、空格、F1键
BOOL CSvcCfg::PreTranslateMessage(MSG* pMsg)
{
    if ((WM_KEYDOWN == pMsg->message) &&
        ((VK_ESCAPE == pMsg->wParam )|| (VK_RETURN == pMsg->wParam) || (VK_SPACE == pMsg->wParam) || (VK_F1 == pMsg->wParam)))
    {
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

// 检查IP地址是否合法
BOOL CSvcCfg::CheckIPIsValid(const char* pStrIP)
{
    if (NULL == pStrIP)
    {
        return FALSE;
    }

    int a, b, c, d;
    if ((4 == sscanf_s(pStrIP, _T("%d.%d.%d.%d"), &a, &b, &c, &d)) &&
        ((a >= 0) && (a <= 255)) &&
        ((b >= 0) && (b <= 255)) &&
        ((c >= 0) && (c <= 255)) &&
        ((d >= 0) && (d <= 255)) )
    {
        CStringA tmp;
        tmp.Format(_T("%d.%d.%d.%d"), a, b, c, d);
        if (tmp.Compare(pStrIP) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

// 获取本机IP地址
CString CSvcCfg::GetLocalIPAddr(void)
{
    char      name[255];
    PHOSTENT  hostinfo;
    if(gethostname(name,sizeof(name)) == 0)
    {   
        if((hostinfo = gethostbyname(name)) != NULL)
        {
            CString csLocalIP = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
            return csLocalIP;
        }
    }

    return _T("");
}

// 背景画刷
HBRUSH CSvcCfg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
BOOL CSvcCfg::InitDlg(int nProcessor)
{
	CRect winRect;
	::GetWindowRect(this->GetSafeHwnd(), winRect);
	ScreenToClient(winRect);
	if (0 == nProcessor && !m_bInit)
	{
		m_dlgCollectorProcessorCfg.Create(IDD_DLG_COLLECTOR_PROCESSOR_CFG, this);
		m_dlgCollectorProcessorCfg.MoveWindow(winRect);
		m_dlgCollectorProcessorCfg.ShowWindow(SW_SHOW);
		m_bInit = TRUE;
	}
	else if (0 == nProcessor && m_bInit)
	{
		m_dlgCollectorProcessorCfg.ShowWindow(SW_SHOW);
		m_bInit = TRUE;
	}
	else if ((2 == nProcessor || 1 == nProcessor) && !m_bInit)
	{
		m_dlgCenterProcessorCfg.Create(IDD_DLG_CENTER_PROCESSOR_CFG, this);
		m_dlgCenterProcessorCfg.MoveWindow(winRect);
		m_dlgCenterProcessorCfg.ShowWindow(SW_SHOW);
		m_bInit = TRUE;
	}
	else if ((2 == nProcessor || 1 == nProcessor) && m_bInit)
	{
		m_dlgCenterProcessorCfg.ShowWindow(SW_SHOW);
		m_bInit = TRUE;
	}

	return TRUE;
}

BOOL CSvcCfg::HideDlg(int nProcessor)
{
	if (0 == nProcessor && m_bInit)
	{
		m_dlgCollectorProcessorCfg.ShowWindow(SW_HIDE);
	}
	else if (1 == nProcessor && m_bInit)
	{
		m_dlgCenterProcessorCfg.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

// 检测服务配置是否改变
BOOL CSvcCfg::CheckIsChanged(void)
{
    if (0 == theApp.m_nAlarmProcessor)
    {
        return FALSE;
    }
    else if ((1 == theApp.m_nAlarmProcessor)
        || (2 == theApp.m_nAlarmProcessor))
    {
        return m_dlgCenterProcessorCfg.CheckIsChanged();
    }

    return FALSE;
}

