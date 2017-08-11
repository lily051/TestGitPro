#include "StdAfx.h"
#include "ServiceFactory.h"
#include "Config.h"
#include "ConnSessionInfo.h"
#include "ServiceFactory.h"
#include "UploadFactory.h"
#include "Storage/StorageMgr.h"
#include "DBDataMgr.h"
#include "ConnSessionMgr.h"
#include <HPR_Config.h>
#include "hak.h"

SINGLETON_IMPLEMENT(CServiceFactory)///<单例相关声明

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
        CUploadFactory::FreeSingleton();
        CStorageMgr::FreeSingleton();
        CConfig::FreeSingleton();
        CConnSessionMgr::FreeSingleton();
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
    ULSERVER_LOG_INFO("CServiceFactory::Init in................");
    if (m_bInit)
    {
        //已经初始化过
        return HPR_OK;
    }

    // 初始化库。
    if (Hak1((char*)103, "DATA", NULL))
    {
        ULSERVER_LOG_ERROR("初始化海康加密库失败");
        return HPR_ERROR;
    }

    //加载配置文件
    if (HPR_OK != CConfig::GetSingletonPtr()->LoadConfigFile())
    {
        ULSERVER_LOG_ERROR("LoadConfigFile Fail");
        return HPR_ERROR;
    }

    //初始化会话层
    CConnSessionMgr::GetSingletonPtr()->Init();

    //初始化数据库交互层
    CDBDataMgr::GetSingletonPtr()->Init(); 

    //初始化中心存储模块
    //CStorageMgr::GetSingletonPtr()->Init();

    //初始化录像上传模块
    CUploadFactory::GetSingletonPtr()->Init();

    m_bInit = true;
    ULSERVER_LOG_INFO("CServiceFactory::Init out................");
    return HPR_OK;
}

/**	@fn	    Fini
*	@brief	反初始化函数，负责资源释放
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CServiceFactory::Fini()
{
    ULSERVER_LOG_INFO("CServiceFactory::Fini in................");
    if (!m_bInit)
    {
        //已经释放了
        return HPR_OK;
    }

    //先停止服务
    Stop();

    //会话层
    CConnSessionMgr::GetSingletonPtr()->Fini();

    //数据库交互层
    CDBDataMgr::GetSingletonPtr()->Fini(); 

    //中心存储模块
    CStorageMgr::GetSingletonPtr()->Fini();

    //录像上传模块
    CUploadFactory::GetSingletonPtr()->Fini();

    m_bInit = false;
    ULSERVER_LOG_INFO("CServiceFactory::Fini out................");
    return HPR_OK;
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
    ULSERVER_LOG_INFO("CServiceFactory::Start in................");
    if (!m_bInit || m_bStart)
    {
        //还没有初始化
        return HPR_ERROR;
    }

    //会话层开始
    CConnSessionMgr::GetSingletonPtr()->Start();

    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->Start())
    {
        ULSERVER_LOG_ERROR("CDBDataMgr Start Fail!!!!");
        return HPR_ERROR;
    } 
    else
    {
        ULSERVER_LOG_INFO("CDBDataMgr Start Suc!!!!!!");
    }

    //录像上传模块开始
    CUploadFactory::GetSingletonPtr()->Start();

    Hak2(NULL, NULL, NULL);
    ULSERVER_LOG_INFO("ULServer Start Suc!!!!!");
    m_bStart = true;

// #ifndef _DEBUG
//     std::stringstream ssVersion,ssDesc;
//     ssVersion <<"AHService.dll: "<<CConfigFile::GetSingletonPtr()->m_sFileVersion<<".";
//     _gShowMsgStr(ssVersion.str());
// 
//     ssDesc<<CConfigFile::GetSingletonPtr()->m_sFileDescription<<".";
//     _gShowMsgStr(ssDesc.str());
// #endif

    return HPR_OK;
}

/**	@fn	    Stop
*	@brief	停止函数
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CServiceFactory::Stop()
{
    ULSERVER_LOG_INFO("CServiceFactory::Stop in................");
    if (!m_bInit || m_bStart)
    {
        //还没有初始化
        return HPR_ERROR;
    }

    //录像上传模块停止
    CUploadFactory::GetSingletonPtr()->Stop();
    //数据库交互层停止
    CDBDataMgr::GetSingletonPtr()->Stop();
    //会话层停止
    CConnSessionMgr::GetSingletonPtr()->Stop();

    m_bStart = false;
    ULSERVER_LOG_INFO("CServiceFactory::Stop Out................");

    return HPR_OK;
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
    return HPR_OK;
}
