#include "StdAfx.h"
#include "Config.h"
#include <HPR_Error.h>
#include <HPR_FileSystem.h>
#include <fclib/fcConfigParser.h>
#include <./SvcGeneralDef.h>
#include <hak.h>
#include "../Tinyxml/tinyxml.h"
#include "../Tinyxml/tinyxmlutils.h"
#include "../util/StringConverter.h"

namespace
{
    TCHAR const INI_SEC_UPLOAD[] = _T("UPLOADCONFIG");
    TCHAR const INI_KEY_LOCALCONFIGENABLE[] = _T("LocalConfigEnable");
    TCHAR const INI_KEY_DELRECORD[] = _T("DelRecord");
    TCHAR const INI_KEY_DELFILE[] = _T("DelFile");
    TCHAR const INI_KEY_UPDATEDATATIME[] = _T("UpdateDataTime");
    INT32 const DEFAULT_UPDATEDATATIME = 30;
    TCHAR const INI_KEY_UPLOADINTEVAL[] = _T("UploadInteval");
    INT32 const DEFAULT_UPLOADINTEVAL = 2;
    TCHAR const INI_KEY_STATUSINTEVAL[] = _T("StatusInteval");
    INT32 const DEFAULT_STATUSINTEVAL = 2;
	TCHAR const INI_KEY_STARTTIMEOFFSET[] = _T("offsetStartTime");
	INT32 const DEFAULT_STARTTIMEOFFSET = 2;
	TCHAR const INI_KEY_STOPTIMEOFFSET[] = _T("offsetStopTime");
	INT32 const DEFAULT_STOPTIMEOFFSET = 2;
    TCHAR const INI_KEY_TASKCOUNT[] = _T("TaskCount");
    INT32 const DEFAULT_TASKCOUNT = 1;
	TCHAR const INI_KEY_UPLOADSPEED[] = _T("UploadSpeed");
	INT32 const DEFAULT_UPLOADSPEED   = 512;
    TCHAR const INI_KEY_STARTTIME[] = _T("StartTime");
    TCHAR const DEFAULT_STARTTIME[] = _T("00:00:00");
    TCHAR const INI_KEY_STOPTIME[] = _T("StopTime");
    TCHAR const DEFAULT_STOPTIME[] = _T("00:00:00");
    TCHAR const INI_KEY_ATSERVERHOST[] = _T("ATServerIP");
    TCHAR const DEFAULT_ATSERVERHOST[] = _T("127.0.0.1");
    TCHAR const INI_KEY_ATSERVERPORT[] = _T("ATServerPort");
    INT32 const DEFAULT_ATSERVERPORT   = 6677;
}

SINGLETON_IMPLEMENT(CConfig)//<单例相关实现

CConfig::CConfig(void)
{
    m_strCenterIP       = "127.0.0.1";
    m_nCenterPort       = 8849;
    m_strATServerIP     = "127.0.0.1";
    m_nATServerPort     = 6677;
    m_isAutoDelRecord   = false;
    m_isAutoDelFile     = false;
    m_bLocalConfig      = false;
    m_nUploadInteval    = 0;
    m_nStatusInteval    = 0;
    m_nUpdateDataTime   = 30;
    m_strStartTime      = "00:00:00";
    m_strStopTime       = "00:00:00";
    m_nTaskCount        = 1;
    m_nMode             = 1;
	m_nUploadSpeed      = 512;
	m_nHaveCenterStorage = 1;
	m_nOffsetStartTime = 2;
	m_nOffsetStopTime  = 2;
}

CConfig::~CConfig(void)
{
//     if (m_hGetCollectEvent != NULL)
//     {
//         CloseHandle(m_hGetCollectEvent);
//         m_hGetCollectEvent = NULL;
//     }
}

/**	@fn	    Deciphering
*	@brief	解密操作
*	@param  [in] strPwd -- 密文密码
*	@param  [out] strPwd -- 明文密码
*	@return	bool
*/
void CConfig::Deciphering(std::string& strPwd)
{
    char aes[1024] = { '\0' };
    int aes_len = 1024;
    if (Hak9((char *)(strPwd.c_str()), NULL, NULL))
    {
        char* out = aes;
        char* outl = (char*)&aes_len;
        Hak7(out, (char *)(strPwd.c_str()), outl);
        strPwd = out;
    }

}

