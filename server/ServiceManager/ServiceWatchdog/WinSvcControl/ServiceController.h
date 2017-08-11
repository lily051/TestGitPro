/**
*   @file   ServiceController.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  Windows Service ������
*   
*   @author ��ͨ��
*   @date   2009/11/17
*
*   @note   ��ʷ��V1.0 ����
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
*   @brief ȫ�̿���һ��Windows Service��������ֹͣ����ͣ���ָ�����ѯ����������״̬
*/
class ServiceController
{
public:
    string m_svcName;       //����ע������

    string m_svcDisplayName;//������ʾ����

    string m_svcShortName;  //������ʾ��д����

    int m_moduleType;       //������ģ�鶨��

    void* m_userData;       //�û��Զ�������

    bool m_bDispNameChnge;  ///< ������ʾ���ı�

    //{//mazhentong add
    string m_svcPath;           ///< ���������·��
    string m_svcStartCmd;       ///< ��������������bat
    string m_svcStopCmd;        ///< ������ֹͣ����bat
    string m_svcInstallCmd;     ///< ��������װ����bat
    string m_svcUninstallCmd;   ///< ������ж������bat
    string m_svcConfigToolName; ///< ���������ù���

    string m_svcStartTime;      ///< ��ʱ����ʱ��
    string m_svcStopTime;       ///< ��ʱֹͣʱ��

    DWORD m_svcStartTimeOut;       ///< ��������ʱ�ĳ�ʱ�ȴ�ʱ��
    DWORD m_svcStopTimeOut;        ///< ֹͣ����ʱ�ĳ�ʱ�ȴ�ʱ��
    //}//add end

protected:
    int m_pollInterval; //ServiceController Ĭ�Ϲ������ڵļ��ʱ�䣨s��

    int m_svcStatus;    //Windows Service ��ǰ������״̬

    //ServiceController ���ܵ���������ö��
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

    time_t m_exceptionTicks;    //�����쳣���ʱ
   
    //PDH
    HQUERY m_pdhQuery;
    HCOUNTER m_pdhProcessorTimeCounter;
    HCOUNTER m_pdhVirtualMemoryUsageCounter;
    HCOUNTER m_pdhHandleCountCounter;

    int m_cpuNum;   //CPU ����

    bool m_bCommandStop;    //��¼�Ƿ��ֶ�ֹͣ

    HANDLE m_hIntervalEvent;    //ServiceController Ĭ�Ϲ������ڵĿ����¼�

    HANDLE m_hControlThread;    //ServiceController �����߳�

    CRITICAL_SECTION m_csControlCommand;//ServiceController ������б�����

    queue<control_command_t> m_queueControlCommand;//ServiceController �������

    bool m_bRunning;        //ServiceController ����״̬��true - ������false - ֹͣ

    HANDLE m_hTimerEvent;   //ServiceController ���ڸ��� Windows Service ״̬�Ķ�ʱ�¼�
 
    SC_HANDLE m_schSCManager;//Windows SC Manager database handle .

    SC_HANDLE m_schService; //Windows Service handle .

    static DWORD WINAPI ControlThreadProc(LPVOID lpParameter);  //ServiceController�߳���

    DWORD CommandProcess(void);     //ִ����������е�����

    void ControlInvalid(void);  // �����״̬���

    bool ControlWinSvc(SC_HANDLE schService, DWORD controlStatus, DWORD& status);   //�ı�Windows Service��״̬

    bool WaitWinSvcPending(SC_HANDLE schService, DWORD& status);    //���� Windows Service �� Pending ״̬

    bool QueryWinSvcStatus(void);   //control_command_query_service_status ������Ӧ

    bool StartWinSvc(void);         //control_command_start_service ������Ӧ

    bool StopWinSvc(void);          //control_command_stop_service ������Ӧ

    bool PauseWinSvc(void);         //control_command_pause_service ������Ӧ

    bool ContinueWinSvc(void);      //control_command_continue_service ������Ӧ

    /* ��ȡ�������ʾ�� */
    bool GetSvcDisplayName(void);

public:
    ServiceController(void);    //��ʼ��һ�� ServiceController ��ʵ��

    ~ServiceController(void);   //���� ServiceController ʵ��

    bool Run(void);     //����ServiceController

    bool Shutdown(void);//ֹͣServiceController

