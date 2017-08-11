#include "StdAfx.h"
#include "ServiceFactory.h"
#include "Config.h"
#include "ConnSessionInfo.h"
#include "AlarmProcessor.h"
#include "DBDataMgr.h"
#include "ClientSessionMgr.h"
#include <HPR_Config.h>
#include "hak.h"
#include "AdjustTime.h"
#include "Msger.h"

CServiceFactory::CServiceFactory(void)
{
    m_bInit = false;
    m_bStart = false;
}

CServiceFactory::~CServiceFactory(void)
{
    try
    {
        CServiceFactory::Fini();
		GetConfigPtr()->free();
		GetAdjustPtr()->free();
		GetCAlarmProcPtr()->free();
		GetDBDataMgrPtr()->free();
		GetClientSessionMgrPtr()->free();	
    }
    catch (...)
    {
    }
}

/**	@fn	    Init
*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CServiceFactory::Init()
{
    TPLOG_INFO("CServiceFactory::Init in................");

    if (m_bInit)
    {
        //已经初始化过
        return HPR_OK;
    }
	// 初始化库。
	if (Hak1((char*)103, "DATA", NULL))
	{
		TPLOG_ERROR("初始化海康加密库失败");
		return ADJUST_ERROR_FAIL;
	}
    
	if (HPR_OK != HPR_Init())
	{
		TPLOG_ERROR("init hpr failed");
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_FAIL;
	}

	//加载配置文件
	if (ADJUST_ERROR_SUCC != GetConfigPtr()->LoadConfigFile())
	{
		TPLOG_ERROR("LoadConfigFile Fail");
		GetConfigPtr()->free();
		HPR_Fini();
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_FAIL;
	}

	//初始化会话层
	if (HPR_OK != GetClientSessionMgrPtr()->Init())
	{
		TPLOG_ERROR("init ClientSessionMgr failed");
		GetClientSessionMgrPtr()->Fini();
		GetClientSessionMgrPtr()->free();
		GetConfigPtr()->free();
		HPR_Fini();
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_HPP;
	}

	//初始化数据库交互层 
	if (ADJUST_ERROR_SUCC != GetDBDataMgrPtr()->Init())
	{
		TPLOG_ERROR("init DBDataMgr failed");
		GetDBDataMgrPtr()->Fini();
		GetDBDataMgrPtr()->free();
		GetClientSessionMgrPtr()->Fini();
		GetClientSessionMgrPtr()->free();
		GetConfigPtr()->free();
		HPR_Fini();
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_FAIL;
	}

	//初始化报警处理模块
	if (ADJUST_ERROR_SUCC != GetCAlarmProcPtr()->Init())
	{
		TPLOG_ERROR("init Alarm Proc failed");
		GetCAlarmProcPtr()->Fini();
		GetCAlarmProcPtr()->free();
		GetDBDataMgrPtr()->Fini();
		GetDBDataMgrPtr()->free();
		GetClientSessionMgrPtr()->Fini();
		GetClientSessionMgrPtr()->free();
		GetConfigPtr()->free();
		HPR_Fini();
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_FAIL;
	}

	//初始化系统巡检服务模块
	if (ADJUST_ERROR_SUCC != GetAdjustPtr()->Init())
	{
		TPLOG_ERROR("init Adjust failed");
		GetAdjustPtr()->Fini();
		GetAdjustPtr()->free();
		GetCAlarmProcPtr()->Fini();
		GetCAlarmProcPtr()->free();
		GetDBDataMgrPtr()->Fini();
		GetDBDataMgrPtr()->free();
		GetClientSessionMgrPtr()->Fini();
		GetClientSessionMgrPtr()->free();
		GetConfigPtr()->free();
		HPR_Fini();
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_FAIL;
	}

	m_bInit = true;
	TPLOG_INFO("CServiceFactory::Init out................");

    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Fini
*	@brief	反初始化函数，负责资源释放
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CServiceFactory::Fini()
{
    TPLOG_INFO("CServiceFactory::Fini in................");
    if (!m_bInit)
    {
        //已经释放了
        return ADJUST_ERROR_SUCC;
    }

    //先停止服务
    Stop();

	//海康加密库
	Hak2(NULL, NULL, NULL);

	HPR_Fini();

	GetConfigPtr()->free();
    //会话层
	GetClientSessionMgrPtr()->Fini();
	GetClientSessionMgrPtr()->free();

    //数据库交互层
	GetDBDataMgrPtr()->Fini();
	GetDBDataMgrPtr()->free();

	//报警处理
	GetCAlarmProcPtr()->Fini();
	GetCAlarmProcPtr()->free();

    //校时模块
	GetAdjustPtr()->Fini();
	GetAdjustPtr()->free();

    m_bInit = false;
    TPLOG_INFO("CServiceFactory::Fini out................");
    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Start
*	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
*          此函数应该尽快返回，不应该占用太多时间，必须小于30s
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CServiceFactory::Start()
{
    if (!m_bInit || m_bStart)
    {
        //还没有初始化
        return ADJUST_ERROR_FAIL;
    }

    //会话层开始
    if (HPR_OK != GetClientSessionMgrPtr()->Start())
	{
		TPLOG_ERROR("ClientSessionMgr Start Fail!!!!");
		return ADJUST_ERROR_FAIL;
	}
    //数据库交互层
    if (ADJUST_ERROR_SUCC != GetDBDataMgrPtr()->Start())
    {
        TPLOG_ERROR("CDBDataMgr Start Fail!!!!");
        return ADJUST_ERROR_FAIL;
    } 

    //系统巡检服务模块开始
    if (ADJUST_ERROR_SUCC != GetAdjustPtr()->Start())
	{
		TPLOG_ERROR("AdjustTime Start Fail!!!!");
		return ADJUST_ERROR_FAIL;
	}

	TPLOG_INFO(" 系统巡检服务启动");
	Msger_NotifyMsg(ALL_USER,_T("系统巡检服务启动"));
    m_bStart = true;

    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Stop
*	@brief	停止函数
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CServiceFactory::Stop()
{
    if (!m_bInit || !m_bStart)
    {
        //还没有初始化
        return ADJUST_ERROR_FAIL;
    }

    //系统巡检服务模块停止
    GetAdjustPtr()->Stop();
    //数据库交互层停止
    GetDBDataMgrPtr()->Stop();
    //会话层停止
    GetClientSessionMgrPtr()->Stop();

    m_bStart = false;
	TPLOG_INFO("系统巡检服务停止");
	Msger_NotifyMsg(ALL_USER,_T("系统巡检服务停止"));

    return ADJUST_ERROR_SUCC;
}

/**	@fn	    GetVersion
*	@brief	获取版本号
*	@param  [in] 无
*	@param  [out] 无
*	@return	DWORD，返回版本，如0x01020304
*/
int CServiceFactory::GetVersion()
{
    return 0x01000001;
}

/**	@fn	    GetLastError
*	@brief	返回错误码
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CServiceFactory::GetLastError()
{
    return ADJUST_ERROR_SUCC;
}
