/**
*   @file   ServiceController.cpp
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  ServiceController��ʵ��
*   
*   @author Hu Tonghai
*   @date   2009/11/17
*
*   @note   ��ʷ��¼��
*           2009/11/17    Hu Tonghai      Create
*           2012/08/29    Ma Zhentong     Modify
*    
*   @warning
*/

//lint -sem(ServiceController::Shutdown, cleanup)

#include "StdAfx.h"

#include "ServiceController.h"

#include "ServiceWatchdogConfig.h"

#include <Windows.h>

#include <Pdh.h>
#pragma comment(lib, "Pdh.lib")

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include <Tlhelp32.h>
#include "Folder.h"

using namespace std;

/*
*     @fn        ServiceController::ServiceController(void)
*     @brief     ��ʼ��һ�� ServiceController ��ʵ��
*     @param     void
*     @return:   NULL
*/
ServiceController::ServiceController(void)
: m_svcStatus(CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST)
, m_pollInterval(SERVICE_CONFIG_POLL_INTERVAL_DEFAULT)
, m_hIntervalEvent(NULL)
, m_hControlThread(NULL)
, m_bRunning(false)
, m_userData(NULL)
, m_schSCManager(NULL)
, m_schService(NULL)
, m_pdhQuery(NULL)
, m_pdhProcessorTimeCounter(NULL)
, m_pdhHandleCountCounter(NULL)
, m_pdhVirtualMemoryUsageCounter(NULL)
, m_exceptionTicks(-1)
, m_bCommandStop(false)
, m_moduleType(0)
, m_hProcHandle(NULL)
, m_bDispNameChnge(false)
, m_svcStartTimeOut(MAX_START_TIME_OUT)
, m_svcStopTimeOut(MAX_STOP_TIME_OUT)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    m_cpuNum = (int)(si.dwNumberOfProcessors);
    if (m_cpuNum <= 0)
    {
        m_cpuNum = 1;
    }

    m_hTimerEvent = NULL;
    
    memset(&m_csControlCommand, 0, sizeof(CRITICAL_SECTION));
    memset(&m_csServiceAndManager, 0, sizeof(CRITICAL_SECTION));

    // ��ʼ���������CPU��FILETIME
    {
        memset(&m_procPreKernelTime, 0, sizeof(FILETIME));
        memset(&m_procPreUserTime, 0, sizeof(FILETIME));
        memset(&m_procCurKernelTime, 0, sizeof(FILETIME));
        memset(&m_procCurUserTime, 0, sizeof(FILETIME));
        memset(&m_procCurCreateTime, 0, sizeof(FILETIME));
        memset(&m_procCurExitTime, 0, sizeof(FILETIME));
        memset(&m_procPreSyskernelTime, 0, sizeof(FILETIME));
        memset(&m_procPreSysUserTime, 0, sizeof(FILETIME));
        memset(&m_procCurSysIdleTime, 0, sizeof(FILETIME));
        memset(&m_procCurSysKernelTime, 0, sizeof(FILETIME));
        memset(&m_procCurSysUserTime, 0, sizeof(FILETIME));
    }

    /* ��������� */
    srand((unsigned int)time(NULL));
}

/*
*     @fn        ServiceController::~ServiceController(void)
*     @brief     ���� ServiceController ʵ��
*     @param     void
*     @return:   NULL
*/
ServiceController::~ServiceController(void)
{
    try
    {
        Shutdown();
    }
    catch (...)
    {
    }
}


/*
*     @fn       DWORD WINAPI ServiceController::ControlThreadProc(LPVOID lpParameter)
*     @brief    ServiceController�߳���
*     @param    (LPVOID) lpParameter [IN]��ServiceController ʵ��
*     @return:  DWORD WINAPI: ������ִ�н��
*/
DWORD WINAPI ServiceController::ControlThreadProc(LPVOID lpParameter)
{
    if (lpParameter)
    {
        ServiceController* pThis = (ServiceController*)lpParameter;

        return pThis->CommandProcess();
    }

    return 0;
}

/*
*     @fn       bool ServiceController::Run(void)
*     @brief    ����ServiceController
*     @param    void
*     @return:  bool: true:�����ɹ�; false:����ʧ��
*/
bool ServiceController::Run(void)
{
    if (m_svcName.empty())
    {
        SVCCONTROLLOG_ERROR(_T("ServiceController::Run : true == m_svcName.IsEmpty()"));
        return false;
    }

    if (m_bRunning)
    {
        SVCCONTROLLOG_INFO(_T("%s already run.") , m_svcName.c_str());
        return true;
    }

    m_hIntervalEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == m_hIntervalEvent)
    {
        SVCCONTROLLOG_ERROR(_T("%s : NULL == m_hIntervalEvent."), m_svcName.c_str());
        return false;
    }

    m_hTimerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == m_hTimerEvent)
    {
        CloseHandle(m_hIntervalEvent);

        SVCCONTROLLOG_ERROR(_T("%s : NULL == m_hTimerEvent."), m_svcName.c_str());
        return false;
    }

    if (FALSE == InitializeCriticalSectionAndSpinCount(&m_csControlCommand, 4000))
    {
        CloseHandle(m_hIntervalEvent);
        CloseHandle(m_hTimerEvent);

        SVCCONTROLLOG_ERROR(_T("%s : InitializeCriticalSectionAndSpinCount m_csControlCommand failed."), m_svcName.c_str());
        return false;
    }

    if (FALSE == InitializeCriticalSectionAndSpinCount(&m_csServiceAndManager, 4000))
    {
        CloseHandle(m_hIntervalEvent);
        CloseHandle(m_hTimerEvent);

        SVCCONTROLLOG_ERROR(_T("%s : InitializeCriticalSectionAndSpinCount m_csServiceAndManager failed."), m_svcName.c_str());
        return false;
    }

    m_bRunning = true;

    DWORD threadId;
    m_hControlThread = CreateThread(NULL, 0, ControlThreadProc, (LPVOID)this, 0, &threadId);

    if (NULL == m_hControlThread)
    {
        m_bRunning = false;

        CloseHandle(m_hIntervalEvent);
        CloseHandle(m_hTimerEvent);
        DeleteCriticalSection(&m_csControlCommand);

        SVCCONTROLLOG_ERROR(_T("%s : CreateThread failed."), m_svcName.c_str());
        return false;
    }

    SVCCONTROLLOG_INFO(_T("%s : Run succeeded."), m_svcName.c_str());
    return true;
}

/*
*     @fn       bool ServiceController::Shutdown(void)
*     @brief    ֹͣServiceController
*     @param    void
*     @return:  bool: true:ֹͣServiceController�ɹ�; false:ֹͣServiceControllerʧ��
*/
bool ServiceController::Shutdown(void)
{
    bool rtn = true;

    if (!m_bRunning)
    {
        return rtn;
    }

    m_bRunning = false;

    if (m_hControlThread)
    {
        // ���������У���ӽ��������߳��źš�
        EnterCriticalSection(&m_csControlCommand);

        while (!m_queueControlCommand.empty())
        {
            m_queueControlCommand.pop();
        }

        m_queueControlCommand.push(control_command_exit);

        // ���ѹ����̡߳�
        SetEvent(m_hTimerEvent);
        SetEvent(m_hIntervalEvent);

        LeaveCriticalSection(&m_csControlCommand);

        // �ȴ��߳̽�����
        WaitForSingleObject(m_hControlThread, INFINITE);

        rtn = CloseHandle(m_hControlThread) && rtn;
        m_hControlThread = NULL;
    }

    if (m_hIntervalEvent)
    {
        rtn = CloseHandle(m_hIntervalEvent) && rtn;
        m_hIntervalEvent = NULL;
    }

    if (m_hTimerEvent)
    {
        rtn = CloseHandle(m_hTimerEvent) && rtn;
        m_hTimerEvent = NULL;
    }

    DeleteCriticalSection(&m_csControlCommand);
    

    // ɾ�� PDH
    if (m_pdhQuery)
    {
        PdhCloseQuery(m_pdhQuery);
         m_pdhQuery = NULL;
    }

    if (m_hProcHandle != NULL)
    {
        CloseHandle(m_hProcHandle);
        m_hProcHandle = NULL;
    }

    {
        CGuard guard(&m_csServiceAndManager);
        if (m_schSCManager != NULL)
        {
            CloseServiceHandle(m_schSCManager);
            m_schSCManager = NULL;
        }

        if (m_schService != NULL)
        {
            CloseServiceHandle(m_schService);
            m_schService = NULL;
        }
    }
    DeleteCriticalSection(&m_csServiceAndManager);

    // ��NULL
    {
        m_userData = NULL;
        m_pdhProcessorTimeCounter = NULL;
        m_pdhVirtualMemoryUsageCounter = NULL;
        m_pdhHandleCountCounter = NULL;
    }

    SVCCONTROLLOG_INFO(_T("%s : Shutdown succeeded."), m_svcName.c_str());
    return rtn;
}

