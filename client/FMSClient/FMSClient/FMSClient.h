
// FMSClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif
#include <vector>
#include "AppDef.h"
#include "resource.h"		// ������
#include <verify/ClientVerify.pb.h>
#include "csclient/HistoryClientInput.pb.h"
#include "FUi/CalendarDlg.h"



// CFMSClientApp:
// �йش����ʵ�֣������ FMSClient.cpp
//

// �ͻ��������ļ���
extern const TCHAR* CLIENT_CONFIG_FILENAME;

class CFMSClientApp : public CWinAppEx
{
public:
	CFMSClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

public:
	BOOL InitRunConfig();
	BOOL ReadConfig(CString& m_strAppCapPath);
	const CString& GetAppPath(){return m_strAppPath;}
	// ��ȡ�������������Ϣ
	void GetConfigInfo();

	//��ʼ�������ͨ���м�㡢�����ȣ�
	void InitCom();

	// ����ʼ�����
	void UnInitCom();
	BOOL CheckInstance(BOOL bReboot);

	// ��ʾ�Ѵ��ڿͻ��˴���
	BOOL ShowWndAlreadyExist();



	CString m_strAppPath;		//��������·��
	CString m_strAppName;    //��������(����·��)
	CString m_strAppCapPath; //��ǰץͼ����·��
	CString m_strDefaultCapPath;
	CString m_strAppVer;			//�ͻ��˰汾��Ϣ
	HANDLE m_hMutex;
	CString m_sCustomTitle;		// �ͻ��˱���
	int m_nSkinType;
	int m_nLanguage;
	int m_nListenPort;	//�ͻ��˼����˿�
	CString m_sCopyright;
	// ��Ļ����
	int m_nMonitorCount;
	int m_nLoginType;

	cms_8120::ReqClientLogin m_pbmsgReqLogin;   // ��¼��Ϣ
	cms_8120::RspClientLogin m_pbmsgRspLogin;   // ��¼���ص���Ϣ
	//��������IP
	CString m_strServerIp;
	// �������Ķ˿�
	int m_nServerPort;
	// �ͻ�������PC��IP
	CString m_strAppIp;
	CString m_sMainTitle;
	int m_nUserId;  //��¼�û���ID
	// ҵ��Ա����
	CString m_strSalesmanName;
	// ҵ��Ա����
	CString m_strSalesmanCode;
	//ҵ��Ա��������
	CString m_strRegionName;

	cms_8120::HisClientInput m_hisClientInput;
	CString m_strNewFileName;

    //�ļ��汾������Ϣ
    CString m_strFileDescription;

	//�Ƿ�Ϊ����
	bool m_bReboot; 
	//���¼��ʱ��
	int m_nMaxRecordTime;
	int m_nCamIndex;
	// //�Ƿ�Ϊ��һ�ε�¼
	int m_nFirstLogin;

	CString GernateUrl();
};

extern CFMSClientApp theApp;