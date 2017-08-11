#pragma once
#include "./DownloadDlg.h"
// CDownloadByTime �Ի���

class CDownloadByTime : public CDialog
{
	DECLARE_DYNAMIC(CDownloadByTime)

public:
	CDownloadByTime(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadByTime();

// �Ի�������
	enum { IDD = IDD_DLG_DOWNLOAD_BYTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	int m_nCameraID;                //��ص�ID
	DWORD m_dwRev;					// ������Ϣ.
	CString m_strCamName;			// ��ص�����.
	int m_nChannel;					// ͨ����.
	ENUM_PLAY_TYPE m_emPlayType;    //��������
	PB_LOGININFO* m_pLoginInfo;		// ��¼��Ϣ.
	CDownloadDlg * m_pDownloadDlg;  //ָ��Ψһ�����ش���
	int m_nCheckNum;				// ������������.

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
	//��ʼ���ؼ�ͼƬ
	void InitCtrl();
	//�ж�ʱ����Ϣ�Ƿ���Ч
	BOOL IsTimeEffective(PB_TIME startTime, PB_TIME endTime);
	// ��ȡ��ѯʱ��.
	void GetDownloadTime(CDateTimeCtrl* pStartDate, CDateTimeCtrl* pStartTime, 
		CDateTimeCtrl* pStopDate, CDateTimeCtrl* pStopTime, PLAY_PARAM& rPlayParam);
	// �ı�ʱ���1�ؼ�����״̬.
	void EnableControlTime1();
	// �ı�ʱ���2�ؼ�����״̬.
	void EnableControlTime2();
	// �ı�ʱ���3�ؼ�����״̬.
	void EnableControlTime3();
	// �ı�ʱ���4�ؼ�����״̬.
	void EnableControlTime4();
	// ��������.
	BOOL AddDownload();
	//�������ش���ָ��
	void SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg);
	// ����������Ϣ.
	void SetInitInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, ENUM_PLAY_TYPE emPlayType, PB_LOGININFO* pLoginInfo);
	// ��ȡ����������.
	int GetTaskNum();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
