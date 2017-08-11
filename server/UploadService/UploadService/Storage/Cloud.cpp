#include "StdAfx.h"
#include "Cloud.h"
#include "hcs_sdk.h"
#include <sstream>
#include <string>

CCloud::CCloud(void)
{
}

CCloud::~CCloud(void)
{
}

/**	@fn	    Init
*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
*	@param  [in] 无
*	@param  [out] 无
*	@return	BOOL
*/
BOOL CCloud::Init()
{
    //云存储初始化
    int nRes = HCS_SDK_Init();
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_Init fail.error[%d]",
            nRes);
        return FALSE; 
    }
    CLOUD_LOG_INFO("call HCS_SDK_Init success.");
    return TRUE;
    
}

/**	@fn	    Cleanup
*	@brief	释放资源
*	@param  [in] 无
*	@param  [out] 无
*	@return	BOOL
*/
BOOL CCloud::Cleanup()
{
    //反初始化
    int nRes = HCS_SDK_Deinit();
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_Deinit fail.error[%d]",
            nRes);
        return FALSE;    
    }

    CLOUD_LOG_INFO("call HCS_SDK_Deinit success.");
    return TRUE;
    
}

/**	@fn	    Login
*	@brief	登录
*	@param  [in] struStorage -- 存储器信息
*	@param  [out] struStorage -- 登录句柄
*	@return	BOOL
*/
LONG CCloud::Login(const Struct_StorageConfig &struStorage)
{
    LONG lLoginID = STORAGE_LOGIN_FAILED;
    unsigned long long lLogin;
    int nRes = HCS_SDK_Login(struStorage.strIp.c_str(), struStorage.nPort, struStorage.strUser.c_str(), struStorage.strPwd.c_str(), &lLogin);
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_Login failed.ip:%s, port:%d, error[%d]",
            struStorage.strIp.c_str(), struStorage.nPort, nRes);
        return STORAGE_LOGIN_FAILED;
    }
    lLoginID = (LONG)lLogin;
    CLOUD_LOG_INFO("Login succeed.ip:%s, port:%d",
        struStorage.strIp.c_str(), struStorage.nPort);
    return lLoginID;
}

/**	@fn	    Logout
*	@brief	登出
*	@param  [in] lLoginID -- 登录句柄
*	@return	BOOL
*/
BOOL CCloud::Logout(LONG lLoginID)
{
    int nRes = HCS_SDK_Logout(lLoginID);
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_Logout failed.lLoginID:%d, error[%d]",
            lLoginID, nRes);
        return FALSE;
    }
    CLOUD_LOG_INFO("Logout succeed.");
    return TRUE;
}

/**	@fn	    Upload
*	@brief	上传录像
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 录像信息
*	@return	BOOL
*/
BOOL CCloud::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    HCS_RecordPlan struRecPlan;
    memset(&struRecPlan, 0, sizeof(HCS_RecordPlan));
    //设置编码器ID