/*
*     @fn       DWORD ServiceController::CommandProcess(void)
*     @brief    ִ����������е�����
*     @param    void
*     @return:  DWORD:ִ�н��
*/
DWORD ServiceController::CommandProcess(void)
{
    if (m_bRunning)
    {
        /* ��ȡ������ʾ�� */
        GetSvcDisplayName();

        while (m_bRunning)
        {
            int svcStatus = m_svcStatus;

            // Ĭ������
            control_command_t cmd = control_command_invalid;
            {// ȡ����
                CGuard guard(&m_csControlCommand);

                if ((int)m_queueControlCommand.size() > 0)
                {
                    cmd = m_queueControlCommand.front();
                    m_queueControlCommand.pop();

                    while ((int)m_queueControlCommand.size() > 0 && cmd == m_queueControlCommand.front())
                    {
                        m_queueControlCommand.pop();
                    }
                }
            }
            CString runDir;
            CString runCmd;
            while (m_bRunning)
            {
                switch (cmd)
                {
                case control_command_invalid:   // ����״̬���
                    ControlInvalid();
                    break;
                case control_command_query_service_status:  // ����������״̬
                    QueryWinSvcStatus();
                    break;
                case control_command_start_service: // ��������
                    StartWinSvc();
                    break;
                case control_command_stop_service:  // ֹͣ����
                    StopWinSvc();
                    break;
                case control_command_restart_service:   // ��������
                    StopWinSvc();
                    runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), m_svcPath.c_str());
                    runCmd.Format(_T("%s%s"), runDir.GetBuffer(), m_svcStartCmd.c_str());
                    //��������
                    RunCmd(runCmd, runDir);
                    StartWinSvc();
                    break;
                case control_command_exit:  // �˳������߳�
                    return 0;
                case control_command_pause_service: // ��ͣ����
                    PauseWinSvc();
                    break;
                case control_command_continue_service:  // �ָ�����
                    ContinueWinSvc();
                    break;
                case control_command_start_status_chg:  // ��ʼ״̬���
                    {
                        if (!StartSvcChangeStatus())
                        {
                            m_bCommandStop = false;
                        }
                    }
                    break;
                case control_command_stop_status_chg:   // ֹͣ״̬���
                    {
                        if (!StopSvcChangeStatus())
                        {
                            m_bCommandStop = true;
                        }
                    }
                    break;
                default:    // ����������״̬
                    QueryWinSvcStatus();
                    break;
                }

                /* ����������ʱ�� */
                if (cmd != control_command_invalid && (strcmp(m_svcLastTime.c_str(), m_svcCurTime.c_str()) >= 0))
                {
                    SetSvcLastTime();
                }

                /* �жϷ����Ƿ�װ��ж�� */
                if ( ((CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST ==  m_svcStatus) ||  
                    (CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST == svcStatus)) && (m_svcStatus != svcStatus))
                {
                    GetSvcDisplayName();
                    svcStatus = m_svcStatus;
                    if (m_bDispNameChnge)
                    {
                        PostServiceStatusMessage(m_svcStatus);
                    }
                }
                else if (CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST == m_svcStatus)
                {
                    int nRet = m_svcDisplayName.find("[");
                    if ((nRet-1) >= 0)
                    {
                        m_svcDisplayName = m_svcDisplayName.substr(0, (nRet-1));
                        m_bDispNameChnge = true;
                        PostServiceStatusMessage(m_svcStatus);
                    }
                }

                {// �����С�
                    CGuard guard(&m_csControlCommand);

                    if ((int)m_queueControlCommand.size() > 0)
                    {
                        cmd = m_queueControlCommand.front();
                        m_queueControlCommand.pop();

                        while ((int)m_queueControlCommand.size() > 0 && cmd == m_queueControlCommand.front())
                        {
                            m_queueControlCommand.pop();
                        }
                    }
                    else
                    {
                        ResetEvent(m_hIntervalEvent);
                        break;
                    }
                }           
            }

            if (!m_bRunning)
            {
                break;
            }

            // �ȴ���һ�β�ѯ��
            WaitForSingleObject(m_hIntervalEvent, m_pollInterval * 1000);
        }
    }

    SVCCONTROLLOG_INFO(_T("%s : CommandProcess exit."), m_svcName.c_str());

    return 0;
}

/** @fn       void ServiceController::ControlInvalid(void)
 *  @brief    ����״̬���
 *  @param    void
 *  @return:  void
 */
void ServiceController::ControlInvalid(void)
{
    QueryWinSvcStatus();

    if (m_svcStatus == CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST)
    {
        SVCCONTROLLOG_DEBUG(_T("%s : m_svcStatus == CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST."), m_svcName.c_str());
        return;
    }

    /* �Ƿ�ʱ����/ֹͣ���� */
    TimeToSvcStartOrStop();

    // ���ֶ��رշ����򲻶�����м�ء�
    if (m_bCommandStop)
    {
        SVCCONTROLLOG_DEBUG(_T("%s : true == m_bCommandStop"), m_svcName.c_str());
        return;
    }

    // �Ƿ�����
    if (m_svcStatus == SERVICE_STOPPED)
    {
        CString runDir;
        CString runCmd;
        SVCCONTROLLOG_INFO(_T("%s : m_svcStatus == SERVICE_STOPPED and StartWinSvc."), m_svcName.c_str());
        runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), m_svcPath.c_str());
        runCmd.Format(_T("%s%s"), runDir.GetBuffer(), m_svcStartCmd.c_str());
        //��������
        RunCmd(runCmd, runDir);

        StartWinSvc();
        return;
    }

    // ������������ܲ���
    CheckSvcPerformanceParam();
}

/** @fn       bool ServiceController::TimeToSvcStartOrStop(void)
 *  @brief    ��ʱ����/ֹͣ����
 *  @param    void
 *  @return:  bool��true:�����ɹ� ; false:����ʧ��
 */
bool ServiceController::TimeToSvcStartOrStop(void)
{
    CTime ctTime = CTime::GetCurrentTime();
    CString csCurTime = ctTime.Format(_T("%H:%M:%S"));
    m_svcCurTime = csCurTime.GetBuffer(0);
    csCurTime.ReleaseBuffer();

    /* ��ʱֹͣ */
    if (!m_svcStopTime.empty())
    {
        if (strcmp(m_svcCurTime.c_str(), m_svcStopTime.c_str()) >= 0 && strcmp(m_svcLastTime.c_str(), m_svcStopTime.c_str()) < 0)
        {
            SVCCONTROLLOG_WARN(_T("%s : TimeToSvcStartOrStop stopService."), m_svcName.c_str());
            StopSvc();
        }
    }

    /* ��ʱ���� */
    if (!m_svcStartTime.empty())
    {
        if (strcmp(m_svcCurTime.c_str(), m_svcStartTime.c_str()) >= 0 && strcmp(m_svcLastTime.c_str(), m_svcStartTime.c_str()) < 0)
        {
            SVCCONTROLLOG_WARN(_T("%s : TimeToSvcStartOrStop startService."), m_svcName.c_str());
            if (m_svcStopTime.empty())
            {// ����
                RestartSvc();
            }
            else
            {// ����
                StartSvc();
            }
        }
    }

    m_svcLastTime = m_svcCurTime.c_str();

    return true;
}

