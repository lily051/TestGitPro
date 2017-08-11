/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			RealPlayViewWnd.h
Description:  		预览窗口
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-4-19     qmx    创建
*****************************************************/
#pragma once
#include "ui/CustomDrawBlackWnd.h"
#include "RealWndToolBar.h"
#include "RealWaveDlg.h"
#include "CaptureTipDlg.h"
#include "util/Markup.h"


// CRealPlayViewWnd

class CRealPlayViewWnd : public CCustomDrawBlackWnd
{
	DECLARE_DYNAMIC(CRealPlayViewWnd)

public:
	CRealPlayViewWnd();
	virtual ~CRealPlayViewWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bIsPreviewing;                       //是否正在预览
	BOOL m_bIsRecording;						//是否在录像
	BOOL m_bIsSounding;						    //音频是否开启
	BOOL m_bIsOsdShowing;                       //OSD是否显示
	BOOL m_bIsOsdShow4Row;                      //OSD4是否4行显示
	int  m_nCamType;                            //监控点类型
	BOOL m_bFullScreen;                         //是否全屏状态
	BOOL m_bIsSteaming;                         //是否取流成功
	int  m_nCurStreamType;                      //当前码流类型: 0-主码流，1-子码流
	int  m_nOsdInfoShowNum;                     //OSD信息显示行数
	BOOL m_bIsRecordNormal;                     //录像是否正常
	BOOL m_bIsOpenPSClient;                     //是否开启了录音通信

	CString m_strShowMsg;						//显示在窗口上的取流信息	
	CString m_strCamName;						//监控点名称	
	int m_nDeviceType;							//设备类型
	HANDLE m_hPreviewhandle;					//取流句柄
	PVOID m_pStreamHead;						//开始播放的头数据
	DWORD m_dwHeadSize;							//开始播放头数据大小
	HANDLE m_hRecordFile;						//保存录像的文件句柄
	LONG m_lPort;								//播放Port
	PLAYERTYPE m_playerType;					//解码类型
	CFont m_FontSmall;							//大字体
	CFont m_FontBig;							//小字体
	CFont* m_pTextFont;							//指向大/小字体

	BOOL m_bIsMouseHover;						//鼠标停留在窗口中

	NET_DVR_SHOWSTRING_V30 m_stShowString;      //OSD信息设置
	PV_EVENT_FUNC m_pvFunc;
	void* m_pCameraDS;

	CCaptureTipDlg *m_pCapDlg;                  //抓图提示对话框
	CRect m_rcClient;							//窗口位置（0，0左上角）
	CRealWndToolBar *m_pWndToolBar;			    //窗口内工具条
	BOOL m_bFirstCleanup;                       //首次清理预览界面信息
	PREVIEW_CAMINFO m_stCamInfo;                //监控点信息
	HWND m_hPlayViewWnd;                        //CPlayViewWnd窗口句柄
	CTime m_dtRecordTime;
	CTime m_dtRecordStartCurTime;
	time_t m_ttStart;
	time_t m_ttEnd;
	LONG m_lUserID;								//登陆设备UserID
	NET_DVR_DEVICEINFO_V30 m_stDevInfo;	        //登陆设备信息
	RECOED_FILE_INFO m_stLastRecordFile;
	int m_nCamID;								//正在预览监控点ID (成功预览的监控点)

	UINT m_uiWndIndex;							//窗口索引号
	CScope *m_pVideoWav;						// 视频波形控件
	CScope *m_pAudioWav;						// 音频波形控件
	CString m_strOsdInfo;
	DWORD m_dwRecFileSize;                      //录像文件长度
	BOOL m_bFirstShow;                          //超时第一次提示
	COleDateTime m_dtRecordStartTime;
	int m_nRecordTimeOut; //总共超时时间（会累加）
	int m_nRecordTimeOutOld;  //原始超时时间（由参数传入，程序不修改）
	int m_nDelayTime;
	BOOL m_bIsShowCaptureInfo;                  //是否显示抓图信息
	LONG m_lLastStreamTime;                     //设备最后一次来流时间
	time_t m_tmOld;  //记录上次点击录像的时间
    LONG m_lUSBPort;
    BOOL m_bProIsSound;       //前一个音频状态
public:
	volatile HANDLE m_hPhoneLogin;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg LRESULT OnToolBarBtnClk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetException(WPARAM wParam, LPARAM lParam);

	//创建窗口
	BOOL CreateWnd(const RECT & rect, CWnd * pParent,unsigned int uiID,unsigned int uiIndex);

	//生成取流参数
	BOOL PreparStreamInfo(const PREVIEW_CAMINFO &struPreviewInfo, STREAMPARAMS &stStreamParam);

	//开始预览
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//停止预览
	BOOL StopPreview();

	//取流回调函数
	static void CALLBACK PreviewStreamCallback(HANDLE hStream, HANDLE hUserData, STREAM_DATA_INFO * pStreamDataInfo);
    static void CALLBACK DecCallBack (LONG nPort, unsigned char *pBuf, long nSize, FRAME_INFO *pFrameInfo, void* pUser);

