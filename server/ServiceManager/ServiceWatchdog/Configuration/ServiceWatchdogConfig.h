#pragma once
//
// Copyright (c) 2009 Hikvision Digital Technology Co., Ltd.
//
// Summary:
//     Service watchdog application configuration .
//
// Modification History:
//   2009-11-17:
//     V1.0 / ��ͨ�� / ����
//
#include <string>
#include <list>
#include <vector>
using namespace std;

//
// Summary:
//     ServiceController Ĭ�Ϲ������ʱ�� 5s ��
//
#define SERVICE_CONFIG_POLL_INTERVAL_DEFAULT    3
#define SERVICE_CONFIG_MAX_PROCESSOR_TIME       100000
#define SERVICE_CONFIG_MAX_VIRTUAL_MEMORY_USAGE (2*1024*1024)
#define SERVICE_CONFIG_MAX_HANDLE_COUNT         20000

static const DWORD MIN_START_TIME_OUT   = 30;
static const DWORD MAX_START_TIME_OUT   = 600;
static const DWORD MIN_STOP_TIME_OUT    = 10;
static const DWORD MAX_STOP_TIME_OUT    = 600;

//
// Summary:
//     Service ���ýڡ�
//
class ServiceConfig
{
public:
    //
    // Summary:
    //     ����ע�����ơ�
    //
    string m_svcName;
    //
    // Summary:
    //     ������ʾ���ơ�
    //
    string m_svcDisplayName;
    //
    // Summary:
    //     ������ʾ��д���ơ�
    //
    string m_svcShortName;
    //
    // Summary:
    //     Watchdog ������״̬ʱ�������룩��
    //
    int m_pollInterval;

    //{//mazhentong add
    DWORD m_nModuleType;        ///< ������������
    string m_svcPath;           ///< ���������·��
    string m_svcStartCmd;       ///< ��������������bat
    string m_svcStopCmd;        ///< ������ֹͣ����bat
    string m_svcInstallCmd;     ///< ��������װ����bat
    string m_svcUninstallCmd;   ///< ������ж������bat
    string m_svcConfigToolName; ///< ���������ù���

    string m_svcStartTime;      ///< ��ʱ����ʱ��
    string m_svcStopTime;       ///< ��ʱֹͣʱ��

    DWORD m_svcStartTimeOut;    ///< �����������ʱ�ȴ�ʱ��
    DWORD m_svcStopTimeOut;     ///< �����ֹͣ��ʱ�ȴ�ʱ��
    //}//add end

public:
    //
    // Summary:
    //     ���� ServiceConfig ��ʵ����
    //
    ServiceConfig(void)
        : m_pollInterval(SERVICE_CONFIG_POLL_INTERVAL_DEFAULT)
    {
        m_svcStartTimeOut = MAX_START_TIME_OUT;
        m_svcStopTimeOut = MAX_STOP_TIME_OUT;
    }
    //
    // Summary:
    //     ���� svcName, svcDisplayName ���� ServiceConfig ��ʵ����
    //
    // Parameters:
    //   [in] svcName:
    //     ����ע�����ơ�
    //   [in] svcDisplayName:
    //     ������ʾ���ơ�
    //
    ServiceConfig(const char* svcName, const char* svcDisplayName, const char* svcShortName,
        const char* svcStartCmd, const char* svcStopCmd, const char* svcInstallCmd, const char* svcUninstallCmd,
        const char* svcConfigToolName, const char* svcStartTime, const char* svcStopTime)
        : m_pollInterval(SERVICE_CONFIG_POLL_INTERVAL_DEFAULT)
    {
        m_svcName = svcName ? svcName : "";
        m_svcDisplayName = svcDisplayName ? svcDisplayName : "";
		m_svcShortName = svcShortName ? svcShortName : "";

        //{//mzt add
        m_svcStartCmd = svcStartCmd ? svcStartCmd : "";
        m_svcStopCmd = svcStopCmd ? svcStopCmd : "";
        m_svcInstallCmd = svcInstallCmd ? svcInstallCmd : "";
        m_svcUninstallCmd = svcUninstallCmd ? svcUninstallCmd : "";
        m_svcConfigToolName = svcConfigToolName ? svcConfigToolName : "";

        m_svcStartTime = svcStartTime ? svcStartTime : "";
        m_svcStopTime = svcStopTime ? svcStopTime : "";
        //}//end add
    }
//{// add
    /** @fn       void ServiceConfig::SetSvcName(const char* svcName)
     *  @brief    ���÷�����
     *  @param    (const char*) svcName [IN] : ������
     *  @return:  void
     */
    inline void SetSvcName(const char* svcName)
    {
        m_svcName = svcName ? svcName : "";
    }

