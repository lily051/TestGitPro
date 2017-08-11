#include "stdafx.h"
#include "./PlaybackSDKApi.h"
#include "./PlaybackBase.h"
#include "./PlayStream.h"
#include "IFactoryProx.h"
#include <map>
#define MAX_OBJECT_COUNT 2048
//lint -e438

// ����طŶ����ȫ�ֱ�.
static std::map<LONG, CPlaybackBase*> g_mapPBObject;
// ����طŶ����ȫ�ּ�������.
static LONG g_lObjCount = 1;
// ȫ�ֶ�����.
static CRITICAL_SECTION g_csObjLock;
// ȫ�ֳ�ʼ������.
static int g_nInitCount = 0;
// ȫ�ִ�����.
static DWORD g_dwLastError = PB_NOERROR;
// ��������.
static CRITICAL_SECTION g_csErrorLock;



/** @fn PB_InitSDK
*   @brief ��ʼ��
*   @param[in] emPlayType: �طŷ�ʽ
*   @param[in] emDeviceType: �豸����
*   @param[in] pUserData: �û�����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_InitSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType, void * pUserData)
{
	CIFactory* pFac = NULL;
	pFac = CIFactoryProx::CreateObject(emDeviceType, emPlayType, pUserData);

	if (NULL == pFac)
	{
		PLAYBACKSDK_ERROR("PB_InitSDK ��������ʧ��, emDeviceType:%d, emPlayType:%d",
			emDeviceType, emPlayType);
		return FALSE;
	}

	BOOL bRet = pFac->InitSdk(emDeviceType, emPlayType, pUserData);

	// ��ʼ��������.
	if (g_nInitCount <= 0)
	{
		CString strDllPath;
		AppFun_GetDllPath(strDllPath);
		strDllPath = strDllPath + _T("LogPlaybackSDK");
		//��־
		hlog_init(strDllPath.GetBuffer(0));
		strDllPath.ReleaseBuffer();
		InitializeCriticalSection(&g_csObjLock);
		InitializeCriticalSection(&g_csErrorLock);
		PLAYBACKSDK_INFO("PB_InitSDK init lock");
	}
	g_nInitCount++;

	pFac = NULL;
	return bRet;
}

/** @fn PB_ReleaseSDK
*   @brief �ͷ�sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
PLAYBACKSDK_API void PB_ReleaseSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType)
{
	// ���������ͷ�Sdk.
	CIFactory* pFac = NULL;
	pFac = CIFactoryProx::CreateObject(emDeviceType, emPlayType, NULL);

	if (NULL == pFac)
	{
		PLAYBACKSDK_ERROR("PB_ReleaseSDK ��������ʧ��, emDeviceType:%d, emPlayType:%d",
			emDeviceType, emPlayType);
		return;
	}
	pFac->ReleaseSdk();
	pFac = NULL;

	// �ͷŶ�����.
	g_nInitCount--;
	if (g_nInitCount > 0)
	{
		return;
	}
	DeleteCriticalSection(&g_csObjLock);
	DeleteCriticalSection(&g_csErrorLock);
	hlog_fini();
	PLAYBACKSDK_INFO("PB_InitSDK delete lock");
}

/**   @fn          GetPlayHandle
 *    @brief   	   ��ȡ�طž��.
 *    @param[in]   stLoginInfo:�ط���Ϣ����.
 *    @param[in]   lChannel:ͨ����.
 *    @return      �طŶ���ָ��.
 */
