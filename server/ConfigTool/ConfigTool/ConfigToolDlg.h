
// ConfigToolDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"
#include "SkinDropdown.h"
#include "MyIPAddressCtrl.h"
#include "IntEdit.h"

// CConfigToolDlg �Ի���
class CConfigToolDlg : public CDialog
{
// ����
public:
	CConfigToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONFIGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
	//��ȡ�����ļ�
	void ReadConfig();
	//���������ļ�
	void SaveConfig();

public:
    void IPHistoryDropdown(void);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnExit();
private:
    //�������Ϸ���
    BOOL CheckParamInvalid();
private:
    CString m_sXMLFileName;
private:
    CString m_sOldDCIP;
    CString m_sOldDCPort;
    CString m_sOldLocalIP;
    std::vector<CString> m_vecLocalIP;
private:
    CMyIPAddressCtrl m_dcIPCtrl;
    CMyIPAddressCtrl m_localIPCtrl;
	CIntEdit m_editDCPort;
    CSkinDropdown m_historyIP;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void OnCancel();   
};
