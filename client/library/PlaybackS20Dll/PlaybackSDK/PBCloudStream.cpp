#include "StdAfx.h"
#include "PBCloudStream.h"
#include <process.h>
unsigned _stdcall _cloud_play(void *pParam)
{
	CPBCloudStream *pObj = (CPBCloudStream *)pParam;
	if (pObj == NULL)
	{
		return 0;
	}

	pObj->CloudPlay();

	return 0;
}

unsigned _stdcall _cloud_StopDownLoad(void *pParam)
{
	PLAYBACKSDK_INFO("_cloud_StopDownLoad!");
    CPBCloudStream *pObj = (CPBCloudStream *)pParam;
    if (pObj == NULL)
    {
        return 0;
    }
    pObj->ThreadStopDownLoad();
    return 0;
}

CPBCloudStream::CPBCloudStream()
: m_nLogin(0)
, m_bIsLogin(FALSE)
, m_nFileNum(0)
, m_nPlayHandle(-1)
, m_strDecoderIP("")
, m_bIsHeadData(TRUE)
, m_hCloudPlayThread(NULL)
, m_hCloudDownloadThread(NULL)
, m_bFinishFindFile(FALSE)
, m_hStopPlayEvent(NULL)
, m_hStopDownloadEvent(NULL)
, m_hStopCheckSearch(NULL)
, m_nDownloadHandle(-1)
{
	memset(m_stFileInfo, 0, sizeof(m_stFileInfo));
	ZeroMemory(m_szHeadBuf, sizeof(m_szHeadBuf));
	m_dwBuffSize = DEFAULT_PLAY_BUFFER / 2;
}

CPBCloudStream::~CPBCloudStream(void)
{
	m_hCloudPlayThread = NULL;
	m_hCloudDownloadThread = NULL;
	m_hStopPlayEvent = NULL;
	m_hStopCheckSearch = NULL;
	m_hStopDownloadEvent = NULL;
}

void CPBCloudStream::FunDownRecordDataCb(int handle, const char* buff ,int bufflen, int Flat, int error_code, void* pUser)
{
	CPBCloudStream *pObj = (CPBCloudStream *)pUser;
	if (pObj == NULL)
	{
		return ;
	}
	if (Flat == 0)
	{
        Sleep(5000);
        SetEvent(pObj->m_hStopDownloadEvent);
		return ;
	}
	pObj->pFunDownload((HANDLE)pObj->GetPlayHandle(), 0, (BYTE*)buff, bufflen);
	return ;
}

/**   @fn          
 *    @brief   	   	
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
int CPBCloudStream::UserMessage(int LogicType,int ErrorCode,void* pUser)
{
	return 0;
}

//lint -e438
DWORD WINAPI CPBCloudStream::ThreadFuncCheckSearch(LPVOID pParam)
{
	CPBCloudStream* pStream = (CPBCloudStream*)pParam;
	if (NULL == pStream)
	{
		return 0;
	}

	int nCount = 0;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pStream->m_hStopCheckSearch, 300))
	{

		if (nCount >= 10)
		{
			nCount = 0;
			PLAYBACKSDK_ERROR("CloudStream-���ҳ�ʱ");
			break;
		}
		nCount++;
	}

	return 0;
}
//lint +e438
/**   @fn          GetStopCheckSearchHandle
 *    @brief   	   ���ü��¼�������ź���.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPBCloudStream::StopCheckSearchFinish()
{
	SetEvent(m_hStopCheckSearch);
}
/**   @fn          InitSdk
 *    @brief   	   ��ʼ��SDK.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
BOOL CPBCloudStream::InitCloudSdk()
{
    int iRet = HCS_SDK_Init();
    if (HCS_OK != iRet)
    {
        PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_Init falid, err = %d", iRet);
        return FALSE;
    }
	return TRUE;
}

/**   @fn          ReleaseSdk
 *    @brief   	   �ͷ�SDK.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPBCloudStream::ReleaseSdk()
{
    int iRet = HCS_SDK_Deinit();
	if (HCS_OK != iRet)
	{
		PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_Init falid, err = %d", iRet);
	}
}

/**   @fn          Login
 *    @brief   	   ��¼�豸.
 *    @param[in]   stLoginInfo:��¼��Ϣ.
 *    @param[in]   
 *    @return      
 */
