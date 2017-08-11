#include "stdafx.h"
#include "./PlaybackSDKApi.h"
#include "./PlaybackBase.h"
#include "./PlayStream.h"
#include "IFactoryProx.h"
#include <map>
#define MAX_OBJECT_COUNT 2048
//lint -e438

// 管理回放对象的全局表.
static std::map<LONG, CPlaybackBase*> g_mapPBObject;
// 管理回放对象的全局计数变量.
static LONG g_lObjCount = 1;
// 全局对象锁.
static CRITICAL_SECTION g_csObjLock;
// 全局初始化计数.
static int g_nInitCount = 0;
// 全局错误码.
static DWORD g_dwLastError = PB_NOERROR;
// 错误码锁.
static CRITICAL_SECTION g_csErrorLock;



/** @fn PB_InitSDK
*   @brief 初始化
*   @param[in] emPlayType: 回放方式
*   @param[in] emDeviceType: 设备类型
*   @param[in] pUserData: 用户数据
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_InitSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType, void * pUserData)
{
	CIFactory* pFac = NULL;
	pFac = CIFactoryProx::CreateObject(emDeviceType, emPlayType, pUserData);

	if (NULL == pFac)
	{
		PLAYBACKSDK_ERROR("PB_InitSDK 创建对象失败, emDeviceType:%d, emPlayType:%d",
			emDeviceType, emPlayType);
		return FALSE;
	}

	BOOL bRet = pFac->InitSdk(emDeviceType, emPlayType, pUserData);

	// 初始化对象锁.
	if (g_nInitCount <= 0)
	{
		CString strDllPath;
		AppFun_GetDllPath(strDllPath);
		strDllPath = strDllPath + _T("LogPlaybackSDK");
		//日志
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
*   @brief 释放sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
PLAYBACKSDK_API void PB_ReleaseSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType)
{
	// 创建对象释放Sdk.
	CIFactory* pFac = NULL;
	pFac = CIFactoryProx::CreateObject(emDeviceType, emPlayType, NULL);

	if (NULL == pFac)
	{
		PLAYBACKSDK_ERROR("PB_ReleaseSDK 创建对象失败, emDeviceType:%d, emPlayType:%d",
			emDeviceType, emPlayType);
		return;
	}
	pFac->ReleaseSdk();
	pFac = NULL;

	// 释放对象锁.
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
 *    @brief   	   获取回放句柄.
 *    @param[in]   stLoginInfo:回放信息引用.
 *    @param[in]   lChannel:通道号.
 *    @return      回放对象指针.
 */
CPlaybackBase * GetPlayObject(PB_LOGININFO& stLoginInfo, LONG lChannel)
{
	CPlaybackBase * pPlayback = NULL;

	// 创建工厂对象.
	CIFactory* pFac = NULL;
	pFac = CIFactoryProx::CreateObject(stLoginInfo.emDevType, stLoginInfo.emPlayType, NULL);

	if (NULL == pFac)
	{
		PLAYBACKSDK_ERROR("GetPlayHandle 创建对象失败, emDeviceType:%d, emPlayType:%d",
			stLoginInfo.emDevType, stLoginInfo.emPlayType);
		return NULL;
	}

	try
	{
		// 创建具体Playback对象.
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
		PLAYBACKSDK_ERROR("CreatePBBase 异常错误, emDeviceType:%d, emPlayType:%d",
			stLoginInfo.emDevType, stLoginInfo.emPlayType);
		return NULL;
	}

	return pPlayback;
}

/**   @fn          SetPlayHandle
 *    @brief   	   设置回放句柄.
 *    @param[in]   pBase:回放对象指针.
 *    @param[in]   
 *    @return      回放对象计数.
 */