/** @fn       void ServiceController::SetSvcLastTime(void)
 *  @brief    ���÷���������ʱ��
 *  @param    void
 *  @return:  void
 */
void ServiceController::SetSvcLastTime(void)
{
    CTime ctTime = CTime::GetCurrentTime();
    CString csCurTime = ctTime.Format(_T("%H:%M:%S"));
    m_svcLastTime = csCurTime.GetBuffer(0);
    csCurTime.ReleaseBuffer();
}

/** @fn       bool ServiceController::GetServiceHandle(SC_HANDLE& scManagerHandle, SC_HANDLE& scServiceHandle)
 *  @brief    ��ȡWinSvc handle
 *  @param    (SC_HANDLE&) scManagerHandle [OUT] : SCManager Handle
 *            (SC_HANDLE&) scServiceHandle [OUT] : Service Handle
 *  @return:  bool��true:�����ɹ� ; false:����ʧ��
 */
bool ServiceController::GetServiceHandle(SC_HANDLE& scManagerHandle, SC_HANDLE& scServiceHandle)
{
    scManagerHandle = NULL;
    scServiceHandle = NULL;
    if (m_svcName.empty())
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_OPEN_SERVICE);
        return false;
    }

    // Get a handle to the SCM database. 
    scManagerHandle = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == scManagerHandle) 
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_OPEN_SCMANAGER);
        return false;
    }

    // Get a handle to the service
    scServiceHandle = OpenService( 
        scManagerHandle,         // SCM database 
        m_svcName.c_str(),      // name of service 
        SERVICE_ALL_ACCESS);    // full access 

    if (NULL == scServiceHandle)
    {
        DWORD lastError = GetLastError();
        if (ERROR_SERVICE_DOES_NOT_EXIST == lastError)
        {
            m_svcStatus = CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST;
            PostControlCommandError(CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST);
        }
        else
        {
            PostControlCommandError(CONTROL_COMMAND_ERROR_OPEN_SERVICE);
        }

        CloseServiceHandle(scManagerHandle);
        scManagerHandle = NULL;

        return false;
    }

    return true;
}

/** @fn       bool ServiceController::QuerySvcPerformanceParam(void)
 *  @brief    ���������ܲ���
 *  @param    void
 *  @return:  bool��true:�����ɹ� ; false:����ʧ��
 */
bool ServiceController::CheckSvcPerformanceParam(void)
{
    if (m_pdhQuery)
    {
        bool bDoNotResetExceptionTicks = false;
        PDH_FMT_COUNTERVALUE counterBuffer;
        PDH_STATUS pdhStatus = PdhCollectQueryData(m_pdhQuery);
        if(pdhStatus == ERROR_SUCCESS)
        {
            // CPU
            if (m_pdhProcessorTimeCounter)
            {
                pdhStatus = PdhGetFormattedCounterValue (
                    m_pdhProcessorTimeCounter,
                    PDH_FMT_DOUBLE,
                    (LPDWORD)NULL,
                    &counterBuffer);
                if (pdhStatus == ERROR_SUCCESS)
                {
                    //
                    // 2010-7-19 16:48:40 Hu Tonghai Remarks:
                    //     \Process\% Processor Time ֵ�ǽ����������̵߳� \Thread\% Processor Time ֮�ͣ���Ӧ���� CPU ����������
                    //
                    if ((counterBuffer.doubleValue / m_cpuNum) >= ServiceWatchdogConfig::Instance().m_maxProcessorTime)
                    {
                        SVCCONTROLLOG_WARN(_T("%s : counterBuffer.doubleValue >= ServiceWatchdogConfig::Instance().m_maxProcessorTime."), m_svcName.c_str());

                        bDoNotResetExceptionTicks = true;
                    }
                }
            }
            // Memory
            if (m_pdhVirtualMemoryUsageCounter)
            {
                pdhStatus = PdhGetFormattedCounterValue (
                    m_pdhVirtualMemoryUsageCounter,
                    PDH_FMT_LARGE,
                    (LPDWORD)NULL,
                    &counterBuffer);
                if (pdhStatus == ERROR_SUCCESS)
                {
                    if ((counterBuffer.largeValue / 1024) >= ServiceWatchdogConfig::Instance().m_maxVirtualMemoryUsage)
                    {
                        SVCCONTROLLOG_WARN(_T("%s : (counterBuffer.largeValue / 1024) >= ServiceWatchdogConfig::Instance().m_maxVirtualMemoryUsage."), m_svcName.c_str());
                        bDoNotResetExceptionTicks = true;
                    }
                }
            }
            // Handle
            if (m_pdhHandleCountCounter)
            {
                pdhStatus = PdhGetFormattedCounterValue (
                    m_pdhHandleCountCounter,
                    PDH_FMT_LARGE,
                    (LPDWORD)NULL,
                    &counterBuffer);
                if (pdhStatus == ERROR_SUCCESS)
                {
                    if (counterBuffer.largeValue >= ServiceWatchdogConfig::Instance().m_maxHandleCount)
                    {
                        SVCCONTROLLOG_WARN(_T("%s : counterBuffer.largeValue >= ServiceWatchdogConfig::Instance().m_maxHandleCount."), m_svcName.c_str());
                        bDoNotResetExceptionTicks = true;
                    }
                }
            }
        }
        else
        {
            SVCCONTROLLOG_WARN(_T("%s : PdhCollectQueryData failed,ERROR[%u]."), m_svcName.c_str(), GetLastError());
        }

        /* ThreadCount check */
        //

        if (!bDoNotResetExceptionTicks)
        {
            m_exceptionTicks = -1;

            //SVCCONTROLLOG_INFO(_T("%s : m_exceptionTicks = -1."), m_svcName.c_str());
        }
        else
        {
            if (m_exceptionTicks >= 0)
            {
                if ((time(NULL) - m_exceptionTicks) >  60)
                {
                    SVCCONTROLLOG_WARN(_T("%s : (time(NULL) - m_exceptionTicks) >  60 and restart winsvc."), m_svcName.c_str());

                    StopWinSvc();
                    StartWinSvc();

                    m_exceptionTicks = -1;
                }
            }
            else
            {
                m_exceptionTicks = time(NULL);
            }
        }
    }
    else
    {
        // ������ܼ�����
        CString strProcessName = GetProcessName();
        if (!strProcessName.IsEmpty())
        {
            // Open a query object.
            PDH_STATUS pdhStatus = PdhOpenQuery(0, 0, &m_pdhQuery);

            if (pdhStatus == ERROR_SUCCESS)
            {
                CString counterPath;

                // ProcessorTimeCounter
                counterPath.Format("\\Process(%s)\\%% Processor Time", strProcessName.GetBuffer());
                pdhStatus = PdhAddCounter(m_pdhQuery, counterPath.GetBuffer(0), 0, &m_pdhProcessorTimeCounter);
                if(pdhStatus != ERROR_SUCCESS)
                {
                    SVCCONTROLLOG_WARN(_T("%s : PdhAddCounter failed, %s, ERROR[%u]."), m_svcName.c_str(), counterPath.GetBuffer(0), GetLastError());
                    m_pdhProcessorTimeCounter = NULL;
                }

                // VirtualMemoryUsageCounter
                counterPath.Format("\\Process(%s)\\Virtual Bytes", (LPCTSTR)strProcessName);
                pdhStatus = PdhAddCounter(m_pdhQuery, counterPath.GetBuffer(0), 0, &m_pdhVirtualMemoryUsageCounter);
                if(pdhStatus != ERROR_SUCCESS)
                {
                    SVCCONTROLLOG_WARN(_T("%s : PdhAddCounter failed, %s, ERROR[%u]."), m_svcName.c_str(), counterPath.GetBuffer(0), GetLastError());
                    m_pdhVirtualMemoryUsageCounter = NULL;
                }

                // HandleCountCounter
                counterPath.Format("\\Process(%s)\\Handle Count", strProcessName.GetBuffer());
                pdhStatus = PdhAddCounter(m_pdhQuery, counterPath.GetBuffer(0), 0, &m_pdhHandleCountCounter);
                if(pdhStatus != ERROR_SUCCESS)
                {
                    SVCCONTROLLOG_WARN(_T("%s : PdhAddCounter failed, %s, error %u."), m_svcName.c_str(), counterPath.GetBuffer(0), GetLastError());
                    m_pdhHandleCountCounter = NULL;
                }
            }
            else
            {
                m_pdhQuery = NULL;
                SVCCONTROLLOG_WARN(_T("%s : PdhOpenQuery failed, ERROR[%u]."), m_svcName.c_str(), GetLastError());
            }
        }
        else
        {
            SVCCONTROLLOG_WARN(_T("%s : strProcessName is empty."), m_svcName.c_str());
        }
    }

    return true;
}

