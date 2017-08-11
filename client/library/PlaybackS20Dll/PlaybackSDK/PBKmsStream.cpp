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
#include "PBKmsStream.h"


int CALLBACK pKmsMsg( int sessionhandle, void* userdata, int opt, void* param1, void* param2, void* param3, void* param4 )
{
	return 1;
}

int CALLBACK pKmsStream(int sessionhandle, void* userdata, int datatype, void* pdata, int ilen)
{
	CPBKmsStream * pInstance = (CPBKmsStream*)ULongToPtr(userdata);
	if (pInstance == NULL)
	{
		return 0;
	}
	// 流类型.
	int nDataType = -1;

	switch (datatype)
	{
	case 2:			//数据流
		{
			nDataType = PB_STREAM_DATA;
		}
		break;
	case 1:         //数据头
		{
			nDataType = PB_STREAM_HEADER;
		}
		break;

	default:        //结束
		nDataType = PB_STREAM_END;
		return 0;
	}

	if (pInstance->m_bDownload)
	{
		pInstance->pFunDownload((HANDLE)sessionhandle, nDataType, (BYTE*)pdata, ilen);
	}

	//KMS无法回放：回调数据是否仅有40字节头(没有实体数据)？下载录像下来看
	if (pInstance->m_lPort != -1)
	{
		pInstance->pFunPlayback((HANDLE)sessionhandle, nDataType, (BYTE*)pdata, ilen);
	}
	return 0;
}

CPBKmsStream::CPBKmsStream(void)
: m_lLogin(-1)
, m_lPlayHandle(-1)
, m_emPlayType(PB_TYPE_DVR)
{
	m_bStreamPause = FALSE;
}

CPBKmsStream::~CPBKmsStream(void)
{
}


/** @fn InitHikSdk
*   @brief 初始化
*   @param[in] 
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBKmsStream::InitKmsSdk()
{
	int iRet = StreamClient_InitLib();
	if (0 != iRet)
	{
		PLAYBACKSDK_ERROR("KmsStream-StreamClient_InitLib falid, error = %d", iRet);
		return FALSE;
	}
	return TRUE;
}

/** @fn ReleaseHikSdk
*   @brief 释放sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBKmsStream::ReleaseHikSdk()
{
	int iRet = StreamClient_FiniLib();
	if (0 != iRet)
	{
		PLAYBACKSDK_ERROR("KmsStream-StreamClient_InitLib falid, error = %d", iRet);
	}
}


/** @fn Login
*   @brief 登录设备，获取类句柄，单个回放或下载操作即在此类上进行
*   @param[in] stLoginInfo: 登录设备信息
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBKmsStream::Login(PB_LOGININFO& stLoginInfo)
{
	return TRUE;
}

/** @fn Logout
*   @brief 登出设备,销毁句柄
*   @param[in] hLogin: 登录句柄
*   @param NULL
*   @return NULL
*/
void CPBKmsStream::Logout()
{
}

