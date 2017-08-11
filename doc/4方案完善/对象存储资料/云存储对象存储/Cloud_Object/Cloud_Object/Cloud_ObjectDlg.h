// Cloud_ObjectDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "hcs_object_api.h"
#include <string>
using namespace std;
const int WRITE_LENGTH = 512 * 1024; //512K�ֽ�

// CCloud_ObjectDlg �Ի���
class CCloud_ObjectDlg : public CDialog
{
// ����
public:
	CCloud_ObjectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLOUD_OBJECT_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CEdit m_edit_ip;
	CEdit m_edit_port;
	CEdit m_edit_access;
	CEdit m_edit_secret;
	CListCtrl m_list_show;
	CEdit m_edit_bucketname;
	CEdit m_edit_objectkey;
	CEdit m_edit_filepath;
	HCSClient* client;     //��ʼ�����
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
