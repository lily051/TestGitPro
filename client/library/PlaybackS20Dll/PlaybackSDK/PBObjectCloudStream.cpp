/**   @file HikStream.cpp
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief �����豸������
 *
 *    @author   litongli
 *    @date     2016/08/24
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#include "StdAfx.h"
#include <process.h>
#include "PBObjectCloudStream.h"

/** @fn ObjectCloudStreamDataCb
*   @param [out] buff:    ¼������
*   @param [out] bufflen: ���ݳ��� 
*   @param [out] offset:  ��ǰ���������������������е�ƫ��
*   @param [out] error:   ����0: ��ǰ���سɹ�; С��0: ��ʾ���س��ִ���,ֵΪ������������
*   @param [out] pUser:   �û��Զ������� 
*   @@return ����0: �����Ѵ���; С��0: ����δ����
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
*   @brief ��ʼ��
*   @param[in] 
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBObjectCloudStream::InitObjectCloudSdk()
{	
	PLAYBACKSDK_INFO("�����Ƴ�ʼ���ɹ�");
	return TRUE;
}

/** @fn ReleaseObjectCloudSdk
*   @brief �ͷ�sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBObjectCloudStream::ReleaseObjectCloudSdk()
{
	PLAYBACKSDK_INFO("�������ͷųɹ�");
}


/** @fn Login
*   @brief ��¼�豸����ȡ�����������طŻ����ز������ڴ����Ͻ���
*   @param[in] stLoginInfo: ��¼�豸��Ϣ
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	// ����Bucket.
	m_strBucket.Format("%s", pStDvrLoginInfo->szBucket);
	// ����ObjectKey.
	m_strObjectKey.Format("%s", pStDvrLoginInfo->szObjectKey);

	PLAYBACKSDK_INFO("szBucket(%s),szObjectKey(%s)", pStDvrLoginInfo->szBucket, pStDvrLoginInfo->szObjectKey);
	return TRUE;
}

/** @fn Logout
*   @brief �ǳ��豸,���پ��
*   @param[in] hLogin: ��¼���
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
*   @brief �����ļ�
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBObjectCloudStream::FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}

	// ��ʼʱ��.
	CTime ctBegin(stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
		stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond);

	// ����ʱ��.
	CTime ctEnd(stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
		stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);

	time_t iStartTime = ctBegin.GetTime();
	time_t iStopTime  = ctEnd.GetTime();

	PLAYBACKSDK_INFO("ObjectCloudStream-��ѯ�ļ�����. channel=%d, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", stFindFileParam.lChannel, stFindFileParam.stStartTime.dwYear, stFindFileParam.stStartTime.dwMonth, stFindFileParam.stStartTime.dwDay,
		stFindFileParam.stStartTime.dwHour, stFindFileParam.stStartTime.dwMinute, stFindFileParam.stStartTime.dwSecond, stFindFileParam.stStopTime.dwYear, stFindFileParam.stStopTime.dwMonth, stFindFileParam.stStopTime.dwDay,
		stFindFileParam.stStopTime.dwHour, stFindFileParam.stStopTime.dwMinute, stFindFileParam.stStopTime.dwSecond);

    HCSStreamTimeSegmentListEx *seg_list = NULL;
	int ret = QueryObjectTimesegmentEx(m_hHCSClient, m_strBucket.GetBuffer(0), m_strObjectKey.GetBuffer(0), iStartTime, iStopTime, &seg_list, 1);
	if (0 != ret)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-QueryObjectTimesegmentEx fail�� ErrorCode = %d", ret);
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
		//¼��ʼʱ��
		stFileInfo.stStartTime.dwYear   = stStartTime.tm_year + 1900;
		stFileInfo.stStartTime.dwMonth  = stStartTime.tm_mon + 1;
		stFileInfo.stStartTime.dwDay    = stStartTime.tm_mday;
		stFileInfo.stStartTime.dwHour   = stStartTime.tm_hour;
		stFileInfo.stStartTime.dwMinute = stStartTime.tm_min;
		stFileInfo.stStartTime.dwSecond = stStartTime.tm_sec;
		//¼�����ʱ��
		stFileInfo.stEndTime.dwYear     = stStopTime.tm_year + 1900;
		stFileInfo.stEndTime.dwMonth    = stStopTime.tm_mon + 1;
		stFileInfo.stEndTime.dwDay      = stStopTime.tm_mday;
		stFileInfo.stEndTime.dwHour     = stStopTime.tm_hour;
		stFileInfo.stEndTime.dwMinute   = stStopTime.tm_min;
		stFileInfo.stEndTime.dwSecond   = stStopTime.tm_sec;
// 		////¼������
// 		stFileInfo.nFileType = stInfo.record_type;
		////¼���С
		stFileInfo.dwFileSize =  seg_list->segment_list[i].size;

		CString str;
		str.Format("objectcloud_%02u%02u%02u-%02u%02u%02u", 
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
	FreeStreamTimeSegmentListEx(m_hHCSClient, &seg_list);
	return TRUE;
}

/** @fn PlaybackTime
*   @brief ʱ��ط�
*   @param[in] stPbTimeInfo: ʱ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
BOOL CPBObjectCloudStream::PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo)
{
	m_stPlaybackTimeInfo = stPbTimeInfo;
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}
	// ��ʼʱ��.
	CTime ctBegin(stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond);

	// ����ʱ��.
	CTime ctEnd(stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

    m_iStartTime = ctBegin.GetTime();
	m_iStopTime  = ctEnd.GetTime();

	m_hPlayHwnd = stPbTimeInfo.hWnd;
	//m_pDrawFunc = (VIDEODRAWFUNC)stPbTimeInfo.pDrawFunc;  //���滭ͼ����
	//m_nDrawUserData = stPbTimeInfo.nUser;     //�û�����

	if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
		return FALSE;
	}

	PLAYBACKSDK_INFO("ObjectCloudStream-ʱ��طŲ���. m_hHCSClient=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_hHCSClient, stPbTimeInfo.stStartTime.dwYear, stPbTimeInfo.stStartTime.dwMonth, stPbTimeInfo.stStartTime.dwDay,
		stPbTimeInfo.stStartTime.dwHour, stPbTimeInfo.stStartTime.dwMinute, stPbTimeInfo.stStartTime.dwSecond, stPbTimeInfo.stStopTime.dwYear, stPbTimeInfo.stStopTime.dwMonth, stPbTimeInfo.stStopTime.dwDay,
		stPbTimeInfo.stStopTime.dwHour, stPbTimeInfo.stStopTime.dwMinute, stPbTimeInfo.stStopTime.dwSecond);

	//�طţ����ھ���趨ΪNULL
	if (!StartData(m_hHCSClient, m_strBucket.GetBuffer(0), m_strObjectKey.GetBuffer(0), m_iStartTime, m_iStopTime))
	{
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		/*PLAYBACKSDK_ERROR("ObjectCloudStream-StartData false��m_hHCSClient:%ld, m_strBucket:%s, m_strObjectKey:%s", 
			m_hHCSClient,
			m_strBucket.GetBuffer(0),
			m_strObjectKey.GetBuffer(0));*/
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartData fail��m_hHCSClient:%ld", m_hHCSClient);
		return FALSE;
	}

	//����������߳�
	if (!StartStreamThread(TRUE))
	{
		StopData();
		GPAPI_FreePort(m_emPlayer, m_lPort);
		m_lPort = -1;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartStreamThread fail, m_hHCSClient=%ld", (LONG)m_hHCSClient);
		return FALSE;
	}

	PLAYBACKSDK_INFO("ObjectCloudStream-ʱ��طųɹ�. m_hHCSClient=%ld", (LONG)m_hHCSClient);

	return TRUE;
}

