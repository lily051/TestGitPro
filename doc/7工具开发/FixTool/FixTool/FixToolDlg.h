
// FixToolDlg.h : 头文件
//

#pragma once
#include "Tinyxml/tinyxml.h"
#include "Tinyxml/tinyxmlutils.h"
#include <WinSvc.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

// CFixToolDlg 对话框
class CFixToolDlg : public CDialog
{
// 构造
public:
	CFixToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FIXTOOL_DIALOG };
	CComboBox	m_comProduct;
	CEdit		m_editPackPath;
	CString		m_sPackPath;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSelectPack();

public:
	BOOL GetProcessPath(const CString & ProcessName, CString & ProcessPath);
};
