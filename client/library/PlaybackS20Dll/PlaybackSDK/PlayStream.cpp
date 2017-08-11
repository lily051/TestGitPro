#include "stdafx.h"
#include "PlayStream.h"

/**   @fn          OnSaveData
 *    @brief   	   保存数据.
 *    @param[in]   nDataType:数据类型.
 *    @param[in]   pBuffer:数据指针.
 *    @param[in]   dwBufSize:数据大小.
 *    @param[in]   dwUser:用户自定义数据.
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
		PLAYBACKSDK_ERROR("PlayStream---保存数据失败. Len=%d", dwBufSize);
	}

	return 0;
}

/**   @fn          OnStreamCB
 *    @brief   	   取流回调.
 *    @param[in]   nDataType:数据类型.
 *    @param[in]   pBuffer:数据指针.
 *    @param[in]   dwBufSize:数据大小.
 *    @param[in]   dwUser:用户自定义数据.
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
	
	// 向上层抛回调流数据.
	if (pfnStreamCB != NULL)
	{
		PLAYBACKSDK_DEBUG("Stream callback, nDataType:%d, dwBufSize:%lu", nDataType, dwBufSize);
		pfnStreamCB(nDataType, pBuffer, dwBufSize, dwUserData);
	}

	return 0;
}

/**   @fn          SetHeadData
 *    @brief   	   设置头数据缓冲.
 *    @param[in]   pBute:数据指针.
 *    @param[in]   dwBufSize:数据大小.
 *    @return      
 */
void CPlayStream::SetHeadData(PBYTE pBuffer, DWORD dwBufSize)
{
	memcpy_s(m_byteHeadBuf, sizeof(m_byteHeadBuf), pBuffer, dwBufSize);
	m_nHeadLen = dwBufSize > 1024 ? 1024:dwBufSize;
}

/**   @fn          GetHeadData
 *    @brief   	   获取头数据.
 *    @param[in]   PbStreamDataCB:数据回调指针.
 *    @param[in]   dwUserData:用户自定义数据.
 *    @return      
 */
void CPlayStream::GetHeadData(PbStreamDataCB pfnStreamData, DWORD dwUserData)
{
	pfnStreamData(PB_STREAM_HEADER, m_byteHeadBuf, m_nHeadLen, dwUserData);
}

/**   @fn          SetBufferData
 *    @brief   	   设置缓冲区数据.
 *    @param[in]   pBute:数据指针.
 *    @param[in]   dwBufSize:数据大小.
 *    @return      
 */
void CPlayStream::SetBufferData(PBYTE pBuffer, DWORD dwBufSize)
{
	// 保存到剪辑缓冲区.
	CGuard guard(&m_csSaveBufLock);

	if (dwBufSize < MAX_SAVEBUF_SIZE)
	{
		// 如果后半段缓冲区空间足够存储本次来的流数据，则将其全部存储.
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
	else	// 流数据不会比缓冲区大，这里没做循环写入处理.
	{
		memcpy_s(m_byteSaveBuf, MAX_SAVEBUF_SIZE, pBuffer, dwBufSize);
		m_dwCurBufPointer = 0;
	}
}

/**   @fn          GetBufferData
 *    @brief   	   获取缓冲区数据.
 *    @param[in]   pfnStreamData:数据回调函数.
 *    @param[in]   dwUserData:用户自定义数据.
 *    @return      
 */
void CPlayStream::GetBufferData(PbStreamDataCB pfnStreamData, DWORD dwUserData)
{
	// 将剪辑缓冲区中的数据保存到文件中.
	CGuard guard(&m_csSaveBufLock);

	// 获取播放库缓冲区中的剩余数据大小.
	DWORD dwRemain = GPAPI_GetSourceBufferRemain(m_emPlayer, m_lPort);
	dwRemain += MAX_PRE_SAVEBUF_SIZE;
	if(dwRemain > MAX_SAVEBUF_SIZE)
	{
		TRACE("@z full write");
		dwRemain = MAX_SAVEBUF_SIZE;
	}

	// 没有从头开始写数据，则保存全部数据.
	if (!m_bIsReWrite)
	{
		pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[0], m_dwCurBufPointer, dwUserData);
	}
	// 从头开始写过数据则先保存缓冲区后半段数据，再保存前半段，后半段数据时间早于前半段.
	else
	{
		// 如果播放库中未解码数据全部保存在前半段数据中，则只保存前半段数据.
		if (dwRemain <= m_dwCurBufPointer)
		{
			pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[m_dwCurBufPointer - dwRemain], dwRemain, dwUserData);
		}
		// 如果播放库中未解码数据在缓存的前半段和后半段都有保存，则先保存缓冲区后半段数据，再保存前半段.
		else
		{
			DWORD dwLen = MAX_SAVEBUF_SIZE - (dwRemain - m_dwCurBufPointer);
			// 保存后半段数据.
			pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[dwLen], dwRemain - m_dwCurBufPointer, dwUserData);

			// 保存前半段数据.
			pfnStreamData(PB_STREAM_DATA, &m_byteSaveBuf[0], m_dwCurBufPointer, dwUserData);
		}
	}
}