/*
*     @fn       bool ServiceController::WaitWinSvcPending(SC_HANDLE schService, DWORD& status)
*     @brief    ���� Windows Service �� Pending ״̬
*     @param    (SC_HANDLE) schService [IN]: Windows Service ���
*     @param    (DWORD) status [OUT]: �ɹ����� Windows Service ��״̬�仯�󷵻� Windows Service �ĵ�ǰ״̬�����򷵻�ʧ������
*     @return:  bool: true:�ɹ� ; false:ʧ��
*/
bool ServiceController::WaitWinSvcPending(SC_HANDLE schService, DWORD& status)
{
    SERVICE_STATUS_PROCESS ssStatus; 
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwTerminateCount = 0;

    // ��ѯ����״̬��
    if (!QueryWinSvcStatus(schService, ssStatus))
    {
        status = CONTROL_COMMAND_ERROR_QUERY_STATUS;
        return false;
    }

    // ������ Pending ״̬ʱ����Ҫ�ȴ� Service ״̬�仯��
    DWORD pendingStatusError;

    switch (ssStatus.dwCurrentState)
    {
    case SERVICE_START_PENDING:
        pendingStatusError = CONTROL_COMMAND_ERROR_START_TIMEOUT;
        break;
    case SERVICE_STOP_PENDING:
        pendingStatusError = CONTROL_COMMAND_ERROR_STOP_TIMEOUT;
        break;
    case SERVICE_PAUSE_PENDING:
        pendingStatusError = CONTROL_COMMAND_ERROR_PAUSE_TIMEOUT;
        break;
    case SERVICE_CONTINUE_PENDING:
        pendingStatusError = CONTROL_COMMAND_ERROR_CONTINUE_TIMEOUT;
        break;
    default:
        status = ssStatus.dwCurrentState;
        return true;
    }

    // ��¼ Pending ״̬��
    DWORD pendingStatus = ssStatus.dwCurrentState;

    // ���õȴ�״̬�仯��Ϣ��
    dwStartTickCount = GetTickCount();
    do
    { 
        // ���ü��״̬�ļ��ʱ�䡣
        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
        {
            dwWaitTime = 1000;
        }
        else if (dwWaitTime > 10000)
        {
            dwWaitTime = 10000;
        }

        // �ȴ�
        if (WAIT_OBJECT_0 == WaitForSingleObject(m_hTimerEvent, dwWaitTime))
        {
            status = CONTROL_COMMAND_ERROR_QUERY_STATUS_TIMEOUT;
            return false;
        }

        // ��ѯ����״̬��
        if (!QueryWinSvcStatus(schService, ssStatus))
        {
            status = CONTROL_COMMAND_ERROR_QUERY_STATUS;
            return false;
        }

        if (ssStatus.dwCurrentState != pendingStatus)
        {
            break;
        }

        if (GetTickCount() <  dwStartTickCount)
        {
            // ��Ҫ���¼�ʱ����״̬�仯
            dwStartTickCount = GetTickCount();
        }
        else
        {
            if (SERVICE_STOP_PENDING == pendingStatus)
            {
                DWORD dwTickCountTime = GetTickCount() - dwStartTickCount;
                if ( dwTickCountTime > (m_svcStopTimeOut * 1000) )
                {
                    dwTerminateCount++;
                    SVCCONTROLLOG_ERROR(_T("%s: WaitWinSvcPending timeout [%d] svcStopTimeOut:[%d]..."), m_svcName.c_str(), dwTerminateCount, m_svcStopTimeOut);

                    DWORD dwProId;
                    GetProcessId(dwProId);
                    HANDLE hProcess = NULL;
                    if (dwProId > 0)
                    {
                        hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ | PROCESS_TERMINATE ,
                            FALSE, dwProId );
                    }

                    if (hProcess != NULL)
                    {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            }
            else
            {
                if ( (GetTickCount() - dwStartTickCount) > (m_svcStartTimeOut * 1000) )
                {
                    dwTerminateCount++;
                    SVCCONTROLLOG_ERROR(_T("%s: WaitWinSvcPending timeout [%d] svcStartTimeOut[%d]..."), m_svcName.c_str(), dwTerminateCount, m_svcStartTimeOut);

                    DWORD dwProId;
                    GetProcessId(dwProId);
                    HANDLE hProcess = NULL;
                    if (dwProId > 0)
                    {
                        hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ | PROCESS_TERMINATE ,
                            FALSE, dwProId );
                    }

                    if (hProcess != NULL)
                    {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            }

            if (dwTerminateCount > 3)
            {
                // �ȴ���ʱ
                status = pendingStatusError;
                return false;
            }
        }

    } while (ssStatus.dwCurrentState == pendingStatus);

    status = ssStatus.dwCurrentState;
    return true;
}

/*
*     @fn       bool ServiceController::ControlWinSvc(SC_HANDLE schService, DWORD controlStatus, DWORD& status)
*     @brief    �ı�Windows Service��״̬���ݲ�������������
*     @param    (SC_HANDLE) schService [IN]: Windows Service ���
*     @param    (DWORD) controlStatus [IN]: �� Windows Service  ״̬����Ϊ controlStatus
*   @param    (DWORD) status [OUT]: �ɹ��ı� Windows Service ��״̬�󷵻� Windows Service �ĵ�ǰ״̬�����򷵻�ʧ������
*     @return:  bool: true:�ɹ�; false:ʧ��
*/
bool ServiceController::ControlWinSvc(SC_HANDLE schService, DWORD controlStatus, DWORD& status)
{
    SVCCONTROLLOG_WARN(_T("%s : ControlWinSvc start."), m_svcName.c_str());
    //DWORD pendingStatus;
    DWORD controlCommandError;
    DWORD successStatus;

    switch (controlStatus)
    {
    case SERVICE_CONTROL_STOP:
        {
            //pendingStatus = SERVICE_STOP_PENDING;
            controlCommandError = CONTROL_COMMAND_ERROR_STOP;
            successStatus = SERVICE_STOPPED;
        }
        break;
    case SERVICE_CONTROL_PAUSE:
        {
            //pendingStatus = SERVICE_PAUSE_PENDING;
            controlCommandError = CONTROL_COMMAND_ERROR_PAUSE;
            successStatus = SERVICE_PAUSED;
        }
        break;
    case SERVICE_CONTROL_CONTINUE:
        {
            //pendingStatus = SERVICE_CONTINUE_PENDING;
            controlCommandError = CONTROL_COMMAND_ERROR_CONTINUE;
            successStatus = SERVICE_RUNNING;
        }
        break;
    default:
        status = CONTROL_COMMAND_ERROR_UNKNOWN;
        return false;
    }

    SERVICE_STATUS_PROCESS ssStatus;

    //
    // 2009-11-20 20:35:24 Hu Tonghai Remarks:
    //     �ݲ����ǻ��������ķ��񣬽����Կ��Ƶ�ǰ����
    //
    if (FALSE == ControlService(schService, controlStatus, (LPSERVICE_STATUS)&ssStatus))
    {
        status = controlCommandError;

        return false;
    }

    if (!WaitWinSvcPending(schService, status))
    {
        return false;
    }

    if (successStatus != status)
    {
        return false;
    }

    return true;
}

/*
*     @fn       bool ServiceController::QueryWinSvcStatus(void)
*     @brief    ��Ӧ control_command_query_service_status �����ѯ Windows Service ��ǰ״̬�����������ִ�н��
*     @param    void
*     @return:  bool: true:�ɹ� ; false:ʧ��
*/
bool ServiceController::QueryWinSvcStatus(void)
{
    CGuard guard(&m_csServiceAndManager);
    // ��ȡ WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // ��ȡ��ǰ�����״̬��
    SERVICE_STATUS_PROCESS ssStatus;
    if (!QueryWinSvcStatus(m_schService, ssStatus))
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_QUERY_STATUS);

        CloseWinSvcHandle();

        return false;
    }
    PostServiceStatusMessage(m_svcStatus);

    CloseWinSvcHandle();

    return true;
}


