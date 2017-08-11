#include "StdAfx.h"
#include "DownloadOper.h"
#include "../PlaybackSimpleDll.h"
//#include "type/LogType.pb.h"
//using namespace cms_8100;

CDownloadOper::CDownloadOper(void)
{
}

CDownloadOper::~CDownloadOper(void)
{
}

// 文件下载.
HANDLE CDownloadOper::DownloadFile(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam, int nCamID, DWORD dwRev)
{
	HANDLE hDownload = PB_DownloadFile(stLoginInfo, stDownloadParam);

	// 写操作日志.
	BOOL bRet = (hDownload == NULL) ? FALSE : TRUE;
	if (!bRet)
	{
		return NULL;
	}
	CString strRemark = _T("按文件下载开始");

	// 开始时间.
	CTime ctBegin(
		stDownloadParam.stFileInfo.stStartTime.dwYear,
		stDownloadParam.stFileInfo.stStartTime.dwMonth,
		stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour,
		stDownloadParam.stFileInfo.stStartTime.dwMinute,
		stDownloadParam.stFileInfo.stStartTime.dwSecond);

	// 结束时间.
	CTime ctEnd(
		stDownloadParam.stFileInfo.stEndTime.dwYear,
		stDownloadParam.stFileInfo.stEndTime.dwMonth,
		stDownloadParam.stFileInfo.stEndTime.dwDay,
		stDownloadParam.stFileInfo.stEndTime.dwHour,
		stDownloadParam.stFileInfo.stEndTime.dwMinute,
		stDownloadParam.stFileInfo.stEndTime.dwSecond
		);

	return hDownload;
}

// 时间下载.
HANDLE CDownloadOper::DownloadTime(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam, int nCamID, DWORD dwRev)
{
	HANDLE hDownload = PB_DownloadTime(stLoginInfo, stDownloadParam);

	// 写操作日志.
	BOOL bRet = (hDownload == NULL) ? FALSE : TRUE;
	if (!bRet)
	{
		return NULL;
	}
	CString strRemark = _T("按时间下载开始");

	// 开始时间.
	CTime ctBegin(
		stDownloadParam.stStartTime.dwYear,
		stDownloadParam.stStartTime.dwMonth,
		stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour,
		stDownloadParam.stStartTime.dwMinute,
		stDownloadParam.stStartTime.dwSecond);
	// 结束时间.
	CTime ctEnd(
		stDownloadParam.stStopTime.dwYear,
		stDownloadParam.stStopTime.dwMonth,
		stDownloadParam.stStopTime.dwDay,
		stDownloadParam.stStopTime.dwHour,
		stDownloadParam.stStopTime.dwMinute,
		stDownloadParam.stStopTime.dwSecond
		);

	return hDownload;
}

// 停止下载.
void CDownloadOper::StopDownload(HANDLE hDlg, HANDLE& hDownload, int nCamID, DWORD dwRev, DWORD dwTimeBegin, DWORD dwTimeEnd, BOOL bAutoStop)
{
	BOOL bRet = PB_StopDownload(hDownload);
	if (!bRet)
	{
		PLAYBACKDLL_ERROR(_T("停止下载失败"));
	}
}

// 获取下载进度.
BOOL CDownloadOper::GetDownloadPos(HANDLE hDownload, int & nPos)
{
	return PB_GetDownloadPos(hDownload, nPos);
}

// 获取SDK错误.
DWORD CDownloadOper::GetSDKLastError()
{
	return PB_GetLastError();
}