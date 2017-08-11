#pragma once
#include "LocalWndTitle.h"
#include "LocalWndView.h"
#include "LocalWndToolBar.h"
#include "CaptureTipDlg.h"


// CLocalVideoFilePlay 对话框

class CLocalVideoFilePlay : public CDialog
{
	DECLARE_DYNAMIC(CLocalVideoFilePlay)

public:
	CLocalVideoFilePlay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLocalVideoFilePlay();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOCALVIDEOFILE_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CLocalWndTitle *m_pLocalWndTitle;
	CLocalWndView *m_pLocalWndView;
	CLocalWndToolBar *m_pLocalWndToolBar;
	CCaptureTipDlg *m_pCaptureTip;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnMsgToolBarOpera(WPARAM wParam, LPARAM lParam);

public:
	// 开始播放
	void StartPlay(const int nVideoType, const CString csFilePath);
	// 停止播放
	void StopPlay();

protected:
	// 创建子窗口
	void CreateWnd(void);
	// 初始化控件位置
	void MoveWnd(void);
	// 将秒数转为00:00:00类型的时间
	CString SecondToStrTime(DWORD seconds);

	// 抓图
	BOOL OnCapture();
	// 全屏
	void OnFullScreen(BOOL bFull);
	// 修改窗口标题
	void OnUpdateWndTitle();

public:
	// 播放端口
	LONG m_lPlayPort;
	//是否需要停止播放
	BOOL m_bStopPlay; 
private:
	// 总时间
	CString m_strTotalTime;
	// 文件名称
	CString m_csFileName;
	// 总时间秒数
	DWORD m_dwTatalSeconds;
	// 是否正在播放
	BOOL m_bIsPlaying;
	// 是否正在暂停
	BOOL m_bIsPausing;
	// 是否音频开启中
	BOOL m_bIsSounding;
	// 是否全屏中
	BOOL m_bIsFullScreening;
	// 倍速
	PLAYSPEED m_nSpeed;


	// 窗口大小
	CRect m_rcWndSize;
	// 全屏窗口句柄
	CWnd* m_pFullScreenWnd;
	// 全屏窗口父窗口句柄
	CWnd* m_pFullScreenParentWnd;
	// 文件路径
	CString m_csFilePath;
	
};