/** @fn FindFile
*   @brief 查找文件
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
BOOL CPBKmsStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	return TRUE;
}

/** @fn PlaybackTime
*   @brief 时间回放
*   @param[in] stPbTimeInfo: 时间回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBKmsStream::PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo)
{
	if ('\0' == stPbTimeInfo.szPlayUrl[0])
	{
		PLAYBACKSDK_ERROR("KmsStream-PlayUrl is empty.");
		return FALSE;
	}
	PLAYBACKSDK_INFO("KmsStream-PlayUrl:%s", stPbTimeInfo.szPlayUrl);
	m_lLogin = StreamClient_CreateSession();
	if (m_lLogin == -1)
	{
		PLAYBACKSDK_ERROR("KmsStream-StreamClient_CreateSession falid.");
		return FALSE;
	}
	//设置流回调
	StreamClient_SetMsgCallBack(m_lLogin, pKmsMsg,this);
	StreamClient_SetDataCallBack(m_lLogin, pKmsStream, this);
	//时间回放
	if (NULL == stPbTimeInfo.hWnd)
	{
		PLAYBACKSDK_ERROR("KmsStream-hWnd is null.");
		return FALSE;
	}
	m_hPlayHwnd = stPbTimeInfo.hWnd;
	if (!GPAPI_Startup(PT_HIK,NULL,NULL,NULL))
	{
		PLAYBACKSDK_ERROR("KmsStream-GPAPI_Startup falid.");
		return FALSE;
	}

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		StreamClient_DestroySession(m_lLogin);
		PLAYBACKSDK_ERROR("KmsStream-GPAPI_GetPort falid.");
		return FALSE;
	}

	ABS_TIME absStartTime, absStopTime;
	memcpy_s(&absStartTime, sizeof(ABS_TIME), &(stPbTimeInfo.stStartTime), sizeof(PB_TIME));
	memcpy_s(&absStopTime, sizeof(ABS_TIME), &(stPbTimeInfo.stStopTime), sizeof(PB_TIME));

	int iRet = 0;
	iRet = StreamClient_PlayBackByTime(m_lLogin, NULL, stPbTimeInfo.szPlayUrl, 
		"StreamClient", RTPRTSP_TRANSMODE, "admin", "admin", &absStartTime, &absStopTime);
	if (iRet < 0)
	{
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("KmsStream-StreamClient_PlayBackByTime falid, error = ", iRet);
		return FALSE;
	}

	//开启流检测线程
	m_bPlaybackThread = TRUE;
	DWORD dwRet;
	m_hPlaybackThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlaybackStreamDetect, (LPVOID)this, 0, &dwRet);
	if (m_hPlaybackThread == NULL)
	{
		m_bPlaybackThread = FALSE;
		StreamClient_Stop(m_lLogin);
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("KmsStream-PlaybackStreamDetect falid.");
		return FALSE;
	}
	m_stPlaybackTimeInfo = stPbTimeInfo;
	m_lPlaybackLastTime = (LONG)CTime::GetCurrentTime().GetTime();
	return TRUE;
}

/** @fn PlaybackFile
*   @brief 文件回放
*   @param[in] stPbFileInfo: 文件回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBKmsStream::PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)
{
	//login
	m_lLogin = StreamClient_CreateSession();
	//设置流回调
	StreamClient_SetMsgCallBack(m_lLogin, pKmsMsg,this);
	StreamClient_SetDataCallBack(m_lLogin, pKmsStream, this);
	//时间回放

	m_hPlayHwnd = stPbFileInfo.hWnd;
	if (!GPAPI_Startup(PT_HIK,NULL,NULL,NULL))
	{
		PLAYBACKSDK_ERROR("KmsStream-GPAPI_Startup falid.");
		return FALSE;
	}
	//用户数据
	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		StreamClient_DestroySession(m_lLogin);
		PLAYBACKSDK_ERROR("KmsStream-GPAPI_GetPort falid.");
		return FALSE;
	}

	m_bBlock = TRUE;
	ABS_TIME absStartTime, absStopTime;
	memcpy_s(&absStartTime, sizeof(ABS_TIME), &(stPbFileInfo.stFileInfo.stStartTime), sizeof(PB_TIME));
	memcpy_s(&absStopTime, sizeof(ABS_TIME), &(stPbFileInfo.stFileInfo.stEndTime), sizeof(PB_TIME));

	int iRet = 0;
	iRet = StreamClient_PlayBackByTime(m_lLogin, NULL, stPbFileInfo.stFileInfo.szPlayUrl, 
		"StreamClient", RTPRTSP_TRANSMODE, "admin", "admin", &absStartTime, &absStopTime);
	if (iRet < 0)
	{
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("KmsStream-StreamClient_PlayBackByTime falid, error = %d", iRet);
		return FALSE;
	}

	//开启流检测线程
	m_bPlaybackThread = TRUE;
	DWORD dwRet;
	m_hPlaybackThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlaybackStreamDetect, (LPVOID)this, 0, &dwRet);
	if (m_hPlaybackThread == NULL)
	{
		m_bPlaybackThread = FALSE;
		StreamClient_Stop(m_lLogin);
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("KmsStream-PlaybackStreamDetect falid");
		return FALSE;
	}
	m_lPlaybackLastTime = (LONG)CTime::GetCurrentTime().GetTime();
	strcpy_s(m_stPlaybackTimeInfo.szPlayUrl, sizeof(m_stPlaybackTimeInfo.szPlayUrl), stPbFileInfo.stFileInfo.szPlayUrl);
	m_stPlaybackTimeInfo.stStartTime = stPbFileInfo.stFileInfo.stStartTime;
	m_stPlaybackTimeInfo.stStopTime = stPbFileInfo.stFileInfo.stEndTime;
	m_stPlaybackTimeInfo.lChannel = stPbFileInfo.lChannel;
	m_stPlaybackTimeInfo.hWnd = stPbFileInfo.hWnd;

	return TRUE;
}

/** @fn StopPlayback
*   @brief 停止回放
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBKmsStream::StopPlayback()
{
	if (-1 == m_lLogin)
	{
		return;
	}
	// 停止取流.
	StopData();
	// 停止解码
	StopPlay();
	// 关闭线程
	StopStreamThread(TRUE);
	
	PLAYBACKSDK_INFO("KmsStream-停止回放. lLogin=%ld", m_lLogin);

	CPlayStream::StopPlayback();
}

/** @fn PB_DownloadFile
*   @brief 文件下载
*   @param[in] stDownloadParam: 下载参数
*   @param NULL
*   @return 下载句柄，NULL表示下载失败
*/
BOOL CPBKmsStream::DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam)
{
	//login
	m_lLogin = StreamClient_CreateSession();
	//设置流回调
	StreamClient_SetMsgCallBack(m_lLogin, pKmsMsg,this);
	StreamClient_SetDataCallBack(m_lLogin, pKmsStream, this);

	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		StreamClient_DestroySession(m_lLogin);
		m_bDownload = FALSE;
		PLAYBACKSDK_ERROR("KmsStream-Create_File falid.");
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;

	//下载流
	ABS_TIME absStartTime, absStopTime;
	memcpy_s(&absStartTime, sizeof(ABS_TIME), &(stDownloadParam.stFileInfo.stStartTime), sizeof(PB_TIME));
	memcpy_s(&absStopTime, sizeof(ABS_TIME), &(stDownloadParam.stFileInfo.stEndTime), sizeof(PB_TIME));
	int iRet = StreamClient_DownLoad(m_lLogin, NULL, stDownloadParam.stFileInfo.szPlayUrl, "StreamClient", RTPRTSP_TRANSMODE,"admin","12345", &absStartTime, &absStopTime);
	if (iRet < 0)
	{
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("KmsStream-StreamClient_DownLoad falid, error = %d", iRet);
		return FALSE;
	}

	//开启流检测线程
	m_bPlaybackThread = TRUE;
	if (!StartStreamThread(FALSE))
	{
		StreamClient_Stop(m_lLogin);
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("KmsStream-KmsStream-DownloadStreamDetect falid.");
		return FALSE;
	}
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();
	return TRUE;
}

