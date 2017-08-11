// Cloud_ObjectDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "hcs_object_api.h"
#include <string>
using namespace std;
const int WRITE_LENGTH = 512 * 1024; //512K字节

// CCloud_ObjectDlg 对话框
class CCloud_ObjectDlg : public CDialog
{
// 构造
public:
	CCloud_ObjectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLOUD_OBJECT_DIALOG };

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
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_edit_ip;
	CEdit m_edit_port;
	CEdit m_edit_access;
	CEdit m_edit_secret;
	CListCtrl m_list_show;
	CEdit m_edit_bucketname;
	CEdit m_edit_objectkey;
	CEdit m_edit_filepath;
	HCSClient* client;     //初始化句柄
	char  fileData[WRITE_LENGTH + 1];
	CString m_strObjectKey;
	FILE * m_file_handle;
	volatile bool m_bDownloadSuc;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	BOOL GetRecordTime(HCSClient *pClient, std::string strBucket, std::string strObjectKey,  long &lStartTime, long &lEndTime);
//	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton13();
};
