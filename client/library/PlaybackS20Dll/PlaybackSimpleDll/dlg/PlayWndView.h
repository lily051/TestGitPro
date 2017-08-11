/**   @file PlayWndView.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    多回放画面窗口类.
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
#include "./PlaybackWnd/PlaybackWnd.h"

// CPlayWndDlg 对话框

class CPlayWndView : public CWnd
{
	DECLARE_DYNAMIC(CPlayWndView)
public:
	CPlayWndView(HANDLE hDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayWndView();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CPlaybackWnd* m_pPlayWnd[MAX_PLAYWND_NUM];
	BOOL m_bMove;    //是否允许子控件移动
	PLAY_LAYOUT_WND m_curLayWnd;
	//CWnd* m_pFullScreenWnd;  //全屏窗口载体
	HANDLE m_hDlg;	 // 对话框句柄.
public:
	UINT m_unSelectWnd;    //当前选中的窗口
	UINT m_unFullWndID;    //双击放大的窗口
	BOOL m_bSynPlay;       //当前是否同步回放在
	BOOL m_bReversePlay;   // 是否正在倒放.
	BOOL m_bSynTimer;      //同步定时器是否开启
private:
	//移动子窗口
	void MoveWnd();

	//接收单个窗口选中的消息
	LRESULT OnMsgWndSelect(WPARAM wParam, LPARAM lParam);
	//接收单个窗口的双击消息
	LRESULT OnMsgWndDbClick(WPARAM wParam, LPARAM lParam);
	//单个窗口全屏消息
	LRESULT OnMsgFullScreen(WPARAM wParam, LPARAM lParam);
	//单个窗口回放状态改变消息
	LRESULT OnMsgPlayStateUpdate(WPARAM wParam, LPARAM lParam);
	// 播放下一个文件.
	LRESULT OnMsgPlayNextFile(WPARAM wParam, LPARAM lParam);
	// 菜单事件.
	LRESULT OnMenuCallBack(WPARAM wParam, LPARAM lParam);
	// 响应查找文件成功消息.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
	// 响应查找文件失败消息.
	LRESULT OnMsgFindFail(WPARAM wParam, LPARAM lParam);
	// 关闭其他窗口的音频.
	//LRESULT OnMsgMiniToolbarOpenSound(WPARAM wParam, LPARAM lParam);
	// 按时间下载.
	LRESULT OnMsgMiniToolbarDownload(WPARAM wParam, LPARAM lParam);
	// 播放速度改变.
	LRESULT OnMsgPlaybackSpeedChange(WPARAM wParam, LPARAM lParam);
	// 单帧播放.
	LRESULT OnMsgPlaybackSingleFrame(WPARAM wParam, LPARAM lParam);

public:
    // 关闭其他窗口的音频.
    void OnMsgMiniToolbarOpenSound(int nWndID);

	//时间回放
	BOOL PlaybackTime(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		const FINDFILE_PARAM& stFindParam,
		CString strChanName, 
		int nCameraID, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	//文件回放
	BOOL PlaybackFile(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_FILE_INFO& stPlaybackParam, 
		CString strChanName, 
		int nCameraID, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	//停止回放
	void StopPlay(int nWndID = -1);
	// 停止回放监控点.
	void StopPlayCam(int nCameraID);
	// 停止回放监控点.
	void StopPlayCam();
	//停止全部
	void StopAll();
    //全部抓图
    void CaptureAll();
	// 获取回放状态信息.
	BOOL GetPlaybackInfo(int nWndNo, PlaybackStatData& stData);
	//回放定位
	BOOL SetPlayTime(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nWndID);
	//按钮点击事件
	void OnButtonClick(int nBtnID, LONG lValue);

	//设置窗口分屏
	void SetWndLayout(PLAY_LAYOUT_WND emLayoutWnd);
	//窗口提示字符
	void ShowWndInfo(CString strInfo, int nWndID = -1);
	//设置OSD回调函数
	void SetOsdCallback(pFunMsgOsdTime pCallbackFun, LPVOID lpvoid);
	//获取当前选中窗口音量大小
	BOOL GetCurSelWndVolumn(BOOL & bVoice, int& nVolumn);
	//获取当前鼠标所在分屏
	int GetCursorWnd();
	// 获取当前选中的分屏.
	int GetCurSelectedWnd();
	// 获取文件列表.
	std::vector<PB_FILEINFO>& GetFileInfo(int nWndID);
	// 获取单个文件信息.
	BOOL GetFileInfo(int nWndID, int nIndex, PB_FILEINFO& stFileInfo);
	// 获取监控点ID.
	int GetCameraID(int nWndID);
	// 获取保留信息.
	DWORD GetRevInfo(int nWndID);
	// 获取监控点名称.
	CString& GetCamName(int nWndID);
	// 获取通道号.
	LONG GetChannel(int nWndID);
	// 获取权限.
	DWORD GetRight(int nWndID);
	// 获取回放类型.
	ENUM_PLAY_TYPE GetPlayType(int nWndID);
	// 获取登录信息.
	PB_LOGININFO& GetLoginInfo(int nWndID);
	// 更新按钮tips.
	void UpdateButtonTips();
	// 设置窗口标题.
	void Invalidate(int nWndID);
	// 是否是倒放状态.
	BOOL IsReversePlay(int nWndID);
	// 获取当前播放文件序号.
	BOOL GetCurPlayFileIndex(int nWndID, int& nPlayIndex);
	// 设置当前播放文件序号.
	void SetCurPlayFileIndex(int nWndID, int nPlayIndex);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
