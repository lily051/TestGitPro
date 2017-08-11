#include "StdAfx.h"
#include <hak.h>
#include <string>
#include <algorithm>
#include "../util/StringConverter.h"
#include <fclib/FCThreadHelper.h>
#include "AdjustTime.h"
#include "Config.h"
#include "hlogConfig.h"
#include <SvcGeneralDef.h>
#include "Msger.h"
#include "DvrPatrol.h"
#include "AlarmProcessor.h"
#include "DBDataMgr.h"
#include "type/DevType.pb.h"

using namespace fclib::fcthread;

namespace {
// �����̼߳�����������ƴ����̵߳Ŀ�ʼ��ֹͣ
DWORD const MANAGE_WORK_INTERVAL             = 5000;
//�����̼߳��
const DWORD HEARTBEAT_WORK_INTERVAL          = 30 *1000;
//dvrѲ����
const DWORD PATROL_WORK_INTERVAL             = 10 * 60 *1000;
//�洢�豸��������� 
DWORD const STORAGE_CAPACITY_DETECT_INTERVAL = 10 * 60 * 1000;
//һСʱ3600��
const int SECONDS_PER_HOUR                   = 3600;
//һ����60��
const unsigned int SECONDS_PER_MIN           = 60;
//ʱ��ת�������� struct tm�����Ϊ1900���������
const int TIME_COMPENSATE                    = 1900;
DWORD  const G_MB                            = 1024;    // 1 G
DWORD  const MIN_GROUP_SIZE                  = 100;    // 100 G
double const MIN_GROUP_PERCENT               = 5.0; // ��С���аٷֱ�

const unsigned int CLOUD_OFFLINE             = 23010;
} //~ end of anonymous namespace

DWORD WINAPI WT_HeartBeat(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        TPLOG_ERROR("WT_HeartBeat invalid point");
        return ADJUST_ERROR_FAIL;
    }

    CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
    return pInstance->HeartBeatProc();
}

DWORD WINAPI WT_Manage(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        TPLOG_ERROR("WT_Manage invalid point");
        return ADJUST_ERROR_FAIL;
    }

    CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
    return pInstance->ManageProc();
}

DWORD WINAPI WT_AdjustTime(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        TPLOG_ERROR("WT_AdjustTime invalid point");
        return ADJUST_ERROR_FAIL;
    }

    CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
    return pInstance->AdjustTimeProc();
}

DWORD WINAPI WT_AddAlarmProc(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        TPLOG_ERROR("WT_AddAlarmProc invalid point");
        return ADJUST_ERROR_FAIL;
    }

    CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
    return pInstance->AddAlarmProc();
}

DWORD WINAPI WT_DisAlarmProc(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        TPLOG_ERROR("WT_DisAlarmProc invalid point");
        return ADJUST_ERROR_FAIL;
    }

    CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
    return pInstance->DisAlarmProc();
}

DWORD WINAPI WT_CollectPatrolProc(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        TPLOG_ERROR("WT_CollectPatrolProc invalid point");
        return ADJUST_ERROR_FAIL;
    }

    CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
    return pInstance->CollectPatrolProc();
}

DWORD WINAPI WT_StoragePatrolProc(LPVOID lpParam)
{
	if (NULL == lpParam)
	{
		TPLOG_ERROR("WT_StoragePatrolProc invalid point");
		return ADJUST_ERROR_FAIL;
	}

	CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
	return pInstance->StoragePatrolProc();
}


DWORD WINAPI WT_ImmAjust(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        TPLOG_ERROR("WT_ImmAjust invalid param");
        Msger_NotifyMsg(ALL_USER,_T("WT_ImmAjust�߳� ���������Ч"));
        return ADJUST_ERROR_FAIL;
    }

    CAdjustTime *pInstance = reinterpret_cast<CAdjustTime*>(lpParam);
    return pInstance->ImmAjustProc();
}

CAdjustTime::CAdjustTime(void)
: m_hThreadManager(NULL)
, m_hThreadHeartBeat(NULL)
, m_hThreadAdjustTime(NULL)
, m_hThreadAddAlarm(NULL)
, m_hThreadDisAlarm(NULL)
, m_hThreadCollectPatrol(NULL)
, m_hThreadStoragepatrol(NULL)
, m_bStopWorker(false)
, m_bStopServer(false)
, m_hGetCollectDeviceListEvent(NULL)
, m_hGetStorageDeviceListEvent(NULL)
{
	InitializeCriticalSection(&m_csListCollectDevice);
	InitializeCriticalSection(&m_cslistStorageDevice);
	memset(&m_stAdjustConfig, 0, sizeof(config_info_t));
}

CAdjustTime::~CAdjustTime(void)
{
    try
    {
        m_bStopServer = true;
        m_bStopWorker = true;
        GetCAlarmProcPtr()->m_bExitFlag = true;
        if (NULL != m_hThreadHeartBeat)
        {
            fcWaitAndCloseThread(m_hThreadHeartBeat);
            m_hThreadHeartBeat = NULL;
        }
        if (NULL != m_hThreadAdjustTime)
        {
            fcWaitAndCloseThread(m_hThreadAdjustTime);
            m_hThreadAdjustTime = NULL;
        }
        if (NULL != m_hThreadAddAlarm)
        {
            fcWaitAndCloseThread(m_hThreadAddAlarm);
            m_hThreadAddAlarm = NULL;
        }
        if (NULL != m_hThreadDisAlarm)
        {
            fcWaitAndCloseThread(m_hThreadDisAlarm);
            m_hThreadDisAlarm = NULL;
        }
        if (NULL != m_hThreadCollectPatrol)
        {
            fcWaitAndCloseThread(m_hThreadCollectPatrol);
            m_hThreadCollectPatrol = NULL;
        }
        if (NULL != m_hThreadManager)
        {
            fcWaitAndCloseThread(m_hThreadManager);
            m_hThreadManager = NULL;
        }
		DeleteCriticalSection(&m_csListCollectDevice);
		DeleteCriticalSection(&m_cslistStorageDevice);

        NET_DVR_Cleanup();
		//����ʼ���ƴ洢
		HCS_SDK_Deinit();
		//����ʼ��curl��
		curl_global_cleanup();
    }
    catch (...)
    {

    }
}

/** @fn	    Init
*   @brief  ��ʼ������
*   @return ���� ADJUST_ERROR_SUCC
            �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::Init(void)
{
    if (TRUE != NET_DVR_Init())
    {
        TPLOG_ERROR("init NET DVR SDK failed");
        return ADJUST_ERROR_DVR;
    }

    //waittime 2�� trytime 1��
    NET_DVR_SetConnectTime(2000, 1);
    //�������10��
    NET_DVR_SetReconnect(10000, true);

	//�ƴ洢��ʼ��
	int nRes = HCS_SDK_Init();
	if (HCS_OK != nRes)
	{
		TPLOG_ERROR("call HCS_SDK_Init fail.error[%d]",nRes);
		return ADJUST_ERROR_FAIL; 
	}

	//��ʼ��curl��
	int nRet= curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != nRet)
	{
		TPLOG_ERROR("call curl_global_init fail.error[%d]",nRet);
		return ADJUST_ERROR_FAIL; 
	}

    return ADJUST_ERROR_SUCC;
}

/** @fn     Fini
*   @brief  ����ʼ������
*   @return ���� ADJUST_ERROR_SUCC
                �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::Fini(void)
{
    NET_DVR_Cleanup();
	//����ʼ���ƴ洢
	HCS_SDK_Deinit();
	//����ʼ��curl��
	curl_global_cleanup();

    return ADJUST_ERROR_SUCC;
}

/** @fn	    Start
*   @brief  ��ʼѲ��
*   @return ���� ADJUST_ERROR_SUCC
            �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::Start(void)
{
    m_bStopServer = false;
    m_bStopWorker = false;	
    GetCAlarmProcPtr()->m_bExitFlag = false;

	m_hGetCollectDeviceListEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hGetStorageDeviceListEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hGetCollectDeviceListEvent || NULL == m_hGetStorageDeviceListEvent)
	{
		TPLOG_ERROR("create Adjust Time event failed:%d", ::GetLastError());
		return ADJUST_ERROR_FAIL;
	}

    if (NULL == m_hThreadManager)
    {
        m_hThreadManager = fcFancyCreateThread(WT_Manage, this);
        if (NULL == m_hThreadManager)
        {
            TPLOG_ERROR("create Adjust Time thread failed:%d", ::GetLastError());
            return ADJUST_ERROR_FAIL;
        }
    }

    if (NULL == m_hThreadHeartBeat)
    {
        m_hThreadHeartBeat = fcFancyCreateThread(WT_HeartBeat, this);
        if (NULL == m_hThreadHeartBeat)
        {
            TPLOG_ERROR("create HeartBeat thread failed:%d", ::GetLastError());
			Msger_NotifyMsg(ALL_USER,_T("���������߳�ʧ��"));
            m_bStopServer = true;
            m_bStopWorker = true;
            GetCAlarmProcPtr()->m_bExitFlag = true;
            if (NULL != m_hThreadAdjustTime)
            {
                fcWaitAndCloseThread(m_hThreadAdjustTime);
                m_hThreadAdjustTime = NULL;
            }
            if (NULL != m_hThreadAddAlarm)
            {
                fcWaitAndCloseThread(m_hThreadAddAlarm);
                m_hThreadAddAlarm = NULL;
            }
            if (NULL != m_hThreadDisAlarm)
            {
                fcWaitAndCloseThread(m_hThreadDisAlarm);
                m_hThreadDisAlarm = NULL;
            }
            if (NULL != m_hThreadCollectPatrol)
            {
                fcWaitAndCloseThread(m_hThreadCollectPatrol);
                m_hThreadCollectPatrol = NULL;
            }
            if (NULL != m_hThreadStoragepatrol)
            {
                fcWaitAndCloseThread(m_hThreadStoragepatrol);
                m_hThreadStoragepatrol = NULL;
            }
            if (NULL != m_hThreadManager)
            {
                fcWaitAndCloseThread(m_hThreadManager);
                m_hThreadManager = NULL;
            }
            return ADJUST_ERROR_FAIL;
        }
    }

    return ADJUST_ERROR_SUCC;
}

/** @fn	    ImmAdjustOnce
*   @brief  ����ִ��һ��Уʱ����
*   @return ���� ADJUST_ERROR_SUCC
            �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::ImmAdjustOnce(void)
{
    //����Уʱһ���߳�
    HANDLE hThreadImmAdjust = fcFancyCreateThread(WT_ImmAjust, this);
    if (NULL == hThreadImmAdjust)
    {
        TPLOG_ERROR("create immediately adjust time thread failed:%d", ::GetLastError());
       Msger_NotifyMsg(ALL_USER,_T("��������Уʱһ���߳�ʧ��"));
        return ADJUST_ERROR_FAIL;
    }

    //�ͷž��
    ::CloseHandle(hThreadImmAdjust);

    return ADJUST_ERROR_SUCC;
}

/** @fn	     Stop
*   @brief   ֹͣѲ��
*   @return  ���� ADJUST_ERROR_SUCC
             �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::Stop(void)
{
    m_bStopWorker = true;
    m_bStopServer = true;
    GetCAlarmProcPtr()->m_bExitFlag = true;

    if (NULL != m_hThreadAdjustTime)
    {
        fcWaitAndCloseThread(m_hThreadAdjustTime);
        m_hThreadAdjustTime = NULL;
    }

    if (NULL != m_hThreadAddAlarm)
    {
        fcWaitAndCloseThread(m_hThreadAddAlarm);
        m_hThreadAddAlarm = NULL;
    }

    if (NULL != m_hThreadDisAlarm)
    {
        fcWaitAndCloseThread(m_hThreadDisAlarm);
        m_hThreadDisAlarm = NULL;
    }

    if (NULL != m_hThreadStoragepatrol)
    {
        fcWaitAndCloseThread(m_hThreadStoragepatrol);
        m_hThreadStoragepatrol = NULL;
    }

    if (NULL != m_hThreadCollectPatrol)
    {
        fcWaitAndCloseThread(m_hThreadCollectPatrol);
        m_hThreadCollectPatrol = NULL;
    }

    if (NULL != m_hThreadManager)
    {
        fcWaitAndCloseThread(m_hThreadManager);
        m_hThreadManager = NULL;
    }

	if (m_hGetCollectDeviceListEvent != NULL)
	{
		CloseHandle(m_hGetCollectDeviceListEvent);
		m_hGetCollectDeviceListEvent = NULL;
	}

	if (m_hGetStorageDeviceListEvent != NULL)
	{
		CloseHandle(m_hGetStorageDeviceListEvent);
		m_hGetStorageDeviceListEvent = NULL;
	}

    if (NULL != m_hThreadHeartBeat)
    {
        fcWaitAndCloseThread(m_hThreadHeartBeat);
        m_hThreadHeartBeat = NULL;
    }

    return ADJUST_ERROR_SUCC;
}

/** @fn     HeartBeatProc
 *  @brief  �����߳� ���뷢һ��
 *  @return ���� ADJUST_ERROR_SUCC
            �쳣 �� ADJUST_ERROR_SUCC
 */
