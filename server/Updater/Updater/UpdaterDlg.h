
// UpdaterDlg.h : ͷ�ļ�
//

#pragma once

class CUpdateThread;
// CUpdaterDlg �Ի���
class CUpdaterDlg : public CDialog
{
// ����
public:
	CUpdaterDlg(CWnd* pParent = NULL);	// ��׼���캯��
	BOOL SetConfigFile(CString& sFilename);	// ���������ļ���
	BOOL DoUpdate(void);
	LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);
	void UpdateProgress(UINT iTotalFileCount, UINT iFinishedFileCount, float fTotalPercent, float fPercent);
	void OnNotifyUpdateFinish(BOOL bSuccess = TRUE);
	BOOL DeleteDirectory(CString strDirName);
	void WriteIni();

// �Ի�������
	enum { IDD = IDD_UPDATER_DIALOG };
	CButton	m_btnUpgrade;
	CButton	m_btnCancel;
	CProgressCtrl m_ProCurCtrl;			// ��ǰ�ļ�������
	CProgressCtrl m_ProTotalCtrl;		// �ܽ�����
	CString m_sAppName;					// Ӧ�ó�����
	CString	m_sPrompt;					// �����ļ�������ʾ��Ϣ
	CString	m_sPromptTotalProgress;		// �����ܽ�����ʾ��Ϣ
	CString m_sStatus;
	CString	m_sProgressBar;				// ��ǰ�ļ�������
	CString	m_sTotalProgressBar;		// �����ܽ�����
	BOOL m_bSilenceMode;				// ��Ĭ��ʽִ������������ʾ����������棬ֻ��������Ϻ������û�
	BOOL m_bUserBreak;					// �û���ֹ����
	CUpdateThread *m_pUpdateThread;		// ִ���������߳�
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CString m_sConfigFilename;			// ���������ļ���
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
private:
	DOWNLOAD_INFO_STRU m_TotalFileInfo;	// Ҫ���ص��ļ������ʹ�С�ܺ�
	DOWNLOAD_INFO_STRU m_FinishedFileInfo;	// �����ص��ļ������ʹ�С�ܺ�
	float m_fDownloadPercent;			// ��ǰ�������ص��ļ�����
	float m_fTotalDownloadPercent;		// ����Ҫ���ص��ļ��ܽ���
};
