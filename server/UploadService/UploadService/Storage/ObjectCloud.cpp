#include "stdafx.h"
#include "ObjectCloud.h"
#include <sstream>
#include <string>
#include <iomanip>
#include "../../util/StringConverter.h"
#include <time.h>
#include "../Config.h"

CObjectCloud::CObjectCloud(void)
{
}

CObjectCloud::~CObjectCloud(void)
{
	try
	{
		std::map<std::string, HCSClient *>::iterator iterHCSClient;
		for (iterHCSClient = m_mapHCSClient.begin(); iterHCSClient != m_mapHCSClient.end(); ++iterHCSClient)
		{
			ClientFinalize(iterHCSClient->second);
			OBJECTCLOUD_LOG_INFO("ClientFinalize  succ,IP(%s)", iterHCSClient->first.c_str());

		}
		m_mapHCSClient.clear();
	}
	catch (...)
	{
	}
}

/**	@fn	    Init
*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CObjectCloud::Init()
{
	m_mapHCSClient.clear();
	memset(fileData,0,sizeof(fileData));
    return TRUE;
}

/**	@fn	    Cleanup
*	@brief	�ͷ���Դ
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CObjectCloud::Cleanup()
{
	std::map<std::string, HCSClient *>::iterator iterHCSClient;
	for (iterHCSClient = m_mapHCSClient.begin(); iterHCSClient != m_mapHCSClient.end(); ++iterHCSClient)
	{
		ClientFinalize(iterHCSClient->second);
		OBJECTCLOUD_LOG_INFO("ClientFinalize  succ,IP(%s)", iterHCSClient->first.c_str());
	}
	m_mapHCSClient.clear();
    return TRUE;
}

/**	@fn	    Login
*	@brief	��¼
*	@param  [in] struStorage -- �洢����Ϣ
*	@return	BOOL -- ��¼���
*/
LONG CObjectCloud::Login(const Struct_StorageConfig &struStorage)
{
	std::map<std::string, HCSClient *>::iterator iterHCSClient = m_mapHCSClient.find(struStorage.strIp);
	if (iterHCSClient == m_mapHCSClient.end())
	{
		char endpoint[30]={0};
		HCSClient* pClient = NULL;
		sprintf(endpoint, "%s:%d", struStorage.strIp.c_str(), struStorage.nPort);
		pClient = ClientInitialize(struStorage.strAccessKey.c_str(), struStorage.strSecretKey.c_str(), endpoint);
		//���ó�ʼ����Ϣ����
		if (NULL == pClient)
		{
			OBJECTCLOUD_LOG_ERROR("ClientInitialize  fail,IP(%s),Port(%d)", struStorage.strIp.c_str(), struStorage.nPort);
			return  STORAGE_LOGIN_FAILED;
		}
		OBJECTCLOUD_LOG_INFO("ClientInitialize  succ,IP(%s),Port(%d)", struStorage.strIp.c_str(), struStorage.nPort);
		m_mapHCSClient.insert(make_pair(struStorage.strIp, pClient));	
	}
    return 0;
}

/**	@fn	    Logout
*	@brief	�ǳ�
*	@param  [in] lLoginID -- ��¼���
*	@return	BOOL
*/
BOOL CObjectCloud::Logout(LONG lLoginID)
{
    return TRUE;
}


/**	@fn	    Upload
*	@brief	�ϴ�¼��
*	@param  [in] struStorage -- �洢����Ϣ
*	@param  [in] struRecord -- ¼����Ϣ
*	@return	BOOL
*/
BOOL CObjectCloud::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
	OBJECTCLOUD_LOG_INFO("CConfig::GetSingletonPtr()->m_nUploadSpeed=%d", CConfig::GetSingletonPtr()->m_nUploadSpeed);
	m_oSpeedControl.SetSpeed(CConfig::GetSingletonPtr()->m_nUploadSpeed);
	std::map<std::string, HCSClient *>::iterator iterHCSClient = m_mapHCSClient.find(struRecord.struStorage.strIp);
	if (iterHCSClient == m_mapHCSClient.end())
	{
		OBJECTCLOUD_LOG_ERROR("UnInitialize,IP:%s", struRecord.struStorage.strIp.c_str());
		return FALSE;
	}
	if (NULL == iterHCSClient->second)
	{
		OBJECTCLOUD_LOG_ERROR("HCSClient * == NULL,IP:%s", struRecord.struStorage.strIp.c_str());
		return FALSE;
	}
    if (FILE_TYPE_VIDEO == struRecord.fileType)
    {
		return UploadStreamFile(iterHCSClient->second, struRecord);
    } 
    else if (FILE_TYPE_AUDIO == struRecord.fileType)
    {
		return UploadNormalFile(iterHCSClient->second, struRecord);
    }
	else
	{
		OBJECTCLOUD_LOG_ERROR("unkown file type");
	}
    return FALSE;
}

