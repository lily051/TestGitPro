/*****************************************************
*  Copyright 2009 Hikvision Digital Technology Co., Ltd.
*
*  FileName:
*      ServiceWatchdogConfig.cpp
*
*  Description:
*      Service watchdog application configuration .
*
*  Modification History:
*    <version>    <time>        <author>      <desc>
*    1.0          2009-11-17    Hu Tonghai    Create
*****************************************************/
#include "StdAfx.h"
#include "ServiceWatchdogConfig.h"
#include "ServiceWatchdog.h"

#include "Folder.h"
#include "Markup.h"

/***************************************************** 
*  Function: 
*      ServiceWatchdogConfig
*
*  Description: 
*      构造 ServiceWatchdogConfig 新实例。
*
*  Parameter: 
*      (null)
*  
*  Return: 
*      (null)
*****************************************************/
ServiceWatchdogConfig::ServiceWatchdogConfig(void)
: m_maxProcessorTime(SERVICE_CONFIG_MAX_PROCESSOR_TIME)
, m_maxVirtualMemoryUsage(SERVICE_CONFIG_MAX_VIRTUAL_MEMORY_USAGE)
, m_maxHandleCount(SERVICE_CONFIG_MAX_HANDLE_COUNT)
{
    //Deserialize(CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceWatchdog.dat"));
    //DeserializeAll(CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceModuleType.dat"));
    DeserializeAll(CFolder::GetExecutionRoot());
}

/***************************************************** 
*  Function: 
*      ~ServiceWatchdogConfig
*
*  Description: 
*      析构 ServiceWatchdogConfig 实例。
*
*  Parameter: 
*      (null)
*  
*  Return: 
*      (null)
*****************************************************/
ServiceWatchdogConfig::~ServiceWatchdogConfig(void)
{
}

/***************************************************** 
*  Function: 
*      StaticInstance
*
*  Description: 
*      获取 ServiceWatchdogConfig 唯一静态实例。
*
*  Parameter: 
*      (null)
*  
*  Return: 
*      ServiceWatchdogConfig 唯一静态实例。
*****************************************************/
ServiceWatchdogConfig& ServiceWatchdogConfig::StaticInstance(void)
{
    static ServiceWatchdogConfig instance;

    return instance;
}

/***************************************************** 
*  Function: 
*      Deserialize
*
*  Description: 
*      根据 content xml 内容构造 ServiceWatchdogConfig 实例。
*
*  Parameter: 
*    [in] content:
*      ServiceWatchdogConfig 实例持久 xml 文本。
*  
*  Return: 
*      (null)
*****************************************************/
/*void ServiceWatchdogConfig::Deserialize(const char* content)
{
    if (!content)
    {
        return;
    }

    m_lstServices.clear();

    CMarkup xmlDoc;
    if (true == xmlDoc.SetDoc(content))
    {
        if (xmlDoc.FindElem("Configuration"))
        {
            if (xmlDoc.IntoElem())
            {
                if (xmlDoc.FindElem("Services") && xmlDoc.IntoElem())
                {
                    while(xmlDoc.FindElem("Service"))
                    {
						ServiceConfig sc(xmlDoc.GetAttrib("name"), xmlDoc.GetData(), xmlDoc.GetAttrib(_T("shortname")),
                            xmlDoc.GetAttrib(_T("path")), xmlDoc.GetAttrib(_T("startCmd")), xmlDoc.GetAttrib(_T("stopCmd")),
                            xmlDoc.GetAttrib(_T("installCmd")), xmlDoc.GetAttrib(_T("uninstallCmd")), xmlDoc.GetAttrib(_T("configTool")));

                        sc.m_pollInterval = atoi(xmlDoc.GetAttrib("poll"));

                        //{//mzt add
                        sc.m_nModuleType = atoi(xmlDoc.GetAttrib("ModuleType"));
                        //}//end add

                        m_lstServices.push_back(sc);
                    }

                    xmlDoc.OutOfElem();
                }

                if (xmlDoc.FindElem("Watchdog") && xmlDoc.IntoElem())
                {
                    if (xmlDoc.FindElem("MaxProcessorTime"))
                    {
                        m_maxProcessorTime = atoi(xmlDoc.GetData());
                    }

                    if (xmlDoc.FindElem("MaxVirtualMemoryUsage"))
                    {
                        m_maxVirtualMemoryUsage = atoi(xmlDoc.GetData());
                    }

                    if (xmlDoc.FindElem("MaxHandleCount"))
                    {
                        m_maxHandleCount = atoi(xmlDoc.GetData());
                    }

                    //xmlDoc.OutOfElem();
                }

                //xmlDoc.OutOfElem();
            }

            //xmlDoc.OutOfElem();
        }

        //xmlDoc.OutOfElem();
    }
}
*/