/*
*     @fn       bool ServiceController::StartWinSvc(void)
*     @brief    ��Ӧ control_command_start_service ������� Windows Service �����������ִ�н��
*     @param    void
*     @return:  bool: true:�ɹ� ; false:ʧ��
*/
bool ServiceController::StartWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : StartWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // ��ȡ WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // ��ȡ����״̬
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // ����ͬ��״̬��
    switch (status)
    {
    case SERVICE_STOPPED:
        break;
    case SERVICE_PAUSED:
        CloseWinSvcHandle();
        return ContinueWinSvc();
    case SERVICE_RUNNING:
        CloseWinSvcHandle();
        return true;
    case SERVICE_CONTINUE_PENDING:
    case SERVICE_START_PENDING:
    case SERVICE_STOP_PENDING:
    case SERVICE_PAUSE_PENDING:
    default:
        PostControlCommandError(CONTROL_COMMAND_ERROR_START);
        CloseWinSvcHandle();
        return false;
    }

    //
    // 2009-11-20 20:43:34 Hu Tonghai Remarks:
    //     �ݲ����ǻ��������ķ��񣬽�����������ǰ����
    //
    if (!StartService(
        m_schService,  // handle to service 
        0,             // number of arguments 
        NULL) )        // no arguments 
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_START);

        CloseWinSvcHandle();
        return false; 
    }
    else
    {
        // ������񼴽�����
        PostServiceStatusMessage(SERVICE_START_PENDING);
    }

    // �ȴ���ǰ������״̬�仯
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    PostServiceStatusMessage(status);

    if (SERVICE_RUNNING != status)
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_START);

        CloseWinSvcHandle();
        return false;
    }

    CloseWinSvcHandle();
    return true;
}

/*
*     @fn       bool ServiceController::StopWinSvc(void)
*     @brief    ��Ӧ control_command_stop_service ���ֹͣ Windows Service �����������ִ�н��
*     @param    void
*     @return:  bool: true:�ɹ� ; false:ʧ��
*/
bool ServiceController::StopWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : StopWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // ��ȡ WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // ��ȡ����״̬
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // ����ͬ��״̬��
    switch (status)
    {
    case SERVICE_RUNNING:
    case SERVICE_PAUSED:
        break;
    case SERVICE_STOPPED:
        CloseWinSvcHandle();
        return true;
    case SERVICE_CONTINUE_PENDING:
    case SERVICE_START_PENDING:
    case SERVICE_STOP_PENDING:
    case SERVICE_PAUSE_PENDING:
    default:
        PostControlCommandError(CONTROL_COMMAND_ERROR_STOP);
        CloseWinSvcHandle();
        return false;
    }

    // ������񼴽�ֹͣ��
    PostServiceStatusMessage(SERVICE_STOP_PENDING);

    if (!ControlWinSvc(m_schService, SERVICE_CONTROL_STOP, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    PostServiceStatusMessage(status);

    if (SERVICE_STOPPED != status)
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_STOP);

        CloseWinSvcHandle();
        return false;
    }

    {// �ͷ�ԭ��ص���Ϣ
        if (m_hProcHandle != NULL)
        {
            CloseHandle(m_hProcHandle);
            m_hProcHandle = NULL;
        }

        if ( (m_procPreKernelTime.dwHighDateTime != 0) || (m_procPreKernelTime.dwLowDateTime != 0) )
        {
            InitProcFileTime();
        }
    }

    CloseWinSvcHandle();
    return true;
}

/*
*     @fn       bool ServiceController::PauseWinSvc(void)
*     @brief    ��Ӧ control_command_pause_service �����ͣ Windows Service �����������ִ�н��
*     @param    void 
*     @return:  bool: true:�ɹ� ; false:ʧ��
*/
bool ServiceController::PauseWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : PauseWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // ��ȡ WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // ��ȡ����״̬
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // ����ͬ��״̬��
    switch (status)
    {
    case SERVICE_RUNNING:
        break;
    case SERVICE_PAUSED:
        CloseWinSvcHandle();
        return true;
    case SERVICE_STOPPED:
    case SERVICE_CONTINUE_PENDING:
    case SERVICE_START_PENDING:
    case SERVICE_STOP_PENDING:
    case SERVICE_PAUSE_PENDING:
    default:
        PostControlCommandError(CONTROL_COMMAND_ERROR_PAUSE);
        CloseWinSvcHandle();
        return false;
    }

    // ������񼴽���ͣ��
    PostServiceStatusMessage(SERVICE_PAUSE_PENDING);

    if (!ControlWinSvc(m_schService, SERVICE_CONTROL_PAUSE, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    PostServiceStatusMessage(status);

    if (SERVICE_PAUSED != status)
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_PAUSE);

        CloseWinSvcHandle();
        return false;
    }

    CloseWinSvcHandle();
    return true;
}

