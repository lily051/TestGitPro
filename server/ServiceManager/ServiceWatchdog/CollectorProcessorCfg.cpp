// CollectorProcessorCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "CollectorProcessorCfg.h"


// CCollectorProcessorCfg 对话框

IMPLEMENT_DYNAMIC(CCollectorProcessorCfg, CDialog)

CCollectorProcessorCfg::CCollectorProcessorCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CCollectorProcessorCfg::IDD, pParent)
{
	m_brushBkgnd.CreateSolidBrush(RGB(/*203*/227, /*203*/227, /*207*/227));
}

CCollectorProcessorCfg::~CCollectorProcessorCfg()
{
}

void CCollectorProcessorCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCollectorProcessorCfg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCollectorProcessorCfg 消息处理程序

// 背景画刷
HBRUSH CCollectorProcessorCfg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor != CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brushBkgnd;
	}

	return hbr;
}