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

// �ļ�����.
HANDLE CDownloadOper::DownloadFile(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam, int nCamID, DWORD dwRev)
{
	HANDLE hDownload = PB_DownloadFile(stLoginInfo, stDownloadParam);

	// д������־.
	BOOL bRet = (hDownload == NULL) ? FALSE : TRUE;
	if (!bRet)
	{
		return NULL;
	}
	CString strRemark = _T("���ļ����ؿ�ʼ");

	// ��ʼʱ��.
	CTime ctBegin(
		stDownloadParam.stFileInfo.stStartTime.dwYear,
		stDownloadParam.stFileInfo.stStartTime.dwMonth,
		stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour,
		stDownloadParam.stFileInfo.stStartTime.dwMinute,
		stDownloadParam.stFileInfo.stStartTime.dwSecond);

	// ����ʱ��.
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

// ʱ������.
HANDLE CDownloadOper::DownloadTime(HANDLE hDlg, PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam, int nCamID, DWORD dwRev)
{
	HANDLE hDownload = PB_DownloadTime(stLoginInfo, stDownloadParam);

	// д������־.
	BOOL bRet = (hDownload == NULL) ? FALSE : TRUE;
	if (!bRet)
	{
		return NULL;
	}
	CString strRemark = _T("��ʱ�����ؿ�ʼ");

	// ��ʼʱ��.
	CTime ctBegin(
		stDownloadParam.stStartTime.dwYear,
		stDownloadParam.stStartTime.dwMonth,
		stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour,
		stDownloadParam.stStartTime.dwMinute,
		stDownloadParam.stStartTime.dwSecond);
	// ����ʱ��.
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

// ֹͣ����.
void CDownloadOper::StopDownload(HANDLE hDlg, HANDLE& hDownload, int nCamID, DWORD dwRev, DWORD dwTimeBegin, DWORD dwTimeEnd, BOOL bAutoStop)
{
	BOOL bRet = PB_StopDownload(hDownload);
	if (!bRet)
	{
		PLAYBACKDLL_ERROR(_T("ֹͣ����ʧ��"));
	}
}

// ��ȡ���ؽ���.
BOOL CDownloadOper::GetDownloadPos(HANDLE hDownload, int & nPos)
{
	return PB_GetDownloadPos(hDownload, nPos);
}

// ��ȡSDK����.
DWORD CDownloadOper::GetSDKLastError()
{
	return PB_GetLastError();
}