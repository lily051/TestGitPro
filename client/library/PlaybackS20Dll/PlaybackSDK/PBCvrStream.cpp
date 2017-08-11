/**   @file HikStream.cpp
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 海康设备管理类
 *
 *    @author   zhoupeiqiang
 *    @date     2012/11/13
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#include "StdAfx.h"
#include "PBCvrStream.h"

/** @fn pStreamPbLayer
*   @brief 取流回调函数
*   @param[in] hPlayData: 回放句柄
*   @param[in] hUserData：用户数据
*   @param[in] nDataType: 流类型
*   @param[in] pBuffer:   流数据
*   @param[in] dwBufferSize: 数据大小
*   @return NULL
*/
void CALLBACK pCvrStream(LONG lPlayHandle, DWORD dwDataType, BYTE * pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	CPBCvrStream * pInstance = (CPBCvrStream*)ULongToPtr(dwUser);
	if (pInstance == NULL)
	{
		return;
	}

	// 流类型.
	int nDataType = -1;

	switch (dwDataType)
	{
	case NET_DVR_STREAMDATA:
	case NET_DVR_AUDIOSTREAMDATA:		//音频流数据
	case NET_DVR_STD_VIDEODATA:			//标准视频流数据
	case NET_DVR_STD_AUDIODATA:			//标准音频流数据
	case NET_DVR_CHANGE_FORWARD:		//码流改变为正放  
	case NET_DVR_CHANGE_REVERSE:		//码流改变为倒放
	case NET_DVR_PRIVATE_DATA:			//私有数据,包括智能信息
		{
			nDataType = PB_STREAM_DATA;
		}
		break;
	case NET_DVR_SYSHEAD:
		{
			nDataType = PB_STREAM_HEADER;
		}
		break;

	default:
		break;
	}

	if (pInstance->m_bDownload)
	{
		pInstance->pFunDownload((HANDLE)lPlayHandle, nDataType, pBuffer, dwBufSize);
	}

	if (pInstance->m_lPort != -1)
	{
		pInstance->pFunPlayback((HANDLE)lPlayHandle, nDataType, pBuffer, dwBufSize);
	}

}

CPBCvrStream::CPBCvrStream(void)
: m_lLogin(-1)
, m_lPlayHandle(-1)
, m_emPlayType(PB_TYPE_DVR)
{
}

CPBCvrStream::~CPBCvrStream(void)
{
}


