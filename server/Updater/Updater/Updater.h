
// Updater.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MultiLanguage.h"
#include <vector>
#include <queue>
using namespace std;

//��������
#define NEED_UPDATE 1
// �����в�����
#define PARA_KEY_APP_NAME _T("AppName=")
#define PARA_KEY_FMSCLIENT_NAME _T("FMSClientName=")
#define PARA_KEY_UPLOADCLIENT_NAME _T("UploadClientName=")
#define PARA_KEY_CURRENTVERSION _T("CurrentVersion=")
#define PARA_KEY_CHECKURL _T("CheckURL=")
#define PARA_KEY_NOTIFYWINDOW _T("NotifyWindow=")
#define PARA_KEY_NOTIFYWINDOWTITLE _T("NotifyWindowTitle=")
#define PARA_KEY_NOTIFYFINISH _T("NotifyFinish=")
#define PARA_KEY_SILENCE _T("Silence=")


// ���ر�������������ļ���
#define UPDATE_CONFIG_FILENAME _T("UpdateConfig.ini")

// ���������ļ��е�������
#define SECTION_UPDATE _T("UPDATE")
#define SECTION_COMMON _T("COMMON")
#define SECTION_LANGUAGE _T("LANGUAGE")

// �ϱ������������������Ϣ
enum
{
	NOTIFY_DOWNLOAD_INFO = 1001,		// ֪ͨҪ���ص��ļ�״��
	NOTIFY_DOWNLOADED_INFO,				// ֪ͨ�����ص��ļ�״��
	NOTIFY_DOWNLOAD_PROGRESS,			// ֪ͨ���ص����ļ�����
	NOTIFY_DOWNLOADING_FILENAME,		// ֪ͨ�������ص��ļ���
	NOTIFY_DOWNLOAD_FILE_FAIL,			// ֪ͨ�����ļ�ʧ��
	NOTIFY_VERIFY_FILE_FAIL,			// ֪ͨУ���ļ�ʧ��
	NOTIFY_UPDATE_FILE_FAIL,			// ֪ͨ�����ļ�ʧ��
    NOTIFY_FINISH_UPDATE,				// ֪ͨ���������Ϣ
};

// ���������ļ�״̬��Ϣ�ṹ
struct DOWNLOAD_INFO_STRU
{
	UINT iFileCount;					// �ϼ��ļ���
	ULONGLONG iFileSize;						// �ϼ��ֽ���
};

// ���ص����ļ�������Ϣ�ṹ
struct DOWNLOAD_PROGRESS_STRU
{
	ULONGLONG iCurrentFileSize;				// ��ǰ�������ص��ļ����ֽ���
	ULONGLONG iCurrentFileFinishedSize;		// ��ǰ�ļ��������ֽ���
};



// CUpdaterApp:
// �йش����ʵ�֣������ Updater.cpp
//

class CUpdaterApp : public CWinAppEx
{
public:
	CString strLocalPackageTime;
	CString strURLPackageTime;
	BOOL m_UpdateOk;  //���³ɹ���־
	vector<FileInfo> m_LocalFilesList;
	vector<FileInfo> m_UrlFilesList;

	vector<FileInfo> m_LocalFilesInfo;
	void RunFMSClient();
	BOOL RunCmd(const CString& cmd, const CString& DefaultDir);
	void OnIconnotifyStart();
	void OnIconnotifyStop();
public:
	CUpdaterApp();
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
	BOOL ParseCommandLine();			// ����������
	BOOL IsAppRunning();
	BOOL CheckUpdate();					// �������վ�������������ļ�������Ƿ����°汾���������
	void CloseProgram(CString strProgram);
	void ReadLocalInI(CString& strLocalPackageTime);                //��ȡ���������ļ�
	void ReadUrlInI(CString &strTime, int& nUpdateNumber);                  //��ȡURL���ص������ļ�
	int UpdateFilesNumber(int &number);
	BOOL IsHaveProgram(CString strProgram);

private:
	CString m_sAppName;					// Ӧ�ó����������Դ���������
	CString m_FMSClientName;            // CS�ͻ�������
	CString m_UploadClientName;         //�ϴ��ͻ�����
	CString m_sVersion;					// Ӧ�ó���ĵ�ǰ�汾
	CString m_sURL;						// �������������ļ���URL
	DWORD m_iNotifyWindow;				// ���������з�����Ϣ��Ŀ�괰�ھ��
	CString m_sNotifyWindowTitle;		// ���������з�����Ϣ��Ŀ�괰�ڱ���
	DWORD m_iNotifyFinish;				// ������Ϸ��͵���Ϣ����
	BOOL m_bSilenceMode;				// ��Ĭ��ʽִ������������ʾ����������棬ֻ��������Ϻ������û�
};

extern CUpdaterApp theApp;