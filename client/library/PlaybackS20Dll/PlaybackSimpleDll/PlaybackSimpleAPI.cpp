//lint -library
#include "stdafx.h"
#include "PlaybackSimpleAPI.h"
#include "PlaybackSimpleDll.h"
#include "PlaybackMain.h"
#include <stack>

// 初始化标志位,只初始化、释放一次,避免内存映射问题多次初始化和释放造成软件崩溃.
static int g_nInitCount = 0;
// 对话框创建锁.
CRITICAL_SECTION g_csDlgLock;
// 允许创建的最大回放窗口数.
CPlaybackMain* g_pDlgArray[256] = {0};
// GDI.
ULONG_PTR g_gdiplusToken = 0;	

/**   @fn          CheckDlgLegal
 *    @brief   	   检查对话框是否合法.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   nFreeSlot:数组下表.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CheckDlgLegal(HANDLE hDlg, int &nFreeSlot)
{
	nFreeSlot = -1;
	CPlaybackMain* pDlg = (CPlaybackMain*)hDlg;

	int nCount = _countof(g_pDlgArray);
	for (int i = 0; i < nCount; i++)
	{
		if (pDlg == g_pDlgArray[i])
		{
			nFreeSlot = i;
			break;
		}
	}

	if (-1 == nFreeSlot)
	{
		return FALSE;
	}

	return TRUE;
}

/**   @fn			LoadResource  
 *    @brief   		加载资源.
 *    @param[in]    emSkinType:皮肤类型.
 *    @param[in]   
 *    @return       TRUE:成功,FALSE:失败.
 */
BOOL LoadResource(EnumSkinType emSkinType)
{
	theApp.m_nSkinType = emSkinType;

	if (theApp.m_hResInstance != NULL)
	{
		return TRUE;
	}

	// 获取资源目录.
	CString strDllPath;
	if (SKIN_RED == emSkinType)
	{
		strDllPath = theApp.m_strModulePath + _T("PBSDSkinRed.dll");
	}
	else
	{
		strDllPath = theApp.m_strModulePath + _T("PBSDSkinBlue.dll");
	}
	theApp.m_hResInstance = LoadLibraryEx(strDllPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (NULL == theApp.m_hResInstance)
	{
		PLAYBACKDLL_ERROR(_T("加载资源失败, 路径:%s"), strDllPath);
		return FALSE;
	}
	else
	{
		PLAYBACKDLL_INFO(_T("加载资源成功,路径:%s"), strDllPath);
	}

	// 获取资源句柄.
	theApp.m_hOldInstance = AfxGetResourceHandle();
	// 设置资源句柄.
	AfxSetResourceHandle(theApp.m_hResInstance);

	return TRUE;
}

/** @fn OnMsgTransform
*   @brief 网络消息，无需返回
*   @param[in] msgSrc: 请求消息
*   @param[in] nCmdID: 请求消息命令码
*   @return NULL
*/
void OnMsgTransform(std::string& sRrc, int nCmdID)
{
	PLAYBACKDLL_INFO(_T("OnMsgTransform nCmdID:%d"), nCmdID);

	FrameMsgData stData;
	stData.sMsg = sRrc;
	stData.nCmdID = nCmdID;
}

void Init()
{
	// 初始化海康SDK,vrm.
   theApp.InitSDK(PB_DEVICE_HIKVISION, PB_TYPE_DVR);
}

void ParseRights(const CString csRights)
{
	int nLength = csRights.GetLength();
	if (0 == nLength)
	{
		return;
	}
	std::stack<int> stRights;
	for(int i=0; i<nLength; i++)
	{
		int pRight = csRights.GetAt(i);
		stRights.push(pRight);
	}
	//抓图权限
	if (stRights.top() == 48)
	{
		theApp.m_stRights.bCapture = FALSE;
	}
	else
	{
		theApp.m_stRights.bCapture = TRUE;
	}
	stRights.pop();
	//剪辑权限
	if (stRights.top() == 48)
	{
		theApp.m_stRights.bMontage = FALSE;
	}
	else
	{
		theApp.m_stRights.bMontage = TRUE;
	}
	stRights.pop();
}
void UnInit()
{
    theApp.UnInitSDK();
}

/**   @fn          PBSD_Init
 *    @brief   	   初始化.
 *    @param[in]   emSkinType:皮肤类型.
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_Init(PBSDSkinType emSkinType /*= PBSD_SKIN_RED*/, DWORD dwStyle /*= 0*/, const CString csRights)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (g_nInitCount <= 0)
	{
		// 初始化GDI.
		GdiplusStartupInput gdiplusStartupInput;
		Status st = GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
		ASSERT(st == Gdiplus::Ok);


		// 获取本地IP地址.
		AppFun_GetLocalIPAddr(theApp.m_strLocalIPAddr);

		PLAYBACKDLL_INFO(_T("Local ip addr:%s"), theApp.m_strLocalIPAddr);

		CString strConfigFile;
		AppFun_GetAppPath(strConfigFile);
		theApp.m_strModulePath = strConfigFile;
		//日志
		CString strPath = theApp.m_strModulePath + _T("LogPlaybackSimpleDLL");
		char szBuffer[1024];
		WCharToMByte(strPath, szBuffer, 1024);

		// 初始化海康大华SDK.
		Init();

		// 加载资源.
		if (!LoadResource((EnumSkinType)emSkinType))
		{
			return FALSE;
		}

		// 组件风格.
		theApp.m_dwStyle = dwStyle;

		// 功能权限
		ParseRights(csRights);

		InitializeCriticalSection(&g_csDlgLock);	
	}

	g_nInitCount++;

	return TRUE;
}

