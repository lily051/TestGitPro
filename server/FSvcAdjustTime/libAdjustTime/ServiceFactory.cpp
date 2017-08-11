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
*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::Init()
{
    TPLOG_INFO("CServiceFactory::Init in................");

    if (m_bInit)
    {
        //�Ѿ���ʼ����
        return HPR_OK;
    }
	// ��ʼ���⡣
	if (Hak1((char*)103, "DATA", NULL))
	{
		TPLOG_ERROR("��ʼ���������ܿ�ʧ��");
		return ADJUST_ERROR_FAIL;
	}
    
	if (HPR_OK != HPR_Init())
	{
		TPLOG_ERROR("init hpr failed");
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_FAIL;
	}

	//���������ļ�
	if (ADJUST_ERROR_SUCC != GetConfigPtr()->LoadConfigFile())
	{
		TPLOG_ERROR("LoadConfigFile Fail");
		GetConfigPtr()->free();
		HPR_Fini();
		Hak2(NULL, NULL, NULL);
		return ADJUST_ERROR_FAIL;
	}

	//��ʼ���Ự��
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

	//��ʼ�����ݿ⽻���� 
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

	//��ʼ����������ģ��
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

	//��ʼ��ϵͳѲ�����ģ��
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
*	@brief	����ʼ��������������Դ�ͷ�
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::Fini()
{
    TPLOG_INFO("CServiceFactory::Fini in................");
    if (!m_bInit)
    {
        //�Ѿ��ͷ���
        return ADJUST_ERROR_SUCC;
    }

    //��ֹͣ����
    Stop();

	//�������ܿ�
	Hak2(NULL, NULL, NULL);

	HPR_Fini();

	GetConfigPtr()->free();
    //�Ự��
	GetClientSessionMgrPtr()->Fini();
	GetClientSessionMgrPtr()->free();

    //���ݿ⽻����
	GetDBDataMgrPtr()->Fini();
	GetDBDataMgrPtr()->free();

	//��������
	GetCAlarmProcPtr()->Fini();
	GetCAlarmProcPtr()->free();

    //Уʱģ��
	GetAdjustPtr()->Fini();
	GetAdjustPtr()->free();

    m_bInit = false;
    TPLOG_INFO("CServiceFactory::Fini out................");
    return ADJUST_ERROR_SUCC;
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
    if (!m_bInit || m_bStart)
    {
        //��û�г�ʼ��
        return ADJUST_ERROR_FAIL;
    }

    //�Ự�㿪ʼ
    if (HPR_OK != GetClientSessionMgrPtr()->Start())
	{
		TPLOG_ERROR("ClientSessionMgr Start Fail!!!!");
		return ADJUST_ERROR_FAIL;
	}
    //���ݿ⽻����
    if (ADJUST_ERROR_SUCC != GetDBDataMgrPtr()->Start())
    {
        TPLOG_ERROR("CDBDataMgr Start Fail!!!!");
        return ADJUST_ERROR_FAIL;
    } 

    //ϵͳѲ�����ģ�鿪ʼ
    if (ADJUST_ERROR_SUCC != GetAdjustPtr()->Start())
	{
		TPLOG_ERROR("AdjustTime Start Fail!!!!");
		return ADJUST_ERROR_FAIL;
	}

	TPLOG_INFO(" ϵͳѲ���������");
	Msger_NotifyMsg(ALL_USER,_T("ϵͳѲ���������"));
    m_bStart = true;

    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Stop
*	@brief	ֹͣ����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CServiceFactory::Stop()
{
    if (!m_bInit || !m_bStart)
    {
        //��û�г�ʼ��
        return ADJUST_ERROR_FAIL;
    }

    //ϵͳѲ�����ģ��ֹͣ
    GetAdjustPtr()->Stop();
    //���ݿ⽻����ֹͣ
    GetDBDataMgrPtr()->Stop();
    //�Ự��ֹͣ
    GetClientSessionMgrPtr()->Stop();

    m_bStart = false;
	TPLOG_INFO("ϵͳѲ�����ֹͣ");
	Msger_NotifyMsg(ALL_USER,_T("ϵͳѲ�����ֹͣ"));

    return ADJUST_ERROR_SUCC;
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
    return ADJUST_ERROR_SUCC;
}
