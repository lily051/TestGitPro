/**
*   @file   ServiceController.cpp
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  ServiceController的实现
*   
*   @author Hu Tonghai
*   @date   2009/11/17
*
*   @note   历史记录：
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
*     @brief     初始化一个 ServiceController 新实例
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

    // 初始化计算进程CPU的FILETIME
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

    /* 产生随机数 */
    srand((unsigned int)time(NULL));
}

/*
*     @fn        ServiceController::~ServiceController(void)
*     @brief     析构 ServiceController 实例
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
*     @brief    ServiceController线程体
*     @param    (LPVOID) lpParameter [IN]：ServiceController 实例
*     @return:  DWORD WINAPI: 函数体执行结果
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
*     @brief    启动ServiceController
*     @param    void
*     @return:  bool: true:启动成功; false:启动失败
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
*     @brief    停止ServiceController
*     @param    void
*     @return:  bool: true:停止ServiceController成功; false:停止ServiceController失败
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
        // 清空任务队列，添加结束工作线程信号。
        EnterCriticalSection(&m_csControlCommand);

        while (!m_queueControlCommand.empty())
        {
            m_queueControlCommand.pop();
        }

        m_queueControlCommand.push(control_command_exit);

        // 唤醒工作线程。
        SetEvent(m_hTimerEvent);
        SetEvent(m_hIntervalEvent);

        LeaveCriticalSection(&m_csControlCommand);

        // 等待线程结束。
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
    

    // 删除 PDH
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

    // 置NULL
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
*     @brief    执行命令队列中的命令
*     @param    void
*     @return:  DWORD:执行结果
*/
DWORD ServiceController::CommandProcess(void)
{
    if (m_bRunning)
    {
        /* 获取服务显示名 */
        GetSvcDisplayName();

        while (m_bRunning)
        {
            int svcStatus = m_svcStatus;

            // 默认命令
            control_command_t cmd = control_command_invalid;
            {// 取命令
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
                case control_command_invalid:   // 服务状态检测
                    ControlInvalid();
                    break;
                case control_command_query_service_status:  // 检查服务运行状态
                    QueryWinSvcStatus();
                    break;
                case control_command_start_service: // 启动服务
                    StartWinSvc();
                    break;
                case control_command_stop_service:  // 停止服务
                    StopWinSvc();
                    break;
                case control_command_restart_service:   // 重启服务
                    StopWinSvc();
                    runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), m_svcPath.c_str());
                    runCmd.Format(_T("%s%s"), runDir.GetBuffer(), m_svcStartCmd.c_str());
                    //运行命令
                    RunCmd(runCmd, runDir);
                    StartWinSvc();
                    break;
                case control_command_exit:  // 退出工作线程
                    return 0;
                case control_command_pause_service: // 暂停服务
                    PauseWinSvc();
                    break;
                case control_command_continue_service:  // 恢复服务
                    ContinueWinSvc();
                    break;
                case control_command_start_status_chg:  // 开始状态监测
                    {
                        if (!StartSvcChangeStatus())
                        {
                            m_bCommandStop = false;
                        }
                    }
                    break;
                case control_command_stop_status_chg:   // 停止状态监测
                    {
                        if (!StopSvcChangeStatus())
                        {
                            m_bCommandStop = true;
                        }
                    }
                    break;
                default:    // 检查服务运行状态
                    QueryWinSvcStatus();
                    break;
                }

                /* 设置最近检测时间 */
                if (cmd != control_command_invalid && (strcmp(m_svcLastTime.c_str(), m_svcCurTime.c_str()) >= 0))
                {
                    SetSvcLastTime();
                }

                /* 判断服务是否安装或卸载 */
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

                {// 检查队列。
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

            // 等待下一次查询。
            WaitForSingleObject(m_hIntervalEvent, m_pollInterval * 1000);
        }
    }

    SVCCONTROLLOG_INFO(_T("%s : CommandProcess exit."), m_svcName.c_str());

    return 0;
}

