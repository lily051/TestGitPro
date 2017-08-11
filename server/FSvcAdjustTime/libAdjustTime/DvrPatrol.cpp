/**	@file    DvrPatrol.cpp 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   dvr设备巡检处理 类
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#include "StdAfx.h" 
#include <fclib/fcTchar.h>
#include "hlogConfig.h"
#include <HCNetSDK.h>
#include <SvcGeneralDef.h>
#include "DvrPatrol.h"
#include "Msger.h"
#include "DBDataMgr.h"
#include "type/DevType.pb.h"
#include "Config.h"

namespace {
	typedef enum _Exception_Type
	{
		EXCEPTION_HD_FULL,
		EXCEPTION_HD_ERROR,
		EXCEPTION_NET_BREAK,
		EXCEPTION_IP_CONFLICT,
		EXCEPTION_INVALID_ACCESS,
		EXCEPTION_VIDEO_FORMAT_UNMATCH,
		EXCEPTION_VIDEO_SIGNAL_EXCEPTION,
		EXCEPTION_VIDEO_RECORD_EXCEPTION,
	}Exception_Type;
} //~ end of anonymous namespace

//报警消息回调函数
void CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer,
                              char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
	if(NULL == pAlarmer || NULL == pAlarmInfo)
	{
		TPLOG_ERROR("无效参数");
		return;
	}
    NET_DVR_ALARMINFO_V30 struAlarmInfo;
    memcpy(&struAlarmInfo, pAlarmInfo, sizeof(NET_DVR_ALARMINFO_V30));
    switch(lCommand)
    {
    case COMM_ALARM_V30:
        {
            switch (struAlarmInfo.dwAlarmType)
            {
            case 1: //硬盘满
            case 4: //硬盘未格式化
            case 5: //读写硬盘出错
                {
                    for(int i = 0; i < MAX_DISKNUM_V30; i++)
                    {
                        //找到异常的硬盘
                        if(1 == struAlarmInfo.byDiskNumber[i])
                        {
                            std::string strDvrIp = pAlarmer->sDeviceIP;
                            TPLOG_ERROR("采集设备:%s 硬盘:%d异常,错误类型(%d)",pAlarmer->sDeviceIP,i+1,struAlarmInfo.dwAlarmType);
                            Msger_DeviceAlarmMsg(WORK_TYPE_DVR_HD_EXCEPTION,
                                ALL_USER,
                                i+1,
                                ALARM_OCCUR,
                                TP_ALARM_DVR,
                                strDvrIp.c_str(),
                                strDvrIp.size(),
                                _T("采集设备:%s 硬盘:%d异常,错误类型(%d)"),
                                fcA2T(pAlarmer->sDeviceIP),
                                i+1,
                                struAlarmInfo.dwAlarmType);
                        }
                    }
                }
                break;
            case 2: //视频信号丢失
                {
                    for(int i = 0; i < MAX_CHANNUM_V30; i++)
                    {
                        //找到异常的通道
                        if(1 == struAlarmInfo.byChannel[i])
                        {
                            std::string strDvrIp = pAlarmer->sDeviceIP;
                            TPLOG_ERROR("采集设备: %s 通道:%d 视频信号丢失",pAlarmer->sDeviceIP,i+1);
                            Msger_DeviceAlarmMsg(WORK_TYPE_DVR_VI_LOST,
                                ALL_USER,
                                i+1,
                                ALARM_OCCUR,
                                TP_ALARM_DVR,
                                strDvrIp.c_str(),
                                strDvrIp.size(),
                                _T("采集设备:%s 通道:%d 视频信号丢失"),
                                fcA2T(pAlarmer->sDeviceIP),
                                i+1);
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }
}

CDvrPatrol::CDvrPatrol()
{

}

CDvrPatrol::~CDvrPatrol()
{

}

/** @fn     ChkDvrOnLine
*  @brief  检查dvr是否在线
*  @param  [in]stDvrInfo, dvr信息
*  @return bool
*/
bool CDvrPatrol::ChkDvrOnLine(const collect_info_t &stDvrInfo)
{
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
	memcpy_s(struLoginInfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, stDvrInfo.strDvrIp.c_str(), strlen(stDvrInfo.strDvrIp.c_str()));
	struLoginInfo.wPort = stDvrInfo.usPort;
	memcpy_s(struLoginInfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, stDvrInfo.strUserName.c_str(), strlen(stDvrInfo.strUserName.c_str()));
	memcpy_s(struLoginInfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, stDvrInfo.strPassWd.c_str(), strlen(stDvrInfo.strPassWd.c_str()));
	struLoginInfo.cbLoginResult = NULL;
	struLoginInfo.pUser = NULL;
	struLoginInfo.bUseAsynLogin = 0;
	LONG lLoginID = DEVICE_LOGIN_FAILED;
	lLoginID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
	if (DEVICE_LOGIN_FAILED == lLoginID)
	{

        DWORD ulErrorCode = NET_DVR_GetLastError();
        TPLOG_ERROR("login dvr %s:%d:%s failed(%d)",
            (LPCSTR)(stDvrInfo.strDvrIp.c_str()),
            stDvrInfo.usPort,
            (LPCSTR)(stDvrInfo.strUserName.c_str()),
            ulErrorCode);

        if(NET_DVR_NETWORK_FAIL_CONNECT == ulErrorCode)
        {
			TPLOG_ERROR("dvr offline %s:%d:%s",
				(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
				stDvrInfo.usPort,
				(LPCSTR)(stDvrInfo.strUserName.c_str()));
            Msger_DeviceAlarmMsg(WORK_TYPE_DVR_OFFLINE,
                ALL_USER,
                ALL_CHANNEL,
                ALARM_OCCUR,
                TP_ALARM_DVR,
                stDvrInfo.strDvrIp.c_str(),
                stDvrInfo.strDvrIp.size(),
				_T("采集设备:%s:%d 不在线"),
                fcA2T(stDvrInfo.strDvrIp.c_str()), stDvrInfo.usPort);
			if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_DVR, stDvrInfo.strDvrIp, stDvrInfo.usPort, OFFLINE))
			{
				TPLOG_ERROR("update dvr:%s offline state failed",
					(LPCSTR)(stDvrInfo.strDvrIp.c_str()));
			}
        }
        else
        {
			TPLOG_ERROR("check dvr online failure %s:%d:%s failed(%d)",
				(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
				stDvrInfo.usPort,
				(LPCSTR)(stDvrInfo.strUserName.c_str()),
				ulErrorCode);
			Msger_NotifyMsg(ALL_USER, _T("检查采集设备是否在线时登陆采集设备:%s,%d,%s 失败, 错误码(%d)"),
				fcA2T(stDvrInfo.strDvrIp.c_str()),
				(int)(stDvrInfo.usPort),
				fcA2T(stDvrInfo.strUserName.c_str()),
				NET_DVR_GetLastError());
        }

        return false;
    }
	if (GetDBDataMgrPtr()->UpdateDeviceOnlineStateReq(cms_8120::DEV_TYPE_DVR, stDvrInfo.strDvrIp, stDvrInfo.usPort, ONLINE))
	{
		TPLOG_ERROR("update dvr:%s online state failed",
			(LPCSTR)(stDvrInfo.strDvrIp.c_str()));
	}
	if (0 == GetConfigPtr()->m_nHaveCenterStorage) //无中心存储要检测DVR容量，容量不足进行报警
	{
		CheckDvrDisk(stDvrInfo, lLoginID);
	}

    //登出dvr
    NET_DVR_Logout(lLoginID);

    return true;
}

