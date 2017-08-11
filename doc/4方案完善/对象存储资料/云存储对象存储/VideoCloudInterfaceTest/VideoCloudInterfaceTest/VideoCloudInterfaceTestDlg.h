
// VideoCloudInterfaceTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "VideoWriteFile.h"
#include "VideoQueryAndDownload.h"
#include "VideoActiveCloud.h"
#include <vector>
// CVideoCloudInterfaceTestDlg �Ի���
class CVideoCloudInterfaceTestDlg : public CDialog
{
// ����
public:
	CVideoCloudInterfaceTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CVideoCloudInterfaceTestDlg();
// �Ի�������
	enum { IDD = IDD_VIDEOCLOUDINTERFACETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
    CIPAddressCtrl m_ctrlDevIp;
    UINT m_nDevPort;
    CString m_csUser;
    CString m_csPWD;

    bool m_bLogin;            //��¼�߼�
    unsigned long long m_nLogID;//��¼���
    CVideoWriteFile m_PageWrite;
    CVideoQueryAndDownload m_PageQueryAndDownload;
    CVideoActiveCloud m_PageActiveCloud;
    std::vector<HCS_PoolInfo> VecPoolInfo;
public:
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnTcnSelchangeTabPlayback(NMHDR *pNMHDR, LRESULT *pResult);
private:
    CTabCtrl m_ctrlTab;
public:
    afx_msg void OnBnClickedButtonGetVersion();
    afx_msg void OnBnClickedButtonLoginGetVersion();
};
