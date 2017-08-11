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
	// ������.
	int nDataType = -1;

	switch (datatype)
	{
	case 2:			//������
		{
			nDataType = PB_STREAM_DATA;
		}
		break;
	case 1:         //����ͷ
		{
			nDataType = PB_STREAM_HEADER;
		}
		break;

	default:        //����
		nDataType = PB_STREAM_END;
		return 0;
	}

	if (pInstance->m_bDownload)
	{
		pInstance->pFunDownload((HANDLE)sessionhandle, nDataType, (BYTE*)pdata, ilen);
	}

	//KMS�޷��طţ��ص������Ƿ����40�ֽ�ͷ(û��ʵ������)������¼��������
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
*   @brief ��ʼ��
*   @param[in] 
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
*   @brief �ͷ�sdk
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
*   @brief ��¼�豸����ȡ�����������طŻ����ز������ڴ����Ͻ���
*   @param[in] stLoginInfo: ��¼�豸��Ϣ
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBKmsStream::Login(PB_LOGININFO& stLoginInfo)
{
	return TRUE;
}

/** @fn Logout
*   @brief �ǳ��豸,���پ��
*   @param[in] hLogin: ��¼���
*   @param NULL
*   @return NULL
*/
void CPBKmsStream::Logout()
{
}

/** @fn FindFile
*   @brief �����ļ�
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBKmsStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	return TRUE;
}

/** @fn PlaybackTime
*   @brief ʱ��ط�
*   @param[in] stPbTimeInfo: ʱ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	//�������ص�
	StreamClient_SetMsgCallBack(m_lLogin, pKmsMsg,this);
	StreamClient_SetDataCallBack(m_lLogin, pKmsStream, this);
	//ʱ��ط�
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

	//����������߳�
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
*   @brief �ļ��ط�
*   @param[in] stPbFileInfo: �ļ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBKmsStream::PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)
{
	//login
	m_lLogin = StreamClient_CreateSession();
	//�������ص�
	StreamClient_SetMsgCallBack(m_lLogin, pKmsMsg,this);
	StreamClient_SetDataCallBack(m_lLogin, pKmsStream, this);
	//ʱ��ط�

	m_hPlayHwnd = stPbFileInfo.hWnd;
	if (!GPAPI_Startup(PT_HIK,NULL,NULL,NULL))
	{
		PLAYBACKSDK_ERROR("KmsStream-GPAPI_Startup falid.");
		return FALSE;
	}
	//�û�����
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

	//����������߳�
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
*   @brief ֹͣ�ط�
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
	// ֹͣȡ��.
	StopData();
	// ֹͣ����
	StopPlay();
	// �ر��߳�
	StopStreamThread(TRUE);
	
	PLAYBACKSDK_INFO("KmsStream-ֹͣ�ط�. lLogin=%ld", m_lLogin);

	CPlayStream::StopPlayback();
}

/** @fn PB_DownloadFile
*   @brief �ļ�����
*   @param[in] stDownloadParam: ���ز���
*   @param NULL
*   @return ���ؾ����NULL��ʾ����ʧ��
*/
BOOL CPBKmsStream::DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam)
{
	//login
	m_lLogin = StreamClient_CreateSession();
	//�������ص�
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

	//������
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

	//����������߳�
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
*   @brief ʱ������
*   @param[in] stDownloadParam: ������Ϣ
*   @param NULL
*   @return ���ؾ����NULL��ʾ����ʧ��
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
    //�������ص�
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

    //������
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

    //����������߳�
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
*   @brief ֹͣ����
*   @param[in] hDownload: ���ؾ��
*   @param NULL
*   @return NULL
*/
void CPBKmsStream::StopDownload()
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

		PLAYBACKSDK_INFO("KmsStream-ֹͣ����. lLogin=%ld", m_lLogin);
	}
}

/**   @fn          StopData
 *    @brief   	   ֹͣȡ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   	   nFileType:�ļ�����ת��.
 *    @param[in]   nFileType:�ļ�����.
 *    @param[in]   
 *    @return      �µ��ļ�����.
 */