BOOL CPBCloudStream::Login(PB_LOGININFO& stLoginInfo)
{
	PB_DVRLOGININFO* pStDvrLoginInfo = &(stLoginInfo.unLoginType.stDvrLoginInfo);
	if (NULL == pStDvrLoginInfo)
	{
		return FALSE;
	}
	m_emPlayer = PT_HIK;

	PLAYBACKSDK_INFO("CloudStream-��¼�豸��Ϣ.ip=%s, port=%d, userName=%s, password=%s, channel=%d",
		pStDvrLoginInfo->szIP, pStDvrLoginInfo->nPort, pStDvrLoginInfo->szUserName, pStDvrLoginInfo->szPassword, m_lChannel);

	int nRet = HCS_SDK_Login(
		pStDvrLoginInfo->szIP, 
		pStDvrLoginInfo->nPort,
		pStDvrLoginInfo->szUserName, 
		pStDvrLoginInfo->szPassword, 
		&m_nLogin);
	if (nRet != HCS_OK)
	{
		PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_Login falid, ErrCode:%d", nRet);
		return FALSE;
	}
	m_bIsLogin = TRUE;
	// ���ƽ�����IP��ַ.
	m_strDecoderIP.Format("%s", pStDvrLoginInfo->szIndex);

	return TRUE;
}

