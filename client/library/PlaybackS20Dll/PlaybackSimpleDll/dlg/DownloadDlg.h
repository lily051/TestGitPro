#pragma once
/**	@file    DownloadDlg.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   下载对话框
*
*	@author	 yudan
*	@date	 2012/07/20
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/07/20 by yudan
*/


#include "../ui/uiBase/TransButton.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "./DownloadingDlg.h"
#include "./DownloadTitleDlg.h"
#include "./DownFailedDlg.h"
#include "./DownSuccessDlg.h"


// CDownloadDlg 对话框

class CDownloadDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadDlg)

public:
	CDownloadDlg(HANDLE hDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	//创建子控件
	void InitCtrl();
	//移动窗口
	void MoveCtrl();
	// 检查文件名.
	void CheckFileName(CStringA& strFileName);

public:
	//添加时间下载信息
	BOOL AddTimeDownloadInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, PB_LOGININFO& stLoginInfo, PLAY_PARAM& stPlayParam, CString& strErrorMsg);
	//增加文件下载信息
	BOOL AddFileDownloadInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, PB_LOGININFO& stLoginInfo, PB_FILEINFO& stFileInfo);
	//事件通知下载
	BOOL NotifyStartDownload();
	// 获取下载信息.
	BOOL GetPlaybackInfo(PlaybackStatData& stData);

	//接收来自static的点击消息
	LRESULT OnMsgListSelect(WPARAM wParam, LPARAM lParam);
	//删除消息
	LRESULT OnMsgDelete(WPARAM wParam, LPARAM lParam);
	//重新下载消息
	LRESULT OnMsgRedown(WPARAM wParam, LPARAM lParam);
	//下载完成消息
	LRESULT OnMsgSuccess(WPARAM wParam, LPARAM lParam);
	//下载失败
	LRESULT OnMsgFailed(WPARAM wParam, LPARAM lParam);
	//下载通知消息
	LRESULT OnMsgNotifyDown(WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	LRESULT OnDownloadFilePlay(WPARAM wParam, LPARAM lParam);
    INT64 CalTotalSizeInTime( DOWNLOAD_INFO &stDownloadInfo, PB_LOGININFO& stLoginInfo );
};