DWORD CAdjustTime::HeartBeatProc(void)
{
    TPLOG_INFO("�����߳�����");
    Msger_NotifyMsg(ALL_USER,_T("�����߳�����"));
    do
    {
		if (GetDBDataMgrPtr()->UpdateServerHeartBeartReq())
		{
			TPLOG_ERROR("Update AdjustTime_Server:%s Online State failure",  GetConfigPtr()->m_strLocalIP.c_str());
		}
    } while (DelayBySleep(HEARTBEAT_WORK_INTERVAL, m_bStopServer), !m_bStopServer);

    TPLOG_INFO("�����߳���ֹ");
    Msger_NotifyMsg(ALL_USER,_T("�����߳���ֹ"));
    return ADJUST_ERROR_SUCC;
}

/** @fn    ManageProc
*  @brief  ��������߳�
*  @return ���� ADJUST_ERROR_SUCC
           �쳣 �� ADJUST_ERROR_SUCC
*/
DWORD CAdjustTime::ManageProc(void)
{
	TPLOG_INFO("�����߳�����");
    do
    {
        if (NULL == m_hThreadAdjustTime
            || NULL == m_hThreadAddAlarm
            || NULL == m_hThreadDisAlarm
            || NULL == m_hThreadCollectPatrol
			|| NULL == m_hThreadStoragepatrol)
        {
            StartWorker();
        }
    } while (DelayBySleep(MANAGE_WORK_INTERVAL, m_bStopServer), !m_bStopServer);

	TPLOG_INFO("�����߳���ֹ");
	Msger_NotifyMsg(ALL_USER,_T("�����߳���ֹ"));
    return 0;
}

//��ʼ����
bool CAdjustTime::StartWorker(void)
{
    m_bStopWorker = false;
    GetCAlarmProcPtr()->m_bExitFlag = false;

    //Уʱ
    if (NULL == m_hThreadAdjustTime)
    {
        m_hThreadAdjustTime = fcFancyCreateThread(WT_AdjustTime, this);
        if (NULL == m_hThreadAdjustTime)
        {
            TPLOG_ERROR("create AdjustTime thread failed:%d", ::GetLastError());
            return false;
        }
    }

    //�������
    if (NULL == m_hThreadAddAlarm)
    {
        m_hThreadAddAlarm = fcFancyCreateThread(WT_AddAlarmProc, this);
        if (NULL == m_hThreadAddAlarm)
        {
            TPLOG_ERROR("create WT_AddAlarmProc thread failed:%d", ::GetLastError());
            return false;
        }
    }

    //��������
    if (NULL == m_hThreadDisAlarm)
    {
        m_hThreadDisAlarm = fcFancyCreateThread(WT_DisAlarmProc, this);
        if (NULL == m_hThreadDisAlarm)
        {
            TPLOG_ERROR("create WT_DisAlarmProc thread failed:%d", ::GetLastError());
            return false;
        }
    }

    //�ɼ��豸Ѳ��
    if (NULL == m_hThreadCollectPatrol)
    {
        m_hThreadCollectPatrol = fcFancyCreateThread(WT_CollectPatrolProc, this);
        if (NULL == m_hThreadCollectPatrol)
        {
            TPLOG_ERROR("create m_hThreadCollectPatrol thread failed:%d", ::GetLastError());
            return false;
        }
    }
   //�洢�豸���
	if (NULL == m_hThreadStoragepatrol)
	{
		m_hThreadStoragepatrol = fcFancyCreateThread(WT_StoragePatrolProc, this);
		if (NULL == m_hThreadStoragepatrol)
		{
			TPLOG_ERROR("create m_hThreadStoragepatrol thread failed:%d", ::GetLastError());
			return false;
		}
	}

    return true;
}

/** @fn    AdjustTimeProc
*  @brief  �ɼ��豸Уʱ�߳�
*  @return ���� ADJUST_ERROR_SUCC
           �쳣 �� ADJUST_ERROR_SUCC
*/
DWORD CAdjustTime::AdjustTimeProc(void)
{
    TPLOG_INFO("�ɼ��豸Уʱ�߳�����");
    Msger_NotifyMsg(ALL_USER,_T("�ɼ��豸Уʱ�߳�����"));
	//��ȡУʱ������Ϣ
	memset(&m_stAdjustConfig, 0, sizeof(config_info_t));
	GetConfigPtr()->GetPolicyInfo(m_stAdjustConfig);
    do
	{
		if (ADJUST_EXEC_ONCE == m_stAdjustConfig.enAdjustMode)       //Уʱһ��
		{
			return (AdjustTime());
		}
		else if (ADJUST_TIMING_MODE == m_stAdjustConfig.enAdjustMode) //ÿ�ն�ʱУʱ
		{
			time_t iCurTime = time(NULL);
			struct tm stAdjustTime = {0};
			time_t iAdjustTimeStamp; //Уʱʱ���
			localtime_s(&stAdjustTime, &iCurTime);
			stAdjustTime.tm_hour = (int)(m_stAdjustConfig.uiTimingHour);
			stAdjustTime.tm_min  = (int)(m_stAdjustConfig.uiTimingMin);
			stAdjustTime.tm_sec  = (int)(m_stAdjustConfig.uiTimingSec);
			iAdjustTimeStamp = mktime(&stAdjustTime);
			if(iAdjustTimeStamp == iCurTime)
			{
				AdjustTime();
				Sleep(5000);
			}
			Sleep(50);
		}
		else if (ADJUST_INTERVAL_MODE == m_stAdjustConfig.enAdjustMode) //ÿ��������Уʱ
		{
			AdjustTime();
			//sleep�û����õ�ʱ���� ��λСʱ
			for(unsigned int i = 0; i < m_stAdjustConfig.uiInterval; i++)
			{
				//sleep1Сʱ
				for(int j = 0; j < SECONDS_PER_HOUR; j++)
				{
					Sleep(1000);
					if(m_bStopWorker)
					{
						TPLOG_INFO("Уʱ�߳���ֹ");
						Msger_NotifyMsg(ALL_USER,_T("Уʱ�߳���ֹ"));
						return ADJUST_ERROR_SUCC;
					}
				}
			}
		} 
		else
		{
			TPLOG_ERROR("adjust time policy mode is invalid(%d)",
				m_stAdjustConfig.enAdjustMode);
			Msger_AlarmMsg(WORK_TYPE_ADJUST_SVC_FAIL,
				ALL_USER,
				ALL_CHANNEL,
				ALARM_OCCUR,
				TP_ALARM_ADJUST,
				_T("Уʱ������Ч(%d)"),
				m_stAdjustConfig.enAdjustMode);
			Sleep(1000);
		}
    } while (!m_bStopWorker);

    TPLOG_INFO("�ɼ��豸Уʱ�߳���ֹ");
    Msger_NotifyMsg(ALL_USER,_T("�ɼ��豸Уʱ�߳���ֹ"));
    return ADJUST_ERROR_SUCC;
}

/** @fn     AddAlarmProc
*  @brief  ��������߳�
*  @return ���� ADJUST_ERROR_SUCC
           �쳣 �� ADJUST_ERROR_SUCC
*/
DWORD CAdjustTime::AddAlarmProc(void)
{
    GetCAlarmProcPtr()->SaveAlarmThreadProc();
    return ADJUST_ERROR_SUCC;
}

/** @fn     DisAlarmProc
*  @brief  ���������߳�
*  @return ���� ADJUST_ERROR_SUCC
    �쳣 �� ADJUST_ERROR_SUCC
*/
DWORD CAdjustTime::DisAlarmProc(void)
{
    GetCAlarmProcPtr()->AlarmDisposeProc();
    return ADJUST_ERROR_SUCC;
}