/**   @fn          Logout
 *    @brief   	   �ǳ��豸.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPBCloudStream::Logout()
{	
	if (m_bIsLogin)
	{
		int nRet = HCS_SDK_Logout(m_nLogin);
		if(nRet != HCS_OK)
		{
			PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_Logout falid, LoginID=%I64d, errCode=%d", m_nLogin, nRet);
		}
		//lint -e570
		m_nLogin = -1;
		//lint +e570
		m_bIsLogin = FALSE;
	}
}

/** @fn			FindFile
*   @brief		�����ļ�
*   @param[in]	stFindFileParam: �����ļ�����
*   @param[in]	lpFileList: �ļ�����
*   @param[in]	nFileCount: �ļ�����
*   @return		TRUE�ɹ� FALSEʧ��
*/
BOOL CPBCloudStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (!m_bIsLogin)
	{
		return FALSE;
	}

	// �ÿ��ļ�����.
	m_nFileNum = 0;

	// ��ʼʱ��.
	CTime ctBegin(stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
		stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond);

	// ����ʱ��.
	CTime ctEnd(stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
		stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);


	PLAYBACKSDK_INFO("CloudStream-��ѯ�ļ�����. channel=%d, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", stFindFileParam.lChannel, stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
		stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond, stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
		stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);

	// ��ʼ����.
	int nResult = 0;
    int nHandle = 0;
    HCS_Time stBeginTime = {0};
    stBeginTime.year = stFindFileParam.stStartTime.dwYear;
    stBeginTime.month = stFindFileParam.stStartTime.dwMonth;
    stBeginTime.day = stFindFileParam.stStartTime.dwDay;
    stBeginTime.hour = stFindFileParam.stStartTime.dwHour;
    stBeginTime.minute = stFindFileParam.stStartTime.dwMinute;
    stBeginTime.second = stFindFileParam.stStartTime.dwSecond;
    stBeginTime.millisecond = 0;

    HCS_Time stEndTime = {0};
    stEndTime.year = stFindFileParam.stStopTime.dwYear;
    stEndTime.month = stFindFileParam.stStopTime.dwMonth;
    stEndTime.day = stFindFileParam.stStopTime.dwDay;
    stEndTime.hour = stFindFileParam.stStopTime.dwHour;
    stEndTime.minute = stFindFileParam.stStopTime.dwMinute;
    stEndTime.second = stFindFileParam.stStopTime.dwSecond;
    stEndTime.millisecond = 0;
	int nCount = 0; 
    nResult = HCS_SDK_RecSearchStart(m_nLogin, m_strDecoderIP.GetBuffer(0), stBeginTime, stEndTime, "", HCS_SRLOCK_TYPE_All, &nCount, &nHandle);
	if (nResult != HCS_OK)
	{
		PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_RecSearchStart falid, m_nLogin:%I64d, CamID:%s, errCode=%d", m_nLogin, (LPCTSTR)m_strDecoderIP, nResult);
		return FALSE;
	}

	// �����ļ����б�.
	for (int i = 0; i < nCount; i++)
	{
        int nStatus = 0;
        int nSearchResult = 0;
        HCS_RecordInfo stInfo;
        memset(&stInfo, 0, sizeof(HCS_RecordInfo));
        nSearchResult = HCS_SDK_RecSearchNext(m_nLogin, nHandle, &stInfo, &nStatus);
        if (nSearchResult != HCS_OK)
        {
            HCS_SDK_RecSearchStop(m_nLogin, nHandle);
            PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_RecSearchNext falid, m_nLogin:%I64d, CamID:%s, errCode=%d", m_nLogin, (LPCTSTR)m_strDecoderIP, nSearchResult);
            return FALSE;
        }

        PB_FILEINFO stFileInfo;
        //¼��ʼʱ��
        stFileInfo.stStartTime.dwYear   = stInfo.begin_time.year;
        stFileInfo.stStartTime.dwMonth  = stInfo.begin_time.month;
        stFileInfo.stStartTime.dwDay    = stInfo.begin_time.day;
        stFileInfo.stStartTime.dwHour   = stInfo.begin_time.hour;
        stFileInfo.stStartTime.dwMinute = stInfo.begin_time.minute;
        stFileInfo.stStartTime.dwSecond = stInfo.begin_time.second;
        //¼�����ʱ��
        stFileInfo.stEndTime.dwYear   = stInfo.end_time.year;
        stFileInfo.stEndTime.dwMonth  = stInfo.end_time.month;
        stFileInfo.stEndTime.dwDay    = stInfo.end_time.day;
        stFileInfo.stEndTime.dwHour   = stInfo.end_time.hour;
        stFileInfo.stEndTime.dwMinute = stInfo.end_time.minute;
        stFileInfo.stEndTime.dwSecond = stInfo.end_time.second;
        //¼������
        stFileInfo.nFileType = stInfo.record_type;
        //¼���С
        stFileInfo.dwFileSize = stInfo.size;

        CString str;
        str.Format("cloud_%02u%02u%02u-%02u%02u%02u", 
			stFileInfo.stStartTime.dwHour,
			stFileInfo.stStartTime.dwMinute,
			stFileInfo.stStartTime.dwSecond,
			stFileInfo.stEndTime.dwHour,
			stFileInfo.stEndTime.dwMinute,
			stFileInfo.stEndTime.dwSecond);
        // �ļ���.
        memset(&stFileInfo.szFileName, 0, sizeof(stFileInfo.szFileName));
        sprintf_s(stFileInfo.szFileName, sizeof(stFileInfo.szFileName), str.GetBuffer());

		memcpy_s(&lpFileList[i], sizeof(PB_FILEINFO), &stFileInfo, sizeof(PB_FILEINFO));
	}

    HCS_SDK_RecSearchStop(m_nLogin, nHandle);
	nFileCount = nCount;
	PLAYBACKSDK_INFO("CloudStream-��ѯ�ļ��ɹ�,�ļ�����:%d. ", nCount);

	return TRUE;
}

/**   @fn          PlaybackTime
 *    @brief   	   ��ʱ��ط�.
 *    @param[in]   stPbTimeInfo:ʱ��طŲ���.
 *    @param[in]   
 *    @return      
 */
