
// MakeINIToolDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define CONFIG_FILE     (TEXT("UpdateConfig.ini"))  
#define MAX_LENGTH 128

// CMakeINIToolDlg 对话框
class CMakeINIToolDlg : public CDialog
{
// 构造
public:
	CMakeINIToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAKEINITOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
