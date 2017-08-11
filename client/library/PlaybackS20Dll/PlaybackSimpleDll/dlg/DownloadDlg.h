#pragma once
/**	@file    DownloadDlg.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ���ضԻ���
*
*	@author	 yudan
*	@date	 2012/07/20
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/07/20 by yudan
*/


#include "../ui/uiBase/TransButton.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "./DownloadingDlg.h"
#include "./DownloadTitleDlg.h"
#include "./DownFailedDlg.h"
#include "./DownSuccessDlg.h"


// CDownloadDlg �Ի���

class CDownloadDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadDlg)

public:
	CDownloadDlg(HANDLE hDlg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	CBrush m_bkBrush;  
	HANDLE m_hDlg;
	LONG m_lDownloadSeq;
	std::map<LONG, DOWNLOAD_INFO> m_mapDownloadInfo;

public:
	CDownloadTitleDlg * m_pDownloadTitleDlg;
	CDownloadingDlg * m_pDownloadingDlg;
	CDownSuccessDlg * m_pDownloadSucDlg;
	CDownFailedDlg * m_pDownloadFailedDlg;

private:
	//�����ӿؼ�
	void InitCtrl();
	//�ƶ�����
	void MoveCtrl();
	// ����ļ���.
	void CheckFileName(CStringA& strFileName);

public:
	//���ʱ��������Ϣ
	BOOL AddTimeDownloadInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, PB_LOGININFO& stLoginInfo, PLAY_PARAM& stPlayParam, CString& strErrorMsg);
	//�����ļ�������Ϣ
	BOOL AddFileDownloadInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, PB_LOGININFO& stLoginInfo, PB_FILEINFO& stFileInfo);
	//�¼�֪ͨ����
	BOOL NotifyStartDownload();
	// ��ȡ������Ϣ.
	BOOL GetPlaybackInfo(PlaybackStatData& stData);

	//��������static�ĵ����Ϣ
	LRESULT OnMsgListSelect(WPARAM wParam, LPARAM lParam);
	//ɾ����Ϣ
	LRESULT OnMsgDelete(WPARAM wParam, LPARAM lParam);
	//����������Ϣ
	LRESULT OnMsgRedown(WPARAM wParam, LPARAM lParam);
	//���������Ϣ
	LRESULT OnMsgSuccess(WPARAM wParam, LPARAM lParam);
	//����ʧ��
	LRESULT OnMsgFailed(WPARAM wParam, LPARAM lParam);
	//����֪ͨ��Ϣ
	LRESULT OnMsgNotifyDown(WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	LRESULT OnDownloadFilePlay(WPARAM wParam, LPARAM lParam);
    INT64 CalTotalSizeInTime( DOWNLOAD_INFO &stDownloadInfo, PB_LOGININFO& stLoginInfo );
};
