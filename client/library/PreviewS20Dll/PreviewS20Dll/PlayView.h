/********************************************************************
    Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.	
	created:	2009/10/09   21:07 
	filename:	playview.h
	author:		GuoLinlin
    Description:  
	Modification History:      
	<version>   <time>        <author>        <desc>	
	   1.0      2009-10-9    GuoLinlin       Created	   
	   1.0      2009-10-9    GuoLinlin       Modify	
	   3.0      2012-4-18    nobody			 Modify	
*********************************************************************/
#pragma once
#include "RealPlayViewWnd.h"
#include "PlayView_WndArrangeOper.h"

#define  PREVIEW_WND_MAXNUM	1 //播放窗口最大数

class CPlayView : public CWnd,public CPlayView_WndArrangeOper
{
	DECLARE_DYNAMIC(CPlayView)

public:
	CPlayView();
	virtual ~CPlayView();

protected:
	DECLARE_MESSAGE_MAP()

private:
	int m_nStartWndIndex;									//起始窗口索引号
	int m_nMaxWndIndex;										//最大窗口索引号
	int m_nFocusWndIndex;									//焦点窗口索引序号
    BOOL m_bFullScreen;                                     //是否全屏状态
	CRealPlayViewWnd *m_pRealPlayWnd[PREVIEW_WND_MAXNUM];	//播放窗口

public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//创建窗口
	BOOL CreateWnd(const RECT & rect, 
		CWnd * pParent, unsigned int uiID,unsigned int uiStartWndIndex = 0,
		unsigned int uiMaxWndNum = PREVIEW_WND_MAXNUM);

	//创建子资源
	void CreateChildWnd();

	//销毁子资源
	void DestroyChildWnd(void);

	//设置焦点窗口
	void SetChildWndZoom(int nWndIndex);

	//窗口最大
	BOOL SetChildWndEnlarge();

	//画窗口边框，焦点窗口边框是红色
	void DrawOutputBorder(void);

	//初始化预览窗口位置
	void InitPose(void);

	//设置画面分割模式
	BOOL SetLayoutMode(WORD wMode);

	//获取鼠标所在窗口的索引号
	int GetHoverWndIndex(void);

	//焦点窗口预览
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//鼠标停留窗口预览
	BOOL StartPreviewMouseHover(const PREVIEW_CAMINFO &struPreviewInfo);

	//指定窗口预览
	BOOL StartPreviewByWndIndex(UINT nWndIndex, const PREVIEW_CAMINFO &struPreviewInfo, 
		int nPresetIndex = -1, BOOL bWriteLog = FALSE);

	//指定窗口停止预览
	BOOL StopPreviewByWndIndex(UINT nWndIndex, int nPresetIndex = -1);

	//停止预览
	void StopPreview(void);

	//获取焦点窗口监控点ID
	int GetCamIDFromFocusWnd();

	//获取焦点窗口指针
	CWnd* GetFocusWndPointer();

	//预览窗口工具条显示控制
	void ShowToolBar(int nWndIndex);

    /** @fn         IsWndPreview
     *  @brief      检查指定窗口是否正在预览
     *  @param[IN]  uWndIndex: 窗口索引
     *  @return     TRUE: 正在预览，FALSE: 空闲
     */
    BOOL IsWndPreview(UINT uWndIndex);

    /** @fn         BwEnforceStopPreview
     *  @brief      带宽控制服务器强制关闭指定监控点的预览
     *  @param[IN]  nCamID: 指定监控点
     *  @return     void
     */
    void BwEnforceStopPreview(int nCamID);

    ///** @fn         BwReRequestPreview
    // *  @brief      客户端重连上带宽控制服务器后重新请求正在预览的监控点
    // *  @return     void
    // */
    //void BwReRequestPreview();

    /** @fn         CheckCameraIsPlaying
     *  @brief      检查指定监控点是否有在预览
     *  @param[IN]  nCamID: 指定监控点ID
     *  @return     TRUE: 有，FALSE: 无
     */
    BOOL CheckCameraIsPlaying(int nCamID);

    /** @fn         SetFullScreenStatus
     *  @brief      设置全屏状态
     *  @param[IN]  bFullScreen: 是否全屏
     *  @return     void
     */
    void SetFullScreenStatus(BOOL bFullScreen = FALSE);

	//开始录像
	BOOL StartRecord();

	//停止录像
	BOOL StopRecord();

	//开启音频
    BOOL OpenSound();

	//关闭音频
	BOOL CloseSound();

	//获取录像文件信息
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	void SetFirstWndWave(CScope *pVideoWav, CScope *pAudioWav);

	//设置OSD显示信息
	BOOL SetShowString(const CString &strBusinessInfo);

	void OnToolBarExecute(int nBtnType);
};