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
#include "PBDvrStream.h"

/** @fn pStreamPbLayer
*   @brief ȡ���ص�����
*   @param[in] hPlayData: �طž��
*   @param[in] hUserData���û�����
*   @param[in] nDataType: ������
*   @param[in] pBuffer:   ������
*   @param[in] dwBufferSize: ���ݴ�С
*   @return NULL
*/
void CALLBACK pDvrStream(LONG lPlayHandle, DWORD dwDataType, BYTE * pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	CPBDvrStream * pInstance = (CPBDvrStream*)ULongToPtr(dwUser);
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
*   @brief ��ʼ��
*   @param[in] 
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	
	PLAYBACKSDK_INFO("DvrStream-��ʼ���ɹ�");

	return TRUE;
}

/** @fn ReleaseHikSdk
*   @brief �ͷ�sdk
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
*   @brief ��¼�豸����ȡ�����������طŻ����ز������ڴ����Ͻ���
*   @param[in] stLoginInfo: ��¼�豸��Ϣ
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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

	PLAYBACKSDK_INFO("DvrStream-��¼�豸��Ϣ.ip=%s, port=%d, userName=%s, channel=%d", pStDvrLoginInfo->szIP, pStDvrLoginInfo->nPort, pStDvrLoginInfo->szUserName, m_lChannel);
	
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
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_Login_V30 falid, ErrCode:%ld,ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}
	m_lChannel += struDeviceInfo.struDeviceV30.byStartChan;
	return TRUE;
}

/** @fn Logout
*   @brief �ǳ��豸,���پ��
*   @param[in] hLogin: ��¼���
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
*   @brief �����ļ�
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBDvrStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (-1 == m_lLogin)
	{
		return FALSE;
	}

    //���¼���ѯ
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

	PLAYBACKSDK_INFO("DvrStream-��ѯ�ļ�����. channel=%d, recordType:%lu, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d, byCard:%lu, CardNum:%s",
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
		case NET_DVR_FILE_SUCCESS:  //��ȡ�ļ��ɹ�
			{
				sprintf_s(lpFileList[nFileCount].szFileName, sizeof(lpFileList[nFileCount].szFileName), stFileData.sFileName);
				lpFileList[nFileCount].dwFileSize = stFileData.dwFileSize;
				memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(stFileData.struStartTime), sizeof(NET_DVR_TIME));
				memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(stFileData.struStopTime), sizeof(NET_DVR_TIME));
				// �ļ�����ת��.
				lpFileList[nFileCount].nFileType = ChangeFileType(stFileData.byFileType);				
				nFileCount++;
				if (nFileCount >= MAX_RECORD_COUNT)
				{
					PLAYBACKSDK_ERROR("DvrStream-���ҵ���%d��¼���ļ�����󻺳�)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //û�в鵽�ļ�
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("DvrStream-û�в鵽�ļ�");
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
                PLAYBACKSDK_ERROR("DvrStream-�����ļ��쳣, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
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

	PLAYBACKSDK_INFO("DvrStream-��ѯ�ļ��ɹ�,�ļ�����:%d. ", nFileCount);

	return bReturn;
}

/** @fn PlaybackTime
*   @brief ʱ��ط�
*   @param[in] stPbTimeInfo: ʱ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	m_pDrawFunc = (VIDEODRAWFUNC)stPbTimeInfo.pDrawFunc;  //���滭ͼ����
	m_nDrawUserData = stPbTimeInfo.nUser;     //�û�����

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("DvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("Hik---ʱ��طŲ���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond, stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	if (!StartData(m_lLogin, stNetStartTime, stNetStopTime, stPbTimeInfo.stConvertParam.byResolution, stPbTimeInfo.stConvertParam.dwVideoBitrate, stPbTimeInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("DvrStream-StartData false��lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//����������߳�
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
*   @brief �ļ��ط�
*   @param[in] stPbFileInfo: �ļ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	m_pDrawFunc = (VIDEODRAWFUNC)stPbFileInfo.pDrawFunc;  //���滭ͼ����
	m_nDrawUserData = stPbFileInfo.nUser;     //�û�����

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("DvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("DvrStream-ʱ��طŲ���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbFileInfo.stFileInfo.stStartTime.dwYear, stPbFileInfo.stFileInfo.stStartTime.dwMonth, stPbFileInfo.stFileInfo.stStartTime.dwDay,
		stPbFileInfo.stFileInfo.stStartTime.dwHour, stPbFileInfo.stFileInfo.stStartTime.dwMinute, stPbFileInfo.stFileInfo.stStartTime.dwSecond, stPbFileInfo.stFileInfo.stEndTime.dwYear, stPbFileInfo.stFileInfo.stEndTime.dwMonth, stPbFileInfo.stFileInfo.stEndTime.dwDay,
		stPbFileInfo.stFileInfo.stEndTime.dwHour, stPbFileInfo.stFileInfo.stEndTime.dwMinute, stPbFileInfo.stFileInfo.stEndTime.dwSecond);

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_lLogin, stNetStartTime, stNetStopTime, stPbFileInfo.stConvertParam.byResolution, stPbFileInfo.stConvertParam.dwVideoBitrate, stPbFileInfo.stConvertParam.dwVideoFrameRate))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        PLAYBACKSDK_ERROR("DvrStream-StartData false��lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//����������߳�
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
*   @brief ֹͣ�ط�
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

	// ֹͣȡ��.
	StopData();
	// ֹͣ����
	StopPlay();
	// �ر��߳�
	StopStreamThread(TRUE);

	CPlayStream::StopPlayback();
}

/** @fn PB_DownloadFile
*   @brief �ļ�����
*   @param[in] stDownloadParam: ���ز���
*   @param NULL
*   @return ���ؾ����NULL��ʾ����ʧ��
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
		PLAYBACKSDK_ERROR("DvrStream-Create_File falid, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;
	//ȡ�ļ���
	PLAYBACKSDK_INFO("DvrStream-�ļ����ز���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
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

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("DvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
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
		PLAYBACKSDK_ERROR("DvrStream-Create_File falid, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nDownloadPos = 0;
	PLAYBACKSDK_INFO("DvrStream-ʱ�����ز���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
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

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("DvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
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
void CPBDvrStream::StopDownload()
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
	}
}

/**   @fn          StartData
 *    @brief   	   ��ʼȡ����
 *    @param[in]   lLogin:��½ID.
 *    @param[in]   stNetStartTime:��ʼʱ��.
 *	  @param[in]   stNetStopTime:����ʱ��.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBDvrStream::StartData(LONG lLogin, NET_DVR_TIME& stNetStartTime, NET_DVR_TIME& stNetStopTime, BYTE byResolution, DWORD dwVideoBitrate, DWORD dwVideoFrameRate)
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
	struVodPara.struIDInfo.dwChannel=m_lChannel; //ͨ����
	struVodPara.hWnd=NULL; //�طŴ���
	struVodPara.struBeginTime = stNetStartTime;
	struVodPara.struEndTime = stNetStopTime;

	m_lPlayHandle = NET_DVR_PlayBackByTime_V40(m_lLogin, &struVodPara);
	if (m_lPlayHandle == -1)
	{
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackByTime falid��lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}

	//�趨�ص�
	NET_DVR_SetPlayDataCallBack(m_lPlayHandle, pDvrStream, PtrToUlong(this));

	//��ʼȡ��
	if (!NET_DVR_PlayBackControl_V40(m_lPlayHandle, NET_DVR_PLAYSTART, 0, NULL))
	{
		NET_DVR_StopPlayBack(m_lPlayHandle);
		m_lPlayHandle = -1;
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackControl falid�� ErrorCode = %ld, ErrMsg:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		return FALSE;
	}
	//������ƣ�������ô���С��ʵ����Ҫ���������Ŵ���
	if (m_stPlaybackTimeInfo.lBwControl != 0)
	{
		LONG lBwControl = (m_stPlaybackTimeInfo.lBwControl);  //��λһ�£�����ת��
		PLAYBACKSDK_INFO("DvrStream-NET_DVRPlayBackControl: BwControl: %d", lBwControl);
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
BOOL CPBDvrStream::StopData()
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
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_PlayBackControl_V40 falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
		bRet = FALSE;
	}
	//�رջط�
	if (bRet && !NET_DVR_StopPlayBack(m_lPlayHandle))
	{
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_StopPlayBack falid, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
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
int CPBDvrStream::ChangeFileType(int nFileType)
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
BOOL CPBDvrStream::IsCardNumAllowed()
{
	return TRUE;
}

/**  @fn          IsEventAllowed
*    @brief   	  �Ƿ�֧�ְ��¼���ѯ.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPBDvrStream::IsEventAllowed()
{
    return TRUE;
}

/**   @fn          GetCompressCfgAbility
*    @brief   	   ��ȡѹ������������.
*    @param[out]   stCompresscfgAbility:�������ṹ��.
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
*   @brief ���ܼ���
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
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

	PLAYBACKSDK_INFO("DvrStream-���ܼ�������. channel=%d, recordType:%d, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d",
		stSmartSearchParam.lChannel, stFileCond.bySearchCondType, stSmartSearchParam.stStartTime.dwYear, stSmartSearchParam.stStartTime.dwMonth
		, stSmartSearchParam.stStartTime.dwDay,stSmartSearchParam.stStartTime.dwHour, stSmartSearchParam.stStartTime.dwMinute
		, stSmartSearchParam.stStartTime.dwSecond, stSmartSearchParam.stStopTime.dwYear, stSmartSearchParam.stStopTime.dwMonth
		, stSmartSearchParam.stStopTime.dwDay,stSmartSearchParam.stStopTime.dwHour, stSmartSearchParam.stStopTime.dwMinute, stSmartSearchParam.stStopTime.dwSecond);

	LONG lFindHandle = NET_DVR_SmartSearch(m_lLogin, &stFileCond);
	if (lFindHandle == -1)
	{
		DWORD dwErrCode = NET_DVR_GetLastError();
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_SmartSearchʧ��. m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
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
					PLAYBACKSDK_ERROR("DvrStream-���ҵ���%d��¼���ļ�����󻺳�)", MAX_RECORD_COUNT);
					nFileCount = MAX_RECORD_COUNT;
					bFind = FALSE;
				}
			}
			break;
		case NET_DVR_FILE_NOFIND:   //û�в鵽�ļ�
			{
				nFileCount = 0;
				bFind = FALSE;
				PLAYBACKSDK_INFO("DvrStream-û�в鵽�ļ�");
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
                PLAYBACKSDK_ERROR("DvrStream-�����ļ��쳣, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
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

	PLAYBACKSDK_INFO("DvrStream-���ܼ����ɹ�,�ļ�����:%d. ", nFileCount);

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

    PLAYBACKSDK_INFO("DvrStream-���¼���ѯ�ļ�����. channel=%d, recordType:%lu, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d,",
        stFindFileParam.lChannel, stFindFileParam.emEventType, stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
        stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond, stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
        stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);

    ZeroMemory(stFileCond.uSeniorParam.struVCADetectByBit.byChan, 256);
    stFileCond.uSeniorParam.struVCADetectByBit.byChan[stFindFileParam.lChannel] = 1;

    LONG lFindHandle = NET_DVR_FindFileByEvent(m_lLogin, &stFileCond);
    if (lFindHandle == -1)
    {
        DWORD dwErrCode = NET_DVR_GetLastError();
        PLAYBACKSDK_ERROR("DvrStream-NET_DVR_FindFileByEvent ʧ��. m_lLogin:%ld, errCode=%lu, ErrMsg:%s", m_lLogin, dwErrCode, NET_DVR_GetErrorMsg());
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
                //lpFileList[nFileCount].dwFileSize = stFileData.dwFileSize;
                memcpy_s(&(lpFileList[nFileCount].stStartTime), sizeof(PB_TIME), &(stFileData.struStartTime), sizeof(NET_DVR_TIME));
                memcpy_s(&(lpFileList[nFileCount].stEndTime), sizeof(PB_TIME), &(stFileData.struEndTime), sizeof(NET_DVR_TIME));
                // �ļ�����ת��.
                lpFileList[nFileCount].nFileType = ChangeFileType(stFileData.wMajorType*10+stFileData.wMinorType);				
                nFileCount++;
                if (nFileCount >= MAX_RECORD_COUNT)
                {
                    PLAYBACKSDK_ERROR("DvrStream-���ҵ���%d��¼���ļ�����󻺳�)", MAX_RECORD_COUNT);
                    nFileCount = MAX_RECORD_COUNT;
                    bFind = FALSE;
                }
            }
            break;
        case NET_DVR_FILE_NOFIND:   //û�в鵽�ļ�
            {
                nFileCount = 0;
                bFind = FALSE;
                PLAYBACKSDK_INFO("DvrStream-û�в鵽�ļ�");
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
                PLAYBACKSDK_ERROR("DvrStream-�����ļ��쳣, lLogin:%ld, errCode=%ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
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

    PLAYBACKSDK_INFO("DvrStream-���¼���ѯ�ļ��ɹ�,�ļ�����:%d. ", nFileCount);

    return bReturn;
}
