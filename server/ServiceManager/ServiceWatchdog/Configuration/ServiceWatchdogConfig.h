#pragma once
//
// Copyright (c) 2009 Hikvision Digital Technology Co., Ltd.
//
// Summary:
//     Service watchdog application configuration .
//
// Modification History:
//   2009-11-17:
//     V1.0 / 胡通海 / 创建
//
#include <string>
#include <list>
#include <vector>
using namespace std;

//
// Summary:
//     ServiceController 默认工作间隔时间 5s 。
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
//     Service 配置节。
//
class ServiceConfig
{
public:
    //
    // Summary:
    //     服务注册名称。
    //
    string m_svcName;
    //
    // Summary:
    //     服务显示名称。
    //
    string m_svcDisplayName;
    //
    // Summary:
    //     服务显示简写名称。
    //
    string m_svcShortName;
    //
    // Summary:
    //     Watchdog 检查服务状态时间间隔（秒）。
    //
    int m_pollInterval;

    //{//mazhentong add
    DWORD m_nModuleType;        ///< 服务器的类型
    string m_svcPath;           ///< 服务器相对路径
    string m_svcStartCmd;       ///< 服务器启动命令bat
    string m_svcStopCmd;        ///< 服务器停止命令bat
    string m_svcInstallCmd;     ///< 服务器安装命令bat
    string m_svcUninstallCmd;   ///< 服务器卸载命令bat
    string m_svcConfigToolName; ///< 服务器配置工具

    string m_svcStartTime;      ///< 定时启动时间
    string m_svcStopTime;       ///< 定时停止时间

    DWORD m_svcStartTimeOut;    ///< 服务的启动超时等待时间
    DWORD m_svcStopTimeOut;     ///< 服务的停止超时等待时间
    //}//add end

public:
    //
    // Summary:
    //     创建 ServiceConfig 新实例。
    //
    ServiceConfig(void)
        : m_pollInterval(SERVICE_CONFIG_POLL_INTERVAL_DEFAULT)
    {
        m_svcStartTimeOut = MAX_START_TIME_OUT;
        m_svcStopTimeOut = MAX_STOP_TIME_OUT;
    }
    //
    // Summary:
    //     根据 svcName, svcDisplayName 创建 ServiceConfig 新实例。
    //
    // Parameters:
    //   [in] svcName:
    //     服务注册名称。
    //   [in] svcDisplayName:
    //     服务显示名称。
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
     *  @brief    设置服务名
     *  @param    (const char*) svcName [IN] : 服务名
     *  @return:  void
     */
    inline void SetSvcName(const char* svcName)
    {
        m_svcName = svcName ? svcName : "";
    }

    /** @fn       void ServiceConfig::SetSvcDispName(const char* svcDisplayName)
     *  @brief    设置服务显示名
     *  @param    (const char*) svcDisplayName [IN] : 服务显示名
     *  @return:  void
     */
    inline void SetSvcDispName(const char* svcDisplayName)
    {
        m_svcDisplayName = svcDisplayName ? svcDisplayName : "";
    }

    /** @fn       void ServiceConfig::SetSvcShortName(const char* svcShortName)
     *  @brief    设置服务名缩写
     *  @param    (const char*) svcShortName [IN] : 设置服务名缩写
     *  @return:  void
     */
    inline void SetSvcShortName(const char* svcShortName)
    {
        m_svcShortName = svcShortName ? svcShortName : "";
    }

    /** @fn       void ServiceConfig::SetSvcStartCmd(const char* svcStartCmd)
     *  @brief    设置服务的开始命令
     *  @param    (const char*) svcStartCmd [IN] : 开始命令
     *  @return:  void
     */
    inline void SetSvcStartCmd(const char* svcStartCmd)
    {
        m_svcStartCmd = svcStartCmd ? svcStartCmd : "";
    }

    /** @fn       void ServiceConfig::SetsvcStopCmd(const char* svcStopCmd)
     *  @brief    设置服务的停止命令
     *  @param    (const char*) svcStopCmd [IN] : 停止命令
     *  @return:  void
     */
    inline void SetsvcStopCmd(const char* svcStopCmd)
    {
        m_svcStopCmd = svcStopCmd ? svcStopCmd : "";
    }