BOOL CPBCloudStream::PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo)
{
	if (!m_bIsLogin)
	{
		return FALSE;
	}

	// ��ʼʱ��.
	CTime ctBegin(stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond);

	// ����ʱ��.
	CTime ctEnd(stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	m_hPlayHwnd = stPbTimeInfo.hWnd;

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("CloudStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CloudStream-ʱ��طŲ���. m_nLogin=%I64d, m_lPort:%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_nLogin, m_lPort, stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond, stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_nLogin, m_strDecoderIP.GetBuffer(0), stPbTimeInfo.stStartTime, stPbTimeInfo.stStopTime))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CloudStream-StartData false��m_nLogin:%I64d", m_nLogin);
		return FALSE;
	}

	//����������߳�
	if (!StartCheckPlayStream(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CloudStream-StartCheckPlayStream, m_nLogin=%I64d", m_nLogin);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CloudStream-ʱ��طųɹ�, m_nLogin=%I64d", m_nLogin);

	m_stPlaybackTimeInfo = stPbTimeInfo;

	return TRUE;
}

/**   @fn          PlaybackFile
 *    @brief   	   ���ļ��ط�.
 *    @param[in]   stPbFileInfo:�ļ��طŲ���.
 *    @param[in]   
 *    @return      
 */
BOOL CPBCloudStream::PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)
{
	if (!m_bIsLogin)
	{
		return FALSE;
	}

	// ��ʼʱ��.
	CTime ctBegin(
		stPbFileInfo.stFileInfo.stStartTime.dwYear, 
		stPbFileInfo.stFileInfo.stStartTime.dwMonth, 
		stPbFileInfo.stFileInfo.stStartTime.dwDay,
		stPbFileInfo.stFileInfo.stStartTime.dwHour, 
		stPbFileInfo.stFileInfo.stStartTime.dwMinute, 
		stPbFileInfo.stFileInfo.stStartTime.dwSecond);
	// ����ʱ��.
	CTime ctEnd(
		stPbFileInfo.stFileInfo.stEndTime.dwYear, 
		stPbFileInfo.stFileInfo.stEndTime.dwMonth, 
		stPbFileInfo.stFileInfo.stEndTime.dwDay,
		stPbFileInfo.stFileInfo.stEndTime.dwHour, 
		stPbFileInfo.stFileInfo.stEndTime.dwMinute, 
		stPbFileInfo.stFileInfo.stEndTime.dwSecond);

	m_hPlayHwnd = stPbFileInfo.hWnd;

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("CloudStream-GPAPI_GetPort, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("CloudStream-ʱ��طŲ���. m_nLogin=%I64d, m_lPort=%d, ʱ��:start=%s  stop=%s", 
		m_nLogin, m_lPort, (LPCTSTR)ctBegin.Format(_T("%Y-%m-%D %H:%M:%S")), (LPCTSTR)ctEnd.Format(_T("%Y-%m-%D %H:%M:%S")));

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_nLogin, m_strDecoderIP.GetBuffer(0), stPbFileInfo.stFileInfo.stStartTime, stPbFileInfo.stFileInfo.stEndTime))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CloudStream-StartData false��m_nLogin:%I64d", m_nLogin);
		return FALSE;
	}

	//����������߳�
	if (!StartCheckPlayStream(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("CloudStream-StartCheckPlayStream, m_nLogin=%I64d", m_nLogin);
		return FALSE;
	}
 
	PLAYBACKSDK_INFO("CloudStream-ʱ��طųɹ�. m_nLogin=%I64d", m_nLogin);

	m_stPlaybackTimeInfo.stStartTime = stPbFileInfo.stFileInfo.stStartTime;
	m_stPlaybackTimeInfo.stStopTime = stPbFileInfo.stFileInfo.stEndTime;
	m_stPlaybackTimeInfo.lChannel = stPbFileInfo.lChannel;
	m_stPlaybackTimeInfo.hWnd = stPbFileInfo.hWnd;

	return TRUE;
}

/**   @fn          StartData
 *    @brief   	   ��ʼȡ��.
 *    @param[in]   uLogin:��¼ID.
 *    @param[in]   pCamID:��ص�ID.
 *    @param[in]   stStartTime:��ʼʱ��.
 *    @param[in]   stStopTime:����ʱ��.
 *    @return      
 */
