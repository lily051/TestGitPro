#include "stdafx.h"
#include "MultiFileUpload.h"
#include <sstream>
#include <string>
#include <iomanip>
#include "../util/StringConverter.h"
#include <HPR_Config.h>
#include "Config.h"
#include "../util/Guard.h"

SINGLETON_IMPLEMENT(CMultiFileUpload)//<单例相关实现

#define MAX_FAIL_TIME       10  //最大失败次数

CMultiFileUpload::CMultiFileUpload(void)
{
	m_bInit = false;
	m_bStart = false;
	m_bStopWorker = false;
	m_client = NULL;
	memset(fileData,0,sizeof(fileData));
	memset(&m_csFile,0,sizeof(m_csFile));
	for(int i = 0; i < MAX_THREAD_NUM; ++i)
	{
		hThread[i] = NULL;
	}
}

CMultiFileUpload::~CMultiFileUpload(void)
{
	try
	{
	}
	catch (...)
	{
	}
}

/**	@fn	    _MultiFileUploadThreadFun
*	@brief	多文件并发上传线程
*	@param  [in] pArguments指向CMultiFileUpload对象
*	@return	
*/
unsigned __stdcall _MultiFileUploadThreadFun( void* pArguments)
{
	CMultiFileUpload::GetSingletonPtr()->MultiFileUpload();
	_endthreadex(0);
	return 0;
}

/**	@fn	    Init
*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
*	@param  [in] 无
*	@param  [out] 无
*	@return	BOOL
*/
BOOL CMultiFileUpload::Init()
{
	InitializeCriticalSection(&m_csFile);
    return TRUE;
}

/**	@fn	    Fini
*	@brief	反初始化函数，释放资源等
*	@param  [in] 无
*	@param  [out] 无
*	@return	BOOL
*/
BOOL CMultiFileUpload::Fini()
{
	if (NULL != m_client)
	{
		ClientFinalize(m_client);
		m_client = NULL;
	}
	DeleteCriticalSection(&m_csFile);
    return TRUE;
}

/**	@fn	    Start
*	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
*          此函数应该尽快返回，不应该占用太多时间，必须小于30s
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CMultiFileUpload::Start()
{
	ULSERVER_LOG_INFO("CMultiFileUpload::Start in................");
	if (!m_bInit || m_bStart)
	{
		//还没有初始化
		return HPR_ERROR;
	}
	for (int i = 0; i < 10; ++i)
	{
		unsigned threadID = 0;
		if (NULL == hThread[i])
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, &_MultiFileUploadThreadFun, this, 0, &threadID);
			if (NULL == hThread[i])
			{
				ULSERVER_LOG_ERROR("Create _MultiFileUploadThreadFun Fail!!!");
				return HPR_ERROR;
			}
		}
	}

	ULSERVER_LOG_INFO("CMultiFileUpload::Start out................");
	m_bStart = true;

	return HPR_OK;
}

/**	@fn	    MultiFileUpload[线程函数]
*	@brief	上传录像
*	@param  [in] 无
*	@return	
*/
void CMultiFileUpload::MultiFileUpload()
{
	ULSERVER_LOG_INFO("MultiFileUpload........IN,ThreadId:%d", GetCurrentThreadId());
	while (!m_bStopWorker)
	{
		BOOL bHasFile = FALSE;
		Struct_RecordInfo fileInfo;
		do
		{
			CGuard guard(&m_csFile);
			if (!m_lstFile.empty())
			{
				fileInfo = m_lstFile.front();
				m_lstFile.pop_front();
				bHasFile = TRUE;
			}
		}while(0);
		//没找到数据
		if (!bHasFile)
		{
			Sleep(500);
			continue;
		}
		ULSERVER_LOG_INFO("file info(fileType:%d, fileName:%s, startTime:%s, stopTime:%s, filePath:%s)", 
			fileInfo.fileType, fileInfo.strRecordName.c_str(),fileInfo.strStartTime.c_str(),fileInfo.strEndTime.c_str(), 
			fileInfo.strFilePath.c_str());
        int fail = 0;
		while (fail < MAX_FAIL_TIME && !m_bStopWorker)
		{

			++fail;
		}

		for (int i = 0; i < CConfig::GetSingletonPtr()->m_nUploadInteval; ++i)
		{
			Sleep(1000);
		}
	}
	ULSERVER_LOG_INFO("MultiFileUpload........OUT,ThreadId:%d", GetCurrentThreadId());
}

