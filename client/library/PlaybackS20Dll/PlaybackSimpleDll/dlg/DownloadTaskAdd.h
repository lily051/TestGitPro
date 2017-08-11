#pragma once
#include "../ui/uiCtrl/UiDialog.h"
#include "../ui/uiBase/xSkinButton.h"
#include "../ui/uiBase/SkinTabCtrl.h"
#include "./DownloadDlg.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "DownloadByTime.h"
#include "DownloadByFile.h"


// CDownloadTaskAdd 对话框

class CDownloadTaskAdd : public UIDialog
{
	DECLARE_DYNAMIC(CDownloadTaskAdd)

public:
	CDownloadTaskAdd(std::vector<PB_FILEINFO>* pFileList, CWnd* pParent = NULL, BOOL bShowDownTimeTab = TRUE);   // 标准构造函数
	virtual ~CDownloadTaskAdd();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOADTASKADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	enum
	{
		SEL_BY_FILE,
		SEL_BY_TIME
	};
private:
	CBrush m_bkBrush;
	CSkinTabCtrl m_tabDownload;			// 下载类型tab.
	int m_nCameraID;					// 监控点ID.
	DWORD m_dwRev;						// 保留字段.
	CString m_strCamName;				// 监控点名称.
	DWORD m_dwRight;					// 权限控制.
	CDownloadDlg * m_pDownloadDlg;  //指向唯一的下载窗口
	int m_nChannel;					// 通道号.
	ENUM_PLAY_TYPE m_emPlayType;    //下载类型
	PB_LOGININFO* m_pLoginInfo;		// 登录信息.
	std::vector<PB_FILEINFO>* m_pFileList;
	CDownloadByTime* m_pDownloadByTime;	// 按时间下载.
	CDownloadByFile* m_pDownloadByFile;	// 按文件下载.
	BOOL m_bShowDownTimeTab;			// 是否显示按时间下载
private:
	//初始化控件图片
	void InitCtrl();
public:
	//传入下载窗口指针
	void SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg);
	// 传入其他信息.
	void SetInitInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, DWORD dwRight, ENUM_PLAY_TYPE emPlayType, PB_LOGININFO* pLoginInfo);

public:
	virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonAdddownload();
	afx_msg void OnBnClickedButtonClosedlg();
	afx_msg void OnTcnSelchangeTabDownload(NMHDR *pNMHDR, LRESULT *pResult);
	CxSkinButton m_btnAddDownload;
	CxSkinButton m_btnCancel;
	CxSkinButton m_btnCloseDlg;
	
	afx_msg void OnDestroy();
};