/** @fn       void ServiceWatchdogConfig::DeserializeAll(const char* content)
 *  @brief    更据content xml 内容获取m_lstAllService
 *  @param    (const char*) content [IN] : ServiceWatchdogConfig 实例持久 xml 文本
 *  @return:  void
 */
void ServiceWatchdogConfig::DeserializeAll(const char* fileDir)
{
    if (strcmp(fileDir, _T("")) == 0)
    {
        return;
    }

    GetSubDir(fileDir);

    if (m_listAllSubDir.size() <= 0)
    {
        return;
    }

    m_lstAllService.clear();
    // 查看子目录是不是有ServiceModuleConfig.dat配置文件
    for (std::list<CString>::iterator itor = m_listAllSubDir.begin(); itor != m_listAllSubDir.end(); itor++)
    {
        CString SubDirName = *itor;
        CString csFileName;
        csFileName.Format(_T("%s%s\\%s"), fileDir, SubDirName.GetBuffer(), _T("ServiceModuleConfig.dat"));
        SubDirName.ReleaseBuffer();

        // 记录配置文件的目录
        SVCMANAGERLOG_INFO(_T("%s"), (LPCTSTR)csFileName);

        CString fileContent = CFolder::ReadFromUTF8File(csFileName);
        if (strcmp(fileContent, _T("")) == 0)
        {
            continue;
        }
       
        { // 获取配置
            CMarkup xmlDoc;
            if (!xmlDoc.SetDoc(fileContent.GetBuffer(0)))
            {
                continue;
            }
            if (!xmlDoc.FindElem("Configuration") || !xmlDoc.IntoElem())
            {
                continue;
            }
            if (!xmlDoc.FindElem("Service"))
            {
                continue;
            }

            ServiceConfig svcCfg;
            svcCfg.SetSvcName(xmlDoc.GetAttrib(_T("name")));

            svcCfg.SetSvcStartCmd(xmlDoc.GetAttrib(_T("startCmd")));
            svcCfg.SetsvcStopCmd(xmlDoc.GetAttrib(_T("stopCmd")));
            svcCfg.SetSvcInstallCmd(xmlDoc.GetAttrib(_T("installCmd")));
            svcCfg.SetSvcUninstallCmd(xmlDoc.GetAttrib(_T("uninstallCmd")));
            svcCfg.SetSvcConfigToolName(xmlDoc.GetAttrib(_T("configTool")));

            svcCfg.SetModuleType(atoi(xmlDoc.GetAttrib("ModuleType")));
            svcCfg.SetSvcPath(SubDirName);
            svcCfg.SetSvcDispName(xmlDoc.GetData());

            //check bat file of ServiceConfig is or not exist
            CheckSerBatFileIsorNotExist(fileDir, svcCfg);

            // 定时时间
            if (xmlDoc.FindElem(_T("Times")))
            {
                if (xmlDoc.FindChildElem(_T("StartTime")))
                {// 定时启动时间
                    svcCfg.SetSvcStartTime(xmlDoc.GetChildData());
                }

                if (xmlDoc.FindChildElem(_T("StopTime")))
                {// 定时停止时间
                    svcCfg.SetSvcStopTime(xmlDoc.GetChildData());
                }
            }

            // 超时时间
            if (xmlDoc.FindElem(_T("Timeout")))
            {
                if (xmlDoc.FindChildElem(_T("StartTimeout")))
                {// 启动超时等待时间
                    svcCfg.SetSvcStartTimeOut(atoi(xmlDoc.GetChildData()));
                }

                if (xmlDoc.FindChildElem(_T("StopTimeout")))
                {// 停止超时等待时间
                    svcCfg.SetSvcStopTimeOut(atoi(xmlDoc.GetChildData()));
                }
            }

            m_lstAllService.push_back(svcCfg);

            xmlDoc.OutOfElem();
        }
    }
}

