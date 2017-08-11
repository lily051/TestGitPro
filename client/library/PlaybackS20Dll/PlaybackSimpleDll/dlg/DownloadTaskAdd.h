#pragma once
#include "../ui/uiCtrl/UiDialog.h"
#include "../ui/uiBase/xSkinButton.h"
#include "../ui/uiBase/SkinTabCtrl.h"
#include "./DownloadDlg.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "DownloadByTime.h"
#include "DownloadByFile.h"


// CDownloadTaskAdd �Ի���

class CDownloadTaskAdd : public UIDialog
{
	DECLARE_DYNAMIC(CDownloadTaskAdd)

public:
	CDownloadTaskAdd(std::vector<PB_FILEINFO>* pFileList, CWnd* pParent = NULL, BOOL bShowDownTimeTab = TRUE);   // ��׼���캯��
	virtual ~CDownloadTaskAdd();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOADTASKADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	enum
	{
		SEL_BY_FILE,
		SEL_BY_TIME
	};
private:
	CBrush m_bkBrush;
	CSkinTabCtrl m_tabDownload;			// ��������tab.
	int m_nCameraID;					// ��ص�ID.
	DWORD m_dwRev;						// �����ֶ�.
	CString m_strCamName;				// ��ص�����.
	DWORD m_dwRight;					// Ȩ�޿���.
	CDownloadDlg * m_pDownloadDlg;  //ָ��Ψһ�����ش���
	int m_nChannel;					// ͨ����.
	ENUM_PLAY_TYPE m_emPlayType;    //��������
	PB_LOGININFO* m_pLoginInfo;		// ��¼��Ϣ.
	std::vector<PB_FILEINFO>* m_pFileList;
	CDownloadByTime* m_pDownloadByTime;	// ��ʱ������.
	CDownloadByFile* m_pDownloadByFile;	// ���ļ�����.
	BOOL m_bShowDownTimeTab;			// �Ƿ���ʾ��ʱ������
private:
	//��ʼ���ؼ�ͼƬ
	void InitCtrl();
public:
	//�������ش���ָ��
	void SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg);
	// ����������Ϣ.
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