/** @fn    CollectPatrolProc
*  @brief  �ɼ��豸Ѳ���߳�
*  @return ���� ADJUST_ERROR_SUCC
    �쳣 �� ADJUST_ERROR_SUCC
*/
DWORD CAdjustTime::CollectPatrolProc(void)
{
    TPLOG_INFO("�ɼ��豸Ѳ���߳�����");
    Msger_NotifyMsg(ALL_USER,_T("�ɼ��豸Ѳ���߳�����"));
    std::list<collect_info_t> lstDvrBackupInfo;
    do
    {
		std::list<collect_info_t> listCollectDevice;    //�ɼ��豸��Ϣ����
        std::list<std::string> lstDvrIp;                //��Ż�ȡ������DVR�б�
        std::list<collect_info_t> lstDvrSucLst;         //��Ų����ɹ���DVR�б�
		//��ȡ�ɼ��豸��Ϣ
		if (!GetAllCollectDeviceReq(listCollectDevice))
		{
			continue;
		}
		if (!listCollectDevice.empty())
		{
			collect_info_t stCurDvr;
			std::list<collect_info_t>::iterator iterDvrInfo;
			for (iterDvrInfo = listCollectDevice.begin(); iterDvrInfo != listCollectDevice.end() && !m_bStopWorker; ++iterDvrInfo)
			{
				stCurDvr = *iterDvrInfo;
				TPLOG_INFO("CollectPatrol dvr info(ip:%s, port:%d, user:%s)", stCurDvr.strDvrIp.c_str(), stCurDvr.usPort, stCurDvr.strUserName.c_str()); 	
				lstDvrIp.push_back(stCurDvr.strDvrIp);
				GetDvrPatrolPtr()->ChkDvrOnLine(stCurDvr);
				//��ʼ��ǰ�����豸����
				if(!GetDvrPatrolPtr()->StartDvrAlarmListen(stCurDvr))
				{
					continue;
				}
				lstDvrSucLst.push_back(stCurDvr);

			}
		}
		else
		{
			TPLOG_INFO("no dvr device");
			//Msger_NotifyMsg(ALL_USER,_T("��ȡ�ɼ��豸��Ϣʧ��"));
			continue;
		}
        //������豸��ɾ���� ֹͣ����豸�ļ���
        for(std::list<collect_info_t>::const_iterator itorBk = lstDvrBackupInfo.begin();
            itorBk != lstDvrBackupInfo.end();)
        {
            std::string strIp = itorBk->strDvrIp;
            std::list<std::string>::iterator itorIp = find(lstDvrIp.begin(), lstDvrIp.end(), strIp);
            if(itorIp == lstDvrIp.end())
            {
                collect_info_t stDvrInfo = *itorBk;
                if(!GetDvrPatrolPtr()->StopDvrAlarmListen(stDvrInfo))
                {
					lstDvrSucLst.push_back(stDvrInfo);    //ֹͣ����ʧ�ܣ����¼��벼���ɹ���DVR�б�
                    ++itorBk;
                    continue;
                }
                itorBk = lstDvrBackupInfo.erase(itorBk);
                continue;
            }
            ++itorBk;
        }
        lstDvrBackupInfo.clear();
        //���浱ǰ�Ѳ����豸��Ϣ�б� ��һ�μ���Ƿ����豸��ɾ��ֹͣ�����á�
        for(std::list<collect_info_t>::iterator itorBackUpIp = lstDvrSucLst.begin();
            itorBackUpIp != lstDvrSucLst.end(); itorBackUpIp++)
        {
            collect_info_t stBackUpDvrInfo = *itorBackUpIp;
            lstDvrBackupInfo.push_back(stBackUpDvrInfo);
        }
    } while (DelayBySleep(PATROL_WORK_INTERVAL, m_bStopWorker), !m_bStopWorker);

    for (std::list<collect_info_t>::iterator itorBackUp = lstDvrBackupInfo.begin();
        itorBackUp != lstDvrBackupInfo.end(); ++itorBackUp)
    {
        collect_info_t stDvrInfo = *itorBackUp;
        GetDvrPatrolPtr()->StopDvrAlarmListen(stDvrInfo);
    }

    TPLOG_INFO("�ɼ��豸Ѳ���߳���ֹ");
    Msger_NotifyMsg(ALL_USER,_T("�ɼ��豸Ѳ���߳���ֹ"));
    return ADJUST_ERROR_SUCC;
}

/** @fn     StoragePatrolProc
*  @brief  �洢�豸Ѳ���̣߳�����豸����״̬���豸������С
*  @return ���� ADJUST_ERROR_SUCC
    �쳣 �� ADJUST_ERROR_SUCC
*/
DWORD CAdjustTime::StoragePatrolProc(void)
{
	TPLOG_INFO("�洢�豸Ѳ���߳�����");
	do 
	{
		std::list<Struct_StorageDevice> listStorageDevice;
		if (!GetAllStorageDeviceReq(listStorageDevice))
		{
			continue;
		}
		if (!listStorageDevice.empty())
		{
			Struct_StorageDevice storageInfo;
			std::list<Struct_StorageDevice>::iterator iterStorageInfo;
			for (iterStorageInfo = listStorageDevice.begin(); iterStorageInfo != listStorageDevice.end() && !m_bStopWorker; ++iterStorageInfo)
			{
				storageInfo = *iterStorageInfo;
				TPLOG_INFO("StoragePatrol storage info(ip:%s, port:%d, user:%s)", storageInfo.strIp.c_str(), storageInfo.nPort, storageInfo.strUser.c_str()); 
				if (STORAGE_TYPE_CVR == storageInfo.nType)
				{
					CheckCVR(storageInfo);
				} 
				else if(STORAGE_TYPE_CLOUD == storageInfo.nType)
				{
					CheckCloud(storageInfo);
				}
				else if (STORAGE_TYPE_KMS == storageInfo.nType)
				{
					CheckKMS(storageInfo);
				}
				else if (STORAGE_TYPE_OBJECTCLOUD == storageInfo.nType)
				{
					CheckObjectCloud(storageInfo);
				}
				else
				{
					TPLOG_ERROR("unknown storage device(type:%d, name:%s, ip:%s, port:%d)",storageInfo.nType,
						storageInfo.strName.c_str(), storageInfo.strIp.c_str(), storageInfo.nPort);
				}
			}
		}
		else
		{
			TPLOG_INFO("no storage device");
			//Msger_NotifyMsg(ALL_USER,_T("��ȡ�洢�豸��Ϣʧ��"));
		}

	} while (DelayBySleep(STORAGE_CAPACITY_DETECT_INTERVAL, m_bStopWorker), !m_bStopWorker);

	TPLOG_INFO("�洢�豸Ѳ���߳���ֹ");
	Msger_NotifyMsg(ALL_USER,_T("�洢�豸Ѳ���߳���ֹ"));
    return 0; 
}

/** @fn     ImmAjustProc
*  @brief  ����Уʱһ�δ����߳�
*  @return ���� ADJUST_ERROR_SUCC
           �쳣 �� ADJUST_ERROR_SUCC
*/
DWORD CAdjustTime::ImmAjustProc(void)
{
    return AdjustTime();
}

