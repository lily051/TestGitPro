#include "StdAfx.h"
#include <fclib/fcTchar.h>
#include "Config.h"
#include <HPR_FileSystem.h>
#include <fclib/fcConfigParser.h>
#include "hlogConfig.h"
#include <HPR_Error.h>
#include <./SvcGeneralDef.h>
#include "../Tinyxml/tinyxml.h"
#include "../Tinyxml/tinyxmlutils.h"
#include "../util/StringConverter.h"
#include <ATLComTime.h>
#include "Msger.h"

namespace {
    TCHAR const INI_CFG_FILE_NAME[]  = _T("config.ini");
    TCHAR const INI_SEC_ADJUSTTIME[] = _T("libAdjustTime");
	TCHAR const INI_KEY_LOCALCONFIGENABLE[] = _T("LocalConfigEnable");
    TCHAR const INI_KEY_ADJMODE[]    = _T("adjustmode");
    TCHAR const DEFAULT_ADJMODE[]    = _T("0");
    TCHAR const INI_KEY_TIMINGHOUR[] = _T("TimingHour");
    TCHAR const DEFAULT_TIMINGHOUR[] = _T("11");
    TCHAR const INI_KEY_TIMINGMIN[]  = _T("TimingMin");
    TCHAR const DEFAULT_TIMINMIN[]   = _T("5");
    TCHAR const INI_KEY_TIMINGSEC[]  = _T("TimingSec");
    TCHAR const DEFAULT_TIMINGSEC[]  = _T("0");
    TCHAR const INI_KEY_INTERVAL[]   = _T("Interval");
    TCHAR const DEFAULT_INTERVAL[]   = _T("6");
    TCHAR const INI_KEY_TIMELAG[]   = _T("maxtimelag");
    TCHAR const DEFAULT_TIMELAG[]   = _T("15");
	TCHAR const INI_KEY_TTS_INTERVAL[]   = _T("TTSInterval");
	TCHAR const DEFAULT_TTS_INTERVAL[]   = _T("10");
} //~ end of anonymous namespace


CConfig::CConfig(void)
: m_strCenterIP("127.0.0.1")
, m_nCenterPort(3306)
, m_strATServerIP("127.0.0.1")
, m_nATServerPort(6680)
, m_iAdjustMode(INVALID_POLICY_MODE)
, m_uiTimingHour(0)
, m_uiTimingMin(0)
, m_uiTimingSec(0)
, m_uiMaxTimeLag(0)
, m_uiInterval(0)
, m_bLocalConfig(false)
, m_nHaveCenterStorage(1)
{

}

CConfig::~CConfig(void)
{
	try
	{
	}
	catch (...)
	{
	}
}

/**	@fn	    LoadConfigFile
*	@brief	加载配置文件
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CConfig::LoadConfigFile(void)
{
	LoadLocalCfg();
	std::string strAppRunPath, strConfigFile;
	GetAppRunPath(strAppRunPath);
	strConfigFile = strAppRunPath + TOP_DIRECTORY_CONFIG_FILE;
	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(strConfigFile))
	{
		LOG_ERROR("xmlDoc.LoadFile top xml fail");
		return ADJUST_ERROR_FAIL;
	}
	TiXmlHandle  docHandle(&xmlDoc);

	//本地IP
	do 
	{
		TiXmlElement * pLocalParamNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_LOCAL_PARAM).ToElement();
		if (pLocalParamNode)
		{
			m_strLocalIP = pLocalParamNode->Attribute("ip");
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

	m_strATServerIP = m_strLocalIP;

	if (m_strATServerIP.empty())
	{
		TPLOG_ERROR("empty local ip!!!");
		Msger_NotifyMsg(ALL_USER,_T("本机IP不能为空"));
		return ADJUST_ERROR_FAIL;
	}

	TPLOG_INFO("local ip:%s, datacenter ip:%s, port:%d",m_strLocalIP.c_str(), m_strCenterIP.c_str(), m_nCenterPort);
	return ADJUST_ERROR_SUCC;
}

/**	@fn	    LoadLocalCfgInfo
*	@brief	加载本地配置文件
*	@param  [in] 无
*	@param  [out] 无
*	@return	void
*/
void CConfig::LoadLocalCfg(void)
{
	// 操作配置文件
	std::string strAppRunPath, strConfigFile;
	GetAppRunPath(strAppRunPath);
	strConfigFile = strAppRunPath + fcT2A(INI_CFG_FILE_NAME);
	CConfigParser myini(fcA2T(strConfigFile.c_str()));
	myini.SetSection(INI_SEC_ADJUSTTIME);

	m_bLocalConfig = myini.GetBooleanValue(INI_KEY_LOCALCONFIGENABLE, false);

	//启用本地配置，不从WEB读取配置信息
	if (m_bLocalConfig)
	{
		//校时策略
		std::_tstring strAjustMode = myini.GetStringValue(INI_KEY_ADJMODE, DEFAULT_ADJMODE);
		m_iAdjustMode = atoi(fcT2A(strAjustMode.c_str()));

		//校时参数
		//每日定时校时时间
		//时
		std::_tstring strTiming = myini.GetStringValue(INI_KEY_TIMINGHOUR, DEFAULT_TIMINGHOUR);
		m_uiTimingHour = atoi(fcT2A(strTiming.c_str()));
		//分
		strTiming = myini.GetStringValue(INI_KEY_TIMINGMIN, DEFAULT_TIMINMIN);
		m_uiTimingMin = atoi(fcT2A(strTiming.c_str()));
		//秒
		strTiming  = myini.GetStringValue(INI_KEY_TIMINGSEC, DEFAULT_TIMINGSEC);
		m_uiTimingSec = atoi(fcT2A(strTiming.c_str()));

		//校时时间间隔
		std::_tstring strInterval = myini.GetStringValue(INI_KEY_INTERVAL, DEFAULT_INTERVAL);
		m_uiInterval = atoi(fcT2A(strInterval.c_str()));

		//dvr和本地允许自动校时的最大时间差 单位分钟
		std::_tstring strMaxTimeLag = myini.GetStringValue(INI_KEY_TIMELAG, DEFAULT_TIMELAG);
		m_uiMaxTimeLag = atoi(fcT2A(strMaxTimeLag.c_str()));

		std::_tstring strTTSInterval = myini.GetStringValue(INI_KEY_TTS_INTERVAL, DEFAULT_TTS_INTERVAL);
	}
	TPLOG_INFO("Load config m_bLocalConfig:%d, m_iAdjustMode:%d, m_uiTimingHour:%d, m_uiTimingMin:%d, m_uiTimingSec:%d, m_uiInterval:%d,m_uiMaxTimeLag:%d",
		m_bLocalConfig, m_iAdjustMode, m_uiTimingHour, m_uiTimingMin, m_uiTimingSec, m_uiInterval, m_uiMaxTimeLag);
}