    /** @fn       void ServiceConfig::SetSvcInstallCmd(const char* svcInstallCmd)
     *  @brief    设置服务的安装服务命令
     *  @param    (const char*) svcInstallCmd [IN] : 安装命令
     *  @return:  void
     */
    inline void SetSvcInstallCmd(const char* svcInstallCmd)
    {

        m_svcInstallCmd = svcInstallCmd ? svcInstallCmd : "";
    }

    /** @fn       void ServiceConfig::SetSvcUninstallCmd(const char* svcUninstallCmd)
     *  @brief    设置服务的卸载命令
     *  @param    (const char*) svcUninstallCmd [IN] : 卸载命令
     *  @return:  void
     */
    inline void SetSvcUninstallCmd(const char* svcUninstallCmd)
    {

        m_svcUninstallCmd = svcUninstallCmd ? svcUninstallCmd : "";
    }
    
    /** @fn       void ServiceConfig::SetModuleType(DWORD dwModuleType)
     *  @brief    设置服务的模块类型
     *  @param    (DWORD) dwModuleType [IN] : 服务的模块类型 
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
     *  @brief    设置配置工具名
     *  @param    (const char*) svcConfigToolName [IN] : 配置工具名
     *  @return:  void
     */
    inline void SetSvcConfigToolName(const char* svcConfigToolName)
    {
        m_svcConfigToolName = svcConfigToolName ? svcConfigToolName : "";

    }

    /** @fn       void ServiceConfig::SetSvcStartTime(const char* svcStartTime)
     *  @brief    设置定时启动时间
     *  @param    (const char*) svcStartTime [IN] : 定时启动时间
     *  @return:  void
     */
    inline void SetSvcStartTime(const char* svcStartTime)
    {
        m_svcStartTime = svcStartTime ? svcStartTime : "";
    }

    /** @fn       void ServiceConfig::SetSvcStopTime(const char* svcStopTime)
     *  @brief    设置定时停止时间
     *  @param    (const char*) svcStopTime [IN] : 定时停止时间
     *  @return:  void
     */
    inline void SetSvcStopTime(const char* svcStopTime)
    {
        m_svcStopTime = svcStopTime ? svcStopTime : "";
    }

    /** @fn       void ServiceConfig::SetSvcStartTimeOut(DWORD startTimeOut)
     *  @brief    设置启动超时等待时间
     *  @param    (DWORD) startTimeOut [IN] : 启动超时等待时间
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
     *  @brief    设置停止超时等待时间
     *  @param    (DWORD) stopTimeOut [IN] : 停止超时等待时间
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
    //     析构 ServiceConfig 实例。
    //
    ~ServiceConfig(void)
    {
    }

};

//
// Summary:
//     ServiceWatchdog 配置。
//
class ServiceWatchdogConfig
{
public:
    //
    // Summary:
    //     Service 实例列表。
    //
    std::list<ServiceConfig> m_lstAllService;    //所有Service的列表
    std::list<CString> m_listAllSubDir;    //所有子文件夹

    int m_maxProcessorTime;
    int m_maxVirtualMemoryUsage;
    int m_maxHandleCount;

protected:
    //
    // Summary:
    //     构造 ServiceWatchdogConfig 新实例。
    //
    ServiceWatchdogConfig(void);
    //
    // Summary:
    //     析构 ServiceWatchdogConfig 实例。
    //
    ~ServiceWatchdogConfig(void);
    //
    // Summary:
    //     获取 ServiceWatchdogConfig 唯一静态实例。
    //
    // Returns:
    //     ServiceWatchdogConfig 唯一静态实例。
    //
    static ServiceWatchdogConfig& StaticInstance(void);
    //
    // Summary:
    //     根据 content xml 内容构造 ServiceWatchdogConfig 实例。
    //
    // Parameters:
    //   [in] content:
    //     ServiceWatchdogConfig 实例持久 xml 文本。
    //
    //void Deserialize(const char* content);

    //add mzt
    void DeserializeAll(const char* fileDir);
    /* 文件夹列表 */
    void GetSubDir(const char* fileDir);
    /* 检查Service的bat文件是否存在 */
    void CheckSerBatFileIsorNotExist(const char* fileDir, ServiceConfig& SerCfg);

public:
    //
    // Summary:
    //     获取 ServiceWatchdogConfig 唯一实例。
    //
    // Returns:
    //     ServiceWatchdogConfig 实例。
    //
    inline static ServiceWatchdogConfig& Instance(void)
    {
        return StaticInstance();
    }
};
