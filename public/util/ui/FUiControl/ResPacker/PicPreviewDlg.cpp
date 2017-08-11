// PicPreviewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ResPacker.h"
#include "PicPreviewDlg.h"
#include "../FUiControl/FUi/FMemDCEx.h"


// CPicPreviewDlg 对话框

IMPLEMENT_DYNAMIC(CPicPreviewDlg, CDialog)

CPicPreviewDlg::CPicPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicPreviewDlg::IDD, pParent)
	, m_pImage(NULL)
{
	LOGFONT logFont = {0};
	_tcscpy_s(logFont.lfFaceName, _T("微软雅黑"));
	logFont.lfHeight = 18;
	logFont.lfWeight = FW_NORMAL;
	m_font.CreateFontIndirect(&logFont);
}

CPicPreviewDlg::~CPicPreviewDlg()
{
	try
	{
		if (m_font.m_hObject)
		{
			m_font.DeleteObject();
		}
	}
	catch (...)
	{
		
	}
}

void CPicPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicPreviewDlg, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPicPreviewDlg 消息处理程序

BOOL CPicPreviewDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rcCtrl;
	GetClientRect(rcCtrl);
	FUi::CMemDCEX memDC(pDC, rcCtrl);
	memDC.FillSolidRect(rcCtrl, RGB(240, 240, 240));

	memDC->MoveTo(0, 24);
	memDC->LineTo(rcCtrl.right, 24);

	memDC->SelectObject(&m_font);
	CRect rcText(0,0, rcCtrl.right, 24);
 	if (m_pImage && m_pImage->GetLastStatus() == Ok)
 	{
		CString strInfo;
		strInfo.Format(_T("分辨率: %d * %d"), m_pImage->GetWidth(), m_pImage->GetWidth());
		memDC->DrawText(strInfo, rcText, DT_LEFT| DT_VCENTER |DT_SINGLELINE);
		Graphics graph(memDC.GetSafeHdc());
		graph.DrawImage(m_pImage, 1, 26, m_pImage->GetWidth(), m_pImage->GetHeight());
 	}
	return TRUE;
}
