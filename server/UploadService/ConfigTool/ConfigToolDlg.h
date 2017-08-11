
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
	/* 运行命令 */
	BOOL RunCmd(const CString& cmd, const CString& DefaultDir);
// 实现
protected:
	HICON m_hIcon;
	NOTIFYICONDATA m_nid;       ///< 系统托盘图标

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	/* WM_ICON_NOTIFY 消息响应 */
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	/* 添加系统托盘图标 */
	void AddNotifyIcon(void);
	/* 删除系统托盘图标 */
	void RemoveNotifyIcon(void);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnExit();
private:
    //检查参数合法性
    BOOL CheckParamInvalid();
public:
	/* 显示窗口 */
	afx_msg void OnIconnotifyShowWindow();
	/* 隐藏窗口 */
	afx_msg void OnIconnotifyHideWindow();
	/* 退出 */
	afx_msg void OnIconnotifyExit();
	/* 启动 */
	afx_msg void OnIconnotifyStart();
	/* 停止 */
	afx_msg void OnIconnotifyStop();
	/* 重启 */
	afx_msg void OnIconnotifyRestart();
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
