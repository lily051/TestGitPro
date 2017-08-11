/**
*   @file   ServiceController.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  Windows Service 控制器
*   
*   @author 胡通海
*   @date   2009/11/17
*
*   @note   历史：V1.0 创建
*
*           2012/08/28 mazhentong modify
*	
*   @warning
*/

#pragma once

#include <queue>
#include <list>
#include <string>

#include <WinSvc.h>
#include <Pdh.h>

#include "UserMsgDef.h"
#include "GenDef.h"
#include "Guard.h"

#include "ServiceWatchdog.h"

using namespace std;
/*
*   @class ServiceController
*   @brief 全程控制一个Windows Service的启动、停止、暂停、恢复及查询服务器运行状态
*/
class ServiceController
{
public:
    string m_svcName;       //服务注册名称

    string m_svcDisplayName;//服务显示名称

    string m_svcShortName;  //服务显示简写名称

    int m_moduleType;       //服务器模块定义

    void* m_userData;       //用户自定义数据

    bool m_bDispNameChnge;  ///< 服务显示名改变

    //{//mazhentong add
    string m_svcPath;           ///< 服务器相对路径
    string m_svcStartCmd;       ///< 服务器启动命令bat
    string m_svcStopCmd;        ///< 服务器停止命令bat
    string m_svcInstallCmd;     ///< 服务器安装命令bat
    string m_svcUninstallCmd;   ///< 服务器卸载命令bat
    string m_svcConfigToolName; ///< 服务器配置工具

    string m_svcStartTime;      ///< 定时启动时间
    string m_svcStopTime;       ///< 定时停止时间

    DWORD m_svcStartTimeOut;       ///< 启动服务时的超时等待时间
    DWORD m_svcStopTimeOut;        ///< 停止服务时的超时等待时间
    //}//add end

protected:
    int m_pollInterval; //ServiceController 默认工作周期的间隔时间（s）

    int m_svcStatus;    //Windows Service 当前的运行状态

    //ServiceController 接受的命令类型枚举
    typedef enum control_command
    {
        control_command_invalid = -1,
        control_command_exit = 0,

        control_command_start_service = 1,
        control_command_stop_service = 2,
        control_command_pause_service = 3,
        control_command_continue_service = 4,
        control_command_query_service_status = 5,
        control_command_restart_service = 6,
        control_command_start_status_chg = 7,
        control_command_stop_status_chg = 8
    } control_command_t;

    time_t m_exceptionTicks;    //出现异常后计时
   
    //PDH
    HQUERY m_pdhQuery;
    HCOUNTER m_pdhProcessorTimeCounter;
    HCOUNTER m_pdhVirtualMemoryUsageCounter;
    HCOUNTER m_pdhHandleCountCounter;

    int m_cpuNum;   //CPU 核数

    bool m_bCommandStop;    //记录是否手动停止

    HANDLE m_hIntervalEvent;    //ServiceController 默认工作周期的控制事件

    HANDLE m_hControlThread;    //ServiceController 工作线程

    CRITICAL_SECTION m_csControlCommand;//ServiceController 命令队列保护区

    queue<control_command_t> m_queueControlCommand;//ServiceController 命令队列

    bool m_bRunning;        //ServiceController 工作状态，true - 工作，false - 停止

    HANDLE m_hTimerEvent;   //ServiceController 用于跟踪 Windows Service 状态的定时事件
 
    SC_HANDLE m_schSCManager;//Windows SC Manager database handle .

    SC_HANDLE m_schService; //Windows Service handle .

    static DWORD WINAPI ControlThreadProc(LPVOID lpParameter);  //ServiceController线程体

    DWORD CommandProcess(void);     //执行命令队列中的命令

    void ControlInvalid(void);  // 服务的状态检测

    bool ControlWinSvc(SC_HANDLE schService, DWORD controlStatus, DWORD& status);   //改变Windows Service的状态

    bool WaitWinSvcPending(SC_HANDLE schService, DWORD& status);    //跟踪 Windows Service 的 Pending 状态

    bool QueryWinSvcStatus(void);   //control_command_query_service_status 命令响应

    bool StartWinSvc(void);         //control_command_start_service 命令响应

    bool StopWinSvc(void);          //control_command_stop_service 命令响应