/**   @fn          PBSD_UnInit
 *    @brief   	   反初始化.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
PBSD_API BOOL PBSD_UnInit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_nInitCount--;
	if (g_nInitCount > 0)
	{
		return FALSE;
	}

	UnInit();

	DeleteCriticalSection(&g_csDlgLock);

	// 释放GDI+资源.
	GdiplusShutdown(g_gdiplusToken);

	return TRUE;
}

/**   @fn          PBSD_Create
 *    @brief   	   创建窗口.
 *    @param[in]   hWnd:父窗口.
 *    @param[in]   emLayout:画面分割模式.
 *    @return      HANDLE:回放窗口唯一标识,NULL表示失败.
 */
PBSD_API HANDLE PBSD_Create(HWND hWnd, PBSDLayout emLayout /*= LAYOUT_WND_1*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == hWnd)
	{
		return NULL;
	}
	if (emLayout != LAYOUT_WND_1  
		&&  emLayout != LAYOUT_WND_4  
		&&  emLayout != LAYOUT_WND_9  
		&&  emLayout != LAYOUT_WND_16)
	{
		return NULL;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeDlg = 0;
	try
	{
		CWnd * pParent = (CWnd* )CWnd::FromHandle(hWnd);
		int nCount = _countof(g_pDlgArray);
		for (nFreeDlg = 0; nFreeDlg < nCount; nFreeDlg++)
		{
			if (NULL == g_pDlgArray[nFreeDlg])
			{
				break;
			}
		}
		if (nFreeDlg >= nCount)
		{
			PLAYBACKDLL_ERROR(_T("达到最大对话框数,nCount:%d, nFreeDlg:%d"), nCount, nFreeDlg);
			return NULL;
		}
		else
		{
			theApp.m_emLayoutStyle = (PLAY_LAYOUT_WND)emLayout;
			//新建组件
			g_pDlgArray[nFreeDlg] = new CPlaybackMain();
			g_pDlgArray[nFreeDlg]->Create(CPlaybackMain::IDD, pParent);
			g_pDlgArray[nFreeDlg]->ShowWindow(SW_SHOW);
			theApp.m_pPlaybackMain = g_pDlgArray[nFreeDlg];
		}
	}
	catch (CMemoryException* )
	{
		return NULL;
	}

	return (HANDLE)g_pDlgArray[nFreeDlg];
}

/**   @fn          PBSD_Destroy
 *    @brief   	   销毁窗口.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   
 *    @return      
 */
PBSD_API BOOL PBSD_Destroy(HANDLE hDlg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->DestroyWindow();
		delete g_pDlgArray[nFreeSlot];
		g_pDlgArray[nFreeSlot] = NULL;
	}

	return TRUE;
}