/** @fn InitHikSdk
*   @brief 初始化
*   @param[in] 
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBCvrStream::InitCvrSdk()
{
	if (!NET_DVR_Init())
	{
		PLAYBACKSDK_ERROR("CvrStream-NET_DVR_Init falid, error = %d", NET_DVR_GetLastError());
		return FALSE;
	}

	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, TRUE);

	CString strIniPath;
	AppFun_GetDllPath(strIniPath);
	strIniPath = strIniPath + _T("PlaybackSDK.ini");
	BOOL bWriteLog = GetPrivateProfileInt(_T("Config"), _T("HikLog"), 0, strIniPath);
	if (bWriteLog)
	{
		NET_DVR_SetLogToFile(3);
	}
	
	PLAYBACKSDK_INFO("初始化成功");

	return TRUE;
}

/** @fn ReleaseHikSdk
*   @brief 释放sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBCvrStream::ReleaseHikSdk()
{
	if (!NET_DVR_Cleanup())
    {
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_Cleanup falid, error = %d", NET_DVR_GetLastError());
		return;
	}
	PLAYBACKSDK_INFO("CvrStream-释放Sdk成功");
}


/** @fn Login
*   @brief 登录设备，获取类句柄，单个回放或下载操作即在此类上进行
*   @param[in] stLoginInfo: 登录设备信息
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBCvrStream::Login(PB_LOGININFO& stLoginInfo)
{
	PB_DVRLOGININFO* pStDvrLoginInfo = &(stLoginInfo.unLoginType.stDvrLoginInfo);
	if (NULL == pStDvrLoginInfo)
	{
		return FALSE;
	}

	m_emPlayer = PT_HIK;
	m_emPlayType = stLoginInfo.emPlayType;

	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
	strcpy((char *)struLoginInfo.sDeviceAddress,pStDvrLoginInfo->szIP);  //设备IP 地址
	strcpy((char *)struLoginInfo.sUserName,pStDvrLoginInfo->szUserName); //设备登录用户名
	strcpy((char *)struLoginInfo.sPassword,pStDvrLoginInfo->szPassword); //设备登录密码
	struLoginInfo.wPort = pStDvrLoginInfo->nPort;                        //设备登录端口
	struLoginInfo.bUseAsynLogin = 0;                                     //同步登录，登录接口返回成功即登录成功

	m_lLogin = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
	if (-1 == m_lLogin)
	{
		PLAYBACKSDK_ERROR("CvrStream-NET_DVR_Login_V40 falid, error = %d", NET_DVR_GetLastError());
		return FALSE;
	}
	return TRUE;
}

/** @fn Logout
*   @brief 登出设备,销毁句柄
*   @param[in] hLogin: 登录句柄
*   @param NULL
*   @return NULL
*/
void CPBCvrStream::Logout()
{
	if (m_lLogin != -1)
	{
		if (!NET_DVR_Logout(m_lLogin))
		{
            PLAYBACKSDK_ERROR("CvrStream-NET_DVR_Logout falid, LoginID=%ld, errCode=%0x,ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		}
		m_lLogin = -1;
	}
}

/** @fn FindFile
*   @brief 查找文件
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
BOOL CPBCvrStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

    //按事件查询
	stFindFileParam.bByEvent = TRUE;
    if (stFindFileParam.bByEvent)
    {
        return FindFileByEvent(stFindFileParam, lpFileList, nFileCount);
    }
	else
	{
		return FindFileByTime(stFindFileParam, lpFileList, nFileCount);
	}
}

/** @fn PlaybackTime
*   @brief 时间回放
*   @param[in] stPbTimeInfo: 时间回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBCvrStream::PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo)
{
		m_stPlaybackTimeInfo = stPbTimeInfo;
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	NET_DVR_TIME stNetStartTime, stNetStopTime;
	memcpy_s(&stNetStartTime, sizeof(NET_DVR_TIME), &(stPbTimeInfo.stStartTime), sizeof(PB_TIME));
	memcpy_s(&stNetStopTime, sizeof(NET_DVR_TIME), &(stPbTimeInfo.stStopTime), sizeof(PB_TIME));

	m_hPlayHwnd = stPbTimeInfo.hWnd;
	m_pDrawFunc = (VIDEODRAWFUNC)stPbTimeInfo.pDrawFunc;  //保存画图函数
	m_nDrawUserData = stPbTimeInfo.nUser;     //用户数据

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("CvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-时间回放参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond, stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	//回放，窗口句柄设定为NULL
	if (!StartData(m_lLogin, stPbTimeInfo.szStreamID, stNetStartTime, stNetStopTime, stPbTimeInfo.bConvertPlay
		, stPbTimeInfo.stConvertParam.byResolution, stPbTimeInfo.stConvertParam.dwVideoBitrate, stPbTimeInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("CvrStream-StartData false，lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread, lLogin=%ld", (LONG)m_lLogin);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-时间回放成功. lLogin=%ld", (LONG)m_lLogin);

	return TRUE;
}

/** @fn PlaybackFile
*   @brief 文件回放
*   @param[in] stPbFileInfo: 文件回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBCvrStream::PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)
{
	m_stPlaybackTimeInfo.stStartTime = stPbFileInfo.stFileInfo.stStartTime;
	m_stPlaybackTimeInfo.stStopTime = stPbFileInfo.stFileInfo.stEndTime;
	m_stPlaybackTimeInfo.lChannel = stPbFileInfo.lChannel;
	memcpy_s(&m_stPlaybackTimeInfo.szStreamID, 32, &(stPbFileInfo.szStreamID), 32);
	m_stPlaybackTimeInfo.hWnd = stPbFileInfo.hWnd;
	m_stPlaybackTimeInfo.bConvertPlay = stPbFileInfo.bConvertPlay;
	m_stPlaybackTimeInfo.stConvertParam = stPbFileInfo.stConvertParam;
	m_stPlaybackTimeInfo.lBwControl = stPbFileInfo.lBwControl;

	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	NET_DVR_TIME stNetStartTime, stNetStopTime;
	memcpy_s(&stNetStartTime, sizeof(NET_DVR_TIME), &(stPbFileInfo.stFileInfo.stStartTime), sizeof(PB_TIME));
	memcpy_s(&stNetStopTime, sizeof(NET_DVR_TIME), &(stPbFileInfo.stFileInfo.stEndTime), sizeof(PB_TIME));

	m_hPlayHwnd = stPbFileInfo.hWnd;
	m_pDrawFunc = (VIDEODRAWFUNC)stPbFileInfo.pDrawFunc;  //保存画图函数
	m_nDrawUserData = stPbFileInfo.nUser;     //用户数据

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("CvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-时间回放参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbFileInfo.stFileInfo.stStartTime.dwYear, stPbFileInfo.stFileInfo.stStartTime.dwMonth, stPbFileInfo.stFileInfo.stStartTime.dwDay,
		stPbFileInfo.stFileInfo.stStartTime.dwHour, stPbFileInfo.stFileInfo.stStartTime.dwMinute, stPbFileInfo.stFileInfo.stStartTime.dwSecond, stPbFileInfo.stFileInfo.stEndTime.dwYear, stPbFileInfo.stFileInfo.stEndTime.dwMonth, stPbFileInfo.stFileInfo.stEndTime.dwDay,
		stPbFileInfo.stFileInfo.stEndTime.dwHour, stPbFileInfo.stFileInfo.stEndTime.dwMinute, stPbFileInfo.stFileInfo.stEndTime.dwSecond);

	//回放，窗口句柄设定为NULL
	if (!StartData(m_lLogin, stPbFileInfo.szStreamID, stNetStartTime, stNetStopTime, stPbFileInfo.bConvertPlay
		, stPbFileInfo.stConvertParam.byResolution, stPbFileInfo.stConvertParam.dwVideoBitrate, stPbFileInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("CvrStream-StartData false，lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-时间回放成功. lLogin=%ld", m_lLogin);

	return TRUE;
}

/** @fn StopPlayback
*   @brief 停止回放
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBCvrStream::StopPlayback()
{
	if (-1 == m_lLogin)
	{
		return;
	}

	// 停止取流.
	StopData();

	//停止解码
	StopPlay();
	
	//关闭线程
	StopStreamThread(TRUE);
	
	PLAYBACKSDK_INFO("CvrStream-停止回放. lLogin=%ld", m_lLogin);

	CPlayStream::StopPlayback();
}

/** @fn PB_DownloadFile
*   @brief 文件下载
*   @param[in] stDownloadParam: 下载参数
*   @param NULL
*   @return 下载句柄，NULL表示下载失败
*/
BOOL CPBCvrStream::DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	NET_DVR_TIME stNetStartTime, stNetStopTime;
	memcpy_s(&stNetStartTime, sizeof(NET_DVR_TIME), &(stDownloadParam.stFileInfo.stStartTime), sizeof(PB_TIME));
	memcpy_s(&stNetStopTime, sizeof(NET_DVR_TIME), &(stDownloadParam.stFileInfo.stEndTime), sizeof(PB_TIME));

	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PLAYBACKSDK_ERROR("CvrStream-Create_File falid, 文件名: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;
	//取文件流
	PLAYBACKSDK_INFO("CvrStream-文件下载参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour, stDownloadParam.stFileInfo.stStartTime.dwMinute, stDownloadParam.stFileInfo.stStartTime.dwSecond, stDownloadParam.stFileInfo.stEndTime.dwYear, stDownloadParam.stFileInfo.stEndTime.dwMonth, stDownloadParam.stFileInfo.stEndTime.dwDay,
		stDownloadParam.stFileInfo.stEndTime.dwHour, stDownloadParam.stFileInfo.stEndTime.dwMinute, stDownloadParam.stFileInfo.stEndTime.dwSecond);

	if (!StartData(m_lLogin, stDownloadParam.szStreamID, stNetStartTime, stNetStopTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CvrStream-StartData, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	//开始取流时间
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/** @fn PB_DownloadTime
*   @brief 时间下载
*   @param[in] stDownloadParam: 下载信息
*   @param NULL
*   @return 下载句柄，NULL表示下载失败
*/
BOOL CPBCvrStream::DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	NET_DVR_TIME stNetStartTime, stNetStopTime;
	memcpy_s(&stNetStartTime, sizeof(NET_DVR_TIME), &(stDownloadParam.stStartTime), sizeof(PB_TIME));
	memcpy_s(&stNetStopTime, sizeof(NET_DVR_TIME), &(stDownloadParam.stStopTime), sizeof(PB_TIME));

	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PLAYBACKSDK_ERROR("CvrStream-Create_File falid, 文件名: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nDownloadPos = 0;
	PLAYBACKSDK_INFO("CvrStream-时间下载参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour, stDownloadParam.stStartTime.dwMinute, stDownloadParam.stStartTime.dwSecond, stDownloadParam.stStopTime.dwYear, stDownloadParam.stStopTime.dwMonth, stDownloadParam.stStopTime.dwDay,
		stDownloadParam.stStopTime.dwHour, stDownloadParam.stStopTime.dwMinute, stDownloadParam.stStopTime.dwSecond);
	if (!StartData(m_lLogin, stDownloadParam.szStreamID, stNetStartTime, stNetStopTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CvrStream-StartData falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
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
void CPBCvrStream::StopDownload()
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

		PLAYBACKSDK_INFO("CvrStream-停止下载. lLogin=%ld", m_lLogin);
	}
}

/**   @fn          StartData
 *    @brief   	   开始取流。
 *    @param[in]   lLogin:登陆ID.
 *    @param[in]   stNetStartTime:开始时间.
 *	  @param[in]   stNetStopTime:结束时间.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPBCvrStream::StartData(LONG lLogin, const char* pStreamID, NET_DVR_TIME& stNetStartTime, NET_DVR_TIME& stNetStopTime
		, BOOL bConvertPlay, BYTE byResolution, DWORD dwVideoBitrate, DWORD dwVideoFrameRate)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	//如果在回放中，停流
	StopData();

	m_bBlock = TRUE;

	//回放，窗口句柄设定为NULL
	NET_DVR_VOD_PARA struVodPara={0};
	struVodPara.dwSize=sizeof(struVodPara);
	strncpy((char*)struVodPara.struIDInfo.byID, pStreamID, STREAM_ID_LEN);
    struVodPara.struIDInfo.byID[STREAM_ID_LEN-1] = NULL;
	struVodPara.hWnd=NULL; //回放窗口
	struVodPara.struBeginTime = stNetStartTime;
	struVodPara.struEndTime = stNetStopTime;
	m_lPlayHandle = NET_DVR_PlayBackByTime_V40(m_lLogin, &struVodPara);
	if (-1 == m_lPlayHandle)
	{
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_PlayBackByTime_V40 falid，lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//设定回调
	NET_DVR_SetPlayDataCallBack(m_lPlayHandle, pCvrStream, PtrToUlong(this));

	//开始取流
	if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_PLAYSTART, 0, NULL))
	{
		NET_DVR_StopPlayBack(m_lPlayHandle);
		m_lPlayHandle = -1;
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_PlayBackControl_V40 falid， ErrorCode = %ld, ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}
	//带宽控制：目前CVR设备还不支持带宽控制，所以调用接口虽然成功，但是设置无效
	if (m_stPlaybackTimeInfo.lBwControl != 0)
	{
		LONG lBwControl = (m_stPlaybackTimeInfo.lBwControl);
		if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_SETSPEED, &lBwControl, NULL))
		{
			NET_DVR_StopPlayBack(m_lPlayHandle);
			m_lPlayHandle = -1;
			PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackControl falid， ErrorCode = %ld, ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
			return FALSE;
		}
	}

	return TRUE;
}

/**   @fn          StopData
 *    @brief   	   停止取流.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPBCvrStream::StopData()
{
	if (-1 == m_lPlayHandle)
	{
		return TRUE;
	}

	// 停止数据回调阻塞.
	m_bBlock = FALSE;

	//停止取流
	BOOL bRet = TRUE;
	
	if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_PLAYSTOP))
	{
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_PlayBackControl_V40 falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		bRet = FALSE;
	}
	//关闭回放
	if (bRet && !NET_DVR_StopPlayBack(m_lPlayHandle))
	{
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_StopPlayBack falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		bRet = FALSE;
	}

	m_lPlayHandle = -1;

	return bRet;
}

/**   @fn          ChangeFileType
 *    @brief   	   nFileType:文件类型转换.
 *    @param[in]   nFileType:文件类型.
 *    @param[in]   
 *    @return      新的文件类型.
 */
int CPBCvrStream::ChangeFileType(int nFileType)
{
	switch (nFileType)
	{
		// 定时录像.
	case 0:
		return RECORD_TYPE_TIMING;

		// 移动侦测.
	case 1:
		return RECORD_TYPE_MOVE;

		// 报警触发.
	case 2:
		return RECORD_TYPE_ALARM_TRIGER;

		// 报警|移动侦测.
	case 3:
		return RECORD_TYPE_ALARM_OR_MOVE;

		// 报警&移动侦测.
	case 4:
		return RECORD_TYPE_ALARM_AND_MOVE;
		
		// 命令触发.
	case 5:
		return RECORD_TYPE_CMD_TRIGER;

		// 手动录像.
	case 6:
		return RECORD_TYPE_MANUAL_RECORD;

		 //越警侦测
	case 41:
		return RECORD_TYPE_VCA_TRAVERSE_PLANE;

		 //区域入侵侦测
	case 42:
		return RECORD_TYPE_FIELD_DETECTION;

		//音频输入异常
	case 43:
		return RECORD_TYPE_AUDIO_INPUT_ALARM;

		//声强突变侦测
	case 44:
		return RECORD_TYPE_SOUND_INTENSITY_ALARM;

		//人脸侦测
	case 45:
		return RECORD_TYPE_FACE_DETECTION;

		//虚焦侦测
	case 46:
		return RECORD_TYPE_VIRTUAL_FOCUS_ALARM;

		//场景变更侦测
	case 47:
		return RECORD_TYPE_SCENE_CHANGE_ALARM;

	default:
		return RECORD_TYPE_ALL;
	}
}

/**  @fn          IsCardNumAllowed
*    @brief   	  是否支持按卡号查询.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPBCvrStream::IsCardNumAllowed()
{
	return TRUE;
}

/**  @fn          IsEventAllowed
*    @brief   	  是否支持按事件查询.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPBCvrStream::IsEventAllowed()
{
    return TRUE;
}

/**   @fn          GetCompressCfgAbility
*    @brief   	   获取压缩参数能力集.
*    @param[out]   stCompresscfgAbility:能力集结构体.
*    @return     
*/
BOOL CPBCvrStream::GetCompressCfgAbility( char* pOutbuf )
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	BOOL bRet = FALSE;
	CMarkup xmlCreator;
	xmlCreator.SetDoc("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	xmlCreator.AddElem("RecordAbility version=\"2.0\"");

	CString strXML = xmlCreator.GetDoc();
	if(!NET_DVR_GetDeviceAbility(m_lLogin, DEVICE_ABILITY_INFO,strXML.GetBuffer(0), strXML.GetLength(), pOutbuf, XML_ABILITY_OUT_LEN))
	{
		bRet = FALSE;
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_GetDeviceAbility falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
	}
	else
	{
		bRet = TRUE;
	}
	return bRet;
}

/** @fn SmartSearch
*   @brief 智能检索
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
BOOL CPBCvrStream::SmartSearch(SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	NET_DVR_SMART_SEARCH_PARAM stFileCond;
	stFileCond.bySearchCondType = (BYTE)stSmartSearchParam.nRecordType;
	stFileCond.byChan = (BYTE)m_lChannel;

	memcpy_s(&(stFileCond.struStartTime), sizeof(NET_DVR_TIME), &(stSmartSearchParam.stStartTime), sizeof(PB_TIME));
	memcpy_s(&(stFileCond.struEndTime), sizeof(NET_DVR_TIME), &(stSmartSearchParam.stStopTime), sizeof(PB_TIME));

	switch(stFileCond.bySearchCondType)
	{
	case 0:
		{
			stFileCond.bySensitivity = (BYTE)stSmartSearchParam.dwSensitivity;
			memcpy_s(&(stFileCond.uSmartSearchCond.byMotionScope), sizeof(stFileCond.uSmartSearchCond.byMotionScope)
				, &(stSmartSearchParam.byMotionScope), sizeof(stSmartSearchParam.byMotionScope));
		}
		break;
	case 1:
		{
			stFileCond.uSmartSearchCond.struTraversPlaneCond.struVcaTraversePlane[stSmartSearchParam.dwCurArea].bySensitivity = (BYTE)stSmartSearchParam.dwSensitivity;
			stFileCond.uSmartSearchCond.struTraversPlaneCond.struVcaTraversePlane[stSmartSearchParam.dwCurArea].dwCrossDirection = (VCA_CROSS_DIRECTION)stSmartSearchParam.dwCrossDirection;
			stFileCond.uSmartSearchCond.struTraversPlaneCond.dwPreTime  = stSmartSearchParam.dwPreTime;
			stFileCond.uSmartSearchCond.struTraversPlaneCond.dwDelayTime = stSmartSearchParam.dwDelayTime;
			memcpy_s(&(stFileCond.uSmartSearchCond.struTraversPlaneCond.struVcaTraversePlane[stSmartSearchParam.dwCurArea].struPlaneBottom.struStart),
				sizeof(NET_VCA_POINT), &(stSmartSearchParam.struPos[0]), sizeof(PB_POINT));
			memcpy_s(&(stFileCond.uSmartSearchCond.struTraversPlaneCond.struVcaTraversePlane[stSmartSearchParam.dwCurArea].struPlaneBottom.struEnd),
				sizeof(NET_VCA_POINT), &(stSmartSearchParam.struPos[1]), sizeof(PB_POINT));
		}
		break;
	case 2:
		{
			stFileCond.uSmartSearchCond.struIntrusionCond.struVcaIntrusion[stSmartSearchParam.dwCurArea].byRate = (BYTE)stSmartSearchParam.dwIntrusionRate;
			stFileCond.uSmartSearchCond.struIntrusionCond.struVcaIntrusion[stSmartSearchParam.dwCurArea].wDuration = (WORD)stSmartSearchParam.dwIntrusionDuration;
			stFileCond.uSmartSearchCond.struIntrusionCond.struVcaIntrusion[stSmartSearchParam.dwCurArea].bySensitivity = (BYTE)stSmartSearchParam.dwSensitivity;
			stFileCond.uSmartSearchCond.struIntrusionCond.dwPreTime = stSmartSearchParam.dwPreTime;
			stFileCond.uSmartSearchCond.struIntrusionCond.dwDelayTime = stSmartSearchParam.dwDelayTime;
			stFileCond.uSmartSearchCond.struIntrusionCond.struVcaIntrusion[stSmartSearchParam.dwCurArea].struRegion.dwPointNum = stSmartSearchParam.dwPointNum;
			for (int i = 0; i < (int)stSmartSearchParam.dwPointNum; i++)
			{
				memcpy_s(&(stFileCond.uSmartSearchCond.struIntrusionCond.struVcaIntrusion[stSmartSearchParam.dwCurArea].struRegion.struPos[i]),
					sizeof(NET_VCA_POINT), &(stSmartSearchParam.struPos[i]), sizeof(PB_POINT));
			}
		}
		break;
	default:
		PB_SetLastError(PB_UNSUPPORT_SMARTSEARCH);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-智能检索参数. channel=%d, recordType:%d, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d",
		stSmartSearchParam.lChannel, stFileCond.bySearchCondType, stSmartSearchParam.stStartTime.dwYear, stSmartSearchParam.stStartTime.dwMonth
		, stSmartSearchParam.stStartTime.dwDay,stSmartSearchParam.stStartTime.dwHour, stSmartSearchParam.stStartTime.dwMinute
		, stSmartSearchParam.stStartTime.dwSecond, stSmartSearchParam.stStopTime.dwYear, stSmartSearchParam.stStopTime.dwMonth
		, stSmartSearchParam.stStopTime.dwDay,stSmartSearchParam.stStopTime.dwHour, stSmartSearchParam.stStopTime.dwMinute, stSmartSearchParam.stStopTime.dwSecond);

	LONG lFindHandle = NET_DVR_SmartSearch(m_lLogin, &stFileCond);
	if (lFindHandle == -1)
	{
		DWORD dwErrCode = NET_DVR_GetLastError();
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_SmartSearch falid, m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
		return FALSE;
	}

	nFileCount = 0;
	BOOL bFind = TRUE;
	BOOL bReturn = TRUE;
	while (bFind)
	{
		NET_DVR_SMART_SEARCH_RET stFileData = {0};
		LONG lFileHandle = NET_DVR_SearchNextInfo(lFindHandle, &stFileData);
		switch (lFileHandle)
		{
		case NET_DVR_FILE_SUCCESS:  //获取文件成功
			{
				CString strFileName;
				//文件名
				strFileName.Format("Hik_SmartSearch_%d", nFileCount);
				sprintf_s(lpFileList[nFileCount].szFileName, sizeof(lpFileList[nFileCount].szFileName), strFileName.GetBuffer());
				lpFileList[nFileCount].dwFileSize = 0;
				memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(stFileData.struStartTime), sizeof(NET_DVR_TIME));
				memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(stFileData.struEndTime), sizeof(NET_DVR_TIME));
				// 文件类型转换.
				lpFileList[nFileCount].nFileType = -1;				
				nFileCount++;
				if (nFileCount >= MAX_RECORD_COUNT)
				{
					PLAYBACKSDK_ERROR("CvrStream-查找到了%d个录像文件（最大缓冲)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //没有查到文件
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("CvrStream-没有查到文件");
			}
			break;
		case NET_DVR_ISFINDING:     //正在查找
			{
				Sleep(5);
			}
			break;
		case NET_DVR_NOMOREFILE:    //查找结束了
			{
				bFind = FALSE;
			}
			break;
		case NET_DVR_FILE_EXCEPTION:     //查找文件异常
			{
				bFind = FALSE;
				bReturn = FALSE;
                PLAYBACKSDK_ERROR("CvrStream-查找文件异常, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
			}
			break;
		default:                    //查找失败
			{
				bFind = FALSE;
				bReturn = FALSE;
			}
			break;
		}
	}

	//关闭查找
	NET_DVR_StopSearch(lFindHandle);

	PLAYBACKSDK_INFO("CvrStream-智能检索成功,文件个数:%d. ", nFileCount);

	return bReturn;
}

BOOL CPBCvrStream::GetVideoFormat( int& nVideoFormat )
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	NET_DVR_PICCFG_V30  struDvrPicCfg;
	memset(&struDvrPicCfg, 0, sizeof(struDvrPicCfg));
	DWORD dwReturn = 0;
	BOOL bRet = FALSE;
	if(!NET_DVR_GetDVRConfig(m_lLogin, NET_DVR_GET_PICCFG_V30, m_lChannel, &struDvrPicCfg, sizeof(struDvrPicCfg), &dwReturn))
	{
		bRet = FALSE;
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_GetDVRConfig falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
	}
	else
	{
		nVideoFormat = struDvrPicCfg.dwVideoFormat;
		bRet = TRUE;
	}
	return bRet;
}

BOOL CPBCvrStream::FindFileByTime( FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount )
{
    if (-1 == m_lLogin)
    {
        return FALSE;
    }
	m_lChannel = stFindFileParam.lChannel + 1;

	NET_DVR_FILECOND_V40 struFileCond={0};
	struFileCond.dwFileType = 0xFF;
	struFileCond.lChannel = stFindFileParam.lChannel; //通道号
	struFileCond.dwIsLocked = 0xFF;
	struFileCond.dwUseCardNo = 0;
    memcpy_s(&(struFileCond.struStartTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStartTime), sizeof(PB_TIME));
    memcpy_s(&(struFileCond.struStopTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStopTime), sizeof(PB_TIME));

    LONG lFindHandle = NET_DVR_FindFile_V40(m_lLogin, &struFileCond);
    if (lFindHandle == -1)
    {
		PLAYBACKSDK_ERROR("CvrStream-NET_DVR_FindFile_V40 falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
        return FALSE;
    }

    nFileCount = 0;
    BOOL bFind = TRUE;
    BOOL bReturn = TRUE;
    while (bFind)
    {
		NET_DVR_FINDDATA_V40 struFileData;
        LONG lFileHandle = NET_DVR_FindNextFile_V40(lFindHandle, &struFileData);
        switch (lFileHandle)
        {
        case NET_DVR_FILE_SUCCESS:  //获取文件成功
            {
                sprintf_s(lpFileList[nFileCount].szFileName, sizeof(lpFileList[nFileCount].szFileName), struFileData.sFileName);
                lpFileList[nFileCount].dwFileSize = struFileData.dwFileSize;
                memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(struFileData.struStartTime), sizeof(NET_DVR_TIME));
                memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(struFileData.struStopTime), sizeof(NET_DVR_TIME));
                // 文件类型转换.
                lpFileList[nFileCount].nFileType = struFileData.byFileType;			
                nFileCount++;
                if (nFileCount >= MAX_RECORD_COUNT)
                {
                    PLAYBACKSDK_ERROR("CvrStream-查找到了%d个录像文件（最大缓冲)", MAX_RECORD_COUNT);
                    nFileCount = MAX_RECORD_COUNT;
                    bFind = FALSE;
                }
            }
            break;
        case NET_DVR_FILE_NOFIND:   //没有查到文件
            {
                nFileCount = 0;
                bFind = FALSE;
                PLAYBACKSDK_INFO("CvrStream-没有查到文件");
            }
            break;
        case NET_DVR_ISFINDING:     //正在查找
            {
                Sleep(5);
            }
            break;
        case NET_DVR_NOMOREFILE:    //查找结束了
            {
                bFind = FALSE;
            }
            break;
        case NET_DVR_FILE_EXCEPTION:     //查找文件异常
            {
                bFind = FALSE;
                bReturn = FALSE;
                PLAYBACKSDK_ERROR("CvrStream-查找文件异常, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
            }
            break;
        default:                    //查找失败
            {
                bFind = FALSE;
                bReturn = FALSE;
            }
            break;
        }
    }

    //关闭查找
    NET_DVR_FindClose_V30(lFindHandle);

    PLAYBACKSDK_INFO("CvrStream-按事件查询文件成功,文件个数:%d. ", nFileCount);

    return bReturn;
}

BOOL CPBCvrStream::FindFileByEvent( FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount )
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	NET_DVR_SEARCH_EVENT_PARAM stFileCond;
	stFileCond.wMajorType = (WORD)EVENT_STREAM_INFO;
	stFileCond.wMinorType = (WORD)EVENT_STREAM_ID;

	memcpy_s(&(stFileCond.struStartTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStartTime), sizeof(PB_TIME));
	memcpy_s(&(stFileCond.struEndTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStopTime), sizeof(PB_TIME));

	stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
	strncpy((char*)stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID, stFindFileParam.szStreamID, STREAM_ID_LEN);
    stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID[STREAM_ID_LEN - 1] = NULL;  //防止没有结尾
	stFileCond.uSeniorParam.struStreamIDParam.byBackupVolumeNum = 0;

	LONG lFindHandle = NET_DVR_FindFileByEvent(m_lLogin, &stFileCond);
	if (-1 == lFindHandle)
	{
		SetLastError(NET_DVR_GetLastError());
		DWORD dwErrCode = NET_DVR_GetLastError();
		PLAYBACKSDK_ERROR("Hik---NET_DVR_FindFileByEvent失败. m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
		return FALSE;
	}

	nFileCount = 0;
	BOOL bFind = TRUE;
	BOOL bReturn = TRUE;
	while (bFind)
	{
		NET_DVR_SEARCH_EVENT_RET stFileData;
		LONG lFileHandle = NET_DVR_FindNextEvent(lFindHandle, &stFileData);
		switch (lFileHandle)
		{
		case NET_DVR_FILE_SUCCESS:  //获取文件成功
			{
				sprintf_s(lpFileList[nFileCount].szFileName, sizeof(lpFileList[nFileCount].szFileName), _T("Event_record_%d"),nFileCount+1);
				memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(stFileData.struStartTime), sizeof(NET_DVR_TIME));
				memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(stFileData.struEndTime), sizeof(NET_DVR_TIME));
				memcpy_s(&(lpFileList[nFileCount].szStreamID), 32, &(stFindFileParam.szStreamID), 32);
				lpFileList[nFileCount].dwFileSize = stFileData.uSeniorRet.struStreamIDRet.dwRecordLength;
				lpFileList[nFileCount].nFileType = stFileData.uSeniorRet.struStreamIDRet.dwRecordType;	
				lpFileList[nFileCount].emPlayType = PB_TYPE_CVR;
				nFileCount++;
				if (nFileCount >= MAX_RECORD_COUNT)
				{
					PLAYBACKSDK_ERROR("Hik---查找到了%d个录像文件（最大缓冲)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //没有查到文件
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("Hik---NET_DVR_FILE_NOFIND.");
				SetLastError(NET_DVR_FILE_NOFIND);
			}
			break;
		case NET_DVR_ISFINDING:     //正在查找
			{
				Sleep(5);
			}
			break;
		case NET_DVR_NOMOREFILE:    //查找结束了
			{
				bFind = FALSE;
			}
			break;
		case NET_DVR_FILE_EXCEPTION:     //查找文件异常
			{
				SetLastError(NET_DVR_GetLastError());
				bFind = FALSE;
				bReturn = FALSE;
				PLAYBACKSDK_ERROR("Hik---NET_DVR_FILE_EXCEPTION. lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
			}
			break;
		default:                    //查找失败
			{
				bFind = FALSE;
				bReturn = FALSE;
				SetLastError(NET_DVR_GetLastError());
			}
			break;
		}
	}

	//关闭查找
	NET_DVR_FindClose_V30(lFindHandle);

	PLAYBACKSDK_INFO("Hik---按事件查询文件成功,文件个数:%d. ", nFileCount);

	return bReturn;
}
