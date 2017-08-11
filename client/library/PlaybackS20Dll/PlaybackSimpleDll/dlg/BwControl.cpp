#include "StdAfx.h"
#include "BwControl.h"
#include "../PlaybackSimpleDll.h"

CBwControl::CBwControl(void)
: m_hRegThread(NULL)
, m_hStopReg(NULL)
{
	m_hStopReg = CreateEvent(NULL, TRUE, FALSE, NULL);
	memset(&m_stData, 0, sizeof(RegisThreadData));
}

CBwControl::~CBwControl(void)
{
	if (m_hStopReg != NULL)
	{
		CloseHandle(m_hStopReg);
		m_hStopReg = NULL;
	}
}

/**   @fn          RegistBwServer
 *    @brief   	   注册带宽控制服务器.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:注册成功,FALSE:注册失败.
 */
BOOL CBwControl::RegistBwServer(BwRegistMsgCB pfnBwFunc, LPVOID pUserData)
{
	// 带宽控制服务器正在注册中.
	if (m_hRegThread != NULL)
	{
		PLAYBACKDLL_INFO(_T("Registing the bw server"));
		return FALSE;
	}

	ResetEvent(m_hStopReg);
	memset(&m_stData, 0, sizeof(RegisThreadData));
	m_stData.pControl = this;
	m_stData.pfnMsgCB = pfnBwFunc;
	m_stData.pUserData = pUserData;
	m_hRegThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFuncRegBwServer), &m_stData, 0, NULL);
	if (NULL == m_hRegThread)
	{
		return FALSE;
	}

	return TRUE;
}

/**   @fn          StopRegistBwServer
 *    @brief   	   停止注册带宽控制服务器.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CBwControl::StopRegistBwServer()
{
	if (NULL == m_hRegThread)
	{
		return;
	}

	SetEvent(m_hStopReg);
	WaitForSingleObject(m_hRegThread, INFINITE);
	CloseHandle(m_hRegThread);
	m_hRegThread = NULL;

	PLAYBACKDLL_INFO(_T("StopRegistBwServer"));
}

/**   @fn          ThreadFuncRegBwServer
 *    @brief   	   注册带宽控制服务器线程函数.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
DWORD WINAPI CBwControl::ThreadFuncRegBwServer(LPVOID pParam)
{
	LPRegisThreadData pData = static_cast<LPRegisThreadData>(pParam);
	if (NULL == pData)
	{
		return 0;
	}

	CBwControl* pControl = pData->pControl;
	if (NULL == pData)
	{
		return 0;
	}

	PLAYBACKDLL_INFO(_T("Begin Register bw server!"));

	// 链接CMS.
	if (0 == HPPDM_Connect(theApp.m_stCmsInfo.strCMSIP, theApp.m_stCmsInfo.nPort))
	{
		TCHAR szIP[16];
		MByteToWChar(theApp.m_stCmsInfo.strCMSIP, szIP, 16);
		PLAYBACKDLL_ERROR(_T("HPPDM_Connect error, ip:%s, port:%d"), 
			szIP, theApp.m_stCmsInfo.nPort);
		// 链接失败.
		pData->pfnMsgCB(FALSE, pData->pUserData);
		return 0;
	}
	else
	{
		TCHAR szIP[16];
		MByteToWChar(theApp.m_stCmsInfo.strCMSIP, szIP, 16);
		PLAYBACKDLL_INFO(_T("HPPDM_Connect success ip:%s, nPort:%d"), szIP, theApp.m_stCmsInfo.nPort);
	}

	BOOL bRet = FALSE;
	while (TRUE)
	{
		//获取带宽服务器信息
		if (!theApp.GetBwServerInfo(theApp.m_stBwServerInfo))
		{
			PLAYBACKDLL_ERROR(_T("GetBwServerInfo error!"));
			break;
		}

		PLAYBACKDLL_INFO(_T("Connect Bwcontrol server, IP:%s, Port:%d"), theApp.m_stBwServerInfo.strIp, theApp.m_stBwServerInfo.nPort);
		//注册带宽服务器
		theApp.m_bBwControl = theApp.RegistBwServer(theApp.m_stBwServerInfo);
		if (!theApp.m_bBwControl)
		{
			PLAYBACKDLL_ERROR(_T("Connect Bwcontrol server failed!"));
		}
		else
		{
			PLAYBACKDLL_INFO(_T("Connect Bwcontrol server success!"));
			bRet = TRUE;
			break;
		}

		if (WAIT_OBJECT_0 == WaitForSingleObject(pControl->m_hStopReg, BWSERVER_RECONNECT_TIMESPAN))
		{
			break;
		}
	}

	PLAYBACKDLL_INFO(_T("End Register bw server!"));

	// 带宽服务器注册成功.
	pData->pfnMsgCB(bRet, pData->pUserData);

	return 0;
}

/**   @fn          ReqBwControl
 *    @brief   	   请求带宽控制.
 *    @param[in]   nCameraID:监控点ID.
 *    @param[in]   bReq:请求类型.
 *    @return      
 */
BOOL CBwControl::ReqBwControl(int nCameraID, BOOL bReq)
{
	return theApp.BwControl(nCameraID, bReq);
}

/**   @fn          SetRegisted
 *    @brief   	   设置是否注册完成.
 *    @param[in]  
 *    @param[in]   
 *    @return      
 */
void CBwControl::SetRegisted()
{
	if (m_hRegThread != NULL)
	{
		CloseHandle(m_hRegThread);
		m_hRegThread = NULL;
	}
}