#pragma once

#include "../ui/uiCtrl/ReportCtrl.h"
#include "./DownloadDlg.h"
#include <vector>
// CDownloadByFile �Ի���

class CDownloadByFile : public CDialog
{
	DECLARE_DYNAMIC(CDownloadByFile)

public:
	CDownloadByFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadByFile();

// �Ի�������
	enum { IDD = IDD_DLG_DOWNLOAD_BYFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCheckAll();

private:
	CReportCtrl m_listFile;
	std::vector<PB_FILEINFO>* m_pFileList;
	CButton m_chkAll;
	CDownloadDlg * m_pDownloadDlg;  //ָ��Ψһ�����ش���
	PB_LOGININFO* m_pLoginInfo;		// ��¼��Ϣ.
	int m_nCameraID;				// ��ص�ID.
	DWORD m_dwRev;					// �����ֶ�.
	CString m_strCamName;			// ��ص�����.
	int m_nChannel;					// ͨ����.
	int m_nCheckNum;				// ����������.
private:
	//��ʼ���ؼ�ͼƬ
	void InitCtrl();
	// �ļ���С����.
	CString GetFileSize(DWORD dwFileSize);

public:
	virtual BOOL OnInitDialog();
	void SetInitInfo(
		std::vector<PB_FILEINFO>* pFileList, 
		PB_LOGININFO* pLoginInfo, 
		CDownloadDlg* pDownloadDlg, 
		int nCameraID, 
		DWORD dwRev,
		const CString& strCamName, 
		int nChannel);

public:
	// ��������.
	BOOL AddDownload();
	// ��ȡ����������.
	int GetTaskNum();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