    bool PauseWinSvc(void);         //control_command_pause_service 命令响应

    bool ContinueWinSvc(void);      //control_command_continue_service 命令响应

    /* 获取服务的显示名 */
    bool GetSvcDisplayName(void);

public:
    ServiceController(void);    //初始化一个 ServiceController 新实例

    ~ServiceController(void);   //析构 ServiceController 实例

    bool Run(void);     //启动ServiceController

    bool Shutdown(void);//停止ServiceController

    bool GetProcessId(DWORD& processId);    //获取进程是否存在

    CString GetProcessName(void);           //获取应用程序名称

    //{//add mzt

    // 获取服务进程的信息
    BOOL GetServerProcInfo(SERVER_PROCESS_INFO &SerProcInfo);

    //运行命令
    BOOL RunCmd(const CString& cmd, const CString& DefaultDir);

    // 启动服务，并改变当前的状态
    bool StartSvcChangeStatus(void);

    // 停止服务，并改变当期的状态
    bool StopSvcChangeStatus(void);

    // 设置m_bCommandStop标志位
    void SetCommandStop(bool bFlag = true);

protected:
    /* 定时启动/停止服务的计时 */
    string m_svcLastTime;           ///< 上次的判断时间
    string m_svcCurTime;            ///< 当前的判断时间

    FILETIME m_procPreKernelTime;       ///< 前一次的内核时间
    FILETIME m_procPreUserTime;         ///< 前一次的用户时间
    FILETIME m_procCurKernelTime;       ///< 当前的内核时间
    FILETIME m_procCurUserTime;         ///< 当前的用户时间
    FILETIME m_procCurCreateTime;       ///< 创建时间
    FILETIME m_procCurExitTime;         ///< 退出时间

    FILETIME m_procCurSysIdleTime;      ///< 当前的系统空闲时间
    FILETIME m_procCurSysKernelTime;    ///< 当前的系统内核时间
    FILETIME m_procCurSysUserTime;      ///< 当期的系统用户时间
    //FILETIME m_procPreSysIdleTime;    ///< 前一次系统空闲时间
    FILETIME m_procPreSyskernelTime;    ///< 前一次系统内核时间
    FILETIME m_procPreSysUserTime;      ///< 前一次系统用户时间

    HANDLE m_hProcHandle;               ///< 用于获取进程CPU

    CRITICAL_SECTION m_csServiceAndManager; ///< protect m_schSCManager And m_schService

    /* 获取服务进程的句柄数 */
    DWORD GetProcHandleCount(DWORD ProcId);

    /* 获取服务进程的线程数 */
    DWORD GetProcThreadCount(DWORD ProcId);

    /* 获取服务进程所用内存 */
    DWORD GetProcMem(DWORD ProcId);

    /* 获取服务进程CPU */
    int GetPrcoCpu(void);

    /* 获取进程启动时间 */
    CString GetProcStartTime(void);

    /* 判断服务进程的状体 */
    bool JudgeProcStatus(void);

    /* 初始化计算进程CPU的FILETIME */
    bool InitProcFileTime(void);

    /* 获取进程的信息查询Handle */
    bool GetProcHandle(void);

    bool CheckSvcPerformanceParam(void);

    /* 检查服务是否定时启动/停止 */
    bool TimeToSvcStartOrStop(void);
    /* 设置服务最近检测时间 */
    void SetSvcLastTime(void);