/** @fn pFunPlayback
*   @brief 取流回调函数
*   @param[in] hPlayData: 回放句柄
*   @param[in] nDataType: 流类型
*   @param[in] pBuffer:   流数据
*   @param[in] dwBufferSize: 数据大小
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
				PLAYBACKSDK_ERROR("PlayStream---funDataback!GPAPI_StartPlay失败. Player=%d, port=%ld, PlayHwnd=%ld, bufSize=%ld", m_emPlayer, m_lPort, (LONG)m_hPlayHwnd, dwBufSize);
			}
			else
			{
                m_bPlaying = TRUE;
				//开启音频
				if (!GPAPI_PlaySound(m_emPlayer,m_lPort))
				{
					PLAYBACKSDK_ERROR("PlayStream---GPAPI_PlaySound, m_lPort:%ld", m_lPort);
				}
				m_bOpenSound = TRUE;
				// 如果是倒放.
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

				// 保存数据.
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

	// 向上层抛回调流数据.
	if (m_bStreamCB)
	{
		if (!m_bGetHead)
		{
			if (m_byteHeadBuf[0] != '\0')
			{
				m_bGetHead = TRUE;
				GetHeadData(OnStreamCB, PtrToUlong(this));
                //清楚残余流
                GPAPI_ResetSourceBuffer(m_emPlayer, m_lPort);
                // 获取流数据.
                //GetBufferData(OnStreamCB, PtrToUlong(this));
			}	
		}

		PushStreamData(nDataType, pBuffer, dwBufSize);
	}
 
	//最后一次取流时间
	m_lPlaybackLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return bRet;
}

/** @fn pFunDownload
*   @brief 取流回调函数
*   @param[in] hPlayData: 回放句柄
*   @param[in] nDataType: 流类型
*   @param[in] pBuffer:   流数据
*   @param[in] dwBufferSize: 数据大小
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
				strMsg.Format("PlayStream---下载进度.pos=%d, bufferSize=%ld, curDownsize=%ld, totleSize=%d", m_nDownloadPos, dwBufferSize, m_dwCurDownSize, m_dwTotleSize);
				OutputDebugString(strMsg);
				m_nDownloadPos = 100;
			}
		}
		//最后一次取流时间
		m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();
	}
}

/** @fn PlaybackStreamDetect
*   @brief 回放流检测线程
*   @param[in] lpvoid: 线程参数
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
		//暂停回放时，不计入停流时间
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
		//PLAYBACK_DETECT_TIME时间内没有来流，标识回放结束
		LONG lCurTime = (LONG)CTime::GetCurrentTime().GetTime();
		DWORD dwRemain = GPAPI_GetSourceBufferRemain(pInstance->m_emPlayer, pInstance->m_lPort);
		if (pInstance->m_lPlaybackLastTime != 0  
			&&  pInstance->m_lPlaybackLastTime + PLAYBACK_DETECT_TIME < lCurTime
			&&  (0 == dwRemain || DEFAULT_PLAY_BUFFER < dwRemain)  
			/*&&  0 == dwBufValue*/)
		{
			pInstance->m_nPlaybackPos = 100;
			PLAYBACKSDK_INFO("PlayStream---%d秒时间内没有来流，停止回放取流", PLAYBACK_DETECT_TIME);
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
*   @brief 下载流检测线程
*   @param[in] lpvoid: 线程参数
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
		//DOWNLOAD_DETECT_TIME时间内没有来流，标识回放结束
		LONG lCurTime = (LONG)CTime::GetCurrentTime().GetTime();
		if (pInstance->m_lDownloadLastTime != 0 && pInstance->m_lDownloadLastTime + DOWNLOAD_DETECT_TIME < lCurTime)
		{
			pInstance->m_nDownloadPos = 100;
			PLAYBACKSDK_INFO("PlayStream---%d秒时间内没有来流，停止下载取流", PLAYBACK_DETECT_TIME);
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
*   @brief 定位绝对时间
*   @param[in]  stStartTime: 定位时间
*   @param[in]  stStopTime:停止时间.
*   @param[out] NULL
*   @return TRUE成功 FALSE失败
*/
BOOL CPlayStream::LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime)
{
	//判断时间
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
		PLAYBACKSDK_ERROR("PlayStream---定位时间大于当前回放结束时间. tPlayTime=%ld, tStopTime=%ld", (LONG)tPlayTime.GetTime(), (LONG)tStopTime.GetTime());
		return FALSE;
	}

	PLAYBACKSDK_INFO("PlayStream---定位时间, starTime:%s, stopTime:%s", 
		(LPCTSTR)tPlayTime.Format("%Y-%m-%d %H:%M:%S"), (LPCTSTR)tStopTime.Format("%Y-%m-%d %H:%M:%S"));

	// 记录是否打开了音频.
	BOOL bOpenSound = m_bOpenSound;
	// 记录原来的回放速度.
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
*   @brief 停止解码
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPlayStream::StopPlay()
{
	if (m_lPort != -1)
	{
		GPAPI_StopPlay(m_emPlayer, m_lPort);
		//清楚残余流
		GPAPI_ResetSourceBuffer(m_emPlayer, m_lPort);
		//释放端口
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
        m_bPlaying = FALSE;
	}
}

/** @fn PlaybackControl
*   @brief 回放控制
*   @param[in] emControlType: 控制类型
*   @param[in] lValueIn: 输入参数
*   @param[out] lValueOut: 输出参数
*   @return TRUE成功 FALSE失败
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
					PLAYBACKSDK_INFO("PlayStream---Reverse 调节播放速度失败. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
					return FALSE;
				}
			}
			else
			{
				if (!GPAPI_SetPlaySpeed(m_emPlayer, m_lPort, (PLAYSPEED)emControlType, m_hPlayHwnd))
				{
                    PLAYBACKSDK_INFO("PlayStream---调节播放速度失败. Player=%d, m_lPort=%ld, speed=%d,error:%d", 
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
				PLAYBACKSDK_INFO("PlayStream---暂停失败. Player=%d, m_lPort=%ld, speed=%d, error = %s", m_emPlayer, m_lPort, (int)emControlType, GPAPI_GetLastErrorString(m_emPlayer, m_lPort));
				return FALSE;
			}
			m_bPause = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_RESTART:
		{
			if (!GPAPI_Pause(m_emPlayer, m_lPort, FALSE))
			{
				PLAYBACKSDK_INFO("PlayStream---恢复失败. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bPause = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_OPEN_AUDIO:
		{
			if (!GPAPI_PlaySound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("PlayStream---开启音频失败. Player=%d, m_lPort=%ld, speed=%d, error=%d", m_emPlayer, m_lPort, (int)emControlType, GPAPI_GetLastError(m_emPlayer, m_lPort));
				return FALSE;
			}
			m_bOpenSound = TRUE;
		}
		break;
	case PB_CONTROL_TYPE_CLOSE_AUDIO:
		{
			if (!GPAPI_StopSound(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("PlayStream---关闭音频失败. Player=%d, m_lPort=%ld, speed=%d", m_emPlayer, m_lPort, (int)emControlType);
				return FALSE;
			}
			m_bOpenSound = FALSE;
		}
		break;
	case PB_CONTROL_TYPE_SET_VOLUME:
		{
			if (!GPAPI_SetVolume(m_emPlayer, m_lPort, (WORD)lValueIn))
			{
				PLAYBACKSDK_INFO("PlayStream---调节音量失败. Player=%d, m_lPort=%ld, speed=%d, volumn=%d", m_emPlayer, m_lPort, (int)emControlType, (int)lValueIn);
				return FALSE;
			}
		}
		break;

	case PB_CONTROL_TYPE_SINGLE_FRAME:
		{
			if (!GPAPI_PlayOneByOne(m_emPlayer, m_lPort))
			{
				PLAYBACKSDK_INFO("PlayStream---单帧播放失败. Player=%d, m_lPort=%ld", m_emPlayer, m_lPort);
				return FALSE;
			}
		}
		break;

	case PB_CONTROL_TYPE_SINGLE_FRAME_RESTORE:
		{
			if (!GPAPI_PlayOneByOneRestore(m_emPlayer, m_lPort, m_hPlayHwnd))
			{
				PLAYBACKSDK_INFO("PlayStream---停止单帧播放失败. Player=%d, m_lPort=%ld", m_emPlayer, m_lPort);
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
*   @brief 获取osd时间
*   @param[in]  NULL
*   @param[out] lOsdTime: osd时间，32time_t数值
*   @return TRUE成功 FALSE失败
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
			nMin >= 0 && nMin <= 59 &&nSec >= 0 && nSec <= 59)//校验时间的是否是合法时间
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
*   @brief 获取回放进度
*   @param[in]  NULL
*   @param[out] lPos: 进度
*   @return TRUE成功 FALSE失败
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
*   @brief 回放抓图
*   @param[in] szPicFile: 图片路径
*   @param[in] emPicType: 图片类型
*   @return TRUE成功 FALSE失败
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

	PLAYBACKSDK_INFO("PlayStream---抓图. Player=%d, picPath=%s, picType=%d", (int)m_emPlayer, szPicFile, picType);

	return GPAPI_Capture(m_emPlayer, m_lPort, szPicFile, picType);
}

/** @fn PB_SaveData
*   @brief 录像剪辑
*   @param[in] szFile: 文件存储地址
*   @param NULL
*   @return TRUE成功 FALSE失败
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
		PLAYBACKSDK_ERROR("PlayStream---创建保存数据文件失败. file: %s", szSaveFile);
		return FALSE;
	}

	// 获取头数据.
	GetHeadData(OnSaveData, HandleToULong(m_hSaveFile));
    CGuard guard(&m_csSaveBufLock);   //对m_bSaveFile访问防止冲突
	// 获取流数据.
	GetBufferData(OnSaveData, HandleToULong(m_hSaveFile));
    
	m_bSaveFile = TRUE;
	return TRUE;
}

/** @fn PB_StopSave
*   @brief 停止剪辑
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
*   @brief 获取下载进度
*   @param[out] nPos: 进度
*   @param NULL
*   @return TRUE成功 FALSE失败
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
*   @brief 开启线程
*   @param[in] bPlayback: TRUE回放线程 FALSE下载线程
*   @param NULL
*   @return BOOL成功 FALSE失败
*/
BOOL CPlayStream::StartStreamThread(BOOL bPlayback)
{
	OutputDebugString("@z StartStreamThread");
	// 给最后取流时间赋初始值.
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
*   @brief 停止流检测线程
*   @param[in] bPlayback: TRUE回放线程 FALSE下载线程
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
		//暂停回放时，不计入停流时间
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
			PLAYBACKSDK_INFO("PlayStream---回放进度到达末尾，停止回放取流");
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
*    @brief   	   停住回放.
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
 *    @brief   	   获取检测流特殊停止标志..
 *    @param[in]   
 *    @param[in]   
 *    @return      0-停止,其他值-等待.
 */
DWORD CPlayStream::GetSpecialStopFlag()
{
	return 0;
}

/**  @fn          GetStreamData
*    @brief   	  获取回放流数据.
*    @param[in]   pStreamData:取流回调函数.
*    @param[in]   dwUser:用户自定义数据.
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPlayStream::StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser)
{
    CGuard guard(&m_csStreamCB);
	m_pfnStreamCB = pStreamData;
	m_dwStreamUserData = dwUser;

	// 获取头数据.
	//GetHeadData(OnStreamCB, PtrToUlong(this));
	// 获取流数据.
	//GetBufferData(OnStreamCB, PtrToUlong(this));

	m_bGetHead = FALSE;
	m_bStreamCB = TRUE;
	return TRUE;
}

/**  @fn          StopGetStreamData
*    @brief   	  停止获取回放流数据.
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
*    @brief   	  获取取流回调函数.
*    @param[in]   
*    @param[in]   
*    @return     取流回调函数.
*/
PbStreamDataCB CPlayStream::GetStreamDataCB()
{
	return m_pfnStreamCB;
}

/**  @fn          GetStreamUserData
*    @brief   	  获取取流回调用户自定义数据.
*    @param[in]   
*    @param[in]   
*    @return     用户自定义数据.
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
	// 向上层抛回调流数据.
	if (m_pfnStreamCB != NULL)
	{
		PLAYBACKSDK_DEBUG("Stream callback, nDataType:%d, dwBufSize:%lu", nDataType, dwBufSize);
		m_pfnStreamCB(nDataType, pBuffer, dwBufSize, m_dwStreamUserData);
	}
}

/**   @fn          GetPlayTypeFromDeviceType
 *    @brief   	   根据设备类型获取播放类型.
 *    @param[in]   nType:设备类型.
 *    @param[in]   
 *    @return      
 */
PLAYERTYPE CPlayStream::GetPlayTypeFromDeviceType(int nType)
{
	return PT_HIK;
}

/**   @fn          GetPlayType
 *    @brief   	   获取回放类型.
 *    @param[in]   
 *    @param[in]   
 *    @return      回放类型.
 */
PLAYERTYPE CPlayStream::GetPlayType()
{
	return m_emPlayer;
}

/**   @fn          GetSaveFileHandle
 *    @brief   	   获取保存文件句柄.
 *    @param[in]   
 *    @param[in]   
 *    @return      文件句柄.
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