    /** @fn       void ServiceConfig::SetSvcDispName(const char* svcDisplayName)
     *  @brief    ���÷�����ʾ��
     *  @param    (const char*) svcDisplayName [IN] : ������ʾ��
     *  @return:  void
     */
    inline void SetSvcDispName(const char* svcDisplayName)
    {
        m_svcDisplayName = svcDisplayName ? svcDisplayName : "";
    }

    /** @fn       void ServiceConfig::SetSvcShortName(const char* svcShortName)
     *  @brief    ���÷�������д
     *  @param    (const char*) svcShortName [IN] : ���÷�������д
     *  @return:  void
     */
    inline void SetSvcShortName(const char* svcShortName)
    {
        m_svcShortName = svcShortName ? svcShortName : "";
    }

    /** @fn       void ServiceConfig::SetSvcStartCmd(const char* svcStartCmd)
     *  @brief    ���÷���Ŀ�ʼ����
     *  @param    (const char*) svcStartCmd [IN] : ��ʼ����
     *  @return:  void
     */
    inline void SetSvcStartCmd(const char* svcStartCmd)
    {
        m_svcStartCmd = svcStartCmd ? svcStartCmd : "";
    }

    /** @fn       void ServiceConfig::SetsvcStopCmd(const char* svcStopCmd)
     *  @brief    ���÷����ֹͣ����
     *  @param    (const char*) svcStopCmd [IN] : ֹͣ����
     *  @return:  void
     */
    inline void SetsvcStopCmd(const char* svcStopCmd)
    {
        m_svcStopCmd = svcStopCmd ? svcStopCmd : "";
    }

    /** @fn       void ServiceConfig::SetSvcInstallCmd(const char* svcInstallCmd)
     *  @brief    ���÷���İ�װ��������
     *  @param    (const char*) svcInstallCmd [IN] : ��װ����
     *  @return:  void
     */
    inline void SetSvcInstallCmd(const char* svcInstallCmd)
    {

        m_svcInstallCmd = svcInstallCmd ? svcInstallCmd : "";
    }

    /** @fn       void ServiceConfig::SetSvcUninstallCmd(const char* svcUninstallCmd)
     *  @brief    ���÷����ж������
     *  @param    (const char*) svcUninstallCmd [IN] : ж������
     *  @return:  void
     */
    inline void SetSvcUninstallCmd(const char* svcUninstallCmd)
    {

        m_svcUninstallCmd = svcUninstallCmd ? svcUninstallCmd : "";
    }
    
    /** @fn       void ServiceConfig::SetModuleType(DWORD dwModuleType)
     *  @brief    ���÷����ģ������
     *  @param    (DWORD) dwModuleType [IN] : �����ģ������ 
     *  @return:  void
     */
    inline void SetModuleType(DWORD dwModuleType)
    {
        m_nModuleType = dwModuleType;
    }

    inline void SetSvcPath(const char* svcPath)
    {
        m_svcPath = svcPath ? svcPath : "";
    }

    /** @fn       void ServiceConfig::SetSvcConfigToolName(const char* svcConfigToolName)
     *  @brief    �������ù�����
     *  @param    (const char*) svcConfigToolName [IN] : ���ù�����
     *  @return:  void
     */
    inline void SetSvcConfigToolName(const char* svcConfigToolName)
    {
        m_svcConfigToolName = svcConfigToolName ? svcConfigToolName : "";

    }

    /** @fn       void ServiceConfig::SetSvcStartTime(const char* svcStartTime)
     *  @brief    ���ö�ʱ����ʱ��
     *  @param    (const char*) svcStartTime [IN] : ��ʱ����ʱ��
     *  @return:  void
     */
    inline void SetSvcStartTime(const char* svcStartTime)
    {
        m_svcStartTime = svcStartTime ? svcStartTime : "";
    }

