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

SINGLETON_IMPLEMENT(CServiceFactory)///<�����������

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
*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::Init()
{
    ULSERVER_LOG_INFO("CServiceFactory::Init in................");
    if (m_bInit)
    {
        //�Ѿ���ʼ����
        return HPR_OK;
    }

    // ��ʼ���⡣
    if (Hak1((char*)103, "DATA", NULL))
    {
        ULSERVER_LOG_ERROR("��ʼ���������ܿ�ʧ��");
        return HPR_ERROR;
    }

    //���������ļ�
    if (HPR_OK != CConfig::GetSingletonPtr()->LoadConfigFile())
    {
        ULSERVER_LOG_ERROR("LoadConfigFile Fail");
        return HPR_ERROR;
    }

    //��ʼ���Ự��
    CConnSessionMgr::GetSingletonPtr()->Init();

    //��ʼ�����ݿ⽻����
    CDBDataMgr::GetSingletonPtr()->Init(); 

    //��ʼ�����Ĵ洢ģ��
    //CStorageMgr::GetSingletonPtr()->Init();

    //��ʼ��¼���ϴ�ģ��
    CUploadFactory::GetSingletonPtr()->Init();

    m_bInit = true;
    ULSERVER_LOG_INFO("CServiceFactory::Init out................");
    return HPR_OK;
}

/**	@fn	    Fini
*	@brief	����ʼ��������������Դ�ͷ�
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::Fini()
{
    ULSERVER_LOG_INFO("CServiceFactory::Fini in................");
    if (!m_bInit)
    {
        //�Ѿ��ͷ���
        return HPR_OK;
    }

    //��ֹͣ����
    Stop();

    //�Ự��
    CConnSessionMgr::GetSingletonPtr()->Fini();

    //���ݿ⽻����
    CDBDataMgr::GetSingletonPtr()->Fini(); 

    //���Ĵ洢ģ��
    CStorageMgr::GetSingletonPtr()->Fini();

    //¼���ϴ�ģ��
    CUploadFactory::GetSingletonPtr()->Fini();

    m_bInit = false;
    ULSERVER_LOG_INFO("CServiceFactory::Fini out................");
    return HPR_OK;
}

/**	@fn	    Start
*	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
*          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::Start()
{
    ULSERVER_LOG_INFO("CServiceFactory::Start in................");
    if (!m_bInit || m_bStart)
    {
        //��û�г�ʼ��
        return HPR_ERROR;
    }

    //�Ự�㿪ʼ
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

    //¼���ϴ�ģ�鿪ʼ
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
*	@brief	ֹͣ����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::Stop()
{
    ULSERVER_LOG_INFO("CServiceFactory::Stop in................");
    if (!m_bInit || m_bStart)
    {
        //��û�г�ʼ��
        return HPR_ERROR;
    }

    //¼���ϴ�ģ��ֹͣ
    CUploadFactory::GetSingletonPtr()->Stop();
    //���ݿ⽻����ֹͣ
    CDBDataMgr::GetSingletonPtr()->Stop();
    //�Ự��ֹͣ
    CConnSessionMgr::GetSingletonPtr()->Stop();

    m_bStart = false;
    ULSERVER_LOG_INFO("CServiceFactory::Stop Out................");

    return HPR_OK;
}

/**	@fn	    GetVersion
*	@brief	��ȡ�汾��
*	@param  [in] ��
*	@param  [out] ��
*	@return	DWORD�����ذ汾����0x01020304
*/
int CServiceFactory::GetVersion()
{
    return 0x01000001;
}

/**	@fn	    GetLastError
*	@brief	���ش�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::GetLastError()
{
    return HPR_OK;
}
