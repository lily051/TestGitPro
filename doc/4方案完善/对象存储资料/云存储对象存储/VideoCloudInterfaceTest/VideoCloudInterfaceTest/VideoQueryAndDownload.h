#pragma once
#include "afxdtctl.h"
#include "hcs_sdk.h"
#include <string>
#include <vector>
#include "afxcmn.h"
#include "afxwin.h"
#include "plaympeg4.h"
#define MAX_LEN    256
#define MAX_HEAD_LEN 40 * 1024
#define MAX_PLAYDATA_LEN 256 * 1024
// CVideoQueryAndDownload 对话框

class CVideoQueryAndDownload : public CPropertyPage
{
	DECLARE_DYNAMIC(CVideoQueryAndDownload)

public:
	CVideoQueryAndDownload();
	virtual ~CVideoQueryAndDownload();

// 对话框数据
	enum { IDD = IDD_DIALOG_VIDEO_QUERY };

protected:
    BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
private:
    CDateTimeCtrl m_DateStart;
    CDateTimeCtrl m_DateEnd;
    CDateTimeCtrl m_TimeStart;
    CDateTimeCtrl m_TimeEnd;
    CString m_camerID;
    CString m_VideoTypeQuery;
    CString m_VideoLockType;
    CString m_VideoTypeDown;
    CString m_VideoOffsetDown;

    unsigned long long m_nLogID;//登录句柄
    std::vector<HCS_RecordInfo> VecRecordInfo;

    CListCtrl m_listVideoQuery;
    int m_ItemIndexInfo;
    CString m_csLockTime;
    int m_handlePlayback;//回放操作句柄
    HANDLE m_hThreadVideoPlayBack;
    volatile int m_nPlayBackMod;
    int m_nPlayBackModCout;
    LONG m_nPort;
    CStatic m_PlayBackShow;
    volatile bool m_bPlayBacking;
    bool m_bIsChangePlayMode;

    int m_handleDownLoad;
    HANDLE m_hThreadVideoDownLoad;
    volatile bool m_bDownLoading;
public:
    char m_FileName[MAX_LEN];
    int m_nFlag;
    int m_nErrorCode;
public:
    afx_msg void OnBnClickedButtonGethead();
    afx_msg void OnBnClickedButtonVideoQuery();
    afx_msg void OnBnClickedButtonDownload();
    afx_msg void OnBnClickedButtonDelVideo();
    afx_msg void OnBnClickedButtonQueryClear();
    afx_msg void OnBnClickedButtonLockVideo();
    afx_msg void OnBnClickedButtonRecordPlayback();
    afx_msg void OnBnClickedButtonStopRecordPlayback();
    afx_msg void OnBnClickedButtonRecordPlaybackMod();
    void SetLoginID(unsigned long long nLogID){m_nLogID = nLogID;}
    void StartVideoPlayBack();
    void VideoDownLoadDeal();
};