CPlaybackBase * GetPlayObject(PB_LOGININFO& stLoginInfo, LONG lChannel)
{
	CPlaybackBase * pPlayback = NULL;

	// ������������.
	CIFactory* pFac = NULL;
	pFac = CIFactoryProx::CreateObject(stLoginInfo.emDevType, stLoginInfo.emPlayType, NULL);

	if (NULL == pFac)
	{
		PLAYBACKSDK_ERROR("GetPlayHandle ��������ʧ��, emDeviceType:%d, emPlayType:%d",
			stLoginInfo.emDevType, stLoginInfo.emPlayType);
		return NULL;
	}

	try
	{
		// ��������Playback����.
		pPlayback = pFac->CreatePBBase();
		pFac = NULL;

		if (NULL == pPlayback)
		{
			PLAYBACKSDK_ERROR("CreatePBBase NULL, emDeviceType:%d, emPlayType:%d",
				stLoginInfo.emDevType, stLoginInfo.emPlayType);
			return NULL;
		}
		pPlayback->m_lChannel = lChannel;
	}
	catch (CMemoryException* )
	{
		PLAYBACKSDK_ERROR("CreatePBBase �쳣����, emDeviceType:%d, emPlayType:%d",
			stLoginInfo.emDevType, stLoginInfo.emPlayType);
		return NULL;
	}

	return pPlayback;
}

/**   @fn          SetPlayHandle
 *    @brief   	   ���ûطž��.
 *    @param[in]   pBase:�طŶ���ָ��.
 *    @param[in]   
 *    @return      �طŶ������.
 */
HANDLE SetPlayHandle(CPlaybackBase* pBase)
{
	if (g_nInitCount <= 0)
	{
		return NULL;
	}

	CGuard guard(&g_csObjLock);

	// ��������.
	pair<std::map<LONG, CPlaybackBase*>::iterator, BOOL> ret;
	ret = g_mapPBObject.insert(pair<LONG, CPlaybackBase*>(g_lObjCount, pBase));
	// �������ʧ��.
	if (!ret.second)
	{
		return NULL;
	}

	LONG nTemp = g_lObjCount;
	g_lObjCount++;
	if (g_lObjCount > MAX_OBJECT_COUNT)
	{
		g_lObjCount = 1;
	}

	return (HANDLE)nTemp;
}

/**   @fn          GetPlayHandle
 *    @brief   	   ��ȡ�طŶ���.
 *    @param[in]   lHandle:�طž��.
 *    @param[in]   bErase:�Ƿ��������.
 *    @return
 */
CPlaybackBase* GetPlayHandle(HANDLE hHandle, BOOL bErase = FALSE)
{
	if (g_nInitCount <= 0)
	{
		PLAYBACKSDK_ERROR("g_nInitCount = %d", g_nInitCount);
		return NULL;
	}

	CGuard guard(&g_csObjLock);

	LONG lKey = (LONG)hHandle;
	std::map<LONG, CPlaybackBase*>::iterator ite = g_mapPBObject.find(lKey);
	if (g_mapPBObject.end() == ite)
	{
		PLAYBACKSDK_ERROR("g_mapPBObject δ�ҵ�");
		return NULL;
	}

	CPlaybackBase* pBase = (*ite).second;
	if (bErase)
	{
		g_mapPBObject.erase(ite);
	}

	return pBase;
}

