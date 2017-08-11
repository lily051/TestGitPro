
// bucketDlg.h : ͷ�ļ�
//

#pragma once
#include "hcs_object_api.h"

// CbucketDlg �Ի���
class CbucketDlg : public CDialog
{
// ����
public:
	CbucketDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BUCKET_DIALOG };

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
};