/**	@fn	    LoadWebConfig
    *	@brief	加载WEB端配置
    *	@param  [in] serverLoginRsp 登陆返回配置
    *	@param  [out] 无
    *	@return	bool
    */
bool CConfig::LoadWebConfig(cms_8120::RspServerLogin &serverLoginRsp)
{
	if (serverLoginRsp.config_info().size()<=0)
	{
		LOG_ERROR("Web config is empty.");
		return false;
	}
    m_nATServerPort = serverLoginRsp.ctrl_port();
	TiXmlDocument xmlDoc;
	xmlDoc.Parse(serverLoginRsp.config_info().c_str());
	TiXmlHandle docHandle(&xmlDoc);
	//校时参数
	TiXmlElement* pTimeNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_PARAM_TIMING).ToElement();
	if (pTimeNode)
	{
		TiXmlUtils::to_number(pTimeNode->Attribute("Model"),     m_iAdjustMode);
        if (ADJUST_TIMING_MODE == m_iAdjustMode)
        {
			std::string strAdjustTime;
			strAdjustTime = pTimeNode->Attribute("Time");
			std::_tstring strTemp = fcA2T(strAdjustTime.c_str());
			COleDateTime adjustTime;
			if (!adjustTime.ParseDateTime(strTemp.c_str()))
			{
				TPLOG_ERROR("parse adjust time failed: %s", strAdjustTime.c_str());
				return FALSE;
			}
			m_uiTimingHour = adjustTime.GetHour();
			m_uiTimingMin = adjustTime.GetMinute();
			m_uiTimingSec = adjustTime.GetSecond();
        }
		else if (ADJUST_INTERVAL_MODE == m_iAdjustMode)
		{
			TiXmlUtils::to_number(pTimeNode->Attribute("Frequency"), m_uiInterval);
		} 
		else
		{
		}
		TiXmlUtils::to_number(pTimeNode->Attribute("Condition"), m_uiMaxTimeLag);
		TiXmlUtils::to_number(pTimeNode->Attribute("HaveCenterStorage"), m_nHaveCenterStorage);
	}
	TPLOG_INFO("Load WebConfig:m_nATServerPort:%d, m_iAdjustMode:%d, m_uiTimingHour:%d, m_uiTimingMin:%d, m_uiTimingSec:%d, m_uiInterval:%d,m_uiMaxTimeLag:%d,m_nHaveCenterStorage:%d",
		       m_nATServerPort, m_iAdjustMode, m_uiTimingHour, m_uiTimingMin, m_uiTimingSec, m_uiInterval, m_uiMaxTimeLag, m_nHaveCenterStorage);
	return true;
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

/** @fn    GetConfigInfo
*  @brief  获取策略配置信息
*  @param  [out]stConfigInfo, 配置信息
*  @return bool
*/
bool CConfig::GetPolicyInfo(config_info_t &stConfigInfo)
{
    stConfigInfo.enAdjustMode = static_cast<adj_mode_en>(m_iAdjustMode);
    stConfigInfo.uiTimingHour = m_uiTimingHour;
    stConfigInfo.uiTimingMin  = m_uiTimingMin;
    stConfigInfo.uiTimingSec  = m_uiTimingSec;
    stConfigInfo.uiInterval   = m_uiInterval;
    stConfigInfo.uiMaxTimeLag = m_uiMaxTimeLag;
    return true;
}