//     std::stringstream ss;
//     ss << struRecord.strDvrIp << "_" << struRecord.nDvrPort << "_" << struRecord.nChannelID;
//     std::string strCameraID;
//     ss >> strCameraID;
    struRecPlan.stream_mode = 0;// 取流类型[0-设备取流，1-URL取流]
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.ip, HCS_DEVICE_IP_LEN, 
        struRecord.strDvrIp.c_str(), strlen(struRecord.strDvrIp.c_str()));//DVR设备IP
    struRecPlan.stream_info.stream_dev.device.endpoint.port = struRecord.nDvrPort;//DVR设备端口
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.username, HCS_USERNAME_LEN, 
        struRecord.strDvrUser.c_str(), strlen(struRecord.strDvrUser.c_str()));//DVR设备用户名
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.password, HCS_PASSWORD_LEN, 
        struRecord.strDvrPwd.c_str(), strlen(struRecord.strDvrPwd.c_str()));//DVR设备密码
    struRecPlan.stream_info.stream_dev.channel = struRecord.nChannelID;//DVR设备通道号
    memcpy_s(struRecPlan.camera_id, HCS_CAMERA_ID_LEN, 
        struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str()));//编码器ID
    memcpy_s(struRecPlan.camera_name, HCS_CAMERA_ID_LEN, 
        struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str()));//编码器名称
    sprintf_s(struRecPlan.pool_id, HCS_POOL_ID_LEN, "%d", 
        struRecord.struStorage.nPoolId);//资源池ID
    //设置录像计划空模板
    int nRes = HCS_SDK_SetRecPlan(lLoginID, &struRecPlan);
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_SetRecPlan failed.cameraID:%s, record name:%s, error[%d]", 
            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), nRes);
        return FALSE;
    }
    //设置录像信息
    HCS_HisRecInfo struRecordInfo;
    memset(&struRecordInfo, 0, sizeof(HCS_HisRecInfo));

    //关联DVR的唯一标识
    memcpy_s(struRecordInfo.camera_id, HCS_CAMERA_ID_LEN, 
        struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str()));
    struRecordInfo.rec_type = 65535;  //录像类型,0表示定时录像，其他类型不做限制
    struRecordInfo.detail.enable = 0;   //不使用取流信息

    //录像开始时间
    struRecordInfo.begin_time.year = struRecord.struRecStartTime.dwYear;     
    struRecordInfo.begin_time.month = struRecord.struRecStartTime.dwMonth;      
    struRecordInfo.begin_time.day = struRecord.struRecStartTime.dwDay;     
    struRecordInfo.begin_time.hour = struRecord.struRecStartTime.dwHour;
    struRecordInfo.begin_time.minute = struRecord.struRecStartTime.dwMinute;
    struRecordInfo.begin_time.second = struRecord.struRecStartTime.dwSecond;
    //录像结束时间
    struRecordInfo.end_time.year = struRecord.struRecStopTime.dwYear;
    struRecordInfo.end_time.month = struRecord.struRecStopTime.dwMonth;
    struRecordInfo.end_time.day = struRecord.struRecStopTime.dwDay;
    struRecordInfo.end_time.hour = struRecord.struRecStopTime.dwHour;
    struRecordInfo.end_time.minute = struRecord.struRecStopTime.dwMinute;
    struRecordInfo.end_time.second = struRecord.struRecStopTime.dwSecond;
//     CLOUD_LOG_INFO("RecStartTime[%d-%d-%d %d:%d:%d], RecStopTime[%d-%d-%d %d:%d:%d]",
//         pStartTime->tm_year, pStartTime->tm_mon, pStartTime->tm_mday, pStartTime->tm_hour, pStartTime->tm_min, pStartTime->tm_sec,
//         pStopTime->tm_year, pStopTime->tm_mon, pStopTime->tm_mday, pStopTime->tm_hour, pStopTime->tm_min, pStopTime->tm_sec);
    //下发录像手动补录任务
    char szUrl[1024] = {0};
    unsigned long long lTaskID = 0;
    nRes = HCS_SDK_HistoryRecord(lLoginID, &struRecordInfo, sizeof(szUrl), szUrl, &lTaskID);
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_HistoryRecord failed.cameraID:%s, record name:%s, error[%d]", 
            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), nRes);
        return FALSE;
    }
    struRecord.nTaskID = (int)lTaskID;
    CLOUD_LOG_INFO("Upload succeed.cameraID:%s, record name:%s, task id:%d", 
        struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), lTaskID);
    return TRUE;
}

/**	@fn	    GetUploadStatus
*	@brief	获取上传状态
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 录像信息
*	@param  [out] struRecord.recordStatus -- 录像状态
*	@return	BOOL
*/
BOOL CCloud::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    HCS_TaskInfo struStatus = {0};
    int nRes = HCS_SDK_HistoryRecordStatus(lLoginID, struRecord.nTaskID, &struStatus);
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_HistoryRecordStatus failed.cameraID:%s, record name:%s, nTaskID:%d, error[%d]", 
            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), struRecord.nTaskID, nRes);
        return FALSE;
    }
    switch (struStatus.task_status)
    {
    case 0: //未执行
        {
            struRecord.recordStatus = RECORDSTAT_UPLOADING;
        }
        break;
    case 1: //正常执行
        {
            struRecord.recordStatus = RECORDSTAT_UPLOADING;
            struRecord.nUploadPercent = struStatus.task_speed;
        }
        break;
    case 2: //异常
        {
            CLOUD_LOG_ERROR("status unnormal.cameraID:%s, record name:%s", 
                struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str());
            struRecord.recordStatus = RECORDSTAT_FAIL; 
        }
        break;
    case 3: //已完成
        {
            struRecord.recordStatus = RECORDSTAT_OVER;
        }
        break;
    default:
        {
            struRecord.recordStatus = RECORDSTAT_FAIL;
        }
        break;
    }
    CLOUD_LOG_INFO("GetUploadStatus succeed.cameraID:%s, record name:%s, taskID:%d, status:%d", 
        struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), struRecord.nTaskID, struStatus.task_status);
    return TRUE;
}

