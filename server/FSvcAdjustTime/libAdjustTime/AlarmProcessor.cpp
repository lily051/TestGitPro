#include "StdAfx.h"
#include <fclib/fcTchar.h>
#include <hak.h>
#include "../util/StringConverter.h"
#include "AlarmProcessor.h"
#include "hlogConfig.h"
#include "Msger.h"
#include <HPR_Netdb.h>
#include "Config.h"
#include "DBDataMgr.h"

namespace {
    const DWORD SAVE_ALARM_INTERVAL    = 2000; //��ӱ���ʱ����
    const DWORD PROCESS_ALARM_INTERVAL = 1000; //������ʱ����
    const DWORD NEW_ALARM = 0;
    const DWORD OLD_ALARM = 1;
    const DWORD CHECKTYPE_DVR = 100;
    const DWORD CHECKTYPE_COLLECT = 102;
    const DWORD CHECKTYPE_CENTER = 200;
	const DWORD ALARMLOG_UNRESOLVED = 0;
	const DWORD ALARMLOG_RESOLVED   = 1;

} //~ end of anonymous namespace

CAlarmProcessor::CAlarmProcessor()
: m_bExitFlag(false)
, m_hAlarmLogEvent(NULL)
{
	InitializeCriticalSection(&m_csRegion);
	InitializeCriticalSection(&m_csWorkState);
	InitializeCriticalSection(&m_csAlarmLog);
}

CAlarmProcessor::~CAlarmProcessor()
{
    try
    {
		DeleteCriticalSection(&m_csRegion);
		DeleteCriticalSection(&m_csWorkState);
        DeleteCriticalSection(&m_csAlarmLog);
    }
    catch (...)
    {

    }
}

/** @fn Init
 *  @brief  ��ʼ������
 *  @return ���� ADJUST_ERROR_SUCC
            �쳣 �� ADJUST_ERROR_SUCC
 */
HPR_INT32 CAlarmProcessor::Init()
{
	m_hAlarmLogEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hAlarmLogEvent)
	{
		TPLOG_ERROR("create CAlarmProcessor event failed:%d", ::GetLastError());
		return ADJUST_ERROR_FAIL;
	}
    return ADJUST_ERROR_SUCC;
}

/** @fn     Fini
*   @brief  ����ʼ������
*   @return ���� ADJUST_ERROR_SUCC
                �쳣 �� ADJUST_ERROR_SUCC
*/
HPR_INT32 CAlarmProcessor::Fini(void)
{
	if (m_hAlarmLogEvent != NULL)
	{
		CloseHandle(m_hAlarmLogEvent);
		m_hAlarmLogEvent = NULL;
	}
    return ADJUST_ERROR_SUCC;
}

/** @fn AddAlarmMsg
 *  @brief  ��ӱ�����Ϣ
 *  @param [in] pAlarm, ������Ϣָ�� 
 *  @return void
 */
void CAlarmProcessor::AddAlarmMsg(const pALARM_MSG pAlarm)
{
    if (NULL == pAlarm)
    {
        TPLOG_ERROR("���뱨����ϢΪ��");
        return;
    }

    work_state_t stWorkState;
    stWorkState.nKey            = pAlarm->dwAlarmType;
    stWorkState.nParentKey      = pAlarm->dwSvcType;
    stWorkState.nMsgType        = pAlarm->dwMsgType;
    stWorkState.nUserID         = pAlarm->dwUserID;//�û�
    stWorkState.nState          = pAlarm->dwStatus;
    stWorkState.strInfo         = fcA2T(pAlarm->szAlarmInfo);
    stWorkState.strHost         = fcA2T(pAlarm->szHost);
    stWorkState.nChannel        = pAlarm->dwChannel;//����ͨ��
    stWorkState.nRev            = pAlarm->dwRev;//���±�����������
    stWorkState.strOccureTime   = strTime(_time32(NULL)).c_str();
    stWorkState.nOccureTime     = _time32(NULL);
    stWorkState.strKeyValue     = GetSvcKeyValue(stWorkState);
    stWorkState.nAlarmID        = 0;
    stWorkState.nLastOccureTime = 0;

    m_msgWorkState.Push(stWorkState);
	//TPLOG_INFO("host:%s, alarm type:%d, module:%d, msg:%s",  CSCT::Tstring2Ansi(stWorkState.strHost).c_str(), stWorkState.nKey, stWorkState.nParentKey, CSCT::Tstring2Ansi(stWorkState.strInfo).c_str());
}

