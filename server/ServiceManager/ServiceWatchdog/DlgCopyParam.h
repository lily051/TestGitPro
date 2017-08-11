#pragma once
#include "resource.h"
#include "afxcmn.h"

// CDlgCopyParam �Ի���

class CDlgCopyParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgCopyParam)

public:
	CDlgCopyParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCopyParam();

// �Ի�������
	enum { IDD = IDD_DLG_COPY };

private:
    CListCtrl m_listCtrlServerModule;
    BOOL m_bSelect;
public:
    CString m_strDCIp;
    int m_nDCPort;
    CString m_strLocalIP;
private:
    void SvaeCfg(const CString & strFile,const CString & sIP, int nPort,const CString & sLocalIP);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnBnClickedBtnCancel();
    afx_msg void OnBnClickedBtnAllSel();
};
