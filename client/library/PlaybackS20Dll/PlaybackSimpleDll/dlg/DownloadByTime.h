#pragma once
#include "./DownloadDlg.h"
// CDownloadByTime 对话框

class CDownloadByTime : public CDialog
{
	DECLARE_DYNAMIC(CDownloadByTime)

public:
	CDownloadByTime(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadByTime();

// 对话框数据
	enum { IDD = IDD_DLG_DOWNLOAD_BYTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	int m_nCameraID;                //监控点ID
	DWORD m_dwRev;					// 保留信息.
	CString m_strCamName;			// 监控点名称.
	int m_nChannel;					// 通道号.
	ENUM_PLAY_TYPE m_emPlayType;    //下载类型
	PB_LOGININFO* m_pLoginInfo;		// 登录信息.
	CDownloadDlg * m_pDownloadDlg;  //指向唯一的下载窗口
	int m_nCheckNum;				// 下载任务数量.

private:
	CDateTimeCtrl m_dttDateStart1;
	CDateTimeCtrl m_dttDateStart2;
	CDateTimeCtrl m_dttDateStart3;
	CDateTimeCtrl m_dttDateStart4;
	CDateTimeCtrl m_dttTimeStart1;
	CDateTimeCtrl m_dttTimeStart2;
	CDateTimeCtrl m_dttTimeStart3;
	CDateTimeCtrl m_dttTimeStart4;
	CDateTimeCtrl m_dttDateEnd1;
	CDateTimeCtrl m_dttDateEnd2;
	CDateTimeCtrl m_dttDateEnd3;
	CDateTimeCtrl m_dttDateEnd4;
	CDateTimeCtrl m_dttTimeEnd1;
	CDateTimeCtrl m_dttTimeEnd2;
	CDateTimeCtrl m_dttTimeEnd3;
	CDateTimeCtrl m_dttTimeEnd4;
	CStatic m_staCamera;
	CButton m_chkEnable1;
	CButton m_chkEnable2;
	CButton m_chkEnable3;
	CButton m_chkEnable4;
	CString m_strCameraName;      

public:
	afx_msg void OnBnClickedChkEnableTime1();
	afx_msg void OnBnClickedChkEnableTime2();
	afx_msg void OnBnClickedChkEnableTime3();
	afx_msg void OnBnClickedChkEnableTime4();

public:
	//初始化控件图片
	void InitCtrl();
	//判断时间信息是否有效
	BOOL IsTimeEffective(PB_TIME startTime, PB_TIME endTime);
	// 获取查询时间.
	void GetDownloadTime(CDateTimeCtrl* pStartDate, CDateTimeCtrl* pStartTime, 
		CDateTimeCtrl* pStopDate, CDateTimeCtrl* pStopTime, PLAY_PARAM& rPlayParam);
	// 改变时间段1控件界面状态.
	void EnableControlTime1();
	// 改变时间段2控件界面状态.
	void EnableControlTime2();
	// 改变时间段3控件界面状态.
	void EnableControlTime3();
	// 改变时间段4控件界面状态.
	void EnableControlTime4();
	// 加入下载.
	BOOL AddDownload();
	//传入下载窗口指针
	void SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg);
	// 传入其他信息.
	void SetInitInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, ENUM_PLAY_TYPE emPlayType, PB_LOGININFO* pLoginInfo);
	// 获取下载任务数.
	int GetTaskNum();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
