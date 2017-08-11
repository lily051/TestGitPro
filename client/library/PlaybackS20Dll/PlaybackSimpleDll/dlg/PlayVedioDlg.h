/**   @file PlayVedioDlg.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    回放父窗口类.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/10/29
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "PlayWndView.h"
#include "PlayToolBarWnd.h"
#include "../ui/PlaybackTimeBar/PlayBarTimeCtrl_For9000.h"
#include "./VolumnSetWnd.h"
#include <vector>
#include "DownloadDlg.h"

// CPlayVedioDlg 对话框

class CPlayVedioDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayVedioDlg)

public:
	CPlayVedioDlg(HANDLE hDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayVedioDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PLAYVEDIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_hDlg;
	BOOL m_bMove;  //子窗口是否允许移动标识
	BOOL m_bFullScreen; //是否全屏
	CRect m_rcWndPos;  //窗口大小
	CDownloadDlg * m_pDownloadDlg;  //指向唯一的下载窗口
	PB_TIME m_stSpanStart;
	PB_TIME m_stSpanStop;

public:
	CPlayWndView * m_pPlayViewWnd;
	CPlayToolBarWnd * m_pToolBarWnd;
    CPlayBarTimeCtrl_For9000 * m_pPlayBarTimeCtrl;
	CVolumnSetWnd * m_pVolumnSetWnd;
	CWnd* m_pFullScreenWnd;
	CWnd* m_pParentWnd;
	time_t m_tMin;
	time_t m_tMax;

private:
	//创建子窗口
	void CreateChildWnd();
	//窗口移动
	void MoveCtrl();
	//初始化TimeBar
	void InitBarTimeCtrl_For9000();
	//窗口全屏切换
	void WndFullScreenSet();

public:
	// 设置下载对话框指针.
	void SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg);
	// 获取鼠标所在窗口号.
	int GetCursorWnd();
	// 获取选中的窗口号.
	int GetSelectedWnd();
	//监控点回放
	BOOL Playback(
		int nCameraID, 
		const CString& strCamName, 
		const PLAY_PARAM& stPlayParam, 
		PB_LOGININFO& stLoginInfo, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	// 停止回放监控点.
	BOOL StopPlayback(int nCameraID);
	// 停止回放监控点.
	BOOL StopPlayback();
	// 停止回放指定窗口.
	void StopPlaybackByWndNo(int nWndNo = -1);
	// 停止全部回放窗口.
	void StopAll();
	// 获取回放状态信息.
	BOOL GetPlaybackInfo(int nWndNo, PlaybackStatData& stData);
	//回放文件
	BOOL PlaybackFile(
		int nCameraID, 
		const CString& strCamName, 
		int nChannel, 
		const char* pStreamID,
		const PB_FILEINFO& stFileInfo, 
		const PB_LOGININFO& stLoginInfo, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	//回放分组回放
	BOOL PlayGroup(int nGroupID, const PLAY_PARAM& stPlaybackParam);
	// 清空录像彩条.
	void Clear(int nWndID);
	//建立录像彩条
	void BuildRecordGraph(const std::vector<PB_FILEINFO>& vecFileInfo, UINT unElementID = -1);
	//录像类型转换
	int GetGraphRecordType(int nRecordType);
	// 获取文件列表.
	std::vector<PB_FILEINFO>& GetFileInfo(int nWndID);
	// 获取单个文件信息.
	BOOL GetFileInfo(int nWndID, int nIndex, PB_FILEINFO& stFileInfo);
	// 获取光标所在窗口.
	int GetCurSelectedWnd();
	// 刷新窗口.
	void Invalidate();

	//工具栏按钮点击响应消息
	LRESULT OnMsgToolButtonClick(WPARAM wParam, LPARAM lParam);
	//录像彩条的定位消息
	LRESULT OnMsgTimeBarSetPos(WPARAM wParam, LPARAM lParam);
	//工具栏按钮状态刷新消息
	LRESULT OnMsgToolBarUpdate(WPARAM wParam, LPARAM lParam); 
	//全屏切换消息
	LRESULT OnMsgFullScreen(WPARAM wParam, LPARAM lParam);
	//音量浮动窗口关闭消息
	LRESULT OnMsgVolumnWndClose(WPARAM wParam, LPARAM lParam);
	// 播放下一个文件.
	LRESULT OnMsgPlayNextFile(WPARAM wParam, LPARAM lParam); 
	// 刷新分组配置.
	LRESULT OnMsgUpdateGroup(WPARAM wParam, LPARAM lParam);
	// 菜单回调.
	LRESULT OnMenuCallBack(WPARAM wParam, LPARAM lParam);
	// 查找文件成功.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
	// 查找文件失败.
	LRESULT OnMsgFindFail(WPARAM wParam, LPARAM lParam);
	// 文件列表切换.
	LRESULT OnFileListChange(WPARAM wParam, LPARAM lParam);
	// 按时间下载.
	LRESULT OnMsgMiniToolbarDownload(WPARAM wParam, LPARAM lParam);
	// 回放状态
	LRESULT OnMsgPlayBackState(WPARAM wParam, LPARAM lParam);
	

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
