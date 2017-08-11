
// UpdaterDlg.h : 头文件
//

#pragma once

class CUpdateThread;
// CUpdaterDlg 对话框
class CUpdaterDlg : public CDialog
{
// 构造
public:
	CUpdaterDlg(CWnd* pParent = NULL);	// 标准构造函数
	BOOL SetConfigFile(CString& sFilename);	// 设置配置文件名
	BOOL DoUpdate(void);
	LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);
	void UpdateProgress(UINT iTotalFileCount, UINT iFinishedFileCount, float fTotalPercent, float fPercent);
	void OnNotifyUpdateFinish(BOOL bSuccess = TRUE);
	BOOL DeleteDirectory(CString strDirName);
	void WriteIni();

// 对话框数据
	enum { IDD = IDD_UPDATER_DIALOG };
	CButton	m_btnUpgrade;
	CButton	m_btnCancel;
	CProgressCtrl m_ProCurCtrl;			// 当前文件进度条
	CProgressCtrl m_ProTotalCtrl;		// 总进度条
	CString m_sAppName;					// 应用程序名
	CString	m_sPrompt;					// 下载文件进度提示信息
	CString	m_sPromptTotalProgress;		// 升级总进度提示信息
	CString m_sStatus;
	CString	m_sProgressBar;				// 当前文件进度条
	CString	m_sTotalProgressBar;		// 升级总进度条
	BOOL m_bSilenceMode;				// 静默方式执行升级，不显示升级程序界面，只在升级完毕后提醒用户
	BOOL m_bUserBreak;					// 用户终止升级
	CUpdateThread *m_pUpdateThread;		// 执行升级的线程
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CString m_sConfigFilename;			// 升级配置文件名
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
private:
	DOWNLOAD_INFO_STRU m_TotalFileInfo;	// 要下载的文件总数和大小总和
	DOWNLOAD_INFO_STRU m_FinishedFileInfo;	// 已下载的文件总数和大小总和
	float m_fDownloadPercent;			// 当前正在下载的文件进度
	float m_fTotalDownloadPercent;		// 所有要下载的文件总进度
};