/**	@fn	    Stop
*	@brief	停止函数
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CMultiFileUpload::Stop()
{
	ULSERVER_LOG_INFO("CMultiFileUpload::Stop in................");
	if (!m_bInit || m_bStart)
	{
		//还没有初始化
		return HPR_ERROR;
	}

	m_bStart = false;
	ULSERVER_LOG_INFO("CMultiFileUpload::Stop Out................");

	return HPR_OK;
}

/**	@fn	    Login
*	@brief	登录
*	@param  [in] struStorage -- 存储器信息
*	@return	BOOL -- 登录句柄
*/
LONG CMultiFileUpload::Login(const Struct_StorageConfig &struStorage)
{
	if (NULL == m_client)
	{
		char endpoint[30]={0};
		sprintf(endpoint, "%s:%d", struStorage.strIp.c_str(), struStorage.nPort);
		m_client = ClientInitialize(struStorage.strAccessKey.c_str(), struStorage.strSecretKey.c_str(), endpoint);
		//调用初始化信息函数
		if (NULL == m_client)
		{
			OBJECTCLOUD_LOG_ERROR("ClientInitialize  fail,IP(%s),Port(%d)", struStorage.strIp.c_str(), struStorage.nPort);
			return  -1;
		}
	}
    return 0;
}

/**	@fn	    Logout
*	@brief	登出
*	@param  [in] lLoginID -- 登录句柄
*	@return	BOOL
*/
BOOL CMultiFileUpload::Logout(LONG lLoginID)
{
    return TRUE;
}


/**	@fn	    Upload
*	@brief	上传录像
*	@param  [in] struStorage -- 存储器信息
*	@param  [in] struRecord -- 录像信息
*	@return	BOOL
*/
BOOL CMultiFileUpload::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
	if (NULL == m_client)
	{
		OBJECTCLOUD_LOG_ERROR("m_client == NULL");
		return FALSE;
	}
    if (FILE_TYPE_VIDEO == struRecord.fileType)
    {
		return UploadStreamFile(struRecord);
    } 
    else if (FILE_TYPE_AUDIO == struRecord.fileType)
    {
		return UploadNormalFile(struRecord);
    }
	else
	{
		OBJECTCLOUD_LOG_ERROR("unkown file type");
	}
    return FALSE;
}

