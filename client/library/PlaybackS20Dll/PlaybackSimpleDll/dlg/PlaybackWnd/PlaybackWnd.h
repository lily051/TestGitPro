#pragma once
#include "PlayTitleWnd.h"
#include "PlayVedioWnd.h"
#include "../PlaybackOper.h"
#include "PlayMiniToolBarWnd.h"
#include "PlayMiniToolBarExpandWnd.h"
#include "../CaptureTipDlg.h"
#include "../SaveDataDlg.h"
#include <vector>
#include "../OrderPlay.h"

/*****************************************************************
*   Copyright 2011-2013 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlaybackWnd.h
*   Description:    回放控件窗口类，该类即可为一个回放的窗口
*   Author:         yudan
*   Date:			2011-1-27 16:32
*   Modification History:
*    <version>      <time>          <author>        <desc>
      v1.1          2011-1-27 16:33   yudan          create
	  v1.2          2012-9-28 11:01   yudan  再见金融 modify
	  v1.3			2013-05-14 09:27  zpz			 modify
*****************************************************************/

//定时器相关
#define TIMER_GET_PLAYPOS 1905
#define TIMER_GET_PLAYOSDTIME 1907
#define GET_PLAYPOS_TIME 1000
#define GET_PLAYOSDTIME_TIME 1000
//menu菜单ID
enum
{
	MENU_ID_SHOWTITLE = WM_USER + 1800,
	MENU_ID_ORIGINAL = WM_USER + 1801,			// 原始比例.
	MENU_ID_DOWNLOADLIST = WM_USER + 1803,
	MENU_ID_PLAYAGAIN = WM_USER + 1804,
	MENU_ID_4_3 = WM_USER + 1805,				// 4:3.
	MENU_ID_16_9 = WM_USER + 1806,
	MENU_ID_PLAY = WM_USER + 1807,
	MENU_ID_STOP = WM_USER + 1808,
	MENU_ID_SLOW = WM_USER + 1809,
	MENU_ID_FAST = WM_USER + 1810,
	MENU_ID_SINGLEFRAME = WM_USER + 1811,
	MENU_ID_DOWNLOAD_BY_TIME = WM_USER + 1812,
	MENU_ID_CAPTURE = WM_USER + 1813,
	MENU_ID_SAVE = WM_USER + 1814,
	MENU_ID_OPENSOUND = WM_USER + 1815,
    MENU_ID_CONVERTPLAY = WM_USER + 1816,
	MENU_ID_FULLSCREEN = WM_USER + 1817,
};

enum
{
	OPERA_ID_CAPTURE			= 0,
	OPERA_ID_SLOW ,
	OPERA_ID_PLAY,
	OPERA_ID_FAST,
	OPERA_ID_SOUND,
	OPERA_ID_FULLSCREEN,
	OPERA_ID_STOP,
	OPERA_ID_SLIDER,
	OPERA_ID_SOUNDVALUE,
	OPERA_ID_NOTICE
};

// 响应线程修改界面信息.
#define WM_UPDATE_MSG (WM_USER + 1900)


//颜色定义
#define COLOR_BORDER_SELECT UI_PLAYWNDSEL_FRAME  //选中状态颜色
#define COLOR_BORDER_NOSELECT UI_PLAYWND_SPACE  //非选中状态下颜色
//数据定义
#define TITLE_WND_HEIGHT 16      //标题栏高度


typedef enum _tagPlayType
{
	PLAY_TYPE_TIME = 1,  //按时间回放
	PLAY_TYPE_FILE,      //按文件回放
}PLAY_TYPE;

/** @fn pOnMsgNeedRsp
*   @brief osd时间
*   @param[in] lOsdTime: osd时间
*   @param[in] unWndID: 窗口ID
*/
typedef void (CALLBACK *pFunMsgOsdTime)(LONG lOsdTime, UINT unWndID, LPVOID lpvoid);


class CPlaybackWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlaybackWnd)
public:
	CPlaybackWnd(HANDLE hDLg, void* pParent = NULL);
	~CPlaybackWnd();

