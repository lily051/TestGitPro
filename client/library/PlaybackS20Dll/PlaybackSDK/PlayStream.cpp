#include "stdafx.h"
#include "PlayStream.h"

/**   @fn          OnSaveData
 *    @brief   	   ��������.
 *    @param[in]   nDataType:��������.
 *    @param[in]   pBuffer:����ָ��.
 *    @param[in]   dwBufSize:���ݴ�С.
 *    @param[in]   dwUser:�û��Զ�������.
 *    @return      
 */
int CALLBACK OnSaveData(int nDataType, BYTE* pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	HANDLE hFile = (HANDLE)UlongToHandle(dwUser);
	if (NULL == hFile)
	{
		return 0;
	}

	DWORD dwWrited = 0;
	PLAYBACKSDK_DEBUG("OnSaveData, nDataType:%d, dwBufSize:%lu", nDataType, dwBufSize);
	if (!WriteFile(hFile, pBuffer, dwBufSize, &dwWrited, NULL))
	{
		PLAYBACKSDK_ERROR("PlayStream---��������ʧ��. Len=%d", dwBufSize);
	}

	return 0;
}

/**   @fn          OnStreamCB
 *    @brief   	   ȡ���ص�.
 *    @param[in]   nDataType:��������.
 *    @param[in]   pBuffer:����ָ��.
 *    @param[in]   dwBufSize:���ݴ�С.
 *    @param[in]   dwUser:�û��Զ�������.
 *    @return      
 */
int CALLBACK OnStreamCB(int nDataType, BYTE* pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	CPlayStream* pStream = (CPlayStream*)ULongToPtr(dwUser);
	if (NULL == pStream)
	{
		return 0;
	}

    CGuard guard(&pStream->m_csStreamCB);
	PbStreamDataCB pfnStreamCB = pStream->GetStreamDataCB();
	DWORD dwUserData = pStream->GetStreamUserData();
	
	// ���ϲ��׻ص�������.
	if (pfnStreamCB != NULL)
	{
		PLAYBACKSDK_DEBUG("Stream callback, nDataType:%d, dwBufSize:%lu", nDataType, dwBufSize);
		pfnStreamCB(nDataType, pBuffer, dwBufSize, dwUserData);
	}

	return 0;
}

/**   @fn          SetHeadData
 *    @brief   	   ����ͷ���ݻ���.
 *    @param[in]   pBute:����ָ��.
 *    @param[in]   dwBufSize:���ݴ�С.
 *    @return      
 */
void CPlayStream::SetHeadData(PBYTE pBuffer, DWORD dwBufSize)
{
	memcpy_s(m_byteHeadBuf, sizeof(m_byteHeadBuf), pBuffer, dwBufSize);
	m_nHeadLen = dwBufSize > 1024 ? 1024:dwBufSize;
}

/**   @fn          GetHeadData
 *    @brief   	   ��ȡͷ����.
 *    @param[in]   PbStreamDataCB:���ݻص�ָ��.
 *    @param[in]   dwUserData:�û��Զ�������.
 *    @return      
 */
void CPlayStream::GetHeadData(PbStreamDataCB pfnStreamData, DWORD dwUserData)
{
	pfnStreamData(PB_STREAM_HEADER, m_byteHeadBuf, m_nHeadLen, dwUserData);
}

/**   @fn          SetBufferData
 *    @brief   	   ���û���������.
 *    @param[in]   pBute:����ָ��.
 *    @param[in]   dwBufSize:���ݴ�С.
 *    @return      
 */
void CPlayStream::SetBufferData(PBYTE pBuffer, DWORD dwBufSize)
{
	// ���浽����������.
	CGuard guard(&m_csSaveBufLock);

	if (dwBufSize < MAX_SAVEBUF_SIZE)
	{
		// ������λ������ռ��㹻�洢�������������ݣ�����ȫ���洢.
		LONG lLen = (LONG)(MAX_SAVEBUF_SIZE - m_dwCurBufPointer - dwBufSize);
		if (lLen >= 0)
		{
			memcpy_s(&m_byteSaveBuf[m_dwCurBufPointer], dwBufSize, pBuffer, dwBufSize);
			m_dwCurBufPointer += dwBufSize;
			if (m_dwCurBufPointer >= MAX_SAVEBUF_SIZE)
			{
				m_bIsReWrite = TRUE;
				m_dwCurBufPointer = 0;
			}
		}
		else
		{
			memcpy_s(&m_byteSaveBuf[m_dwCurBufPointer], MAX_SAVEBUF_SIZE - m_dwCurBufPointer, pBuffer, MAX_SAVEBUF_SIZE - m_dwCurBufPointer);
			memcpy_s(&m_byteSaveBuf[0], dwBufSize - (MAX_SAVEBUF_SIZE - m_dwCurBufPointer), 
				pBuffer + MAX_SAVEBUF_SIZE - m_dwCurBufPointer, dwBufSize - (MAX_SAVEBUF_SIZE - m_dwCurBufPointer));
			m_dwCurBufPointer = dwBufSize - (MAX_SAVEBUF_SIZE - m_dwCurBufPointer);
			m_bIsReWrite = TRUE;
		}
	}
	else	// �����ݲ���Ȼ�����������û��ѭ��д�봦��.
	{
		memcpy_s(m_byteSaveBuf, MAX_SAVEBUF_SIZE, pBuffer, dwBufSize);
		m_dwCurBufPointer = 0;
	}
}