HANDLE SetPlayHandle(CPlaybackBase* pBase)
{
	if (g_nInitCount <= 0)
	{
		return NULL;
	}

	CGuard guard(&g_csObjLock);

	// 保存数据.
	pair<std::map<LONG, CPlaybackBase*>::iterator, BOOL> ret;
	ret = g_mapPBObject.insert(pair<LONG, CPlaybackBase*>(g_lObjCount, pBase));
	// 添加数据失败.
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
 *    @brief   	   获取回放对象.
 *    @param[in]   lHandle:回放句柄.
 *    @param[in]   bErase:是否清空数据.
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
		PLAYBACKSDK_ERROR("g_mapPBObject 未找到");
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
*   @brief 开始时间回放
*   @param[in] stLoginInfo: 设备登录信息
*   @param[in] stPlaybackParam: 回放参数
*   @return 回放句柄
*/
PLAYBACKSDK_API HANDLE PB_PlaybackTime(PB_LOGININFO& stLoginInfo, PLAYBACK_TIME_INFO& stPlaybackParam)
{
	CPlaybackBase * pPlayback = GetPlayObject(stLoginInfo, stPlaybackParam.lChannel);
	if (NULL == pPlayback)
	{
		return NULL;
	}

	// 重置错误.
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
	
	// 设置回放句柄.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	// 设置失败.
	if (NULL == hHandle)
	{
		//PB_StopPlayback(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_PlaybackFile
*   @brief 开始文件回放
*   @param[in] stLoginInfo: 设备登录信息
			   stPlaybackParam: 文件回放参数
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

	// 重置错误.
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

	// 设置回放句柄.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	// 设置失败.
	if (NULL == hHandle)
	{
		//PB_StopPlayback(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_LocatePlayTime
*   @brief 定位回放绝对时间
*   @param[in]  hPlayback:  回放句柄
*	@param[in]	stStartTime: 定位时间
*   @parma[in]  stStopTime:停止时间.
*   @param[out] NULL
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_LocatePlayTime(HANDLE hPlayback, const PB_TIME& stStartTime, const PB_TIME& stStopTime)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
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
*   @brief 停止回放
*   @param[in] hPlayback: 回放句柄
*   @param NULL
*   @return TRUE:成功,FALSE:失败.
*/
PLAYBACKSDK_API BOOL PB_StopPlayback(HANDLE& hPlayback)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback, TRUE);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	//停止回放
	pPlayback->StopPlayback();
	pPlayback->Logout();
	delete pPlayback;
	pPlayback = NULL;

	return TRUE;
}

/** @fn PB_FindFile
*   @brief 查找文件
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_FindFile(PB_LOGININFO& stLoginInfo, FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO & lpFileList, int & nFileCount)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stFindFileParam.lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
	PB_SetLastError(PB_NOERROR);

	// 是否支持按卡号查询.
	if (!pPlayback->IsCardNumAllowed()  &&  stFindFileParam.dwUseCardNo > 0)
	{
		PB_SetLastError(PB_UNSUPPORT_CARDNUM);
		delete pPlayback;
		pPlayback = NULL;
		return FALSE;
	}

    // 是否支持按事件查询.
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
*   @brief 查找文件
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_SmartSearch(PB_LOGININFO& stLoginInfo, SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO & lpFileList, int & nFileCount)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stSmartSearchParam.lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
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
*   @brief 回放控制
*   @param[in] hPlayback: 回放句柄
*   @param[in] emControlType: 控制类型
*   @param[in] lValueIn: 输入参数
*   @param[out] lValueOut: 输出参数
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_PlaybackControl(HANDLE hPlayback, ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
	PB_SetLastError(PB_NOERROR);

	return pPlayback->PlaybackControl(emControlType, lValueIn, lValueOut);
}

/** @fn PB_GetOsdTime
*   @brief 获取OSD时间
*   @param[in] hPlayback: 回放句柄
*   @param[out] lOsdTime: OSD时间
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_GetOsdTime(HANDLE hPlayback, LONG & lOsdTime)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
	PB_SetLastError(PB_NOERROR);

	return pPlayback->GetOsdTime(lOsdTime);
}

/** @fn PB_GetPlayPos
*   @brief 获取回放进度
*   @param[in] hPlayback: 回放句柄
*   @param[out] lPos: 进度
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_GetPlayPos(HANDLE hPlayback, LONG & lPos)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
	PB_SetLastError(PB_NOERROR);

	return pPlayback->GetPlayPos(lPos);
}

/** @fn PB_Capture
*   @brief 抓图
*   @param[in] hPlayback: 回放句柄
*   @param[in] szPicFile: 文件存储地址
*   @param[in] emPicType: 抓图类型
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_Capture(HANDLE hPlayback, char * szPicFile, ENUM_PIC_TYPE emPicType)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
	PB_SetLastError(PB_NOERROR);

	MakeSureDirectoryPathExists(szPicFile);

	return pPlayback->PlaybackCapture(szPicFile, emPicType);
}

/** @fn PB_SaveData
*   @brief 录像剪辑
*   @param[in] hPlayback: 回放句柄
*   @param[in] szFile: 文件存储地址
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_SaveData(HANDLE hPlayback, char * szPicFile)
{
	CPlaybackBase * pPlayback = NULL;
	pPlayback = GetPlayHandle(hPlayback);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
	PB_SetLastError(PB_NOERROR);

	MakeSureDirectoryPathExists(szPicFile);

	return pPlayback->SaveData(szPicFile);
}

/** @fn PB_StopSave
*   @brief 停止剪辑
*   @param[in] hPlayback: 回放句柄
*   @param NULL
*   @return TRUE:成功,FALSE:失败.
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
*   @brief 文件下载
*   @param[in] stLoginInfo: 登录信息
*   @param[in] stDownloadParam: 下载参数
*   @return 下载句柄，NULL表示下载失败
*/
PLAYBACKSDK_API HANDLE PB_DownloadFile(PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stDownloadParam.lChannel);
	if (NULL == pPlayback)
	{
		return NULL;
	}

	// 重置错误.
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

	// 设置句柄.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	if (NULL == hHandle)
	{
		PB_StopDownload(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_DownloadTime
*   @brief 时间下载
*   @param[in] stLoginInfo: 登录信息
*   @param[in] stDownloadParam: 下载信息
*   @return 下载句柄，NULL表示下载失败
*/
PLAYBACKSDK_API HANDLE PB_DownloadTime(PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, stDownloadParam.lChannel);
	if (NULL == pPlayback)
	{
		return NULL;
	}

	// 重置错误.
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

	// 设置句柄.
	HANDLE hHandle = SetPlayHandle(pPlayback);
	if (NULL == hHandle)
	{
		PB_StopDownload(hHandle);
		return NULL;
	}

	return hHandle;
}

/** @fn PB_StopDownload
*   @brief 停止下载
*   @param[in] hDownload: 下载句柄
*   @param NULL
*   @return TRUE:成功,FALSE:失败.
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
*   @brief 获取下载进度
*   @param[in] hDownload: 下载句柄
*   @param[out] nPos: 进度
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_GetDownloadPos(HANDLE hDownload, int & nPos)
{
	CPlaybackBase * pDownload = NULL;
	pDownload = GetPlayHandle(hDownload);
	if (NULL == pDownload)
	{
		return FALSE;
	}

	// 重置错误.
	PB_SetLastError(PB_NOERROR);

	return pDownload->GetDownloadPos(nPos);
}


/**   @fn          PB_GetLastError
 *    @brief   	   获取错误码.
 *    @param[in]   
 *    @param[in]   
 *    @return      错误码.
 */
PLAYBACKSDK_API DWORD PB_GetLastError()
{
	CGuard guard(&g_csErrorLock);
	return g_dwLastError;
}

/**   @fn          PB_SetLastError
 *    @brief   	   设置全局错误码.
 *    @param[in]   dwLastError:错误码.
 *    @param[in]   
 *    @return      
 */
void PB_SetLastError(DWORD dwLastError)
{
	CGuard guard(&g_csErrorLock);
	g_dwLastError = dwLastError;
}

/**   @fn          PB_GetStreamData
 *    @brief   	   获取回放流数据.
 *    @param[in]   hPlayback:回放句柄.
 *    @param[in]   pStreamData:取流回调函数.
 *    @param[in]   dwUser:用户自定义数据.
 *    @return      TRUE-成功,FALSE-失败.
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
 *    @brief   	   停止获取回放流数据.
 *    @param[in]   hPlayback:回放句柄.
 *    @return      TRUE-成功,FALSE-失败.
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
 *    @brief   	   获取dvr压缩参数能力集.
 *    @param[in]   stLoginInfo: 登录信息
 *    @param[in]   lChannel: 通道
 *    @param[out]  stCompresscfgAbility: 压缩参数能力集结构体
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetCompressCfgAbility(PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutbuf)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
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
 *    @brief   	   获取dvr视频参数，此处主要为了获取视频制式.
 *    @param[in]   stLoginInfo: 登录信息
 *    @param[in]   lChannel: 通道
 *    @param[out]  nVideoFormat: 视频制式：1-NTSC，2-PAL 
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetDvrConfig(PB_LOGININFO& stLoginInfo, LONG lChannel, int & nVideoFormat)
{
	CPlaybackBase* pPlayback = GetPlayObject(stLoginInfo, lChannel);
	if (NULL == pPlayback)
	{
		return FALSE;
	}

	// 重置错误.
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

	// 重置错误.
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

    // 重置错误.
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

    // 重置错误.
    PB_SetLastError(PB_NOERROR);
    return pPlayback->StopGetKeyFrameData();
}
//lint +e438