/** @fn     GetCollectTimeStamp
*  @brief  ��ȡ�ɼ��豸��ǰʱ���ʱ���
*  @param  [in]lUserId, �ɼ��豸��½���
*  @param  [out]iCollectTimeStamp, �ɼ��豸��ǰʱ���ʱ���
*  @return ���� ADJUST_ERROR_SUCC
           �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::GetCollectTimeStamp(long lUserId, time_t &iCollectTimeStamp, DWORD& dwError)
{
    DWORD        dwReturned = 0;
    NET_DVR_TIME stDvrTime;
    memset(&stDvrTime, 0, sizeof(NET_DVR_TIME));
    //��ȡdvrʱ��
    if (!NET_DVR_GetDVRConfig(lUserId, NET_DVR_GET_TIMECFG, 0,
        &stDvrTime, sizeof(NET_DVR_TIME), &dwReturned))
    {
        dwError = NET_DVR_GetLastError();
        return ADJUST_ERROR_DVR;
    }

    struct tm    stTime = {0};
    //dvrʱ��ת��Ϊʱ���
    stTime.tm_year = stDvrTime.dwYear - TIME_COMPENSATE; //struct tm ��yearΪ����1900������
    stTime.tm_mon  = stDvrTime.dwMonth - 1; //struct tm�·ݴ�0��ʼ ���Լ�һ
    stTime.tm_mday = stDvrTime.dwDay;
    stTime.tm_hour = stDvrTime.dwHour;
    stTime.tm_min  = stDvrTime.dwMinute;
    stTime.tm_sec  = stDvrTime.dwSecond;
    iCollectTimeStamp = mktime(&stTime);

    return ADJUST_ERROR_SUCC;
}

/** @fn    SetCollectTime
*  @brief  ���òɼ��豸ʱ��
*  @param  [in]lUserId, �ɼ��豸��½���
*  @param  [in]iCollectTimeStamp, ʱ���
*  @param  [out]dwError, �쳣��
*  @return ���� ADJUST_ERROR_SUCC
           �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::SetCollectTime(long lUserId, time_t &iCollectTimeStamp, DWORD& dwError)
{
    NET_DVR_TIME stDvrTime;
    memset(&stDvrTime, 0, sizeof(NET_DVR_TIME));

    //ʱ���ת��Ϊstruct tm,���ø�NET_DVR_TIME
    struct tm stTime = {0};
    localtime_s(&stTime, &iCollectTimeStamp);
    stDvrTime.dwYear   = stTime.tm_year + TIME_COMPENSATE; //struct tm ��yearΪ����1900������
    stDvrTime.dwMonth  = stTime.tm_mon + 1; //struct tm�·ݴ�0��ʼ ���Լ�һ
    stDvrTime.dwDay    = stTime.tm_mday;
    stDvrTime.dwHour   = stTime.tm_hour;
    stDvrTime.dwMinute = stTime.tm_min;
    stDvrTime.dwSecond = stTime.tm_sec;

    //����dvrʱ��
    if (!NET_DVR_SetDVRConfig(lUserId, NET_DVR_SET_TIMECFG, 0, 
        &stDvrTime, sizeof(NET_DVR_TIME)))
    {
        dwError = NET_DVR_GetLastError();
        return ADJUST_ERROR_DVR;
    }

    return ADJUST_ERROR_SUCC;
}

/** @fn     AdjustTime
*  @brief  ���������Ļ�ȡ���вɼ��豸��Ϣ ��ÿ���ɼ��豸����Уʱ
*  @return ���� ADJUST_ERROR_SUCC
           �쳣 �� ADJUST_ERROR_SUCC
*/
int CAdjustTime::AdjustTime(void)
{
	TPLOG_INFO("adjust time in");
	std::list<collect_info_t> listCollectDevice;    //�ɼ��豸��Ϣ����
	//��ȡ�ɼ��豸��Ϣ
	if (!GetAllCollectDeviceReq(listCollectDevice))
	{
		return ADJUST_ERROR_DVR;
	}
	if (!listCollectDevice.empty())
	{
		collect_info_t stCurcollectInfo;
		NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
		NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
		std::list<collect_info_t>::iterator iterDvrInfo;
		for (iterDvrInfo = listCollectDevice.begin(); iterDvrInfo != listCollectDevice.end() && !m_bStopWorker; ++iterDvrInfo)
		{
			stCurcollectInfo = *iterDvrInfo;
			TPLOG_INFO("AdjustTime Start dvr info(ip:%s, port:%d, user:%s)", stCurcollectInfo.strDvrIp.c_str(), stCurcollectInfo.usPort, stCurcollectInfo.strUserName.c_str()); 	
			memcpy_s(struLoginInfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, stCurcollectInfo.strDvrIp.c_str(), strlen(stCurcollectInfo.strDvrIp.c_str()));
			struLoginInfo.wPort = stCurcollectInfo.usPort;
			memcpy_s(struLoginInfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, stCurcollectInfo.strUserName.c_str(), strlen(stCurcollectInfo.strUserName.c_str()));
			memcpy_s(struLoginInfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, stCurcollectInfo.strPassWd.c_str(), strlen(stCurcollectInfo.strPassWd.c_str()));
			struLoginInfo.cbLoginResult = NULL;
			struLoginInfo.pUser = NULL;
			struLoginInfo.bUseAsynLogin = 0;
			LONG lLoginID = DEVICE_LOGIN_FAILED;
			lLoginID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
			if (DEVICE_LOGIN_FAILED == lLoginID)
			{
				TPLOG_ERROR("login dvr %s:%d:%s failed(%d)",
					(LPCSTR)(stCurcollectInfo.strDvrIp.c_str()),
					stCurcollectInfo.usPort,
					(LPCSTR)(stCurcollectInfo.strUserName.c_str()),
					NET_DVR_GetLastError());
				Msger_NotifyMsg(ALL_USER, _T("��½�ɼ��豸:%s,%d,%s ʧ��, ������(%d)"),
					fcA2T(stCurcollectInfo.strDvrIp.c_str()),
					(int)(stCurcollectInfo.usPort),
					fcA2T(stCurcollectInfo.strUserName.c_str()),
					NET_DVR_GetLastError());
				continue;
			}
			time_t iCollectTimeStamp = 0;
			DWORD  dwError       = 0;
			if(ADJUST_ERROR_SUCC != GetCollectTimeStamp(lLoginID, iCollectTimeStamp, dwError))
			{
				TPLOG_ERROR("Get Dvr Time %s:%d:%s failed(%d)",
					(LPCSTR)(stCurcollectInfo.strDvrIp.c_str()),
					stCurcollectInfo.usPort,
					(LPCSTR)(stCurcollectInfo.strUserName.c_str()),
					dwError);
				Msger_NotifyMsg(ALL_USER, _T("��ȡ�ɼ��豸:%s,%d,%sʱ�� ʧ��, ������(%d)"),
					fcA2T(stCurcollectInfo.strDvrIp.c_str()),
					(int)(stCurcollectInfo.usPort),
					fcA2T(stCurcollectInfo.strUserName.c_str()),
					dwError);
				NET_DVR_Logout(lLoginID);
				continue;
			}

			//����ʱ���
			time_t iLocalTime = time(NULL);

			//���ʱ��һ�²���ҪУʱ
			if(iLocalTime == iCollectTimeStamp)
			{
				TPLOG_INFO("iLocalTime == iCollectTimeStamp dvr:%s ", (LPCSTR)(stCurcollectInfo.strDvrIp.c_str()));
				NET_DVR_Logout(lLoginID);
				continue;
			}

			//���㱾�� �� dvr��ʱ��
			time_t iTimeLag = 0;
			if(iLocalTime > iCollectTimeStamp)
			{
				iTimeLag = iLocalTime - iCollectTimeStamp;
			}
			else
			{
				iTimeLag = iCollectTimeStamp - iLocalTime;
			}

			//ʱ�������û����������Զ�Уʱ���ʱ��� ������Զ�Уʱ���������û��ֶ�����
			//m_uiMaxTimeLag ��λ�Ƿ��� ת��Ϊ��
			time_t iMaxTimeLagSec = (time_t)m_stAdjustConfig.uiMaxTimeLag * (time_t)SECONDS_PER_MIN;
			if(iTimeLag >= iMaxTimeLagSec)
			{
				TPLOG_ERROR("dvr %s Time Lag is too long, please use manual operation ",
					(LPCSTR)(stCurcollectInfo.strDvrIp.c_str()));
				Msger_DeviceAlarmMsg(WORK_TYPE_DVR_TIME_ERROR,
					ALL_USER,
					ALL_CHANNEL,
					ALARM_OCCUR,
					TP_ALARM_ADJUST,
					stCurcollectInfo.strDvrIp.c_str(),
					stCurcollectInfo.strDvrIp.size(),
					_T("�ɼ��豸:%s �ͱ���ʱ���̫��(%d��),���˹�����"),
					fcA2T(stCurcollectInfo.strDvrIp.c_str()),
					iTimeLag);
				NET_DVR_Logout(lLoginID);
				continue;
			}

			//����dvrʱ��
			if(ADJUST_ERROR_SUCC != SetCollectTime(lLoginID, iLocalTime, dwError))
			{
				TPLOG_ERROR("set Device Time %s:%d:%s failed(%d)",
					(LPCSTR)(stCurcollectInfo.strDvrIp.c_str()),
					stCurcollectInfo.usPort,
					(LPCSTR)(stCurcollectInfo.strUserName.c_str()),
					dwError);
				Msger_NotifyMsg(ALL_USER, _T("���òɼ��豸:%s,%d,%sʱ�� ʧ��, ������(%d)"),
					fcA2T(stCurcollectInfo.strDvrIp.c_str()),
					(int)(stCurcollectInfo.usPort),
					fcA2T(stCurcollectInfo.strUserName.c_str()),
					dwError);
				NET_DVR_Logout(lLoginID);
				continue;
			}

			//�ǳ�dvr
			NET_DVR_Logout(lLoginID);
			TPLOG_INFO("AdjustTime End dvr info(ip:%s, port:%d, user:%s)", stCurcollectInfo.strDvrIp.c_str(), stCurcollectInfo.usPort, stCurcollectInfo.strUserName.c_str());

		}//for (iterDvrInfo = m_listCollectDevice.begin(); iterDvrInfo != m_listCollectDevice.end() && !m_bStopWorker; ++iterDvrInfo)
	}
	else
	{
		TPLOG_INFO("no dvr device");
		//Msger_NotifyMsg(ALL_USER, _T("��ȡ�ɼ��豸��Ϣʧ��"));
	}

    return ADJUST_ERROR_SUCC;
}

//�ص���������ȡ���ؽ��
size_t OnCallBackGetHttpInfo(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str)
	{
		TPLOG_ERROR("str is NULL...");
		return 0;
	}
	if( NULL == buffer )
	{
		TPLOG_ERROR("buffer is NULL...");
		return 0;
	}

	char* pData = (char*)buffer;
	str->append(pData, size * nmemb);

	return nmemb * size;   
}

//�ص���������ȡ���ؽ��
size_t OnCallBackGetKmsSpaceInfo(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		TPLOG_ERROR("str or buffer is NULL...");
		return 0;
	}

	char* pData = (char*)buffer;
	str->append(pData, size * nmemb);

	return nmemb * size;   
}

