#include "StdAfx.h"
#include "InitOper.h"
#include "PreviewS20Dll.h"

//lint -library
CInitOper::CInitOper(void)
    : m_bInit(FALSE)
{
}

CInitOper::~CInitOper(void)
{
}

/** @fn         Init
 *  @brief      ��ʼ��
 *  @return     TRUE: �ɹ�, FALSE: ʧ��
 */
BOOL CInitOper::Init()
{
    OutputDebugString(_T("......................InitOper begin......................"));
	CString strLogPath;
	strLogPath = theApp.m_strModulePath + _T("log4cxx.properties");
	OutputDebugString(strLogPath);
	//hlog_set_configfile_name(CStringW2A(strLogPath).GetBuffer());

    // StreamLayer
    OutputDebugString(_T("...Init StreamLayer start...\n"));
    STARTUP_INFO slStrart;
    slStrart.bInitHcSdk = FALSE;
    slStrart.nTimeout = 3;
    slStrart.nTryTimes = 0;
    if (!SLStartup(&slStrart))
    {
        PV_ERROR("StreamLayer Init failed");

        OutputDebugString(_T("...Init StreamLayer failed...\n"));
        OutputDebugString(_T("......................InitOper end......................\n"));
        return FALSE;
    }
    OutputDebugString(_T("...Init StreamLayer stop...\n"));

	OutputDebugString(_T("...Init VSClient_Startup start...\n"));
	//VSClient_Startup();
	OutputDebugString(_T("...Init VSClient_Startup stop...\n"));

	OutputDebugString(_T("...Init PSClient_Startup start...\n"));
	PSClient_Startup();
	OutputDebugString(_T("...Init PSClient_Startup stop...\n"));

    OutputDebugString(_T("...start Init TipDlg...\n"));
    InitTipDlg(1);
    OutputDebugString(_T("...stop Init TipDlg...\n"));
	BOOL bRet = FALSE;
	LONG nPort = -1;
	if (!USBCamera_InitSDK())
	{
		PV_ERROR("USBCamera_InitSDK Failed![Error:%d]", USBCamera_GetLastError());
	}
	if (!USBCamera_SetFilter(USBCAMERA_ALL))//ĿǰSDK�汾�������⣬���ӿڷ���ֵ�����ж�
	{
		PV_ERROR("USBCamera_SetFilter Failed![Error:%d]", USBCamera_GetLastError());
	}
	if(!USBCamera_GetPort(&theApp.m_lUSBPort))
	{
		PV_ERROR("USBCamera_GetPort Failed![Error:%d]", USBCamera_GetLastError());
	}
    m_bInit = TRUE;

    OutputDebugString(_T("......................InitOper end......................\n"));
    return TRUE;
}

/** @fn         UnInit
 *  @brief      ����ʼ��
 *  @return     void
 */
void CInitOper::UnInit()
{
    int nStart = 0;
    int nStop = 0;
    CString strTime;
    nStart = GetTickCount();

    OutputDebugString(_T("......................UnInitOper begin......................\n"));

    TRACE(_T("%d...UnInit TipDlg start...\n"), (int)time(NULL));
    DestoryTipDlg();
    TRACE(_T("%d...UnInit TipDlg stop...\n"), (int)time(NULL));

    nStop = GetTickCount();
    strTime.Format(_T("<<<<<<<<<< CInitOper::UnInit::DestoryTipDlg Time=%d >>>>>>>>>>"), nStop-nStart);
    OutputDebugString(strTime);

	nStart = GetTickCount();

	TRACE(_T("%d...UnInit PSClient_Cleanup start...\n"), (int)time(NULL));

	OutputDebugString(_T("...UnInit PSClient_Cleanup start...\n"));
	PSClient_Cleanup();
	OutputDebugString(_T("...UnInit PSClient_Cleanup stop...\n"));

	nStop = GetTickCount();
	strTime.Format(_T("<<<<<<<<<< CInitOper::UnInit::PSClient_Cleanup Time=%d >>>>>>>>>>\n"), nStop-nStart);
	OutputDebugString(strTime);

	nStart = GetTickCount();

	TRACE(_T("%d...UnInit VSClient_Cleanup start...\n"), (int)time(NULL));

	OutputDebugString(_T("...UnInit VSClient_Cleanup start...\n"));
	//VSClient_Cleanup();
	OutputDebugString(_T("...UnInit VSClient_Cleanup stop...\n"));

	nStop = GetTickCount();
	strTime.Format(_T("<<<<<<<<<< CInitOper::UnInit::VSClient_Cleanup Time=%d >>>>>>>>>>\n"), nStop-nStart);
	OutputDebugString(strTime);

    nStart = GetTickCount();

    TRACE(_T("%d...UnInit StreamLayer start...\n"), (int)time(NULL));
    SLCleanup();
    TRACE(_T("%d...UnInit StreamLayer stop...\n"), (int)time(NULL));

    nStop = GetTickCount();
    strTime.Format(_T("<<<<<<<<<< CInitOper::UnInit::SLCleanup Time=%d >>>>>>>>>>\n"), nStop-nStart);
    OutputDebugString(strTime);

	if (!USBCamera_FreePort(theApp.m_lUSBPort))
	{
		PV_ERROR("USBCamera_FreePort Failed![Error:%d]", USBCamera_GetLastError());
	}

	if (!USBCamera_UninitSDK())
	{
		PV_ERROR("USBCamera_UninitSDK Failed![Error:%d]", USBCamera_GetLastError());
	}

    m_bInit = FALSE;

    OutputDebugString(_T("......................UnInitOper end......................\n"));
}

/** @fn         IsInited
 *  @brief      �Ƿ�����ɳ�ʼ��
 *  @return     TRUE: ��, FALSE: ��
 */
BOOL CInitOper::IsInited()
{
    return m_bInit;
}
