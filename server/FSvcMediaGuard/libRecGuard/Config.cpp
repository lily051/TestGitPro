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

// namespace
// {
//     TCHAR const INI_SEC_UPLOAD[] = _T("UPLOADCONFIG");
//     TCHAR const INI_KEY_LOCALCONFIGENABLE[] = _T("LocalConfigEnable");
//     TCHAR const INI_KEY_DELRECORD[] = _T("DelRecord");
//     TCHAR const INI_KEY_DELFILE[] = _T("DelFile");
//     TCHAR const INI_KEY_UPLOADINTEVAL[] = _T("UploadInteval");
//     INT32 const DEFAULT_UPLOADINTEVAL = 2;
// //     TCHAR const INI_KEY_CENTERHOST[] = _T("centerip");
// //     TCHAR const DEFAULT_CENTERHOST[] = _T("127.0.0.1");
// //     TCHAR const INI_KEY_CENTERPORT[] = _T("centerPort");
// //     INT32 const DEFAULT_CENTERPORT   = 8849;
// //     TCHAR const INI_KEY_COLLECTHOST[] = _T("CollectIP");
// //     TCHAR const DEFAULT_COLLECTHOST[] = _T("127.0.0.1");
// //     TCHAR const INI_KEY_COLLECTPORT[] = _T("CollectPort");
// //     INT32 const DEFAULT_COLLECTPORT   = 8849;
//     TCHAR const INI_KEY_ATSERVERHOST[] = _T("ATServerIP");
//     TCHAR const DEFAULT_ATSERVERHOST[] = _T("127.0.0.1");
//     TCHAR const INI_KEY_ATSERVERPORT[] = _T("ATServerPort");
//     INT32 const DEFAULT_ATSERVERPORT   = 6677;
// }

SINGLETON_IMPLEMENT(CConfig)//<单例相关实现

CConfig::CConfig(void)
{
    m_strCenterIP       = "127.0.0.1";
    m_nCenterPort       = 8849;
    m_strATServerIP     = "127.0.0.1";
    m_nATServerPort     = 6677;
//     m_strCollectIP      = "127.0.0.1";
//     m_nCollectPort      = 8849;
//     m_isAutoDelRecord   = false;
//     m_isAutoDelFile     = false;
//     m_bLocalConfig      = false;
    m_nTaskInteval      = 0;
//    m_nUpdateDataTime   = 30;
    m_strStartTime      = "00:00:00";
    m_strStopTime       = "00:00:00";
//    m_nTaskCount        = 0;
	m_nHaveCenterStorage = 1;
}

CConfig::~CConfig(void)
{
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
    TPLOG_INFO("ATServer:%s,%d",m_strATServerIP.c_str(),m_nATServerPort);
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
    //LoadLocalCfgInfo();
    std::string sAppRunPath, sConfigFile;
    GetAppRunPath(sAppRunPath);
    sConfigFile = sAppRunPath + TOP_DIRECTORY_CONFIG_FILE;
    TiXmlDocument xmlDoc;
    if (!xmlDoc.LoadFile(sConfigFile))
    {
        TPLOG_ERROR("xmlDoc.LoadFile top xml fail");
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

    TPLOG_INFO("****LOCAL CONFIG **** begin...");
    TPLOG_INFO("LOCAL_PARAM:ip %s",m_strLocalIP.c_str());
    TPLOG_INFO("DataCenter:%s,%d",m_strCenterIP.c_str(), m_nCenterPort);
    TPLOG_INFO("****LOCAL CONFIG **** end..");
    return HPR_OK;
}

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
        TPLOG_ERROR("Web config is empty.");
        return;
    }
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(sConfigXML.c_str());
    TiXmlHandle docHandle(&xmlDoc);
    //任务时间间隔
    TiXmlElement* pUploadNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_PARAM_RECGUARD).ToElement();
    if (pUploadNode)
    {
        TiXmlUtils::to_number(pUploadNode->Attribute("Frequency"), m_nTaskInteval);
		TiXmlUtils::to_number(pUploadNode->Attribute("HaveCenterStorage"), m_nHaveCenterStorage);
    }
    //关联的系统巡检服务IP和端口
    TiXmlElement* pATServerNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_PARAM_ATSERVER).ToElement();
    if (pATServerNode)
    {
        m_strATServerIP = pATServerNode->Attribute("ip");
        TiXmlUtils::to_number(pATServerNode->Attribute("port"),m_nATServerPort);
    }
	TPLOG_INFO("LoadWebConfigXML succeed.Frequency:%d, HaveCenterStorage:%d, m_strATServerIP:%s, m_nATServerPort:%d",
		m_nTaskInteval, m_nHaveCenterStorage, m_strATServerIP.c_str(), m_nATServerPort);
}