/**	@fn	    LoadLocalCfgInfo
*	@brief	加载本地配置文件
*	@param  [in] 无
*	@param  [out] 无
*	@return	void
*/
void CConfig::LoadLocalCfgInfo(void)
{
    //本地配置
    std::string sAppRunPath, sConfigFile;
    GetAppRunPath(sAppRunPath);
    sConfigFile = sAppRunPath + "UploadConfig.ini";
    CConfigParser config(fcA2T(sConfigFile.c_str()));
    config.SetSection(INI_SEC_UPLOAD);
    m_bLocalConfig = config.GetBooleanValue(INI_KEY_LOCALCONFIGENABLE, false);
    m_isAutoDelFile = config.GetBooleanValue(INI_KEY_DELFILE, false);
    m_isAutoDelRecord = config.GetBooleanValue(INI_KEY_DELRECORD, false);
    m_nUpdateDataTime = config.GetInt32Value(INI_KEY_UPDATEDATATIME, DEFAULT_UPDATEDATATIME);   
    m_nStatusInteval = config.GetInt32Value(INI_KEY_STATUSINTEVAL, DEFAULT_STATUSINTEVAL);
	m_nOffsetStartTime = config.GetInt32Value(INI_KEY_STARTTIMEOFFSET, DEFAULT_STARTTIMEOFFSET);
	m_nOffsetStopTime = config.GetInt32Value(INI_KEY_STOPTIMEOFFSET, DEFAULT_STOPTIMEOFFSET);
    //启用本地配置，不从WEB读取配置信息
    if (m_bLocalConfig)
    {
        m_nUploadInteval = config.GetInt32Value(INI_KEY_UPLOADINTEVAL, DEFAULT_UPLOADINTEVAL);      
        m_strATServerIP = CSCT::Unicode2Ansi(config.GetStringValue(INI_KEY_ATSERVERHOST, DEFAULT_ATSERVERHOST));
        m_nATServerPort = config.GetInt32Value(INI_KEY_ATSERVERPORT, DEFAULT_ATSERVERPORT);
        m_strStartTime = CSCT::Unicode2Ansi(config.GetStringValue(INI_KEY_STARTTIME, DEFAULT_STARTTIME));
        m_strStopTime = CSCT::Unicode2Ansi(config.GetStringValue(INI_KEY_STOPTIME, DEFAULT_STOPTIME));
        m_nTaskCount = config.GetInt32Value(INI_KEY_TASKCOUNT, DEFAULT_TASKCOUNT);
		m_nUploadSpeed = config.GetInt32Value(INI_KEY_UPLOADSPEED, DEFAULT_UPLOADSPEED);
		ULSERVER_LOG_INFO("Load local config.m_nUploadInteval:%d, m_nTaskCount:%d, m_nUploadSpeed:%d, m_strATServerIP:%s, m_nATServerPort:%d, m_strStartTime:%s, m_strStopTime:%s",
            m_nUploadInteval, m_nTaskCount, m_nUploadSpeed, m_strATServerIP.c_str(), m_nATServerPort, m_strStartTime.c_str(), m_strStopTime.c_str());
    }
	ULSERVER_LOG_INFO("Load config.m_bLocalConfig:%d, m_isAutoDelFile:%d, m_isAutoDelRecord:%d, m_nUpdateDataTime:%d, m_nStatusInteval:%d, m_nOffsetStartTime:%d, m_nOffsetStopTime:%d",
        m_bLocalConfig, m_isAutoDelFile, m_isAutoDelRecord, m_nUpdateDataTime, m_nStatusInteval, m_nOffsetStartTime, m_nOffsetStopTime);
}

/**	@fn	    UpdateATSvrInfo
*	@brief	更新系统巡检服务器信息
*	@param  [in] sATSvrIP 巡检服务器IP nATPort巡检服务器端口
*	@param  [out] 无
*	@return	void
*/
void CConfig::UpdateATSvrInfo(const std::string &sATSvrIP, const int &nATPort)
{
    m_strATServerIP = sATSvrIP;
    m_nATServerPort = nATPort;
    ULSERVER_LOG_INFO("ATServer:%s,%d",m_strATServerIP.c_str(),m_nATServerPort);
}

