#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// NTPTimingdlg �Ի���

class NTPTimingdlg : public CDialog
{
	DECLARE_DYNAMIC(NTPTimingdlg)

public:
	NTPTimingdlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NTPTimingdlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TIMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCheck();

    CString m_DevIp;
    CString m_DevPort ;
    CString m_Interval;
    CIPAddressCtrl m_editNTPip;
    CEdit m_editNTPport;
    CEdit m_editNTPinterval;
    CButton m_checkNTPswitch;

    void setNTPedit(BOOL flag);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