/**   @fn          GetBufferData
 *    @brief   	   ��ȡ����������.
 *    @param[in]   pfnStreamData:���ݻص�����.
 *    @param[in]   dwUserData:�û��Զ�������.
 *    @return      
 */
void CPlayStream::GetBufferData(PbStreamDataCB pfnStreamData, DWORD dwUserData)
{
	// �������������е����ݱ��浽�ļ���.
	CGuard guard(&m_csSaveBufLock);

	// ��ȡ���ſ⻺�����е�ʣ�����ݴ�С.
	DWORD dwRemain = GPAPI_GetSourceBufferRemain(m_emPlayer, m_lPort);
	dwRemain += MAX_PRE_SAVEBUF_SIZE;
	if(dwRemain > MAX_SAVEBUF_SIZE)
	{
		TRACE("@z full write");
		dwRemain = MAX_SAVEBUF_SIZE;
	}

	// û�д�ͷ��ʼд���ݣ��򱣴�ȫ������.
	if (!m_bIsReWrite)
	{
		pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[0], m_dwCurBufPointer, dwUserData);
	}
	// ��ͷ��ʼд���������ȱ��滺�����������ݣ��ٱ���ǰ��Σ���������ʱ������ǰ���.
	else
	{
		// ������ſ���δ��������ȫ��������ǰ��������У���ֻ����ǰ�������.
		if (dwRemain <= m_dwCurBufPointer)
		{
			pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[m_dwCurBufPointer - dwRemain], dwRemain, dwUserData);
		}
		// ������ſ���δ���������ڻ����ǰ��κͺ��ζ��б��棬���ȱ��滺�����������ݣ��ٱ���ǰ���.
		else
		{
			DWORD dwLen = MAX_SAVEBUF_SIZE - (dwRemain - m_dwCurBufPointer);
			// �����������.
			pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[dwLen], dwRemain - m_dwCurBufPointer, dwUserData);

			// ����ǰ�������.
			pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[0], m_dwCurBufPointer, dwUserData);
		}
	}
}

