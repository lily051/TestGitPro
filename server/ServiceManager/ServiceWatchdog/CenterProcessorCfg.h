#pragma once

#include "resource.h"
#include "./Utilities/MyIPAddressCtrl.h"
#include "./Utilities/IntEdit.h"

// CCenterProcessorCfg 对话框

class CCenterProcessorCfg : public CDialog
{
	DECLARE_DYNAMIC(CCenterProcessorCfg)

public:
	CCenterProcessorCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCenterProcessorCfg();

// 对话框数据
	enum { IDD = IDD_DLG_CENTER_PROCESSOR_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	/* 画刷 */
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnCenterCfgSave();
	afx_msg void OnBnClickedBtnCenterCfgReset();

	BOOL CheckIsChanged(void);

private:
	BOOL InitDlg(void);
	BOOL ReadConfig();
	void BuildDefaultFile();
	BOOL SaveConfig();

	//检查参数合法性
	BOOL CheckParamInvalid();

	CString m_sOldAlramIP;
	CString m_sOldAlarmPort;
	CString m_sOldlocalIP;
	CString m_sOldLocalPort;

	CMyIPAddressCtrl m_alramIPCtrl;
	CIntEdit m_editAlarmPort;
	CMyIPAddressCtrl m_localIPCtrl;
	CIntEdit m_editLocalPort;

	BOOL m_bMove;
	CBrush m_brushBkgnd; ///< 画刷
};