/**	@fn	    GetUploadRecordInfo
*	@brief	获取存储设备中已上传的录像信息
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 上传录像信息
*	@param  [out] struRecordParam -- 已上传到存储设备中的录像信息
*	@return	BOOL
*/
BOOL CCloud::GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam)
{
	HCS_Time stBeginTime = {0};
	stBeginTime.year = struRecord.struRecStartTime.dwYear;
	stBeginTime.month = struRecord.struRecStartTime.dwMonth;
	stBeginTime.day = struRecord.struRecStartTime.dwDay;
	stBeginTime.hour = struRecord.struRecStartTime.dwHour;
	stBeginTime.minute = struRecord.struRecStartTime.dwMinute;
	stBeginTime.second = struRecord.struRecStartTime.dwSecond;
	stBeginTime.millisecond = 0;

	HCS_Time stEndTime = {0};
	stEndTime.year = struRecord.struRecStopTime.dwYear;
	stEndTime.month = struRecord.struRecStopTime.dwMonth;
	stEndTime.day = struRecord.struRecStopTime.dwDay;
	stEndTime.hour = struRecord.struRecStopTime.dwHour;
	stEndTime.minute = struRecord.struRecStopTime.dwMinute;
	stEndTime.second = struRecord.struRecStopTime.dwSecond;
	stEndTime.millisecond = 0;
	// 开始查找.
	int nResult = 0;
	int nHandle = 0;
	int nCount = 0; 
	nResult = HCS_SDK_RecSearchStart(lLoginID, struRecord.strCameraID.c_str(), stBeginTime, stEndTime, "", HCS_SRLOCK_TYPE_All, &nCount, &nHandle);
	if (nResult != HCS_OK)
	{
		CLOUD_LOG_ERROR("CloudStream-HCS_SDK_RecSearchStart falid, m_nLogin:%I64d, CamID:%s, errCode=%d", lLoginID,  struRecord.strCameraID.c_str(), nResult);
		return FALSE;
	}

	// 更新文件至列表.
	for (int i = 0; i < nCount; i++)
	{
		int nStatus = 0;
		int nSearchResult = 0;
		HCS_RecordInfo stInfo;
		memset(&stInfo, 0, sizeof(HCS_RecordInfo));
		nSearchResult = HCS_SDK_RecSearchNext(lLoginID, nHandle, &stInfo, &nStatus);
		if (nSearchResult != HCS_OK)
		{
			HCS_SDK_RecSearchStop(lLoginID, nHandle);
			CLOUD_LOG_ERROR("CloudStream-HCS_SDK_RecSearchNext falid, m_nLogin:%I64d, CamID:%s, errCode=%d", lLoginID, struRecord.strCameraID.c_str(), nSearchResult);
			return FALSE;
		}
		if (0 == i)
		{
			//录像开始时间
			struRecordParam.struRecStartTime.dwYear   = stInfo.begin_time.year;
			struRecordParam.struRecStartTime.dwMonth  = stInfo.begin_time.month;
			struRecordParam.struRecStartTime.dwDay    = stInfo.begin_time.day;
			struRecordParam.struRecStartTime.dwHour   = stInfo.begin_time.hour;
			struRecordParam.struRecStartTime.dwMinute = stInfo.begin_time.minute;
			struRecordParam.struRecStartTime.dwSecond = stInfo.begin_time.second;
		}
		//录像结束时间
		struRecordParam.struRecStopTime.dwYear   = stInfo.end_time.year;
		struRecordParam.struRecStopTime.dwMonth  = stInfo.end_time.month;
		struRecordParam.struRecStopTime.dwDay    = stInfo.end_time.day;
		struRecordParam.struRecStopTime.dwHour   = stInfo.end_time.hour;
		struRecordParam.struRecStopTime.dwMinute = stInfo.end_time.minute;
		struRecordParam.struRecStopTime.dwSecond = stInfo.end_time.second;

		//录像大小
		struRecordParam.dwRecordLength += stInfo.size;
		if (2 == stInfo.lock) //0: 全部类型(包括锁定的录像和没有锁定的录像)、1: 没有锁定、2: 锁定
		{
			struRecordParam.byLockFlag = 1;
		}

	}

	HCS_SDK_RecSearchStop(lLoginID, nHandle);
	struRecordParam.dwRecordNum = nCount;
	CLOUD_LOG_INFO("CloudStream-查询文件成功,文件个数:%d. ", nCount);
	return TRUE;
}