/** @fn pFunPlayback
*   @brief ȡ���ص�����
*   @param[in] hPlayData: �طž��
*   @param[in] nDataType: ������
*   @param[in] pBuffer:   ������
*   @param[in] dwBufferSize: ���ݴ�С
*   @return NULL
*/
BOOL CPlayStream::pFunPlayback(HANDLE hPlayData, int nDataType, BYTE * pBuffer, DWORD dwBufSize)
{
	BOOL bRet = FALSE;

	switch (nDataType)
	{
	case PB_STREAM_HEADER:
		{
			PLAYBACKSDK_INFO("PlayStream---pFunPlayback, StartPlay STREAM_HEADER.");
            if (m_bPlaying)
            {
                goto STREAM;
            }
			if (-1 == GPAPI_StartPlay(m_emPlayer, m_lPort, m_hPlayHwnd, pBuffer, dwBufSize, "", m_pDrawFunc, m_nDrawUserData, DEFAULT_PLAY_BUFFER, SM_FILE))
			{
                m_nPlaybackPos = 200;
				PLAYBACKSDK_ERROR("PlayStream---funDataback!GPAPI_StartPlayʧ��. Player=%d, port=%ld, PlayHwnd=%ld, bufSize=%ld", m_emPlayer, m_lPort, (LONG)m_hPlayHwnd, dwBufSize);
			}
			else
			{
                m_bPlaying = TRUE;
				//������Ƶ
				if (!GPAPI_PlaySound(m_emPlayer,m_lPort))
				{
					PLAYBACKSDK_ERROR("PlayStream---GPAPI_PlaySound, m_lPort:%ld", m_lPort);
				}
				m_bOpenSound = TRUE;
				// ����ǵ���.
				if (m_bReversePlay)
				{
					if (!GPAPI_ReversePlay(m_emPlayer, m_lPort))
					{
						PLAYBACKSDK_ERROR("PlayStream---GPAPI_ReversePlay, m_lPort:%ld", m_lPort);
					}
				}
				if (m_bOpenSound)
				{
					PLAYBACKSDK_INFO("PlayStream---pFunPlayback, OpenSound.");
					LONG lValueOut = 0;
					if (!PlaybackControl(PB_CONTROL_TYPE_OPEN_AUDIO, 0, lValueOut))
					{
						m_bOpenSound = FALSE;
					}
				}
				if (m_emPlaySpeed != PB_CONTROL_TYPE_NOREMAL)
				{
					PLAYBACKSDK_INFO("PlayStream---pFunPlayback, SetSpeed.");
					LONG lValueOut = 0;
					if (!PlaybackControl((ENUM_CONTROL_TYPE)m_emPlaySpeed, 0, lValueOut))
					{
						m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
					}
				}
			}
			SetHeadData(pBuffer, dwBufSize);
		}
		break;
	case PB_STREAM_DATA:
		{
STREAM:
            if (dwBufSize > 0)
			{
				SetBufferData(pBuffer, dwBufSize);
				while (!bRet)
				{
					bRet = GPAPI_InputData(m_emPlayer, m_lPort, pBuffer, dwBufSize);
					if (bRet)
					{
						break;
					}
					if (!m_bBlock)
					{
						break;
					}
					Sleep(10);
			    }

				// ��������.
				if (m_bSaveFile)
				{
					DWORD dwWrited;
					WriteFile(m_hSaveFile, pBuffer, dwBufSize, &dwWrited, NULL);
				}
			}
		}
		break;
	default:
		return bRet;
	}

	// ���ϲ��׻ص�������.
	if (m_bStreamCB)
	{
		if (!m_bGetHead)
		{
			if (m_byteHeadBuf[0] != '\0')
			{
				m_bGetHead = TRUE;
				GetHeadData(OnStreamCB, PtrToUlong(this));
                //���������
                GPAPI_ResetSourceBuffer(m_emPlayer, m_lPort);
                // ��ȡ������.
                //GetBufferData(OnStreamCB, PtrToUlong(this));
			}	
		}

		PushStreamData(nDataType, pBuffer, dwBufSize);
	}
 
	//���һ��ȡ��ʱ��
	m_lPlaybackLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return bRet;
}

/** @fn pFunDownload
*   @brief ȡ���ص�����
*   @param[in] hPlayData: �طž��
*   @param[in] nDataType: ������
*   @param[in] pBuffer:   ������
*   @param[in] dwBufferSize: ���ݴ�С
*   @return NULL
*/
void CPlayStream::pFunDownload(HANDLE hPlayData, int nDataType, BYTE * pBuffer, int dwBufferSize)
{
	if (m_bDownload && m_hDownloadFile)
	{
		PLAYBACKSDK_INFO("pFunDownload:dwBufferSize = %d, m_dwCurDownSize = %d, m_dwTotleSize = %d, m_nDownloadPos= %d", 
			dwBufferSize, 
			m_dwCurDownSize,
			m_dwTotleSize,
			m_nDownloadPos);
		DWORD dwRet;
		WriteFile(m_hDownloadFile, pBuffer, dwBufferSize, &dwRet, NULL);
		m_dwCurDownSize += dwBufferSize;
		if (m_dwTotleSize > 0)
		{
			m_nDownloadPos = int(((float)m_dwCurDownSize / (float)m_dwTotleSize)*100.0);
			if (m_nDownloadPos < 0 || m_nDownloadPos > 100)
			{
				CString strMsg;
				strMsg.Format("PlayStream---���ؽ���.pos=%d, bufferSize=%ld, curDownsize=%ld, totleSize=%d", m_nDownloadPos, dwBufferSize, m_dwCurDownSize, m_dwTotleSize);
				OutputDebugString(strMsg);
				m_nDownloadPos = 100;
			}
		}
		//���һ��ȡ��ʱ��
		m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();
	}
}