    bool GetProcessId(DWORD& processId);    //��ȡ�����Ƿ����

    CString GetProcessName(void);           //��ȡӦ�ó�������

    //{//add mzt

    // ��ȡ������̵���Ϣ
    BOOL GetServerProcInfo(SERVER_PROCESS_INFO &SerProcInfo);

    //��������
    BOOL RunCmd(const CString& cmd, const CString& DefaultDir);

    // �������񣬲��ı䵱ǰ��״̬
    bool StartSvcChangeStatus(void);

    // ֹͣ���񣬲��ı䵱�ڵ�״̬
    bool StopSvcChangeStatus(void);

    // ����m_bCommandStop��־λ
    void SetCommandStop(bool bFlag = true);

protected:
    /* ��ʱ����/ֹͣ����ļ�ʱ */
    string m_svcLastTime;           ///< �ϴε��ж�ʱ��
    string m_svcCurTime;            ///< ��ǰ���ж�ʱ��

    FILETIME m_procPreKernelTime;       ///< ǰһ�ε��ں�ʱ��
    FILETIME m_procPreUserTime;         ///< ǰһ�ε��û�ʱ��
    FILETIME m_procCurKernelTime;       ///< ��ǰ���ں�ʱ��
    FILETIME m_procCurUserTime;         ///< ��ǰ���û�ʱ��
    FILETIME m_procCurCreateTime;       ///< ����ʱ��
    FILETIME m_procCurExitTime;         ///< �˳�ʱ��

    FILETIME m_procCurSysIdleTime;      ///< ��ǰ��ϵͳ����ʱ��
    FILETIME m_procCurSysKernelTime;    ///< ��ǰ��ϵͳ�ں�ʱ��
    FILETIME m_procCurSysUserTime;      ///< ���ڵ�ϵͳ�û�ʱ��
    //FILETIME m_procPreSysIdleTime;    ///< ǰһ��ϵͳ����ʱ��
    FILETIME m_procPreSyskernelTime;    ///< ǰһ��ϵͳ�ں�ʱ��
    FILETIME m_procPreSysUserTime;      ///< ǰһ��ϵͳ�û�ʱ��

    HANDLE m_hProcHandle;               ///< ���ڻ�ȡ����CPU

    CRITICAL_SECTION m_csServiceAndManager; ///< protect m_schSCManager And m_schService

    /* ��ȡ������̵ľ���� */
    DWORD GetProcHandleCount(DWORD ProcId);

    /* ��ȡ������̵��߳��� */
    DWORD GetProcThreadCount(DWORD ProcId);

    /* ��ȡ������������ڴ� */
    DWORD GetProcMem(DWORD ProcId);

    /* ��ȡ�������CPU */
    int GetPrcoCpu(void);

    /* ��ȡ��������ʱ�� */
    CString GetProcStartTime(void);

    /* �жϷ�����̵�״�� */
    bool JudgeProcStatus(void);

    /* ��ʼ���������CPU��FILETIME */
    bool InitProcFileTime(void);

    /* ��ȡ���̵���Ϣ��ѯHandle */
    bool GetProcHandle(void);

    bool CheckSvcPerformanceParam(void);

    /* �������Ƿ�ʱ����/ֹͣ */
    bool TimeToSvcStartOrStop(void);
    /* ���÷���������ʱ�� */
    void SetSvcLastTime(void);

