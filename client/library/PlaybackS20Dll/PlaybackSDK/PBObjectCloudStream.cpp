/**   @file HikStream.cpp
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 海康设备管理类
 *
 *    @author   litongli
 *    @date     2016/08/24
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#include "StdAfx.h"
#include <process.h>
#include "PBObjectCloudStream.h"

/** @fn ObjectCloudStreamDataCb
*   @param [out] buff:    录像数据
*   @param [out] bufflen: 数据长度 
*   @param [out] offset:  当前数据在整个待下载数据中的偏移
*   @param [out] error:   等于0: 当前下载成功; 小于0: 表示下载出现错误,值为具体出错错误码
*   @param [out] pUser:   用户自定义数据 
*   @@return 等于0: 数据已处理; 小于0: 数据未处理
*/
int CPBObjectCloudStream::ObjectCloudStreamDataCb(const char* buff, int bufflen, long long offset, int error, void* p_user)
{
	CPBObjectCloudStream * pInstance = (CPBObjectCloudStream*)p_user;
	if (pInstance == NULL)
	{
		PLAYBACKSDK_ERROR("pInstance = NULL");
		return -1;
	}
	PLAYBACKSDK_INFO("bufflen = %d, offset = %d", bufflen, offset);
	if (pInstance->m_bStopDataCb)
	{
		PLAYBACKSDK_INFO("StopDataCallBack");
		return -1;
	}
	if (error < 0)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-DownloadStreamDataCb error, errorcode = %d", error);
		return -1;
	}
	if (buff == NULL || bufflen <= 0)
	{
		PLAYBACKSDK_INFO("invalid buff or bufflen, buff = %ld, bufflen= %d", buff, bufflen);
		return -1;
	} 

	if (pInstance->m_bDownload)
	{
		pInstance->pFunDownload((HANDLE)0, PB_STREAM_DATA, (BYTE*)buff, bufflen);
	}

	if (pInstance->m_lPort != -1)
	{
		pInstance->pFunPlayback((HANDLE)0, PB_STREAM_DATA, (BYTE*)buff, bufflen);
	}

	return 0;

}

DWORD WINAPI CPBObjectCloudStream::ThreadFuncGetObjectCloudStream(void *pParam)
{
	CPBObjectCloudStream *pObjectCloudStream = (CPBObjectCloudStream *)pParam;
	if (NULL == pObjectCloudStream)
	{
		return -1;
	}

	pObjectCloudStream->GetObjectCloudStreamData();

	return 0;
}

CPBObjectCloudStream::CPBObjectCloudStream(void)
: m_hHCSClient(NULL)
, m_bStopDataCb(FALSE)
, m_hGetObjectCloudStreamThread(NULL)
, m_iStartTime(0)
, m_iStopTime(0)
{
}

CPBObjectCloudStream::~CPBObjectCloudStream(void)
{
}