    /* 获取WinSvc handle */
    bool GetServiceHandle(SC_HANDLE& scManagerHandle, SC_HANDLE& scServiceHandle);
    //}// add end
protected:
    /*
    * 	@fn       bool ServiceController::GetWinSvcHandle(DWORD& commandError)
    * 	@brief    获取 Windows SCManager 和 Windows Service 句柄
    * 	@param    (DWORD) commandError (OUT):执行失败类型
    * 	@return:  bool: true:成功 ; false:失败
    */
    inline bool GetWinSvcHandle(DWORD& commandError)
    {
        if (m_svcName.empty())
        {
            commandError = CONTROL_COMMAND_ERROR_OPEN_SERVICE;
            return false;
        }

        if (m_schSCManager != NULL)
        {
            CloseServiceHandle(m_schSCManager);
            m_schSCManager = NULL;
        }

        // Get a handle to the SCM database. 
        m_schSCManager = OpenSCManager( 
            NULL,                    // local computer
            NULL,                    // ServicesActive database 
            SC_MANAGER_ALL_ACCESS);  // full access rights 

        if (NULL == m_schSCManager) 
        {
            commandError = CONTROL_COMMAND_ERROR_OPEN_SCMANAGER;
            return false;
        }

        if (m_schService != NULL)
        {
            CloseServiceHandle(m_schService);
            m_schService = NULL;
        }

        // Get a handle to the service
        m_schService = OpenService( 
            m_schSCManager,         // SCM database 
            m_svcName.c_str(),      // name of service 
            SERVICE_ALL_ACCESS);    // full access 

        if (NULL == m_schService)
        {
            DWORD lastError = GetLastError();
            if (ERROR_SERVICE_DOES_NOT_EXIST == lastError)
            {
                m_svcStatus = CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST;
                commandError = CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST;
            }
            else
            {
                commandError = CONTROL_COMMAND_ERROR_OPEN_SERVICE;
            }

            CloseServiceHandle(m_schSCManager);
            m_schSCManager = NULL;

            return false;
        }

        return true;
    }
 
    /*
    * 	@fn       void ServiceController::CloseWinSvcHandle(void)
    * 	@brief    闭 Windows SCManager 和 Windows Service 句柄
    * 	@param    void
    * 	@return:  void
    */
    inline void CloseWinSvcHandle(void)
    {
        if (m_schService)
        {
            CloseServiceHandle(m_schService);
            m_schService = NULL;
        }

        if (m_schSCManager)
        {
            CloseServiceHandle(m_schSCManager);
            m_schSCManager = NULL;
        }
    }
 
    /*
    * 	@fn       bool ServiceController::QueryWinSvcStatus(SC_HANDLE schService, SERVICE_STATUS_PROCESS& status, bool setStatus = true)
    * 	@brief    查询 Windows Service 当前状态
    * 	@param    (SC_HANDLE) schService (IN): Windows Service 句柄
    *   @param    (SERVICE_STATUS_PROCESS) status (OUT): Windows Service 状态
//    *   @param    (bool) setStatus (IN): 是否设置当前服务状态，默认执行设置操作
    * 	@return:  bool: true:成功 ; false:失败
    */
    inline bool QueryWinSvcStatus(SC_HANDLE schService, SERVICE_STATUS_PROCESS& status)
    {

        DWORD dwBytesNeeded;

        BOOL rtn = QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)(&status), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);

        if (TRUE == rtn)
        {
            m_svcStatus = status.dwCurrentState;
            return true;
        }

        return false;
    }

    /*
    * 	@fn       void ServiceController::PostServiceStatusMessage(DWORD status)
    * 	@brief    发送 Windows Service 状态改变消息
    * 	@param    (DWORD) status (IN):  Windows Service 的状态
    * 	@return:  void
    */
    inline void PostServiceStatusMessage(DWORD status)
    {
        HWND target = ::AfxGetApp()->GetMainWnd()->m_hWnd;

        if (target)
        {
            ::PostMessage(target, WM_SERVICE_STATUS_CHANGED, (WPARAM)status, (LPARAM)this);
        }
    }

    /*
    * 	@fn       void ServiceController::PostControlCommandError(DWORD error)
    * 	@brief    发送 ServiceController 执行命令过程中产生的异常类型
    * 	@param    (DWORD) error [IN]:  异常类型
    * 	@return:  void
    */
    inline void PostControlCommandError(DWORD error)
    {
        HWND target = ::AfxGetApp()->GetMainWnd()->m_hWnd;

        if (target)
        {
            ::PostMessage(target, WM_SERVICE_CONTROL_COMMAND_ERROR, (WPARAM)error, (LPARAM)this);
        }
    }

    /*
    * 	@fn       bool ServiceController::PushControlCommand(list<control_command_t>& commands)
    * 	@brief    添加命令
    * 	@param    (list<control_command_t>) commands [IN]: 命令列表
    * 	@return:  bool: true:成功 ; false:失败
    */
    inline bool PushControlCommand(list<control_command_t>& commands)
    {
        bool rtn = false;

        if (m_bRunning)
        {
            EnterCriticalSection(&m_csControlCommand);

            if (m_bRunning)
            {
                // 将命令放入命令队列。
                for (list<control_command_t>::iterator itor = commands.begin(); itor != commands.end(); itor++)
                {
                    m_queueControlCommand.push(*itor);
                }

                // 唤醒工作线程。
                SetEvent(m_hIntervalEvent);

                rtn = true;
            }

            LeaveCriticalSection(&m_csControlCommand);
        }

        return rtn;
    }

