// CVR_ObjectDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "HCNetSDK.h"
#include "afxwin.h"
#include <vector>
using namespace std;


// CCVR_ObjectDlg �Ի���
class CCVR_ObjectDlg : public CDialog
{
// ����
public:
	CCVR_ObjectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CVR_OBJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	vector<NET_DVR_PHY_DISK_INFO> m_disk;
	//int m_lGetPhyDiskHanle;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	CIPAddressCtrl m_ctrlDevIp;
	LONG lLoginID;    //��¼����ֵ
	LONG m_lUploadHandle;//�ϴ��ľ��
	LONG m_download;
	NET_DVR_UPLOAD_DATA_PRARAM m_struDataInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editnum;
	CEdit m_editcode;
	CEdit m_editport;
	CEdit m_editfile;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CButton m_upload;
	NET_DVR_UPLOAD_FILE_RET m_struFileRet;
	NET_DVR_DOWNLOAD_DATA_PARAM m_struFileDownLoad;
	CEdit m_editurl;
	CString m_strFileOut;    //�ļ���·��
	afx_msg void OnBnClickedButton6();
	CEdit m_showfile;
	CButton m_URL;
	CButton m_btnDownLoad;
	afx_msg void OnBnClickedButton8();
	CComboBox m_comb;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	CEdit m_editInfo;
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	CComboBox m_combID;
};
