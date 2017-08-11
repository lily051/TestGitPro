#pragma once
#include "fui\fstatic.h"
#include "fui\fdatetimectrl.h"
#include "fui\fedit.h"
#include "fui\fbutton.h"
#include "fui\flistctrl.h"
#include "csclient\HistoryClientInput.pb.h"


// CHistoryListDlg 对话框

class CHistoryListDlg : public CFDialog
{
	DECLARE_DYNAMIC(CHistoryListDlg)

public:
	CHistoryListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistoryListDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HISTORY_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void LoadSkin(void);
	void InitCtrl(void);
	void MoveCtrl(void);
	void InitData(void);
	CFStatic m_staticStartTime;
	CFDateTimeCtrl m_dateTimeStart;
	CFStatic m_staticEndTime;
	CFDateTimeCtrl m_dateTimeEnd;
	CFStatic m_staticCustomerName;
	CFEdit m_editCustomerName;
	CFStatic m_staticBusinessNum;
	CFEdit m_editBusinessNum;
	CFButton m_btnQuery;
	CFButton m_btnReset;
	CFListCtrl m_listHistory;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	CFButton m_btnOK;
	CFButton m_btnCancel;
	afx_msg void OnBnClickedBtnQuery();
	CArray<cms_8120::HisClientInput> m_arrayHisClientInput;
	void InsertToList(void);
	//查询历史业务单列表
	BOOL QueryHistoryList(cms_8120::HisClientInput& hisClientInput, CString strStartTime, CString strEndTime);
	afx_msg void OnBnClickedBtnReset();
	CFStatic m_staticBlank1;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