/*
*     @fn       bool ServiceController::ContinueWinSvc(void)
*     @brief    ��Ӧ control_command_continue_service ����ָ� Windows Service �����������ִ�н��
*     @param    void
*     @return:  bool: true:�ɹ� ; false:ʧ��
*/
bool ServiceController::ContinueWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : ContinueWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // ��ȡ WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // ��ȡ����״̬
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // ����ͬ��״̬��
    switch (status)
    {
    case SERVICE_PAUSED:
        break;
    case SERVICE_RUNNING:
        CloseWinSvcHandle();
        return true;
    case SERVICE_STOPPED:
    case SERVICE_CONTINUE_PENDING:
    case SERVICE_START_PENDING:
    case SERVICE_STOP_PENDING:
    case SERVICE_PAUSE_PENDING:
    default:
        PostControlCommandError(CONTROL_COMMAND_ERROR_CONTINUE);
        CloseWinSvcHandle();
        return false;
    }

    // ������񼴽��ָ���
    PostServiceStatusMessage(SERVICE_CONTINUE_PENDING);

    if (!ControlWinSvc(m_schService, SERVICE_CONTROL_CONTINUE, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    PostServiceStatusMessage(status);

    if (SERVICE_RUNNING != status)
    {
        PostControlCommandError(CONTROL_COMMAND_ERROR_CONTINUE);

        CloseWinSvcHandle();
        return false;
    }

    CloseWinSvcHandle();
    return true;
}

/** @fn       bool ServiceController::GetSvcDisplayName(void)
 *  @brief    ��ȡ������ʾ��
 *  @param    void
 *  @return:  bool��true:�����ɹ� ; false:����ʧ��
 */
bool ServiceController::GetSvcDisplayName(void)
{
    // ��ȡ�������ʾ���ơ�
    if (false == m_svcName.empty())
    {
        SC_HANDLE hSCManager;   //Windows SC Manager database handle .

        // Get a handle to the SCM database. 
        hSCManager = OpenSCManager( 
            NULL,                    // local computer
            NULL,                    // ServicesActive database 
            SC_MANAGER_ALL_ACCESS);  // full access rights 

        if (hSCManager != NULL) 
        {
            DWORD bufferSize = 0;
            if (!GetServiceDisplayName(hSCManager, m_svcName.c_str(), NULL, &bufferSize))
            {
                DWORD lastError = GetLastError();
                if (ERROR_INSUFFICIENT_BUFFER == lastError)
                {
                    int nRet = m_svcDisplayName.find("[");
                    if (bufferSize > 0 && (nRet <= 0))
                    {
                        CString tmp;
                        tmp.Preallocate(bufferSize + 1);
                        GetServiceDisplayName(hSCManager, m_svcName.c_str(), tmp.GetBuffer(0), &bufferSize);
                        tmp.ReleaseBuffer(bufferSize);

                        // �����ļ���shortname�������
                        if (m_svcShortName.length() > 0)
                        {
                            CString tmp2;
                            tmp2.Format(" [ %s ]", m_svcShortName.c_str());
                            m_svcDisplayName += tmp2.GetBuffer(0);
                        }
                        // �����ļ���shortname������£�����֮ǰ�������ļ�
                        else if (m_svcDisplayName.length() > 0)
                        {
                            CString tmp2;
                            tmp2.Format(" [ %s ]", tmp.GetBuffer());
                            m_svcDisplayName += tmp2.GetBuffer(0);
                        }
                        else
                        {
                            m_svcDisplayName = tmp;
                        }

                        m_bDispNameChnge = true;
                    }
                }
                else if (ERROR_SERVICE_DOES_NOT_EXIST == lastError)
                {
                    int nRet = m_svcDisplayName.find("[");
                    if ((nRet-1) >= 0)
                    {
                        m_svcDisplayName = m_svcDisplayName.substr(0, (nRet-1));
                        m_bDispNameChnge = true;
                    }
                    
                    m_svcStatus = CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST;

                    PostControlCommandError(CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST);
                }
                else
                {
                    SVCCONTROLLOG_WARN(_T("%s : GetServiceDisplayName failed, ERROR[%u]."), m_svcName.c_str(), GetLastError());
                }
            }

            CloseServiceHandle(hSCManager);
            //hSCManager = NULL;
        }
    }

    return true;
}

/*
*     @fn       bool ServiceController::GetProcessId(DWORD& processId)
*     @brief    ��ȡ�����Ƿ����
*     @param    (DWORD)processId(OUT):��ȡ���Ľ���ID
*     @return:  bool: true:���̴���; false:���̲�����
*/
bool ServiceController::GetProcessId(DWORD& processId)
{
    //CGuard guard(&m_csServiceAndManager);
    processId = 0;

    // ��ȡ WinSvc handle
    SC_HANDLE schSCManager = NULL;//Windows SC Manager database handle .
    SC_HANDLE schService = NULL; //Windows Service handle .
    {
        if (m_svcName.empty())
        {
            PostControlCommandError(CONTROL_COMMAND_ERROR_OPEN_SERVICE);
            return false;
        }

        // Get a handle to the SCM database. 
        schSCManager = OpenSCManager( 
            NULL,                    // local computer
            NULL,                    // ServicesActive database 
            SC_MANAGER_ALL_ACCESS);  // full access rights 

        if (NULL == schSCManager) 
        {
            PostControlCommandError(CONTROL_COMMAND_ERROR_OPEN_SCMANAGER);
            return false;
        }

        // Get a handle to the service
        schService = OpenService( 
            schSCManager,         // SCM database 
            m_svcName.c_str(),      // name of service 
            SERVICE_ALL_ACCESS);    // full access 

        if (NULL == schService)
        {
            DWORD lastError = GetLastError();
            if (ERROR_SERVICE_DOES_NOT_EXIST == lastError)
            {
                m_svcStatus = CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST;
                PostControlCommandError(CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST);
            }
            else
            {
                PostControlCommandError(CONTROL_COMMAND_ERROR_OPEN_SERVICE);
            }

            CloseServiceHandle(schSCManager);
            //schSCManager = NULL;

            return false;
        }
    }

    // ��ȡ��ǰ�����״̬��
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    BOOL rtn = QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)(&ssStatus),
        sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);

    if (TRUE == rtn)
    {
        processId = ssStatus.dwProcessId;

        //theLogger.PDebug(m_svcName, "QueryServiceStatusEx succeeded, processId %u.", processId);
    }
    else
    {
        SVCCONTROLLOG_WARN(_T("%s : QueryServiceStatusEx failed, ERROR[%u]."), m_svcName.c_str(), GetLastError());
    }

    {// CLOSE SC_HANDLE
        if (schService)
        {
            CloseServiceHandle(schService);
            //schService = NULL;
        }

        if (schSCManager)
        {
            CloseServiceHandle(schSCManager);
            //schSCManager = NULL;
        }
    }
    return (TRUE == rtn);
}

/*
*     @fn       CString ServiceController::GetProcessName()
*     @brief    ��ȡӦ�ó�������
*     @param    void
*     @return:  CString:�����ɹ�����Ӧ�ó����������򷵻ؿ��ַ���
*/
CString ServiceController::GetProcessName()
{
    CString rtn;

    DWORD processId;
    if (JudgeProcStatus() && GetProcessId(processId))
    {
        TCHAR szProcessName[MAX_PATH] = TEXT("");

        // Get a handle to the process.

        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, processId );

        // Get the process name.

        if (NULL != hProcess )
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
                &cbNeeded) )
            {
                GetModuleBaseName( hProcess, hMod, szProcessName, 
                    sizeof(szProcessName)/sizeof(TCHAR) );

                rtn = szProcessName;
                int idx = rtn.ReverseFind('.');
                if (idx > 0)
                {
                    rtn.Truncate(idx);
                }
            }
            else
            {
                SVCCONTROLLOG_WARN(_T("%s : EnumProcessModules failed, ERROR[%u]."), m_svcName.c_str(), GetLastError());
            }
        }
        else
        {
            SVCCONTROLLOG_WARN(_T("%s : OpenProcess failed, ERROR[%u]."), m_svcName.c_str(), GetLastError());
        }

        CloseHandle( hProcess );
    }

    return rtn;
}

//{//mzt add
/** @fn       bool ServiceController::StartSvcChangeStatus(void)
 *  @brief    ��������󣬸ı�������б��е�״̬
 *  @param    void
 *  @return:  bool : true:�����ɹ� ; false:����ʧ�� 
 */
bool ServiceController::StartSvcChangeStatus(void)
{
    CGuard guard(&m_csServiceAndManager);
    m_bCommandStop = false;

    // ��ȡ WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        SVCCONTROLLOG_ERROR(_T("%s : StartSvcChangeStatus GetWinSvcHandle failed."), m_svcName.c_str());
        return false;
    }

    // sleep 1s
    Sleep(1000);

    // ��ȡ����״̬
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);
        CloseWinSvcHandle();

        SVCCONTROLLOG_ERROR(_T("%s : StartSvcChangeStatus WaitWinSvcPending failed."), m_svcName.c_str());
        return false;
    }

    // ����ͬ��״̬��
    switch (status)
    {
    case SERVICE_RUNNING:
       PostServiceStatusMessage(SERVICE_RUNNING);
       break;
    case SERVICE_START_PENDING:
        PostServiceStatusMessage(SERVICE_START_PENDING);
        break;
    case SERVICE_PAUSED:
    case SERVICE_STOPPED:
    case SERVICE_CONTINUE_PENDING:
    case SERVICE_STOP_PENDING:
    case SERVICE_PAUSE_PENDING:
    default:
        {
            SVCCONTROLLOG_ERROR(_T("%s : StartSvcChangeStatus default status."), m_svcName.c_str());
            PostControlCommandError(CONTROL_COMMAND_ERROR_START);
            CloseWinSvcHandle();
            return false;
        }
    }

    CloseWinSvcHandle();
    return true;
}

/** @fn       bool ServiceController::StopSvcChangeStatus(void)
 *  @brief    ֹͣ����󣬸ı�������б��е�״̬
 *  @param    (DWORD) ProcId [IN] : ���̺�
 *  @return:  bool : true:�����ɹ� ; false:����ʧ��
 */