/** @fn     CheckDvrDisk
*  @brief  检查dvr硬盘容量是否满
*  @param  [in]lUserId登录句柄
*  @return bool
*/
bool CDvrPatrol::CheckDvrDisk(const collect_info_t &stDvrInfo, long lUserId)
{
	DWORD   dwReturned = 0;
	NET_DVR_HDCFG stDvrHDCfg;
	if (!NET_DVR_GetDVRConfig(lUserId, NET_DVR_GET_HDCFG, 0,
		&stDvrHDCfg, sizeof(NET_DVR_HDCFG), &dwReturned))
	{
		DWORD ulErrorCode = NET_DVR_GetLastError();
		TPLOG_ERROR("获取 dvr %s:%d:%s 磁盘信息失败(%d)",
			(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
			stDvrInfo.usPort,
			(LPCSTR)(stDvrInfo.strUserName.c_str()),
			ulErrorCode);
		Msger_NotifyMsg(ALL_USER, _T("设备:%s 获取磁盘信息失败(错误码%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			ulErrorCode);
		return false;
	}

	//硬盘总容量 单位MB
	unsigned int uiTotalSpace = 0;
	//硬盘剩余容量 单位MB
	unsigned int uiFreeSpace = 0;
	//硬盘剩余容量 单位GB
	unsigned int uiFreeSpaceGB = 0;
	//硬盘剩余容量百分比
	double dFreeSpacePercent = 0.0;
	for (DWORD i = 0; i < stDvrHDCfg.dwHDCount; ++i)
	{
		//硬盘状态HD_STAT 0-正常
		if (HD_STAT_OK == stDvrHDCfg.struHDInfo[i].dwHdStatus)
		{
			//硬盘容量
			uiTotalSpace += stDvrHDCfg.struHDInfo[i].dwCapacity;
			//硬盘剩余空间
			uiFreeSpace += stDvrHDCfg.struHDInfo[i].dwFreeSpace;
		}
	}
	if (uiTotalSpace == 0)
	{
		TPLOG_ERROR("dvr %s 磁盘总容量为0",(LPCSTR)(stDvrInfo.strDvrIp.c_str()));
		return false;
	}

	uiFreeSpaceGB     = uiFreeSpace / 1024;
	dFreeSpacePercent = (double)(((double)uiFreeSpace / (double)uiTotalSpace) * 100);
	//小于100G
	if (uiFreeSpaceGB < 100)
	{
		TPLOG_ERROR("dvr %s 磁盘容量不足，剩余%dGB",
			(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
			uiFreeSpaceGB);
		Msger_DeviceAlarmMsg(WORK_TYPE_DVR_NEARLLY_FULL,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_DVR,
			stDvrInfo.strDvrIp.c_str(),
			stDvrInfo.strDvrIp.size(),
			_T("采集设备磁盘快满了(设备ip:%s, 剩余%dGB)"), 
			fcA2T(stDvrInfo.strDvrIp.c_str()), 
			uiFreeSpaceGB);
		return true;
	}

	//小于5%
	if (dFreeSpacePercent < 5)
	{
		TPLOG_ERROR("dvr %s 磁盘剩余容量占比不足，剩余%d%%",
			(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
			dFreeSpacePercent);
		Msger_DeviceAlarmMsg(WORK_TYPE_DVR_NEARLLY_FULL,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_DVR,
			stDvrInfo.strDvrIp.c_str(),
			stDvrInfo.strDvrIp.size(),
			_T("采集设备磁盘快满了(设备ip:%s, 剩余%d%%)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			(int)dFreeSpacePercent);
		return true;
	}

	return true;
}

/** @fn     StartDvrAlarmListen
*  @brief  开始dvr报警布防
*  @param  [in]stDvrInfo, dvr信息
*  @return bool
*/
bool CDvrPatrol::StartDvrAlarmListen(const collect_info_t &stDvrInfo)
{

    //判断 DVR是否已经在报警侦听 有的话直接返回。
    std::map<std::string, unsigned long>::iterator itor = m_mapAlarmListen.find(stDvrInfo.strDvrIp.c_str());
    if(itor != m_mapAlarmListen.end())
    {
        /*TPLOG_INFO("dvr %s is Already Setup Alarm",
            (LPCSTR)(stDvrInfo.strDvrIp.c_str()));*/
        return true;
    }

	NET_DVR_USER_LOGIN_INFO *pStruLoginInfo = new(std::nothrow) NET_DVR_USER_LOGIN_INFO;
	if (pStruLoginInfo == NULL)
	{
		TPLOG_ERROR("new NET_DVR_USER_LOGIN_INFO fail");
		return FALSE;
	}
	memset(pStruLoginInfo, 0, sizeof(NET_DVR_USER_LOGIN_INFO));

	NET_DVR_DEVICEINFO_V40 *pStruDeviceInfo = new(std::nothrow) NET_DVR_DEVICEINFO_V40;
	if (pStruDeviceInfo == NULL)
	{
		TPLOG_ERROR("new NET_DVR_DEVICEINFO_V40 fail");
		return FALSE;
	}
	memset(pStruDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V40));

	memcpy_s(pStruLoginInfo->sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, stDvrInfo.strDvrIp.c_str(), strlen(stDvrInfo.strDvrIp.c_str()));
	pStruLoginInfo->wPort = stDvrInfo.usPort;
	memcpy_s(pStruLoginInfo->sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, stDvrInfo.strUserName.c_str(), strlen(stDvrInfo.strUserName.c_str()));
	memcpy_s(pStruLoginInfo->sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, stDvrInfo.strPassWd.c_str(), strlen(stDvrInfo.strPassWd.c_str()));
	pStruLoginInfo->cbLoginResult = NULL;
	pStruLoginInfo->pUser = NULL;
	pStruLoginInfo->bUseAsynLogin = 0;
	LONG lLoginID = DEVICE_LOGIN_FAILED;
	lLoginID = NET_DVR_Login_V40(pStruLoginInfo, pStruDeviceInfo);
	if (DEVICE_LOGIN_FAILED == lLoginID)
	{
        TPLOG_ERROR("login dvr %s:%d:%s failed(%d)",
            (LPCSTR)(stDvrInfo.strDvrIp.c_str()),
            stDvrInfo.usPort,
            (LPCSTR)(stDvrInfo.strUserName.c_str()),
            NET_DVR_GetLastError());
		Msger_NotifyMsg(ALL_USER, _T("登陆采集设备:%s,%d,%s 失败, 错误码(%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			(int)(stDvrInfo.usPort),
			fcA2T(stDvrInfo.strUserName.c_str()),
			NET_DVR_GetLastError());
		delete pStruLoginInfo;
		delete pStruDeviceInfo;
        return false;
    }
	delete pStruLoginInfo;
	delete pStruDeviceInfo;

	//配置上报中心报警类型
	DWORD dwReturned;
	NET_DVR_EXCEPTION_V40 *pStruExceptionInfo = new(std::nothrow) NET_DVR_EXCEPTION_V40;
	if (pStruExceptionInfo == NULL)
	{
		TPLOG_ERROR("new NET_DVR_EXCEPTION_V40 fail");
		NET_DVR_Logout(lLoginID);
		return false;
	}
	memset(pStruExceptionInfo, 0, sizeof(NET_DVR_EXCEPTION_V40));
	if (!NET_DVR_GetDVRConfig(lLoginID, NET_DVR_GET_EXCEPTIONCFG_V40, 0, pStruExceptionInfo, sizeof(NET_DVR_EXCEPTION_V40), &dwReturned))
	{
		DWORD dwErrorNo = NET_DVR_GetLastError();
		TPLOG_ERROR("NET_DVR_GetDVRConfig error(%d),DVRIP(%s)",
			         dwErrorNo, (LPCSTR)(stDvrInfo.strDvrIp.c_str()));
		Msger_NotifyMsg(ALL_USER, _T("布防采集设备:%s 获得异常报警配置函数失败, 错误码(%d)"),
			            fcA2T(stDvrInfo.strDvrIp.c_str()),
			            dwErrorNo);
		NET_DVR_Logout(lLoginID);
		delete pStruExceptionInfo;
		return false;
	}
	for (int nExceptionType = EXCEPTION_HD_FULL; nExceptionType <= EXCEPTION_HD_ERROR; ++nExceptionType)
	{
		pStruExceptionInfo->struExceptionHandle[nExceptionType].dwHandleType |= (1 << 2);   //报警上传中心
	}
	if (!NET_DVR_SetDVRConfig(lLoginID,NET_DVR_SET_EXCEPTIONCFG_V40, 0, pStruExceptionInfo, sizeof(NET_DVR_EXCEPTION_V40)))
	{
		DWORD dwErrorNo = NET_DVR_GetLastError();
		TPLOG_ERROR("NET_DVR_SetDVRConfig error(%d),DVRIP(%s)",
			         dwErrorNo, (LPCSTR)(stDvrInfo.strDvrIp.c_str()));
		Msger_NotifyMsg(ALL_USER, _T("布防采集设备:%s 设置异常报警配置函数失败, 错误码(%d)"),
			            fcA2T(stDvrInfo.strDvrIp.c_str()),
			            dwErrorNo);
		NET_DVR_Logout(lLoginID);
		delete pStruExceptionInfo;
		return false;		
	}
	delete pStruExceptionInfo;

    //设置报警回调函数
    if(!NET_DVR_SetDVRMessageCallBack_V30(MessageCallback, NULL))
    {
        TPLOG_ERROR("NET_DVR_SetDVRMessageCallBack_V31 error(%d)",
            NET_DVR_GetLastError());
		Msger_NotifyMsg(ALL_USER, _T("布防采集设备:%s 设置报警回调函数失败, 错误码(%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			NET_DVR_GetLastError());
        NET_DVR_Logout(lLoginID);
        return false;
    }

    //启用布防
    long lFortifyHandle = NET_DVR_SetupAlarmChan_V30(lLoginID);
    if (lFortifyHandle < 0)
    {
        TPLOG_ERROR("NET_DVR_StartListen_V30 error(%d)",
            NET_DVR_GetLastError());
		Msger_NotifyMsg(ALL_USER, _T("布防采集设备:%s 报警失败, 错误码(%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			NET_DVR_GetLastError());
        NET_DVR_Logout(lLoginID);
        return false;
    }

    //监听句柄放在map里，停止监听时使用。
    m_mapAlarmListen.insert(std::pair<std::string, unsigned long>(stDvrInfo.strDvrIp, lFortifyHandle));
    m_mapAlarmLogin.insert(std::pair<unsigned long, long>(lFortifyHandle, lLoginID));
    //不长期占用登陆资源，需要停止监听时重新登录。
    //NET_DVR_Logout(lUserId);
    return true;
}

/** @fn    StopDvrAlarmListen
*  @brief  停止dvr布防报警
*  @param  [in]stDvrInfo, dvr信息
*  @return bool
*/
bool CDvrPatrol::StopDvrAlarmListen(const collect_info_t &stDvrInfo)
{
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));

    //找到对应的监听句柄
    std::map<std::string, unsigned long>::iterator itor = m_mapAlarmListen.find(stDvrInfo.strDvrIp.c_str());
    if(itor == m_mapAlarmListen.end())
    {
        TPLOG_ERROR("DVR:%s can not find Listen handle",stDvrInfo.strDvrIp.c_str());
        return false;
    }

    //停止布防
    if (!NET_DVR_CloseAlarmChan_V30(itor->second))
    {
        TPLOG_ERROR("NET_DVR_CloseAlarmChan_V30 error(%d)",
            NET_DVR_GetLastError());
		Msger_NotifyMsg(ALL_USER, _T("停止布防采集设备:%s 报警失败, 错误码(%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			NET_DVR_GetLastError());
        return false;
    }

    
    //找到监听句柄对应的登录句柄
    std::map<unsigned long, long>::iterator itorLogin = m_mapAlarmLogin.find(itor->second);
    if(itorLogin == m_mapAlarmLogin.end())
    {
		TPLOG_ERROR("DVR:%s can not find login handle",stDvrInfo.strDvrIp.c_str());
        m_mapAlarmListen.erase(itor);
        return true;
    }

    NET_DVR_Logout(itorLogin->second);
    m_mapAlarmListen.erase(itor);
    m_mapAlarmLogin.erase(itorLogin);
    return true;
}
