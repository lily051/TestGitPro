//lint -library
#include "stdafx.h"
#include "PlaybackSimpleAPI.h"
#include "PlaybackSimpleDll.h"
#include "PlaybackMain.h"
#include <stack>

// ��ʼ����־λ,ֻ��ʼ�����ͷ�һ��,�����ڴ�ӳ�������γ�ʼ�����ͷ�����������.
static int g_nInitCount = 0;
// �Ի��򴴽���.
CRITICAL_SECTION g_csDlgLock;
// �����������طŴ�����.
CPlaybackMain* g_pDlgArray[256] = {0};
// GDI.
ULONG_PTR g_gdiplusToken = 0;	

/**   @fn          CheckDlgLegal
 *    @brief   	   ���Ի����Ƿ�Ϸ�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   nFreeSlot:�����±�.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   		������Դ.
 *    @param[in]    emSkinType:Ƥ������.
 *    @param[in]   
 *    @return       TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL LoadResource(EnumSkinType emSkinType)
{
	theApp.m_nSkinType = emSkinType;

	if (theApp.m_hResInstance != NULL)
	{
		return TRUE;
	}

	// ��ȡ��ԴĿ¼.
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
		PLAYBACKDLL_ERROR(_T("������Դʧ��, ·��:%s"), strDllPath);
		return FALSE;
	}
	else
	{
		PLAYBACKDLL_INFO(_T("������Դ�ɹ�,·��:%s"), strDllPath);
	}

	// ��ȡ��Դ���.
	theApp.m_hOldInstance = AfxGetResourceHandle();
	// ������Դ���.
	AfxSetResourceHandle(theApp.m_hResInstance);

	return TRUE;
}

/** @fn OnMsgTransform
*   @brief ������Ϣ�����践��
*   @param[in] msgSrc: ������Ϣ
*   @param[in] nCmdID: ������Ϣ������
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
	// ��ʼ������SDK,vrm.
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
	//ץͼȨ��
	if (stRights.top() == 48)
	{
		theApp.m_stRights.bCapture = FALSE;
	}
	else
	{
		theApp.m_stRights.bCapture = TRUE;
	}
	stRights.pop();
	//����Ȩ��
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
 *    @brief   	   ��ʼ��.
 *    @param[in]   emSkinType:Ƥ������.
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_Init(PBSDSkinType emSkinType /*= PBSD_SKIN_RED*/, DWORD dwStyle /*= 0*/, const CString csRights)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (g_nInitCount <= 0)
	{
		// ��ʼ��GDI.
		GdiplusStartupInput gdiplusStartupInput;
		Status st = GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
		ASSERT(st == Gdiplus::Ok);


		// ��ȡ����IP��ַ.
		AppFun_GetLocalIPAddr(theApp.m_strLocalIPAddr);

		PLAYBACKDLL_INFO(_T("Local ip addr:%s"), theApp.m_strLocalIPAddr);

		CString strConfigFile;
		AppFun_GetAppPath(strConfigFile);
		theApp.m_strModulePath = strConfigFile;
		//��־
		CString strPath = theApp.m_strModulePath + _T("LogPlaybackSimpleDLL");
		char szBuffer[1024];
		WCharToMByte(strPath, szBuffer, 1024);

		// ��ʼ��������SDK.
		Init();

		// ������Դ.
		if (!LoadResource((EnumSkinType)emSkinType))
		{
			return FALSE;
		}

		// ������.
		theApp.m_dwStyle = dwStyle;

		// ����Ȩ��
		ParseRights(csRights);

		InitializeCriticalSection(&g_csDlgLock);	
	}

	g_nInitCount++;

	return TRUE;
}