/**	@fn	    UploadNormalFile
*	@brief	��ͨ�ϴ�
*	@param  [in] pClient    -- �ƴ洢��ʼ�����
*	@param  [in] struRecord -- ¼����Ϣ
*	@param  [out] struRecord.strCameraID -- �ļ�key
*	@return	BOOL
*/
BOOL CObjectCloud::UploadNormalFile(HCSClient *pClient, Struct_RecordInfo &struRecord)
{
	int  ret = -1;
	char streamHandle[64]={0};
	char objectKey[60]={0};
	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	long fileOffset = 0;
	FILE * fileHandle = NULL;
	int  readLength = 0;
	sprintf(objectKey, "%s_%s_%d_%d",            //����洢Key������Ψһ��ʶ�ļ�
		struRecord.strHost.c_str(),
		struRecord.strStartTime.c_str(),
		struRecord.fileType,
		struRecord.nID);
	
	fileHandle = fopen(struRecord.strFilePath.c_str(), "rb+");
	if (fileHandle == NULL)
	{
		OBJECTCLOUD_LOG_ERROR("call fopen fail,filename(%s),host(%s),filePath(%s)", 
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str(),
			struRecord.strFilePath.c_str());
		return FALSE;
	}

	ret = PutObjectFromFile(pClient,struRecord.strBucket.c_str(),objectKey,fileHandle,NULL,"STANDARD",NULL);
	fclose(fileHandle);
	if(HCS_OK != ret)
	{
		OBJECTCLOUD_LOG_ERROR("call PutObjectFromFile fail,errorcode(%d),bucket(%s),objectKey(%s),filename(%s),host(%s)", 
			ret, 
			struRecord.strBucket.c_str(),
			objectKey,
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str());
		return FALSE;
	}
    struRecord.strCameraID = objectKey;
    return TRUE;
}