/**	@fn	    GetAppRunPath
*	@brief	获取程序运行路径
*	@param  [in] 无
*	@param  [out] sPathName 程序路径
*	@return	
*/
void CConfig::GetAppRunPath(std::string &sPathName)
{
    TCHAR szFileName[MAX_PATH];
    if (0 != GetModuleFileName(NULL, szFileName, MAX_PATH - 1))
    {
        szFileName[MAX_PATH - 1] = '\0';
    }
    TCHAR * p = _tcsrchr(szFileName, '\\') + 1;

    *p = '\0';
    sPathName.assign(fcW2A(szFileName));
}

/**	@fn	    LoadConfigFile
*	@brief	加载配置文件
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CConfig::LoadConfigFile(void)
{
    LoadLocalCfgInfo();
    std::string sAppRunPath, sConfigFile;
    GetAppRunPath(sAppRunPath);
    sConfigFile = sAppRunPath + TOP_DIRECTORY_CONFIG_FILE;
    TiXmlDocument xmlDoc;
    if (!xmlDoc.LoadFile(sConfigFile))
    {
        ULSERVER_LOG_ERROR("xmlDoc.LoadFile top xml fail");
        return HPR_ERROR;
    }
    TiXmlHandle  docHandle(&xmlDoc);

    //本地IP
    do 
    {
        TiXmlElement * pLocalParamNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_LOCAL_PARAM).ToElement();
        if (pLocalParamNode)
        {
            m_strLocalIP = pLocalParamNode->Attribute("ip");
//             m_sLoginName = pLocalParamNode->Attribute("loginname");
//             m_sLoginPwd = pLocalParamNode->Attribute("loginpwd");
        }
    } while (0);

    //数据中心
    do 
    {
        TiXmlElement * pDataCenterNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_DATA_CENTER).ToElement();
        if (pDataCenterNode)
        {
            m_strCenterIP = pDataCenterNode->Attribute("ip");
            TiXmlUtils::to_number(pDataCenterNode->Attribute("port"),m_nCenterPort);
        }
    } while (0);

    ULSERVER_LOG_INFO("****LOCAL CONFIG **** begin...");
    ULSERVER_LOG_INFO("LOCAL_PARAM:ip %s",m_strLocalIP.c_str());
    ULSERVER_LOG_INFO("DataCenter:%s,%d,update time %d",m_strCenterIP.c_str(), m_nCenterPort, m_nUpdateDataTime);
    ULSERVER_LOG_INFO("****LOCAL CONFIG **** end..");
    return HPR_OK;
}

/**	@fn	    UpdateCollectInfo
*	@brief	更新采集平台信息
*	@param  [in] rspCollect -- 采集平台信息
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
// void CConfig::UpdateCollectInfo(const cms_8120::RspCollectconfig &rspCollect)
// {
//     m_strCollectIP = CSCT::Utf82Ansi(rspCollect.collectconfig().ip());
//     m_nCollectPort = rspCollect.collectconfig().port();
//     m_strStartTime = CSCT::Utf82Ansi(rspCollect.collectconfig().worktimebegin());
//     m_strStopTime = CSCT::Utf82Ansi(rspCollect.collectconfig().worktimeend());
// }

/**	@fn	    LoadWebConfigXML
*	@brief	加载WEB客户端生成的XML
*	@param  [in] 无
*	@param  [out] 无
*	@return	
*/
void CConfig::LoadWebConfigXML(const std::string &sConfigXML)
{
    if (sConfigXML.size()<=0)
    {
        ULSERVER_LOG_ERROR("Web config is empty.");
        return;
    }
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(sConfigXML.c_str());
    TiXmlHandle docHandle(&xmlDoc);
    //上传时间间隔
    TiXmlElement* pUploadNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_PARAM_UPLOAD).ToElement();
    if (pUploadNode)
    {
        TiXmlUtils::to_number(pUploadNode->Attribute("Frequency"), m_nUploadInteval);
        TiXmlUtils::to_number(pUploadNode->Attribute("TaskCount"), m_nTaskCount); 
        TiXmlUtils::to_number(pUploadNode->Attribute("Mode"), m_nMode);
		TiXmlUtils::to_number(pUploadNode->Attribute("Speed"), m_nUploadSpeed);
		TiXmlUtils::to_number(pUploadNode->Attribute("HaveCenterStorage"), m_nHaveCenterStorage);
    }
    //关联的系统巡检服务IP和端口
    TiXmlElement* pATServerNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_PARAM_ATSERVER).ToElement();
    if (pATServerNode)
    {
        m_strATServerIP = pATServerNode->Attribute("ip");
        TiXmlUtils::to_number(pATServerNode->Attribute("port"),m_nATServerPort);
    }
    //上传时间
    TiXmlElement* pCollectNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_PARAM_COLLECT).ToElement();
    if (pCollectNode)
    {
        m_strStartTime = pCollectNode->Attribute("startTime");
        m_strStopTime = pCollectNode->Attribute("stopTime");
    }
	ULSERVER_LOG_INFO("LoadWebConfigXML succeed.m_nUploadInteval:%d, m_nTaskCount:%d, m_nUploadSpeed:%d, m_strATServerIP:%s, m_nATServerPort:%d, m_strStartTime:%s, m_strStopTime:%s, m_nMode:%d, m_nHaveCenterStorage:%d",
        m_nUploadInteval, m_nTaskCount, m_nUploadSpeed, m_strATServerIP.c_str(), m_nATServerPort, m_strStartTime.c_str(), m_strStopTime.c_str(), m_nMode, m_nHaveCenterStorage);
}

