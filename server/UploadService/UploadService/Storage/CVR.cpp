#include "StdAfx.h"
#include "CVR.h"
#include <sstream>
#include <string>

#include <time.h>

CCVR::CCVR(void)
{
}

CCVR::~CCVR(void)
{
}

/**	@fn	    Init
*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CCVR::Init()
{
    if (NET_DVR_Init())
    {
        CVR_LOG_INFO("call NET_DVR_Init success.");

        return TRUE;
    }
    else
    {
        CVR_LOG_ERROR("call NET_DVR_Init fail.error[%d]",
            NET_DVR_GetLastError());
    }
    return FALSE;
}

/**	@fn	    Cleanup
*	@brief	�ͷ���Դ
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CCVR::Cleanup()
{
    if (NET_DVR_Cleanup())
    {
        CVR_LOG_INFO("call NET_DVR_Cleanup success.");

        return TRUE;
    }
    else
    {
        CVR_LOG_ERROR("call NET_DVR_Cleanup fail.error[%d]",
            NET_DVR_GetLastError());
    }

    return FALSE;
}

/**	@fn	    Login
*	@brief	��¼
*	@param  [in] struStorage -- �洢��Ϣ
*	@return	LONG -- ��¼���
*/
LONG CCVR::Login(const Struct_StorageConfig &struStorage)
{
    LONG lLoginID = STORAGE_LOGIN_FAILED;
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
    if (STORAGE_LOGIN_FAILED == lLoginID)
    {
        CVR_LOG_ERROR("call NET_DVR_Login_V40 fail. ip:%s, port:%d, error[%d]",
            struStorage.strIp.c_str(), struStorage.nPort, NET_DVR_GetLastError());
        return lLoginID;
    }
    CVR_LOG_INFO("Login succeed. ip:%s, port:%d",
        struStorage.strIp.c_str(), struStorage.nPort);
    return lLoginID;
}

/**	@fn	    Logout
*	@brief	�ǳ�
*	@param  [in] lLoginID -- ��¼���
*	@return	BOOL
*/
BOOL CCVR::Logout(LONG lLoginID)
{
    if (!NET_DVR_Logout(lLoginID))
    {
        CVR_LOG_ERROR("Logout Device failed.lLoginID:%d, error[%d]",
            lLoginID, NET_DVR_GetLastError());

        return FALSE;
    }

    CVR_LOG_INFO("Logout succeed. lLoginID:%d",
        lLoginID);
    return TRUE;
}