/** @fn InitObjectCloudSdk
*   @brief 初始化
*   @param[in] 
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBObjectCloudStream::InitObjectCloudSdk()
{	
	PLAYBACKSDK_INFO("对象云初始化成功");
	return TRUE;
}

/** @fn ReleaseObjectCloudSdk
*   @brief 释放sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBObjectCloudStream::ReleaseObjectCloudSdk()
{
	PLAYBACKSDK_INFO("对象云释放成功");
}


/** @fn Login
*   @brief 登录设备，获取类句柄，单个回放或下载操作即在此类上进行
*   @param[in] stLoginInfo: 登录设备信息
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBObjectCloudStream::Login(PB_LOGININFO& stLoginInfo)
{
	PB_DVRLOGININFO* pStDvrLoginInfo = &(stLoginInfo.unLoginType.stDvrLoginInfo);
	if (NULL == pStDvrLoginInfo)
	{
		return FALSE;
	}

	m_emPlayer = PT_HIK;

	if (NULL == m_hHCSClient)
	{
		char endpoint[32]={0};
		HCSClient* pClient = NULL;
		sprintf(endpoint, "%s:%d", pStDvrLoginInfo->szIP, pStDvrLoginInfo->nPort);
		m_hHCSClient = ClientInitialize(pStDvrLoginInfo->szAccessKey, pStDvrLoginInfo->szSecretKey, endpoint);
		if (NULL == m_hHCSClient)
		{
			PLAYBACKSDK_ERROR("ClientInitialize fail,IP(%s),Port(%d)", pStDvrLoginInfo->szIP, pStDvrLoginInfo->nPort);
			return  FALSE;
		}
		PLAYBACKSDK_INFO("ClientInitialize succ,IP(%s),Port(%d)", pStDvrLoginInfo->szIP, pStDvrLoginInfo->nPort);	
	}
	// 保存Bucket.
	m_strBucket.Format("%s", pStDvrLoginInfo->szBucket);
	// 保存ObjectKey.
	m_strObjectKey.Format("%s", pStDvrLoginInfo->szObjectKey);

	PLAYBACKSDK_INFO("szBucket(%s),szObjectKey(%s)", pStDvrLoginInfo->szBucket, pStDvrLoginInfo->szObjectKey);
	return TRUE;
}

/** @fn Logout
*   @brief 登出设备,销毁句柄
*   @param[in] hLogin: 登录句柄
*   @param NULL
*   @return NULL
*/
void CPBObjectCloudStream::Logout()
{
	if (NULL != m_hHCSClient)
	{
		ClientFinalize(m_hHCSClient);
		m_hHCSClient = NULL;
		PLAYBACKSDK_INFO("ClientFinalize  succ");
	}	
}