/**   @fn          PBSD_MoveWindow
 *    @brief   	   移动窗口.
 *    @param[in]   pRect:窗口大小.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @return      TRUE:成功，FALSE:失败.
 */
PBSD_API BOOL PBSD_MoveWindow(HANDLE hDlg, LPCRECT pRect)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (NULL == hDlg || NULL == pRect)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->MoveWindow(pRect);
	}

	return TRUE;
}

/**   @fn          PBSD_ShowWindow
 *    @brief   	   显示隐藏窗口.
 *    @param[in]   bShow:TRUE-显示,FALSE-失败.
 *    @param[in]   PBSD_Create的返回值.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_ShowWindow(HANDLE hDlg, BOOL bShow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == hDlg)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		if (bShow)
		{
			g_pDlgArray[nFreeSlot]->ShowWindow(SW_SHOW);
		}
		else
		{
			g_pDlgArray[nFreeSlot]->ShowWindow(SW_HIDE);
		}
	}

	return TRUE;
}

/**   @fn          PBSD_PlaybackByWndNo
 *    @brief   	   在指定窗口进行回放.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pLoginParam:登录参数.
 *	  @param[in]   nWndNo:窗口号.
 *    @param[in]   pPlayParam:回放参数.
 *    @param[in]   dwRight:权限控制.
 *    @return      
 */
PBSD_API BOOL PBSD_PlaybackByWndNo(HANDLE hDlg, int nWndNo, 
								   LPPBSDLoginParam pLoginParam, 
								   LPPBSDPlayParam pPlayParam, 
								   DWORD dwRight /*= PBSD_RIGHT_ALL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//Begin 由于GPAPI 采用延时加载有时，获取到的工作路径为IE的运行路径导致加载播放库失败页面崩溃,所以重新设置工作路径
	CString strPath;
	AppFun_GetAppPath(strPath);
	SetCurrentDirectory(strPath);
	//End
	if (NULL == hDlg)
	{
		return FALSE;
	}
	if (nWndNo < -1  ||  nWndNo >= 16)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}
	theApp.m_lBwControl = pPlayParam->lBwControl;
	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		if (!g_pDlgArray[nFreeSlot]->PlaybackCam(pLoginParam, pPlayParam, nWndNo, dwRight))
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**   @fn          PBSD_LocalPlayback
 *    @brief   	   本地回放.
 *    @param[in]   nVideoType:录像类型，0-海康
 *    @param[in]   csFilePath:本地文件路径.
 *    @return  
 */
PBSD_API BOOL PBSD_LocalPlayback(HANDLE hDlg, const int nVideoType, const CString csFilePath)
{
	if (NULL == hDlg)
	{
		return FALSE;
	}
	CGuard guard(&g_csDlgLock);
	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}
	if (NULL != g_pDlgArray[nFreeSlot])
	{
		if (NULL != g_pDlgArray[nFreeSlot]->LocalPlayback(nVideoType, csFilePath))
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

/**   @fn          PBSD_PlaybackOnSelWnd
 *    @brief   	   在光标选中的窗口进行回放.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pLoginParam:登录参数.
 *    @param[in]   pPlayParam:回放参数.
 *    @param[in]   dwRight:权限控制.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_PlaybackOnSelWnd(HANDLE hDlg, 
									LPPBSDLoginParam pLoginParam, 
									LPPBSDPlayParam pPlayParam, 
									DWORD dwRight /*= PBSD_RIGHT_ALL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//Begin 由于GPAPI 采用延时加载有时，获取到的工作路径为IE的运行路径导致加载播放库失败页面崩溃,所以重新设置工作路径
	CString strPath;
	AppFun_GetAppPath(strPath);
	SetCurrentDirectory(strPath);
	//End
	if (NULL == hDlg)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		if (!g_pDlgArray[nFreeSlot]->PlaybackCam(pLoginParam, pPlayParam, -1, dwRight))
		{
			return FALSE;
		}
	}

	return TRUE;
}

PBSD_API BOOL PBSD_PlaybackByTelNo(HANDLE hDlg,
								   int nWndNo,
								   LPTelNoInfo pLoginParam,
								   LPPBSDTelPlayParam pPlayParam, 
								   DWORD dwRight/* = PBSD_RIGHT_ALL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return TRUE;
}

/**   @fn          PBSD_StopPlayByWndNo
 *    @brief   	   停止指定的回放窗口.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   nWndNo:窗口号.
 *    @return      
 */
PBSD_API BOOL PBSD_StopPlayByWndNo(HANDLE hDlg, int nWndNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == hDlg)
	{
		return FALSE;
	}
	if (nWndNo < -1  ||  nWndNo >= 16)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->StopPlayByWndNo(nWndNo);
	}

	return TRUE;
}