/**	@fn	    UploadStreamFile
*	@brief	��ʽ�ϴ�
*	@param  [in] pClient    -- �ƴ洢��ʼ�����
*	@param  [in] struRecord -- ¼����Ϣ
*	@param  [out] struRecord.strCameraID -- �ļ�key
*	@return	BOOL
*/
BOOL CObjectCloud::UploadStreamFile(HCSClient *pClient, Struct_RecordInfo &struRecord)
{
	int  ret = -1;
	char streamHandle[64]={0};
	char objectKey[60]={0};
	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	long fileOffset = 0;
	FILE * fileHandle = NULL;
	long long readLength = 0;
	sprintf(objectKey, "%s_%4d-%02d-%02d_%02d:%02d:%02d_%d_%d",            //����洢Key������Ψһ��ʶ�ļ�
		struRecord.strHost.c_str(),
		struRecord.struRecStartTime.dwYear,
		struRecord.struRecStartTime.dwMonth,
		struRecord.struRecStartTime.dwDay,
		struRecord.struRecStartTime.dwHour,
		struRecord.struRecStartTime.dwMinute,
		struRecord.struRecStartTime.dwSecond,
		struRecord.fileType,
		struRecord.nID);
	//��ѯ��objectKey�����Ƿ��ϴ����ļ�������ϴ���С
	ret = ListObjectSummary(pClient, struRecord.strBucket.c_str(), objectKey, &listCount, &pSummary);
	if (0 != ret)
	{
		OBJECTCLOUD_LOG_ERROR("call ListObjectSummary fail,errorcode(%d),bucket(%s),objectKey(%s),filename(%s),host(%s)", 
			ret, 
			struRecord.strBucket.c_str(),
			objectKey,
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str());
		FreeHCSObjectSummary(pClient, &listCount, &pSummary);
		return FALSE;
	}
	if (listCount == 1)
	{
		fileOffset = pSummary->size;
		OBJECTCLOUD_LOG_INFO("listCount = %d", listCount);
	}
	else if (listCount > 1)
	{
		OBJECTCLOUD_LOG_ERROR("too many files(%d) uploaded in the same objectKey(%s),filename(%s),host(%s)", 
			listCount,
			objectKey, 
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str());
		FreeHCSObjectSummary(pClient, &listCount, &pSummary);
		return FALSE;
	}
	else
	{
		OBJECTCLOUD_LOG_INFO("listCount = %d", listCount);
	}
	FreeHCSObjectSummary(pClient, &listCount, &pSummary);

	ret = -1;
	ret = OpenStream(pClient,struRecord.strBucket.c_str(),objectKey,streamHandle);
	if(HCS_OK != ret)
	{
		OBJECTCLOUD_LOG_ERROR("call OpenStream fail,errorcode(%d),bucket(%s),objectKey(%s),filename(%s),host(%s)", 
			ret, 
			struRecord.strBucket.c_str(),
			objectKey,
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str());
		return FALSE;
	}

	fileHandle = fopen(struRecord.strFilePath.c_str(), "rb+");
	if (fileHandle == NULL)
	{
		OBJECTCLOUD_LOG_ERROR("call fopen fail,filename(%s),host(%s),filePath(%s)", 
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str(),
			struRecord.strFilePath.c_str());
		CloseStream(pClient,struRecord.strBucket.c_str(),objectKey,streamHandle);
		return FALSE;
	}

	if (fileOffset > 0)
	{
		OBJECTCLOUD_LOG_INFO("fileOffset = %d", fileOffset);
		ret = -1;
		ret = fseek(fileHandle, fileOffset, 0);
		if (0 != ret)
		{
			OBJECTCLOUD_LOG_ERROR("call fseek fail,errorcode(%d),filename(%s),host(%s),filePath(%s)", 
				ret, 
				struRecord.strRecordName.c_str(), 
				struRecord.strHost.c_str(),
				struRecord.strFilePath.c_str());
			fclose(fileHandle);
			CloseStream(pClient,struRecord.strBucket.c_str(),objectKey,streamHandle);
			return FALSE;
		}
	}

	while(true)
	{
		readLength = 0;
		memset(fileData,0,sizeof(fileData));
		readLength = fread(fileData,1,WRITE_LENGTH,fileHandle);
		if(readLength != 0)
		{
			ret = -1;
			ret = UploadStreamData(pClient,struRecord.strBucket.c_str(),objectKey,streamHandle,fileData,readLength);
			if(HCS_OK != ret)
			{
				OBJECTCLOUD_LOG_ERROR("call UploadStreamData fail,errorcode(%d),bucket(%s),objectKey(%s),filename(%s),host(%s)", 
					ret, 
					struRecord.strBucket.c_str(),
					objectKey,
					struRecord.strRecordName.c_str(), 
					struRecord.strHost.c_str());
				fclose(fileHandle);
				CloseStream(pClient,struRecord.strBucket.c_str(),objectKey,streamHandle);
				return FALSE;
			}
			m_oSpeedControl.SpeedControl(readLength);
			/*std::stringstream ss;
			for (int i = 0; i < readLength; ++i)
			{
				int tm = fileData[i];
				ss << std::hex << std::setw(2) <<std::setfill('0') << tm;
				ss<< " ";
			}*/
			//OBJECTCLOUD_LOG_INFO("write lenth = %d", readLength);
			//OBJECTCLOUD_LOG_INFO("write content = %s", ss.str().c_str());

		}
		else
		{
			//OBJECTCLOUD_LOG_INFO("write lenth = %d", readLength);
			break;
		}
	}
	fclose(fileHandle);
	CloseStream(pClient,struRecord.strBucket.c_str(),objectKey,streamHandle);
    struRecord.strCameraID = objectKey;
	GetRecordTime(pClient, struRecord.strBucket, objectKey, struRecord.strStartTime, struRecord.strEndTime);
    return TRUE; 
}