/** @fn PlaybackStreamDetect
*   @brief �ط�������߳�
*   @param[in] lpvoid: �̲߳���
*   @param NULL
*   @return NULL
*/
void CPlayStream::PlaybackStreamDetect(LPVOID lpvoid)
{
	OutputDebugString("@z PlaybackStreamDetect");
	CPlayStream * pInstance = (CPlayStream*)lpvoid;
	if (pInstance == NULL)
	{
		return;
	}

	while (pInstance->m_bPlaybackThread)
	{
		//��ͣ�ط�ʱ��������ͣ��ʱ��
		if (pInstance->m_bPause)
		{
			pInstance->m_lPlaybackLastTime = (LONG)CTime::GetCurrentTime().GetTime();
			for (int i=0; i<10; i++)
			{
				if (!pInstance->m_bPlaybackThread)
				{
					return;
				}
				Sleep(100);
			}
			continue;
		}
		//PLAYBACK_DETECT_TIMEʱ����û����������ʶ�طŽ���
		LONG lCurTime = (LONG)CTime::GetCurrentTime().GetTime();
		DWORD dwRemain = GPAPI_GetSourceBufferRemain(pInstance->m_emPlayer, pInstance->m_lPort);
		if (pInstance->m_lPlaybackLastTime != 0  
			&&  pInstance->m_lPlaybackLastTime + PLAYBACK_DETECT_TIME < lCurTime
			&&  (0 == dwRemain || DEFAULT_PLAY_BUFFER < dwRemain)  
			/*&&  0 == dwBufValue*/)
		{
			pInstance->m_nPlaybackPos = 100;
			PLAYBACKSDK_INFO("PlayStream---%d��ʱ����û��������ֹͣ�ط�ȡ��", PLAYBACK_DETECT_TIME);
		}

		for (int i=0; i<10; i++)
		{
			if (!pInstance->m_bPlaybackThread)
			{
				return;
			}
			Sleep(100);
		}
	}

}

/** @fn DownloadStreamDetect
*   @brief ����������߳�
*   @param[in] lpvoid: �̲߳���
*   @param NULL
*   @return NULL
*/
void CPlayStream::DownloadStreamDetect(LPVOID lpvoid)
{
	CPlayStream * pInstance = (CPlayStream*)lpvoid;
	if (pInstance == NULL)
	{
		return;
	}
	while (pInstance->m_bDownloadThread)
	{
		//DOWNLOAD_DETECT_TIMEʱ����û����������ʶ�طŽ���
		LONG lCurTime = (LONG)CTime::GetCurrentTime().GetTime();
		if (pInstance->m_lDownloadLastTime != 0 && pInstance->m_lDownloadLastTime + DOWNLOAD_DETECT_TIME < lCurTime)
		{
			pInstance->m_nDownloadPos = 100;
			PLAYBACKSDK_INFO("PlayStream---%d��ʱ����û��������ֹͣ����ȡ��", PLAYBACK_DETECT_TIME);
		}

		Sleep(1000);
	}
}

CPlayStream::CPlayStream()
{
	m_lPort = -1;
	m_emPlayer = PT_HIK;
	m_hPlayHwnd = NULL;
	ZeroMemory(m_byteHeadBuf, 1024);
	m_nHeadLen = 1024;
	m_hSaveFile = NULL;
	m_bSaveFile = FALSE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_bDownload = FALSE;
	m_hDownloadFile = NULL;
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nPlaybackPos = 0;
	m_nDownloadPos = 0;
	m_lPlaybackLastTime = 0;
	m_lDownloadLastTime = 0;
	m_bPause = FALSE;
	m_bPlaybackThread = FALSE;
	m_hPlaybackThread = NULL;
	m_bDownloadThread = FALSE;
	m_hDownloadThread = NULL;
	m_bStopPlay = FALSE;
	m_bBlock = TRUE;
	m_bIsReWrite = FALSE;
	ZeroMemory(m_byteSaveBuf, MAX_SAVEBUF_SIZE);
	m_dwCurBufPointer = 0;
	m_bOpenSound = FALSE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_bReversePlay = FALSE;
	m_pfnStreamCB = NULL;
	m_pDrawFunc = NULL;
	m_nDrawUserData = 0;
	m_dwStreamUserData = 0;
	m_bStreamCB = FALSE;
	m_bGetHead = FALSE;
    m_bPlaying = FALSE;
	InitializeCriticalSection(&m_csSaveBufLock);
    InitializeCriticalSection(&m_csStreamCB);
	m_dwBuffSize = 0;
}

CPlayStream::~CPlayStream()
{
	if (m_hSaveFile)
	{
		CloseHandle(m_hSaveFile);
		m_hSaveFile = NULL;
	}
	if (m_hDownloadFile)
	{
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
	}

	DeleteCriticalSection(&m_csSaveBufLock);
    DeleteCriticalSection(&m_csStreamCB);
}