/**	@fn	    CheckCVR
*	@brief	���CVR�Ƿ������Լ�CVR�����Ƿ��㹻
*	@param  [in] struStorage -- CVR��½��Ϣ
*	@return	bool
*/
bool CAdjustTime::CheckCVR(const Struct_StorageDevice &struStorage)
{
	LONG lLoginID = DEVICE_LOGIN_FAILED;
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
	memcpy_s(struLoginInfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, struStorage.strIp.c_str(), strlen(struStorage.strIp.c_str()));
	struLoginInfo.wPort = struStorage.nPort;
	memcpy_s(struLoginInfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, struStorage.strUser.c_str(), strlen(struStorage.strUser.c_str()));
	memcpy_s(struLoginInfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, struStorage.strPwd.c_str(), strlen(struStorage.strPwd.c_str()));
	struLoginInfo.cbLoginResult = NULL;
	struLoginInfo.pUser = NULL;
	struLoginInfo.bUseAsynLogin = 0;
	lLoginID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);

	if (DEVICE_LOGIN_FAILED == lLoginID)
	{
		DWORD dwError = NET_DVR_GetLastError();
		TPLOG_ERROR("login cvr %s:%d:%s failed(%d)",
			struStorage.strIp.c_str(),
			struStorage.nPort,
			struStorage.strUser.c_str(),
			dwError);
		Msger_NotifyMsg(ALL_USER, _T("��½CVRʧ��:%s:%d:%s������(%d)"),
			CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
			struStorage.nPort,
			CSCT::Ansi2Unicode(struStorage.strUser).c_str(),
			dwError);
		if(NET_DVR_NETWORK_FAIL_CONNECT == dwError)
		{
			TPLOG_ERROR("cvr offline %s:%d:%s",
				(LPCSTR)(struStorage.strIp.c_str()),
				struStorage.nPort,
				(LPCSTR)(struStorage.strName.c_str()));
			Msger_DeviceAlarmMsg(WORK_TYPE_CVR_OFFLINE,
				ALL_USER,
				ALL_CHANNEL,
				ALARM_OCCUR,
				TP_ALARM_CVR,
				struStorage.strIp.c_str(),
				struStorage.strIp.size(),
				_T("CVR:%s:%d ������"),
				fcA2T(struStorage.strIp.c_str()), struStorage.nPort);
			if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_CVR, struStorage.strIp, struStorage.nPort, OFFLINE))
			{
				TPLOG_ERROR("update cvr:%s offline state failed",
					(LPCSTR)(struStorage.strIp.c_str()));
			}
		}
		return false;
	}
	if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_CVR, struStorage.strIp, struStorage.nPort, ONLINE))
	{
		TPLOG_ERROR("update cvr:%s:%d online state failed",
			(LPCSTR)(struStorage.strIp.c_str()), struStorage.nPort);
	}
	BOOL  bRet  = false;
	DWORD dwRet = 0;
	NET_DVR_HDCFG struCvrHDCfg; 
    struCvrHDCfg.dwSize = sizeof(NET_DVR_HDCFG);
	bRet = NET_DVR_GetDVRConfig(lLoginID, NET_DVR_GET_HDCFG, 0, &struCvrHDCfg, sizeof(struCvrHDCfg), &dwRet);
	NET_DVR_Logout(lLoginID);
	if (bRet)
	{
		std::_tstring strAlarmInfo;
		unsigned long long ntotalSpace = 0;
		unsigned long long nfreeSpace = 0;  
		for (DWORD dwHDNum = 0; dwHDNum < struCvrHDCfg.dwHDCount; ++dwHDNum)
		{
			if (dwHDNum >= MAX_DISKNUM_V30)
			{
				break;
			}
			CheckCVRCapacity(struCvrHDCfg.struHDInfo[dwHDNum], strAlarmInfo);
			ntotalSpace += struCvrHDCfg.struHDInfo[dwHDNum].dwCapacity;
			nfreeSpace  += struCvrHDCfg.struHDInfo[dwHDNum].dwFreeSpace;
		}
		if (!strAlarmInfo.empty())
		{
			TPLOG_INFO("cvr alarm info %s:%d:%s:%s",
				struStorage.strIp.c_str(),
				struStorage.nPort,
				struStorage.strUser.c_str(),
				CSCT::Tstring2Ansi(strAlarmInfo).c_str());
			Msger_DeviceAlarmMsg(WORK_TYPE_CVR_NEARLLY_FULL,
				ALL_USER,
				ALL_CHANNEL,
				ALARM_OCCUR,
				TP_ALARM_CVR,
				struStorage.strIp.c_str(),
				struStorage.strIp.size(),
				_T("CVR�洢::%s,%s"),
				CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
				strAlarmInfo.c_str());
		}
		ntotalSpace /= G_MB;
		nfreeSpace /= G_MB;
 		if (GetDBDataMgrPtr()->UpdateStorageCapacityReq(cms_8120::DEV_TYPE_CVR, struStorage.strIp, struStorage.nPort, ntotalSpace, nfreeSpace))
		{
			TPLOG_ERROR("update cvr:%s capacity failure", struStorage.strIp.c_str());
		}
	} 
	else
	{
		DWORD dwError = NET_DVR_GetLastError();
		TPLOG_ERROR("get cvr capacity %s:%d:%s failed(%d)",
			struStorage.strIp.c_str(),
			struStorage.nPort,
			struStorage.strUser.c_str(),
			dwError);
		Msger_NotifyMsg(ALL_USER, _T("��ȡCVRӲ��������Ϣʧ��:%s:%d:%s������(%d)"),
			CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
			struStorage.nPort,
			CSCT::Ansi2Unicode(struStorage.strUser).c_str(),
			dwError);
		return false;
	}

	return true;
}

/**	@fn	    CheckCloud
*	@brief	����ƴ洢�Ƿ������Լ��ƴ洢�����Ƿ��㹻
*	@param  [in] struStorage -- �ƴ洢��½��Ϣ
*	@return	bool
*/
bool CAdjustTime::CheckCloud(const Struct_StorageDevice &struStorage)
{
	HCS_Version stCloudVersion;
	memset(&stCloudVersion, 0 ,sizeof(stCloudVersion));
	if (CLOUD_OFFLINE != HCS_SDK_GetVersion(struStorage.strIp.c_str(), struStorage.nPort, &stCloudVersion)) //Cloud����
	{
		if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_CLOUD_STORAGE, struStorage.strIp, struStorage.nPort, ONLINE))
		{
			TPLOG_ERROR("update cloud:%s:%d online state failed",
				(LPCSTR)(struStorage.strIp.c_str()), struStorage.nPort);
		}
	}
	else //Cloud������
	{
		TPLOG_ERROR("cloud offline %s:%d:%s",
			(LPCSTR)(struStorage.strIp.c_str()),
			struStorage.nPort,
			(LPCSTR)(struStorage.strName.c_str()));
		Msger_DeviceAlarmMsg(WORK_TYPE_CLOUD_OFFLINE,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_CLOUD,
			struStorage.strIp.c_str(),
			struStorage.strIp.size(),
			_T("Cloud:%s:%d ������"),
			fcA2T(struStorage.strIp.c_str()), struStorage.nPort);
		if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_CLOUD_STORAGE, struStorage.strIp, struStorage.nPort, OFFLINE))
		{
			TPLOG_ERROR("update cloud:%s:%d offline state failed",
				(LPCSTR)(struStorage.strIp.c_str()), struStorage.nPort);
		}
		return false;
	}

	LONG loginID = LoginCloud(struStorage);
	if (DEVICE_LOGIN_FAILED == loginID)
	{
		TPLOG_ERROR("login cloud %s:%d:%s failed(%d)",
			struStorage.strIp.c_str(),
			struStorage.nPort,
			struStorage.strUser.c_str(),
			loginID);
		Msger_NotifyMsg(ALL_USER, _T("��½Cloudʧ��:%s:%d:%s������(%d)"),
			CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
			struStorage.nPort,
			CSCT::Ansi2Unicode(struStorage.strUser).c_str(),
			loginID);
		return false;
	}

	std::vector<HCS_PoolInfo> vecPoolInfo;
	bool bRetGetCloudPool = GetCloudPoolInfo(loginID, vecPoolInfo);
	HCS_SDK_Logout(loginID);
	if (bRetGetCloudPool)//��ȡ���ݳسɹ�
	{
		std::_tstring strAlarmInfo;
		unsigned long long ntotalSpace = 0;
		unsigned long long nfreeSpace = 0;
		for(std::vector<HCS_PoolInfo>::iterator iter=vecPoolInfo.begin();iter!=vecPoolInfo.end();iter++)
		{
			CheckCloudCapacity(*iter, strAlarmInfo);
			ntotalSpace += iter->pool_size;
			nfreeSpace  += iter->free_size;
		}
		if (!strAlarmInfo.empty())
		{
			TPLOG_INFO("cloud alarm info %s:%d:%s:%s",
				struStorage.strIp.c_str(),
				struStorage.nPort,
				struStorage.strUser.c_str(),
				CSCT::Tstring2Ansi(strAlarmInfo).c_str());
			Msger_DeviceAlarmMsg(WORK_TYPE_CLOUD_NEARLLY_FULL,
				ALL_USER,
				ALL_CHANNEL,
				ALARM_OCCUR,
				TP_ALARM_CLOUD,
				struStorage.strIp.c_str(),
				struStorage.strIp.size(),
				_T("�ƴ洢::%s,%s"),
				CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
				strAlarmInfo.c_str());
		}
		ntotalSpace /= G_MB;
		nfreeSpace /= G_MB;
		if (GetDBDataMgrPtr()->UpdateStorageCapacityReq(cms_8120::DEV_TYPE_CLOUD_STORAGE, struStorage.strIp, struStorage.nPort, ntotalSpace, nfreeSpace))
		{
			TPLOG_ERROR("update cloud:%s capacity failure", struStorage.strIp.c_str());
		}
	} 
	else
	{
		TPLOG_ERROR("find pool failed in cloud %s:%d:%s",
			struStorage.strIp.c_str(),
			struStorage.nPort,
			struStorage.strUser.c_str());
		Msger_NotifyMsg(ALL_USER, _T("�ƴ洢 %s:%d:%s ��ȡ���ݳ�ʧ��"),
			CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
			struStorage.nPort,
			CSCT::Ansi2Unicode(struStorage.strUser).c_str());
		return false;
	}

	return true;
}

/**	@fn	    CheckKMS
*	@brief	���KMS�Ƿ������Լ�KMS�����Ƿ��㹻
*	@param  [in] struStorage -- KMS��½��Ϣ
*	@return	bool
*/
bool CAdjustTime::CheckKMS(const Struct_StorageDevice &struStorage)
{
	if (TestRemoteHost(struStorage.strIp.c_str(), (USHORT)(struStorage.nPort))) //KMS����
	{
		if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_KMS, struStorage.strIp, struStorage.nPort, ONLINE))
		{
			TPLOG_ERROR("update kms:%s:%d online state failed",
				(LPCSTR)(struStorage.strIp.c_str()), struStorage.nPort);
		}
	}
	else //KMS������
	{
		TPLOG_ERROR("kms offline %s:%d:%s",
			(LPCSTR)(struStorage.strIp.c_str()),
			struStorage.nPort,
			(LPCSTR)(struStorage.strName.c_str()));
		Msger_DeviceAlarmMsg(WORK_TYPE_KMS_OFFLINE,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_KMS,
			struStorage.strIp.c_str(),
			struStorage.strIp.size(),
			_T("KMS:%s:%d ������"),
			fcA2T(struStorage.strIp.c_str()), struStorage.nPort);
		if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_KMS, struStorage.strIp, struStorage.nPort, OFFLINE))
		{
			TPLOG_ERROR("update kms:%s:%d offline state failed",
				(LPCSTR)(struStorage.strIp.c_str()), struStorage.nPort);
		}
		return false;
	}
	
	std::string strToken;
	if (!GetHttpToken(struStorage, strToken))
	{
		TPLOG_ERROR("kms[ip:%s, port:%d] GetHttpToken failed.", 
			struStorage.strIp.c_str(), struStorage.nPort);
		Msger_NotifyMsg(ALL_USER, _T("��ȡKMS��token��֤��Ϣʧ��"));
		return false;
	}

	double dKmsFreeSpace = 0;
	double dKmsTotalSpace = 0;
	if(!GetKmsSpaceInfo(struStorage, strToken, dKmsFreeSpace, dKmsTotalSpace))
	{
		TPLOG_ERROR("kms[ip:%s, port:%d] GetKmsSpaceInfo failed.", 
			struStorage.strIp.c_str(), struStorage.nPort);
		Msger_NotifyMsg(ALL_USER, _T("��ȡKMS��������Ϣʧ��"));
		return false;    
	}
	std::_tstring strAlarmInfo;
	CheckKmsCapacity(dKmsFreeSpace, dKmsTotalSpace, strAlarmInfo);
	if (!strAlarmInfo.empty())
	{
		TPLOG_INFO("kms alarm info %s:%d:%s:%s",
			struStorage.strIp.c_str(),
			struStorage.nPort,
			struStorage.strUser.c_str(),
			CSCT::Tstring2Ansi(strAlarmInfo).c_str());
		Msger_DeviceAlarmMsg(WORK_TYPE_KMS_NEARLLY_FULL,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_KMS,
			struStorage.strIp.c_str(),
			struStorage.strIp.size(),
			_T("KMS�洢:%s,%s"),
			CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
			strAlarmInfo.c_str());
	}
	if (GetDBDataMgrPtr()->UpdateStorageCapacityReq(cms_8120::DEV_TYPE_KMS, struStorage.strIp, struStorage.nPort, static_cast<unsigned long long>(dKmsTotalSpace), static_cast<unsigned long long>(dKmsFreeSpace)))
	{
		TPLOG_ERROR("update KMS:%s capacity failure", struStorage.strIp.c_str());
	}
	return true;
}