    /** @fn       void ServiceConfig::SetSvcStopTime(const char* svcStopTime)
     *  @brief    ���ö�ʱֹͣʱ��
     *  @param    (const char*) svcStopTime [IN] : ��ʱֹͣʱ��
     *  @return:  void
     */
    inline void SetSvcStopTime(const char* svcStopTime)
    {
        m_svcStopTime = svcStopTime ? svcStopTime : "";
    }

    /** @fn       void ServiceConfig::SetSvcStartTimeOut(DWORD startTimeOut)
     *  @brief    ����������ʱ�ȴ�ʱ��
     *  @param    (DWORD) startTimeOut [IN] : ������ʱ�ȴ�ʱ��
     *  @return:  void
     */
    inline void SetSvcStartTimeOut(DWORD startTimeOut)
    {
        m_svcStartTimeOut = startTimeOut;
        
        if (m_svcStartTimeOut < MIN_START_TIME_OUT)
        {
            m_svcStartTimeOut = MIN_START_TIME_OUT;
        }
        else if (m_svcStartTimeOut > MAX_START_TIME_OUT)
        {
            m_svcStartTimeOut = MAX_START_TIME_OUT;
        }
    }

    /** @fn       void ServiceConfig::SetSvcStopTimeOut(DWORD stopTimeOut)
     *  @brief    ����ֹͣ��ʱ�ȴ�ʱ��
     *  @param    (DWORD) stopTimeOut [IN] : ֹͣ��ʱ�ȴ�ʱ��
     *  @return:  void
     */
    inline void SetSvcStopTimeOut(DWORD stopTimeOut)
    {
        m_svcStopTimeOut = stopTimeOut;

        if (m_svcStopTimeOut < MIN_STOP_TIME_OUT)
        {
            m_svcStopTimeOut = MIN_STOP_TIME_OUT;
        }
        else if (m_svcStopTimeOut > MAX_STOP_TIME_OUT)
        {
            m_svcStopTimeOut = MAX_STOP_TIME_OUT;
        }
    }
//{// add end
    //
    // Summary:
    //     ���� ServiceConfig ʵ����
    //
    ~ServiceConfig(void)
    {
    }

};

//
// Summary:
//     ServiceWatchdog ���á�
//
class ServiceWatchdogConfig
{
public:
    //
    // Summary:
    //     Service ʵ���б�
    //
    std::list<ServiceConfig> m_lstAllService;    //����Service���б�
    std::list<CString> m_listAllSubDir;    //�������ļ���

    int m_maxProcessorTime;
    int m_maxVirtualMemoryUsage;
    int m_maxHandleCount;

protected:
    //
    // Summary:
    //     ���� ServiceWatchdogConfig ��ʵ����
    //
    ServiceWatchdogConfig(void);
    //
    // Summary:
    //     ���� ServiceWatchdogConfig ʵ����
    //
    ~ServiceWatchdogConfig(void);
    //
    // Summary:
    //     ��ȡ ServiceWatchdogConfig Ψһ��̬ʵ����
    //
    // Returns:
    //     ServiceWatchdogConfig Ψһ��̬ʵ����
    //
    static ServiceWatchdogConfig& StaticInstance(void);
    //
    // Summary:
    //     ���� content xml ���ݹ��� ServiceWatchdogConfig ʵ����
    //
    // Parameters:
    //   [in] content:
    //     ServiceWatchdogConfig ʵ���־� xml �ı���
    //
    //void Deserialize(const char* content);

    //add mzt
    void DeserializeAll(const char* fileDir);
    /* �ļ����б� */
    void GetSubDir(const char* fileDir);
    /* ���Service��bat�ļ��Ƿ���� */
    void CheckSerBatFileIsorNotExist(const char* fileDir, ServiceConfig& SerCfg);

public:
    //
    // Summary:
    //     ��ȡ ServiceWatchdogConfig Ψһʵ����
    //
    // Returns:
    //     ServiceWatchdogConfig ʵ����
    //
    inline static ServiceWatchdogConfig& Instance(void)
    {
        return StaticInstance();
    }
};
