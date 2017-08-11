
// VideoCloudInterfaceTestDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "VideoWriteFile.h"
#include "VideoQueryAndDownload.h"
#include "VideoActiveCloud.h"
#include <vector>
// CVideoCloudInterfaceTestDlg 对话框
class CVideoCloudInterfaceTestDlg : public CDialog
{
// 构造
public:
	CVideoCloudInterfaceTestDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CVideoCloudInterfaceTestDlg();
// 对话框数据
	enum { IDD = IDD_VIDEOCLOUDINTERFACETEST_DIALOG };

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
private:
    CIPAddressCtrl m_ctrlDevIp;
    UINT m_nDevPort;
    CString m_csUser;
    CString m_csPWD;

    bool m_bLogin;            //登录逻辑
    unsigned long long m_nLogID;//登录句柄
    CVideoWriteFile m_PageWrite;
    CVideoQueryAndDownload m_PageQueryAndDownload;
    CVideoActiveCloud m_PageActiveCloud;
    std::vector<HCS_PoolInfo> VecPoolInfo;
public:
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnTcnSelchangeTabPlayback(NMHDR *pNMHDR, LRESULT *pResult);
private:
    CTabCtrl m_ctrlTab;
public:
    afx_msg void OnBnClickedButtonGetVersion();
    afx_msg void OnBnClickedButtonLoginGetVersion();
};