/** @fn SaveAlarmThreadProc
 *  @brief  ��ӱ���������
 *  @return HPR_INT32�� 0 �˳�������������
 */
HPR_INT32 CAlarmProcessor::SaveAlarmThreadProc()
{
    TPLOG_INFO("��������߳�����");
    Msger_NotifyMsg(ALL_USER,_T("��������߳�����"));
    do
    {
		CGuard guard(&m_csWorkState);
        work_state_t stWorkState;
        while (m_msgWorkState.Pop(stWorkState) && !m_bExitFlag)
        {
            //�Ǹ澯 ������
            if (MSG_TYPE_NOTIFY == stWorkState.nMsgType)
            {
                continue;
            }
            std::_tstring strKey = FormatKey(stWorkState);
            std::map<std::_tstring, work_state_t>::iterator itrWorkState = m_mapWorkState.find(strKey);
            if (itrWorkState == m_mapWorkState.end())//����������
            {
                //�±��� �������־
                stWorkState.nAlarmID = NEW_ALARM;
                stWorkState.nLastOccureTime = stWorkState.nOccureTime;
                stWorkState.nBeginOccureTime = stWorkState.nOccureTime;
				m_mapWorkState[strKey] = stWorkState;
            }
            else//�����Ѵ���
            {
				//�ɱ����������±���ʱ��
                itrWorkState->second.nOccureTime   = stWorkState.nOccureTime;
				itrWorkState->second.strOccureTime = stWorkState.strOccureTime;
            }
            
        }
    } while (DelayBySleep(SAVE_ALARM_INTERVAL, m_bExitFlag), !m_bExitFlag);

    TPLOG_INFO("��������߳���ֹ");
    Msger_NotifyMsg(ALL_USER,_T("��������߳���ֹ"));
    return ADJUST_ERROR_SUCC;
}

