// PlaybackSimpleDll.h : PlaybackSimpleDll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "DeviceManager.h"
#include "PlaybackDef.h"
#include "PlaybackMain.h"

// CPlaybackSimpleDllApp
// 有关此类实现的信息，请参阅 PlaybackSimpleDll.cpp
//
#define FLAG_PLAYTYPE 100
class CPlaybackSimpleDllApp : public CWinApp
{
public:
	CPlaybackSimpleDllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	int m_nSkinType;						// 皮肤类型.
	HINSTANCE m_hResInstance;				// 资源句柄.
	HINSTANCE m_hOldInstance;				// 原来的资源句柄.
	HINSTANCE m_hIEFrame;					// ieframe.dll加载句柄.
	CString	m_strModulePath;				// 程序路径.
	PLAYBACK_PARAM m_stPlaybackParam;       // 客户端参数信息.
	CDeviceManager m_manDevice;				// 设备管理类.
	DWORD m_dwStyle;						// 组件风格.
	BOOL m_bIsIEProtected;					// 是否是IE保护模式.
	PLAY_LAYOUT_WND m_emLayoutStyle;		// 回放画面分割模式.
	BOOL m_bBwControl;                      //是否需要带宽控制
	BOOL m_bBwMode;							// 是否是带宽控制模式.
	int m_nBWUserID;						// 带宽控制ID.
	PBSDUserInfo m_stDataInfo;                 //客户端登录信息
	LPVOID m_pClientVoid;                   //客户端定义的数据
	PBSDCmsData m_stCmsInfo;				// CMS数据，带宽控制使用.
	CString m_strLocalIPAddr;				// 本地IP地址.
	CPlaybackMain * m_pPlaybackMain;        //回放组件对话框指针
	PBSDClientData m_stClientData;			// 客户端类型信息.
	LONG m_lBwControl;                       //支持带宽控制
	// 是否初始化代理.
	BOOL m_bInitProx;
	// 是否初始化PCNVR.
	BOOL m_bInitNvr;
	// 是否初始化bdp.
	BOOL m_bInitBdp;
	// 是否初始化FinAudio.
	BOOL m_bInitFinAudio;
	// 功能权限
	PBSDRights m_stRights;

private:
    // SDK初始化,服务初始化类型值+100
    std::set<int> m_setSDKInit;

    CRITICAL_SECTION m_csSDK;
public:
	// 初始化配置信息.
	void InitConfigInfo();
	//获取监控点下载权限
	BOOL IsCamDownloadRight(DWORD dwRight);
	// 获取监控点抓图权限.
	BOOL IsCamCaptureRight(DWORD dwRight);
	// 获取监控点剪辑权限.
	BOOL IsCamSaveRight(DWORD dwRight);
	//获取组件内部权限
	BOOL GetTabRight(int& nTabRight);
	// 是否有下载模块.
	BOOL HasDownloadModule();
	// 是否本地回放.
	BOOL HasLocalPlayback();
	// 获取IE低级别路径.
	BOOL GetIESafePath(const CString& strFileName, CString& strTempPath, CString& strSelectPath, HANDLE& hState, IE_SAVE_FILETYPE emFileType, ENUM_PIC_TYPE emPicType = PIC_TYPE_BMP);
	// 保存IE文件.
	BOOL SaveIEFile(const CString& strPath, const HANDLE& hState);
	// 取消保存IE文件.
	BOOL CancelIEFile(const HANDLE& hState);

	//获取监控点下载权限
	virtual int ExitInstance();

    BOOL InitSDK(ENUM_DEVICE_TYPE emDevType, ENUM_PLAY_TYPE emPlayType);
    void UnInitSDK();

private:
	// 获取文件过滤字符串.
	void GetFileFilter(IE_SAVE_FILETYPE emFileType, ENUM_PIC_TYPE emPicType, CString& strFilter, CString& strExt);
};

extern CPlaybackSimpleDllApp theApp;