/**   @fn          PBSD_GetPlaybackInfo
 *    @brief   	   获取回放状态信息.	
 *    @param[in]   hDlg:PBSD_Create返回值.
 *    @param[in]   nWndNo:窗口号.
 *    @param[out]  stData:状态信息.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_GetPlaybackInfo(HANDLE hDlg, int nWndNo, PlaybackStatData& stData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == hDlg)
	{
		return FALSE;
	}
	if (nWndNo < -1  ||  nWndNo >= 16)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->GetPlaybackInfo(nWndNo, stData);
	}

	return TRUE;
}

/**   @fn          PBSD_StopAll
 *    @brief   	   停止全部窗口.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
PBSD_API BOOL PBSD_StopAll(HANDLE hDlg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->StopAll();
	}

	return TRUE;
}

/**   @fn          PBSD_SetMenuCallBack
 *    @brief   	   设置菜单回调.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pfnMenuCB:回调函数.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetMenuCallBack(HANDLE hDlg, PBSD_MenuCB pfnMenuCB, LPVOID pUserData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == hDlg  ||  NULL == pfnMenuCB)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->SetMenuCB(pfnMenuCB, pUserData);
	}

	return TRUE;
}

/**   @fn          PBSD_SetWorkLogCallBack
 *    @brief   	   设置写操作日志回调.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pfnWorkLogCB:回调函数.
 *    @param[in]   pUserData:用户数据.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetWorkLogCallBack(HANDLE hDlg, PBSD_WorkLogCB pfnWorkLogCB, LPVOID pUserData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == hDlg ||  NULL == pfnWorkLogCB)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->SetWorkLogCallBack(pfnWorkLogCB, pUserData);
	}

	return TRUE;
}

/**   @fn          PBSD_SetPlaybackCallBack
 *    @brief   	   设置回放回调.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pfnPlaybackCB:回调函数.
 *    @param[in]   pUserData:用户数据.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetPlaybackCallBack(HANDLE hDlg, PBSD_PlaybackCB pfnPlaybackCB, LPVOID pUserData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == hDlg  ||  NULL == pfnPlaybackCB)
	{
		return FALSE;
	}

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return FALSE;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->SetPlaybackCB(pfnPlaybackCB, pUserData);
	}

	return TRUE;
}

/**   @fn          PBSD_SetParameter
 *    @brief   	   设置参数.
 *    @param[in]   stParam:参数引用
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetParameter(const PLAYBACK_PARAM& stParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	memcpy_s(&theApp.m_stPlaybackParam, sizeof(PLAYBACK_PARAM), &stParam, sizeof(PLAYBACK_PARAM));

	return TRUE;
}

/**   @fn          PBSD_SetIEProtected
 *    @brief   	   设置IE保护模式,用于网页OCX.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
PBSD_API void PBSD_SetIEProtected()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	theApp.m_bIsIEProtected = TRUE;
}

/**   @fn          PBSD_SetBwControl
 *    @brief   	   设置IE设置带宽控制模式.
 *    @param[in]   stUserData:用户信息.
 *    @param[in]   stCmsData:CMS数据.
 *    @return      
 */