BOOL CPBCloudStream::StartData(unsigned long long uLogin, const char* pCamID, const PB_TIME &stStartTime, const PB_TIME &stStopTime)
{
	if (!m_bIsLogin)
	{
		return FALSE;
	}
    HCS_Time stBeginTime = {0};
    HCS_Time stEndTime = {0};
    stBeginTime.year        = stStartTime.dwYear;
    stBeginTime.month       = stStartTime.dwMonth;
    stBeginTime.day         = stStartTime.dwDay;
    stBeginTime.hour        = stStartTime.dwHour;
    stBeginTime.minute      = stStartTime.dwMinute;
    stBeginTime.second      = stStartTime.dwSecond;
    stBeginTime.millisecond = 0;

    stEndTime.year        = stStopTime.dwYear;
    stEndTime.month       = stStopTime.dwMonth;
    stEndTime.day         = stStopTime.dwDay;
    stEndTime.hour        = stStopTime.dwHour;
    stEndTime.minute      = stStopTime.dwMinute;
    stEndTime.second      = stStopTime.dwSecond;
    stEndTime.millisecond = 0;
	PLAYBACKSDK_INFO("CPBCloudStream::StartData:%s, %d-%d-%d %d:%d:%d--%d-%d-%d %d:%d:%d", 
		pCamID, stStartTime.dwYear, stStartTime.dwMonth, stStartTime.dwDay, stStartTime.dwHour, stStartTime.dwMinute, stStartTime.dwSecond,
		stStopTime.dwYear, stStopTime.dwMonth, stStopTime.dwDay, stStopTime.dwHour, stStopTime.dwMinute, stStopTime.dwSecond);
	PLAYBACKSDK_INFO("CPBCloudStream::StartData:m_bDownload=%d", m_bDownload);
	if (m_bDownload)
	{
        StopData();
		int nHeadSize = 0;
		ZeroMemory(m_szHeadBuf, sizeof(m_szHeadBuf));
        int nRet = HCS_SDK_GetRecHead(m_nLogin, pCamID, stBeginTime, stEndTime, m_szHeadBuf, 4*1024, &nHeadSize);
		if (nRet != HCS_OK)
		{
			PB_SetLastError(PB_SEND_TO_DEV_FAILD);
			PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_GetRecHead falid��m_nLogin=%I64d ErrorCode = %ld", m_nLogin, nRet);
			return FALSE;
		}
        seg_head_t pSegData;
        memset(&pSegData, 0, sizeof(seg_head_t));
        memcpy(pSegData.headData, m_szHeadBuf, sizeof(pSegData.headData));
		pFunDownload((HANDLE)m_nDownloadHandle, 0, (BYTE*)pSegData.headData, pSegData.headSize);
        nRet = HCS_SDK_RecDownloadStart(m_nLogin, pCamID, stBeginTime, stEndTime, "", 0, FunDownRecordDataCb, this, &m_nDownloadHandle);
		if (nRet != HCS_OK)
		{
			PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_RecDownloadStart falid��m_nLogin=%I64d ErrorCode = %ld", m_nLogin, nRet);
			return FALSE;
		}

		//����������߳̿��ܵ��±���
//         m_hStopDownloadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//         ResetEvent(m_hStopDownloadEvent);
//         m_hCloudDownloadThread = (HANDLE)_beginthreadex(NULL, 0, _cloud_StopDownLoad, this, 0, NULL);
	}
	else
	{
		//����ڻط��У�ͣ��
		StopData();

		m_bBlock = TRUE;
		
		int nHeadSize = 0;
		ZeroMemory(m_szHeadBuf, sizeof(m_szHeadBuf));
        int nRet = HCS_SDK_GetRecHead(m_nLogin, pCamID, stBeginTime, stEndTime, m_szHeadBuf, 4*1024, &nHeadSize);
        if (nRet != HCS_OK)
        {
			PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_GetRecHead��m_nLogin=%I64d ErrorCode = %ld", m_nLogin, nRet);
			return FALSE;
		}
        seg_head_t pSegData;
        memset(&pSegData, 0, sizeof(seg_head_t));
        memcpy(pSegData.headData, m_szHeadBuf, sizeof(pSegData.headData));
        pSegData.headSize = nHeadSize;
		pFunPlayback((HANDLE)m_nPlayHandle, PB_STREAM_HEADER, pSegData.headData, pSegData.headSize);

		// ȡ���ط�.
        nRet = HCS_SDK_PlaybackStart(m_nLogin, pCamID, stBeginTime, stEndTime, HCS_PBMODE_NORMAL, "", &m_nPlayHandle);
        if (nRet != HCS_OK)
        {
			PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_PlaybackStart��m_nLogin=%I64d ErrorCode = %ld", m_nLogin, m_nPlayHandle);
			return FALSE;
		}
		m_hStopPlayEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ResetEvent(m_hStopPlayEvent);
		m_hCloudPlayThread = (HANDLE)_beginthreadex(NULL, 0, _cloud_play, this, 0, NULL);
	}
	
	return TRUE;
}