    /* ��ȡWinSvc handle */
    bool GetServiceHandle(SC_HANDLE& scManagerHandle, SC_HANDLE& scServiceHandle);
    //}// add end
protected:
    /*
    * 	@fn       bool ServiceController::GetWinSvcHandle(DWORD& commandError)
    * 	@brief    ��ȡ Windows SCManager �� Windows Service ���
    * 	@param    (DWORD) commandError (OUT):ִ��ʧ������
    * 	@return:  bool: true:�ɹ� ; false:ʧ��
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
    * 	@brief    �� Windows SCManager �� Windows Service ���
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
    * 	@brief    ��ѯ Windows Service ��ǰ״̬
    * 	@param    (SC_HANDLE) schService (IN): Windows Service ���
    *   @param    (SERVICE_STATUS_PROCESS) status (OUT): Windows Service ״̬
//    *   @param    (bool) setStatus (IN): �Ƿ����õ�ǰ����״̬��Ĭ��ִ�����ò���
    * 	@return:  bool: true:�ɹ� ; false:ʧ��
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
    * 	@brief    ���� Windows Service ״̬�ı���Ϣ
    * 	@param    (DWORD) status (IN):  Windows Service ��״̬
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
    * 	@brief    ���� ServiceController ִ����������в������쳣����
    * 	@param    (DWORD) error [IN]:  �쳣����
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
    * 	@brief    �������
    * 	@param    (list<control_command_t>) commands [IN]: �����б�
    * 	@return:  bool: true:�ɹ� ; false:ʧ��
    */
    inline bool PushControlCommand(list<control_command_t>& commands)
    {
        bool rtn = false;

        if (m_bRunning)
        {
            EnterCriticalSection(&m_csControlCommand);

            if (m_bRunning)
            {
                // ���������������С�
                for (list<control_command_t>::iterator itor = commands.begin(); itor != commands.end(); itor++)
                {
                    m_queueControlCommand.push(*itor);
                }

                // ���ѹ����̡߳�
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
    * 	@brief    ��ͣWindows Service 
    * 	@param    void 
    * 	@return:  bool: true:��ͣWindows Service�ɹ�; false:��ͣWindows Serviceʧ��
    */
    inline bool PauseSvc(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_pause_service);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::ContinueSvc(void)
    * 	@brief    �ָ�Windows Service
    * 	@param    void
    * 	@return:  bool: true:�ָ��ɹ�; false:�ָ�ʧ��
    */
    inline bool ContinueSvc(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_continue_service);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::StartSvc(void)
    * 	@brief    ����Windows Service
    * 	@param    void
    * 	@return:  bool: true:�����ɹ�; false:����ʧ��
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
     *  @brief    �����������״̬
     *  @param    void
     *  @return:  bool��true:����������ӳɹ� ; false:ʧ��
     */
    inline bool StartSvcStatus(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_start_status_chg);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::StopSvc(void)
    * 	@brief    ֹͣ Windows Service
    * 	@param    void
    * 	@return:  bool: true:ֹͣ Windows Service�ɹ� ; false:ֹͣ Windows Serviceʧ��
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
     *  @brief    ������ֹͣ״̬
     *  @param    void
     *  @return:  bool��true:����������ӳɹ� ; false:ʧ��
     */
    inline bool StopSvcStatus(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_stop_status_chg);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       bool ServiceController::RestartSvc(void)
    * 	@brief    ���� Windows Service
    * 	@param    void
    * 	@return:  bool: true:���� Windows Service �ɹ�; false:���� Windows Service ʧ��
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
    * 	@brief    ��ѯ Windows Service ��ǰ״̬
    * 	@param    void 
    * 	@return:  bool: true:��ѯ Windows Service ��ǰ״̬�ɹ� ; false:��ѯ Windows Service ��ǰ״̬ʧ��
    */
    inline bool QuerySvcStatus(void)
    {
        list<control_command_t> commands;
        commands.push_back(control_command_query_service_status);

        return PushControlCommand(commands);
    }

    /*
    * 	@fn       DWORD ServiceController::getServiceStatus(void)
    * 	@brief    ��ȡ Windows Service ��ǰ״̬
    * 	@param    void
    * 	@return:  DWORD:Windows Service ��ǰ״̬
    */
    inline DWORD getServiceStatus(void)
    {
        return m_svcStatus;
    }

    /*
    * 	@fn       bool ServiceController::getServiceExists(void)
    * 	@brief    �жϷ������Ƿ��Ѿ�����
    * 	@param    void
    * 	@return:  bool:true:���ڣ�false:������
    */
    inline bool getServiceExists(void)
    {
        return (CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST != m_svcStatus);
    }

    /*
    * 	@fn       void ServiceController::setPollInterval(int poll)
    * 	@brief    ���� ServiceController �������ڼ��ʱ�䣨s��
    * 	@param    (int) poll [IN]: ServiceController �������ڼ��ʱ�䣨s��
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
    * 	@brief    ��ȡ ServiceController �������ڼ��ʱ�䣨s��
    * 	@param    void
    * 	@return:  int: ServiceController �������ڼ��ʱ�䣨s��
    */
    inline int getPollInterval(void) const
    {
        return m_pollInterval;
    }

};