/** @fn       void ServiceController::ControlInvalid(void)
 *  @brief    服务状态检测
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

    /* 是否定时启动/停止服务 */
    TimeToSvcStartOrStop();

    // 若手动关闭服务，则不对其进行监控。
    if (m_bCommandStop)
    {
        SVCCONTROLLOG_DEBUG(_T("%s : true == m_bCommandStop"), m_svcName.c_str());
        return;
    }

    // 是否启动
    if (m_svcStatus == SERVICE_STOPPED)
    {
        CString runDir;
        CString runCmd;
        SVCCONTROLLOG_INFO(_T("%s : m_svcStatus == SERVICE_STOPPED and StartWinSvc."), m_svcName.c_str());
        runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), m_svcPath.c_str());
        runCmd.Format(_T("%s%s"), runDir.GetBuffer(), m_svcStartCmd.c_str());
        //运行命令
        RunCmd(runCmd, runDir);

        StartWinSvc();
        return;
    }

    // 检查服务相关性能参数
    CheckSvcPerformanceParam();
}

/** @fn       bool ServiceController::TimeToSvcStartOrStop(void)
 *  @brief    定时启动/停止服务
 *  @param    void
 *  @return:  bool：true:操作成功 ; false:操作失败
 */
bool ServiceController::TimeToSvcStartOrStop(void)
{
    CTime ctTime = CTime::GetCurrentTime();
    CString csCurTime = ctTime.Format(_T("%H:%M:%S"));
    m_svcCurTime = csCurTime.GetBuffer(0);
    csCurTime.ReleaseBuffer();

    /* 定时停止 */
    if (!m_svcStopTime.empty())
    {
        if (strcmp(m_svcCurTime.c_str(), m_svcStopTime.c_str()) >= 0 && strcmp(m_svcLastTime.c_str(), m_svcStopTime.c_str()) < 0)
        {
            SVCCONTROLLOG_WARN(_T("%s : TimeToSvcStartOrStop stopService."), m_svcName.c_str());
            StopSvc();
        }
    }

    /* 定时启动 */
    if (!m_svcStartTime.empty())
    {
        if (strcmp(m_svcCurTime.c_str(), m_svcStartTime.c_str()) >= 0 && strcmp(m_svcLastTime.c_str(), m_svcStartTime.c_str()) < 0)
        {
            SVCCONTROLLOG_WARN(_T("%s : TimeToSvcStartOrStop startService."), m_svcName.c_str());
            if (m_svcStopTime.empty())
            {// 重启
                RestartSvc();
            }
            else
            {// 启动
                StartSvc();
            }
        }
    }

    m_svcLastTime = m_svcCurTime.c_str();

    return true;
}