/**	@fn	    CheckObjectCloud
*	@brief	�������ƴ洢�Ƿ������Լ������ƴ洢�����Ƿ��㹻
*	@param  [in] struStorage -- �����ƴ洢��½��Ϣ
*	@return	bool
*/
bool CAdjustTime::CheckObjectCloud(const Struct_StorageDevice &struStorage)
{
	if (TestRemoteHost(struStorage.strIp.c_str(), (USHORT)(struStorage.nPort))) //�����ƴ洢SDKδ�ṩ����״̬���ӿڣ����Դ˷������
	{
		if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_OBJECTCLOUD, struStorage.strIp, struStorage.nPort, ONLINE))
		{
			TPLOG_ERROR("update objectcloud:%s:%d online state failed",
				(LPCSTR)(struStorage.strIp.c_str()), struStorage.nPort);
		}
	}
	else //ObjectCloud������
	{
		TPLOG_ERROR("objectcloud offline %s:%d:%s",
			(LPCSTR)(struStorage.strIp.c_str()),
			struStorage.nPort,
			(LPCSTR)(struStorage.strName.c_str()));
		Msger_DeviceAlarmMsg(WORK_TYPE_OBJECTCLOUD_OFFLINE,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_OBJECTCLOUD,
			struStorage.strIp.c_str(),
			struStorage.strIp.size(),
			_T("��׼��-����洢:%s:%d ������"),
			fcA2T(struStorage.strIp.c_str()), struStorage.nPort);
		if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_OBJECTCLOUD, struStorage.strIp, struStorage.nPort, OFFLINE))
		{
			TPLOG_ERROR("update objectcloud:%s:%d offline state failed",
				(LPCSTR)(struStorage.strIp.c_str()), struStorage.nPort);
		}
		return false;
	}

	char endpoint[30]={0};
	HCSClient* pClient = NULL;
	sprintf(endpoint, "%s:%d", struStorage.strIp.c_str(), struStorage.nPort);
	pClient = ClientInitialize(struStorage.strAccessKey.c_str(), struStorage.strSecretKey.c_str(), endpoint);
	//���ó�ʼ����Ϣ����
	if (NULL == pClient)
	{
		TPLOG_ERROR("ClientInitialize  fail,IP(%s),Port(%d)", struStorage.strIp.c_str(), struStorage.nPort);
		Msger_NotifyMsg(ALL_USER, _T("��½��׼��ʧ��:%s:%d"),
			CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
			struStorage.nPort);
		return false;
	}
	std::_tstring strAlarmInfo;
	unsigned long long ntotalSpace = 0;
	unsigned long long nfreeSpace = 0;
	int buckets_num = 0;
	HCSBucketInfo *buckets_list = NULL;
	int result = ListBuckets(pClient, &buckets_num, &buckets_list);
	if (0 == result)
	{
		if (0 == buckets_num)
		{
			TPLOG_ERROR("ListBuckets list is empty!");
			FreeHCSBucketInfo(pClient, &buckets_num, &buckets_list);
			ClientFinalize(pClient);
			return false;
		}
		for (int i = 0; i < buckets_num; i++)
		{
			ntotalSpace += buckets_list[i].bucket_size;
			nfreeSpace  += buckets_list[i].free_size;
			CheckObjectCloudCapacity(&(buckets_list[i]), strAlarmInfo);
		}
		if (!strAlarmInfo.empty())
		{
			TPLOG_INFO("objectcloud alarm info %s:%d:%s",
				struStorage.strIp.c_str(),
				struStorage.nPort,
				CSCT::Tstring2Ansi(strAlarmInfo).c_str());
			Msger_DeviceAlarmMsg(WORK_TYPE_OBJECTCLOUD_NEARLLY_FULL,
				ALL_USER,
				ALL_CHANNEL,
				ALARM_OCCUR,
				TP_ALARM_OBJECTCLOUD,
				struStorage.strIp.c_str(),
				struStorage.strIp.size(),
				_T("��׼��-����洢::%s,%s"),
				CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
				strAlarmInfo.c_str());
		}
		if (GetDBDataMgrPtr()->UpdateStorageCapacityReq(cms_8120::DEV_TYPE_OBJECTCLOUD, struStorage.strIp, struStorage.nPort, ntotalSpace, nfreeSpace))
		{
			TPLOG_ERROR("update objectcloud:%s capacity failure", struStorage.strIp.c_str());
		}

	}
	else
	{
		TPLOG_ERROR("ListBuckets failed in objectcloud %s:%d,errorcode(%d)",
			struStorage.strIp.c_str(),
			struStorage.nPort,
			result);
		Msger_NotifyMsg(ALL_USER, _T("�ƴ洢 %s:%d ��ȡ���ݳ�ʧ��,������(%d)"),
			CSCT::Ansi2Unicode(struStorage.strIp).c_str(),
			struStorage.nPort,
			result);
		FreeHCSBucketInfo(pClient, &buckets_num, &buckets_list);
		ClientFinalize(pClient);
		return false;
	}
	FreeHCSBucketInfo(pClient, &buckets_num, &buckets_list);
	ClientFinalize(pClient);
	return true;
}

/**	@fn	    LoginCloud
*	@brief	��¼�ƴ洢
*	@param  [in] struStorage -- �ƴ洢��½��Ϣ
*	@return	LONG �ƴ洢��¼���
*/
LONG CAdjustTime::LoginCloud(const Struct_StorageDevice &struStorage)
{
	LONG lLoginID = DEVICE_LOGIN_FAILED;
	unsigned long long lLogin;
	int nRes = HCS_SDK_Login(struStorage.strIp.c_str(), struStorage.nPort, struStorage.strUser.c_str(), struStorage.strPwd.c_str(), &lLogin);
	if (HCS_OK != nRes)
	{
		TPLOG_ERROR("call HCS_SDK_Login failed.ip:%s, port:%d, error[%d]",
			struStorage.strIp.c_str(), struStorage.nPort, nRes);
		return DEVICE_LOGIN_FAILED;
	}
	lLoginID = (LONG)lLogin;
	return lLoginID;
}

/**	@fn	    GetCloudPoolInfo
 *	@brief	��ȡCloud���ݳ���Ϣ
 *	@param  [in] loginID,     �ƴ洢��½���
 *	@param  [out]vecPoolInfo, ���ݳ���Ϣ����
 *	@return	bool
 */
bool CAdjustTime::GetCloudPoolInfo(LONG loginID, std::vector<HCS_PoolInfo>& vecPoolInfo)
{
	int hGetPoolInfo;
	int nPoolCount;
	int retGetPoolStart = HCS_SDK_GetAllPoolInfoStart(loginID, &nPoolCount, &hGetPoolInfo);
	if (HCS_OK == retGetPoolStart)
	{
		while(true)
		{
			int retQueryPool = -1;
			int queryState   = -1;
			HCS_PoolInfo struPoolInfo = {0};
			retQueryPool = HCS_SDK_GetAllPoolInfoNext(loginID, hGetPoolInfo, &struPoolInfo, &queryState);
			if (HCS_OK  != retQueryPool) //����ʧ��
			{
				TPLOG_ERROR("call HCS_SDK_GetAllPoolInfoNext failed.error[%d]", retQueryPool);
				HCS_SDK_GetAllPoolInfoStop(loginID, hGetPoolInfo);
				return false;
			}
			else if(CLOUD_GET_NEXT_POOL_FINISH == queryState) //���ҽ���
			{
				break;
			}
			else if(CLOUD_GET_NEXT_POOL_NEED_WAIT == queryState) //���ڲ���
			{
				continue;
			}
			else if(CLOUD_GET_NEXT_POOL_SUCCESS == queryState) //��ȡ���ݳ���Ϣ�ɹ�
			{	
				vecPoolInfo.push_back(struPoolInfo);
			}
			else
			{
				TPLOG_ERROR("Call HCS_SDK_GetAllPoolInfoNext Return State Failure");
			}
		}//while(true)
	}//if (HCS_OK == nRet)
	else
	{
		TPLOG_ERROR("Call HCS_SDK_GetAllPoolInfoStart Failure");
		return false;
	}
	HCS_SDK_GetAllPoolInfoStop(loginID, hGetPoolInfo);
	if (vecPoolInfo.size() != nPoolCount) //��ȡ���ݳ�ʧ��
	{
		TPLOG_ERROR("get cloud pool info failed.");
		return false;
	} 
	return true;
}

/**	@fn	    GetKmsSpaceInfo
 *	@brief	��ȡKMSʣ����������������С
 *	@param  [in]struStorage, KMS��Ϣ
 *	@param  [in]strToken, token��֤��Ϣ
 *	@param  [out]dFreeSpace,  KMSʣ������
 *	@param  [out]dTotalSpace, KMS������
 *	@return	bool
 */
bool CAdjustTime::GetKmsSpaceInfo(const Struct_StorageDevice &struStorage, const std::string& strToken, double& dFreeSpace, double& dTotalSpace)
{
    std::string strResponse;
    CURL* curl = curl_easy_init();
    if(NULL == curl)  
    {  
        TPLOG_ERROR("curl is null...");
        return false;  
    }  
    CURLcode res; 
    char szURL[200] = {0};
    sprintf_s(szURL, sizeof(szURL), "http://%s:%d/kms/services/rest/CRL/getSpaceInfo?token=%s", 
        struStorage.strIp.c_str(), struStorage.nPort, strToken.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, szURL); 
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&strResponse ); 
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, OnCallBackGetKmsSpaceInfo); 

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    res = curl_easy_perform( curl ); 
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        TPLOG_ERROR("curl_easy_perform failed.res: %d", res);
        return false;
    }
    TPLOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
