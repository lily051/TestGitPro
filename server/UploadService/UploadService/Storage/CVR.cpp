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
*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
*	@param  [in] 无
*	@param  [out] 无
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
*	@brief	释放资源
*	@param  [in] 无
*	@param  [out] 无
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
*	@brief	登录
*	@param  [in] struStorage -- 存储信息
*	@return	LONG -- 登录句柄
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
*	@brief	登出
*	@param  [in] lLoginID -- 登录句柄
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
*	@brief	上传录像
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struStorage -- 存储器信息
*	@param  [in] struRecord -- 录像信息
*	@return	BOOL
*/
BOOL CCVR::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    //1.设置流信息
    //流来源信息
    NET_DVR_STREAM_SRC_INFO struStreamSrc = {0};
    struStreamSrc.dwSize = sizeof(NET_DVR_STREAM_SRC_INFO);
    struStreamSrc.struStreamSrcInfo.byGetStreamType = 0;    //取流方式，0表示直接从设备取流

    memcpy_s(struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.struIP.sIpV4, 16, 
        struRecord.strDvrIp.c_str(), strlen(struRecord.strDvrIp.c_str())); //采集DVRip
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.wDVRPort = struRecord.nDvrPort; //采集DVR端口
    memcpy_s(struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.sUserName, NAME_LEN, 
        struRecord.strDvrUser.c_str(), strlen(struRecord.strDvrUser.c_str()));//采集DVR用户名
    memcpy_s(struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.sPassword, PASSWD_LEN, 
        struRecord.strDvrPwd.c_str(), strlen(struRecord.strDvrPwd.c_str()));  //采集DVR密码
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.dwChannel = struRecord.nChannelID; //采集DVR通道号
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byEnable = 1;       //使能信号
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byOnline = 1;       //在线标识
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byPriority = 1;     //优先级
    struStreamSrc.struStreamSrcInfo.uGetStream.struChanInfo.byLocalBackUp = 0;  //是否本地备份
    //流ID信息
    NET_DVR_STREAM_INFO struStreamInfo = {0};
    struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
    memcpy_s(struStreamInfo.byID, STREAM_ID_LEN, struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str())); //流ID，唯一标识
    struStreamInfo.dwChannel = 0xffffffff;              //不关联设备通道
    int nError = 0; //错误信息

    //设置流信息
    if (!NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_SET_STREAM_SRC_INFO, 1, &struStreamInfo, 
        sizeof(struStreamInfo), &nError, &struStreamSrc, sizeof(struStreamSrc)))
    {
        CVR_LOG_ERROR("call NET_DVR_SetDeviceConfig(NET_DVR_SET_STREAM_SRC_INFO) fail.stream id:%s, error[%d]",
            struRecord.strCameraID.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }

    //2.设置存储录像卷信息
    NET_DVR_STREAM_RECORD_STATUS struRecordStatus = {0};
    struRecordStatus.dwSize = sizeof(NET_DVR_STREAM_RECORD_STATUS);
    struRecordStatus.dwRelatedHD = struRecord.struStorage.nPoolId; //录像卷

    //设置存储录像卷信息
    if (!NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_SET_STREAM_RECORD_STATUS, 1, &struStreamInfo, 
        sizeof(struStreamInfo), &nError, &struRecordStatus, sizeof(struRecordStatus)))
    {
        CVR_LOG_ERROR("call NET_DVR_SetDeviceConfig(NET_DVR_SET_STREAM_RECORD_STATUS) fail.stream id:%s, error[%d]",
            struRecord.strCameraID.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }
    //3.设置录像回传任务
    NET_DVR_ADD_RECORD_PASSBACK_MANUAL_COND struRecordBack = {0};
    struRecordBack.dwSize = sizeof(NET_DVR_ADD_RECORD_PASSBACK_MANUAL_COND);
    struRecordBack.struStreamInfo = struStreamInfo; //流信息

    //设置录像开始时间和结束时间
    NET_DVR_ADD_RECORD_PASSBACK_MANUAL_CFG struRecordInfo = {0};
    struRecordInfo.dwSize = sizeof(NET_DVR_ADD_RECORD_PASSBACK_MANUAL_CFG);
    //录像开始时间
    struRecordInfo.struBeginTime.wYear = struRecord.struRecStartTime.dwYear;
    struRecordInfo.struBeginTime.byMonth = struRecord.struRecStartTime.dwMonth;
    struRecordInfo.struBeginTime.byDay = struRecord.struRecStartTime.dwDay;
    struRecordInfo.struBeginTime.byHour = struRecord.struRecStartTime.dwHour;
    struRecordInfo.struBeginTime.byMinute = struRecord.struRecStartTime.dwMinute;
    struRecordInfo.struBeginTime.bySecond = struRecord.struRecStartTime.dwSecond;   
    //录像结束时间
    struRecordInfo.struEndTime.wYear = struRecord.struRecStopTime.dwYear;
    struRecordInfo.struEndTime.byMonth = struRecord.struRecStopTime.dwMonth;
    struRecordInfo.struEndTime.byDay = struRecord.struRecStopTime.dwDay;
    struRecordInfo.struEndTime.byHour = struRecord.struRecStopTime.dwHour;
    struRecordInfo.struEndTime.byMinute = struRecord.struRecStopTime.dwMinute;
    struRecordInfo.struEndTime.bySecond = struRecord.struRecStopTime.dwSecond;  

    //设置录像手动回传任务
    if (!NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_ADD_RECORD_PASSBACK_TASK_MANUAL, 1, &struRecordBack, 
        sizeof(struRecordBack), &nError, &struRecordInfo, sizeof(struRecordInfo)))
    {
        CVR_LOG_ERROR("call NET_DVR_SetDeviceConfig(NET_DVR_ADD_RECORD_PASSBACK_TASK_MANUAL) fail.stream id:%s, record name:%s, error[%d]",
            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), NET_DVR_GetLastError());
        return FALSE;
    }

    //4.控制回传任务执行
    NET_DVR_RECORD_PASSBACK_MANUAL_CTRL struRecordControl = {0};
    struRecordControl.byControlType = 1;    //恢复所有任务
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
*	@brief	获取上传状态
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 录像信息
*	@param  [out] struRecord.recordStatus -- 录像状态
*	@return	BOOL
*/
BOOL CCVR::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    //流信息
    NET_DVR_STREAM_INFO struStreamInfo = {0};
    struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
    memcpy_s(struStreamInfo.byID, STREAM_ID_LEN, struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str())); //流ID，唯一标识
    struStreamInfo.dwChannel = 0xffffffff;              //不关联设备通道

    NET_DVR_RECORD_PASSBACK_MANUAL_COND struStatus = {0};
    struStatus.dwSize = sizeof(NET_DVR_RECORD_PASSBACK_MANUAL_COND);
    struStatus.byType = 3;  //根据流ID获取任务
    struStatus.struStreamInfo = struStreamInfo;
    LONG lControl = NET_DVR_StartRemoteConfig(lLoginID, NET_DVR_GET_ALL_RECORD_PASSBACK_TASK_MANUAL, &struStatus
        , sizeof(struStatus), NULL, NULL);
    if (lControl != -1)
    {
        NET_DVR_RECORD_PASSBACK_MANUAL_TASK_RET struRecordStatus = {0};
        BOOL bFind = FALSE;
        //未找到录像
        while (!bFind)
        {
            BOOL bEnd = FALSE;
            //循环调用NET_DVR_GetNextRemoteConfig函数查找录像，知道找到录像或者数据全部取完为止
            LONG lRes = NET_DVR_GetNextRemoteConfig(lControl, &struRecordStatus, sizeof(NET_DVR_RECORD_PASSBACK_MANUAL_TASK_RET));
            if (-1 == lRes)
            {
                CVR_LOG_ERROR("call NET_DVR_GetNextRemoteConfig fail.Control id:%d, error[%d]",
                    lControl, NET_DVR_GetLastError());
                return FALSE;
            }
            switch (lRes)
            {
            //数据全部取完，先关闭长连接，再判断最后一次取到的数据是否为要查找的录像记录
            case NET_SDK_GET_NEXT_STATUS_FINISH:
                {
                    NET_DVR_StopRemoteConfig(lControl);
                    lControl = -1;
                    bEnd = TRUE;
                }
				break;
            case NET_SDK_GET_NEXT_STATUS_SUCCESS:
                {
                    //根据录像回传时间获取对应的任务
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
                        //找到该录像，关闭长连接，退出循环
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
            //未找到录像记录或者查找失败，需要退出循环
            if (bEnd)
            {
                break;
            }
            
        }
        if (bFind)
        {
            switch (struRecordStatus.byTaskStatus)
            {
            case 0: //未执行
                {
                    struRecord.recordStatus = RECORDSTAT_UPLOADING; 
                }
                break;
            case 1: //暂停中,恢复回传任务
                {
                    NET_DVR_RECORD_PASSBACK_MANUAL_CTRL struRecordControl = {0};
                    struRecordControl.byControlType = 1;    //恢复所有任务
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
            case 2: //已执行对该录像文件进行锁定
                {
                    NET_DVR_STREAM_TIME_LOCK struLock = {0};
                    struLock.dwSize = sizeof(NET_DVR_STREAM_TIME_LOCK);//大小
                    //录像开始时间
                    struLock.strBeginTime.dwYear = struRecordStatus.struStartTime.wYear;
                    struLock.strBeginTime.dwMonth = struRecordStatus.struStartTime.byMonth;
                    struLock.strBeginTime.dwDay = struRecordStatus.struStartTime.byDay;
                    struLock.strBeginTime.dwHour = struRecordStatus.struStartTime.byHour;
                    struLock.strBeginTime.dwMinute = struRecordStatus.struStartTime.byMinute;
                    struLock.strBeginTime.dwSecond = struRecordStatus.struStartTime.bySecond;
                    //录像结束时间        
                    struLock.strEndTime.dwYear = struRecordStatus.struStopTime.wYear;
                    struLock.strEndTime.dwMonth = struRecordStatus.struStopTime.byMonth;
                    struLock.strEndTime.dwDay = struRecordStatus.struStopTime.byDay;
                    struLock.strEndTime.dwHour = struRecordStatus.struStopTime.byHour;
                    struLock.strEndTime.dwMinute = struRecordStatus.struStopTime.byMinute;
                    struLock.strEndTime.dwSecond = struRecordStatus.struStopTime.bySecond;
                    struLock.struStreamInfo = struStreamInfo;    //流信息
                    struLock.dwRecordType = 10;         //录像类型（回传录像）
                    struLock.dwLockDuration = 0xffffffff;   //永久锁定
                    //录像解锁结果
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
            case 3: //回传中
                {
                    struRecord.recordStatus = RECORDSTAT_UPLOADING;
                }
                break;
			case 4: //回传失败
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
*	@brief	获取存储设备中已上传的录像信息
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 上传录像信息
*	@param  [out] struRecordParam -- 已上传到存储设备中的录像信息
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
	stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID[STREAM_ID_LEN - 1] = NULL;  //防止没有结尾
	stFileCond.uSeniorParam.struStreamIDParam.byBackupVolumeNum = 0;

	LONG lFindHandle = NET_DVR_FindFileByEvent(lLoginID, &stFileCond);
	if (-1 == lFindHandle)
	{
		DWORD dwErrCode = NET_DVR_GetLastError();
		CVR_LOG_ERROR("CVR---NET_DVR_FindFileByEvent失败. lLoginID:%ld, errCode=%lu, ErrMsg:%s", lLoginID, dwErrCode, NET_DVR_GetErrorMsg());
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
		case NET_DVR_FILE_SUCCESS:  //获取文件成功
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
		case NET_DVR_FILE_NOFIND:   //没有查到文件
			{
				bFind = FALSE;
				CVR_LOG_INFO("Hik---NET_DVR_FILE_NOFIND.");
			}
			break;
		case NET_DVR_ISFINDING:     //正在查找
			{
				Sleep(5);
			}
			break;
		case NET_DVR_NOMOREFILE:    //查找结束了
			{
				bFind = FALSE;
			}
			break;
		case NET_DVR_FILE_EXCEPTION:     //查找文件异常
			{
				bFind = FALSE;
				bReturn = FALSE;
				CVR_LOG_ERROR("Hik---NET_DVR_FILE_EXCEPTION. lLogin:%ld, errCode=%ld, ErrMsg:%s", lLoginID, NET_DVR_GetLastError(),NET_DVR_GetErrorMsg());
			}
			break;
		default:                    //查找失败
			{
				bFind = FALSE;
				bReturn = FALSE;
			}
			break;
		}
	}

	//关闭查找
	NET_DVR_FindClose_V30(lFindHandle);

	CVR_LOG_INFO("Hik---按事件查询文件成功,文件个数:%d. lock:%d", struRecordParam.dwRecordNum, struRecordParam.byLockFlag);

	return bReturn;
}


/**	@fn	    setRecordTime
*	@brief	设置时间信息
*	@param  [in] nTime -- 时间戳
*	@param  [out] struTime -- 时间信息
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
*	@brief	设置搜索时间
*	@param  [in] struRecordTime -- 录像时间
*	@param  [out] struSearchTime -- 搜索时间
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
*	@brief	设置文件时间
*	@param  [in] struRecordTime -- 录像时间
*	@param  [out] struFileTime -- 文件时间
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