/** @fn PB_PlaybackTime
*   @brief ��ʼʱ��ط�
*   @param[in] stLoginInfo: �豸��¼��Ϣ
*   @param[in] stPlaybackParam: �طŲ���
*   @return �طž��
*/
PLAYBACKSDK_API HANDLE PB_PlaybackTime(PB_LOGININFO& stLoginInfo, PLAYBACK_TIME_INFO& stPlaybackParam)
{
	CPlaybackBase * pPlayback = GetPlayObject(stLoginInfo, stPlaybackParam.lChannel);
	if (NULL == pPlayback)
	{
		return NULL;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->Login(stLoginInfo))
	{
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}

	if (!pPlayback->PlaybackTime(stPlaybackParam))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}
	
	// ���ûطž��.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	// ����ʧ��.
	if (NULL == hHandle)
	{
		//PB_StopPlayback(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_PlaybackFile
*   @brief ��ʼ�ļ��ط�
*   @param[in] stLoginInfo: �豸��¼��Ϣ
			   stPlaybackParam: �ļ��طŲ���
*   @param NULL
*   @return NULL
*/
PLAYBACKSDK_API HANDLE PB_PlaybackFile(PB_LOGININFO& stLoginInfo, PLAYBACK_FILE_INFO& stPlaybackParam)
{
	CPlaybackBase * pPlayback = GetPlayObject(stLoginInfo, stPlaybackParam.lChannel);
	if (NULL == pPlayback)
	{
		return NULL;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->Login(stLoginInfo))
	{
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}

	if (!pPlayback->PlaybackFile(stPlaybackParam))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}

	// ���ûطž��.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	// ����ʧ��.
	if (NULL == hHandle)
	{
		//PB_StopPlayback(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_LocatePlayTime
*   @brief ��λ�طž���ʱ��
*   @param[in]  hPlayback:  �طž��
*	@param[in]	stStartTime: ��λʱ��
*   @parma[in]  stStopTime:ֹͣʱ��.
*   @param[out] NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_LocatePlayTime(HANDLE hPlayback, const PB_TIME& stStartTime, const PB_TIME& stStopTime)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->LocatePlayTime(stStartTime, stStopTime))
	{
		pPlayback->StopPlayback();
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	return TRUE; 
}

/** @fn PB_StopPlayback
*   @brief ֹͣ�ط�
*   @param[in] hPlayback: �طž��
*   @param NULL
*   @return TRUE:�ɹ�,FALSE:ʧ��.
*/
PLAYBACKSDK_API BOOL PB_StopPlayback(HANDLE& hPlayback)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback, TRUE);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	//ֹͣ�ط�
	pPlayback->StopPlayback();
	pPlayback->Logout();
	delete pPlayback;
	pPlayback = NULL;

	return TRUE;
}

/** @fn PB_FindFile
*   @brief �����ļ�
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_FindFile(PB_LOGININFO& stLoginInfo, FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO & lpFileList, int & nFileCount)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stFindFileParam.lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	// �Ƿ�֧�ְ����Ų�ѯ.
	if (!pPlayback->IsCardNumAllowed()  &&  stFindFileParam.dwUseCardNo > 0)
	{
		PB_SetLastError(PB_UNSUPPORT_CARDNUM);
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

    // �Ƿ�֧�ְ��¼���ѯ.
    if (!pPlayback->IsEventAllowed()  &&  stFindFileParam.bByEvent)
    {
        PB_SetLastError(PB_UNSUPPORT_SMARTSEARCH);
        delete pPlayback;
        pPlayback = NULL;
        return FALSE;
    }

	if (!pPlayback->Login(stLoginInfo))
	{
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	if (!pPlayback->FindFile(stFindFileParam, lpFileList, nFileCount))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	pPlayback->Logout();
	delete pPlayback;
	pPlayback = NULL;

	return TRUE;
}

/** @fn PB_FindFile
*   @brief �����ļ�
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_SmartSearch(PB_LOGININFO& stLoginInfo, SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO & lpFileList, int & nFileCount)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stSmartSearchParam.lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->Login(stLoginInfo))
	{
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	if (!pPlayback->SmartSearch(stSmartSearchParam, lpFileList, nFileCount))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	pPlayback->Logout();
	delete pPlayback;
	pPlayback = NULL;

	return TRUE;
}

/** @fn PlaybackControl
*   @brief �طſ���
*   @param[in] hPlayback: �طž��
*   @param[in] emControlType: ��������
*   @param[in] lValueIn: �������
*   @param[out] lValueOut: �������
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_PlaybackControl(HANDLE hPlayback, ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	return pPlayback->PlaybackControl(emControlType, lValueIn, lValueOut);
}

/** @fn PB_GetOsdTime
*   @brief ��ȡOSDʱ��
*   @param[in] hPlayback: �طž��
*   @param[out] lOsdTime: OSDʱ��
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_GetOsdTime(HANDLE hPlayback, LONG & lOsdTime)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	return pPlayback->GetOsdTime(lOsdTime);
}

/** @fn PB_GetPlayPos
*   @brief ��ȡ�طŽ���
*   @param[in] hPlayback: �طž��
*   @param[out] lPos: ����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_GetPlayPos(HANDLE hPlayback, LONG & lPos)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	return pPlayback->GetPlayPos(lPos);
}

/** @fn PB_Capture
*   @brief ץͼ
*   @param[in] hPlayback: �طž��
*   @param[in] szPicFile: �ļ��洢��ַ
*   @param[in] emPicType: ץͼ����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_Capture(HANDLE hPlayback, char * szPicFile, ENUM_PIC_TYPE emPicType)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	MakeSureDirectoryPathExists(szPicFile);

	return pPlayback->PlaybackCapture(szPicFile, emPicType);
}

/** @fn PB_SaveData
*   @brief ¼�����
*   @param[in] hPlayback: �طž��
*   @param[in] szFile: �ļ��洢��ַ
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_SaveData(HANDLE hPlayback, char * szPicFile)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	MakeSureDirectoryPathExists(szPicFile);

	return pPlayback->SaveData(szPicFile);
}

/** @fn PB_StopSave
*   @brief ֹͣ����
*   @param[in] hPlayback: �طž��
*   @param NULL
*   @return TRUE:�ɹ�,FALSE:ʧ��.
*/
PLAYBACKSDK_API BOOL PB_StopSave(HANDLE hPlayback)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	pPlayback->StopSave();

	return TRUE;
}