/** @fn LocatePlayTime
*   @brief ��λ����ʱ��
*   @param[in]  stStartTime: ��λʱ��
*   @param[in]  stStopTime:ֹͣʱ��.
*   @param[out] NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPlayStream::LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime)
{
	//�ж�ʱ��
    if (!IsPBTimeCorrect(stStartTime) 
        || !IsPBTimeCorrect(stStopTime))
    {
        PLAYBACKSDK_ERROR("PlayStream---IsPBTimeCorrect error");
        return FALSE;
    }
	CTime tPlayTime(stStartTime.dwYear, stStartTime.dwMonth, stStartTime.dwDay, stStartTime.dwHour, stStartTime.dwMinute, stStartTime.dwSecond);
	CTime tStopTime(stStopTime.dwYear, stStopTime.dwMonth, stStopTime.dwDay, 
		stStopTime.dwHour, stStopTime.dwMinute, stStopTime.dwSecond);
	if (tPlayTime.GetTime() > tStopTime.GetTime())
	{
		PLAYBACKSDK_ERROR("PlayStream---��λʱ����ڵ�ǰ�طŽ���ʱ��. tPlayTime=%ld, tStopTime=%ld", (LONG)tPlayTime.GetTime(), (LONG)tStopTime.GetTime());
		return FALSE;
	}

	PLAYBACKSDK_INFO("PlayStream---��λʱ��, starTime:%s, stopTime:%s", 
		(LPCTSTR)tPlayTime.Format("%Y-%m-%d %H:%M:%S"), (LPCTSTR)tStopTime.Format("%Y-%m-%d %H:%M:%S"));

	// ��¼�Ƿ������Ƶ.
	BOOL bOpenSound = m_bOpenSound;
	// ��¼ԭ���Ļط��ٶ�.
	ENUM_CONTROL_TYPE emSpeed = m_emPlaySpeed;

	StopPlayback();

	m_bOpenSound = bOpenSound;
	m_emPlaySpeed = emSpeed;
	m_stPlaybackTimeInfo.stStartTime = stStartTime;
	m_stPlaybackTimeInfo.stStopTime = stStopTime;

	if (!PlaybackTime(m_stPlaybackTimeInfo))
	{
		m_bOpenSound = FALSE;
		m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
		return FALSE;
	}

	return TRUE;
}

/** @fn StopPlay
*   @brief ֹͣ����
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPlayStream::StopPlay()
{
	if (m_lPort != -1)
	{
		GPAPI_StopPlay(m_emPlayer, m_lPort);
		//���������
		GPAPI_ResetSourceBuffer(m_emPlayer, m_lPort);
		//�ͷŶ˿�
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        m_bPlaying = FALSE;
	}
}

/** @fn PlaybackControl
*   @brief �طſ���
*   @param[in] emControlType: ��������
*   @param[in] lValueIn: �������
*   @param[out] lValueOut: �������
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPlayStream::PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut)
{
	if (m_lPort == -1)
	{
		return FALSE;
	}

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
			TRACE("@z PlaySpeed:%d", emControlType);
			if (m_bReversePlay)
			{
				if (!GPAPI_SetPlayReverseSpeed(m_emPlayer, m_lPort, (PLAYSPEED)emControlType, m_hPlayHwnd))
				{
					PLAYBACKSDK_INFO("PlayStream---Reverse ���ڲ����ٶ�ʧ��. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
					return FALSE;
				}
			}
			else
			{
				if (!GPAPI_SetPlaySpeed(m_emPlayer, m_lPort, (PLAYSPEED)emControlType, m_hPlayHwnd))
				{
                    PLAYBACKSDK_INFO("PlayStream---���ڲ����ٶ�ʧ��. Player=%d, m_lPort=%ld, speed=%d,error:%d", 
                        m_emPlayer, m_lPort, (int)emControlType, GPAPI_GetLastError(m_emPlayer, m_lPort));
					return FALSE;
				}
			}
			
			m_emPlaySpeed = emControlType;
		}
		break;
	case PB_CONTROL_TYPE_PAUSE:
		{
			if (!GPAPI_Pause(m_emPlayer, m_lPort, TRUE))
			{
				PLAYBACKSDK_INFO("PlayStream---��ͣʧ��. Player=%d, m_lPort=%ld, speed=%d, error = %s", m_emPlayer, m_lPort, (int)emControlType, GPAPI_GetLastErrorString(m_emPlayer, m_lPort));
				return FALSE;
			}
			m_bPause = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_RESTART:
		{
			if (!GPAPI_Pause(m_emPlayer, m_lPort, FALSE))
			{
				PLAYBACKSDK_INFO("PlayStream---�ָ�ʧ��. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bPause = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_OPEN_AUDIO:
		{
			if (!GPAPI_PlaySound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("PlayStream---������Ƶʧ��. Player=%d, m_lPort=%ld, speed=%d, error=%d", m_emPlayer, m_lPort, (int)emControlType, GPAPI_GetLastError(m_emPlayer, m_lPort));
				return FALSE;
			}
			m_bOpenSound = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_CLOSE_AUDIO:
		{
			if (!GPAPI_StopSound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("PlayStream---�ر���Ƶʧ��. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bOpenSound = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_SET_VOLUME:
		{
			if (!GPAPI_SetVolume(m_emPlayer, m_lPort, (WORD)lValueIn))
			{
				PLAYBACKSDK_INFO("PlayStream---��������ʧ��. Player=%d, m_lPort=%ld, speed=%d, volumn=%d", m_emPlayer, m_lPort, (int)emControlType, (int)lValueIn);
				return FALSE;
			}
		}
		break;

	case PB_CONTROL_TYPE_SINGLE_FRAME:
		{
			if (!GPAPI_PlayOneByOne(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("PlayStream---��֡����ʧ��. Player=%d, m_lPort=%ld", m_emPlayer, m_lPort);
				return FALSE;
			}
		}
		break;

	case PB_CONTROL_TYPE_SINGLE_FRAME_RESTORE:
		{
			if (!GPAPI_PlayOneByOneRestore(m_emPlayer, m_lPort, m_hPlayHwnd))
			{
				PLAYBACKSDK_INFO("PlayStream---ֹͣ��֡����ʧ��. Player=%d, m_lPort=%ld", m_emPlayer, m_lPort);
				return FALSE;
			}
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

/** @fn GetOsdTime
*   @brief ��ȡosdʱ��
*   @param[in]  NULL
*   @param[out] lOsdTime: osdʱ�䣬32time_t��ֵ
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPlayStream::GetOsdTime(LONG & lOsdTime)
{
	if (m_lPort == -1)
	{
		return FALSE;
	}

	DWORD wTime = GPAPI_GetSpecialData(m_emPlayer, m_lPort);
	if (wTime == 0)
	{
		PB_SetLastError(PB_UNSUPPORT_OPERATION);
		return FALSE;
	}
	else
	{
		int nYear  = OSDTIME_GET_YEAR(wTime);
		int nMonth = OSDTIME_GET_MONTH(wTime);
		int nDay   = OSDTIME_GET_DAY(wTime);
		int nHour  = OSDTIME_GET_HOUR(wTime);
		int nMin   = OSDTIME_GET_MIN(wTime);
		int nSec   = OSDTIME_GET_SEC(wTime);
		if ( nYear >= 1900 && nMonth >= 1 && nMonth <= 12 && nDay >= 1 && nDay <= 31 && nHour >= 0 && nHour <= 23 &&
			nMin >= 0 && nMin <= 59 &&nSec >= 0 && nSec <= 59)//У��ʱ����Ƿ��ǺϷ�ʱ��
		{
			CTime cPlayTime;
			cPlayTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
			lOsdTime = (long)cPlayTime.GetTime();
		}
		else
		{
			PB_SetLastError(PB_UNSUPPORT_OPERATION);
			return FALSE;
		}
	}

	return TRUE;
}

/** @fn GetPlayPos
*   @brief ��ȡ�طŽ���
*   @param[in]  NULL
*   @param[out] lPos: ����
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPlayStream::GetPlayPos(LONG & lPos)
{
	if (m_lPort == -1)
	{
		return FALSE;
	}

	lPos = m_nPlaybackPos;

	return TRUE;
}

/** @fn PlaybackCapture
*   @brief �ط�ץͼ
*   @param[in] szPicFile: ͼƬ·��
*   @param[in] emPicType: ͼƬ����
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPlayStream::PlaybackCapture(char * szPicFile, ENUM_PIC_TYPE emPicType)
{
	if (m_lPort == -1)
	{
		return FALSE;
	}

	PICTUREFORMAT picType = PF_BMP;
	if (emPicType == PIC_TYPE_JPG)
	{
		picType = PF_JPEG;
	}

	PLAYBACKSDK_INFO("PlayStream---ץͼ. Player=%d, picPath=%s, picType=%d", (int)m_emPlayer, szPicFile, picType);

	return GPAPI_Capture(m_emPlayer, m_lPort, szPicFile, picType);
}

/** @fn PB_SaveData
*   @brief ¼�����
*   @param[in] szFile: �ļ��洢��ַ
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPlayStream::SaveData(char * szSaveFile)
{
	if (m_lPort == -1)
	{
		return FALSE;
	}

	if (m_bSaveFile)
	{
		return FALSE;
	}

	if (m_hSaveFile)
	{
		CloseHandle(m_hSaveFile);
		m_hSaveFile = NULL;
	}

	m_strSaveFilePath.Format("%s", szSaveFile);
	m_hSaveFile = Create_File(szSaveFile);
	if (m_hSaveFile == NULL)
	{
		PLAYBACKSDK_ERROR("PlayStream---�������������ļ�ʧ��. file: %s", szSaveFile);
		return FALSE;
	}

	// ��ȡͷ����.
	GetHeadData(OnSaveData, HandleToULong(m_hSaveFile));
    CGuard guard(&m_csSaveBufLock);   //��m_bSaveFile���ʷ�ֹ��ͻ
	// ��ȡ������.
	GetBufferData(OnSaveData, HandleToULong(m_hSaveFile));
    
	m_bSaveFile = TRUE;
	return TRUE;
}

/** @fn PB_StopSave
*   @brief ֹͣ����
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPlayStream::StopSave()
{
    CGuard guard(&m_csSaveBufLock);
	m_bSaveFile = FALSE;
	if (m_hSaveFile)
	{
		CloseHandle(m_hSaveFile);
		m_hSaveFile = NULL;
	}
}

/** @fn PB_GetDownloadPos
*   @brief ��ȡ���ؽ���
*   @param[out] nPos: ����
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPlayStream::GetDownloadPos(int & nPos)
{
    if (m_bDownload)
	{
		nPos = m_nDownloadPos;
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}

/** @fn StartStreamThread
*   @brief �����߳�
*   @param[in] bPlayback: TRUE�ط��߳� FALSE�����߳�
*   @param NULL
*   @return BOOL�ɹ� FALSEʧ��
*/
BOOL CPlayStream::StartStreamThread(BOOL bPlayback)
{
	OutputDebugString("@z StartStreamThread");
	// �����ȡ��ʱ�丳��ʼֵ.
	m_lPlaybackLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	if (bPlayback)
	{
		m_bPlaybackThread = TRUE;
		DWORD dwRet;
		m_hPlaybackThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlaybackStreamDetect, (LPVOID)this, 0, &dwRet);
		if (m_hPlaybackThread == NULL)
		{
			m_bPlaybackThread = FALSE;
			return FALSE;
		}
	}
	else
	{
		m_bDownloadThread = TRUE;
		DWORD dwRet;
		m_hDownloadThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DownloadStreamDetect, (LPVOID)this, 0, &dwRet);
		if (m_hDownloadThread == NULL)
		{
			m_bDownloadThread = FALSE;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPlayStream::StartCheckPlayStream(BOOL bPlayback)
{

	if (bPlayback)
	{
		m_bPlaybackThread = TRUE;
		DWORD dwRet;
		m_hPlaybackThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFuncCheckPlayStream, (LPVOID)this, 0, &dwRet);
		if (m_hPlaybackThread == NULL)
		{
			m_bPlaybackThread = FALSE;
			return FALSE;
		}
	}

	return TRUE;
}

