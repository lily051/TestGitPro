
// FMSClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#include <vector>
#include "AppDef.h"
#include "resource.h"		// 主符号
#include <verify/ClientVerify.pb.h>
#include "csclient/HistoryClientInput.pb.h"
#include "FUi/CalendarDlg.h"



// CFMSClientApp:
// 有关此类的实现，请参阅 FMSClient.cpp
//

// 客户端配置文件名
extern const TCHAR* CLIENT_CONFIG_FILENAME;

class CFMSClientApp : public CWinAppEx
{
public:
	CFMSClientApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

public:
	BOOL InitRunConfig();
	BOOL ReadConfig(CString& m_strAppCapPath);
	const CString& GetAppPath(){return m_strAppPath;}
	// 获取程序配置相关信息
	void GetConfigInfo();

	//初始化组件（通信中间层、界面库等）
	void InitCom();

	// 反初始化组件
	void UnInitCom();
	BOOL CheckInstance(BOOL bReboot);

	// 显示已存在客户端窗口
	BOOL ShowWndAlreadyExist();



	CString m_strAppPath;		//程序运行路径
	CString m_strAppName;    //程序名称(包括路径)
	CString m_strAppCapPath; //当前抓图保存路径
	CString m_strDefaultCapPath;
	CString m_strAppVer;			//客户端版本信息
	HANDLE m_hMutex;
	CString m_sCustomTitle;		// 客户端标题
	int m_nSkinType;
	int m_nLanguage;
	int m_nListenPort;	//客户端监听端口
	CString m_sCopyright;
	// 屏幕数量
	int m_nMonitorCount;
	int m_nLoginType;

	cms_8120::ReqClientLogin m_pbmsgReqLogin;   // 登录信息
	cms_8120::RspClientLogin m_pbmsgRspLogin;   // 登录返回的信息
	//数据中心IP
	CString m_strServerIp;
	// 数据中心端口
	int m_nServerPort;
	// 客户端所在PC的IP
	CString m_strAppIp;
	CString m_sMainTitle;
	int m_nUserId;  //登录用户的ID
	// 业务员姓名
	CString m_strSalesmanName;
	// 业务员工号
	CString m_strSalesmanCode;
	//业务员所在区域
	CString m_strRegionName;

	cms_8120::HisClientInput m_hisClientInput;
	CString m_strNewFileName;

    //文件版本描述信息
    CString m_strFileDescription;

	//是否为重启
	bool m_bReboot; 
	//最大录像时长
	int m_nMaxRecordTime;
	int m_nCamIndex;
	// //是否为第一次登录
	int m_nFirstLogin;

	CString GernateUrl();
};

extern CFMSClientApp theApp;