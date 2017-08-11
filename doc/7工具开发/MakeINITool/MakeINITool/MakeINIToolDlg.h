
// MakeINIToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define CONFIG_FILE     (TEXT("UpdateConfig.ini"))  
#define MAX_LENGTH 128

// CMakeINIToolDlg �Ի���
class CMakeINIToolDlg : public CDialog
{
// ����
public:
	CMakeINIToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAKEINITOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	CString m_strUrl;
	CString m_LocalPath;
	void InitCtr();
	void GetExePath();
	void ReadIni();
	void FindFile(CString strPath, CString& strFilelod);
	BOOL GetFileType(CString strFileName);
	BOOL GetFileVersion(LPCTSTR strFile, CString& strVersion,CString &filename,CString &description);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnWriteini();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	//CString m_strVersion;
	CString m_strAppName;
	CString m_strFlod ;
	CEdit m_Edit;
	int m_row;
	int m_column;
	
	afx_msg void OnEnKillfocusEdit();
	afx_msg void OnHdnItemdblclickList(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_Search;
	CEdit m_Url;
	CEdit m_App;
};