/**	@fn	    Upload
*	@brief	�ϴ�¼��
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struStorage -- �洢����Ϣ
*	@param  [in] struRecord -- ¼����Ϣ
*	@return	BOOL
*/
BOOL CCVR::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    //1.��������Ϣ
    //����Դ��Ϣ
    NET_DVR_STREAM_SRC_INFO struStreamSrc = {0};
    struStreamSrc.dwSize = sizeof(NET_DVR_STREAM_SRC_INFO);
    struStreamSrc.struStreamSrcInfo.byGetStreamType = 0;    //ȡ����ʽ��0��ʾֱ�Ӵ��豸ȡ��

    memcpy_s(struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.struIP.sIpV4, 16, 
        struRecord.strDvrIp.c_str(), strlen(struRecord.strDvrIp.c_str())); //�ɼ�DVRip
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.wDVRPort = struRecord.nDvrPort; //�ɼ�DVR�˿�
    memcpy_s(struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.sUserName, NAME_LEN, 
        struRecord.strDvrUser.c_str(), strlen(struRecord.strDvrUser.c_str()));//�ɼ�DVR�û���
    memcpy_s(struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.sPassword, PASSWD_LEN, 
        struRecord.strDvrPwd.c_str(), strlen(struRecord.strDvrPwd.c_str()));  //�ɼ�DVR����
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.dwChannel = struRecord.nChannelID; //�ɼ�DVRͨ����
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byEnable = 1;       //ʹ���ź�
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byOnline = 1;       //���߱�ʶ
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byPriority = 1;     //���ȼ�
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byLocalBackUp = 0;  //�Ƿ񱾵ر���
    //��ID��Ϣ
    NET_DVR_STREAM_INFO struStreamInfo = {0};
    struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
    memcpy_s(struStreamInfo.byID, STREAM_ID_LEN, struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str())); //��ID��Ψһ��ʶ
    struStreamInfo.dwChannel = 0xffffffff;              //�������豸ͨ��
    int nError = 0; //������Ϣ

    //��������Ϣ
    if (!NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_SET_STREAM_SRC_INFO, 1, &struStreamInfo, 
        sizeof(struStreamInfo), &nError, &struStreamSrc, sizeof(struStreamSrc)))
    {
        CVR_LOG_ERROR("call NET_DVR_SetDeviceConfig(NET_DVR_SET_STREAM_SRC_INFO) fail.stream id:%s, error[%d]",
            struRecord.strCameraID.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }

    //2.���ô洢¼�����Ϣ
    NET_DVR_STREAM_RECORD_STATUS struRecordStatus = {0};
    struRecordStatus.dwSize = sizeof(NET_DVR_STREAM_RECORD_STATUS);
    struRecordStatus.dwRelatedHD = struRecord.struStorage.nPoolId; //¼���

    //���ô洢¼�����Ϣ
    if (!NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_SET_STREAM_RECORD_STATUS, 1, &struStreamInfo, 
        sizeof(struStreamInfo), &nError, &struRecordStatus, sizeof(struRecordStatus)))
    {
        CVR_LOG_ERROR("call NET_DVR_SetDeviceConfig(NET_DVR_SET_STREAM_RECORD_STATUS) fail.stream id:%s, error[%d]",
            struRecord.strCameraID.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }
    //3.����¼��ش�����
    NET_DVR_ADD_RECORD_PASSBACK_MANUAL_COND struRecordBack = {0};
    struRecordBack.dwSize = sizeof(NET_DVR_ADD_RECORD_PASSBACK_MANUAL_COND);
    struRecordBack.struStreamInfo = struStreamInfo; //����Ϣ

    //����¼��ʼʱ��ͽ���ʱ��
    NET_DVR_ADD_RECORD_PASSBACK_MANUAL_CFG struRecordInfo = {0};
    struRecordInfo.dwSize = sizeof(NET_DVR_ADD_RECORD_PASSBACK_MANUAL_CFG);
    //¼��ʼʱ��
    struRecordInfo.struBeginTime.wYear = struRecord.struRecStartTime.dwYear;
    struRecordInfo.struBeginTime.byMonth = struRecord.struRecStartTime.dwMonth;
    struRecordInfo.struBeginTime.byDay = struRecord.struRecStartTime.dwDay;
    struRecordInfo.struBeginTime.byHour = struRecord.struRecStartTime.dwHour;
    struRecordInfo.struBeginTime.byMinute = struRecord.struRecStartTime.dwMinute;
    struRecordInfo.struBeginTime.bySecond = struRecord.struRecStartTime.dwSecond;   
    //¼�����ʱ��
    struRecordInfo.struEndTime.wYear = struRecord.struRecStopTime.dwYear;
    struRecordInfo.struEndTime.byMonth = struRecord.struRecStopTime.dwMonth;
    struRecordInfo.struEndTime.byDay = struRecord.struRecStopTime.dwDay;
    struRecordInfo.struEndTime.byHour = struRecord.struRecStopTime.dwHour;
    struRecordInfo.struEndTime.byMinute = struRecord.struRecStopTime.dwMinute;
    struRecordInfo.struEndTime.bySecond = struRecord.struRecStopTime.dwSecond;  

    //����¼���ֶ��ش�����
    if (!NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_ADD_RECORD_PASSBACK_TASK_MANUAL, 1, &struRecordBack, 
        sizeof(struRecordBack), &nError, &struRecordInfo, sizeof(struRecordInfo)))
    {
        CVR_LOG_ERROR("call NET_DVR_SetDeviceConfig(NET_DVR_ADD_RECORD_PASSBACK_TASK_MANUAL) fail.stream id:%s, record name:%s, error[%d]",
            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }

    //4.���ƻش�����ִ��
    NET_DVR_RECORD_PASSBACK_MANUAL_CTRL struRecordControl = {0};
    struRecordControl.byControlType = 1;    //�ָ���������
    struRecordControl.dwSize = sizeof(NET_DVR_RECORD_PASSBACK_MANUAL_CTRL);
    if (!NET_DVR_RemoteControl(lLoginID, NET_DVR_RECORD_PASSBACK_TASK_MANUAL_CTRL, &struRecordControl, sizeof(struRecordControl)))
    {
        CVR_LOG_ERROR("call NET_DVR_RemoteControl(NET_DVR_RECORD_PASSBACK_TASK_MANUAL_CTRL) fail.stream id:%s, record name:%s, error[%d]",
            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }

    CVR_LOG_INFO("set upload task succeed.stream id:%s, record name:%s",
        struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str());
    return TRUE;
}

/**	@fn	    GetUploadStatus
*	@brief	��ȡ�ϴ�״̬
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- ¼����Ϣ
*	@param  [out] struRecord.recordStatus -- ¼��״̬
*	@return	BOOL
*/
BOOL CCVR::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    //����Ϣ
    NET_DVR_STREAM_INFO struStreamInfo = {0};
    struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
    memcpy_s(struStreamInfo.byID, STREAM_ID_LEN, struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str())); //��ID��Ψһ��ʶ
    struStreamInfo.dwChannel = 0xffffffff;              //�������豸ͨ��

    NET_DVR_RECORD_PASSBACK_MANUAL_COND struStatus = {0};
    struStatus.dwSize = sizeof(NET_DVR_RECORD_PASSBACK_MANUAL_COND);
    struStatus.byType = 3;  //������ID��ȡ����
    struStatus.struStreamInfo = struStreamInfo;
    LONG lControl = NET_DVR_StartRemoteConfig(lLoginID, NET_DVR_GET_ALL_RECORD_PASSBACK_TASK_MANUAL, &struStatus
        , sizeof(struStatus), NULL, NULL);
    if (lControl != -1)
    {
        NET_DVR_RECORD_PASSBACK_MANUAL_TASK_RET struRecordStatus = {0};
        BOOL bFind = FALSE;
        //δ�ҵ�¼��
        while (!bFind)
        {
            BOOL bEnd = FALSE;
            //ѭ������NET_DVR_GetNextRemoteConfig��������¼��֪���ҵ�¼���������ȫ��ȡ��Ϊֹ
            LONG lRes = NET_DVR_GetNextRemoteConfig(lControl, &struRecordStatus, sizeof(NET_DVR_RECORD_PASSBACK_MANUAL_TASK_RET));
            if (-1 == lRes)
            {
                CVR_LOG_ERROR("call NET_DVR_GetNextRemoteConfig fail.Control id:%d, error[%d]",
                    lControl, NET_DVR_GetLastError());
                return FALSE;
            }
            switch (lRes)
            {
            //����ȫ��ȡ�꣬�ȹرճ����ӣ����ж����һ��ȡ���������Ƿ�ΪҪ���ҵ�¼���¼
            case NET_SDK_GET_NEXT_STATUS_FINISH:
                {
                    NET_DVR_StopRemoteConfig(lControl);
                    lControl = -1;
                    bEnd = TRUE;
                }
				break;
            case NET_SDK_GET_NEXT_STATUS_SUCCESS:
                {
                    //����¼��ش�ʱ���ȡ��Ӧ������
                    if ((struRecordStatus.struStartTime.wYear       == struRecord.struRecStartTime.dwYear) && 
                        (struRecordStatus.struStartTime.byMonth     == struRecord.struRecStartTime.dwMonth) &&
                        (struRecordStatus.struStartTime.byDay       == struRecord.struRecStartTime.dwDay) &&
                        (struRecordStatus.struStartTime.byHour      == struRecord.struRecStartTime.dwHour) &&
                        (struRecordStatus.struStartTime.byMinute    == struRecord.struRecStartTime.dwMinute) &&
                        (struRecordStatus.struStartTime.bySecond    == struRecord.struRecStartTime.dwSecond) &&
                        (struRecordStatus.struStopTime.wYear        == struRecord.struRecStopTime.dwYear) && 
                        (struRecordStatus.struStopTime.byMonth      == struRecord.struRecStopTime.dwMonth) &&
                        (struRecordStatus.struStopTime.byDay        == struRecord.struRecStopTime.dwDay) &&
                        (struRecordStatus.struStopTime.byHour       == struRecord.struRecStopTime.dwHour) &&
                        (struRecordStatus.struStopTime.byMinute     == struRecord.struRecStopTime.dwMinute) &&
                        (struRecordStatus.struStopTime.bySecond     == struRecord.struRecStopTime.dwSecond))
                    {
                        //�ҵ���¼�񣬹رճ����ӣ��˳�ѭ��
                        bFind = TRUE;
                        if (-1 != lControl)
                        {
                            NET_DVR_StopRemoteConfig(lControl);
                            lControl = -1;
                        }
                    }
                }
                break;
            case NET_SDK_GET_NETX_STATUS_NEED_WAIT:
                {
                    Sleep(500);
                }
                break;
            case NET_SDK_GET_NEXT_STATUS_FAILED:
                {
                    NET_DVR_StopRemoteConfig(lControl);
                    lControl = -1;
                    bEnd = TRUE;
                }
                break;
			default:
				{
					CVR_LOG_ERROR("unknown status return by NET_DVR_GetNextRemoteConfig");
				}
				break;
            }
            //δ�ҵ�¼���¼���߲���ʧ�ܣ���Ҫ�˳�ѭ��
            if (bEnd)
            {
                break;
            }
            
        }
        if (bFind)
        {
            switch (struRecordStatus.byTaskStatus)
            {
            case 0: //δִ��
                {
                    struRecord.recordStatus = RECORDSTAT_UPLOADING; 
                }
                break;
            case 1: //��ͣ��,�ָ��ش�����
                {
                    NET_DVR_RECORD_PASSBACK_MANUAL_CTRL struRecordControl = {0};
                    struRecordControl.byControlType = 1;    //�ָ���������
                    struRecordControl.dwSize = sizeof(NET_DVR_RECORD_PASSBACK_MANUAL_CTRL);
                    if (!NET_DVR_RemoteControl(lLoginID, NET_DVR_RECORD_PASSBACK_TASK_MANUAL_CTRL, &struRecordControl, sizeof(struRecordControl)))
                    {
                        CVR_LOG_ERROR("call NET_DVR_RemoteControl(NET_DVR_RECORD_PASSBACK_TASK_MANUAL_CTRL) fail.Login id:%d, stream id:%s, record name:%s, error[%d]",
                            lLoginID, struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), NET_DVR_GetLastError());
                        return FALSE;
                    }
                    struRecord.recordStatus = RECORDSTAT_UPLOADING;
                }
                break;
            case 2: //��ִ�жԸ�¼���ļ���������
                {
                    NET_DVR_STREAM_TIME_LOCK struLock = {0};
                    struLock.dwSize = sizeof(NET_DVR_STREAM_TIME_LOCK);//��С
                    //¼��ʼʱ��
                    struLock.strBeginTime.dwYear = struRecordStatus.struStartTime.wYear;
                    struLock.strBeginTime.dwMonth = struRecordStatus.struStartTime.byMonth;
                    struLock.strBeginTime.dwDay = struRecordStatus.struStartTime.byDay;
                    struLock.strBeginTime.dwHour = struRecordStatus.struStartTime.byHour;
                    struLock.strBeginTime.dwMinute = struRecordStatus.struStartTime.byMinute;
                    struLock.strBeginTime.dwSecond = struRecordStatus.struStartTime.bySecond;
                    //¼�����ʱ��        
                    struLock.strEndTime.dwYear = struRecordStatus.struStopTime.wYear;
                    struLock.strEndTime.dwMonth = struRecordStatus.struStopTime.byMonth;
                    struLock.strEndTime.dwDay = struRecordStatus.struStopTime.byDay;
                    struLock.strEndTime.dwHour = struRecordStatus.struStopTime.byHour;
                    struLock.strEndTime.dwMinute = struRecordStatus.struStopTime.byMinute;
                    struLock.strEndTime.dwSecond = struRecordStatus.struStopTime.bySecond;
                    struLock.struStreamInfo = struStreamInfo;    //����Ϣ
                    struLock.dwRecordType = 10;         //¼�����ͣ��ش�¼��
                    struLock.dwLockDuration = 0xffffffff;   //��������
                    //¼��������
                    NET_DVR_LOCK_RETURN struLockReturn = {0};
                    struLockReturn.dwSize = sizeof(NET_DVR_LOCK_RETURN);
                    if (!NET_DVR_LockStreamFileByTime(lLoginID, &struLock, &struLockReturn))
                    {
                        CVR_LOG_ERROR("call NET_DVR_LockStreamFileByTime failed.stream id:%s, record name:%s, error[%d]",
                            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), NET_DVR_GetLastError());
                    }
                    struRecord.recordStatus = RECORDSTAT_OVER;                        
                }
                break;
            case 3: //�ش���
                {
                    struRecord.recordStatus = RECORDSTAT_UPLOADING;
                }
                break;
			case 4: //�ش�ʧ��
				{
					struRecord.recordStatus = RECORDSTAT_FAIL;
				}
				break;
            default:
                {
                    struRecord.recordStatus = RECORDSTAT_FAIL;
                }
                break;
            }
        }
        else
        {
            CVR_LOG_ERROR("not find the record.");
            return FALSE;
        }
    }
    else
    {
        CVR_LOG_ERROR("call NET_DVR_StartRemoteConfig(NET_DVR_GET_ALL_RECORD_PASSBACK_TASK_MANUAL) fail.lLoginID:%d, stream id:%s, error[%d]",
            lLoginID, struRecord.strCameraID.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }

    CVR_LOG_INFO("GetUploadStatus succeed.lLoginID:%d, stream id:%s, record name:%s, Status:%d",
        lLoginID, struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), struRecord.recordStatus);
    return TRUE;
}