/**   @fn          PBSD_UnInit
 *    @brief   	   ����ʼ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
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

	// �ͷ�GDI+��Դ.
	GdiplusShutdown(g_gdiplusToken);

	return TRUE;
}

/**   @fn          PBSD_Create
 *    @brief   	   ��������.
 *    @param[in]   hWnd:������.
 *    @param[in]   emLayout:����ָ�ģʽ.
 *    @return      HANDLE:�طŴ���Ψһ��ʶ,NULL��ʾʧ��.
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
			PLAYBACKDLL_ERROR(_T("�ﵽ���Ի�����,nCount:%d, nFreeDlg:%d"), nCount, nFreeDlg);
			return NULL;
		}
		else
		{
			theApp.m_emLayoutStyle = (PLAY_LAYOUT_WND)emLayout;
			//�½����
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
 *    @brief   	   ���ٴ���.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
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
 *    @brief   	   �ƶ�����.
 *    @param[in]   pRect:���ڴ�С.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @return      TRUE:�ɹ���FALSE:ʧ��.
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
 *    @brief   	   ��ʾ���ش���.
 *    @param[in]   bShow:TRUE-��ʾ,FALSE-ʧ��.
 *    @param[in]   PBSD_Create�ķ���ֵ.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   	   ��ָ�����ڽ��лط�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pLoginParam:��¼����.
 *	  @param[in]   nWndNo:���ں�.
 *    @param[in]   pPlayParam:�طŲ���.
 *    @param[in]   dwRight:Ȩ�޿���.
 *    @return      
 */
PBSD_API BOOL PBSD_PlaybackByWndNo(HANDLE hDlg, int nWndNo, 
								   LPPBSDLoginParam pLoginParam, 
								   LPPBSDPlayParam pPlayParam, 
								   DWORD dwRight /*= PBSD_RIGHT_ALL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//Begin ����GPAPI ������ʱ������ʱ����ȡ���Ĺ���·��ΪIE������·�����¼��ز��ſ�ʧ��ҳ�����,�����������ù���·��
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
 *    @brief   	   ���ػط�.
 *    @param[in]   nVideoType:¼�����ͣ�0-����
 *    @param[in]   csFilePath:�����ļ�·��.
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
 *    @brief   	   �ڹ��ѡ�еĴ��ڽ��лط�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pLoginParam:��¼����.
 *    @param[in]   pPlayParam:�طŲ���.
 *    @param[in]   dwRight:Ȩ�޿���.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_PlaybackOnSelWnd(HANDLE hDlg, 
									LPPBSDLoginParam pLoginParam, 
									LPPBSDPlayParam pPlayParam, 
									DWORD dwRight /*= PBSD_RIGHT_ALL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//Begin ����GPAPI ������ʱ������ʱ����ȡ���Ĺ���·��ΪIE������·�����¼��ز��ſ�ʧ��ҳ�����,�����������ù���·��
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
 *    @brief   	   ָֹͣ���ĻطŴ���.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   nWndNo:���ں�.
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
 *    @brief   	   ��ȡ�ط�״̬��Ϣ.	
 *    @param[in]   hDlg:PBSD_Create����ֵ.
 *    @param[in]   nWndNo:���ں�.
 *    @param[out]  stData:״̬��Ϣ.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   	   ֹͣȫ������.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
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
 *    @brief   	   ���ò˵��ص�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pfnMenuCB:�ص�����.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   	   ����д������־�ص�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pfnWorkLogCB:�ص�����.
 *    @param[in]   pUserData:�û�����.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   	   ���ûطŻص�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pfnPlaybackCB:�ص�����.
 *    @param[in]   pUserData:�û�����.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   	   ���ò���.
 *    @param[in]   stParam:��������
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_SetParameter(const PLAYBACK_PARAM& stParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	memcpy_s(&theApp.m_stPlaybackParam, sizeof(PLAYBACK_PARAM), &stParam, sizeof(PLAYBACK_PARAM));

	return TRUE;
}

/**   @fn          PBSD_SetIEProtected
 *    @brief   	   ����IE����ģʽ,������ҳOCX.
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
 *    @brief   	   ����IE���ô������ģʽ.
 *    @param[in]   stUserData:�û���Ϣ.
 *    @param[in]   stCmsData:CMS����.
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
 *    @brief   	   ˢ�´�����Ϣ.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
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