/** @fn FindFile
*   @brief 查找文件
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
BOOL CPBObjectCloudStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}

	// 开始时间.
	CTime ctBegin(stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
		stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond);

	// 结束时间.
	CTime ctEnd(stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
		stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);

	time_t iStartTime = ctBegin.GetTime();
	time_t iStopTime  = ctEnd.GetTime();

	PLAYBACKSDK_INFO("ObjectCloudStream-查询文件参数. channel=%d, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", stFindFileParam.lChannel, stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
		stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond, stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
		stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);

    HCSStreamTimeSegmentListEx *seg_list = NULL;
	int ret = QueryObjectTimesegmentEx(m_hHCSClient, m_strBucket.GetBuffer(0), m_strObjectKey.GetBuffer(0), iStartTime, iStopTime, &seg_list, 1);
	if (0 != ret)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-QueryObjectTimesegmentEx fail， ErrorCode = %d", ret);
		return FALSE;
	}

	if (((seg_list) && (seg_list->segment_count == 0)) || (seg_list == NULL))
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-QueryObjectTimesegmentEx  list is empty!");
		return FALSE;
	}
    nFileCount = seg_list->segment_count;
	for (int i = 0; i < seg_list->segment_count; i++)
	{
		PB_FILEINFO stFileInfo;
		struct tm stStartTime = {0};
		struct tm stStopTime = {0};
		localtime_s(&stStartTime, &seg_list->segment_list[i].start_time);
		localtime_s(&stStopTime, &seg_list->segment_list[i].end_time);
		//录像开始时间
		stFileInfo.stStartTime.dwYear   = stStartTime.tm_year + 1900;
		stFileInfo.stStartTime.dwMonth  = stStartTime.tm_mon + 1;
		stFileInfo.stStartTime.dwDay    = stStartTime.tm_mday;
		stFileInfo.stStartTime.dwHour   = stStartTime.tm_hour;
		stFileInfo.stStartTime.dwMinute = stStartTime.tm_min;
		stFileInfo.stStartTime.dwSecond = stStartTime.tm_sec;
		//录像结束时间
		stFileInfo.stEndTime.dwYear     = stStopTime.tm_year + 1900;
		stFileInfo.stEndTime.dwMonth    = stStopTime.tm_mon + 1;
		stFileInfo.stEndTime.dwDay      = stStopTime.tm_mday;
		stFileInfo.stEndTime.dwHour     = stStopTime.tm_hour;
		stFileInfo.stEndTime.dwMinute   = stStopTime.tm_min;
		stFileInfo.stEndTime.dwSecond   = stStopTime.tm_sec;
// 		////录像类型
// 		stFileInfo.nFileType = stInfo.record_type;
		////录像大小
		stFileInfo.dwFileSize =  seg_list->segment_list[i].size;

		CString str;
		str.Format("objectcloud_%02u%02u%02u-%02u%02u%02u", 
			stFileInfo.stStartTime.dwHour,
			stFileInfo.stStartTime.dwMinute,
			stFileInfo.stStartTime.dwSecond,
			stFileInfo.stEndTime.dwHour,
			stFileInfo.stEndTime.dwMinute,
			stFileInfo.stEndTime.dwSecond);
		// 文件名.
		memset(&stFileInfo.szFileName, 0, sizeof(stFileInfo.szFileName));
		sprintf_s(stFileInfo.szFileName, sizeof(stFileInfo.szFileName), str.GetBuffer());

		memcpy_s(&lpFileList[i], sizeof(PB_FILEINFO), &stFileInfo, sizeof(PB_FILEINFO));
	}
	FreeStreamTimeSegmentListEx(m_hHCSClient, &seg_list);
	return TRUE;
}

/** @fn PlaybackTime
*   @brief 时间回放
*   @param[in] stPbTimeInfo: 时间回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBObjectCloudStream::PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo)
{
	m_stPlaybackTimeInfo = stPbTimeInfo;
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}
	// 开始时间.
	CTime ctBegin(stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond);

	// 结束时间.
	CTime ctEnd(stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

    m_iStartTime = ctBegin.GetTime();
	m_iStopTime  = ctEnd.GetTime();

	m_hPlayHwnd = stPbTimeInfo.hWnd;
	//m_pDrawFunc = (VIDEODRAWFUNC)stPbTimeInfo.pDrawFunc;  //保存画图函数
	//m_nDrawUserData = stPbTimeInfo.nUser;     //用户数据

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("ObjectCloudStream-时间回放参数. m_hHCSClient=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_hHCSClient, stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond, stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	//回放，窗口句柄设定为NULL
	if (!StartData(m_hHCSClient, m_strBucket.GetBuffer(0), m_strObjectKey.GetBuffer(0), m_iStartTime, m_iStopTime))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		/*PLAYBACKSDK_ERROR("ObjectCloudStream-StartData false，m_hHCSClient:%ld, m_strBucket:%s, m_strObjectKey:%s", 
			m_hHCSClient,
			m_strBucket.GetBuffer(0),
			m_strObjectKey.GetBuffer(0));*/
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartData fail，m_hHCSClient:%ld", m_hHCSClient);
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartStreamThread fail, m_hHCSClient=%ld", (LONG)m_hHCSClient);
		return FALSE;
	}

	PLAYBACKSDK_INFO("ObjectCloudStream-时间回放成功. m_hHCSClient=%ld", (LONG)m_hHCSClient);

	return TRUE;
}

