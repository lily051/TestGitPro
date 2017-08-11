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
	EM_LIST_TOTAL = 1,			//ȫ��ҵ���¼
	EM_LIST_EXCEPTION = 2	//�쳣¼��
}; 


// CBusinessListDlg �Ի���

class CBusinessListDlg : public CFDialog
{
	DECLARE_DYNAMIC(CBusinessListDlg)

public:
	CBusinessListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBusinessListDlg();

// �Ի�������
	enum { IDD = IDD_DLG_BUSINESSLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	int m_emListType;  //��ǰ��ǩҳ��1-ȫ����2-�쳣


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
	//��ѯҵ���¼��ҵ���б�
	BOOL QueryBusinessList(CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName);
	//��ѯ�쳣¼���б�
	BOOL QueryErrorRecordsList(CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName);

	BOOL InsertToTotalList(cms_8120::RecordsInfo& recordInfo);
	BOOL InsertToErrorList(cms_8120::ErrRecordsInfo& errorRecInfo);
	CList<cms_8120::RecordsInfo> m_listBusinessInfo;  //ҵ���¼��ѯ���
	CList<cms_8120::ErrRecordsInfo> m_listErrorRecInfo;	//�쳣¼���ѯ���

	//�쳣���ط�¼��
	BOOL PlaybackErrorRecord(int nItem);
	//�쳣�������ϴ�¼��
	BOOL ReUploadErrorRecord(int nItem, BOOL bAll = FALSE);
	//�쳣��ɾ��
	BOOL DeleteErrorRecord(int nItem);

	//ȫ�����ط�
	BOOL PlaybackBusiness(int nItem);
	//ȫ���������ϴ�
	BOOL ReUploadBusiness(int nItem, BOOL bAll = FALSE);
	//ȫ������ӡ
	void PrintBusiness(int nItem);

	//��ѯ�ӿ�
	void QueryBusiness(BOOL bShowError = TRUE);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnExport();
	//����ҵ���¼
	void AddBusinessListSheet(CExcel& excel);
	//�����쳣¼���¼
	void AddExceptionListSheet(CExcel& excel);


	CPlaybackCtrlDlg *m_pPlaybackDlg;
	CPlaybackCtrlDlg m_playbackDlg;
	afx_msg void OnBnClickedBtnUpload();
	CStatic m_staticGroup;
	afx_msg void OnNMClickListTotal(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
