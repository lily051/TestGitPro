#pragma once

#include "resource.h"

// CCollectorProcessorCfg 对话框

class CCollectorProcessorCfg : public CDialog
{
	DECLARE_DYNAMIC(CCollectorProcessorCfg)

public:
	CCollectorProcessorCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCollectorProcessorCfg();

// 对话框数据
	enum { IDD = IDD_DLG_COLLECTOR_PROCESSOR_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	/* 画刷 */
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBrush m_brushBkgnd; ///< 画刷
};