bool ServiceController::StopSvcChangeStatus(void)
{
    CGuard guard(&m_csServiceAndManager);
    m_bCommandStop = true;

    // ��ȡ WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        SVCCONTROLLOG_ERROR(_T("StopSvcChangeStatus GetWinSvcHandle failed."));
        return false;
    }

    // ������񼴽�ֹͣ��
    //PostServiceStatusMessage(SERVICE_STOP_PENDING);
    // sleep 1s
    Sleep(1000);

    // ��ȡ����״̬
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);
        CloseWinSvcHandle();

        SVCCONTROLLOG_ERROR(_T("StopSvcChangeStatus WaitWinSvcPending failed."));
        return false;
    }

    // ����ͬ��״̬��
    switch (status)
    {
    case SERVICE_STOP_PENDING:
        PostServiceStatusMessage(SERVICE_STOP_PENDING);
        break;
    case SERVICE_STOPPED:
        PostServiceStatusMessage(SERVICE_STOPPED);
        break;
    case SERVICE_PAUSED:
    case SERVICE_START_PENDING:
    case SERVICE_CONTINUE_PENDING:
    case SERVICE_RUNNING:
    case SERVICE_PAUSE_PENDING:
    default:
        {
            SVCCONTROLLOG_ERROR(_T("StopSvcChangeStatus default status."));
            PostControlCommandError(CONTROL_COMMAND_ERROR_STOP);
            CloseWinSvcHandle();
            return false;
        }
    }

    CloseWinSvcHandle();
    return true;
}

/** @fn       void ServiceController::SetCommandStop(bool bFlag  = true )
 *  @brief    ����m_bCommandStop��ֵ
 *  @param    (bool) bFlag [IN] : ���õ�ֵ
 *  @return:  void
 */
void  ServiceController::SetCommandStop(bool bFlag /* = true */)
{
    m_bCommandStop = bFlag;
}

/** @fn       DWORD ServiceController::GetProcHandleCount(DWORD ProcId)
 *  @brief    ��ȡ������̵ľ����
 *  @param    (DWORD) ProcId [IN] : ���̺�
 *  @return:  DWORD�����̾����
 */
DWORD ServiceController::GetProcHandleCount(DWORD ProcId)
{
    DWORD ProcHandleCount = 0;

    if (ProcId != 0)
    {
        // ��ȡ���̾��
        HANDLE hProcess = NULL;
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcId);

        if (hProcess != NULL)
        {
            // ��ȡ�����
           BOOL bGetHandleRet = GetProcessHandleCount(hProcess, &ProcHandleCount);
           if (FALSE == bGetHandleRet)
           {
               ProcHandleCount = 0;
           }

           CloseHandle(hProcess);
           //hProcess = NULL;
        }
    }

    return ProcHandleCount;
}

/** @fn       DWORD ServiceController::GetProcThreadCount(DWORD ProcId)
 *  @brief    ��ȡ������̵��߳���
 *  @param    (DWORD) ProcId [IN] : ���̺�
 *  @return:  DWORD�����̵��߳���
 */
DWORD ServiceController::GetProcThreadCount(DWORD ProcId)
{
    DWORD ProcThreadCount = 0;

    if (ProcId != 0)
    {
        // ��ȡ�߳���
        HANDLE hGetThreadCountProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 procEntry;
        if (hGetThreadCountProcess != INVALID_HANDLE_VALUE)
        {
            memset(&procEntry, 0, sizeof(procEntry));
            procEntry.dwSize = sizeof(procEntry);
            BOOL bRet = Process32First(hGetThreadCountProcess, &procEntry);

            while (TRUE == bRet)
            {
                if (procEntry.th32ProcessID == ProcId)
                {
                    ProcThreadCount = procEntry.cntThreads;
                    break;
                }

                memset(&procEntry, 0, sizeof(procEntry));
                procEntry.dwSize = sizeof(procEntry);
                bRet = Process32Next(hGetThreadCountProcess, &procEntry);
            }

            CloseHandle(hGetThreadCountProcess);
           // hGetThreadCountProcess = NULL;
        }
    }

    return ProcThreadCount;
}

/** @fn       DWORD ServiceController::GetProcMem(DWORD ProcId)
 *  @brief    ��ȡ�������ʹ�õ��ڴ� (KB)
 *  @param    void
 *  @return:  DWORD������ʹ�õ��ڴ�
 */
DWORD ServiceController::GetProcMem(DWORD ProcId)
{
    DWORD ProcMem = 0;

    if (ProcId != 0)
    {
        // ��ȡ���̾��
        HANDLE hProcess = NULL;
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcId);
        if (hProcess != NULL)
        {
            PROCESS_MEMORY_COUNTERS ProcessMemCounters;
            memset(&ProcessMemCounters, 0, sizeof(PROCESS_MEMORY_COUNTERS));
            ProcessMemCounters.cb = sizeof(ProcessMemCounters);

            // ��ȡ�ڴ�
            BOOL bRet = GetProcessMemoryInfo(hProcess, &ProcessMemCounters, sizeof(ProcessMemCounters));
            if (TRUE == bRet)
            {
                ProcMem = (DWORD) (ProcessMemCounters.WorkingSetSize / (1024));
            }

            CloseHandle(hProcess);
            //hProcess = NULL;
        }
    }
    return ProcMem;
}

/** @fn       bool ServiceController::JudgeProcStatus(void)
 *  @brief    �жϽ����Ƿ����
 *  @param    void
 *  @return:  bool��true:���� ; false:������
 */
bool ServiceController::JudgeProcStatus(void)
{
    bool bRet = false;
    switch (m_svcStatus)
    {
    case SERVICE_RUNNING:
    case SERVICE_CONTINUE_PENDING:
    case SERVICE_PAUSE_PENDING:
    case SERVICE_PAUSED:
    case SERVICE_STOP_PENDING:
        bRet = true;
        break;
    case SERVICE_START_PENDING:
    case SERVICE_STOPPED:
    case CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST:
    default:
        bRet = false;
        break;
    }

    return bRet;
}

/** @fn       bool ServiceController::InitProcFileTime(void)
 *  @brief    ��ʼ���������CPU��FILETIME
 *  @param    void
 *  @return:  bool��true:��ʼ���ɹ� ; false:��ʼ��ʧ��
 */
bool ServiceController::InitProcFileTime(void)
{
    memset(&m_procPreKernelTime, 0, sizeof(FILETIME));
    memset(&m_procPreUserTime, 0, sizeof(FILETIME));
    memset(&m_procCurKernelTime, 0, sizeof(FILETIME));
    memset(&m_procCurUserTime, 0, sizeof(FILETIME));
    memset(&m_procCurCreateTime, 0, sizeof(FILETIME));
    memset(&m_procCurExitTime, 0, sizeof(FILETIME));
    memset(&m_procPreSyskernelTime, 0, sizeof(FILETIME));
    memset(&m_procPreSysUserTime, 0, sizeof(FILETIME));
    memset(&m_procCurSysIdleTime, 0, sizeof(FILETIME));
    memset(&m_procCurSysKernelTime, 0, sizeof(FILETIME));
    memset(&m_procCurSysUserTime, 0, sizeof(FILETIME));
    return true;
}

/** @fn       int ServiceController::GetPrcoCpu(void)
 *  @brief    ��ȡ���̵�CPUռ����
 *  @param    void
 *  @return:  int������CPU��%��
 */