/** @fn StopPlayback
*   @brief 停止回放
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBObjectCloudStream::StopPlayback()
{
    PLAYBACKSDK_INFO("StopPlayback in");
	// 停止取流.
	StopData();

	//停止解码
	StopPlay();
	
	//关闭线程
	StopStreamThread(TRUE);
	
	PLAYBACKSDK_INFO("ObjectCloudStream-停止回放. m_hHCSClient=%ld", m_hHCSClient);

	CPlayStream::StopPlayback();

	PLAYBACKSDK_INFO("StopPlayback out");
}

/** @fn PB_DownloadTime
*   @brief 时间下载
*   @param[in] stDownloadParam: 下载信息
*   @param NULL
*   @return 下载句柄，NULL表示下载失败
*/
BOOL CPBObjectCloudStream::DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}
	// 开始时间.
	CTime ctBegin(stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour, stDownloadParam.stStartTime.dwMinute, stDownloadParam.stStartTime.dwSecond);

	// 结束时间.
	CTime ctEnd(stDownloadParam.stStopTime.dwYear, stDownloadParam.stStopTime.dwMonth, stDownloadParam.stStopTime.dwDay,
		stDownloadParam.stStopTime.dwHour, stDownloadParam.stStopTime.dwMinute, stDownloadParam.stStopTime.dwSecond);

	m_iStartTime = ctBegin.GetTime();
	m_iStopTime = ctEnd.GetTime();

	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-Create_File fail, 文件名: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nDownloadPos = 0;
	PLAYBACKSDK_INFO("ObjectCloudStream-时间下载参数. m_hHCSClient=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_hHCSClient, stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour, stDownloadParam.stStartTime.dwMinute, stDownloadParam.stStartTime.dwSecond, stDownloadParam.stStopTime.dwYear, stDownloadParam.stStopTime.dwMonth, stDownloadParam.stStopTime.dwDay,
		stDownloadParam.stStopTime.dwHour, stDownloadParam.stStopTime.dwMinute, stDownloadParam.stStopTime.dwSecond);
	if (!StartData(m_hHCSClient, m_strBucket.GetBuffer(0), m_strObjectKey.GetBuffer(0), m_iStartTime, m_iStopTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartData fail, m_hHCSClient=%ld", m_hHCSClient);
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartStreamThread fail, m_hHCSClient=%ld", m_hHCSClient);
		return FALSE;
	}

	//开始取流时间
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/** @fn PlaybackFile
*   @brief 文件回放
*   @param[in] stPbFileInfo: 文件回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBObjectCloudStream::PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)
{
	//m_stPlaybackTimeInfo.stStartTime = stPbFileInfo.stFileInfo.stStartTime;
	//m_stPlaybackTimeInfo.stStopTime = stPbFileInfo.stFileInfo.stEndTime;
	//m_stPlaybackTimeInfo.lChannel = stPbFileInfo.lChannel;
	//memcpy_s(&m_stPlaybackTimeInfo.szStreamID, 32, &(stPbFileInfo.szStreamID), 32);
	//m_stPlaybackTimeInfo.hWnd = stPbFileInfo.hWnd;
	//m_stPlaybackTimeInfo.bConvertPlay = stPbFileInfo.bConvertPlay;
	//m_stPlaybackTimeInfo.stConvertParam = stPbFileInfo.stConvertParam;
	//m_stPlaybackTimeInfo.lBwControl = stPbFileInfo.lBwControl;

	//if (NULL == m_hHCSClient)
	//{
	//	return FALSE;
	//}

	//NET_DVR_TIME stNetStartTime, stNetStopTime;
	//memcpy_s(&stNetStartTime, sizeof(NET_DVR_TIME), &(stPbFileInfo.stFileInfo.stStartTime), sizeof(PB_TIME));
	//memcpy_s(&stNetStopTime, sizeof(NET_DVR_TIME), &(stPbFileInfo.stFileInfo.stEndTime), sizeof(PB_TIME));

	//m_hPlayHwnd = stPbFileInfo.hWnd;
	//m_pDrawFunc = (VIDEODRAWFUNC)stPbFileInfo.pDrawFunc;  //保存画图函数
	//m_nDrawUserData = stPbFileInfo.nUser;     //用户数据

	//if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	//{
	//	PLAYBACKSDK_ERROR("CvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
	//	return FALSE;
	//}

	//PLAYBACKSDK_INFO("CvrStream-时间回放参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbFileInfo.stFileInfo.stStartTime.dwYear, stPbFileInfo.stFileInfo.stStartTime.dwMonth, stPbFileInfo.stFileInfo.stStartTime.dwDay,
	//	stPbFileInfo.stFileInfo.stStartTime.dwHour, stPbFileInfo.stFileInfo.stStartTime.dwMinute, stPbFileInfo.stFileInfo.stStartTime.dwSecond, stPbFileInfo.stFileInfo.stEndTime.dwYear, stPbFileInfo.stFileInfo.stEndTime.dwMonth, stPbFileInfo.stFileInfo.stEndTime.dwDay,
	//	stPbFileInfo.stFileInfo.stEndTime.dwHour, stPbFileInfo.stFileInfo.stEndTime.dwMinute, stPbFileInfo.stFileInfo.stEndTime.dwSecond);

	////回放，窗口句柄设定为NULL
	//if (!StartData(m_hHCSClient, stPbFileInfo.szStreamID, stNetStartTime, stNetStopTime, stPbFileInfo.bConvertPlay
	//	, stPbFileInfo.stConvertParam.byResolution, stPbFileInfo.stConvertParam.dwVideoBitrate, stPbFileInfo.stConvertParam.dwVideoFrameRate))
	//{
	//	GPAPI_FreePort(m_emPlayer, m_lPort);
	//	m_lPort = -1;
	//       PLAYBACKSDK_ERROR("CvrStream-StartData false，lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
	//	return FALSE;
	//}

	////开启流检测线程
	//if (!StartStreamThread(TRUE))
	//{
	//	StopData();
	//	GPAPI_FreePort(m_emPlayer, m_lPort);
	//	m_lPort = -1;
	//	PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
	//	return FALSE;
	//}

	//PLAYBACKSDK_INFO("CvrStream-时间回放成功. lLogin=%ld", m_lLogin);

	return TRUE;
}

/** @fn PB_DownloadFile
*   @brief 文件下载
*   @param[in] stDownloadParam: 下载参数
*   @param NULL
*   @return 下载句柄，NULL表示下载失败
*/
BOOL CPBObjectCloudStream::DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}
	// 开始时间.
	CTime ctBegin(stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour, stDownloadParam.stFileInfo.stStartTime.dwMinute, stDownloadParam.stFileInfo.stStartTime.dwSecond);

	// 结束时间.
	CTime ctEnd(stDownloadParam.stFileInfo.stEndTime.dwYear, stDownloadParam.stFileInfo.stEndTime.dwMonth, stDownloadParam.stFileInfo.stEndTime.dwDay,
		stDownloadParam.stFileInfo.stEndTime.dwHour, stDownloadParam.stFileInfo.stEndTime.dwMinute, stDownloadParam.stFileInfo.stEndTime.dwSecond);

	m_iStartTime = ctBegin.GetTime();
	m_iStopTime = ctEnd.GetTime();

	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-Create_File falid, 文件名: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;
	//取文件流
	PLAYBACKSDK_INFO("ObjectCloudStream-文件下载参数. m_hHCSClient=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_hHCSClient, stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour, stDownloadParam.stFileInfo.stStartTime.dwMinute, stDownloadParam.stFileInfo.stStartTime.dwSecond, stDownloadParam.stFileInfo.stEndTime.dwYear, stDownloadParam.stFileInfo.stEndTime.dwMonth, stDownloadParam.stFileInfo.stEndTime.dwDay,
		stDownloadParam.stFileInfo.stEndTime.dwHour, stDownloadParam.stFileInfo.stEndTime.dwMinute, stDownloadParam.stFileInfo.stEndTime.dwSecond);

	if (!StartData(m_hHCSClient, m_strBucket.GetBuffer(0), m_strObjectKey.GetBuffer(0), m_iStartTime, m_iStopTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartData, m_hHCSClient=%ld", m_hHCSClient);
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartStreamThread falid, m_hHCSClient=%ld", m_hHCSClient);
		return FALSE;
	}

	//开始取流时间
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/** @fn PB_StopDownload
*   @brief 停止下载
*   @param[in] hDownload: 下载句柄
*   @param NULL
*   @return NULL
*/
void CPBObjectCloudStream::StopDownload()
{
	if (m_bDownload)
	{
		//停流
		StopData();
		//关闭检测线程
		StopStreamThread(FALSE);
		m_bDownload = FALSE;
		if (m_hDownloadFile)
		{
			CloseHandle(m_hDownloadFile);
			m_hDownloadFile = FALSE;
		}

		PLAYBACKSDK_INFO("ObjectCloudStream-停止下载. m_hHCSClient=%ld", m_hHCSClient);
	}
}

/**   @fn          StartData
 *    @brief   	   开始取流。
 *    @param[in]   pClient:对象云初始化句柄.
 *    @param[in]   pBucket:资源池.
 *	  @param[in]   pObjectKey:文件key.
 *    @param[in]   iStartTime:文件开始时间.
 *    @param[in]   iStopTime:文件结束时间.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPBObjectCloudStream::StartData(HCSClient* pClient, const char* pBucket, const char* pObjectKey, const time_t iStartTime, const time_t iStopTime)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}

	//如果在回放中，停流
	StopData();
	m_bBlock = TRUE;
	m_bStopDataCb = FALSE;  //开始取流不返回
    //取文件头
	HCSGetObjectRequest stObjectRequest;
	memset(&stObjectRequest, 0, sizeof(HCSGetObjectRequest));
	HCSGetObjectRequest *request = &stObjectRequest;
	request->bucket_name = (char*)m_strBucket.GetBuffer(0);
	request->key = (char*)m_strObjectKey.GetBuffer(0);
	request->version_id = NULL;
	request->start_pos = 0;
	request->end_pos = 39;
	char *read_buffer = NULL;
	unsigned int read_len = 0;
    PLAYBACKSDK_ERROR("~*************GetObjectToBuffer************Begin;");
	int set_result = GetObjectToBuffer(m_hHCSClient, request, (void**)&read_buffer, &read_len);
    PLAYBACKSDK_ERROR("~*************GetObjectToBuffer************%d*****End;", read_len);

	if (0 == set_result)
	{
		PLAYBACKSDK_INFO("ObjectCloudStream-StartData GetObjectToBuffer, read_len=%d", read_len);
		if (m_bDownload)
		{
			pFunDownload((HANDLE)0, PB_STREAM_HEADER, (BYTE*)read_buffer, read_len);
		}

		if (m_lPort != -1)
		{
			pFunPlayback((HANDLE)0, PB_STREAM_HEADER, (BYTE*)read_buffer, read_len);
		}
	}
	else
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartData GetObjectToBuffer, set_result=%d", set_result);
		FreeBuffer(m_hHCSClient,(void**) &read_buffer);
		return FALSE;
	}
	FreeBuffer(m_hHCSClient, (void**)&read_buffer);
    
	//取录像数据
	m_hGetObjectCloudStreamThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFuncGetObjectCloudStream), this, 0, NULL);
	if (NULL == m_hGetObjectCloudStreamThread)
	{
		PLAYBACKSDK_ERROR("GetObjectCloudStreamThread create fail");
		return FALSE;
	}

	return TRUE;
}

/**   @fn          StopData
 *    @brief   	   停止取流.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPBObjectCloudStream::StopData()
{
	PLAYBACKSDK_INFO("StopData start");
	// 停止数据回调阻塞.
	m_bBlock      = FALSE;
	m_bStopDataCb = TRUE;

	if (m_hGetObjectCloudStreamThread != NULL)
	{
		PLAYBACKSDK_INFO("Stop GetObjectCloudStreamThread start");
		WaitForSingleObject(m_hGetObjectCloudStreamThread, INFINITE);
		CloseHandle(m_hGetObjectCloudStreamThread);
		m_hGetObjectCloudStreamThread = NULL;
		PLAYBACKSDK_INFO("Stop GetObjectCloudStreamThread end");
	}
    PLAYBACKSDK_INFO("StopData end");
	return TRUE;
}

void CPBObjectCloudStream::GetObjectCloudStreamData()
{
	PLAYBACKSDK_INFO("GetObjectCloudStreamData start");
	int result = DownloadStreamData(m_hHCSClient, m_strBucket.GetBuffer(0), m_strObjectKey.GetBuffer(0), m_iStartTime, m_iStopTime, OUTPUT_VIDEO_SOURCE, ObjectCloudStreamDataCb, this);
	if (0 != result)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-DownloadStreamData fail， ErrorCode = %d", result);
		return;
	}
	PLAYBACKSDK_INFO("GetObjectCloudStreamData end");
}



