#pragma once

class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL); 
	CPreviewDlg(PREV_INIT &stPrevInit, CWnd* pParent = NULL);
	virtual ~CPreviewDlg();

	enum { IDD = IDD_DIALOG_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
    //开始预览
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//停止预览
	BOOL StopPreview();

	//开始录像
	BOOL StartRecord(LONG lWndNum);

	//停止录像
	BOOL StopRecord(LONG lWndNum, RECOED_FILE_INFO& recoedFile);

	//设置OSD信息
	BOOL SetShowString(const CString& strBussInfo);

	//设置配置信息
	BOOL SetConfigData( const CString& strCfgInfo );

	//设置可信任站点
	BOOL SetTrustedSite( const CString& strTrustedSite );

	//获取所有USB监控点信息
	BOOL GetAllUSBCameraInfo(CString& strCameraInfo);


private:
	HANDLE m_hPreview;
	PREV_INIT m_stPrevInit;
};