//     {
//         "ret": 0,
//             "msg": "�����ɹ�",
//             "freeSpace": 3211368378368,
//             "totalSpace": 3221127163904
//     }
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            double dTemp = 0;
            dTemp = root["freeSpace"].asDouble();
            dFreeSpace = (( dTemp / 1024 ) / 1024 ) / 1024;
            
            dTemp = root["totalSpace"].asDouble();
            dTotalSpace = (( dTemp / 1024 ) / 1024 ) / 1024;
            TPLOG_INFO("kms[ip:%s, port:%d] freeSpace is %lgGB, totalSpace is %lgGB", 
                struStorage.strIp.c_str(), struStorage.nPort, dFreeSpace, dTotalSpace);
            return true;
        }
        else
        {
            std::string strMsg = root["msg"].asString();
            TPLOG_ERROR("deleteFiles failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
            return false;
        }
    }
    else
    {
        TPLOG_ERROR("parse upload info failed...");
        return false;
    }    
}

/**	@fn	    CheckCVRCapacity
 *	@brief	���CVRӲ�������Ƿ��㹻�������������Ϣ
 *	@param  [in]struCvrHDInfo, CVRӲ����Ϣ
 *	@param  [out]alarmInfo, ������Ϣ
 *	@return	void
 */
void CAdjustTime::CheckCVRCapacity(const NET_DVR_SINGLE_HD &struCvrHDInfo, std::_tstring &alarmInfo)
{
	const DWORD dwGroup = (DWORD)(struCvrHDInfo.dwCapacity / G_MB);
	if (dwGroup < MIN_GROUP_SIZE) // ������̫С�ˣ�����100G
	{
		std::_tstringstream ss;
		ss << _T(" ���� ") << struCvrHDInfo.dwHDNo << _T(":")
			<< _T(" ̫С�ˣ���С:") << dwGroup << _T("G");
		alarmInfo += ss.str();

		return;
	}

	const double freePercent = struCvrHDInfo.dwFreeSpace * 100.0 / struCvrHDInfo.dwCapacity;

	if (freePercent < MIN_GROUP_PERCENT) // ʣ������̫С�ˣ�����ռ��С��5%
	{
		std::_tstringstream ss;
		ss << _T(" ���� ") << struCvrHDInfo.dwHDNo << _T(":")
			<< _T("�����ˣ���ʣ:") << freePercent << _T("%");;
		alarmInfo += ss.str();

		return;
	}
}

/**	@fn	    CheckCloudCapacity
 *	@brief	����ƴ洢�����Ƿ��㹻�������������Ϣ
 *	@param  [in]struPoolInfo, �ƴ洢��Դ����Ϣ
 *	@param  [out]alarmInfo, ������Ϣ
 *	@return	void
 */
void CAdjustTime::CheckCloudCapacity(const HCS_PoolInfo &struPoolInfo, std::_tstring &alarmInfo)
{
	const DWORD dwGroup = (DWORD)(struPoolInfo.pool_size / G_MB);
	if (dwGroup < MIN_GROUP_SIZE) // ������̫С�ˣ�����100G
	{
		std::_tstringstream ss;
		ss << _T(" ¼��� ") << struPoolInfo.pool_id << _T(":") << struPoolInfo.pool_name
			<< _T(" ̫С�ˣ���С:") << dwGroup << _T("G");
		alarmInfo += ss.str();

		return;
	}

	const double freePercent = struPoolInfo.free_size * 100.0 / struPoolInfo.pool_size;
	if (freePercent < MIN_GROUP_PERCENT) // ʣ������̫С�ˣ�����ռ��С��5%
	{
		std::_tstringstream ss;
		ss << _T(" ¼��� ") << struPoolInfo.pool_id << _T(":") << struPoolInfo.pool_name
			<< _T("�����ˣ���ʣ:") << freePercent << _T("%");;
		alarmInfo += ss.str();

		return;
	}
}

/**	@fn	    CheckKmsCapacity
 *	@brief	���KMS�����Ƿ��㹻�������������Ϣ
 *	@param  [in]dFreeSpace,  KMSʣ������
 *	@param  [in]dTotalSpace, KMS������
 *	@param  [out]alarmInfo, ������Ϣ
 *	@return	void
 */
void CAdjustTime::CheckKmsCapacity(const double& dFreeSpace, const double& dTotalSpace, std::_tstring &alarmInfo)
{
    if (dTotalSpace < MIN_GROUP_SIZE) // ������̫С�ˣ�����100G
    {
		std::_tstringstream ss;
        ss <<_T(" ���������㣬��С:") << dTotalSpace << _T("G");
        alarmInfo += ss.str();

        return;
    }

    const double freePercent = (dFreeSpace / dTotalSpace) * 100;

    if (freePercent < MIN_GROUP_PERCENT) // ʣ������̫С�ˣ�����ռ��С��5%
    {
		std::_tstringstream ss;
        ss << _T("ʣ���������㣬ʣ��:") << freePercent << _T("%");
        alarmInfo += ss.str();

        return;
    }
}

/**	@fn	    CheckObjectCloudCapacity
 *	@brief	�������ƴ洢�����Ƿ��㹻�������������Ϣ
 *	@param  [in]bucket_info, �����ƴ洢��Դ����Ϣ
 *	@param  [out]alarmInfo, ������Ϣ
 *	@return	void
 */
void CAdjustTime::CheckObjectCloudCapacity(HCSBucketInfo* bucket_info, std::_tstring &alarmInfo)
{
	if (bucket_info->bucket_size < MIN_GROUP_SIZE) // ������̫С�ˣ�����100G
	{
		std::_tstringstream ss;
		ss << _T(" ¼��� ") << _T(":") << bucket_info->bucket_name
			<< _T(" ̫С�ˣ���С:") << bucket_info->bucket_size << _T("G");
		alarmInfo += ss.str();

		return;
	}

	const double freePercent = bucket_info->free_size * 100.0 / bucket_info->bucket_size;
	if (freePercent < MIN_GROUP_PERCENT) // ʣ������̫С�ˣ�����ռ��С��5%
	{
		std::_tstringstream ss;
		ss << _T(" ¼��� ") << _T(":") << bucket_info->bucket_name
			<< _T("�����ˣ���ʣ:") << freePercent << _T("%");;
		alarmInfo += ss.str();

		return;
	}
}

/** @fn GetHttpToken
*  @brief  ��KMS��ȡToken��֤��Ϣ
*  @param [in]struStorage KMS��������Ϣ
*  @param [out]strToken ��ȡ����token��Ϣ
*  @return bool
*/
bool CAdjustTime::GetHttpToken(const Struct_StorageDevice &struStorage, std::string& strToken)
{
	std::string strResponse;
	CURLcode res;  
	CURL* curl = curl_easy_init();  
	if(NULL == curl)  
	{  
		TPLOG_ERROR("curl is null...");
		return false;  
	}  

	char strURL[200] = {0};
	sprintf_s(strURL, sizeof(strURL), "http://%s:%d/kms/services/rest/operationService/getTokenInfo?username=%s&&password=%s", 
		struStorage.strIp.c_str(), struStorage.nPort, struStorage.strUser.c_str(), struStorage.strPwd.c_str());
	curl_easy_setopt(curl, CURLOPT_URL, strURL);
	/*curl_easy_setopt(curl, CURLOPT_URL, "http://10.16.38.27:8080/kms/services/rest/operationService/getTokenInfo?username=administrator&&password=hik12345_");*/
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL); 
	//���ûص�
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo);

	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);      
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
	res = curl_easy_perform(curl);  
	curl_easy_cleanup(curl);
	if (res != CURLE_OK)
	{
		TPLOG_ERROR("curl_easy_perform failed.res:%d", (int)res);
		return false;
	}
	TPLOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
	//{"node":"10.16.38.27:8080","ret":0,"msg":"�����ɹ�","tokenError":false,"token":"807d6bf90000014f60ecfe434f42a54555"}
	//����jsoncpp��������ص�����
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResponse.c_str(), root))
	{
		int nRet = root["ret"].asInt();
		if (0 == nRet)
		{
			strToken = root["token"].asString();
		}
		else
		{
			std::string strMsg = root["msg"].asString();
			TPLOG_ERROR("getToken failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
			return false;
		}
	}
	else
	{
		TPLOG_ERROR("parse token info failed...");
		return false;
	}
	TPLOG_INFO("strToken is %s", fcU2A(strToken.c_str())); 
	return true;    
}

/**	@fn		TestRemoteHost
*   @brief    ����Զ�������Ƿ�����
*   @param  [in]  szIP, Զ������IP��ַ
*   @param  [in]  nPort,Զ�������˿�
*   @return	BOOL, TRUE ���� FALSE ������
*/
BOOL CAdjustTime::TestRemoteHost(const char *szIP, USHORT nPort)
{
	class CInitSocket{
	public:
		CInitSocket():m_bInited(false){}
		bool Init()
		{
			WSADATA wsaData;
			int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
			if( iResult == NO_ERROR )
			{
				m_bInited = true;
			}
			return m_bInited;
		}
		~CInitSocket()
		{
			if (!m_bInited)
			{
				WSACleanup();
			}

		}
	private:
		bool m_bInited;
	};
	CInitSocket initSock;
	initSock.Init();

	timeval timeout = {5,0};
	fd_set fdset;
	bool bConnected = FALSE;
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	int iRet = -1;
	int iMode=1;//������ģʽ
	SOCKET iSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( iSockfd == INVALID_SOCKET )
	{
		/*int i = */WSAGetLastError();
		return FALSE;
	}
	if(setsockopt(iSockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen) == SOCKET_ERROR)
	{
		closesocket(iSockfd);
		return FALSE;
	}
	linger m_sLinger;
	m_sLinger.l_onoff  = 1;
	m_sLinger.l_linger = 0;//m_sLinger.l_linger=5;
	if (setsockopt(iSockfd, SOL_SOCKET, SO_LINGER, (char *)&m_sLinger, sizeof(linger)) == SOCKET_ERROR) //��ֹ����time_wait״̬
	{
		closesocket(iSockfd);
		return FALSE;
	}
	struct sockaddr_in remoteaddr;
	memset(&remoteaddr, 0, sizeof(remoteaddr));
	remoteaddr.sin_port = htons((u_short)nPort);
	remoteaddr.sin_addr.s_addr = inet_addr(szIP);
	remoteaddr.sin_family = AF_INET;
	//////////////////////////////////////////////////////////
	iRet = ioctlsocket(iSockfd, FIONBIO, (u_long FAR*) &iMode);//lint !e569
	if(iRet == SOCKET_ERROR)
	{
		closesocket(iSockfd);
		return FALSE;
	}
	iRet = connect(iSockfd,(LPSOCKADDR)&remoteaddr, sizeof(remoteaddr));
	if(iRet < 0)
	{
		FD_ZERO(&fdset);
		FD_SET((unsigned)iSockfd, &fdset);
		int iSelect = select((int)iSockfd + 1, 0, &fdset, 0, &timeout);//�ȴ����ӷ���
		if(iSelect > 0)
		{
			int iErr = -1;
			int iLen = sizeof(iErr);
			getsockopt(iSockfd, SOL_SOCKET, SO_ERROR, (char*)&iErr, &iLen);
			if(iErr == 0)
			{
				bConnected = TRUE;//���ӳɹ�
			}
		}
	}
	closesocket(iSockfd);
	return bConnected;
}