/** @fn StopStreamThread
*   @brief ֹͣ������߳�
*   @param[in] bPlayback: TRUE�ط��߳� FALSE�����߳�
*   @param NULL
*   @return NULL
*/
void CPlayStream::StopStreamThread(BOOL bPlayback)
{
	if (bPlayback)
	{
		if (m_hPlaybackThread)
		{
			m_bPlaybackThread = FALSE;
			WaitForSingleObject(m_hPlaybackThread, INFINITE);
			CloseHandle(m_hPlaybackThread);
			m_hPlaybackThread = NULL;
		}
	}
	else
	{
		if (m_hDownloadThread)
		{
			m_bDownloadThread = FALSE;
			WaitForSingleObject(m_hDownloadThread, INFINITE);
			CloseHandle(m_hDownloadThread);
			m_hDownloadThread = NULL;
		}
	}
}

void CPlayStream::StopCheckPlayStream(BOOL bPlayback)
{
	if (bPlayback)
	{
		if (m_hPlaybackThread)
		{
			m_bPlaybackThread = FALSE;
			WaitForSingleObject(m_hPlaybackThread, INFINITE);
			CloseHandle(m_hPlaybackThread);
			m_hPlaybackThread = NULL;
		}
	}
}

DWORD WINAPI CPlayStream::ThreadFuncCheckPlayStream(LPVOID pParam)
{
	CPlayStream* pStream = static_cast<CPlayStream*>(pParam);

	DWORD dwRemain = 0;
	DWORD dwBufValue = 0; 

	while (pStream->m_bPlaybackThread)
	{
		//��ͣ�ط�ʱ��������ͣ��ʱ��
		if (pStream->m_bPause)
		{
			Sleep(100);
			continue;
		}
		
		dwRemain = GPAPI_GetSourceBufferRemain(pStream->m_emPlayer, pStream->m_lPort);
		dwBufValue = pStream->GetSpecialStopFlag();

		PLAYBACKSDK_DEBUG("@z m_emPlayer:%d, m_lPort:%ld, dwRemain:%ld, dwBufValue:%ld, m_bStop:%d", 
			pStream->m_emPlayer, pStream->m_lPort, dwRemain, dwBufValue, pStream->m_bStopPlay);

		if (dwRemain == 0  
			&& dwBufValue == 0  && pStream->m_bStopPlay)
		{
			pStream->m_nPlaybackPos = 100;
			PLAYBACKSDK_INFO("PlayStream---�طŽ��ȵ���ĩβ��ֹͣ�ط�ȡ��");
		}

		for (int i = 0; i < 10; i++)
		{
			Sleep(100);
			if (!pStream->m_bPlaybackThread)
			{
				return 0;
			}
		}
	}

	return 0;
}