/** @fn AlarmDisposeProc
*  @brief  ������Ϣ������
*  @return HPR_INT32�� 0 �˳�������������
*/
HPR_INT32 CAlarmProcessor::AlarmDisposeProc()
{
    TPLOG_INFO("���������߳�����");
    Msger_NotifyMsg(ALL_USER,_T("���������߳�����"));
    do
    {
        CGuard guard(&m_csWorkState);
        if (!m_mapWorkState.empty())
        {
			std::map<std::_tstring, work_state_t>::iterator itrWorkState = m_mapWorkState.begin();
			for(; itrWorkState != m_mapWorkState.end() && !m_bExitFlag;)
			{

				switch (itrWorkState->second.nKey)
				{
					case WORK_TYPE_TRANSPORT_SVC_FAIL:
					case WORK_TYPE_GUARD_SVC_FAIL:
					case WORK_TYPE_ADJUST_SVC_FAIL:
					case WORK_TYPE_PATROL_SVC_FAIL:
					case WORK_TYPE_DVR_OFFLINE:
					case WORK_TYPE_DVR_TIME_ERROR:
					case WORK_TYPE_DVR_VI_LOST:
					case WORK_TYPE_DVR_HD_EXCEPTION:
					case WORK_TYPE_DVR_OPERATION_FAIL:
					case WORK_TYPE_DVR_NEARLLY_FULL:
					case WORK_TYPE_CVR_OFFLINE:
					case WORK_TYPE_CVR_CANNOT_LOGIN:
					case WORK_TYPE_CVR_NEARLLY_FULL:
					case WORK_TYPE_CVR_OPERATION_FAIL:
					case WORK_TYPE_KMS_OFFLINE:
					case WORK_TYPE_KMS_NEARLLY_FULL:
					case WORK_TYPE_KMS_OPERATION_FAIL:
					case WORK_TYPE_CLOUD_OFFLINE:
					case WORK_TYPE_CLOUD_CANNOT_LOGIN:
					case WORK_TYPE_CLOUD_GET_POOLLIST_FAIL:
					case WORK_TYPE_CLOUD_NEARLLY_FULL:
					case WORK_TYPE_CLOUD_OPERATION_FAIL:
					case WORK_TYPE_OBJECTCLOUD_OFFLINE:
					case WORK_TYPE_OBJECTCLOUD_NEARLLY_FULL:
					{
						//����60��û�н��յ�ͬһ���������ж�Ϊ���������Ѿ������
						int iDiffTime = 60;
						//�ɼ��豸���洢�豸Ѳ�� 10���Ӽ��һ�� ����Ϊ30������һ�α���
						if (WORK_TYPE_PATROL_SVC_FAIL             == itrWorkState->second.nKey
							|| WORK_TYPE_DVR_OFFLINE              == itrWorkState->second.nKey
							|| WORK_TYPE_DVR_NEARLLY_FULL         == itrWorkState->second.nKey
							|| WORK_TYPE_CVR_OFFLINE              == itrWorkState->second.nKey
							|| WORK_TYPE_CVR_CANNOT_LOGIN         == itrWorkState->second.nKey
							|| WORK_TYPE_CVR_NEARLLY_FULL         == itrWorkState->second.nKey
							|| WORK_TYPE_CVR_OPERATION_FAIL       == itrWorkState->second.nKey
							|| WORK_TYPE_CLOUD_OFFLINE            == itrWorkState->second.nKey
							|| WORK_TYPE_CLOUD_CANNOT_LOGIN       == itrWorkState->second.nKey
							|| WORK_TYPE_CLOUD_GET_POOLLIST_FAIL  == itrWorkState->second.nKey
							|| WORK_TYPE_CLOUD_NEARLLY_FULL       == itrWorkState->second.nKey
							|| WORK_TYPE_CLOUD_OPERATION_FAIL     == itrWorkState->second.nKey
							|| WORK_TYPE_KMS_OFFLINE              == itrWorkState->second.nKey
							|| WORK_TYPE_KMS_NEARLLY_FULL         == itrWorkState->second.nKey
							|| WORK_TYPE_KMS_OPERATION_FAIL       == itrWorkState->second.nKey
							|| WORK_TYPE_OBJECTCLOUD_OFFLINE      == itrWorkState->second.nKey
							|| WORK_TYPE_OBJECTCLOUD_NEARLLY_FULL == itrWorkState->second.nKey)
						{
							iDiffTime = 60 * 30;
						}
						if (abs((int)difftime(_time32(NULL), (time_t)itrWorkState->second.nOccureTime)) > iDiffTime)
						{
							itrWorkState = m_mapWorkState.erase(itrWorkState);
						}
						else
						{
							//�±����Ļ���¼����
							if(NEW_ALARM == itrWorkState->second.nAlarmID)
							{
								AddAlarmRegionId(itrWorkState->second);
								if (!GetDBDataMgrPtr()->AddCenterAlarmReq(itrWorkState->second, CHECKTYPE_CENTER))
								{
									itrWorkState->second.nAlarmID = OLD_ALARM;
									//TPLOG_INFO("add new alarm,host:%s, alarm type:%d, module:%d, msg:%s",  CSCT::Tstring2Ansi(itrWorkState->second.strHost).c_str(), itrWorkState->second.nKey, itrWorkState->second.nParentKey, CSCT::Tstring2Ansi(itrWorkState->second.strInfo).c_str());
								}
								else
								{
									TPLOG_ERROR("AddCenterAlarmReq error: %d, %d",
										itrWorkState->second.nKey,
										itrWorkState->second.nChannel);
								}
							}
							else 
							{
								Struct_AlarmLog stAlarmLog;
								if (GetAlarmLogReq(itrWorkState->second, stAlarmLog))
								{

									if (ALARMLOG_RESOLVED == stAlarmLog.nState)//�����Ѵ���
									{
										itrWorkState = m_mapWorkState.erase(itrWorkState);
										continue;
									}
									else if (ALARMLOG_UNRESOLVED == stAlarmLog.nState) //����δ����
									{
										if (itrWorkState->second.nOccureTime != itrWorkState->second.nLastOccureTime)
										{
											if (UpdateCenterAlarmReq(itrWorkState->second))
											{
												itrWorkState->second.nLastOccureTime = itrWorkState->second.nOccureTime;
											}
											else
											{
												TPLOG_ERROR("UpdateCenterAlarmReq error: %d, %d",
													itrWorkState->second.nKey,
													itrWorkState->second.nChannel);
											}
										}
										else
										{
											//do nothing
										}
									}
									else
									{
										TPLOG_ERROR("invalid alarm state : %d, %d, %d",
											itrWorkState->second.nKey,
											itrWorkState->second.nChannel,
											stAlarmLog.nState);
									}
								}
								else
								{
									TPLOG_ERROR("get alarm log error: %d, %d",
										itrWorkState->second.nKey,
										itrWorkState->second.nChannel);
								}
							}  
							++itrWorkState;
						}
					}
					break;
				default:
					{
						TPLOG_ERROR("error work state type:%d, module:%d, msg:%s", itrWorkState->second.nKey, itrWorkState->second.nParentKey, CSCT::Tstring2Ansi(itrWorkState->second.strInfo).c_str());
						itrWorkState = m_mapWorkState.erase(itrWorkState);
						break;
					}
				}//switch (itrWorkState->second.nKey)
			}//for(; itrWorkState != m_mapWorkState.end() && !m_bExitFlag;)
        }//if (!m_mapWorkState.empty())
    } while (DelayBySleep(PROCESS_ALARM_INTERVAL, m_bExitFlag), !m_bExitFlag);

    TPLOG_INFO("���������߳���ֹ");
    Msger_NotifyMsg(ALL_USER,_T("���������߳���ֹ"));

    return ADJUST_ERROR_SUCC;
}

