#pragma once

#include "../ui/uiCtrl/ReportCtrl.h"
#include "./DownloadDlg.h"
#include <vector>
// CDownloadByFile 对话框

class CDownloadByFile : public CDialog
{
	DECLARE_DYNAMIC(CDownloadByFile)

public:
	CDownloadByFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadByFile();

// 对话框数据
	enum { IDD = IDD_DLG_DOWNLOAD_BYFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCheckAll();

private:
	CReportCtrl m_listFile;
	std::vector<PB_FILEINFO>* m_pFileList;
	CButton m_chkAll;
	CDownloadDlg * m_pDownloadDlg;  //指向唯一的下载窗口
	PB_LOGININFO* m_pLoginInfo;		// 登录信息.
	int m_nCameraID;				// 监控点ID.
	DWORD m_dwRev;					// 保留字段.
	CString m_strCamName;			// 监控点名称.
	int m_nChannel;					// 通道号.
	int m_nCheckNum;				// 下载任务数.
private:
	//初始化控件图片
	void InitCtrl();
	// 文件大小换算.
	CString GetFileSize(DWORD dwFileSize);

public:
	virtual BOOL OnInitDialog();
	void SetInitInfo(
		std::vector<PB_FILEINFO>* pFileList, 
		PB_LOGININFO* pLoginInfo, 
		CDownloadDlg* pDownloadDlg, 
		int nCameraID, 
		DWORD dwRev,
		const CString& strCamName, 
		int nChannel);

public:
	// 加入下载.
	BOOL AddDownload();
	// 获取下载任务数.
	int GetTaskNum();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
