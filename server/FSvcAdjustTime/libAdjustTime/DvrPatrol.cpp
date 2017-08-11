/**	@file    DvrPatrol.cpp 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   dvr�豸Ѳ�촦�� ��
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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

//������Ϣ�ص�����
void CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer,
                              char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
	if(NULL == pAlarmer || NULL == pAlarmInfo)
	{
		TPLOG_ERROR("��Ч����");
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
            case 1: //Ӳ����
            case 4: //Ӳ��δ��ʽ��
            case 5: //��дӲ�̳���
                {
                    for(int i = 0; i < MAX_DISKNUM_V30; i++)
                    {
                        //�ҵ��쳣��Ӳ��
                        if(1 == struAlarmInfo.byDiskNumber[i])
                        {
                            std::string strDvrIp = pAlarmer->sDeviceIP;
                            TPLOG_ERROR("�ɼ��豸:%s Ӳ��:%d�쳣,��������(%d)",pAlarmer->sDeviceIP,i+1,struAlarmInfo.dwAlarmType);
                            Msger_DeviceAlarmMsg(WORK_TYPE_DVR_HD_EXCEPTION,
                                ALL_USER,
                                i+1,
                                ALARM_OCCUR,
                                TP_ALARM_DVR,
                                strDvrIp.c_str(),
                                strDvrIp.size(),
                                _T("�ɼ��豸:%s Ӳ��:%d�쳣,��������(%d)"),
                                fcA2T(pAlarmer->sDeviceIP),
                                i+1,
                                struAlarmInfo.dwAlarmType);
                        }
                    }
                }
                break;
            case 2: //��Ƶ�źŶ�ʧ
                {
                    for(int i = 0; i < MAX_CHANNUM_V30; i++)
                    {
                        //�ҵ��쳣��ͨ��
                        if(1 == struAlarmInfo.byChannel[i])
                        {
                            std::string strDvrIp = pAlarmer->sDeviceIP;
                            TPLOG_ERROR("�ɼ��豸: %s ͨ��:%d ��Ƶ�źŶ�ʧ",pAlarmer->sDeviceIP,i+1);
                            Msger_DeviceAlarmMsg(WORK_TYPE_DVR_VI_LOST,
                                ALL_USER,
                                i+1,
                                ALARM_OCCUR,
                                TP_ALARM_DVR,
                                strDvrIp.c_str(),
                                strDvrIp.size(),
                                _T("�ɼ��豸:%s ͨ��:%d ��Ƶ�źŶ�ʧ"),
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
*  @brief  ���dvr�Ƿ�����
*  @param  [in]stDvrInfo, dvr��Ϣ
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
				_T("�ɼ��豸:%s:%d ������"),
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
			Msger_NotifyMsg(ALL_USER, _T("���ɼ��豸�Ƿ�����ʱ��½�ɼ��豸:%s,%d,%s ʧ��, ������(%d)"),
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
	if (0 == GetConfigPtr()->m_nHaveCenterStorage) //�����Ĵ洢Ҫ���DVR����������������б���
	{
		CheckDvrDisk(stDvrInfo, lLoginID);
	}

    //�ǳ�dvr
    NET_DVR_Logout(lLoginID);

    return true;
}

/** @fn     CheckDvrDisk
*  @brief  ���dvrӲ�������Ƿ���
*  @param  [in]lUserId��¼���
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
		TPLOG_ERROR("��ȡ dvr %s:%d:%s ������Ϣʧ��(%d)",
			(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
			stDvrInfo.usPort,
			(LPCSTR)(stDvrInfo.strUserName.c_str()),
			ulErrorCode);
		Msger_NotifyMsg(ALL_USER, _T("�豸:%s ��ȡ������Ϣʧ��(������%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			ulErrorCode);
		return false;
	}

	//Ӳ�������� ��λMB
	unsigned int uiTotalSpace = 0;
	//Ӳ��ʣ������ ��λMB
	unsigned int uiFreeSpace = 0;
	//Ӳ��ʣ������ ��λGB
	unsigned int uiFreeSpaceGB = 0;
	//Ӳ��ʣ�������ٷֱ�
	double dFreeSpacePercent = 0.0;
	for (DWORD i = 0; i < stDvrHDCfg.dwHDCount; ++i)
	{
		//Ӳ��״̬HD_STAT 0-����
		if (HD_STAT_OK == stDvrHDCfg.struHDInfo[i].dwHdStatus)
		{
			//Ӳ������
			uiTotalSpace += stDvrHDCfg.struHDInfo[i].dwCapacity;
			//Ӳ��ʣ��ռ�
			uiFreeSpace += stDvrHDCfg.struHDInfo[i].dwFreeSpace;
		}
	}
	if (uiTotalSpace == 0)
	{
		TPLOG_ERROR("dvr %s ����������Ϊ0",(LPCSTR)(stDvrInfo.strDvrIp.c_str()));
		return false;
	}

	uiFreeSpaceGB     = uiFreeSpace / 1024;
	dFreeSpacePercent = (double)(((double)uiFreeSpace / (double)uiTotalSpace) * 100);
	//С��100G
	if (uiFreeSpaceGB < 100)
	{
		TPLOG_ERROR("dvr %s �����������㣬ʣ��%dGB",
			(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
			uiFreeSpaceGB);
		Msger_DeviceAlarmMsg(WORK_TYPE_DVR_NEARLLY_FULL,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_DVR,
			stDvrInfo.strDvrIp.c_str(),
			stDvrInfo.strDvrIp.size(),
			_T("�ɼ��豸���̿�����(�豸ip:%s, ʣ��%dGB)"), 
			fcA2T(stDvrInfo.strDvrIp.c_str()), 
			uiFreeSpaceGB);
		return true;
	}

	//С��5%
	if (dFreeSpacePercent < 5)
	{
		TPLOG_ERROR("dvr %s ����ʣ������ռ�Ȳ��㣬ʣ��%d%%",
			(LPCSTR)(stDvrInfo.strDvrIp.c_str()),
			dFreeSpacePercent);
		Msger_DeviceAlarmMsg(WORK_TYPE_DVR_NEARLLY_FULL,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_DVR,
			stDvrInfo.strDvrIp.c_str(),
			stDvrInfo.strDvrIp.size(),
			_T("�ɼ��豸���̿�����(�豸ip:%s, ʣ��%d%%)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			(int)dFreeSpacePercent);
		return true;
	}

	return true;
}

/** @fn     StartDvrAlarmListen
*  @brief  ��ʼdvr��������
*  @param  [in]stDvrInfo, dvr��Ϣ
*  @return bool
*/
bool CDvrPatrol::StartDvrAlarmListen(const collect_info_t &stDvrInfo)
{

    //�ж� DVR�Ƿ��Ѿ��ڱ������� �еĻ�ֱ�ӷ��ء�
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
		Msger_NotifyMsg(ALL_USER, _T("��½�ɼ��豸:%s,%d,%s ʧ��, ������(%d)"),
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

	//�����ϱ����ı�������
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
		Msger_NotifyMsg(ALL_USER, _T("�����ɼ��豸:%s ����쳣�������ú���ʧ��, ������(%d)"),
			            fcA2T(stDvrInfo.strDvrIp.c_str()),
			            dwErrorNo);
		NET_DVR_Logout(lLoginID);
		delete pStruExceptionInfo;
		return false;
	}
	for (int nExceptionType = EXCEPTION_HD_FULL; nExceptionType <= EXCEPTION_HD_ERROR; ++nExceptionType)
	{
		pStruExceptionInfo->struExceptionHandle[nExceptionType].dwHandleType |= (1 << 2);   //�����ϴ�����
	}
	if (!NET_DVR_SetDVRConfig(lLoginID,NET_DVR_SET_EXCEPTIONCFG_V40, 0, pStruExceptionInfo, sizeof(NET_DVR_EXCEPTION_V40)))
	{
		DWORD dwErrorNo = NET_DVR_GetLastError();
		TPLOG_ERROR("NET_DVR_SetDVRConfig error(%d),DVRIP(%s)",
			         dwErrorNo, (LPCSTR)(stDvrInfo.strDvrIp.c_str()));
		Msger_NotifyMsg(ALL_USER, _T("�����ɼ��豸:%s �����쳣�������ú���ʧ��, ������(%d)"),
			            fcA2T(stDvrInfo.strDvrIp.c_str()),
			            dwErrorNo);
		NET_DVR_Logout(lLoginID);
		delete pStruExceptionInfo;
		return false;		
	}
	delete pStruExceptionInfo;

    //���ñ����ص�����
    if(!NET_DVR_SetDVRMessageCallBack_V30(MessageCallback, NULL))
    {
        TPLOG_ERROR("NET_DVR_SetDVRMessageCallBack_V31 error(%d)",
            NET_DVR_GetLastError());
		Msger_NotifyMsg(ALL_USER, _T("�����ɼ��豸:%s ���ñ����ص�����ʧ��, ������(%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			NET_DVR_GetLastError());
        NET_DVR_Logout(lLoginID);
        return false;
    }

    //���ò���
    long lFortifyHandle = NET_DVR_SetupAlarmChan_V30(lLoginID);
    if (lFortifyHandle < 0)
    {
        TPLOG_ERROR("NET_DVR_StartListen_V30 error(%d)",
            NET_DVR_GetLastError());
		Msger_NotifyMsg(ALL_USER, _T("�����ɼ��豸:%s ����ʧ��, ������(%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			NET_DVR_GetLastError());
        NET_DVR_Logout(lLoginID);
        return false;
    }

    //�����������map�ֹͣ����ʱʹ�á�
    m_mapAlarmListen.insert(std::pair<std::string, unsigned long>(stDvrInfo.strDvrIp, lFortifyHandle));
    m_mapAlarmLogin.insert(std::pair<unsigned long, long>(lFortifyHandle, lLoginID));
    //������ռ�õ�½��Դ����Ҫֹͣ����ʱ���µ�¼��
    //NET_DVR_Logout(lUserId);
    return true;
}

/** @fn    StopDvrAlarmListen
*  @brief  ֹͣdvr��������
*  @param  [in]stDvrInfo, dvr��Ϣ
*  @return bool
*/
bool CDvrPatrol::StopDvrAlarmListen(const collect_info_t &stDvrInfo)
{
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));

    //�ҵ���Ӧ�ļ������
    std::map<std::string, unsigned long>::iterator itor = m_mapAlarmListen.find(stDvrInfo.strDvrIp.c_str());
    if(itor == m_mapAlarmListen.end())
    {
        TPLOG_ERROR("DVR:%s can not find Listen handle",stDvrInfo.strDvrIp.c_str());
        return false;
    }

    //ֹͣ����
    if (!NET_DVR_CloseAlarmChan_V30(itor->second))
    {
        TPLOG_ERROR("NET_DVR_CloseAlarmChan_V30 error(%d)",
            NET_DVR_GetLastError());
		Msger_NotifyMsg(ALL_USER, _T("ֹͣ�����ɼ��豸:%s ����ʧ��, ������(%d)"),
			fcA2T(stDvrInfo.strDvrIp.c_str()),
			NET_DVR_GetLastError());
        return false;
    }

    
    //�ҵ����������Ӧ�ĵ�¼���
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