/**	@fn	    UploadNormalFile
*	@brief	普通上传
*	@param  [in] struRecord -- 录像信息
*	@param  [out] struRecord.strCameraID -- 文件key
*	@return	BOOL
*/
BOOL CMultiFileUpload::UploadNormalFile(Struct_RecordInfo &struRecord)
{
	int  ret = -1;
	char streamHandle[64]={0};
	char objectKey[50]={0};
	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	long fileOffset = 0;
	FILE * fileHandle = NULL;
	int  readLength = 0;
	sprintf(objectKey, "%s_%4d%02d%02d%2d%02d%02d_%4d%02d%02d%2d%02d%02d",
		struRecord.strHost.c_str(),
		struRecord.struRecStartTime.dwYear,
		struRecord.struRecStartTime.dwMonth,
		struRecord.struRecStartTime.dwDay,
		struRecord.struRecStartTime.dwHour,
		struRecord.struRecStartTime.dwMinute,
		struRecord.struRecStartTime.dwSecond,
		struRecord.struRecStopTime.dwYear,
		struRecord.struRecStopTime.dwMonth,
		struRecord.struRecStopTime.dwDay,
		struRecord.struRecStopTime.dwHour,
		struRecord.struRecStopTime.dwMinute,
		struRecord.struRecStopTime.dwSecond);
	
	fileHandle = fopen(struRecord.strFilePath.c_str(), "rb+");
	if (fileHandle == NULL)
	{
		OBJECTCLOUD_LOG_ERROR("call fopen fail,filename(%s),host(%s),filePath(%s)", 
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str(),
			struRecord.strFilePath.c_str());
		return FALSE;
	}

	ret = PutObjectFromFile(m_client,struRecord.strBucket.c_str(),objectKey,fileHandle,NULL,"STANDARD",NULL);
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
*	@brief	流式上传
*	@param  [in] struRecord -- 录像信息
*	@param  [out] struRecord.strCameraID -- 文件key
*	@return	BOOL
*/
BOOL CMultiFileUpload::UploadStreamFile(Struct_RecordInfo &struRecord)
{
	int  ret = -1;
	char streamHandle[64]={0};
	char objectKey[50]={0};
	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	long fileOffset = 0;
	FILE * fileHandle = NULL;
	long long readLength = 0;
	sprintf(objectKey, "%s_%4d%02d%02d%2d%02d%02d_%4d%02d%02d%2d%02d%02d",
		struRecord.strHost.c_str(),
		struRecord.struRecStartTime.dwYear,
		struRecord.struRecStartTime.dwMonth,
		struRecord.struRecStartTime.dwDay,
		struRecord.struRecStartTime.dwHour,
		struRecord.struRecStartTime.dwMinute,
		struRecord.struRecStartTime.dwSecond,
		struRecord.struRecStopTime.dwYear,
		struRecord.struRecStopTime.dwMonth,
		struRecord.struRecStopTime.dwDay,
		struRecord.struRecStopTime.dwHour,
		struRecord.struRecStopTime.dwMinute,
		struRecord.struRecStopTime.dwSecond);
	//查询该objectKey曾经是否上传过文件并获得上传大小
	ret = -1;
	ret = ListObjectSummary(m_client, struRecord.strBucket.c_str(), objectKey, &listCount, &pSummary);
	if (0 != ret)
	{
		OBJECTCLOUD_LOG_ERROR("call ListObjectSummary fail,errorcode(%d),bucket(%s),objectKey(%s),filename(%s),host(%s)", 
			ret, 
			struRecord.strBucket.c_str(),
			objectKey,
			struRecord.strRecordName.c_str(), 
			struRecord.strHost.c_str());
		FreeHCSObjectSummary(m_client, &listCount, &pSummary);
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
		FreeHCSObjectSummary(m_client, &listCount, &pSummary);
		return FALSE;
	}
	else
	{
		OBJECTCLOUD_LOG_INFO("listCount = %d", listCount);
	}
	FreeHCSObjectSummary(m_client, &listCount, &pSummary);

	ret = -1;
	ret = OpenStream(m_client,struRecord.strBucket.c_str(),objectKey,streamHandle);
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
		CloseStream(m_client,struRecord.strBucket.c_str(),objectKey,streamHandle);
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
			CloseStream(m_client,struRecord.strBucket.c_str(),objectKey,streamHandle);
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
			ret = UploadStreamData(m_client,struRecord.strBucket.c_str(),objectKey,streamHandle,fileData,readLength);
			if(HCS_OK != ret)
			{
				OBJECTCLOUD_LOG_ERROR("call UploadStreamData fail,errorcode(%d),bucket(%s),objectKey(%s),filename(%s),host(%s)", 
					ret, 
					struRecord.strBucket.c_str(),
					objectKey,
					struRecord.strRecordName.c_str(), 
					struRecord.strHost.c_str());
				fclose(fileHandle);
				CloseStream(m_client,struRecord.strBucket.c_str(),objectKey,streamHandle);
				return FALSE;
			}
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
	CloseStream(m_client,struRecord.strBucket.c_str(),objectKey,streamHandle);
    struRecord.strCameraID = objectKey;
    return TRUE; 
}

/**	@fn	    GetUploadStatus
*	@brief	获取上传状态
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 录像信息
*	@param  [out] struRecord.recordStatus -- 录像状态
*	@return	BOOL
*/
BOOL CMultiFileUpload::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    return FALSE; 
}