/**	@fn	    GetRecordTime
*	@brief	��ȡ���ϴ�¼��Ŀ�ʼ/����ʱ��
*	@param  [in] pClient    -- �ƴ洢��ʼ�����
*	@param  [in] strBucket -- ��Դ��
*	@param  [in] strObjectKey -- �ļ�key
*	@param  [out] strStartTime -- ¼��ʼʱ��
*	@param  [out] strEndTime -- ¼�����ʱ��
*	@return	BOOL
*/
BOOL CObjectCloud::GetRecordTime(HCSClient *pClient, std::string strBucket, std::string strObjectKey, std::string &strStartTime, std::string &strEndTime)
{
	//time_t iStartTime = GetTimeStamp(strStartTime);
	//time_t iStopTime  = GetTimeStamp(strEndTime);
	HCSStreamTimeSegmentListEx *seg_list = NULL;
	int result = QueryObjectTimesegmentEx(pClient, strBucket.c_str(), strObjectKey.c_str(), 0, 999999999999, &seg_list, 1);
	if (0 != result)
	{
		OBJECTCLOUD_LOG_ERROR("GetRecordTime-QueryObjectTimesegmentEx fail�� ErrorCode = %d", result);
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}

	if (((seg_list) && (seg_list->segment_count == 0)) || (seg_list == NULL))
	{
		OBJECTCLOUD_LOG_ERROR("GetRecordTime-QueryObjectTimesegmentEx  list is empty!");
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}
	if (1 != seg_list->segment_count)
	{
		OBJECTCLOUD_LOG_ERROR("GetRecordTime-QueryObjectTimesegmentEx  ListNum != 1, ListNum = %d", seg_list->segment_count);
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}
	char szStartTime[32] = {0};
	char szStopTime[32] = {0};
	struct tm stStartTime = {0};
	struct tm stStopTime = {0};
	localtime_s(&stStartTime, &seg_list->segment_list[0].start_time);
	localtime_s(&stStopTime, &seg_list->segment_list[0].end_time);
	//¼��ʼʱ��
	sprintf(szStartTime, "%4d-%02d-%02d %2d:%02d:%02d",     
		stStartTime.tm_year + 1900,
		stStartTime.tm_mon + 1,
		stStartTime.tm_mday,
		stStartTime.tm_hour,
		stStartTime.tm_min,
		stStartTime.tm_sec);
	//¼��ʼʱ��
	sprintf(szStopTime, "%4d-%02d-%02d %2d:%02d:%02d",     
		stStopTime.tm_year + 1900,
		stStopTime.tm_mon + 1,
		stStopTime.tm_mday,
		stStopTime.tm_hour,
		stStopTime.tm_min,
		stStopTime.tm_sec);
	strStartTime = szStartTime;
    strEndTime   = szStopTime;
	FreeStreamTimeSegmentListEx(pClient, &seg_list);
	return TRUE;
}

/**	@fn	    GetTimeStamp
*	@brief	�õ�ʱ���
*	@param  [in] strTime -- ʱ���ִ�
*	@return	time_t
*/
time_t CObjectCloud::GetTimeStamp(std::string strTime)
{
	DWORD	dwYear;					//��
	DWORD	dwMonth;				//��
	DWORD	dwDay;					//��
	DWORD	dwHour;					//ʱ
	DWORD	dwMinute;				//��
	DWORD	dwSecond;				//��
	sscanf(strTime.c_str(), "%d-%02d-%02d %02d:%02d:%02d", &dwYear, &dwMonth, &dwDay,
		&dwHour, &dwMinute, &dwSecond);

	struct tm    stTime = {0};
	stTime.tm_year = dwYear - 1900;         //struct tm ��yearΪ����1900������
	stTime.tm_mon  = dwMonth - 1; //struct tm�·ݴ�0��ʼ ���Լ�һ
	stTime.tm_mday = dwDay;
	stTime.tm_hour = dwHour;
	stTime.tm_min  = dwMinute;
	stTime.tm_sec  = dwSecond;
	return mktime(&stTime);
}

/**	@fn	    GetUploadStatus
*	@brief	��ȡ�ϴ�״̬
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- ¼����Ϣ
*	@param  [out] struRecord.recordStatus -- ¼��״̬
*	@return	BOOL
*/
BOOL CObjectCloud::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    return FALSE; 
}

/**	@fn	    GetUploadRecordInfo
*	@brief	��ȡ�洢�豸�����ϴ���¼����Ϣ
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- �ϴ�¼����Ϣ
*	@param  [out] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
*	@return	BOOL
*/
BOOL CObjectCloud::GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam)
{
	return TRUE;
}