/** @fn       void ServiceController::SetSvcLastTime(void)
 *  @brief    设置服务最近检测时间
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
 *  @brief    获取WinSvc handle
 *  @param    (SC_HANDLE&) scManagerHandle [OUT] : SCManager Handle
 *            (SC_HANDLE&) scServiceHandle [OUT] : Service Handle
 *  @return:  bool：true:操作成功 ; false:操作失败
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
 *  @brief    检查服务性能参数
 *  @param    void
 *  @return:  bool：true:操作成功 ; false:操作失败
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
                    //     \Process\% Processor Time 值是进程内所有线程的 \Thread\% Processor Time 之和，故应除以 CPU 核心数量。
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
        // 添加性能计数。
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
*     @brief    跟踪 Windows Service 的 Pending 状态
*     @param    (SC_HANDLE) schService [IN]: Windows Service 句柄
*     @param    (DWORD) status [OUT]: 成功跟踪 Windows Service 的状态变化后返回 Windows Service 的当前状态，否则返回失败类型
*     @return:  bool: true:成功 ; false:失败
*/
bool ServiceController::WaitWinSvcPending(SC_HANDLE schService, DWORD& status)
{
    SERVICE_STATUS_PROCESS ssStatus; 
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwTerminateCount = 0;

    // 查询服务状态。
    if (!QueryWinSvcStatus(schService, ssStatus))
    {
        status = CONTROL_COMMAND_ERROR_QUERY_STATUS;
        return false;
    }

    // 当处于 Pending 状态时，需要等待 Service 状态变化。
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

    // 记录 Pending 状态。
    DWORD pendingStatus = ssStatus.dwCurrentState;

    // 设置等待状态变化信息。
    dwStartTickCount = GetTickCount();
    do
    { 
        // 设置检查状态的间隔时间。
        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
        {
            dwWaitTime = 1000;
        }
        else if (dwWaitTime > 10000)
        {
            dwWaitTime = 10000;
        }

        // 等待
        if (WAIT_OBJECT_0 == WaitForSingleObject(m_hTimerEvent, dwWaitTime))
        {
            status = CONTROL_COMMAND_ERROR_QUERY_STATUS_TIMEOUT;
            return false;
        }

        // 查询服务状态。
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
            // 需要重新计时服务状态变化
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
                // 等待超时
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
*     @brief    改变Windows Service的状态，暂不考虑依赖服务
*     @param    (SC_HANDLE) schService [IN]: Windows Service 句柄
*     @param    (DWORD) controlStatus [IN]: 将 Windows Service  状态更改为 controlStatus
*   @param    (DWORD) status [OUT]: 成功改变 Windows Service 的状态后返回 Windows Service 的当前状态，否则返回失败类型
*     @return:  bool: true:成功; false:失败
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
    //     暂不考虑互相依赖的服务，仅尝试控制当前服务。
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
*     @brief    响应 control_command_query_service_status 命令，查询 Windows Service 当前状态，并报告操作执行结果
*     @param    void
*     @return:  bool: true:成功 ; false:失败
*/
bool ServiceController::QueryWinSvcStatus(void)
{
    CGuard guard(&m_csServiceAndManager);
    // 获取 WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // 获取当前服务的状态。
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
*     @brief    响应 control_command_start_service 命令，启动 Windows Service ，并报告操作执行结果
*     @param    void
*     @return:  bool: true:成功 ; false:失败
*/
bool ServiceController::StartWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : StartWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // 获取 WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // 获取服务状态
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // 处理不同的状态。
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
    //     暂不考虑互相依赖的服务，仅尝试启动当前服务。
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
        // 报告服务即将启动
        PostServiceStatusMessage(SERVICE_START_PENDING);
    }

    // 等待当前服务器状态变化
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
*     @brief    响应 control_command_stop_service 命令，停止 Windows Service ，并报告操作执行结果
*     @param    void
*     @return:  bool: true:成功 ; false:失败
*/
bool ServiceController::StopWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : StopWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // 获取 WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // 获取服务状态
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // 处理不同的状态。
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

    // 报告服务即将停止。
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

    {// 释放原相关的信息
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
*     @brief    响应 control_command_pause_service 命令，暂停 Windows Service ，并报告操作执行结果
*     @param    void 
*     @return:  bool: true:成功 ; false:失败
*/
bool ServiceController::PauseWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : PauseWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // 获取 WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // 获取服务状态
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // 处理不同的状态。
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

    // 报告服务即将暂停。
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
*     @brief    响应 control_command_continue_service 命令，恢复 Windows Service ，并报告操作执行结果
*     @param    void
*     @return:  bool: true:成功 ; false:失败
*/
bool ServiceController::ContinueWinSvc(void)
{
    SVCCONTROLLOG_WARN(_T("%s : ContinueWinSvc start."), m_svcName.c_str());
    CGuard guard(&m_csServiceAndManager);
    // 获取 WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        return false;
    }

    // 获取服务状态
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);

        CloseWinSvcHandle();
        return false;
    }

    // 处理不同的状态。
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

    // 报告服务即将恢复。
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
 *  @brief    获取服务显示名
 *  @param    void
 *  @return:  bool：true:操作成功 ; false:操作失败
 */
bool ServiceController::GetSvcDisplayName(void)
{
    // 获取服务的显示名称。
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

                        // 配置文件有shortname的情况下
                        if (m_svcShortName.length() > 0)
                        {
                            CString tmp2;
                            tmp2.Format(" [ %s ]", m_svcShortName.c_str());
                            m_svcDisplayName += tmp2.GetBuffer(0);
                        }
                        // 配置文件无shortname的情况下，兼容之前的配置文件
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
*     @brief    获取进程是否存在
*     @param    (DWORD)processId(OUT):获取到的进程ID
*     @return:  bool: true:进程存在; false:进程不存在
*/
bool ServiceController::GetProcessId(DWORD& processId)
{
    //CGuard guard(&m_csServiceAndManager);
    processId = 0;

    // 获取 WinSvc handle
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

    // 获取当前服务的状态。
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
*     @brief    获取应用程序名称
*     @param    void
*     @return:  CString:操作成功返回应用程序名，否则返回空字符串
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
 *  @brief    启动服务后，改变服务在列表中的状态
 *  @param    void
 *  @return:  bool : true:操作成功 ; false:操作失败 
 */
bool ServiceController::StartSvcChangeStatus(void)
{
    CGuard guard(&m_csServiceAndManager);
    m_bCommandStop = false;

    // 获取 WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        SVCCONTROLLOG_ERROR(_T("%s : StartSvcChangeStatus GetWinSvcHandle failed."), m_svcName.c_str());
        return false;
    }

    // sleep 1s
    Sleep(1000);

    // 获取服务状态
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);
        CloseWinSvcHandle();

        SVCCONTROLLOG_ERROR(_T("%s : StartSvcChangeStatus WaitWinSvcPending failed."), m_svcName.c_str());
        return false;
    }

    // 处理不同的状态。
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
 *  @brief    停止服务后，改变服务在列表中的状态
 *  @param    (DWORD) ProcId [IN] : 进程号
 *  @return:  bool : true:操作成功 ; false:操作失败
 */
bool ServiceController::StopSvcChangeStatus(void)
{
    CGuard guard(&m_csServiceAndManager);
    m_bCommandStop = true;

    // 获取 WinSvc handle
    DWORD commandError;
    if (!GetWinSvcHandle(commandError))
    {
        PostControlCommandError(commandError);

        SVCCONTROLLOG_ERROR(_T("StopSvcChangeStatus GetWinSvcHandle failed."));
        return false;
    }

    // 报告服务即将停止。
    //PostServiceStatusMessage(SERVICE_STOP_PENDING);
    // sleep 1s
    Sleep(1000);

    // 获取服务状态
    DWORD status;
    if (!WaitWinSvcPending(m_schService, status))
    {
        PostControlCommandError(status);
        CloseWinSvcHandle();

        SVCCONTROLLOG_ERROR(_T("StopSvcChangeStatus WaitWinSvcPending failed."));
        return false;
    }

    // 处理不同的状态。
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
 *  @brief    设置m_bCommandStop的值
 *  @param    (bool) bFlag [IN] : 设置的值
 *  @return:  void
 */
void  ServiceController::SetCommandStop(bool bFlag /* = true */)
{
    m_bCommandStop = bFlag;
}

/** @fn       DWORD ServiceController::GetProcHandleCount(DWORD ProcId)
 *  @brief    获取服务进程的句柄数
 *  @param    (DWORD) ProcId [IN] : 进程号
 *  @return:  DWORD：进程句柄数
 */
DWORD ServiceController::GetProcHandleCount(DWORD ProcId)
{
    DWORD ProcHandleCount = 0;

    if (ProcId != 0)
    {
        // 获取进程句柄
        HANDLE hProcess = NULL;
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcId);

        if (hProcess != NULL)
        {
            // 获取句柄数
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
 *  @brief    获取服务进程的线程数
 *  @param    (DWORD) ProcId [IN] : 进程号
 *  @return:  DWORD：进程的线程数
 */
DWORD ServiceController::GetProcThreadCount(DWORD ProcId)
{
    DWORD ProcThreadCount = 0;

    if (ProcId != 0)
    {
        // 获取线程数
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
 *  @brief    获取服务进程使用的内存 (KB)
 *  @param    void
 *  @return:  DWORD：进程使用的内存
 */
DWORD ServiceController::GetProcMem(DWORD ProcId)
{
    DWORD ProcMem = 0;

    if (ProcId != 0)
    {
        // 获取进程句柄
        HANDLE hProcess = NULL;
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcId);
        if (hProcess != NULL)
        {
            PROCESS_MEMORY_COUNTERS ProcessMemCounters;
            memset(&ProcessMemCounters, 0, sizeof(PROCESS_MEMORY_COUNTERS));
            ProcessMemCounters.cb = sizeof(ProcessMemCounters);

            // 获取内存
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
 *  @brief    判断进程是否可用
 *  @param    void
 *  @return:  bool：true:可用 ; false:不可用
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
 *  @brief    初始化计算进程CPU的FILETIME
 *  @param    void
 *  @return:  bool：true:初始化成功 ; false:初始化失败
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
 *  @brief    获取进程的CPU占用率
 *  @param    void
 *  @return:  int：进程CPU（%）
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

    // 首次获取进程时间
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

        // 获取进程当前时间信息
        BOOL bProcRet = GetProcessTimes(m_hProcHandle, &m_procCurCreateTime, &m_procCurExitTime, &m_procCurKernelTime, &m_procCurUserTime);
        BOOL bSysRet = GetSystemTimes(&m_procCurSysIdleTime, &m_procCurSysKernelTime, &m_procCurSysUserTime);
        if ( (TRUE == bProcRet) && (TRUE == bSysRet) )
        {
            // 用于计算CPU
            int kernelTime = (int) GetFileTimeInterval(m_procPreKernelTime, m_procCurKernelTime) / 10000;
            int userTime = (int) GetFileTimeInterval(m_procPreUserTime, m_procCurUserTime) / 10000;
            int TimeInterval = (int) (GetFileTimeInterval(m_procPreSyskernelTime, m_procCurSysKernelTime)+GetFileTimeInterval(m_procPreSysUserTime,  m_procCurSysUserTime)) / 10000;

            if (TimeInterval > 0)
            {
                // 计算CPU
                ProcCpu = (int)(100 * ((double)kernelTime+userTime)/TimeInterval + 0.5);   //四舍五入
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
 *  @brief    获取进程查询句柄
 *  @param    void
 *  @return:  bool:true:操作成功 ; false:操作失败
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
 *  @brief    获取进程启动时间
 *  @param    void
 *  @return:  CString：进程启动时间(YYYY-MM-DD HH:MM:SS)
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
 *  @brief    获取服务进程信息
 *  @param    (SERVER_PROCESS_INFO &) SerProcInfo [OUT] : 服务进程信息
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
 */
BOOL ServiceController::GetServerProcInfo(SERVER_PROCESS_INFO &SerProcInfo)
{
    // 服务器名
    SerProcInfo.ServerName.Format(_T("%s"), m_svcName.c_str());

    // 服务模块类型
    SerProcInfo.ModuleType = m_moduleType;

    // 进程名
    CString csTmp = GetProcessName();
    if (strcmp(csTmp, _T("")) != 0)
    {
        SerProcInfo.ProcName.Format(_T("%s.exe"), csTmp.GetBuffer());
    }
    // 服务状态
    SerProcInfo.ServerStatus = (int)getServiceStatus();

    DWORD ProcId = 0;
    
    bool bRet = GetProcessId(ProcId);
    
    if ((true == bRet) && (ProcId != 0))
    {
        SVCCONTROLLOG_DEBUG(_T("%s : QueryServiceStatusEx succeeded, processId[%u]."), m_svcName.c_str(), ProcId);

        // 线程数
        SerProcInfo.ThreadCount = GetProcThreadCount(ProcId);
        // 句柄数
        SerProcInfo.HandleCount = GetProcHandleCount(ProcId);
        // 内存
        SerProcInfo.MemWorkingSize = GetProcMem(ProcId);
    }

    // CPU
    SerProcInfo.CpuWorking = GetPrcoCpu();
    // 启动时间
    SerProcInfo.startTime = GetProcStartTime();
    // 监听端口

    return TRUE;
}

/** @fn       void ServiceController::RunCmd(const CString& cmd, const CString& DefaultDir)
 *  @brief    运行命令
 *  @param    (CString) cmd [IN] : ShellExecute的命令
 *            (CString) DefaultDir [IN] : 默认的运行目录
 *  @return:  void
 */
BOOL ServiceController::RunCmd(const CString& cmd, const CString& DefaultDir)
{
    OutputDebugString(_T("---------------------> RunCmd start.\n"));

    // 日志
    SVCMANAGERLOG_INFO(_T("cmd[%s], DefaultDir[%s]."), (LPCTSTR)cmd, (LPCTSTR)DefaultDir);

    // 执行命令
    HINSTANCE hInstance = ShellExecute(NULL, _T("open"), cmd, NULL, DefaultDir, SW_SHOWNORMAL);

    BOOL bRet = FALSE;
    switch((int)hInstance)
    {
    case 0:
        MessageBox(NULL, _T("内存不足"), _T("Error"), MB_OKCANCEL);
        break;
    case ERROR_FILE_NOT_FOUND:
        MessageBox(NULL, _T("文件名或路径名错误"), _T("Error"), MB_OKCANCEL);
        break;
    case ERROR_PATH_NOT_FOUND:
        MessageBox(NULL, _T("文件名或路径名错误"), _T("Error"), MB_OKCANCEL);
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
