/**   @file HikStream.cpp
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief �����豸������
 *
 *    @author   zhoupeiqiang
 *    @date     2012/11/13
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#include "StdAfx.h"
#include "PBCvrStream.h"

/** @fn pStreamPbLayer
*   @brief ȡ���ص�����
*   @param[in] hPlayData: �طž��
*   @param[in] hUserData���û�����
*   @param[in] nDataType: ������
*   @param[in] pBuffer:   ������
*   @param[in] dwBufferSize: ���ݴ�С
*   @return NULL
*/
void CALLBACK pCvrStream(LONG lPlayHandle, DWORD dwDataType, BYTE * pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	CPBCvrStream * pInstance = (CPBCvrStream*)ULongToPtr(dwUser);
	if (pInstance == NULL)
	{
		return;
	}

	// ������.
	int nDataType = -1;

	switch (dwDataType)
	{
	case NET_DVR_STREAMDATA:
	case NET_DVR_AUDIOSTREAMDATA:		//��Ƶ������
	case NET_DVR_STD_VIDEODATA:			//��׼��Ƶ������
	case NET_DVR_STD_AUDIODATA:			//��׼��Ƶ������
	case NET_DVR_CHANGE_FORWARD:		//�����ı�Ϊ����  
	case NET_DVR_CHANGE_REVERSE:		//�����ı�Ϊ����
	case NET_DVR_PRIVATE_DATA:			//˽������,����������Ϣ
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
*   @brief ��ʼ��
*   @param[in] 
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	
	PLAYBACKSDK_INFO("��ʼ���ɹ�");

	return TRUE;
}

/** @fn ReleaseHikSdk
*   @brief �ͷ�sdk
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
	PLAYBACKSDK_INFO("CvrStream-�ͷ�Sdk�ɹ�");
}


/** @fn Login
*   @brief ��¼�豸����ȡ�����������طŻ����ز������ڴ����Ͻ���
*   @param[in] stLoginInfo: ��¼�豸��Ϣ
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	strcpy((char *)struLoginInfo.sDeviceAddress,pStDvrLoginInfo->szIP);  //�豸IP ��ַ
	strcpy((char *)struLoginInfo.sUserName,pStDvrLoginInfo->szUserName); //�豸��¼�û���
	strcpy((char *)struLoginInfo.sPassword,pStDvrLoginInfo->szPassword); //�豸��¼����
	struLoginInfo.wPort = pStDvrLoginInfo->nPort;                        //�豸��¼�˿�
	struLoginInfo.bUseAsynLogin = 0;                                     //ͬ����¼����¼�ӿڷ��سɹ�����¼�ɹ�

	m_lLogin = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
	if (-1 == m_lLogin)
	{
		PLAYBACKSDK_ERROR("CvrStream-NET_DVR_Login_V40 falid, error = %d", NET_DVR_GetLastError());
		return FALSE;
	}
	return TRUE;
}

/** @fn Logout
*   @brief �ǳ��豸,���پ��
*   @param[in] hLogin: ��¼���
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
*   @brief �����ļ�
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBCvrStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

    //���¼���ѯ
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
*   @brief ʱ��ط�
*   @param[in] stPbTimeInfo: ʱ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	m_pDrawFunc = (VIDEODRAWFUNC)stPbTimeInfo.pDrawFunc;  //���滭ͼ����
	m_nDrawUserData = stPbTimeInfo.nUser;     //�û�����

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("CvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-ʱ��طŲ���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond, stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_lLogin, stPbTimeInfo.szStreamID, stNetStartTime, stNetStopTime, stPbTimeInfo.bConvertPlay
		, stPbTimeInfo.stConvertParam.byResolution, stPbTimeInfo.stConvertParam.dwVideoBitrate, stPbTimeInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("CvrStream-StartData false��lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//����������߳�
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread, lLogin=%ld", (LONG)m_lLogin);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-ʱ��طųɹ�. lLogin=%ld", (LONG)m_lLogin);

	return TRUE;
}

/** @fn PlaybackFile
*   @brief �ļ��ط�
*   @param[in] stPbFileInfo: �ļ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	m_pDrawFunc = (VIDEODRAWFUNC)stPbFileInfo.pDrawFunc;  //���滭ͼ����
	m_nDrawUserData = stPbFileInfo.nUser;     //�û�����

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("CvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-ʱ��طŲ���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbFileInfo.stFileInfo.stStartTime.dwYear, stPbFileInfo.stFileInfo.stStartTime.dwMonth, stPbFileInfo.stFileInfo.stStartTime.dwDay,
		stPbFileInfo.stFileInfo.stStartTime.dwHour, stPbFileInfo.stFileInfo.stStartTime.dwMinute, stPbFileInfo.stFileInfo.stStartTime.dwSecond, stPbFileInfo.stFileInfo.stEndTime.dwYear, stPbFileInfo.stFileInfo.stEndTime.dwMonth, stPbFileInfo.stFileInfo.stEndTime.dwDay,
		stPbFileInfo.stFileInfo.stEndTime.dwHour, stPbFileInfo.stFileInfo.stEndTime.dwMinute, stPbFileInfo.stFileInfo.stEndTime.dwSecond);

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_lLogin, stPbFileInfo.szStreamID, stNetStartTime, stNetStopTime, stPbFileInfo.bConvertPlay
		, stPbFileInfo.stConvertParam.byResolution, stPbFileInfo.stConvertParam.dwVideoBitrate, stPbFileInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("CvrStream-StartData false��lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//����������߳�
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CvrStream-ʱ��طųɹ�. lLogin=%ld", m_lLogin);

	return TRUE;
}

/** @fn StopPlayback
*   @brief ֹͣ�ط�
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

	// ֹͣȡ��.
	StopData();

	//ֹͣ����
	StopPlay();
	
	//�ر��߳�
	StopStreamThread(TRUE);
	
	PLAYBACKSDK_INFO("CvrStream-ֹͣ�ط�. lLogin=%ld", m_lLogin);

	CPlayStream::StopPlayback();
}

/** @fn PB_DownloadFile
*   @brief �ļ�����
*   @param[in] stDownloadParam: ���ز���
*   @param NULL
*   @return ���ؾ����NULL��ʾ����ʧ��
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
		PLAYBACKSDK_ERROR("CvrStream-Create_File falid, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;
	//ȡ�ļ���
	PLAYBACKSDK_INFO("CvrStream-�ļ����ز���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
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

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	//��ʼȡ��ʱ��
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/** @fn PB_DownloadTime
*   @brief ʱ������
*   @param[in] stDownloadParam: ������Ϣ
*   @param NULL
*   @return ���ؾ����NULL��ʾ����ʧ��
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
		PLAYBACKSDK_ERROR("CvrStream-Create_File falid, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nDownloadPos = 0;
	PLAYBACKSDK_INFO("CvrStream-ʱ�����ز���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
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

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
		return FALSE;
	}

	//��ʼȡ��ʱ��
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/** @fn PB_StopDownload
*   @brief ֹͣ����
*   @param[in] hDownload: ���ؾ��
*   @param NULL
*   @return NULL
*/
void CPBCvrStream::StopDownload()
{
	if (m_bDownload)
	{
		//ͣ��
		StopData();
		//�رռ���߳�
		StopStreamThread(FALSE);
		m_bDownload = FALSE;
		if (m_hDownloadFile)
		{
			CloseHandle(m_hDownloadFile);
			m_hDownloadFile = FALSE;
		}

		PLAYBACKSDK_INFO("CvrStream-ֹͣ����. lLogin=%ld", m_lLogin);
	}
}

/**   @fn          StartData
 *    @brief   	   ��ʼȡ����
 *    @param[in]   lLogin:��½ID.
 *    @param[in]   stNetStartTime:��ʼʱ��.
 *	  @param[in]   stNetStopTime:����ʱ��.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBCvrStream::StartData(LONG lLogin, const char* pStreamID, NET_DVR_TIME& stNetStartTime, NET_DVR_TIME& stNetStopTime
		, BOOL bConvertPlay, BYTE byResolution, DWORD dwVideoBitrate, DWORD dwVideoFrameRate)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

	//����ڻط��У�ͣ��
	StopData();

	m_bBlock = TRUE;

	//�طţ����ھ���趨ΪNULL
	NET_DVR_VOD_PARA struVodPara={0};
	struVodPara.dwSize=sizeof(struVodPara);
	strncpy((char*)struVodPara.struIDInfo.byID, pStreamID, STREAM_ID_LEN);
    struVodPara.struIDInfo.byID[STREAM_ID_LEN-1] = NULL;
	struVodPara.hWnd=NULL; //�طŴ���
	struVodPara.struBeginTime = stNetStartTime;
	struVodPara.struEndTime = stNetStopTime;
	m_lPlayHandle = NET_DVR_PlayBackByTime_V40(m_lLogin, &struVodPara);
	if (-1 == m_lPlayHandle)
	{
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_PlayBackByTime_V40 falid��lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//�趨�ص�
	NET_DVR_SetPlayDataCallBack(m_lPlayHandle, pCvrStream, PtrToUlong(this));

	//��ʼȡ��
	if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_PLAYSTART, 0, NULL))
	{
		NET_DVR_StopPlayBack(m_lPlayHandle);
		m_lPlayHandle = -1;
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_PlayBackControl_V40 falid�� ErrorCode = %ld, ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}
	//������ƣ�ĿǰCVR�豸����֧�ִ�����ƣ����Ե��ýӿ���Ȼ�ɹ�������������Ч
	if (m_stPlaybackTimeInfo.lBwControl != 0)
	{
		LONG lBwControl = (m_stPlaybackTimeInfo.lBwControl);
		if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_SETSPEED, &lBwControl, NULL))
		{
			NET_DVR_StopPlayBack(m_lPlayHandle);
			m_lPlayHandle = -1;
			PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackControl falid�� ErrorCode = %ld, ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
			return FALSE;
		}
	}

	return TRUE;
}

/**   @fn          StopData
 *    @brief   	   ֹͣȡ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBCvrStream::StopData()
{
	if (-1 == m_lPlayHandle)
	{
		return TRUE;
	}

	// ֹͣ���ݻص�����.
	m_bBlock = FALSE;

	//ֹͣȡ��
	BOOL bRet = TRUE;
	
	if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_PLAYSTOP))
	{
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_PlayBackControl_V40 falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		bRet = FALSE;
	}
	//�رջط�
	if (bRet && !NET_DVR_StopPlayBack(m_lPlayHandle))
	{
        PLAYBACKSDK_ERROR("CvrStream-NET_DVR_StopPlayBack falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		bRet = FALSE;
	}

	m_lPlayHandle = -1;

	return bRet;
}

/**   @fn          ChangeFileType
 *    @brief   	   nFileType:�ļ�����ת��.
 *    @param[in]   nFileType:�ļ�����.
 *    @param[in]   
 *    @return      �µ��ļ�����.
 */
int CPBCvrStream::ChangeFileType(int nFileType)
{
	switch (nFileType)
	{
		// ��ʱ¼��.
	case 0:
		return RECORD_TYPE_TIMING;

		// �ƶ����.
	case 1:
		return RECORD_TYPE_MOVE;

		// ��������.
	case 2:
		return RECORD_TYPE_ALARM_TRIGER;

		// ����|�ƶ����.
	case 3:
		return RECORD_TYPE_ALARM_OR_MOVE;

		// ����&�ƶ����.
	case 4:
		return RECORD_TYPE_ALARM_AND_MOVE;
		
		// �����.
	case 5:
		return RECORD_TYPE_CMD_TRIGER;

		// �ֶ�¼��.
	case 6:
		return RECORD_TYPE_MANUAL_RECORD;

		 //Խ�����
	case 41:
		return RECORD_TYPE_VCA_TRAVERSE_PLANE;

		 //�����������
	case 42:
		return RECORD_TYPE_FIELD_DETECTION;

		//��Ƶ�����쳣
	case 43:
		return RECORD_TYPE_AUDIO_INPUT_ALARM;

		//��ǿͻ�����
	case 44:
		return RECORD_TYPE_SOUND_INTENSITY_ALARM;

		//�������
	case 45:
		return RECORD_TYPE_FACE_DETECTION;

		//�齹���
	case 46:
		return RECORD_TYPE_VIRTUAL_FOCUS_ALARM;

		//����������
	case 47:
		return RECORD_TYPE_SCENE_CHANGE_ALARM;

	default:
		return RECORD_TYPE_ALL;
	}
}

/**  @fn          IsCardNumAllowed
*    @brief   	  �Ƿ�֧�ְ����Ų�ѯ.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPBCvrStream::IsCardNumAllowed()
{
	return TRUE;
}

/**  @fn          IsEventAllowed
*    @brief   	  �Ƿ�֧�ְ��¼���ѯ.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPBCvrStream::IsEventAllowed()
{
    return TRUE;
}

/**   @fn          GetCompressCfgAbility
*    @brief   	   ��ȡѹ������������.
*    @param[out]   stCompresscfgAbility:�������ṹ��.
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
*   @brief ���ܼ���
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
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

	PLAYBACKSDK_INFO("CvrStream-���ܼ�������. channel=%d, recordType:%d, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d",
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
		case NET_DVR_FILE_SUCCESS:  //��ȡ�ļ��ɹ�
			{
				CString strFileName;
				//�ļ���
				strFileName.Format("Hik_SmartSearch_%d", nFileCount);
				sprintf_s(lpFileList[nFileCount].szFileName, sizeof(lpFileList[nFileCount].szFileName), strFileName.GetBuffer());
				lpFileList[nFileCount].dwFileSize = 0;
				memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(stFileData.struStartTime), sizeof(NET_DVR_TIME));
				memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(stFileData.struEndTime), sizeof(NET_DVR_TIME));
				// �ļ�����ת��.
				lpFileList[nFileCount].nFileType = -1;				
				nFileCount++;
				if (nFileCount >= MAX_RECORD_COUNT)
				{
					PLAYBACKSDK_ERROR("CvrStream-���ҵ���%d��¼���ļ�����󻺳�)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //û�в鵽�ļ�
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("CvrStream-û�в鵽�ļ�");
			}
			break;
		case NET_DVR_ISFINDING:     //���ڲ���
			{
				Sleep(5);
			}
			break;
		case NET_DVR_NOMOREFILE:    //���ҽ�����
			{
				bFind = FALSE;
			}
			break;
		case NET_DVR_FILE_EXCEPTION:     //�����ļ��쳣
			{
				bFind = FALSE;
				bReturn = FALSE;
                PLAYBACKSDK_ERROR("CvrStream-�����ļ��쳣, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
			}
			break;
		default:                    //����ʧ��
			{
				bFind = FALSE;
				bReturn = FALSE;
			}
			break;
		}
	}

	//�رղ���
	NET_DVR_StopSearch(lFindHandle);

	PLAYBACKSDK_INFO("CvrStream-���ܼ����ɹ�,�ļ�����:%d. ", nFileCount);

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
	struFileCond.lChannel = stFindFileParam.lChannel; //ͨ����
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
        case NET_DVR_FILE_SUCCESS:  //��ȡ�ļ��ɹ�
            {
                sprintf_s(lpFileList[nFileCount].szFileName, sizeof(lpFileList[nFileCount].szFileName), struFileData.sFileName);
                lpFileList[nFileCount].dwFileSize = struFileData.dwFileSize;
                memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(struFileData.struStartTime), sizeof(NET_DVR_TIME));
                memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(struFileData.struStopTime), sizeof(NET_DVR_TIME));
                // �ļ�����ת��.
                lpFileList[nFileCount].nFileType = struFileData.byFileType;			
                nFileCount++;
                if (nFileCount >= MAX_RECORD_COUNT)
                {
                    PLAYBACKSDK_ERROR("CvrStream-���ҵ���%d��¼���ļ�����󻺳�)", MAX_RECORD_COUNT);
                    nFileCount = MAX_RECORD_COUNT;
                    bFind = FALSE;
                }
            }
            break;
        case NET_DVR_FILE_NOFIND:   //û�в鵽�ļ�
            {
                nFileCount = 0;
                bFind = FALSE;
                PLAYBACKSDK_INFO("CvrStream-û�в鵽�ļ�");
            }
            break;
        case NET_DVR_ISFINDING:     //���ڲ���
            {
                Sleep(5);
            }
            break;
        case NET_DVR_NOMOREFILE:    //���ҽ�����
            {
                bFind = FALSE;
            }
            break;
        case NET_DVR_FILE_EXCEPTION:     //�����ļ��쳣
            {
                bFind = FALSE;
                bReturn = FALSE;
                PLAYBACKSDK_ERROR("CvrStream-�����ļ��쳣, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
            }
            break;
        default:                    //����ʧ��
            {
                bFind = FALSE;
                bReturn = FALSE;
            }
            break;
        }
    }

    //�رղ���
    NET_DVR_FindClose_V30(lFindHandle);

    PLAYBACKSDK_INFO("CvrStream-���¼���ѯ�ļ��ɹ�,�ļ�����:%d. ", nFileCount);

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
    stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID[STREAM_ID_LEN - 1] = NULL;  //��ֹû�н�β
	stFileCond.uSeniorParam.struStreamIDParam.byBackupVolumeNum = 0;

	LONG lFindHandle = NET_DVR_FindFileByEvent(m_lLogin, &stFileCond);
	if (-1 == lFindHandle)
	{
		SetLastError(NET_DVR_GetLastError());
		DWORD dwErrCode = NET_DVR_GetLastError();
		PLAYBACKSDK_ERROR("Hik---NET_DVR_FindFileByEventʧ��. m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
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
		case NET_DVR_FILE_SUCCESS:  //��ȡ�ļ��ɹ�
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
					PLAYBACKSDK_ERROR("Hik---���ҵ���%d��¼���ļ�����󻺳�)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //û�в鵽�ļ�
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("Hik---NET_DVR_FILE_NOFIND.");
				SetLastError(NET_DVR_FILE_NOFIND);
			}
			break;
		case NET_DVR_ISFINDING:     //���ڲ���
			{
				Sleep(5);
			}
			break;
		case NET_DVR_NOMOREFILE:    //���ҽ�����
			{
				bFind = FALSE;
			}
			break;
		case NET_DVR_FILE_EXCEPTION:     //�����ļ��쳣
			{
				SetLastError(NET_DVR_GetLastError());
				bFind = FALSE;
				bReturn = FALSE;
				PLAYBACKSDK_ERROR("Hik---NET_DVR_FILE_EXCEPTION. lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
			}
			break;
		default:                    //����ʧ��
			{
				bFind = FALSE;
				bReturn = FALSE;
				SetLastError(NET_DVR_GetLastError());
			}
			break;
		}
	}

	//�رղ���
	NET_DVR_FindClose_V30(lFindHandle);

	PLAYBACKSDK_INFO("Hik---���¼���ѯ�ļ��ɹ�,�ļ�����:%d. ", nFileCount);

	return bReturn;
}
