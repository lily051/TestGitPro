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
*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CCloud::Init()
{
    //�ƴ洢��ʼ��
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
*	@brief	�ͷ���Դ
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CCloud::Cleanup()
{
    //����ʼ��
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
*	@brief	��¼
*	@param  [in] struStorage -- �洢����Ϣ
*	@param  [out] struStorage -- ��¼���
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
*	@brief	�ǳ�
*	@param  [in] lLoginID -- ��¼���
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
*	@brief	�ϴ�¼��
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- ¼����Ϣ
*	@return	BOOL
*/
BOOL CCloud::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    HCS_RecordPlan struRecPlan;
    memset(&struRecPlan, 0, sizeof(HCS_RecordPlan));
    //���ñ�����ID
//     std::stringstream ss;
//     ss << struRecord.strDvrIp << "_" << struRecord.nDvrPort << "_" << struRecord.nChannelID;
//     std::string strCameraID;
//     ss >> strCameraID;
    struRecPlan.stream_mode = 0;// ȡ������[0-�豸ȡ����1-URLȡ��]
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.ip, HCS_DEVICE_IP_LEN, 
        struRecord.strDvrIp.c_str(), strlen(struRecord.strDvrIp.c_str()));//DVR�豸IP
    struRecPlan.stream_info.stream_dev.device.endpoint.port = struRecord.nDvrPort;//DVR�豸�˿�
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.username, HCS_USERNAME_LEN, 
        struRecord.strDvrUser.c_str(), strlen(struRecord.strDvrUser.c_str()));//DVR�豸�û���
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.password, HCS_PASSWORD_LEN, 
        struRecord.strDvrPwd.c_str(), strlen(struRecord.strDvrPwd.c_str()));//DVR�豸����
    struRecPlan.stream_info.stream_dev.channel = struRecord.nChannelID;//DVR�豸ͨ����
    memcpy_s(struRecPlan.camera_id, HCS_CAMERA_ID_LEN, 
        struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str()));//������ID
    memcpy_s(struRecPlan.camera_name, HCS_CAMERA_ID_LEN, 
        struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str()));//����������
    sprintf_s(struRecPlan.pool_id, HCS_POOL_ID_LEN, "%d", 
        struRecord.struStorage.nPoolId);//��Դ��ID
    //����¼��ƻ���ģ��
    int nRes = HCS_SDK_SetRecPlan(lLoginID, &struRecPlan);
    if (HCS_OK != nRes)
    {
        CLOUD_LOG_ERROR("call HCS_SDK_SetRecPlan failed.cameraID:%s, record name:%s, error[%d]", 
            struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str(), nRes);
        return FALSE;
    }
    //����¼����Ϣ
    HCS_HisRecInfo struRecordInfo;
    memset(&struRecordInfo, 0, sizeof(HCS_HisRecInfo));

    //����DVR��Ψһ��ʶ
    memcpy_s(struRecordInfo.camera_id, HCS_CAMERA_ID_LEN, 
        struRecord.strCameraID.c_str(), strlen(struRecord.strCameraID.c_str()));
    struRecordInfo.rec_type = 65535;  //¼������,0��ʾ��ʱ¼���������Ͳ�������
    struRecordInfo.detail.enable = 0;   //��ʹ��ȡ����Ϣ

    //¼��ʼʱ��
    struRecordInfo.begin_time.year = struRecord.struRecStartTime.dwYear;     
    struRecordInfo.begin_time.month = struRecord.struRecStartTime.dwMonth;      
    struRecordInfo.begin_time.day = struRecord.struRecStartTime.dwDay;     
    struRecordInfo.begin_time.hour = struRecord.struRecStartTime.dwHour;
    struRecordInfo.begin_time.minute = struRecord.struRecStartTime.dwMinute;
    struRecordInfo.begin_time.second = struRecord.struRecStartTime.dwSecond;
    //¼�����ʱ��
    struRecordInfo.end_time.year = struRecord.struRecStopTime.dwYear;
    struRecordInfo.end_time.month = struRecord.struRecStopTime.dwMonth;
    struRecordInfo.end_time.day = struRecord.struRecStopTime.dwDay;
    struRecordInfo.end_time.hour = struRecord.struRecStopTime.dwHour;
    struRecordInfo.end_time.minute = struRecord.struRecStopTime.dwMinute;
    struRecordInfo.end_time.second = struRecord.struRecStopTime.dwSecond;