/**	@fn	    GetUploadRecordInfo
*	@brief	��ȡ�洢�豸�����ϴ���¼����Ϣ
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- �ϴ�¼����Ϣ
*	@param  [out] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
*	@return	BOOL
*/
BOOL CCVR::GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam)
{
	NET_DVR_SEARCH_EVENT_PARAM stFileCond;
	stFileCond.wMajorType = (WORD)EVENT_STREAM_INFO;
	stFileCond.wMinorType = (WORD)EVENT_STREAM_ID;

	SetSearchTime(struRecord.struRecStartTime, stFileCond.struStartTime);
	SetSearchTime(struRecord.struRecStopTime, stFileCond.struEndTime);

	stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
	strncpy((char*)stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID, struRecord.strCameraID.c_str(), STREAM_ID_LEN);
	stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID[STREAM_ID_LEN - 1] = NULL;  //��ֹû�н�β
	stFileCond.uSeniorParam.struStreamIDParam.byBackupVolumeNum = 0;

	LONG lFindHandle = NET_DVR_FindFileByEvent(lLoginID, &stFileCond);
	if (-1 == lFindHandle)
	{
		DWORD dwErrCode = NET_DVR_GetLastError();
		CVR_LOG_ERROR("CVR---NET_DVR_FindFileByEventʧ��. lLoginID:%ld, errCode=%lu, ErrMsg:%s", lLoginID, dwErrCode, NET_DVR_GetErrorMsg());
		return FALSE;
	}

	BOOL bFind = TRUE;
	BOOL bReturn = TRUE;
	while (bFind)
	{
		NET_DVR_SEARCH_EVENT_RET stFileData;
		LONG lFileHandle = NET_DVR_FindNextEvent(lFindHandle, &stFileData);
		switch (lFileHandle)
		{
		case NET_DVR_FILE_SUCCESS:  //��ȡ�ļ��ɹ�
			{
				if (0 == struRecordParam.dwRecordNum)
				{
					SetFileTime(stFileData.struStartTime ,struRecordParam.struRecStartTime);
				}
				SetFileTime(stFileData.struEndTime ,struRecordParam.struRecStopTime);
				struRecordParam.dwRecordLength += stFileData.uSeniorRet.struStreamIDRet.dwRecordLength;
				struRecordParam.byLockFlag = stFileData.uSeniorRet.struStreamIDRet.byLockFlag;
				struRecordParam.dwRecordNum++;
			}
			break;
		case NET_DVR_FILE_NOFIND:   //û�в鵽�ļ�
			{
				bFind = FALSE;
				CVR_LOG_INFO("Hik---NET_DVR_FILE_NOFIND.");
			}
			break;
		case NET_DVR_ISFINDING:     //���ڲ���
			{
				Sleep(5);
			}
			break;
		case NET_DVR_NOMOREFILE:    //���ҽ�����
			{
				bFind = FALSE;
			}
			break;
		case NET_DVR_FILE_EXCEPTION:     //�����ļ��쳣
			{
				bFind = FALSE;
				bReturn = FALSE;
				CVR_LOG_ERROR("Hik---NET_DVR_FILE_EXCEPTION. lLogin:%ld, errCode=%ld, ErrMsg:%s", lLoginID, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
			}
			break;
		default:                    //����ʧ��
			{
				bFind = FALSE;
				bReturn = FALSE;
			}
			break;
		}
	}

	//�رղ���
	NET_DVR_FindClose_V30(lFindHandle);

	CVR_LOG_INFO("Hik---���¼���ѯ�ļ��ɹ�,�ļ�����:%d. lock:%d", struRecordParam.dwRecordNum, struRecordParam.byLockFlag);

	return bReturn;
}


/**	@fn	    setRecordTime
*	@brief	����ʱ����Ϣ
*	@param  [in] nTime -- ʱ���
*	@param  [out] struTime -- ʱ����Ϣ
*	@return	BOOL
*/
BOOL CCVR::setRecordTime(const int &nTime, NET_DVR_TIME_EX &struTime)
{
    time_t tTmp = nTime;
    struct tm* pTime = gmtime(&tTmp);
    if (NULL == pTime)
    {
        CVR_LOG_ERROR("pTime is NULL.time:%d", nTime);
        return FALSE;
    }
    struTime.wYear = pTime->tm_year;     
    struTime.byMonth = pTime->tm_mon;      
    struTime.byDay = pTime->tm_mday;     
    struTime.byHour = pTime->tm_hour;
    struTime.byMinute = pTime->tm_min;
    struTime.bySecond = pTime->tm_sec;
    return TRUE;
}

/**	@fn	    SetSearchTime
*	@brief	��������ʱ��
*	@param  [in] struRecordTime -- ¼��ʱ��
*	@param  [out] struSearchTime -- ����ʱ��
*	@return	BOOL
*/
BOOL CCVR::SetSearchTime(const Struct_RecordTime &struRecordTime, NET_DVR_TIME &struSearchTime)
{
	struSearchTime.dwYear    = struRecordTime.dwYear;     
	struSearchTime.dwMonth  = struRecordTime.dwMonth;      
	struSearchTime.dwDay    = struRecordTime.dwDay;     
	struSearchTime.dwHour   = struRecordTime.dwHour;
	struSearchTime.dwMinute = struRecordTime.dwMinute;
	struSearchTime.dwSecond = struRecordTime.dwSecond;
	return TRUE;
}

/**	@fn	    SetFileTime
*	@brief	�����ļ�ʱ��
*	@param  [in] struRecordTime -- ¼��ʱ��
*	@param  [out] struFileTime -- �ļ�ʱ��
*	@return	BOOL
*/
BOOL CCVR::SetFileTime(const NET_DVR_TIME &struRecordTime, Struct_RecordTime &struFileTime)
{
	struFileTime.dwYear    = struRecordTime.dwYear;     
	struFileTime.dwMonth   = struRecordTime.dwMonth;      
	struFileTime.dwDay     = struRecordTime.dwDay;     
	struFileTime.dwHour    = struRecordTime.dwHour;
	struFileTime.dwMinute  = struRecordTime.dwMinute;
	struFileTime.dwSecond  = struRecordTime.dwSecond;
	return TRUE;
}