/**	@fn	    InitTransInterval
*	@brief	初始化上传时间间隔
*	@param  [in] strConfig -- 配置文件名
*	@param  [out] 无
*	@return	void
*/
// void CConfig::InitTransInterval(const std::string strConfig)
// {
//     int TRANSINTEVAL_MAX = 600; // 10分钟
//     int TRANSINTEVAL_MIN = 2; // 2秒钟
// 
//     // 操作配置文件
//     CConfigParser myini(strConfig.c_str());
//     myini.SetSection(INI_SEC_LIBTRANSPORT);
// 
//     int transInterval = myini.GetInt32Value(INI_KEY_TRANSINTEVAL, DEFAULT_TRANSINTEVAL);
//     ULSERVER_LOG_INFO("transInterval:%d", transInterval);
//     if (transInterval < TRANSINTEVAL_MIN)
//     {
//         transInterval = TRANSINTEVAL_MIN;
//     }
//     else if (transInterval > TRANSINTEVAL_MAX)
//     {
//         transInterval = TRANSINTEVAL_MAX;
//     }
//     m_nUploadInteval = transInterval * 1000;
//     ULSERVER_LOG_INFO("transInterval:%d", m_nUploadInteval);
// }

/**	@fn	    InitReservedTime
*	@brief	初始化预留时间
*	@param  [in] strConfig -- 配置文件名
*	@param  [out] 无
*	@return	void
*/
// void CConfig::InitReservedTime(const std::string strConfig)
// {
//     int RESERVED_MAX = 5; // 10分钟
//     int RESERVED_MIN = 0; // 2秒钟
// 
//     // 操作配置文件
//     CConfigParser myini(strConfig.c_str());
//     myini.SetSection(INI_SEC_LIBTRANSPORT);
// 
//     int iPreTime = myini.GetInt32Value(INI_KEY_PRETIME, DEFAULT_PRETIME);
//     int iPostTime = myini.GetInt32Value(INI_KEY_POSTTIME, DEFAULT_POSTTIME);
//     ULSERVER_LOG_INFO("iPreTime:%d,iPostTime:%d", iPreTime,iPostTime);
//     if (iPreTime < RESERVED_MIN)
//     {
//         iPreTime = RESERVED_MIN;
//     }
//     else if (iPreTime > RESERVED_MAX)
//     {
//         iPreTime = RESERVED_MAX;
//     }
// 
//     if (iPostTime < RESERVED_MIN)
//     {
//         iPostTime = RESERVED_MIN;
//     }
//     else if (iPostTime > RESERVED_MAX)
//     {
//         iPostTime = RESERVED_MAX;
//     }
//     m_PreTime = iPreTime;
//     m_PostTime = iPostTime;
//     ULSERVER_LOG_INFO("m_PreTime:%d,m_PostTime:%d", m_PreTime,m_PostTime);
// }
