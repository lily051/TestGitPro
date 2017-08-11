#pragma once
#include "vcacfgocx1.h"

// CIntelDlgEx �Ի���

class CIntelDlgEx : public CDialog
{
	DECLARE_DYNAMIC(CIntelDlgEx)

public:
	CIntelDlgEx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIntelDlgEx();

// �Ի�������
	enum { IDD = IDD_DLG_INTEL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()
private:
    CComboBox       m_comDevChan;
    CVcacfgocx1     m_vcaCfgDll;
    FUN_PARAM       m_struFunParam;
public:
    afx_msg void OnBnClickedButtonVcaDev();
    afx_msg void OnBnClickedButtonVcaChan();
    void SetDevInfo(FUN_PARAM struFunParam);
};