/** @fn PB_DownloadFile
*   @brief �ļ�����
*   @param[in] stLoginInfo: ��¼��Ϣ
*   @param[in] stDownloadParam: ���ز���
*   @return ���ؾ����NULL��ʾ����ʧ��
*/
PLAYBACKSDK_API HANDLE PB_DownloadFile(PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stDownloadParam.lChannel);
	if (NULL == pPlayback)
	{
		return NULL;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->Login(stLoginInfo))
	{
		PB_SetLastError(PB_LOGIN_FAILD);
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}

	if (!pPlayback->DownloadFile(stDownloadParam))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}

	// ���þ��.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	if (NULL == hHandle)
	{
		PB_StopDownload(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_DownloadTime
*   @brief ʱ������
*   @param[in] stLoginInfo: ��¼��Ϣ
*   @param[in] stDownloadParam: ������Ϣ
*   @return ���ؾ����NULL��ʾ����ʧ��
*/
PLAYBACKSDK_API HANDLE PB_DownloadTime(PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stDownloadParam.lChannel);
	if (NULL == pPlayback)
	{
		return NULL;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->Login(stLoginInfo))
	{
		PB_SetLastError(PB_LOGIN_FAILD);
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}

	if (!pPlayback->DownloadTime(stDownloadParam))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return NULL;
	}

	// ���þ��.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	if (NULL == hHandle)
	{
		PB_StopDownload(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_StopDownload
*   @brief ֹͣ����
*   @param[in] hDownload: ���ؾ��
*   @param NULL
*   @return TRUE:�ɹ�,FALSE:ʧ��.
*/
PLAYBACKSDK_API BOOL PB_StopDownload(HANDLE& hDownload)
{
	CPlaybackBase * pDownload = NULL;
	pDownload = GetPlayHandle(hDownload, TRUE);
	if (NULL == pDownload)
	{
		return FALSE;
	}

	pDownload->StopDownload();
	pDownload->Logout();
	delete pDownload;
	pDownload = NULL;

	return TRUE;
}

/** @fn PB_GetDownloadPos
*   @brief ��ȡ���ؽ���
*   @param[in] hDownload: ���ؾ��
*   @param[out] nPos: ����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_GetDownloadPos(HANDLE hDownload, int & nPos)
{
	CPlaybackBase * pDownload = NULL;
	pDownload = GetPlayHandle(hDownload);
	if (NULL == pDownload)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	return pDownload->GetDownloadPos(nPos);
}


/**   @fn          PB_GetLastError
 *    @brief   	   ��ȡ������.
 *    @param[in]   
 *    @param[in]   
 *    @return      ������.
 */
PLAYBACKSDK_API DWORD PB_GetLastError()
{
	CGuard guard(&g_csErrorLock);
	return g_dwLastError;
}

/**   @fn          PB_SetLastError
 *    @brief   	   ����ȫ�ִ�����.
 *    @param[in]   dwLastError:������.
 *    @param[in]   
 *    @return      
 */
void PB_SetLastError(DWORD dwLastError)
{
	CGuard guard(&g_csErrorLock);
	g_dwLastError = dwLastError;
}

/**   @fn          PB_GetStreamData
 *    @brief   	   ��ȡ�ط�������.
 *    @param[in]   hPlayback:�طž��.
 *    @param[in]   pStreamData:ȡ���ص�����.
 *    @param[in]   dwUser:�û��Զ�������.
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
PLAYBACKSDK_API BOOL PB_StartGetStreamData(HANDLE hPlayback, PbStreamDataCB pStreamData, DWORD dwUser)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	if (NULL == pStreamData)
	{
		return FALSE;
	}
	PLAYBACKSDK_INFO("PB_StartGetStreamData");

	return pPlayback->StartGetStreamData(pStreamData, dwUser);
}

/**   @fn          PB_StopGetStreamData
 *    @brief   	   ֹͣ��ȡ�ط�������.
 *    @param[in]   hPlayback:�طž��.
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
PLAYBACKSDK_API BOOL PB_StopGetStreamData(HANDLE hPlayback)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	PLAYBACKSDK_INFO("PB_StopGetStreamData");
	pPlayback->StopGetStreamData();
	return TRUE;
}

/**   @fn          PB_GetDeviceAbility
 *    @brief   	   ��ȡdvrѹ������������.
 *    @param[in]   stLoginInfo: ��¼��Ϣ
 *    @param[in]   lChannel: ͨ��
 *    @param[out]  stCompresscfgAbility: ѹ�������������ṹ��
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetCompressCfgAbility(PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutbuf)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->Login(stLoginInfo))
	{
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	if (!pPlayback->GetCompressCfgAbility(pOutbuf))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	pPlayback->Logout();
	delete pPlayback;
	pPlayback = NULL;

	return TRUE;
}

/**   @fn          PB_GetDvrConfig
 *    @brief   	   ��ȡdvr��Ƶ�������˴���ҪΪ�˻�ȡ��Ƶ��ʽ.
 *    @param[in]   stLoginInfo: ��¼��Ϣ
 *    @param[in]   lChannel: ͨ��
 *    @param[out]  nVideoFormat: ��Ƶ��ʽ��1-NTSC��2-PAL 
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetDvrConfig(PB_LOGININFO& stLoginInfo, LONG lChannel, int & nVideoFormat)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);

	if (!pPlayback->Login(stLoginInfo))
	{
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	if (!pPlayback->GetVideoFormat(nVideoFormat))
	{
		pPlayback->Logout();
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

	pPlayback->Logout();
	delete pPlayback;
	pPlayback = NULL;

	return TRUE;
}

PLAYBACKSDK_API BOOL PB_GetPictureSize( HANDLE hPlayback, LONG* lWidth, LONG* lHeight )
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// ���ô���.
	PB_SetLastError(PB_NOERROR);
	return pPlayback->GetPictureSize(lWidth, lHeight);
}

PLAYBACKSDK_API BOOL PB_GetKeyFrameData( HANDLE hPlayback,const PB_TIME& stFrameTime, PbKeyFrameDataCB pFunGetKeyFrame, DWORD dwUser )
{
    CPlaybackBase * pPlayback = NULL;
    pPlayback = GetPlayHandle(hPlayback);
    if (NULL == pPlayback)
    {
        return FALSE;
    }

    // ���ô���.
    PB_SetLastError(PB_NOERROR);
    return pPlayback->GetKeyFrameData(stFrameTime, pFunGetKeyFrame, dwUser);
}

PLAYBACKSDK_API BOOL PB_StopGetKeyFrameData( HANDLE hPlayback )
{
    CPlaybackBase * pPlayback = NULL;
    pPlayback = GetPlayHandle(hPlayback);
    if (NULL == pPlayback)
    {
        return FALSE;
    }

    // ���ô���.
    PB_SetLastError(PB_NOERROR);
    return pPlayback->StopGetKeyFrameData();
}
//lint +e438