/** @fn PB_DownloadTime
*   @brief 时间下载
*   @param[in] stDownloadParam: 下载信息
*   @param NULL
*   @return 下载句柄，NULL表示下载失败
*/
BOOL CPBKmsStream::DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam)
{
	if (stDownloadParam.szPlayUrl[0] == '\0')
	{
		return FALSE;
	}
	PLAYBACKSDK_INFO("KmsStream-PlayURL:%s",stDownloadParam.szPlayUrl);
    CTime startTime(stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
        stDownloadParam.stStartTime.dwHour, stDownloadParam.stStartTime.dwMinute, stDownloadParam.stStartTime.dwSecond);
    CTime stopTime(stDownloadParam.stStopTime.dwYear, stDownloadParam.stStopTime.dwMonth, stDownloadParam.stStopTime.dwDay,
        stDownloadParam.stStopTime.dwHour, stDownloadParam.stStopTime.dwMinute, stDownloadParam.stStopTime.dwSecond);

    //login
    m_lLogin = StreamClient_CreateSession();
    //设置流回调
    StreamClient_SetMsgCallBack(m_lLogin, pKmsMsg,this);
    StreamClient_SetDataCallBack(m_lLogin, pKmsStream, this);

    m_bDownload = TRUE;
    m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
    if (m_hDownloadFile == NULL)
    {
        StreamClient_DestroySession(m_lLogin);
        m_bDownload = FALSE;
		PLAYBACKSDK_INFO("KmsStream-Create_File falid.");
        return FALSE;
    }
    m_dwCurDownSize = 0;
    m_dwTotleSize = 0;
    m_nDownloadPos = 0;

    //下载流
    ABS_TIME absStartTime, absStopTime;
    memcpy_s(&absStartTime, sizeof(ABS_TIME), &(stDownloadParam.stStartTime), sizeof(PB_TIME));
    memcpy_s(&absStopTime, sizeof(ABS_TIME), &(stDownloadParam.stStopTime), sizeof(PB_TIME));
	absStartTime.dwYear = 1970;
	absStopTime.dwYear = 2100;
	int iRet = StreamClient_DownLoad(m_lLogin, NULL, stDownloadParam.szPlayUrl, "StreamClient", RTPRTSP_TRANSMODE, "admin", "12345", &absStartTime, &absStopTime);
    if (iRet < 0)
    {
        StreamClient_DestroySession(m_lLogin);
        m_lLogin = -1;
        CloseHandle(m_hDownloadFile);
        m_hDownloadFile = NULL;
		PLAYBACKSDK_INFO("KmsStream-StreamClient_DownLoad falid, error = %d", iRet);
        return FALSE;
    }

    //开启流检测线程
	m_bPlaybackThread = TRUE;
	if (!StartStreamThread(FALSE))
	{
		StreamClient_Stop(m_lLogin);
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("KmsStream-KmsStream-DownloadStreamDetect falid.");
		return FALSE;
	}
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();
	return TRUE;
}

