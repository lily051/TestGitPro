#pragma once
#include "fui\fbutton.h"
#include "fui\flistctrl.h"
#include "fui\fdatetimectrl.h"
#include "fui\fstatic.h"
#include "fui\fedit.h"
#include "csclient\CsClientRecord.pb.h"
#include "BusinessInfoDlg.h"
#include "PlaybackCtrlDlg.h"
#include "afxwin.h"

class CExcel;

enum EM_LIST_TYPE {
	EM_LIST_TOTAL = 1,			//全部业务记录
	EM_LIST_EXCEPTION = 2	//异常录像
}; 


// CBusinessListDlg 对话框

class CBusinessListDlg : public CFDialog
{
	DECLARE_DYNAMIC(CBusinessListDlg)

public:
	CBusinessListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBusinessListDlg();

// 对话框数据
	enum { IDD = IDD_DLG_BUSINESSLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFButton m_btnTotal;
	CFButton m_btnException;
	CFButton m_btnUpload;
	CFButton m_btnRefresh;
	CFButton m_btnExport;
	CFListCtrl m_listTotal;
	CFListCtrl m_listException;
	void InitCtrl(void);
	void MoveCtrl(void);
	void InitData(void);
	void ShowSelectList(void);
	void DrawSelectLine(CDC& dc);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnTotal();
	afx_msg void OnBnClickedBtnException();
	int m_emListType;  //当前标签页，1-全部，2-异常


	afx_msg void OnPaint();
	CFDateTimeCtrl m_dateTimeStart;
	CFDateTimeCtrl m_dateTimeEnd;
	CFButton m_btnQuery;
	afx_msg void OnBnClickedBtnQuery();
	CFStatic m_staticStartTime;
	CFStatic m_staticEndTime;
	CFStatic m_staticConsumerName;
	CFEdit m_editConsumerName;
	CFStatic m_staticProductName;
	CFEdit m_editProductName;
	CFButton m_btnReset;
	//查询业务记录（业务单列表）
	BOOL QueryBusinessList(CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName);
	//查询异常录像列表
	BOOL QueryErrorRecordsList(CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName);

	BOOL InsertToTotalList(cms_8120::RecordsInfo& recordInfo);
	BOOL InsertToErrorList(cms_8120::ErrRecordsInfo& errorRecInfo);
	CList<cms_8120::RecordsInfo> m_listBusinessInfo;  //业务记录查询结果
	CList<cms_8120::ErrRecordsInfo> m_listErrorRecInfo;	//异常录像查询结果

	//异常：回放录像
	BOOL PlaybackErrorRecord(int nItem);
	//异常：重新上传录像
	BOOL ReUploadErrorRecord(int nItem, BOOL bAll = FALSE);
	//异常：删除
	BOOL DeleteErrorRecord(int nItem);

	//全部：回放
	BOOL PlaybackBusiness(int nItem);
	//全部：重新上传
	BOOL ReUploadBusiness(int nItem, BOOL bAll = FALSE);
	//全部：打印
	void PrintBusiness(int nItem);

	//查询接口
	void QueryBusiness(BOOL bShowError = TRUE);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnExport();
	//导出业务记录
	void AddBusinessListSheet(CExcel& excel);
	//导出异常录像记录
	void AddExceptionListSheet(CExcel& excel);


	CPlaybackCtrlDlg *m_pPlaybackDlg;
	CPlaybackCtrlDlg m_playbackDlg;
	afx_msg void OnBnClickedBtnUpload();
	CStatic m_staticGroup;
	afx_msg void OnNMClickListTotal(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
