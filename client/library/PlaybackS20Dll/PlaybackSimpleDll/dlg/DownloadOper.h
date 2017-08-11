/**   @file DownloadOper.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	���ز�����.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/22
 *
 *    @note		���ڽ������ص��߳̿���.
 *    @note 
 *    @note ��ʷ��¼��
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
	// ��ʱ������.
	HANDLE DownloadTime(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam, int nCamID, DWORD dwRev);
	// ���ļ�����.
	HANDLE DownloadFile(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam, int nCamID, DWORD dwRev);
	// ֹͣ����.
	void StopDownload(HANDLE hDlg, HANDLE& hDownload, int nCamID, DWORD dwRev, DWORD dwTimeBegin, DWORD dwTimeEnd, BOOL bAutoStop = TRUE);
	// ��ȡ���ؽ���.
	BOOL GetDownloadPos(HANDLE hDownload, int & nPos);
	// ��ȡ������.
	DWORD GetSDKLastError();
};