/**   @fn          StopData
 *    @brief   	   ֹͣȡ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
BOOL CPBCloudStream::StopData()
{
	if (-1 == m_nPlayHandle)
	{
		return TRUE;
	}

	// ֹͣ���ݻص�����.
	m_bBlock = FALSE;

	// ֹͣȡ��.
	int nReult = HCS_SDK_PlaybackStop(m_nLogin, m_nPlayHandle);
	if (nReult != HCS_OK)
	{
		PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_PlaybackStop falid, m_nLogin:%I64d, errCode=%d", m_nLogin, nReult);
	}
	CPlayStream::StopPlay();
	if (m_hCloudPlayThread != NULL)
	{
		SetEvent(m_hStopPlayEvent);
		WaitForSingleObject(m_hCloudPlayThread, INFINITE);
		CloseHandle(m_hCloudPlayThread);
		m_hCloudPlayThread = NULL;
		CloseHandle(m_hStopPlayEvent);
		m_hStopPlayEvent = NULL;
		m_nPlayHandle = -1;
		m_bStopPlay = FALSE;
	}
    if (m_hCloudDownloadThread != NULL)
    {
        SetEvent(m_hStopDownloadEvent);
        WaitForSingleObject(m_hCloudDownloadThread, INFINITE);
        CloseHandle(m_hCloudDownloadThread);
        m_hCloudDownloadThread = NULL;
        CloseHandle(m_hStopDownloadEvent);
        m_hStopDownloadEvent = NULL;
    }
    
	return TRUE;
}

/**   @fn          StopPlayback
 *    @brief   	   ֹͣ�ط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPBCloudStream::StopPlayback()
{
	if (!m_bIsLogin)
	{
		return;
	}

	// ֹͣȡ��.
	StopData();

	//ֹͣ����
	StopPlay();

	//�ر��߳�
	StopCheckPlayStream(TRUE);

	PLAYBACKSDK_INFO("CloudStream-ֹͣ�ط�. m_nLogin=%I64d", m_nLogin);

	CPlayStream::StopPlayback();
}

/**   @fn          StartCheckSearchFinish
 *    @brief   	   ���¼�������Ƿ����.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
//lint -e438
void CPBCloudStream::StartCheckSearchFinish()
{
	PLAYBACKSDK_INFO("CloudStream-begin StartCheckSearchFinish");
	m_hStopCheckSearch = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hStopCheckSearch)
	{
		PLAYBACKSDK_ERROR("CloudStream-m_hStopCheckSearch is null");
		return;
	}
	ResetEvent(m_hStopCheckSearch);
	HANDLE hThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFuncCheckSearch), this, 0, NULL);
	if (hThread != NULL)
	{
		WaitForWindowThread(hThread);
		CloseHandle(hThread);
		hThread = NULL;
	}

	if (m_hStopCheckSearch != NULL)
	{
		CloseHandle(m_hStopCheckSearch);
		m_hStopCheckSearch = NULL;
	}
	PLAYBACKSDK_INFO("CloudStream-end StartCheckSearchFinish");
}
//lint +e438

/**   @fn          DownloadFile
 *    @brief   	   �����ļ�.
 *    @param[in]   stDownloadParam:���ز���.
 *    @param[in]   
 *    @return      
 */
BOOL CPBCloudStream::DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam)
{
	if (!m_bIsLogin)
	{
		return FALSE;
	}
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
		stDownloadParam.stFileInfo.stEndTime.dwSecond);

	m_bDownload = TRUE;
	PB_SetLastError(PB_NOERROR);
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PB_SetLastError(PB_CREATEFILE_FILE);
		PLAYBACKSDK_ERROR("CloudStream-Create_File falid, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;
	//ȡ�ļ���
	PLAYBACKSDK_INFO("CloudStream-�ļ����ز���. m_nLogin=%I64d, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_nLogin, stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour, stDownloadParam.stFileInfo.stStartTime.dwMinute, stDownloadParam.stFileInfo.stStartTime.dwSecond, stDownloadParam.stFileInfo.stEndTime.dwYear, stDownloadParam.stFileInfo.stEndTime.dwMonth, stDownloadParam.stFileInfo.stEndTime.dwDay,
		stDownloadParam.stFileInfo.stEndTime.dwHour, stDownloadParam.stFileInfo.stEndTime.dwMinute, stDownloadParam.stFileInfo.stEndTime.dwSecond);

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_nLogin, m_strDecoderIP.GetBuffer(0), stDownloadParam.stFileInfo.stStartTime, stDownloadParam.stFileInfo.stEndTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CloudStream-StartData falid m_nLogin=%I64d", m_nLogin);
		return FALSE;
	}

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CloudStream-StartStreamThread falid, m_nLogin=%I64d", m_nLogin);
		return FALSE;
	}

	//��ʼȡ��ʱ��
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/**   @fn          DownloadTime
 *    @brief   	   ����ʱ��.
 *    @param[in]   stDownloadParam:���ز���.
 *    @param[in]   
 *    @return      
 */