/** @fn StopPlayback
*   @brief ֹͣ�ط�
*   @param NULL
*   @param NULL
*   @return NULL
*/
void CPBObjectCloudStream::StopPlayback()
{
    PLAYBACKSDK_INFO("StopPlayback in");
	// ֹͣȡ��.
	StopData();

	//ֹͣ����
	StopPlay();
	
	//�ر��߳�
	StopStreamThread(TRUE);
	
	PLAYBACKSDK_INFO("ObjectCloudStream-ֹͣ�ط�. m_hHCSClient=%ld", m_hHCSClient);

	CPlayStream::StopPlayback();

	PLAYBACKSDK_INFO("StopPlayback out");
}

/** @fn PB_DownloadTime
*   @brief ʱ������
*   @param[in] stDownloadParam: ������Ϣ
*   @param NULL
*   @return ���ؾ����NULL��ʾ����ʧ��
*/
BOOL CPBObjectCloudStream::DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}
	// ��ʼʱ��.
	CTime ctBegin(stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
		stDownloadParam.stStartTime.dwHour, stDownloadParam.stStartTime.dwMinute, stDownloadParam.stStartTime.dwSecond);

	// ����ʱ��.
	CTime ctEnd(stDownloadParam.stStopTime.dwYear, stDownloadParam.stStopTime.dwMonth, stDownloadParam.stStopTime.dwDay,
		stDownloadParam.stStopTime.dwHour, stDownloadParam.stStopTime.dwMinute, stDownloadParam.stStopTime.dwSecond);

	m_iStartTime = ctBegin.GetTime();
	m_iStopTime = ctEnd.GetTime();

	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-Create_File fail, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = 0;
	m_nDownloadPos = 0;
	PLAYBACKSDK_INFO("ObjectCloudStream-ʱ�����ز���. m_hHCSClient=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_hHCSClient, stDownloadParam.stStartTime.dwYear, stDownloadParam.stStartTime.dwMonth, stDownloadParam.stStartTime.dwDay,
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

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartStreamThread fail, m_hHCSClient=%ld", m_hHCSClient);
		return FALSE;
	}

	//��ʼȡ��ʱ��
	m_lDownloadLastTime = (LONG)CTime::GetCurrentTime().GetTime();

	return TRUE;
}