private:
    void* m_pParent;				 //父窗口指针
	CPlaybackOper m_opePB;			 // 回放控制类.
	COrderPlay m_OrderPlay;			 // 连续回放文件控制类.
	COLORREF m_clrBorder;            //边框颜色
	BOOL m_bIsMax;                   //控件是否最大化
	BOOL m_bMove;                    //是否允许移动控件

	HANDLE m_hDlg;					 // 对话框句柄.
	CPlayTitleWnd* m_pPlayTitleWnd;  //回放窗口标题控件
	CPlayVedioWnd* m_pPlayVedioWnd;  //回放窗口视频控件
	CPlayMiniToolBarWnd* m_pToolBarWnd; // 回放窗口工具栏.
	CPlayMiniToolBarExpandWnd* m_pToolBarExpandWnd;	// 回放扩展工具栏.
	CCaptureTipDlg * m_pCaptureTip;  //图片展示窗口
	CSaveDataDlg* m_pSaveData;		// 回放剪辑消息窗口.
	CButtonST_Skin *m_pbtnClose;		 //关闭窗口按钮
	BOOL m_bShowTitle;               //是否显示标题
	CRect m_rcClient;                //窗口大小
	ShowProportion m_emShowPro;      // 显示比例.
	BOOL m_bIsInitTask;			     // 是否初始化了任务.
public:
	UINT m_unWndID;                  //窗口绑定序号
	HWND m_hRecvMsg;                 //消息接收窗口句柄
	BOOL m_bFullScreen;              //是否全屏在
	BOOL m_bSynPlay;                 //是否同步回放中
	BOOL m_bReversePlay;			 // 是否是倒放模式.
	BOOL m_bSelect;                  //当前窗口是否被选中
	BOOL m_bUsed;                    //当前窗口是否被使用过

    //获取设备录像相关能力
    CString m_strAbilityXml;
    void GetDeviceRecordAbility();
    void ParseDeviceRecordAbility();

public:
	BOOL m_bPause;                   //是否暂停中
	BOOL m_bSoundOpen;               //声音是否打开
	BOOL m_bSaveData;                //是否保存数据中
	BOOL m_bSingleSound;             //是否是单音频播放
 	ENUM_CONTROL_TYPE m_emPlaySpeed; //回放速度
	ENUM_CONTROL_TYPE m_emSynPlaySpeed;	// 同步回放速度.
	HANDLE m_hPlayback;              //回放返回ID
	PB_TIME m_tStartTime;            //当前回放开始时间(按时间回放有效)
	PB_TIME m_tEndTime;              //当前回放结束时间(按时间回放有效)
	LONG m_lEndTime;                 //当前回放结束时间
	LONG m_lBeginTime;				 // 当前回放开始时间.
	CString m_strPlayFile;           //当前回放文件名(按文件回放有效)
	CString m_strChanName;           //当前回放的监控点名称
	LONG m_lPlayPos;                 //回放进度
	int m_nVolumn;                   //当前音量
	PB_LOGININFO m_stLoginInfo;      //设备登录参数
	PLAYBACK_TIME_INFO m_stPlayTimeParam;  //时间回放参数
	PLAYBACK_FILE_INFO m_stPlayFileParam;  //文件回放参数
	FINDFILE_PARAM m_stFindParam;	 // 查找文件参数.
	int m_nCameraID;                 //当前回放信息的监控点ID
	LONG m_lChannel;					 // 挡墙回放信息的通道号.
	PLAY_TYPE m_emPbType;          //回放参数
	ENUM_PLAY_TYPE m_emPlayType;   // 回放种类.
	pFunMsgOsdTime m_pSetOsdTime;    //设置OSD时间回调
	LPVOID m_lpSetOsdTime;           //设置OSD回调自定义参数
	LONG m_lOsdTime;                 //当前播放的osd时间
	BOOL m_bOperating;				// 是否正在操作中.
	BOOL m_bIsSingleFrame;			// 是否正在进行单帧播放.
	DWORD m_dwRight;				// 权限控制.
	DWORD m_dwRev;					// 预留字段.
	CString m_strSaveDataPath;		// IE保护模式下的剪辑临时路径.
	HANDLE m_hSaveDataState;		// IE保护模式下的剪辑文件对话框句柄.
	CString m_strFilePath;			// 回放剪辑文件路径.
	HANDLE m_hCheckSpaceThread;		 // 检查磁盘空间线程.
	HANDLE m_hStopCheck;			 // 停止检查磁盘空间.
	ThreadDataCheckSpace m_stCheckData;	// 磁盘检测线程参数.
	BOOL m_bPlayNextFile;			// 自动播放下一个文件.
