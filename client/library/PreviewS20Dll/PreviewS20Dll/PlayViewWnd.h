#pragma once
/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			PlayViewWnd.h
Description:  		预览窗口 包括预览播放窗口和预览工具条
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-4-7     qmx    创建
*****************************************************/
// CPlayViewWnd
#include "PlayViewToolbar.h"
#include "PlayView.h"
#include "LayoutMoreWnd.h"


class CPlayViewWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayViewWnd)

public:
	CPlayViewWnd();
	virtual ~CPlayViewWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CWnd *				m_pParentWnd;					//父窗口指针 全屏时需用到
	CWnd *				m_pFullScreenWnd;				//全屏窗口载体
	CPlayView *			m_pPlayView;					//播放窗口
	CPlayViewToolbar *	m_pToolBarWnd;					//工具条
	CLayoutMoreWnd *	m_pLayoutMoreWnd;				//扩展分割模式窗口
	BOOL				m_bWndFullScreen;               //播放窗口是否全屏
	BOOL				m_bIsWndInit;					//窗口是否初始化 控件可移动标志
	WORD				m_wLayoutMode;                  //当前分割模式
	UINT				m_nWndNum;                      //当前窗口数
	CRect				m_rcWndPos;                     //保存窗口全屏前的位置
	PREVIEW_CAMINFO m_stCamInfo;                //监控点信息

public:
	afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdateBtnState(WPARAM wParam, LPARAM lParam);

	//创建窗口
	BOOL CreateWnd(const RECT & rect, CWnd * pParent, unsigned int uiID,DWORD dwSyle,DWORD dwFunStyle = 0);
	
	//创建子资源
	void CreateChildWnd();

	//初始化位置
	void InitPose();

	//设置画面分割模式
	BOOL SetLayoutMode(WORD wMode);
	
	//设置全屏/分割模式
	BOOL SetPlayViewWndFullScreen();

	//响应工具条btn事件
	LRESULT OnToolBarBtnClk(WPARAM wParam, LPARAM lParam);

	//响应工具条btn事件
	LRESULT OnToolBarBtnExecute(WPARAM wParam, LPARAM lParam);

	// 子窗口反射调用工具条
	LRESULT OnToolBarBtnClkRef(WPARAM wParam, LPARAM lParam);
	
	//调出扩展分割窗口
	void OnClkLayoutMoreBtn();

	//销毁扩展分割窗口
	LRESULT OnLayoutMoreWndClose(WPARAM wparam, LPARAM lparam);
	
	//在选中的窗口上预览
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//停止预览
	void StopPreview();

	//开始录像
	BOOL StartRecord();

	//停止录像
	BOOL StopRecord();

	//开启音频
    BOOL StartSound();

	//关闭音频
    BOOL StopSound();

	//获取录像文件信息
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	//显示OSD信息
	BOOL SetShowString(const CString &strBusinessInfo);

	BOOL OpenSound();


private:
	vector<int> m_veCamID;					//保存区域下面监控点ID
	vector<int>::iterator m_iterFirstNonPreview;		//正在播放最后监控点的下一个位置
	vector<int>::iterator m_iterFirstPreview;			//正在播放的第一个监控点
	int m_nResID;							//预览树选中项的ID值
	int	m_nLastChgedResID;
};