/** @fn GetSvcKeyValue
 *  @brief  ��ȡ״̬��������
 *  @param [in] workstate, ����״̬�ṹ��
 *  @return std::_tstring��״̬��������
 */
std::_tstring CAlarmProcessor::GetSvcKeyValue(const work_state_t &workstate)
{
    std::_tstring strKeyValue = _T("");
    switch (workstate.nKey)
    {
    case WORK_TYPE_CENTTER_OFFLINE:
        strKeyValue = _T("���ķ�������");
        break;
	case WORK_TYPE_TRANSPORT_SVC_FAIL:
		strKeyValue = _T("�ϴ�����ʧ��");
		break;
    case WORK_TYPE_GUARD_SVC_FAIL:
        strKeyValue = _T("¼����������ʧ��");
        break;
	case WORK_TYPE_ADJUST_SVC_FAIL:
		strKeyValue = _T("Ѳ�����ʧ��");
		break;
	case WORK_TYPE_PATROL_SVC_FAIL:
		strKeyValue = _T("�ɼ��豸Ѳ��ʧ��");
		break;
	case WORK_TYPE_DVR_OFFLINE:
		strKeyValue = _T("�ɼ��豸������");
		break;
	case WORK_TYPE_DVR_TIME_ERROR:
		strKeyValue = _T("�ɼ��豸ʱ��ͱ���ʱ���ܴ�");
		break;
	case WORK_TYPE_DVR_VI_LOST:
		strKeyValue = _T("�ɼ��豸��Ƶ�źŶ�ʧ");
		break;
	case WORK_TYPE_DVR_HD_EXCEPTION:
		strKeyValue = _T("�ɼ��豸Ӳ���쳣");
		break;
	case WORK_TYPE_DVR_OPERATION_FAIL:
        strKeyValue = _T("�ɼ��豸ĳЩ����ʧ��");
		break;
	case WORK_TYPE_DVR_NEARLLY_FULL:
		strKeyValue = _T("�ɼ��豸������");
		break;
	case WORK_TYPE_CVR_OFFLINE:
		strKeyValue = _T("CVR������");
		break;
    case WORK_TYPE_CVR_CANNOT_LOGIN:
        strKeyValue = _T("CVR��¼ʧ��");
        break;
	case WORK_TYPE_CVR_NEARLLY_FULL:
		strKeyValue = _T("CVR������");
		break;
    case WORK_TYPE_CVR_OPERATION_FAIL:
        strKeyValue = _T("CVRĳЩ����ʧ��");
        break;
	case WORK_TYPE_CLOUD_OFFLINE:
		strKeyValue = _T("Cloud������");
		break;
	case WORK_TYPE_CLOUD_CANNOT_LOGIN:
		strKeyValue = _T("Cloud��¼ʧ��");
		break;
	case WORK_TYPE_CLOUD_NEARLLY_FULL:
		strKeyValue = _T("Cloud������");
		break;
	case WORK_TYPE_CLOUD_OPERATION_FAIL:
		strKeyValue = _T("CloudĳЩ����ʧ��");
		break;
    case WORK_TYPE_ADJUST_SVC_OFFLINE:
        strKeyValue = _T("ϵͳѲ���������");
        break;
    case WORK_TYPE_KMS_OFFLINE:
        strKeyValue = _T("KMS������");
        break;
	case WORK_TYPE_KMS_NEARLLY_FULL:
		strKeyValue = _T("KMS������");
		break;
	case WORK_TYPE_KMS_OPERATION_FAIL:
		strKeyValue = _T("KMSĳЩ����ʧ��");
		break;
    default:
        strKeyValue = _T("δ֪״̬");
        break;
    }

    return strKeyValue;
}

