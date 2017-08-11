// CVR_ObjectDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "HCNetSDK.h"
#include "afxwin.h"
#include <vector>
using namespace std;


// CCVR_ObjectDlg 对话框
class CCVR_ObjectDlg : public CDialog
{
// 构造
public:
	CCVR_ObjectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CVR_OBJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	LONG lLoginID;    //登录返回值
	LONG m_lUploadHandle;//上传的句柄
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
	CString m_strFileOut;    //文件夹路径
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