/** @fn       void ServiceWatchdogConfig::GetSubDir(const char* fileDir)
 *  @brief    获取子文件夹
 *  @param    (const char*) fileDir [IN] : 目录
 *  @return:  void
 */
void ServiceWatchdogConfig::GetSubDir(const char* fileDir)
{
    m_listAllSubDir.clear();

    if (strcmp(fileDir, _T("")) == 0)
    {
        return;
    }

    WIN32_FIND_DATA fd;
    CString csfileDir;
    csfileDir.Format(_T("%s"), fileDir);
    HANDLE hFind = ::FindFirstFile(csfileDir+"\\*.*", &fd); 

    // 遍历文件目录
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ( (strcmp(fd.cFileName, _T(".")) == 0) || (strcmp(fd.cFileName, _T("..")) == 0))
            {
                continue;
            }

            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                CString csFileName;
                csFileName.Format(_T("%s"), fd.cFileName);
                
                CString FilePath = csfileDir;
                FilePath.AppendFormat(_T("%s\\ServiceModuleConfig.dat"), (LPCTSTR)csFileName);
                if (CFolder::FileExists(FilePath))
                {
                    m_listAllSubDir.push_back(csFileName);
                }
            }
        } while (::FindNextFile(hFind, &fd)); 

        ::FindClose(hFind); 
    }
    else
    {
        SVCMANAGERLOG_ERROR(_T("获取文件夹列表错误，ERROR[%d]\n"), GetLastError());
    }
}

/** @fn       void ServiceWatchdogConfig::CheckSerBatFileIsorNotExist(const char* fileDir, ServiceConfig* pSerCfg)
 *  @brief    检查Service的bat文件和配置工具文件是否存在
 *  @param    (const char *) fileDir [IN] : 可运行文件的目录
 *            (ServiceConfig*) pSerCfg [IN] : Service配置
 *  @return:  void
 */
void ServiceWatchdogConfig::CheckSerBatFileIsorNotExist(const char* fileDir, ServiceConfig& SerCfg)
{
    /* 文件路径 */
    CString FilePath;

    /* Start bat */
    if (!SerCfg.m_svcStartCmd.empty())
    {
        FilePath.Format(_T("%s%s\\%s"), fileDir, SerCfg.m_svcPath.c_str(), SerCfg.m_svcStartCmd.c_str());
        if (!CFolder::FileExists(FilePath))
        {
            SerCfg.SetSvcStartCmd("");
        }
    }

    /* Stop bat */
    if (!SerCfg.m_svcStopCmd.empty())
    {
        FilePath.Format(_T("%s%s\\%s"), fileDir, SerCfg.m_svcPath.c_str(), SerCfg.m_svcStopCmd.c_str());
        if (!CFolder::FileExists(FilePath))
        {
            SerCfg.SetsvcStopCmd("");
        }
    }

    /* Install bat */
    if (!SerCfg.m_svcInstallCmd.empty())
    {
        FilePath.Format(_T("%s%s\\%s"), fileDir, SerCfg.m_svcPath.c_str(), SerCfg.m_svcInstallCmd.c_str());
        if (!CFolder::FileExists(FilePath))
        {
            SerCfg.SetSvcInstallCmd("");
        }
    }
    
    /* Uninstall bat */
    if (!SerCfg.m_svcUninstallCmd.empty())
    {
        FilePath.Format(_T("%s%s\\%s"), fileDir, SerCfg.m_svcPath.c_str(), SerCfg.m_svcUninstallCmd.c_str());
        if (!CFolder::FileExists(FilePath))
        {
            SerCfg.SetSvcUninstallCmd("");
        }
    }

    /* ConfigTool exe */
    if (!SerCfg.m_svcConfigToolName.empty())
    {
        FilePath.Format(_T("%s%s\\%s"), fileDir, SerCfg.m_svcPath.c_str(), SerCfg.m_svcConfigToolName.c_str());
        if (!CFolder::FileExists(FilePath))
        {
            SerCfg.SetSvcConfigToolName("");
        }
    }
}