int CPBKmsStream::ChangeFileType(int nFileType)
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

BOOL CPBKmsStream::PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut)
{
	if (m_lLogin < 0)
	{
		PLAYBACKSDK_ERROR("KmsStream-û�лط�");
		return FALSE;
	}
	if (m_lPort == -1)
	{
		PLAYBACKSDK_ERROR("KmsStream-û�н���");
		return FALSE;
	}

	PLAYBACKSDK_INFO("KmsStream-���ſ���. emControlType=%d", emControlType);
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

			// �ı������ٶ�
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
				PLAYBACKSDK_ERROR("KmsStream-��������ʧ��. lLogin=%d, speed=%d", m_lLogin, nSpeed);
				return FALSE;
			}

			if (!GPAPI_SetPlaySpeed(m_emPlayer, m_lPort, (PLAYSPEED)emControlType, m_hPlayHwnd))
			{
				PLAYBACKSDK_INFO("KmsStream-���ڲ����ٶ�ʧ��. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_emPlaySpeed = emControlType;

		}
		break;
	case PB_CONTROL_TYPE_PAUSE:
		{
			if (StreamClient_Pause(m_lLogin) < 0)
			{
				PLAYBACKSDK_ERROR("KmsStream-��ͣ��ʧ��. lLogin=%d", m_lLogin);
				return FALSE;
			}

			if (!GPAPI_Pause(m_emPlayer, m_lPort, TRUE))
			{
				PLAYBACKSDK_INFO("KmsStream-������ͣʧ��. m_lPort=%ld,", m_lPort);
				return FALSE;
			}
			m_bPause = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_RESTART:
		{
			if (StreamClient_Resume(m_lLogin) < 0)
			{
				PLAYBACKSDK_ERROR("KmsStream-�ָ���ʧ��. lLogin=%d", m_lLogin);
				return FALSE;
			}

			if (!GPAPI_Pause(m_emPlayer, m_lPort, FALSE))
			{
				PLAYBACKSDK_INFO("KmsStream-�ָ�ʧ��. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bPause = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_OPEN_AUDIO:
		{
			if (!GPAPI_PlaySound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("KmsStream-������Ƶʧ��. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bOpenSound = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_CLOSE_AUDIO:
		{
			if (!GPAPI_StopSound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("KmsStream-�ر���Ƶʧ��. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bOpenSound = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_SET_VOLUME:
		{
			if (!GPAPI_SetVolume(m_emPlayer, m_lPort, (WORD)lValueIn))
			{
				PLAYBACKSDK_INFO("KmsStream-��������ʧ��. Player=%d, m_lPort=%ld, speed=%d, volumn=%d", m_emPlayer, m_lPort, (int)emControlType, (int)lValueIn);
				return FALSE;
			}
		}
		break;
	case PB_CONTROL_TYPE_SINGLE_FRAME:
		{
			if (!GPAPI_PlayOneByOne(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("KmsStream-��֡����ʧ��. Player=%d, m_lPort=%ld LastError = %ld info = %s", m_emPlayer, m_lPort, GPAPI_GetLastError(m_emPlayer, m_lPort), GPAPI_GetLastErrorString(m_emPlayer, m_lPort));
				return FALSE;
			}
		}
		break;
	case PB_CONTROL_TYPE_SINGLE_FRAME_RESTORE:
		{
			if (!GPAPI_PlayOneByOneRestore(m_emPlayer, m_lPort, m_hPlayHwnd))
			{
				PLAYBACKSDK_INFO("KmsStream-ֹͣ��֡����ʧ��. Player=%d, m_lPort=%ld", m_emPlayer, m_lPort);
				return FALSE;
			}
		}
		break;
	default:
		PLAYBACKSDK_ERROR("KmsStream-�ò�����֧�� %d", (int)emControlType);
		return FALSE;
	}

	return TRUE;
}
