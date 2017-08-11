#pragma once
#include "afxcmn.h"
#include "PlayVedioDlg.h"
#include "DownloadDlg.h"
// #include "TreeParamDlg.h"
// #include "LabelMgr.h"
// #include "SubSerachDlg.h"
// #include "PlaybackCfg.h"
#include "LocalPlayback.h"
#include "../ui/uiBase/SkinTabCtrl.h"


// CPlaybackMain 对话框

class CPlaybackMain : public CDialog
{
	DECLARE_DYNAMIC(CPlaybackMain)

public:
	CPlaybackMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlaybackMain();

// 对话框数据
	enum { IDD = IDD_DIALOG_PLAYBACK_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	MAIN_TAB_ITEM m_emShowDlg;

	// 回放回调函数.
	PBSD_PlaybackCB m_pfnPlayBackCB;
	// 菜单回调函数.
	PBSD_MenuCB m_pfnMenuCB;
	// 回放自定义数据.
	LPVOID m_pPlaybackUserData;
	// 用户自定义数据.
	LPVOID m_pUserData;
	// 写操作日志用户自定义数据.
	LPVOID m_pWorkLogUserData;
	BOOL m_bIsLocalPlayback;

public:
	CPlayVedioDlg* m_pDlgPlayVedio;
	CDownloadDlg* m_pDownload;
	//CTreeParamDlg* m_pDlgTreeParam;
	//CLabelMgr* m_pDlgLabelMgr;
	//CSubSerachDlg* m_pDlgSubSerach;
	//CPlaybackCfg* m_pDlgPlaybackCfg;
	CLocalPlayback* m_pDlgLocalPlayback;

private:
	//子窗口创建
	void CreateChildWnd();
	//移动控件
	void MoveCtrl();
	//tab控件初始化
	BOOL InitTab();
	// 改变显示的标签页.
	void ChangeTab(int nItem);
	// 监控点停止回放.
	void StopPlayCam(int nResID);
	// 构建XML报文.
	BOOL BuildFileXML(int nWnd, const std::vector<PB_FILEINFO>& vecFileList, CString& strXML);

public:
	// 监控点回放.
	BOOL PlaybackCam(LPPBSDLoginParam pLoginParam, LPPBSDPlayParam pPlayParam, int nWnd = -1, DWORD dwRight = PBSD_RIGHT_ALL);
	// 监控点回放.
	BOOL LocalPlayback(const int nVideoType, const CString csFilePath);
	// 停止指定的窗口.
	void StopPlayByWndNo(int nWndNo = -1);
	// 停止全部窗口.
	void StopAll();
	// 设置回放回调函数.
	void SetPlaybackCB(PBSD_PlaybackCB pfnPlaybackCB, LPVOID pUserData);
	// 设置菜单回调函数.
	void SetMenuCB(PBSD_MenuCB pfnMenuCB, LPVOID pUserData);
	// 设置写操作日志回调.
	void SetWorkLogCallBack(PBSD_WorkLogCB pfnWorkLogCB, LPVOID pUserData);
	// 获取回放状态信息.
	BOOL GetPlaybackInfo(int nWndNo, PlaybackStatData& stData);
	// 监控点停止回放.
	void StopPlayCam();
	// 刷新窗口.
	void Invalidate();
public:

	//树窗口监控点确定/双击信息
	LRESULT OnMsgCameraPlay(WPARAM wParam, LPARAM lParam);
	//树窗口监控点拖拽消息
	LRESULT OnMsgCameraDrag(WPARAM wParam, LPARAM lParam);
	//文件下载响应消息
	LRESULT OnMsgFileDownload(WPARAM wParam, LPARAM lParam);
	//事件通知下载消息
	LRESULT OnMsgDownloadNotify(WPARAM wParam, LPARAM lParam);
	// 带宽控制通知消息，停止回放.
	LRESULT OnMsgBWControlStopPlay(WPARAM wParam, LPARAM lParam);
	// 刷新回放分组.
	LRESULT OnUpdateGroup(WPARAM wParam, LPARAM lParam);
	// 菜单回调.
	LRESULT OnMenuCallBack(WPARAM wParam, LPARAM lParam);
	// 查找文件成功.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
    // 文件列表切换显示.
    LRESULT OnFileListChange(WPARAM wParam, LPARAM lParam);
public:
	CSkinTabCtrl m_tabPlay;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabPlay(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnDownloadFilePlay(WPARAM wParam, LPARAM lParam);
};