PBSD_API void PBSD_SetBwControl(const PBSDUserInfo& stUserData, const PBSDCmsData& stCmsData, const PBSDClientData& stClientData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	theApp.m_bBwMode = TRUE;
	memcpy_s(&theApp.m_stDataInfo, sizeof(theApp.m_stDataInfo), &stUserData, sizeof(stUserData));
	memcpy_s(&theApp.m_stCmsInfo, sizeof(theApp.m_stCmsInfo), &stCmsData, sizeof(stCmsData));
	memcpy_s(&theApp.m_stClientData, sizeof(theApp.m_stClientData), &stClientData, sizeof(stClientData));
}	

/**   @fn          PBSD_Invalida
 *    @brief   	   刷新窗口信息.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   
 *    @return      
 */
PBSD_API void PBSD_Invalidate(HANDLE hDlg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CGuard guard(&g_csDlgLock);

	int nFreeSlot = -1;
	if (!CheckDlgLegal(hDlg, nFreeSlot))
	{
		return;
	}

	if (g_pDlgArray[nFreeSlot] != NULL)
	{
		g_pDlgArray[nFreeSlot]->Invalidate();
	}
}

PBSD_API void PDSD_SetPlayBackConfig(const PLAYBACK_CONFIG& stPBConfig)
{
	CString strCapPath, strDownPath;

	wchar_t* wszPath = NULL;
	int nLength = 0;
	CP936_A2W(stPBConfig.stPBParam.szPicPath, strlen(stPBConfig.stPBParam.szPicPath) + 1, wszPath, nLength);
	strCapPath.Format(_T("%s"), wszPath);
	CodeSet_Free(wszPath);
	wszPath = NULL;
	CP936_A2W(stPBConfig.stPBParam.szDownloadPath, strlen(stPBConfig.stPBParam.szDownloadPath) + 1, wszPath, nLength);
	strDownPath.Format(_T("%s"), wszPath);
	CodeSet_Free(wszPath);
	wszPath = NULL;

	if (strCapPath.IsEmpty())
	{
		strCapPath = _T("C:\\iVMS8120\\");
	}

	if (strDownPath.IsEmpty())
	{
		strDownPath = _T("C:\\iVMS8120\\");
	}

	int nPicType = 0;
	if (stPBConfig.stPBParam.nPicType < 0 || stPBConfig.stPBParam.nPicType > 1)
	{
		nPicType = 0;
	}
	else
	{
		nPicType = stPBConfig.stPBParam.nPicType;
	}

	
	strCapPath += _T("PB_Capture\\");
	_snprintf_s(theApp.m_stPlaybackParam.szPicPath, sizeof(theApp.m_stPlaybackParam.szPicPath),
		_TRUNCATE, CStringW2A(strCapPath));
	theApp.m_stPlaybackParam.nPicType = nPicType;
	strDownPath += _T("PB_Record\\");
	_snprintf_s(theApp.m_stPlaybackParam.szDownloadPath, sizeof(theApp.m_stPlaybackParam.szDownloadPath),
		_TRUNCATE, CStringW2A(strDownPath));

	theApp.m_stPlaybackParam.nShowFileWnd = stPBConfig.stPBParam.nShowFileWnd;
	theApp.m_stPlaybackParam.nShowWndTitle = stPBConfig.stPBParam.nShowWndTitle;


	theApp.m_bInitProx = stPBConfig.bInitProx;
	theApp.m_bInitNvr = stPBConfig.bInitNvr;
	theApp.m_bInitBdp = stPBConfig.bInitBdp;
	theApp.m_bInitFinAudio = stPBConfig.bInitFinAudio;
	
}