BOOL CPBCloudStream::DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam)
{
	if (!m_bIsLogin)
	{
		return FALSE;
	}

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
		stDownloadParam.stStopTime.dwSecond);

	PB_SetLastError(PB_NOERROR);
	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PB_SetLastError(PB_CREATEFILE_FILE);
		PLAYBACKSDK_ERROR("CloudStream-Create_File falid, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nDownloadPos = 0;
	//ȡ�ļ���
	PLAYBACKSDK_INFO("CloudStream-ʱ�����ز���. m_nLogin=%I64d, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_nLogin, stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour, stDownloadParam.stStartTime.dwMinute, stDownloadParam.stStartTime.dwSecond, stDownloadParam.stStopTime.dwYear, stDownloadParam.stStopTime.dwMonth, stDownloadParam.stStopTime.dwDay,
		stDownloadParam.stStopTime.dwHour, stDownloadParam.stStopTime.dwMinute, stDownloadParam.stStopTime.dwSecond);

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_nLogin, m_strDecoderIP.GetBuffer(0), stDownloadParam.stStartTime, stDownloadParam.stStopTime))
	{
		m_bDownload = FALSE;
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CloudStream-StartData falid, m_nLogin=%I64d", m_nLogin);
		return FALSE;
	}

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("CloudStream-StartStreamThread falid, m_nLogin=%I64d", m_nLogin);
		return FALSE;
	}

	//��ʼȡ��ʱ��
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/**   @fn          StopDownload
 *    @brief   	   ֹͣ����.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPBCloudStream::StopDownload()
{
	if (m_bDownload)
	{
		//ֹͣ����
		int nRet = HCS_SDK_RecDownloadStop(m_nLogin, (int)m_nDownloadHandle);
		if(nRet < 0)
		{
			PLAYBACKSDK_ERROR("CloudStream-HCS_SDK_RecDownloadStop falid, ErrorCode=%d", nRet);
		}
		//�رռ���߳�
		StopStreamThread(FALSE);
		m_bDownload = FALSE;
		if (m_hDownloadFile)
		{
			CloseHandle(m_hDownloadFile);
			m_hDownloadFile = NULL;
		}

		PLAYBACKSDK_INFO("CloudStream-ֹͣ����. lLogin=%I64d", m_nLogin);
	}
}

void CPBCloudStream::CloudPlay()
{
	char szBuff[10 * 1024];    //����Ĵ�Сԭ��Ϊ64����̫��coverity �޷�����ͨ������������ÿ�����Ӧ�ļӴ�
	int nDataLen = 0;
	int nMaxSize = sizeof(szBuff);

	while (WAIT_OBJECT_0 != WaitForSingleObject(m_hStopPlayEvent, 0))
	{
        int iRet = HCS_SDK_PlaybackData(m_nLogin, m_nPlayHandle, szBuff, sizeof(szBuff), &nDataLen);
        if (HCS_OK != iRet || (HCS_OK == iRet && nDataLen == 0))
		{
			m_bStopPlay = TRUE;
			break;
		}
		if (nDataLen > nMaxSize)
		{
			nDataLen = nMaxSize;
		}
	
		if (m_lPort != -1)
		{
			pFunPlayback((HANDLE)m_nPlayHandle, PB_STREAM_DATA, (BYTE*)szBuff, nDataLen);
		}
	}
}
void CPBCloudStream::ThreadStopDownLoad()
{
    while (WAIT_OBJECT_0 != WaitForSingleObject(m_hStopDownloadEvent, 0))
    {
        Sleep(50);
    }
    Sleep(10);
    SetDownloadPos(100);
    StopDownload();
}