//     CLOUD_LOG_INFO("RecStartTime[%d-%d-%d %d:%d:%d], RecStopTime[%d-%d-%d %d:%d:%d]",
//         pStartTime->tm_year, pStartTime->tm_mon, pStartTime->tm_mday, pStartTime->tm_hour, pStartTime->tm_min, pStartTime->tm_sec,
//         pStopTime->tm_year, pStopTime->tm_mon, pStopTime->tm_mday, pStopTime->tm_hour, pStopTime->tm_min, pStopTime->tm_sec);
    //�·�¼���ֶ���¼����
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
*	@brief	��ȡ�ϴ�״̬
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- ¼����Ϣ
*	@param  [out] struRecord.recordStatus -- ¼��״̬
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
    case 0: //δִ��
        {
            struRecord.recordStatus = RECORDSTAT_UPLOADING;
        }
        break;
    case 1: //����ִ��
        {
            struRecord.recordStatus = RECORDSTAT_UPLOADING;
            struRecord.nUploadPercent = struStatus.task_speed;
        }
        break;
    case 2: //�쳣
        {
            CLOUD_LOG_ERROR("status unnormal.cameraID:%s, record name:%s", 
                struRecord.strCameraID.c_str(), struRecord.strRecordName.c_str());
            struRecord.recordStatus = RECORDSTAT_FAIL; 
        }
        break;
    case 3: //�����
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
*	@brief	��ȡ�洢�豸�����ϴ���¼����Ϣ
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- �ϴ�¼����Ϣ
*	@param  [out] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
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
	// ��ʼ����.
	int nResult = 0;
	int nHandle = 0;
	int nCount = 0; 
	nResult = HCS_SDK_RecSearchStart(lLoginID, struRecord.strCameraID.c_str(), stBeginTime, stEndTime, "", HCS_SRLOCK_TYPE_All, &nCount, &nHandle);
	if (nResult != HCS_OK)
	{
		CLOUD_LOG_ERROR("CloudStream-HCS_SDK_RecSearchStart falid, m_nLogin:%I64d, CamID:%s, errCode=%d", lLoginID,  struRecord.strCameraID.c_str(), nResult);
		return FALSE;
	}

	// �����ļ����б�.
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
			//¼��ʼʱ��
			struRecordParam.struRecStartTime.dwYear   = stInfo.begin_time.year;
			struRecordParam.struRecStartTime.dwMonth  = stInfo.begin_time.month;
			struRecordParam.struRecStartTime.dwDay    = stInfo.begin_time.day;
			struRecordParam.struRecStartTime.dwHour   = stInfo.begin_time.hour;
			struRecordParam.struRecStartTime.dwMinute = stInfo.begin_time.minute;
			struRecordParam.struRecStartTime.dwSecond = stInfo.begin_time.second;
		}
		//¼�����ʱ��
		struRecordParam.struRecStopTime.dwYear   = stInfo.end_time.year;
		struRecordParam.struRecStopTime.dwMonth  = stInfo.end_time.month;
		struRecordParam.struRecStopTime.dwDay    = stInfo.end_time.day;
		struRecordParam.struRecStopTime.dwHour   = stInfo.end_time.hour;
		struRecordParam.struRecStopTime.dwMinute = stInfo.end_time.minute;
		struRecordParam.struRecStopTime.dwSecond = stInfo.end_time.second;

		//¼���С
		struRecordParam.dwRecordLength += stInfo.size;
		if (2 == stInfo.lock) //0: ȫ������(����������¼���û��������¼��)��1: û��������2: ����
		{
			struRecordParam.byLockFlag = 1;
		}

	}

	HCS_SDK_RecSearchStop(lLoginID, nHandle);
	struRecordParam.dwRecordNum = nCount;
	CLOUD_LOG_INFO("CloudStream-��ѯ�ļ��ɹ�,�ļ�����:%d. ", nCount);
	return TRUE;
}