/**   @fn          StopPlayback
*    @brief   	   ͣס�ط�.
*    @param[in]   
*    @param[in]   
*    @return      
*/
void CPlayStream::StopPlayback()
{
	m_bPause = FALSE;
	m_bOpenSound = FALSE;
	m_emPlaySpeed = PB_CONTROL_TYPE_NOREMAL;
	m_bReversePlay = FALSE;
}

/**   @fn          GetSpecialStopFlag
 *    @brief   	   ��ȡ���������ֹͣ��־..
 *    @param[in]   
 *    @param[in]   
 *    @return      0-ֹͣ,����ֵ-�ȴ�.
 */
DWORD CPlayStream::GetSpecialStopFlag()
{
	return 0;
}

/**  @fn          GetStreamData
*    @brief   	  ��ȡ�ط�������.
*    @param[in]   pStreamData:ȡ���ص�����.
*    @param[in]   dwUser:�û��Զ�������.
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPlayStream::StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser)
{
    CGuard guard(&m_csStreamCB);
	m_pfnStreamCB = pStreamData;
	m_dwStreamUserData = dwUser;

	// ��ȡͷ����.
	//GetHeadData(OnStreamCB, PtrToUlong(this));
	// ��ȡ������.
	//GetBufferData(OnStreamCB, PtrToUlong(this));

	m_bGetHead = FALSE;
	m_bStreamCB = TRUE;
	return TRUE;
}

/**  @fn          StopGetStreamData
*    @brief   	  ֹͣ��ȡ�ط�������.
*    @param[in]   
*    @param[in]   
*    @return     
*/
void CPlayStream::StopGetStreamData()
{
    CGuard guard(&m_csStreamCB);
	m_bStreamCB = FALSE;
	m_bGetHead = FALSE;
	m_pfnStreamCB = NULL;
	m_dwStreamUserData = 0;
}

