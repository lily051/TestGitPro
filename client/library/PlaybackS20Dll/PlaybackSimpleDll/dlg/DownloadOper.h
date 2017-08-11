/**   @file DownloadOper.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	下载操作类.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/22
 *
 *    @note		用于进行下载的线程控制.
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */
//lint -library
#pragma once

#include "TaskCustom.h"

class CDownloadOper : public CTaskCustom
{
public:
	CDownloadOper(void);
	~CDownloadOper(void);

public:
	// 按时间下载.
	HANDLE DownloadTime(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam, int nCamID, DWORD dwRev);
	// 按文件下载.
	HANDLE DownloadFile(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam, int nCamID, DWORD dwRev);
	// 停止下载.
	void StopDownload(HANDLE hDlg, HANDLE& hDownload, int nCamID, DWORD dwRev, DWORD dwTimeBegin, DWORD dwTimeEnd, BOOL bAutoStop = TRUE);
	// 获取下载进度.
	BOOL GetDownloadPos(HANDLE hDownload, int & nPos);
	// 获取错误码.
	DWORD GetSDKLastError();
};