/**	@fn	    GetAllCollectDeviceReq
*	@brief	�����ȡ�ɼ��豸��Ϣ�б�
*	@param  [out] listCollectDevice -- �ɼ��豸��Ϣ�б�
*   @param  [in] bFinish -- �Ƿ����
*	@return	bool
*/
bool CAdjustTime::GetAllCollectDeviceReq(std::list<collect_info_t>& listCollectDevice)
{
	::ResetEvent(m_hGetCollectDeviceListEvent);
	if (GetDBDataMgrPtr()->GetAllCollectDeviceReq())
	{
		TPLOG_ERROR("failed to get Dvr Device");
		Msger_NotifyMsg(ALL_USER, _T("��ȡDVR�豸��Ϣʧ��"));
		return false;
	}
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_hGetCollectDeviceListEvent, 1000*10))
	{
		TPLOG_ERROR("get dvr device failed, overtime");
		Msger_NotifyMsg(ALL_USER, _T("��ȡDVR�豸��Ϣ��ʱ"));
		return false;
	}
	CGuard guard(&m_csListCollectDevice);
	std::list<collect_info_t>::iterator iterDvrInfo;
	for (iterDvrInfo = m_listCollectDevice.begin(); iterDvrInfo != m_listCollectDevice.end(); ++iterDvrInfo)
	{
		listCollectDevice.push_back(*iterDvrInfo);
	}
	return true;

}

/**	@fn	    GetAllCollectDeviceRsp
*	@brief	���DVR�豸��Ϣ�б�
*	@param  [in] deviceList -- DVR�豸��Ϣ�б�
*   @param  [in] bFinish -- �Ƿ����
*	@return	void
*/
void CAdjustTime::GetAllCollectDeviceRsp(const cms_8120::DeviceList& deviceList, BOOL bFinish /*= true*/)
{
	if (bFinish)
	{
		{
			CGuard guardRegion(&GetCAlarmProcPtr()->m_csRegion);
			for (int i = 0; i < deviceList.device_size(); ++i)
			{
				const cms_8120::Device& deviceInfo = deviceList.device(i);
				GetCAlarmProcPtr()->m_mapRegion[deviceInfo.strip()] = deviceInfo.nregionid();
			}
		}
		CGuard guard(&m_csListCollectDevice);
		m_listCollectDevice.clear();
		for (int i = 0; i < deviceList.device_size(); ++i)
		{
			const cms_8120::Device& deviceInfo = deviceList.device(i);
			//DVR�豸��Ϣת��
			collect_info_t stDvrInfo;
			ConvertCollectInfo(deviceInfo, stDvrInfo);
			m_listCollectDevice.push_back(stDvrInfo);
		}
	}
	else
	{
		TPLOG_INFO("GetAllCollectDeviceRsp....one time not send all device!");
	}
	::SetEvent(m_hGetCollectDeviceListEvent);
}

/**	@fn	    GetAllStorageDeviceReq
*	@brief	�����ȡ�洢�豸��Ϣ�б�
*	@param  [out] listStorageDevice -- �洢�豸��Ϣ�б�
*   @param  [in] bFinish -- �Ƿ����
*	@return	bool
*/
bool CAdjustTime::GetAllStorageDeviceReq(std::list<Struct_StorageDevice>& listStorageDevice)
{
	//��ȡ�洢�豸��Ϣ
	::ResetEvent(m_hGetStorageDeviceListEvent);
	if (GetDBDataMgrPtr()->GetAllStorageDeviceReq())
	{
		TPLOG_ERROR("failed to get Storage Device");
		Msger_NotifyMsg(ALL_USER, _T("��ȡ�洢�豸��Ϣʧ��"));
		return false;
	}
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_hGetStorageDeviceListEvent, 1000*10))
	{
		TPLOG_ERROR("get storage device failed, overtime");
		Msger_NotifyMsg(ALL_USER, _T("��ȡ�洢�豸��Ϣ��ʱ"));
		return false;
	}
	
	CGuard guard(&m_cslistStorageDevice);
	std::list<Struct_StorageDevice>::iterator iterStorageInfo;
	for (iterStorageInfo = m_listStorageDevice.begin(); iterStorageInfo != m_listStorageDevice.end(); ++iterStorageInfo)
	{
		listStorageDevice.push_back(*iterStorageInfo);
	}
	return true;

}

/**	@fn	    GetAllStorageDeviceRsp
*	@brief	��Ӵ洢�豸��Ϣ�б�
*	@param  [in] deviceList -- �洢�豸��Ϣ�б�
*   @param  [in] bFinish -- �Ƿ����
*	@return	void
*/
void CAdjustTime::GetAllStorageDeviceRsp(const cms_8120::DeviceList& deviceList, BOOL bFinish /*= true*/)
{
	if (bFinish)
	{
		{
			CGuard guardRegion(&GetCAlarmProcPtr()->m_csRegion);
			for (int i = 0; i < deviceList.device_size(); ++i)
			{
				const cms_8120::Device& deviceInfo = deviceList.device(i);
				GetCAlarmProcPtr()->m_mapRegion[deviceInfo.strip()] = deviceInfo.nregionid();
			}
		}
		CGuard guard(&m_cslistStorageDevice);
		m_listStorageDevice.clear();
		for (int i = 0; i < deviceList.device_size(); ++i)
		{
			const cms_8120::Device& deviceInfo = deviceList.device(i);
			//�洢�豸��Ϣת��
			Struct_StorageDevice stStorageInfo;
			ConvertStorageInfo(deviceInfo, stStorageInfo);
			m_listStorageDevice.push_back(stStorageInfo);
		}
	}
	else
	{
		TPLOG_INFO("GetAllStorageDeviceRsp....one time not send all device!");
	}
	::SetEvent(m_hGetStorageDeviceListEvent);
}

/**	@fn	    convertStorageInfo
*	@brief	ת���洢�豸��Ϣ
*	@param  [in] storageInfo -- ԭ�洢�豸��Ϣ
*   @param  [out] struStorage -- ת����Ĵ洢�豸��Ϣ
*	@return	BOOL
*/
BOOL CAdjustTime::ConvertStorageInfo(const cms_8120::Device &storageInfo, Struct_StorageDevice &struStorage)
{
	struStorage.nType = (ENUM_STORAGE_TYPE)storageInfo.ntype();
	struStorage.strName = CSCT::Utf82Ansi(storageInfo.strname());
	struStorage.strIp = storageInfo.strip();
	struStorage.nPort = storageInfo.nport();
	struStorage.strUser = CSCT::Utf82Ansi(storageInfo.struser());
	std::string strPwd = CSCT::Utf82Ansi(storageInfo.strpassword());
	Deciphering(strPwd);
	struStorage.strPwd = strPwd;
	if (STORAGE_TYPE_OBJECTCLOUD == struStorage.nType)
	{
		std::string strAccessKey = CSCT::Utf82Ansi(storageInfo.straccesskey());
		Deciphering(strAccessKey);
		struStorage.strAccessKey = strAccessKey;

		std::string strSecretKey = CSCT::Utf82Ansi(storageInfo.strsecretkey());
		Deciphering(strSecretKey);
		struStorage.strSecretKey = strSecretKey;
	}
	return true;
}

/**	@fn	    ConvertCollectInfo
*	@brief	ת���ɼ��豸��Ϣ
*	@param  [in] collectInfo -- ԭ�ɼ��豸��Ϣ
*   @param  [out]stCollectCfg -- ת����Ĳɼ��豸��Ϣ
*	@return	BOOL
*/
BOOL CAdjustTime::ConvertCollectInfo(const cms_8120::Device &collectInfo, collect_info_t &stCollectCfg)
{
	stCollectCfg.strDvrIp = collectInfo.strip();
	stCollectCfg.usPort = collectInfo.nport();
	stCollectCfg.strUserName = CSCT::Utf82Ansi(collectInfo.struser());
	std::string strPwd = CSCT::Utf82Ansi(collectInfo.strpassword());
	Deciphering(strPwd);
	stCollectCfg.strPassWd = strPwd;
	return true;
}

/** @fn     strTime
*  @brief  ʱ���ת��Ϊʱ���ַ���
*  @param  [in]t, ʱ���
*  @return ʱ���ַ���
*/
std::_tstring CAdjustTime::strTime(__time32_t const t)
{
	struct tm _tm;
	if (0 != _localtime32_s(&_tm, &t))
	{
		TPLOG_ERROR("bad time:%d", t);
		return _T("");
	}

	TCHAR szTime[32] = {0};
	_stprintf_s(szTime, _countof(szTime), _T("%4d-%02d-%02d %2d:%02d:%02d"),
		_tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
		_tm.tm_hour, _tm.tm_min, _tm.tm_sec);

	return szTime;    
}

/**	@fn	    Deciphering
*	@brief	���ܲ���
*	@param  [in] strPwd -- ��������
*	@param  [out] strPwd -- ��������
*	@return	void
*/
void CAdjustTime::Deciphering(std::string& strPwd)
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

/**	@fn     DelayBySleep
*   @brief  ͨ��ѭ��sleep�ķ�ʽ�ȴ�һ��ʱ��
*   @param  [in]dwMilliseconds, ��Ҫ�ȴ���ʱ�䣬��λ:����
*   @param  [in]stopFlag, ��־�����Ϊtrue��ȴ���dwMilliseconds���������ǰ����
*   @param  [in]dwInterval, �ȴ��������λ:����
*   @return	void
*/
void CAdjustTime::DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval/* = 500*/)
{
	if (0 == dwInterval)
	{
		dwInterval = 500;
	}
	DWORD const nTimes = dwMilliseconds / dwInterval;
	if(nTimes < 1)
	{
		Sleep(dwInterval);
	}
	else
	{
		for (DWORD i = 0; !stopFlag && i < nTimes; ++i)
		{
			Sleep(dwInterval);
		}
	}
}