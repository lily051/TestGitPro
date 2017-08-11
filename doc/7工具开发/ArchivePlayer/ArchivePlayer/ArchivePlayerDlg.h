
// ArchivePlayerDlg.h : 头文件
//

#pragma once
#include "BusinessDialg.h"
#include "DevinfoDialog.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "HCNetSDK.h"
#include "hcs_sdk.h"

#include "NTPTimingdlg.h"
#include "afxwin.h"

#include "operateDBV20.h"
#include "operateDBV21.h"
// CArchivePlayerDlg 对话框
class CArchivePlayerDlg : public CDialog
{
// 构造
public:
	CArchivePlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ARCHIVEPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
    BusinessDialg *m_bdlg;//业务显示页面
    DevinfoDialog m_ddlg;//设备信息显示页面
    CDialog* pDialog[2];  //用来保存对话框对象指针
    int m_CurSelTab;
    void InitPage();
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
    CTabCtrl m_tab; 

    DBInterface* pDBinter;
    HANDLE m_serachThreadHandle;
    HANDLE m_loginThreadHandle;
    int m_interval;

    CRect m_rect;
    afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonSearch();
    void GetStartStopTime(NET_DVR_TIME &StartDayTime, NET_DVR_TIME &StopDayTime);

    //BOOL searchBusiness();
    void SearchRecord();

    BOOL SearchInCLoud(RecordInfo &record);

    BOOL SearchInCVR(RecordInfo &record);

    BOOL RemoteFileSearch(RecordInfo &record);
    BOOL CameraFileSearch(RecordInfo &record);
  
    void getInfoFromDevice(DevicInfo &pDev);
    void getDeviceResoureCfg(DevicInfo &pDev);
    void getNTPinfo(DevicInfo &pDev);
    void getRecordPlan(DevicInfo &pDev);
    void ShowInList(const int items,RecordInfo& pBinfo);
    void save2File(CListCtrl & plist);
    BOOL setNPTTiming(DevicInfo &pDev,NET_DVR_NTPPARA &ntpinfo);
    void InitComBox(CComboBox &pCom,std::vector<Iteminfo> &plist);
    void getCondition( CComboBox &pCom,CEdit &pEdit,std::vector<filtrate>& pfilter);

    CDateTimeCtrl m_ctrlDateStart;
    CDateTimeCtrl m_ctrlDateEnd;
    CDateTimeCtrl m_ctrlTimeStart;
    CDateTimeCtrl m_ctrlTimeEnd;    
    CEdit m_edit_diff;
    CEdit m_sqlport;
    CEdit m_sqlname;
    CEdit m_sqluser;
    CEdit m_sqlpassword;
    CIPAddressCtrl m_sqlip;
    volatile LONG m_beSearchRecord;
    BOOL m_beSearchDev;
    BOOL m_beLogin;

    BOOL m_beOldVersions;
    volatile LONG m_beLoginMysql;
    afx_msg void OnClose();
    afx_msg void OnBnClickedButtonDev();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedButton2();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedButtonNtpTiming();
    afx_msg void OnBnClickedButtonLogin();
 
    afx_msg void OnBnClickedButtonSelect();
    CComboBox m_comCondition1;
    CComboBox m_comCondition2;
    CComboBox m_comCondition3;
    CComboBox m_comCondition4;
    CEdit m_editCondition3;
    CEdit m_editCondition4;
    CEdit m_editCondition2;
    CEdit m_editCondition1;
    CComboBox m_cbVersion;
};
