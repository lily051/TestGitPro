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
    const DWORD SAVE_ALARM_INTERVAL    = 2000; //添加报警时间间隔
    const DWORD PROCESS_ALARM_INTERVAL = 1000; //处理报警时间间隔
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
 *  @brief  初始化函数
 *  @return 正常 ADJUST_ERROR_SUCC
            异常 非 ADJUST_ERROR_SUCC
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
*   @brief  反初始化函数
*   @return 正常 ADJUST_ERROR_SUCC
                异常 非 ADJUST_ERROR_SUCC
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
 *  @brief  添加报警消息
 *  @param [in] pAlarm, 报警消息指针 
 *  @return void
 */
void CAlarmProcessor::AddAlarmMsg(const pALARM_MSG pAlarm)
{
    if (NULL == pAlarm)
    {
        TPLOG_ERROR("输入报警信息为空");
        return;
    }

    work_state_t stWorkState;
    stWorkState.nKey            = pAlarm->dwAlarmType;
    stWorkState.nParentKey      = pAlarm->dwSvcType;
    stWorkState.nMsgType        = pAlarm->dwMsgType;
    stWorkState.nUserID         = pAlarm->dwUserID;//用户
    stWorkState.nState          = pAlarm->dwStatus;
    stWorkState.strInfo         = fcA2T(pAlarm->szAlarmInfo);
    stWorkState.strHost         = fcA2T(pAlarm->szHost);
    stWorkState.nChannel        = pAlarm->dwChannel;//报警通道
    stWorkState.nRev            = pAlarm->dwRev;//导致报警操作类型
    stWorkState.strOccureTime   = strTime(_time32(NULL)).c_str();
    stWorkState.nOccureTime     = _time32(NULL);
    stWorkState.strKeyValue     = GetSvcKeyValue(stWorkState);
    stWorkState.nAlarmID        = 0;
    stWorkState.nLastOccureTime = 0;

    m_msgWorkState.Push(stWorkState);
	//TPLOG_INFO("host:%s, alarm type:%d, module:%d, msg:%s",  CSCT::Tstring2Ansi(stWorkState.strHost).c_str(), stWorkState.nKey, stWorkState.nParentKey, CSCT::Tstring2Ansi(stWorkState.strInfo).c_str());
}

/** @fn SaveAlarmThreadProc
 *  @brief  添加报警处理函数
 *  @return HPR_INT32， 0 退出，其它无意义
 */
HPR_INT32 CAlarmProcessor::SaveAlarmThreadProc()
{
    TPLOG_INFO("报警添加线程启动");
    Msger_NotifyMsg(ALL_USER,_T("报警添加线程启动"));
    do
    {
		CGuard guard(&m_csWorkState);
        work_state_t stWorkState;
        while (m_msgWorkState.Pop(stWorkState) && !m_bExitFlag)
        {
            //非告警 不处理
            if (MSG_TYPE_NOTIFY == stWorkState.nMsgType)
            {
                continue;
            }
            std::_tstring strKey = FormatKey(stWorkState);
            std::map<std::_tstring, work_state_t>::iterator itrWorkState = m_mapWorkState.find(strKey);
            if (itrWorkState == m_mapWorkState.end())//报警不存在
            {
                //新报警 需添加日志
                stWorkState.nAlarmID = NEW_ALARM;
                stWorkState.nLastOccureTime = stWorkState.nOccureTime;
                stWorkState.nBeginOccureTime = stWorkState.nOccureTime;
				m_mapWorkState[strKey] = stWorkState;
            }
            else//报警已存在
            {
				//旧报警更新最新报警时间
                itrWorkState->second.nOccureTime   = stWorkState.nOccureTime;
				itrWorkState->second.strOccureTime = stWorkState.strOccureTime;
            }
            
        }
    } while (DelayBySleep(SAVE_ALARM_INTERVAL, m_bExitFlag), !m_bExitFlag);

    TPLOG_INFO("报警添加线程终止");
    Msger_NotifyMsg(ALL_USER,_T("报警添加线程终止"));
    return ADJUST_ERROR_SUCC;
}

/** @fn AlarmDisposeProc
*  @brief  报警消息处理函数
*  @return HPR_INT32， 0 退出，其它无意义
*/
HPR_INT32 CAlarmProcessor::AlarmDisposeProc()
{
    TPLOG_INFO("报警处理线程启动");
    Msger_NotifyMsg(ALL_USER,_T("报警处理线程启动"));
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
						//超过60秒没有接收到同一条报警，判断为这条报警已经解除了
						int iDiffTime = 60;
						//采集设备及存储设备巡检 10分钟检测一次 暂设为30分钟清一次报警
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
							//新报警的话记录报警
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

									if (ALARMLOG_RESOLVED == stAlarmLog.nState)//报警已处理
									{
										itrWorkState = m_mapWorkState.erase(itrWorkState);
										continue;
									}
									else if (ALARMLOG_UNRESOLVED == stAlarmLog.nState) //报警未处理
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

    TPLOG_INFO("报警处理线程终止");
    Msger_NotifyMsg(ALL_USER,_T("报警处理线程终止"));

    return ADJUST_ERROR_SUCC;
}

