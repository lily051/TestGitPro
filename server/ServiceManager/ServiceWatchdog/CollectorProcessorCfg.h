#pragma once

#include "resource.h"

// CCollectorProcessorCfg �Ի���

class CCollectorProcessorCfg : public CDialog
{
	DECLARE_DYNAMIC(CCollectorProcessorCfg)

public:
	CCollectorProcessorCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCollectorProcessorCfg();

// �Ի�������
	enum { IDD = IDD_DLG_COLLECTOR_PROCESSOR_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	/* ��ˢ */
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBrush m_brushBkgnd; ///< ��ˢ
};
