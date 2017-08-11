
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

    CString m_hostIP;       ///< ����IP��ַ
    int m_nListenPort;       ///< �����˿�

    int m_nMsgMaxCount;     ///< MsgListCtrl����ʾ����Ϣ�������
    int m_nRestartInterval; ///< �رյ��Եĳ�ʱʱ��
	int m_nAlarmProcessor; ///< ������������

    CCtrlMsgQueue m_ctrlMsgQueue;   ///< ���յ��Ŀ�����Ϣ����

    //BOOL m_bCorrTimeFlag;   ///< Уʱ��־λ

    CString m_strDcPath;    ///< �������ķ����·��

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

//extern ConnSession* g_pCMSConnSession;  ///< �����ķ���֮���Session

// ��������FILETIME֮��ļ��(100ns)
 __int64 GetFileTimeInterval(const FILETIME& PreTime, const FILETIME& CurTime);