/** @fn PlaybackFile
*   @brief �ļ��ط�
*   @param[in] stPbFileInfo: �ļ��طŲ���
*   @param NULL
*   @return TRUE�ɹ� FALSEʧ��
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
	//m_pDrawFunc = (VIDEODRAWFUNC)stPbFileInfo.pDrawFunc;  //���滭ͼ����
	//m_nDrawUserData = stPbFileInfo.nUser;     //�û�����

	//if (!GPAPI_GetPort(m_emPlayer, m_lPort))
	//{
	//	PLAYBACKSDK_ERROR("CvrStream-GPAPI_GetPort falid, player: %d", (int)m_emPlayer);
	//	return FALSE;
	//}

	//PLAYBACKSDK_INFO("CvrStream-ʱ��طŲ���. Login=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_lLogin, stPbFileInfo.stFileInfo.stStartTime.dwYear, stPbFileInfo.stFileInfo.stStartTime.dwMonth, stPbFileInfo.stFileInfo.stStartTime.dwDay,
	//	stPbFileInfo.stFileInfo.stStartTime.dwHour, stPbFileInfo.stFileInfo.stStartTime.dwMinute, stPbFileInfo.stFileInfo.stStartTime.dwSecond, stPbFileInfo.stFileInfo.stEndTime.dwYear, stPbFileInfo.stFileInfo.stEndTime.dwMonth, stPbFileInfo.stFileInfo.stEndTime.dwDay,
	//	stPbFileInfo.stFileInfo.stEndTime.dwHour, stPbFileInfo.stFileInfo.stEndTime.dwMinute, stPbFileInfo.stFileInfo.stEndTime.dwSecond);

	////�طţ����ھ���趨ΪNULL
	//if (!StartData(m_hHCSClient, stPbFileInfo.szStreamID, stNetStartTime, stNetStopTime, stPbFileInfo.bConvertPlay
	//	, stPbFileInfo.stConvertParam.byResolution, stPbFileInfo.stConvertParam.dwVideoBitrate, stPbFileInfo.stConvertParam.dwVideoFrameRate))
	//{
	//	GPAPI_FreePort(m_emPlayer, m_lPort);
	//	m_lPort = -1;
	//       PLAYBACKSDK_ERROR("CvrStream-StartData false��lLogin:%ld, ErrorCode = %ld, ErrMsg:%s", m_lLogin, NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
	//	return FALSE;
	//}

	////����������߳�
	//if (!StartStreamThread(TRUE))
	//{
	//	StopData();
	//	GPAPI_FreePort(m_emPlayer, m_lPort);
	//	m_lPort = -1;
	//	PLAYBACKSDK_ERROR("CvrStream-StartStreamThread falid, lLogin=%ld", m_lLogin);
	//	return FALSE;
	//}

	//PLAYBACKSDK_INFO("CvrStream-ʱ��طųɹ�. lLogin=%ld", m_lLogin);

	return TRUE;
}

/** @fn PB_DownloadFile
*   @brief �ļ�����
*   @param[in] stDownloadParam: ���ز���
*   @param NULL
*   @return ���ؾ����NULL��ʾ����ʧ��
*/
BOOL CPBObjectCloudStream::DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}
	// ��ʼʱ��.
	CTime ctBegin(stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
		stDownloadParam.stFileInfo.stStartTime.dwHour, stDownloadParam.stFileInfo.stStartTime.dwMinute, stDownloadParam.stFileInfo.stStartTime.dwSecond);

	// ����ʱ��.
	CTime ctEnd(stDownloadParam.stFileInfo.stEndTime.dwYear, stDownloadParam.stFileInfo.stEndTime.dwMonth, stDownloadParam.stFileInfo.stEndTime.dwDay,
		stDownloadParam.stFileInfo.stEndTime.dwHour, stDownloadParam.stFileInfo.stEndTime.dwMinute, stDownloadParam.stFileInfo.stEndTime.dwSecond);

	m_iStartTime = ctBegin.GetTime();
	m_iStopTime = ctEnd.GetTime();

	m_bDownload = TRUE;
	m_hDownloadFile = Create_File(stDownloadParam.szDownloadFile);
	if (m_hDownloadFile == NULL)
	{
		PLAYBACKSDK_ERROR("ObjectCloudStream-Create_File falid, �ļ���: %s", stDownloadParam.szDownloadFile);
		return FALSE;
	}
	m_dwCurDownSize = 0;
	m_dwTotleSize = stDownloadParam.stFileInfo.dwFileSize;
	m_nDownloadPos = 0;
	//ȡ�ļ���
	PLAYBACKSDK_INFO("ObjectCloudStream-�ļ����ز���. m_hHCSClient=%ld, ʱ��:start=%04d-%02d-%02d %02d:%02d:%02d  stop=%04d-%02d-%02d %02d:%02d:%02d", m_hHCSClient, stDownloadParam.stFileInfo.stStartTime.dwYear, stDownloadParam.stFileInfo.stStartTime.dwMonth, stDownloadParam.stFileInfo.stStartTime.dwDay,
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

	//����������߳�
	if (!StartStreamThread(FALSE))
	{
		StopData();
		CloseHandle(m_hDownloadFile);
		m_hDownloadFile = NULL;
		PLAYBACKSDK_ERROR("ObjectCloudStream-StartStreamThread falid, m_hHCSClient=%ld", m_hHCSClient);
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
void CPBObjectCloudStream::StopDownload()
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

		PLAYBACKSDK_INFO("ObjectCloudStream-ֹͣ����. m_hHCSClient=%ld", m_hHCSClient);
	}
}

