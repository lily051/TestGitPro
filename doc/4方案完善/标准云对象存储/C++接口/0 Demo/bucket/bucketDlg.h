
// bucketDlg.h : 头文件
//

#pragma once
#include "hcs_object_api.h"
#include "afxcmn.h"
#include <string>
const int WRITE_LENGTH = 512 * 1024; //512K字节

// CbucketDlg 对话框
class CbucketDlg : public CDialog
{
// 构造
public:
	CbucketDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BUCKET_DIALOG };

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
	afx_msg void OnBnClickedBtnCreateBucket();
	afx_msg void OnBnClickedBtnDeleteBucket();
	CString m_strBucketName;
	CString m_strObjectKey;
	CString m_strCloudIp;
	int m_strCloudPort;
	CString m_strAccessKey;
	CString m_strSecretKey;
	afx_msg void OnBnClickedBtnDeleteObject();
	CString m_strBucketName2;
	int m_nBucketSize;
	HCSClient* m_HcsClient;
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnInitial();
	afx_msg void OnBnClickedBtnDeleteAllobject();
	afx_msg void OnBnClickedBtnSetBucketCapacity();
	int m_nBucketLife;
	afx_msg void OnBnClickedBtnSetBucketLife();
	CListCtrl m_listBucketQuery;
	afx_msg void OnBnClickedBtnQueryAllBucket();
	CListCtrl m_listObjectQuery;
	afx_msg void OnBnClickedBtnQueryAllObject();
	afx_msg void OnBnClickedBtnNormalUploadObject();
	afx_msg void OnBnClickedBtnNormalDownloadObject();
	afx_msg void OnBnClickedBtnStreamUploadobject();
	afx_msg void OnBnClickedBtnStreamDownloadobject();
	CString m_csPathName;
	FILE * m_file_handle;
	volatile bool m_bDownloadSuc;
	afx_msg void OnBnClickedBtnSelectFile();
	afx_msg void OnBnClickedBtnNormalUploadObjectFromBuf();
	char  fileData[WRITE_LENGTH + 1];
	//char  m_fileData[10 * 1024 * 1024 + 1];
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL GetRecordTime(HCSClient *pClient, std::string strBucket, std::string strObjectKey,  long &lStartTime, long &lEndTime);
	afx_msg void OnBnClickedBtnSmallFileNormalUploadObjectFromBuf();
};