/**  @fn          GetStreamDataCB
*    @brief   	  ��ȡȡ���ص�����.
*    @param[in]   
*    @param[in]   
*    @return     ȡ���ص�����.
*/
PbStreamDataCB CPlayStream::GetStreamDataCB()
{
	return m_pfnStreamCB;
}

/**  @fn          GetStreamUserData
*    @brief   	  ��ȡȡ���ص��û��Զ�������.
*    @param[in]   
*    @param[in]   
*    @return     �û��Զ�������.
*/
DWORD CPlayStream::GetStreamUserData()
{
	return m_dwStreamUserData;
}

/**  @fn          PushStreamData
*    @brief   	  
*    @param[in]   
*    @param[in]   
*    @return     
*/
void CPlayStream::PushStreamData(int nDataType, BYTE * pBuffer, DWORD dwBufSize)
{
    CGuard guard(&m_csStreamCB);
	// ���ϲ��׻ص�������.
	if (m_pfnStreamCB != NULL)
	{
		PLAYBACKSDK_DEBUG("Stream callback, nDataType:%d, dwBufSize:%lu", nDataType, dwBufSize);
		m_pfnStreamCB(nDataType, pBuffer, dwBufSize, m_dwStreamUserData);
	}
}

/**   @fn          GetPlayTypeFromDeviceType
 *    @brief   	   �����豸���ͻ�ȡ��������.
 *    @param[in]   nType:�豸����.
 *    @param[in]   
 *    @return      
 */
PLAYERTYPE CPlayStream::GetPlayTypeFromDeviceType(int nType)
{
	return PT_HIK;
}

/**   @fn          GetPlayType
 *    @brief   	   ��ȡ�ط�����.
 *    @param[in]   
 *    @param[in]   
 *    @return      �ط�����.
 */
PLAYERTYPE CPlayStream::GetPlayType()
{
	return m_emPlayer;
}

/**   @fn          GetSaveFileHandle
 *    @brief   	   ��ȡ�����ļ����.
 *    @param[in]   
 *    @param[in]   
 *    @return      �ļ����.
 */
HANDLE CPlayStream::GetSaveFileHandle()
{
	return m_hSaveFile;
}

BOOL CPlayStream::GetPictureSize( LONG* lWidth, LONG* lHeight)
{
	if (m_lPort == -1)
	{
		return FALSE;
	}

	if (!GPAPI_GetPictureSize(m_emPlayer,m_lPort, lWidth, lHeight))
	{
		return FALSE;
	}

	return TRUE;
}