/** @fn FormatKey
 *  @brief  ���ɹ���״̬key
 *  @param [in] workstate, ����״̬�ṹ��
 *  @return std::_tstring��key�ַ���
 */
std::_tstring CAlarmProcessor::FormatKey(const work_state_t &workstate)
{
    TCHAR buff[512] = {0};
    _stprintf_s(buff, 512, _T("%d_%d_%d_%d_%s_%s"),
        workstate.nKey,
        workstate.nUserID,
        workstate.nChannel,
        workstate.nRev,
        workstate.strHost.c_str(),
        workstate.strInfo.c_str());
    return std::_tstring(buff);
}

/** @fn     DelayBySleep
*   @brief  ͨ��ѭ��sleep�ķ�ʽ�ȴ�һ��ʱ��
*   @param  [in]dwMilliseconds, ��Ҫ�ȴ���ʱ�䣬��λ����
*   @param  [in]stopFlag, ��־�����Ϊtrue��ȴ���dwMilliseconds��
*                        �������ǰ����
*   @param  [in]dwInterval, �ȴ��������λ����
*   @return	void
*/
void CAlarmProcessor::DelayBySleep(DWORD dwMilliseconds,
                                volatile bool &stopFlag,
                                DWORD dwInterval/* = 500*/)
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

/** @fn     strTime
*  @brief  ʱ��ת��Ϊ�ַ���
*  @param  [in]t, ʱ����Ϣ
*  @return ʱ���ַ���
*/
std::_tstring CAlarmProcessor::strTime(__time32_t const t)
{
    struct tm _tm;
    if (0 != _localtime32_s(&_tm, &t))
    {
        //TPLOG_ERROR("bad time:%d", t);
        return _T("");
    }

    TCHAR szTime[32] = {0};
    _stprintf_s(szTime, _countof(szTime), _T("%4d-%02d-%02d %2d:%02d:%02d"),
        _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
        _tm.tm_hour, _tm.tm_min, _tm.tm_sec);

    return (std::_tstring)szTime;
}

/**	@fn	    GetAlarmLogReq
*	@brief	��ñ�����־
*   @param  [in]workstate, workstate��Ϣ
*   @param  [out]stAlarmLog, ������־��Ϣ
*   @return bool
*/
bool CAlarmProcessor::GetAlarmLogReq(const work_state_t &workstate, Struct_AlarmLog &stAlarmLog)
{
	::ResetEvent(m_hAlarmLogEvent);
	if (GetDBDataMgrPtr()->GetAlarmLogReq(workstate))
	{
		TPLOG_ERROR("failed to get alarm log:%s", CSCT::Tstring2Ansi(workstate.strInfo).c_str());
		return false;
	}
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_hAlarmLogEvent, 1000*10))
	{
		TPLOG_ERROR("failed to get alarm log,overtime:%s", CSCT::Tstring2Ansi(workstate.strInfo).c_str());
		return false;
	}
	CGuard guard(&m_csAlarmLog);
	stAlarmLog = m_stAlarmLog;
	return true;

}