public:
    /*
    * 	@fn       bool ServiceController::PauseSvc(void)
    * 	@brief    暂停Windows Service 
    * 	@param    void 
    * 	@return:  bool: true:暂停Windows Service成功; false:暂停Windows Service失败
    */
    inline bool PauseSvc(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_pause_service);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::ContinueSvc(void)
    * 	@brief    恢复Windows Service
    * 	@param    void
    * 	@return:  bool: true:恢复成功; false:恢复失败
    */
    inline bool ContinueSvc(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_continue_service);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::StartSvc(void)
    * 	@brief    启动Windows Service
    * 	@param    void
    * 	@return:  bool: true:启动成功; false:启动失败
    */
    inline bool StartSvc(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_start_service);

        if (PushControlCommand(commands))
        {
            m_bCommandStop = false;
            return true;
        }

        return false;
    }

    /** @fn       bool ServiceController::StartSvcStatus(void)
     *  @brief    监测服务的启动状态
     *  @param    void
     *  @return:  bool：true:控制命令入队成功 ; false:失败
     */
    inline bool StartSvcStatus(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_start_status_chg);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::StopSvc(void)
    * 	@brief    停止 Windows Service
    * 	@param    void
    * 	@return:  bool: true:停止 Windows Service成功 ; false:停止 Windows Service失败
    */
    inline bool StopSvc(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_stop_service);

        if (PushControlCommand(commands))
        {
            m_bCommandStop = true;
            return true;
        }

        return false;
    }

    /** @fn       bool ServiceController::StopSvcStatus(void)
     *  @brief    监测服务停止状态
     *  @param    void
     *  @return:  bool：true:控制命令入队成功 ; false:失败
     */
    inline bool StopSvcStatus(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_stop_status_chg);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::RestartSvc(void)
    * 	@brief    重启 Windows Service
    * 	@param    void
    * 	@return:  bool: true:重启 Windows Service 成功; false:重启 Windows Service 失败
    */
    inline bool RestartSvc(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_restart_service);

        //return PushControlCommand(commands);
        if (PushControlCommand(commands))
        {
            m_bCommandStop = false;
            return true;
        }

        return false;
    }

    /*
    * 	@fn       bool ServiceController::QuerySvcStatus(void)
    * 	@brief    查询 Windows Service 当前状态
    * 	@param    void 
    * 	@return:  bool: true:查询 Windows Service 当前状态成功 ; false:查询 Windows Service 当前状态失败
    */
    inline bool QuerySvcStatus(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_query_service_status);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       DWORD ServiceController::getServiceStatus(void)
    * 	@brief    获取 Windows Service 当前状态
    * 	@param    void
    * 	@return:  DWORD:Windows Service 当前状态
    */
    inline DWORD getServiceStatus(void)
    {
        return m_svcStatus;
    }

    /*
    * 	@fn       bool ServiceController::getServiceExists(void)
    * 	@brief    判断服务器是否已经存在
    * 	@param    void
    * 	@return:  bool:true:存在；false:不存在
    */
    inline bool getServiceExists(void)
    {
        return (CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST != m_svcStatus);
    }

    /*
    * 	@fn       void ServiceController::setPollInterval(int poll)
    * 	@brief    设置 ServiceController 工作周期间隔时间（s）
    * 	@param    (int) poll [IN]: ServiceController 工作周期间隔时间（s）
    * 	@return:  void
    */
    inline void setPollInterval(int poll)
    {
        if (poll > 0)
        {
            m_pollInterval = poll;
        }
    }

    /*
    * 	@fn       int ServiceController::getPollInterval(void)
    * 	@brief    获取 ServiceController 工作周期间隔时间（s）
    * 	@param    void
    * 	@return:  int: ServiceController 工作周期间隔时间（s）
    */
    inline int getPollInterval(void) const
    {
        return m_pollInterval;
    }

};
