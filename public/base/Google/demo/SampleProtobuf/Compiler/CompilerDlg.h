
// CompilerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CCompilerDlg 对话框
class CCompilerDlg : public CDialog
{
// 构造
public:
	CCompilerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COMPILER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	CString m_strSearchPath;
	CString m_strProtoFile;
	CString m_strOutputPath;
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
	afx_msg void OnBnClickedBtnSelectProtoFile();
	afx_msg void OnBnClickedBtnSelectBuildPath();
	afx_msg void OnBnClickedBtnCompiler();
	afx_msg void OnBnClickedButton3();
	CEdit m_editSelPath;
	CEdit m_editProtoFile;
};
