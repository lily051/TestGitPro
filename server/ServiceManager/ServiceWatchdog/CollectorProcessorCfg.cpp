// CollectorProcessorCfg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CollectorProcessorCfg.h"


// CCollectorProcessorCfg �Ի���

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


// CCollectorProcessorCfg ��Ϣ�������

// ������ˢ
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