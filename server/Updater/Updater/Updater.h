
// Updater.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "MultiLanguage.h"
#include <vector>
#include <queue>
using namespace std;

//升级命令
#define NEED_UPDATE 1
// 命令行参数名
#define PARA_KEY_APP_NAME _T("AppName=")
#define PARA_KEY_FMSCLIENT_NAME _T("FMSClientName=")
#define PARA_KEY_UPLOADCLIENT_NAME _T("UploadClientName=")
#define PARA_KEY_CURRENTVERSION _T("CurrentVersion=")
#define PARA_KEY_CHECKURL _T("CheckURL=")
#define PARA_KEY_NOTIFYWINDOW _T("NotifyWindow=")
#define PARA_KEY_NOTIFYWINDOWTITLE _T("NotifyWindowTitle=")
#define PARA_KEY_NOTIFYFINISH _T("NotifyFinish=")
#define PARA_KEY_SILENCE _T("Silence=")


// 本地保存的升级配置文件名
#define UPDATE_CONFIG_FILENAME _T("UpdateConfig.ini")

// 升级配置文件中的区段名
#define SECTION_UPDATE _T("UPDATE")
#define SECTION_COMMON _T("COMMON")
#define SECTION_LANGUAGE _T("LANGUAGE")

// 上报到界面的升级进度消息
enum
{
	NOTIFY_DOWNLOAD_INFO = 1001,		// 通知要下载的文件状况
	NOTIFY_DOWNLOADED_INFO,				// 通知已下载的文件状况
	NOTIFY_DOWNLOAD_PROGRESS,			// 通知下载单个文件进度
	NOTIFY_DOWNLOADING_FILENAME,		// 通知正在下载的文件名
	NOTIFY_DOWNLOAD_FILE_FAIL,			// 通知下载文件失败
	NOTIFY_VERIFY_FILE_FAIL,			// 通知校验文件失败
	NOTIFY_UPDATE_FILE_FAIL,			// 通知更新文件失败
    NOTIFY_FINISH_UPDATE,				// 通知升级完毕消息
};

// 升级下载文件状态消息结构
struct DOWNLOAD_INFO_STRU
{
	UINT iFileCount;					// 合计文件数
	ULONGLONG iFileSize;						// 合计字节数
};

// 下载单个文件进度消息结构
struct DOWNLOAD_PROGRESS_STRU
{
	ULONGLONG iCurrentFileSize;				// 当前正在下载的文件的字节数
	ULONGLONG iCurrentFileFinishedSize;		// 当前文件已下载字节数
};



// CUpdaterApp:
// 有关此类的实现，请参阅 Updater.cpp
//

class CUpdaterApp : public CWinAppEx
{
public:
	CString strLocalPackageTime;
	CString strURLPackageTime;
	BOOL m_UpdateOk;  //更新成功标志
	vector<FileInfo> m_LocalFilesList;
	vector<FileInfo> m_UrlFilesList;

	vector<FileInfo> m_LocalFilesInfo;
	void RunFMSClient();
	BOOL RunCmd(const CString& cmd, const CString& DefaultDir);
	void OnIconnotifyStart();
	void OnIconnotifyStop();
public:
	CUpdaterApp();
// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

private:
	BOOL ParseCommandLine();			// 处理命令行
	BOOL IsAppRunning();
	BOOL CheckUpdate();					// 从软件网站下载升级配置文件，检查是否有新版本的软件可用
	void CloseProgram(CString strProgram);
	void ReadLocalInI(CString& strLocalPackageTime);                //读取本地配置文件
	void ReadUrlInI(CString &strTime, int& nUpdateNumber);                  //读取URL下载的配置文件
	int UpdateFilesNumber(int &number);
	BOOL IsHaveProgram(CString strProgram);

private:
	CString m_sAppName;					// 应用程序名，用以创建互斥量
	CString m_FMSClientName;            // CS客户端名称
	CString m_UploadClientName;         //上传客户端名
	CString m_sVersion;					// 应用程序的当前版本
	CString m_sURL;						// 下载升级配置文件的URL
	DWORD m_iNotifyWindow;				// 升级过程中发送消息的目标窗口句柄
	CString m_sNotifyWindowTitle;		// 升级过程中发送消息的目标窗口标题
	DWORD m_iNotifyFinish;				// 升级完毕发送的消息代码
	BOOL m_bSilenceMode;				// 静默方式执行升级，不显示升级程序界面，只在升级完毕后提醒用户
};

extern CUpdaterApp theApp;