int ServiceController::GetPrcoCpu(void)
{
    int ProcCpu = 0;
    if (!JudgeProcStatus())
    {
        if (m_hProcHandle != NULL)
        {
            CloseHandle(m_hProcHandle);
            m_hProcHandle = NULL;
        }

        if ( (m_procPreKernelTime.dwHighDateTime != 0) || (m_procPreKernelTime.dwLowDateTime != 0) )
        {
            InitProcFileTime();
        }
        return ProcCpu;
    }

    // �״λ�ȡ����ʱ��
    if ((0 == m_procPreKernelTime.dwHighDateTime) && (0 == m_procPreKernelTime.dwLowDateTime))
    {
        if (GetProcHandle())
        {
            BOOL bProcRet = FALSE;
            BOOL bSysRet = FALSE;
            bProcRet = GetProcessTimes(m_hProcHandle, &m_procCurCreateTime, &m_procCurExitTime, &m_procPreKernelTime, &m_procPreUserTime);
            bSysRet = GetSystemTimes(&m_procCurSysIdleTime, &m_procPreSyskernelTime, &m_procPreSysUserTime);
            if ( (FALSE == bProcRet) || (FALSE == bSysRet) )
            {
                InitProcFileTime();
            }
        }
    }
    else
    {
        if (NULL == m_hProcHandle)
        {
            InitProcFileTime();
            return ProcCpu;
        }

        // ��ȡ���̵�ǰʱ����Ϣ
        BOOL bProcRet = GetProcessTimes(m_hProcHandle, &m_procCurCreateTime, &m_procCurExitTime, &m_procCurKernelTime, &m_procCurUserTime);
        BOOL bSysRet = GetSystemTimes(&m_procCurSysIdleTime, &m_procCurSysKernelTime, &m_procCurSysUserTime);
        if ( (TRUE == bProcRet) && (TRUE == bSysRet) )
        {
            // ���ڼ���CPU
            int kernelTime = (int) GetFileTimeInterval(m_procPreKernelTime, m_procCurKernelTime) / 10000;
            int userTime = (int) GetFileTimeInterval(m_procPreUserTime, m_procCurUserTime) / 10000;
            int TimeInterval = (int) (GetFileTimeInterval(m_procPreSyskernelTime, m_procCurSysKernelTime)+GetFileTimeInterval(m_procPreSysUserTime,  m_procCurSysUserTime)) / 10000;

            if (TimeInterval > 0)
            {
                // ����CPU
                ProcCpu = (int)(100 * ((double)kernelTime+userTime)/TimeInterval + 0.5);   //��������
            }

            m_procPreSyskernelTime = m_procCurSysKernelTime;
            m_procPreSysUserTime = m_procCurSysUserTime;
            m_procPreKernelTime = m_procCurKernelTime;
            m_procPreUserTime = m_procCurUserTime;
        }
    }

    return ProcCpu;
}

/** @fn       bool ServiceController::GetProcHandle(void)
 *  @brief    ��ȡ���̲�ѯ���
 *  @param    void
 *  @return:  bool:true:�����ɹ� ; false:����ʧ��
 */
bool ServiceController::GetProcHandle(void)
{
    if (m_hProcHandle != NULL)
    {
        return true;
    }
    DWORD ProcId = 0;
    if (GetProcessId(ProcId))
    {
        if (ProcId != 0)
        {
            m_hProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcId);
            if (m_hProcHandle != NULL)
            {
                return true;
            }
        }
    }
    return false;
}

/** @fn       CString ServiceController::GetProcStartTime(void)
 *  @brief    ��ȡ��������ʱ��
 *  @param    void
 *  @return:  CString����������ʱ��(YYYY-MM-DD HH:MM:SS)
 */
CString ServiceController::GetProcStartTime(void)
{
    CString procStartTime;

    if (JudgeProcStatus())
    {
        if ( (m_procCurCreateTime.dwLowDateTime != 0) || (m_procCurCreateTime.dwHighDateTime != 0) )
        {
            CTime ProcCurTime(m_procCurCreateTime);
            procStartTime = ProcCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));
        }
    }

    return procStartTime;
}

/** @fn       BOOL ServiceController::GetServerProcInfo(SERVER_PROCESS_INFO &SerProcInfo)
 *  @brief    ��ȡ���������Ϣ
 *  @param    (SERVER_PROCESS_INFO &) SerProcInfo [OUT] : ���������Ϣ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL ServiceController::GetServerProcInfo(SERVER_PROCESS_INFO &SerProcInfo)
{
    // ��������
    SerProcInfo.ServerName.Format(_T("%s"), m_svcName.c_str());

    // ����ģ������
    SerProcInfo.ModuleType = m_moduleType;

    // ������
    CString csTmp = GetProcessName();
    if (strcmp(csTmp, _T("")) != 0)
    {
        SerProcInfo.ProcName.Format(_T("%s.exe"), csTmp.GetBuffer());
    }
    // ����״̬
    SerProcInfo.ServerStatus = (int)getServiceStatus();

    DWORD ProcId = 0;
    
    bool bRet = GetProcessId(ProcId);
    
    if ((true == bRet) && (ProcId != 0))
    {
        SVCCONTROLLOG_DEBUG(_T("%s : QueryServiceStatusEx succeeded, processId[%u]."), m_svcName.c_str(), ProcId);

        // �߳���
        SerProcInfo.ThreadCount = GetProcThreadCount(ProcId);
        // �����
        SerProcInfo.HandleCount = GetProcHandleCount(ProcId);
        // �ڴ�
        SerProcInfo.MemWorkingSize = GetProcMem(ProcId);
    }

    // CPU
    SerProcInfo.CpuWorking = GetPrcoCpu();
    // ����ʱ��
    SerProcInfo.startTime = GetProcStartTime();
    // �����˿�

    return TRUE;
}

/** @fn       void ServiceController::RunCmd(const CString& cmd, const CString& DefaultDir)
 *  @brief    ��������
 *  @param    (CString) cmd [IN] : ShellExecute������
 *            (CString) DefaultDir [IN] : Ĭ�ϵ�����Ŀ¼
 *  @return:  void
 */
BOOL ServiceController::RunCmd(const CString& cmd, const CString& DefaultDir)
{
    OutputDebugString(_T("---------------------> RunCmd start.\n"));

    // ��־
    SVCMANAGERLOG_INFO(_T("cmd[%s], DefaultDir[%s]."), (LPCTSTR)cmd, (LPCTSTR)DefaultDir);

    // ִ������
    HINSTANCE hInstance = ShellExecute(NULL, _T("open"), cmd, NULL, DefaultDir, SW_SHOWNORMAL);

    BOOL bRet = FALSE;
    switch((int)hInstance)
    {
    case 0:
        MessageBox(NULL, _T("�ڴ治��"), _T("Error"), MB_OKCANCEL);
        break;
    case ERROR_FILE_NOT_FOUND:
        MessageBox(NULL, _T("�ļ�����·��������"), _T("Error"), MB_OKCANCEL);
        break;
    case ERROR_PATH_NOT_FOUND:
        MessageBox(NULL, _T("�ļ�����·��������"), _T("Error"), MB_OKCANCEL);
        break;
    case ERROR_BAD_FORMAT:
        MessageBox(NULL, _T("ERROR_BAD_FORMAT"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_ASSOCINCOMPLETE:
        MessageBox(NULL, _T("SE_ERR_ASSOCINCOMPLETE"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_ACCESSDENIED:
        MessageBox(NULL, _T("SE_ERR_ACCESSDENIED"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_DDEBUSY:
        MessageBox(NULL, _T("SE_ERR_DDEBUSY"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_DDEFAIL:
        MessageBox(NULL, _T("SE_ERR_DDEFAIL"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_DDETIMEOUT:
        MessageBox(NULL, _T("SE_ERR_DDETIMEOUT"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_DLLNOTFOUND:
        MessageBox(NULL, _T("SE_ERR_DLLNOTFOUND"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_NOASSOC:
        MessageBox(NULL, _T("SE_ERR_NOASSOC"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_OOM:
        MessageBox(NULL, _T("SE_ERR_OOM"), _T("Error"), MB_OKCANCEL);
        break;
    case SE_ERR_SHARE:
        MessageBox(NULL, _T("SE_ERR_SHARE"), _T("Error"), MB_OKCANCEL);
        break;
    default:
        bRet = TRUE;
        break;
    }

    OutputDebugString(_T("---------------------> RunCmd end.\n"));
    return bRet;
}

//}//end add