/** @fn PB_StopDownload
*   @brief 停止下载
*   @param[in] hDownload: 下载句柄
*   @param NULL
*   @return NULL
*/
void CPBKmsStream::StopDownload()
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

		PLAYBACKSDK_INFO("KmsStream-停止下载. lLogin=%ld", m_lLogin);
	}
}

/**   @fn          StopData
 *    @brief   	   停止取流.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPBKmsStream::StopData()
{
	if (m_lLogin >= 0)
	{
		StreamClient_Stop(m_lLogin);
		StreamClient_DestroySession(m_lLogin);
		m_lLogin = -1;
	}
	return TRUE;
}

/**   @fn          ChangeFileType
 *    @brief   	   nFileType:文件类型转换.
 *    @param[in]   nFileType:文件类型.
 *    @param[in]   
 *    @return      新的文件类型.
 */
int CPBKmsStream::ChangeFileType(int nFileType)
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

BOOL CPBKmsStream::PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut)
{
	if (m_lLogin < 0)
	{
		PLAYBACKSDK_ERROR("KmsStream-没有回放");
		return FALSE;
	}
	if (m_lPort == -1)
	{
		PLAYBACKSDK_ERROR("KmsStream-没有解码");
		return FALSE;
	}

	PLAYBACKSDK_INFO("KmsStream-播放控制. emControlType=%d", emControlType);
	switch (emControlType)
	{
	case PB_CONTROL_TYPE_NOREMAL:
	case PB_CONTROL_TYPE_FAST2:
	case PB_CONTROL_TYPE_FAST4:
	case PB_CONTROL_TYPE_FAST8:
	case PB_CONTROL_TYPE_FAST16:
	case PB_CONTROL_TYPE_SLOW2:
	case PB_CONTROL_TYPE_SLOW4:
	case PB_CONTROL_TYPE_SLOW8:
	case PB_CONTROL_TYPE_SLOW16:
		{

			// 改变流控速度
			int iScale = 0;
			int nSpeed = (int)emControlType;
			if (nSpeed >= 0)
			{
				iScale = static_cast<int>(pow((double)2, nSpeed));
			}
			else
			{
				iScale = -static_cast<int>(pow((double)2, -nSpeed));
			}

			if (StreamClient_ChangeRate(m_lLogin, (float)iScale) < 0)
			{
				PLAYBACKSDK_ERROR("KmsStream-流控速率失败. lLogin=%d, speed=%d", m_lLogin, nSpeed);
				return FALSE;
			}

			if (!GPAPI_SetPlaySpeed(m_emPlayer, m_lPort, (PLAYSPEED)emControlType, m_hPlayHwnd))
			{
				PLAYBACKSDK_INFO("KmsStream-调节播放速度失败. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_emPlaySpeed = emControlType;

		}
		break;
	case PB_CONTROL_TYPE_PAUSE:
		{
			if (StreamClient_Pause(m_lLogin) < 0)
			{
				PLAYBACKSDK_ERROR("KmsStream-暂停流失败. lLogin=%d", m_lLogin);
				return FALSE;
			}

			if (!GPAPI_Pause(m_emPlayer, m_lPort, TRUE))
			{
				PLAYBACKSDK_INFO("KmsStream-解码暂停失败. m_lPort=%ld,", m_lPort);
				return FALSE;
			}
			m_bPause = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_RESTART:
		{
			if (StreamClient_Resume(m_lLogin) < 0)
			{
				PLAYBACKSDK_ERROR("KmsStream-恢复流失败. lLogin=%d", m_lLogin);
				return FALSE;
			}

			if (!GPAPI_Pause(m_emPlayer, m_lPort, FALSE))
			{
				PLAYBACKSDK_INFO("KmsStream-恢复失败. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bPause = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_OPEN_AUDIO:
		{
			if (!GPAPI_PlaySound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("KmsStream-开启音频失败. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bOpenSound = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_CLOSE_AUDIO:
		{
			if (!GPAPI_StopSound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("KmsStream-关闭音频失败. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bOpenSound = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_SET_VOLUME:
		{
			if (!GPAPI_SetVolume(m_emPlayer, m_lPort, (WORD)lValueIn))
			{
				PLAYBACKSDK_INFO("KmsStream-调节音量失败. Player=%d, m_lPort=%ld, speed=%d, volumn=%d", m_emPlayer, m_lPort, (int)emControlType, (int)lValueIn);
				return FALSE;
			}
		}
		break;
	case PB_CONTROL_TYPE_SINGLE_FRAME:
		{
			if (!GPAPI_PlayOneByOne(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("KmsStream-单帧播放失败. Player=%d, m_lPort=%ld LastError = %ld info = %s", m_emPlayer, m_lPort, GPAPI_GetLastError(m_emPlayer, m_lPort), GPAPI_GetLastErrorString(m_emPlayer, m_lPort));
				return FALSE;
			}
		}
		break;
	case PB_CONTROL_TYPE_SINGLE_FRAME_RESTORE:
		{
			if (!GPAPI_PlayOneByOneRestore(m_emPlayer, m_lPort, m_hPlayHwnd))
			{
				PLAYBACKSDK_INFO("KmsStream-停止单帧播放失败. Player=%d, m_lPort=%ld", m_emPlayer, m_lPort);
				return FALSE;
			}
		}
		break;
	default:
		PLAYBACKSDK_ERROR("KmsStream-该操作不支持 %d", (int)emControlType);
		return FALSE;
	}

	return TRUE;
}
