
// CompilerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CCompilerDlg �Ի���
class CCompilerDlg : public CDialog
{
// ����
public:
	CCompilerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COMPILER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	CString m_strSearchPath;
	CString m_strProtoFile;
	CString m_strOutputPath;
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
	afx_msg void OnBnClickedBtnSelectProtoFile();
	afx_msg void OnBnClickedBtnSelectBuildPath();
	afx_msg void OnBnClickedBtnCompiler();
	afx_msg void OnBnClickedButton3();
	CEdit m_editSelPath;
	CEdit m_editProtoFile;
};
