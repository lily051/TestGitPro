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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
    //��ʼԤ��
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//ֹͣԤ��
	BOOL StopPreview();

	//��ʼ¼��
	BOOL StartRecord(LONG lWndNum);

	//ֹͣ¼��
	BOOL StopRecord(LONG lWndNum, RECOED_FILE_INFO& recoedFile);

	//����OSD��Ϣ
	BOOL SetShowString(const CString& strBussInfo);

	//����������Ϣ
	BOOL SetConfigData( const CString& strCfgInfo );

	//���ÿ�����վ��
	BOOL SetTrustedSite( const CString& strTrustedSite );

	//��ȡ����USB��ص���Ϣ
	BOOL GetAllUSBCameraInfo(CString& strCameraInfo);


private:
	HANDLE m_hPreview;
	PREV_INIT m_stPrevInit;
};
