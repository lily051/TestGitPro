
// ConfigToolDlg.h : 头文件
//

#pragma once
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"
#include "SkinDropdown.h"
#include "MyIPAddressCtrl.h"
#include "IntEdit.h"

// CConfigToolDlg 对话框
class CConfigToolDlg : public CDialog
{
// 构造
public:
	CConfigToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONFIGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	//读取配置文件
	void ReadConfig();
	//保存配置文件
	void SaveConfig();

public:
    void IPHistoryDropdown(void);
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
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnExit();
private:
    //检查参数合法性
    BOOL CheckParamInvalid();
private:
    CString m_sXMLFileName;
private:
    CString m_sOldDCIP;
    CString m_sOldDCPort;
    CString m_sOldLocalIP;
    std::vector<CString> m_vecLocalIP;
private:
    CMyIPAddressCtrl m_dcIPCtrl;
    CMyIPAddressCtrl m_localIPCtrl;
	CIntEdit m_editDCPort;
    CSkinDropdown m_historyIP;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void OnCancel();   
};