//回放处理类
public:
	static void CALLBACK DrawOnWnd(LONG nPort, HDC hDC, LONG nUser);
	//窗口参数初始话
	void InitWnd(UINT unWndID, PLAY_LAYOUT_WND emLayoutWnd);
	//窗口清理释放
	void clear();
 	//按时间回放
 	BOOL PlaybackByTime(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		const FINDFILE_PARAM& stFindParam, 
		CString strChanName, 
		int nCameraID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
 	//按文件回放
 	BOOL PlaybackByFile(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_FILE_INFO& stPlaybackParam, 
		CString strChanName, 
		int nCameraID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
 	//回放改变进度
 	BOOL PlaybackSetPos(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime);
 	//停止回放
 	BOOL StopPlayback(BOOL bAutoStop = FALSE, BWControlType emBwControl = BWCONTROL_NOCONTROL);
	// 程序退出时停止回放.
	BOOL DestroyStopPlayback();
 	//暂停回放
 	BOOL PausePlayback();
 	//恢复回放
 	BOOL RePlayback();
 	//改变回放速度
 	BOOL SetPlaySpeed(ENUM_CONTROL_TYPE enumPlaySpeed, BOOL bSynPlay = FALSE);
	// 获取回放速度.
	ENUM_CONTROL_TYPE GetSynPlaySpeed() const;
 	//打开声音
 	BOOL OpenPlaySound(DWORD dwSound = 50);
 	//关闭声音
 	BOOL ClosePlaySound();
 	//改变音量大小
 	BOOL SetPlayVolume(DWORD dwSound);
	//截图
	BOOL CapturePic(CString& strPic);
 	//保存数据
 	int SaveData(CString& strPath);
 	//停止保存数据
 	BOOL StopSaveData();
	// 获取回放状态信息.
	BOOL GetPlaybackInfo(PlaybackStatData& stData);
	//设置osd回调信息
	void SetOsdCallback(pFunMsgOsdTime pSetOsdTime, LPVOID lpvoid);
	//设置是否同步
	void SetSynPlay(BOOL bSynPlay);
	// 单帧播放.
	void PlaySingleFrame(BOOL bPlay, BOOL bSynPlay = FALSE);
	// 是否正在单帧回放.
	BOOL IsSingleFrame() const;
	// 是否正在进行回放.
	BOOL IsPlayback();
	// 是否已经使用过该窗口.
	BOOL IsUsed();
	// 获取监控点ID.
	int GetCameraID();
	// 获取保留字段信息.
	DWORD GetRevInfo();
	// 获取文件列表.
	std::vector<PB_FILEINFO>& GetFileInfo();
	// 设置文件列表.
	BOOL SetFileInfo(const std::vector<PB_FILEINFO>& vFileInfo);
	// 获取单个文件信息
	BOOL GetFileInfo(int nIndex, PB_FILEINFO& stFileInfo);
	// 获取权限.
	DWORD GetRight();
	// 获取时间信息.
	PLAYBACK_TIME_INFO& GetPlayTimeInfo();
	// 获取文件信息.
	PLAYBACK_FILE_INFO& GetPlayFileInfo();
	// 获取文件查找信息.
	FINDFILE_PARAM& GetFindInf();
	// 获取监控点名称.
	CString& GetCamName();
	// 获取通道号.
	LONG GetChannel();
	// 获取回放类型.
	ENUM_PLAY_TYPE GetPlayType();
	// 获取登录信息.
	PB_LOGININFO& GetLoginInfo();
	// 更新按钮tips.
	void UpdateButtonTips();
	// 重复回放.
	void PlayAgain();
	// 重置回放.
	void Invalidate();
	// 是否是倒放状态.
	BOOL IsReversePlay();
	// 获取当前播放文件序号.
	BOOL GetCurPlayFileIndex(int& nPlayIndex);
	// 设置当前播放文件序号.
	void SetCurPlayFileIndex(int nPlayIndex);
	// 减速播放.
	void PlaySlow();
	// 加速播放.
	void PlayFast();

private:
	//开始回放
	void StartPlay(BOOL bFindFile);
 	//获取回放OSD时间
    LONG GetOsdTime();
 	//获取回放进度
 	LONG GetPlayPos();
	//发送按钮状态刷新消息
	void PostUpdateMsg();
	// 隐藏工具栏.
	void HideToolBar();
	// 检查磁盘空间线程函数.
	static DWORD WINAPI ThreadFuncCheckSpace(LPVOID pParam);
	// 显示错误信息.
	void ShowErrorMsg(const CString& strDefMsg);


//控件处理函数
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClosePlayWindow();//停止播放
	LRESULT OnUpdateMsg(WPARAM wParam, LPARAM lParam);
	// 查找文件失败.
	LRESULT OnMsgFindFail(WPARAM wParam, LPARAM lParam);
	// 查找文件成功.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
	// 回放成功.
	LRESULT OnMsgPlaySuccess(WPARAM wParam, LPARAM lParam);
	// 回放失败.
	LRESULT OnMsgPlayFail(WPARAM wParam, LPARAM lParam);
	// 带宽连接限制.
	LRESULT OnMsgBWLimit(WPARAM wParam, LPARAM lParam);
	// 停止回放.
	LRESULT OnMsgStopPlay(WPARAM wParam, LPARAM lParam);
	// 正在连接设备.
	LRESULT OnMsgPlaying(WPARAM wParam, LPARAM lParam);
	// 设置回放进度成功.
	LRESULT OnMsgLocatePlaySuccess(WPARAM wParam, LPARAM lParam);
	// 设置回放进度失败.
	LRESULT OnMsgLocatePlayFail(WPARAM wParam, LPARAM lParam);
	// 带宽控制服务器请求断开回放.
	LRESULT OnMsgBwStopPlay(WPARAM wParam, LPARAM lParam);
	// 鼠标离开视频窗口消息.
	LRESULT OnMsgMouseLeaveMiniToolbar(WPARAM wParam, LPARAM lParam);
	// 鼠标进入视频窗口消息.
	LRESULT OnMsgMouseMoveMiniToolbar(WPARAM wParam, LPARAM lParam);
	// 工具栏按钮点击消息.
	LRESULT OnMsgToolButtonClick(WPARAM wParam, LPARAM lParam);
	// 扩展工具栏关闭消息.
	LRESULT OnMsgToolBarExpandClose(WPARAM wParam, LPARAM lParam);
	// 磁盘空间不足.
	LRESULT OnMsgCheckSpaceNotEnough(WPARAM wParam, LPARAM lParam);
	// 没有找到符合条件的文件个数.
	LRESULT OnMsgFindNoFile(WPARAM wParam, LPARAM lParam);
	// 刷新窗口消息响应.
	LRESULT OnMsgInvalidateWindow(WPARAM wParam, LPARAM lParam);
	
public:
	//标题显示
	void SetTitleText(CString strTitleText);
	//移动控件
	void MoveCtrl();
	//根据回放状态显示标题
	void SetTitleText();
	//获取标题
	CString GetTitleText();
	//显示取消标题栏
	void ShowTitle(BOOL bShowTitle = TRUE);
	//显示窗口文案信息
	void ShowWndInfo(CString strMsg);
	//设置窗口ID
	void SetWndID(UINT unWndID);
    //设置窗口边框颜色
	void SetBorderClr(BOOL bSet);
	//改变消息接收句柄
	BOOL SetMsgRecvHwnd(HWND hWnd);
	//改变视频窗口比例
	void SetVedioOriginal();
	// 设置回放句柄.
	void SetPlayHandle(HANDLE hPlayHandle);

private:
	//边框绘制
	void DrawBorder(COLORREF clrBorder);
	// 设置停止文字.
	void SetStopTextShow(CString strVideo, BOOL bErase = TRUE);
	// 设置窗口叠加文字.
	void SetTextOnWnd(CString strVideo);
	// 工具栏播放.
	void OnMiniToolbarControlPlay();
	// 工具栏按时间下载.
	void OnMiniToolbarDownload();
	// 工具栏保存数据.
	void OnMiniToolbarControlSave();
	// 工具栏扩展.
	void OnMiniToolbarControlExpand();

public:
    // 工具栏截图.
    void OnMiniToolbarControlCapture();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