/**   @fn          StartData
 *    @brief   	   ��ʼȡ����
 *    @param[in]   pClient:�����Ƴ�ʼ�����.
 *    @param[in]   pBucket:��Դ��.
 *	  @param[in]   pObjectKey:�ļ�key.
 *    @param[in]   iStartTime:�ļ���ʼʱ��.
 *    @param[in]   iStopTime:�ļ�����ʱ��.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBObjectCloudStream::StartData(HCSClient* pClient, const char* pBucket, const char* pObjectKey, const time_t iStartTime, const time_t iStopTime)
{
	if (NULL == m_hHCSClient)
	{
		return FALSE;
	}

	//����ڻط��У�ͣ��
	StopData();
	m_bBlock = TRUE;
	m_bStopDataCb = FALSE;  //��ʼȡ��������
    //ȡ�ļ�ͷ
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
    
	//ȡ¼������
	m_hGetObjectCloudStreamThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFuncGetObjectCloudStream), this, 0, NULL);
	if (NULL == m_hGetObjectCloudStreamThread)
	{
		PLAYBACKSDK_ERROR("GetObjectCloudStreamThread create fail");
		return FALSE;
	}

	return TRUE;
}

/**   @fn          StopData
 *    @brief   	   ֹͣȡ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBObjectCloudStream::StopData()
{
	PLAYBACKSDK_INFO("StopData start");
	// ֹͣ���ݻص�����.
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
		PLAYBACKSDK_ERROR("ObjectCloudStream-DownloadStreamData fail�� ErrorCode = %d", result);
		return;
	}
	PLAYBACKSDK_INFO("GetObjectCloudStreamData end");
}