/**	@fn	    GetAlarmLogRsp
*	@brief	��ñ�����־
*	@param  [in] alarmlogInfo -- ������־��Ϣ
*   @param  [in] bFinish -- �Ƿ����
*	@return	
*/
void CAlarmProcessor::GetAlarmLogRsp(const cms_8120::AlarmlogInfo& alarmlogInfo, BOOL bFinish /*= true*/)
{
	if (bFinish)
	{
		if(alarmlogInfo.has_alarmlog())
		{
			const cms_8120::Alarmlog& alarmlog = alarmlogInfo.alarmlog();
			CGuard guard(&m_csAlarmLog);
			m_stAlarmLog.nID = -1;
			m_stAlarmLog.nState = -1;
			m_stAlarmLog.strHost = "";
			convertAlarmLog(alarmlog, m_stAlarmLog);
		}
		else
		{
			TPLOG_ERROR("get alarm log failure!");
		}
	}
	else
	{
		TPLOG_INFO("GetAlarmLogRsp....one time not send all alarmlog!");
	}
	::SetEvent(m_hAlarmLogEvent);
}

/** @fn    UpdateCenterAlarmReq
*  @brief  ���ı�����Ϣ���±���
*  @param  [in]workstate, workstate��Ϣ
*  @return bool
*/
bool CAlarmProcessor::UpdateCenterAlarmReq(const work_state_t &workstate)
{
	Struct_AlarmLog stAlarmLog;
	if(!GetAlarmLogReq(workstate, stAlarmLog))
	{
		TPLOG_ERROR("GetAlarmLogReq error: %d, %d",
			workstate.nKey,
			workstate.nChannel);
		return false;
	}
    if (-1 == stAlarmLog.nID)
	{
		TPLOG_ERROR("UpdateCenterAlarmReq: %d, %d have no recode",
			workstate.nKey,
			workstate.nChannel);
		return false;
    }
	else
	{
		if(GetDBDataMgrPtr()->UpdateCenterAlarmReq(workstate, stAlarmLog.nID))
		{
			TPLOG_ERROR("UpdateCenterAlarmReq error: %d, %d",
				workstate.nKey,
				workstate.nChannel);
			return false;
		}
	}

	return true;
}

/**	@fn	    convertAlarmLog
*	@brief	ת��������־
*	@param  [in]  alarmlogInfo -- ԭ������־
*   @param  [out] stAlarmLog -- ת����ı�����־
*	@return	BOOL
*/
BOOL CAlarmProcessor::convertAlarmLog(const cms_8120::Alarmlog &alarmlogInfo, Struct_AlarmLog &stAlarmLog)
{
	stAlarmLog.nID     = alarmlogInfo.id();
	stAlarmLog.nState  = alarmlogInfo.status();
	stAlarmLog.strHost = CSCT::Utf82Ansi(alarmlogInfo.strhost());
	return true;
}

/**	@fn	    AddAlarmRegionId
*	@brief	���ӱ�������ID
*	@param  [in]  work_state_t -- ������Ϣ
*   @param  [out] work_state_t -- ��������ID
*	@return	void
*/
void CAlarmProcessor::AddAlarmRegionId(work_state_t& workstate)
{
	int nTempRegionID = 1;
	{
		CGuard guard(&m_csRegion);
		std::map<std::string, int>::iterator iterRegion = m_mapRegion.find(CSCT::Tstring2Ansi(workstate.strHost));
		if (iterRegion != m_mapRegion.end())
		{
			nTempRegionID = iterRegion->second;
		}
	}
	workstate.nRegionID = nTempRegionID;
}