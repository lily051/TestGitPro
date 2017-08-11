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
#include "PBDvrStream.h"

/** @fn pStreamPbLayer
*   @brief 取流回调函数
*   @param[in] hPlayData: 回放句柄
*   @param[in] hUserData：用户数据
*   @param[in] nDataType: 流类型
*   @param[in] pBuffer:   流数据
*   @param[in] dwBufferSize: 数据大小
*   @return NULL
*/
void CALLBACK pDvrStream(LONG lPlayHandle, DWORD dwDataType, BYTE * pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	CPBDvrStream * pInstance = (CPBDvrStream*)ULongToPtr(dwUser);
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

CPBDvrStream::CPBDvrStream(void)
: m_lLogin(-1)
, m_lPlayHandle(-1)
, m_emPlayType(PB_TYPE_DVR)
{
}

CPBDvrStream::~CPBDvrStream(void)
{
}


/** @fn InitHikSdk
*   @brief 初始化
*   @param[in] 
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBDvrStream::InitDvrSdk()
{
	if (!NET_DVR_Init())
	{
		PLAYBACKSDK_ERROR("NET_DVR_Init falid, error = ", NET_DVR_GetLastError());
		return FALSE;
	}

	NET_DVR_SetConnectTime(8000, 5);

	CString strIniPath;
	AppFun_GetDllPath(strIniPath);
	strIniPath = strIniPath + _T("PlaybackSDK.ini");
	BOOL bWriteLog = GetPrivateProfileInt(_T("Config"), _T("HikLog"), 0, strIniPath);
	if (bWriteLog)
	{
		NET_DVR_SetLogToFile(3);
	}
	
	PLAYBACKSDK_INFO("DvrStream-初始化成功");

	return TRUE;
}

/** @fn ReleaseHikSdk
*   @brief 释放sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBDvrStream::ReleaseHikSdk()
{
	if (!NET_DVR_Cleanup())
    {
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_Cleanup falid, error = ", NET_DVR_GetLastError());
		return;
	}
}


/** @fn Login
*   @brief 登录设备，获取类句柄，单个回放或下载操作即在此类上进行
*   @param[in] stLoginInfo: 登录设备信息
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBDvrStream::Login(PB_LOGININFO& stLoginInfo)
{
	PB_DVRLOGININFO* pStDvrLoginInfo = &(stLoginInfo.unLoginType.stDvrLoginInfo);
	if (NULL == pStDvrLoginInfo)
	{
		return FALSE;
	}

	m_emPlayer = PT_HIK;
	m_emPlayType = stLoginInfo.emPlayType;

	PLAYBACKSDK_INFO("DvrStream-登录设备信息.ip=%s, port=%d, userName=%s, channel=%d", pStDvrLoginInfo->szIP, pStDvrLoginInfo->nPort, pStDvrLoginInfo->szUserName, m_lChannel);
	
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
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_Login_V30 falid, ErrCode:%ld,ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}
	m_lChannel += struDeviceInfo.struDeviceV30.byStartChan;
	return TRUE;
}

/** @fn Logout
*   @brief 登出设备,销毁句柄
*   @param[in] hLogin: 登录句柄
*   @param NULL
*   @return NULL
*/
void CPBDvrStream::Logout()
{
	if (m_lLogin != -1)
	{
		if (!NET_DVR_Logout(m_lLogin))
		{
            PLAYBACKSDK_ERROR("DvrStream-NET_DVR_Logout_V30 falid, LoginID=%ld, errCode=%0x,ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
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
BOOL CPBDvrStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

    //按事件查询
    if (stFindFileParam.bByEvent)
    {
        return FindFileByEvent(stFindFileParam, lpFileList, nFileCount);
    }

	NET_DVR_FILECOND_V40 stFileCond;
	stFileCond.dwFileType = (DWORD)stFindFileParam.emRecordType;
	stFileCond.dwIsLocked = 0xff;
	stFileCond.dwUseCardNo = stFindFileParam.dwUseCardNo;
	stFileCond.lChannel = m_lChannel;
	if (stFindFileParam.dwUseCardNo > 0)
	{
		memcpy_s(stFileCond.sCardNumber, sizeof(stFileCond.sCardNumber), stFindFileParam.sCardNumber, sizeof(stFindFileParam.sCardNumber));
	}
	else
	{
		memset(stFileCond.sCardNumber, 0, sizeof(stFileCond.sCardNumber));
	}
	memcpy_s(&(stFileCond.struStartTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStartTime), sizeof(PB_TIME));
	memcpy_s(&(stFileCond.struStopTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStopTime), sizeof(PB_TIME));

	PLAYBACKSDK_INFO("DvrStream-查询文件参数. channel=%d, recordType:%lu, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d, byCard:%lu, CardNum:%s",
		stFindFileParam.lChannel, stFileCond.dwFileType, stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
		stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond, stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
		stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond,
		stFindFileParam.dwUseCardNo, stFindFileParam.sCardNumber);

	LONG lFindHandle = NET_DVR_FindFile_V40(m_lLogin, &stFileCond);
	if (lFindHandle == -1)
	{
		DWORD dwErrCode = NET_DVR_GetLastError();
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_FindFile_V30 falid, m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
		if (NET_DVR_NOSUPPORT == dwErrCode  &&  stFindFileParam.dwUseCardNo > 0)
		{
			PB_SetLastError(PB_UNSUPPORT_CARDNUM);
		}
		return FALSE;
	}

	nFileCount = 0;
	BOOL bFind = TRUE;
	BOOL bReturn = TRUE;
	while (bFind)
	{
		NET_DVR_FINDDATA_V40 stFileData;
		LONG lFileHandle = NET_DVR_FindNextFile_V40(lFindHandle, &stFileData);
		switch (lFileHandle)
		{
		case NET_DVR_FILE_SUCCESS:  //获取文件成功
			{
				sprintf_s(lpFileList[nFileCount].szFileName, sizeof(lpFileList[nFileCount].szFileName), stFileData.sFileName);
				lpFileList[nFileCount].dwFileSize = stFileData.dwFileSize;
				memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(stFileData.struStartTime), sizeof(NET_DVR_TIME));
				memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(stFileData.struStopTime), sizeof(NET_DVR_TIME));
				// 文件类型转换.
				lpFileList[nFileCount].nFileType = ChangeFileType(stFileData.byFileType);				
				nFileCount++;
				if (nFileCount >= MAX_RECORD_COUNT)
				{
					PLAYBACKSDK_ERROR("DvrStream-查找到了%d个录像文件（最大缓冲)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //没有查到文件
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("DvrStream-没有查到文件");
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
                PLAYBACKSDK_ERROR("DvrStream-查找文件异常, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
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

	PLAYBACKSDK_INFO("DvrStream-查询文件成功,文件个数:%d. ", nFileCount);

	return bReturn;
}

/** @fn PlaybackTime
*   @brief 时间回放
*   @param[in] stPbTimeInfo: 时间回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBDvrStream::PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo)
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
		PLAYBACKSDK_ERROR("DvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("Hik---时间回放参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond, stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	if (!StartData(m_lLogin, stNetStartTime, stNetStopTime, stPbTimeInfo.stConvertParam.byResolution, stPbTimeInfo.stConvertParam.dwVideoBitrate, stPbTimeInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("DvrStream-StartData false，lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("DvrStream-StartStreamThread falid, lLogin=%ld", (LONG)m_lLogin);
		return FALSE;
	}
	return TRUE;
}

/** @fn PlaybackFile
*   @brief 文件回放
*   @param[in] stPbFileInfo: 文件回放参数
*   @param NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPBDvrStream::PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)
{
	m_stPlaybackTimeInfo.stStartTime = stPbFileInfo.stFileInfo.stStartTime;
	m_stPlaybackTimeInfo.stStopTime = stPbFileInfo.stFileInfo.stEndTime;
	m_stPlaybackTimeInfo.lChannel = stPbFileInfo.lChannel;
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
		PLAYBACKSDK_ERROR("DvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("DvrStream-时间回放参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbFileInfo.stFileInfo.stStartTime.dwYear, stPbFileInfo.stFileInfo.stStartTime.dwMonth, stPbFileInfo.stFileInfo.stStartTime.dwDay,
		stPbFileInfo.stFileInfo.stStartTime.dwHour, stPbFileInfo.stFileInfo.stStartTime.dwMinute, stPbFileInfo.stFileInfo.stStartTime.dwSecond, stPbFileInfo.stFileInfo.stEndTime.dwYear, stPbFileInfo.stFileInfo.stEndTime.dwMonth, stPbFileInfo.stFileInfo.stEndTime.dwDay,
		stPbFileInfo.stFileInfo.stEndTime.dwHour, stPbFileInfo.stFileInfo.stEndTime.dwMinute, stPbFileInfo.stFileInfo.stEndTime.dwSecond);

	//回放，窗口句柄设定为NULL
	if (!StartData(m_lLogin, stNetStartTime, stNetStopTime, stPbFileInfo.stConvertParam.byResolution, stPbFileInfo.stConvertParam.dwVideoBitrate, stPbFileInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("DvrStream-StartData false，lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("DvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	return TRUE;
}

/** @fn StopPlayback
*   @brief 停止回放
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBDvrStream::StopPlayback()
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

	CPlayStream::StopPlayback();
}

/** @fn PB_DownloadFile
*   @brief 文件下载
*   @param[in] stDownloadParam: 下载参数
*   @param NULL
*   @return 下载句柄，NULL表示下载失败
*/
BOOL CPBDvrStream::DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam)
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
		PLAYBACKSDK_ERROR("DvrStream-Create_File falid, 文件名: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;
	//取文件流
	PLAYBACKSDK_INFO("DvrStream-文件下载参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour, stDownloadParam.stFileInfo.stStartTime.dwMinute, stDownloadParam.stFileInfo.stStartTime.dwSecond, stDownloadParam.stFileInfo.stEndTime.dwYear, stDownloadParam.stFileInfo.stEndTime.dwMonth, stDownloadParam.stFileInfo.stEndTime.dwDay,
		stDownloadParam.stFileInfo.stEndTime.dwHour, stDownloadParam.stFileInfo.stEndTime.dwMinute, stDownloadParam.stFileInfo.stEndTime.dwSecond);

	if (!StartData(m_lLogin, stNetStartTime, stNetStopTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("DvrStream-StartData falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("DvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
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
BOOL CPBDvrStream::DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam)
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
		PLAYBACKSDK_ERROR("DvrStream-Create_File falid, 文件名: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nDownloadPos = 0;
	PLAYBACKSDK_INFO("DvrStream-时间下载参数. Login=%ld, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour, stDownloadParam.stStartTime.dwMinute, stDownloadParam.stStartTime.dwSecond, stDownloadParam.stStopTime.dwYear, stDownloadParam.stStopTime.dwMonth, stDownloadParam.stStopTime.dwDay,
		stDownloadParam.stStopTime.dwHour, stDownloadParam.stStopTime.dwMinute, stDownloadParam.stStopTime.dwSecond);
	if (!StartData(m_lLogin, stNetStartTime, stNetStopTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("DvrStream-StartData falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	//开启流检测线程
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("DvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
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
void CPBDvrStream::StopDownload()
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
	}
}

/**   @fn          StartData
 *    @brief   	   开始取流。
 *    @param[in]   lLogin:登陆ID.
 *    @param[in]   stNetStartTime:开始时间.
 *	  @param[in]   stNetStopTime:结束时间.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPBDvrStream::StartData(LONG lLogin, NET_DVR_TIME& stNetStartTime, NET_DVR_TIME& stNetStopTime, BYTE byResolution, DWORD dwVideoBitrate, DWORD dwVideoFrameRate)
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
	struVodPara.struIDInfo.dwChannel=m_lChannel; //通道号
	struVodPara.hWnd=NULL; //回放窗口
	struVodPara.struBeginTime = stNetStartTime;
	struVodPara.struEndTime = stNetStopTime;

	m_lPlayHandle = NET_DVR_PlayBackByTime_V40(m_lLogin, &struVodPara);
	if (m_lPlayHandle == -1)
	{
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackByTime falid，lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//设定回调
	NET_DVR_SetPlayDataCallBack(m_lPlayHandle, pDvrStream, PtrToUlong(this));

	//开始取流
	if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_PLAYSTART, 0, NULL))
	{
		NET_DVR_StopPlayBack(m_lPlayHandle);
		m_lPlayHandle = -1;
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackControl falid， ErrorCode = %ld, ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}
	//带宽控制：如果设置带宽小于实际需要，会以慢放处理
	if (m_stPlaybackTimeInfo.lBwControl != 0)
	{
		LONG lBwControl = (m_stPlaybackTimeInfo.lBwControl);  //单位一致，不做转换
		PLAYBACKSDK_INFO("DvrStream-NET_DVRPlayBackControl: BwControl: %d", lBwControl);
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
BOOL CPBDvrStream::StopData()
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
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackControl_V40 falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		bRet = FALSE;
	}
	//关闭回放
	if (bRet && !NET_DVR_StopPlayBack(m_lPlayHandle))
	{
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_StopPlayBack falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
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
int CPBDvrStream::ChangeFileType(int nFileType)
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
BOOL CPBDvrStream::IsCardNumAllowed()
{
	return TRUE;
}

/**  @fn          IsEventAllowed
*    @brief   	  是否支持按事件查询.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPBDvrStream::IsEventAllowed()
{
    return TRUE;
}

/**   @fn          GetCompressCfgAbility
*    @brief   	   获取压缩参数能力集.
*    @param[out]   stCompresscfgAbility:能力集结构体.
*    @return     
*/
BOOL CPBDvrStream::GetCompressCfgAbility( char* pOutbuf )
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
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_GetDeviceAbility falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
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
BOOL CPBDvrStream::SmartSearch(SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount)
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

	PLAYBACKSDK_INFO("DvrStream-智能检索参数. channel=%d, recordType:%d, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d",
		stSmartSearchParam.lChannel, stFileCond.bySearchCondType, stSmartSearchParam.stStartTime.dwYear, stSmartSearchParam.stStartTime.dwMonth
		, stSmartSearchParam.stStartTime.dwDay,stSmartSearchParam.stStartTime.dwHour, stSmartSearchParam.stStartTime.dwMinute
		, stSmartSearchParam.stStartTime.dwSecond, stSmartSearchParam.stStopTime.dwYear, stSmartSearchParam.stStopTime.dwMonth
		, stSmartSearchParam.stStopTime.dwDay,stSmartSearchParam.stStopTime.dwHour, stSmartSearchParam.stStopTime.dwMinute, stSmartSearchParam.stStopTime.dwSecond);

	LONG lFindHandle = NET_DVR_SmartSearch(m_lLogin, &stFileCond);
	if (lFindHandle == -1)
	{
		DWORD dwErrCode = NET_DVR_GetLastError();
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_SmartSearch失败. m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
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
					PLAYBACKSDK_ERROR("DvrStream-查找到了%d个录像文件（最大缓冲)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //没有查到文件
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("DvrStream-没有查到文件");
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
                PLAYBACKSDK_ERROR("DvrStream-查找文件异常, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
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

	PLAYBACKSDK_INFO("DvrStream-智能检索成功,文件个数:%d. ", nFileCount);

	return bReturn;
}

BOOL CPBDvrStream::GetVideoFormat( int& nVideoFormat )
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
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_GetDVRConfig. lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
	}
	else
	{
		nVideoFormat = struDvrPicCfg.dwVideoFormat;
		bRet = TRUE;
	}
	return bRet;
}

BOOL CPBDvrStream::FindFileByEvent( FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount )
{
    if (-1 == m_lLogin)
    {
        return FALSE;
    }

    NET_DVR_SEARCH_EVENT_PARAM stFileCond;
    stFileCond.wMajorType = (WORD)EVENT_VCA_DETECTION;
    stFileCond.wMinorType = (WORD)stFindFileParam.emEventType;
    stFileCond.byLockType = 0xff;
    stFileCond.byValue = 0;

    memcpy_s(&(stFileCond.struStartTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStartTime), sizeof(PB_TIME));
    memcpy_s(&(stFileCond.struEndTime), sizeof(NET_DVR_TIME), &(stFindFileParam.stStopTime), sizeof(PB_TIME));

    PLAYBACKSDK_INFO("DvrStream-按事件查询文件参数. channel=%d, recordType:%lu, 时间:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d,",
        stFindFileParam.lChannel, stFindFileParam.emEventType, stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
        stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond, stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
        stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);

    ZeroMemory(stFileCond.uSeniorParam.struVCADetectByBit.byChan, 256);
    stFileCond.uSeniorParam.struVCADetectByBit.byChan[stFindFileParam.lChannel] = 1;

    LONG lFindHandle = NET_DVR_FindFileByEvent(m_lLogin, &stFileCond);
    if (lFindHandle == -1)
    {
        DWORD dwErrCode = NET_DVR_GetLastError();
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_FindFileByEvent 失败. m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
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
                //lpFileList[nFileCount].dwFileSize = stFileData.dwFileSize;
                memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(stFileData.struStartTime), sizeof(NET_DVR_TIME));
                memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(stFileData.struEndTime), sizeof(NET_DVR_TIME));
                // 文件类型转换.
                lpFileList[nFileCount].nFileType = ChangeFileType(stFileData.wMajorType*10+stFileData.wMinorType);				
                nFileCount++;
                if (nFileCount >= MAX_RECORD_COUNT)
                {
                    PLAYBACKSDK_ERROR("DvrStream-查找到了%d个录像文件（最大缓冲)", MAX_RECORD_COUNT);
                    nFileCount = MAX_RECORD_COUNT;
                    bFind = FALSE;
                }
            }
            break;
        case NET_DVR_FILE_NOFIND:   //没有查到文件
            {
                nFileCount = 0;
                bFind = FALSE;
                PLAYBACKSDK_INFO("DvrStream-没有查到文件");
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
                PLAYBACKSDK_ERROR("DvrStream-查找文件异常, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
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

    PLAYBACKSDK_INFO("DvrStream-按事件查询文件成功,文件个数:%d. ", nFileCount);

    return bReturn;
}