	//塞入流数据
	BOOL InputData(PBYTE pBuffer,DWORD dwSize,long lFrameType);

	//播放库开始解码
	BOOL StartPlay(PLAYERTYPE playerType, PBYTE pBuffer, DWORD dwBufferSize,CString strCamName = _T(""));

	//播放库停止解码
	void StopPlay();

	//设备类型转换成解码类型
	PLAYERTYPE GetPlayerType(int nDevType);

	//播放画面dc回调函数
	static void CALLBACK DrawFunMsgCB(long nPort,HDC hDC,long lUser);

	//流画面上显示提示信息
	void DrawMyText(CDC * pDC,CString csDrawText,CRect rcDraw);

	//设置事件回调函数
	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	//获取录像文件信息
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL StopRecordByCaminfo(const PREVIEW_CAMINFO &struPreviewInfo);

	//是否在录像
	BOOL IsRecord(){return m_bIsRecording;}

	//是否在预览
	BOOL IsPreview(){return m_bIsPreviewing;}

	//判断音频是否开启
	BOOL IsSound(){return m_bIsSounding;}

	//判断OSD是否显示
	BOOL IsShowOsd(){return m_bIsOsdShowing;}

	//判断OSD是否显示
	BOOL IsShow4Osd(){return m_bIsOsdShow4Row;}

	//设置窗口索引号
	void SetWndowIndex(int iIndex){m_uiWndIndex = iIndex;}

	//获取正在预览的监控点ID
	int GetCamID(){return m_nCamID;};

	//设置要显示的取流消息
	void SetShowMsg(int nMsgID);
    void SetShowMsg(const CString &strShowMsg);

	//右键弹出菜单
	void PopupMenu(BOOL bJustClose = FALSE);

	//执行菜单
	void ExcuteSelectOption(int nCmd);

	//工具条显示控制
	void ShowToolBar(BOOL bShow);
	void HideToolBar();

    //获取监控点名称
    CString GetChanName();

	//预览取流消息回调
	static void CALLBACK PreviewMsgNotifyCallback(HANDLE hStream, HANDLE hUserData, int nMsgID);

    /** @fn         CheckOperateRights
     *  @brief      根据菜单项检查是否有操作权限
     *  @param[IN]  nMenuID: 焦点播放窗口
     *  @return     TRUE: 有，FALSE: 无
     */
    BOOL CheckOperateRights(int nMenuID);
    
    /** @fn         SetFullScreenStatus
     *  @brief      设置全屏状态
     *  @param[IN]  bFullScreen: 是否全屏
     *  @return     void
     */
    void SetFullScreenStatus(BOOL bFullScreen = FALSE); 

    // 录像数据处理
    BOOL DealDecodeData(long nPort,char * pBuf,long nSize,void * pFrameInfo);

	// 录音数据流回调处理
	void StreamDataProc(long datatype, void *pBuffer, DWORD dwSize);

	// 录音错误消息回调处理
	void PhoneErrorProc(int nErrortype);

	// 录像错误消息回调处理
	void VideoErrorProc(int nErrortype);

	void SetFirstWndWave(CScope *pVideoWav, CScope *pAudioWav);

	// 打开录音通信
	BOOL OpenPSCLient(const PREVIEW_CAMINFO &struPreviewInfo);

	// 关闭录音通信
	void ClosePSCLient();

	// 打开录像通信
	BOOL OpenVSClient(const PREVIEW_CAMINFO &struPreviewInfo);

	// 关闭录像通信
	BOOL CloseVSClient();

	//开始录像
	BOOL StartRecord();

	//停止录像
	BOOL StopRecord();

	//开启音频
	BOOL OpenSound();

	//关闭音频
	BOOL CloseSound();

	// 主子码流切换
	void StreamTypeSwitch(const int nStreamType);

	//开始预览时设置osd的状态
	void SetOsdState();

	// 显示OSD
	void ShowOSD();

	// 隐藏OSD
	void HideOSD();

	//设置OSD信息
	BOOL SetOSDInfo(const CString &strOsdInfo);

	 /* 
	 *  @fn         CaptureByMyself
     *  @brief      抓图
     *  @return     TRUE: 成功，FALSE: 失败
     */
    BOOL CapturePic();

	//USB开始预览
	BOOL USBStartPreview(const PREVIEW_CAMINFO& stUSBPreInfo);

	//USB停止预览
	BOOL USBStopPreview(LONG lCameraIndex);

	//USB开始录像
	BOOL USBStartRecord();

	//USB停止录像
	BOOL USBStopRecord();

	//USB摄像头取流回调函数
	static void CALLBACK USBPreviewStreamCallback(OUTPUTDATA_INFO *pDataInfo, unsigned long pUser);

    // 获取指定dvr当前时间戳
    BOOL GetDvrTimeStamp(long lUserId, time_t &iDvrTimeStamp, DWORD& dwError);
};