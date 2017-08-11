
// ServiceWatchdog.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <time.h>
#include "MsgQueue.h"

// CServiceWatchdogApp:
// See ServiceWatchdog.cpp for the implementation of this class
//

class CServiceWatchdogApp : public CWinApp//public CWinAppEx
{
public:
	CServiceWatchdogApp();

private:
    HANDLE m_hMutex;

    ULONG_PTR gdiplusToken;

public:
    COleDateTime m_startTime;

    CString m_hostIP;       ///< 本机IP地址
    int m_nListenPort;       ///< 监听端口

    int m_nMsgMaxCount;     ///< MsgListCtrl中显示的消息最多条数
    int m_nRestartInterval; ///< 关闭电脑的超时时间
	int m_nAlarmProcessor; ///< 报警处理类型

    CCtrlMsgQueue m_ctrlMsgQueue;   ///< 接收到的控制信息队列

    //BOOL m_bCorrTimeFlag;   ///< 校时标志位

    CString m_strDcPath;    ///< 数据中心服务的路径

	//CString m_localDBhost;       ///< DB Host
	//int m_localDBPort;			 ///< DB Port
	//CString m_localDBUser;		 ///< DB user
	//CString m_localDBPassword;   ///< DB password
	//CString m_localDBSchema;     ///< DB schema

// Overrides
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();

    //BOOL InitLocalDBConfig(void);
    BOOL InitParamInfo(void);

    // Implementation
	DECLARE_MESSAGE_MAP()
    
};

extern CServiceWatchdogApp theApp;

//extern ConnSession* g_pCMSConnSession;  ///< 与中心服务之间的Session

// 计算两个FILETIME之间的间隔(100ns)
 __int64 GetFileTimeInterval(const FILETIME& PreTime, const FILETIME& CurTime);