/** @fn GetSvcKeyValue
 *  @brief  获取状态类型名称
 *  @param [in] workstate, 工作状态结构体
 *  @return std::_tstring，状态类型名称
 */
std::_tstring CAlarmProcessor::GetSvcKeyValue(const work_state_t &workstate)
{
    std::_tstring strKeyValue = _T("");
    switch (workstate.nKey)
    {
    case WORK_TYPE_CENTTER_OFFLINE:
        strKeyValue = _T("中心服务不在线");
        break;
	case WORK_TYPE_TRANSPORT_SVC_FAIL:
		strKeyValue = _T("上传服务失败");
		break;
    case WORK_TYPE_GUARD_SVC_FAIL:
        strKeyValue = _T("录像守卫服务失败");
        break;
	case WORK_TYPE_ADJUST_SVC_FAIL:
		strKeyValue = _T("巡检服务失败");
		break;
	case WORK_TYPE_PATROL_SVC_FAIL:
		strKeyValue = _T("采集设备巡检失败");
		break;
	case WORK_TYPE_DVR_OFFLINE:
		strKeyValue = _T("采集设备不在线");
		break;
	case WORK_TYPE_DVR_TIME_ERROR:
		strKeyValue = _T("采集设备时间和本地时间差很大");
		break;
	case WORK_TYPE_DVR_VI_LOST:
		strKeyValue = _T("采集设备视频信号丢失");
		break;
	case WORK_TYPE_DVR_HD_EXCEPTION:
		strKeyValue = _T("采集设备硬盘异常");
		break;
	case WORK_TYPE_DVR_OPERATION_FAIL:
        strKeyValue = _T("采集设备某些操作失败");
		break;
	case WORK_TYPE_DVR_NEARLLY_FULL:
		strKeyValue = _T("采集设备磁盘满");
		break;
	case WORK_TYPE_CVR_OFFLINE:
		strKeyValue = _T("CVR不在线");
		break;
    case WORK_TYPE_CVR_CANNOT_LOGIN:
        strKeyValue = _T("CVR登录失败");
        break;
	case WORK_TYPE_CVR_NEARLLY_FULL:
		strKeyValue = _T("CVR磁盘满");
		break;
    case WORK_TYPE_CVR_OPERATION_FAIL:
        strKeyValue = _T("CVR某些操作失败");
        break;
	case WORK_TYPE_CLOUD_OFFLINE:
		strKeyValue = _T("Cloud不在线");
		break;
	case WORK_TYPE_CLOUD_CANNOT_LOGIN:
		strKeyValue = _T("Cloud登录失败");
		break;
	case WORK_TYPE_CLOUD_NEARLLY_FULL:
		strKeyValue = _T("Cloud磁盘满");
		break;
	case WORK_TYPE_CLOUD_OPERATION_FAIL:
		strKeyValue = _T("Cloud某些操作失败");
		break;
    case WORK_TYPE_ADJUST_SVC_OFFLINE:
        strKeyValue = _T("系统巡检服务不在线");
        break;
    case WORK_TYPE_KMS_OFFLINE:
        strKeyValue = _T("KMS不在线");
        break;
	case WORK_TYPE_KMS_NEARLLY_FULL:
		strKeyValue = _T("KMS磁盘满");
		break;
	case WORK_TYPE_KMS_OPERATION_FAIL:
		strKeyValue = _T("KMS某些操作失败");
		break;
    default:
        strKeyValue = _T("未知状态");
        break;
    }

    return strKeyValue;
}

/** @fn FormatKey
 *  @brief  生成工作状态key
 *  @param [in] workstate, 工作状态结构体
 *  @return std::_tstring，key字符串
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
*   @brief  通过循环sleep的方式等待一段时间
*   @param  [in]dwMilliseconds, 需要等待的时间，单位毫秒
*   @param  [in]stopFlag, 标志，如果为true则等待完dwMilliseconds，
*                        否则会提前返回
*   @param  [in]dwInterval, 等待间隔，单位毫秒
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
*  @brief  时间转换为字符串
*  @param  [in]t, 时间信息
*  @return 时间字符串
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
*	@brief	获得报警日志
*   @param  [in]workstate, workstate信息
*   @param  [out]stAlarmLog, 报警日志信息
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
*	@brief	获得报警日志
*	@param  [in] alarmlogInfo -- 报警日志信息
*   @param  [in] bFinish -- 是否结束
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
*  @brief  中心报警信息更新报警
*  @param  [in]workstate, workstate信息
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
*	@brief	转换报警日志
*	@param  [in]  alarmlogInfo -- 原报警日志
*   @param  [out] stAlarmLog -- 转换后的报警日志
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
*	@brief	增加报警区域ID
*	@param  [in]  work_state_t -- 报警信息
*   @param  [out] work_state_t -- 报警区域ID
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