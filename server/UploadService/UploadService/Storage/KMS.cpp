#include "stdafx.h"
#include "KMS.h"
#include "../../lib/curl/curl.h"
#include "../../lib/jsoncpp/include/json.h"
#include "../util/fclib/fcTchar.h"

#define CURL_CONNECT_TIMEOUT 3
#define CURL_TIMEOUT         3
CKMS::CKMS(void)
{
}

CKMS::~CKMS(void)
{
}

/**	@fn	    Init
*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
*	@param  [in] 无
*	@param  [out] 无
*	@return	BOOL
*/
BOOL CKMS::Init()
{
    //初始化curl库
    curl_global_init(CURL_GLOBAL_ALL);
    return TRUE;
}

/**	@fn	    Cleanup
*	@brief	释放资源
*	@param  [in] 无
*	@param  [out] 无
*	@return	BOOL
*/
BOOL CKMS::Cleanup()
{
    //反初始化curl库
    curl_global_cleanup();
    return TRUE;
}

/**	@fn	    Login
*	@brief	登录
*	@param  [in] struStorage -- 存储器信息
*	@return	BOOL -- 登录句柄
*/
LONG CKMS::Login(const Struct_StorageConfig &struStorage)
{
    return 0;
}

/**	@fn	    Logout
*	@brief	登出
*	@param  [in] lLoginID -- 登录句柄
*	@return	BOOL
*/
BOOL CKMS::Logout(LONG lLoginID)
{
    return TRUE;
}

//回调函数：获取返回结果
size_t OnCallBackGetHttpInfo(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{
    //返回结果由lpVoid传出
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        KMS_LOG_ERROR("str or buffer is NULL...");
        return 0;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);

    return nmemb * size;   
}

//回调函数：获取文件上传进度
int assetsManagerProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    if (NULL == ptr)
    {
        KMS_LOG_ERROR("ptr == NULL");
        return -1;
    }
    int nUploaded = 0;
    if (0 == totalToUpLoad)
    {
        nUploaded = 0;
    }
    else
    {
        nUploaded = (int)(nowUpLoaded / totalToUpLoad) * 100; 
    }   
    if (100 == nUploaded)
    {
        KMS_LOG_INFO("upload succeed.");
    }

    return 0;
}

/**	@fn	    Upload
*	@brief	上传录像
*	@param  [in] struStorage -- 存储器信息
*	@param  [in] struRecord -- 录像信息
*	@return	BOOL
*/
BOOL CKMS::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    //获取KMS的Token信息
    std::string strToken;
    if (!getHttpToken(struRecord.struStorage, strToken))
    {
        KMS_LOG_ERROR("getHttpToken failed.kms ip:%s, port:%d", struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort);
        return FALSE;
    }
    //上传文件
    std::string strFileID;
    if (!uploadFile(strToken, struRecord.struStorage, struRecord.strFilePath, strFileID))
    {
        KMS_LOG_ERROR("uploadFile failed.kms ip:%s, port:%d.filepath:%s", 
            struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort, struRecord.strFilePath.c_str());
        return FALSE;
    }
    struRecord.strCameraID = strFileID;
    //获取文件上传后的RTSP路径
    std::string strRtspPath;
	std::string strStartTime;
	std::string strEndTime;
    if (!getRtspPath(strToken, struRecord.struStorage, strFileID, strRtspPath, strStartTime, strEndTime))
    {
        KMS_LOG_ERROR("getRtspPath failed.kms ip:%s, port:%d.filepath:%s", 
            struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort, struRecord.strFilePath.c_str());
        return FALSE;
    }
    struRecord.strRtspPath = strRtspPath;
    //获取该文件上传后在KMS存储的大小
    int nFileSize = 0;
    if (!getFileSize(strToken, struRecord.struStorage, strFileID, nFileSize))
    {
        KMS_LOG_ERROR("getFileSize failed.kms ip:%s, port:%d.filepath:%s", 
            struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort, struRecord.strFilePath.c_str());
        return FALSE;
    }
    struRecord.nFileSize = nFileSize;
	struRecord.strStartTime = strStartTime;
	struRecord.strEndTime = strEndTime;
    return TRUE; 
}

/**	@fn	    GetUploadStatus
*	@brief	获取上传状态
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 录像信息
*	@param  [out] struRecord.recordStatus -- 录像状态
*	@return	BOOL
*/
BOOL CKMS::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    return FALSE; 
}

/**	@fn	    GetUploadRecordInfo
*	@brief	获取存储设备中已上传的录像信息
*	@param  [in] lLoginID -- 登录句柄
*	@param  [in] struRecord -- 上传录像信息
*	@param  [out] struRecordParam -- 已上传到存储设备中的录像信息
*	@return	BOOL
*/
BOOL CKMS::GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam)
{
	return TRUE;
}

/** @fn getHttpToken
*  @brief  向KMS获取Token认证信息
*  @param [in]struKMSInfo -- KMS的配置信息
*  @param [out]strToken -- 获取到的token信息
*  @return BOOL
*/
BOOL CKMS::getHttpToken(const Struct_StorageConfig &struKMSInfo, std::string &strToken)
{
    std::string strResponse;
    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        KMS_LOG_ERROR("curl is null...");
        return FALSE;  
    }  

    char strURL[200] = {0};
    sprintf_s(strURL, sizeof(strURL), "http://%s:%d/kms/services/rest/operationService/getTokenInfo?username=%s&&password=%s", 
        fcA2U(struKMSInfo.strIp.c_str()), struKMSInfo.nPort, fcA2U(struKMSInfo.strUser.c_str()), fcA2U(struKMSInfo.strPwd.c_str()));
    curl_easy_setopt(curl, CURLOPT_URL, strURL);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL); 
    //设置回调
    //设置返回值，由回调函数传出
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);      
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

    //设置超时时间
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CURL_CONNECT_TIMEOUT);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, CURLOPT_TIMEOUT);  
    res = curl_easy_perform(curl);  
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        KMS_LOG_ERROR("curl_easy_perform failed.");
        return FALSE;
    }
    KMS_LOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
    //{"node":"10.16.38.27:8080","ret":0,"msg":"操作成功","tokenError":false,"token":"807d6bf90000014f60ecfe434f42a54555"}
    //调用jsoncpp库解析返回的内容
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            strToken = root["token"].asString();
        }
        else
        {
            std::string strMsg = root["msg"].asString();
            KMS_LOG_ERROR("getToken failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
            return FALSE;
        }
    }
    else
    {
        KMS_LOG_ERROR("parse token info failed...");
        return FALSE;
    }
    KMS_LOG_ERROR("strToken is %s", fcU2A(strToken.c_str())); 
    return TRUE;
}

/** @fn uploadFile
*  @brief  向KMS上传文件
*  @param [in]strToken -- KMS的token认证信息
*  @param [in]struKMSInfo -- KMS的配置信息
*  @param [in]filePath -- 上传文件的路径
*  @param [out]strFileId -- KMS返回的文件id
*  @return BOOL
*/
BOOL CKMS::uploadFile(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &filePath, std::string &strFileId)
{
    std::string strResponse;
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";

    char szFilePath[200] = {0};
    sprintf_s(szFilePath, sizeof(szFilePath), "%s", filePath.c_str());
    curl_formadd(&formpost,
        &lastptr,
        CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, szFilePath,
        CURLFORM_CONTENTTYPE, "application/octet-stream",
        CURLFORM_END);

    curl_formadd(&formpost,
        &lastptr,
        CURLFORM_COPYNAME, "submit",
        CURLFORM_COPYCONTENTS, "提交",
        CURLFORM_END);

    headerlist = curl_slist_append(headerlist, buf);

    CURLcode res;  
    CURL* curl = curl_easy_init();
    if(NULL == curl)  
    {  
        ULSERVER_LOG_ERROR("curl is null...");
        return false;  
    }  
    char strURL[200] = {0};
    sprintf_s(strURL, sizeof(strURL), "http://%s:%d/kms/services/rest/uploadService/httpFileUpload?token=%s",
        struKMSInfo.strIp.c_str(), struKMSInfo.nPort, strToken.c_str());
    ULSERVER_LOG_INFO("strURL is %s", strURL);
    curl_easy_setopt(curl, CURLOPT_URL, strURL);  

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);//设为false 下面才能设置进度响应函数
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)this);//数据传输的对象
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, assetsManagerProgressFunc);//进度响应函数


    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        //错误输出
        ULSERVER_LOG_ERROR("curl_easy_perform failed.res:%d", (int)res);
        curl_easy_cleanup(curl);  
        curl_formfree(formpost);
        return false;
    }
    curl_easy_cleanup(curl);  
    curl_formfree(formpost);

    ULSERVER_LOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
    //     {
    //         "ret": 0,
    //             "msg": "操作成功",
    //             "id": "a946e65a56a3406585ad1547f7df6fbd",
    //             "docMd5": "499527a2e3f6c2ebcccb0fc52336c279"，
    //             "url": "/kms/services/rest/dataInfoService/downloadFile?id=ae431c644eaf4024aa0ad92908317f45",
    //             "thumbUrl": "/kms/services/rest/CRL/getThumbnail?id=ae431c644eaf4024aa0ad92908317f45",
    //             "beginTime": 1311131696000,
    //             "endTime": 1311131701000
    //     }

    //调用jsoncpp库解析返回的内容
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            strFileId = root["id"].asString();
        }
        else
        {
            std::string strMsg = root["msg"].asString();
            KMS_LOG_ERROR("uploadFile failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
            return FALSE;
        }
    }
    else
    {
        KMS_LOG_ERROR("parse upload info failed...");
        return FALSE;
    }
    KMS_LOG_INFO("strFileId is %s", fcU2A(strFileId.c_str())); 
    return TRUE;
}

/** @fn getRtspPath
*  @brief  向KMS上传文件
*  @param [in]strToken -- KMS的token认证信息
*  @param [in]struKMSInfo -- KMS的配置信息
*  @param [in]strFileId -- 上传文件的id
*  @param [out]strRtspPath -- 该文件的RTSP播放路径
*  @return bool
*/
BOOL CKMS::getRtspPath(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &strFileId, std::string &strRtspPath, std::string &strStartTime, std::string &strEndTime)
{
    std::string strResponse;
    CURL* curl = curl_easy_init();
    if(NULL == curl)  
    {  
        KMS_LOG_ERROR("curl is null...");
        return FALSE;  
    }  
    CURLcode res; 
    char szURL[200] = {0};
    sprintf_s(szURL, sizeof(szURL), "http://%s:%d/kms/services/rest/dataInfoService/getRtspPath?id=%s&&token=%s", 
        struKMSInfo.strIp.c_str(), struKMSInfo.nPort, strFileId.c_str(), strToken.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, szURL); 
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&strResponse ); 
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo); 
    res = curl_easy_perform( curl ); 
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        KMS_LOG_ERROR("curl_easy_perform failed.");
        return FALSE;
    }
    KMS_LOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
    //     {
    //         "ret": 0,
    //             "msg": "操作成功",
    //             "rtspPath": "rtsp://10.196.147.199:1554/file://X:/CQ/1185655dfc7845c8b4850986ff8547ca/1185655dfc7845c8b4850986ff8547ca.mp4"
    //     }
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            strRtspPath  = root["rtspPath"].asString();
			strStartTime = root["strBeginTime"].asString();
			strEndTime   = root["strEndTime"].asString();

        }
        else
        {
            std::string strMsg = root["msg"].asString();
            KMS_LOG_ERROR("getRtspPath failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
            return FALSE;
        }
    }
    else
    {
        KMS_LOG_ERROR("parse upload info failed...");
        return FALSE;
    }
    KMS_LOG_INFO("strRtspPath is %s", fcU2A(strRtspPath.c_str())); 
    return TRUE;
}

/** @fn getFileSize
*  @brief  获取文件大小
*  @param [in]strToken -- KMS的token认证信息
*  @param [in]struKMSInfo -- KMS的配置信息
*  @param [in]strFileId -- 上传文件的id
*  @param [out]nFileSize -- 该文件大小
*  @return bool
*/
BOOL CKMS::getFileSize(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &strFileId, int &nFileSize)
{
    std::string strResponse;
    CURL* curl = curl_easy_init();
    if(NULL == curl)  
    {  
        KMS_LOG_ERROR("curl is null...");
        return FALSE;  
    }  
    CURLcode res; 
    char szURL[200] = {0};
    sprintf_s(szURL, sizeof(szURL), "http://%s:%d/kms/services/rest/dataInfoService/getFileInfo?id=%s&&token=%s", 
        struKMSInfo.strIp.c_str(), struKMSInfo.nPort, strFileId.c_str(), strToken.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, szURL); 
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&strResponse ); 
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo); 
    res = curl_easy_perform( curl ); 
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        KMS_LOG_ERROR("curl_easy_perform failed.");
        return FALSE;
    }
    KMS_LOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
    //     {
    //         "ret": 0,
    //             "msg": "操作成功",
    //     }
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            //获取文件大小，只取第一条数据
            const Json::Value docObj = root["doc"];
            if (docObj.size() > 0)
            {
                nFileSize = docObj["docSize"].asInt();
            }
            else
            {
                KMS_LOG_ERROR("getFileSize failed.the doc not exist");
                return FALSE;
            }
        }
        else
        {
            std::string strMsg = root["msg"].asString();
            KMS_LOG_ERROR("getFileSize failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
            return FALSE;
        }
    }
    else
    {
        KMS_LOG_ERROR("parse upload info failed...");
        return FALSE;
    }
    KMS_LOG_INFO("filesize is %d", nFileSize); 
    return TRUE;
}

/** @fn setUploadPercent
*  @brief  设置上传进度
*  @param [in]fileId -- 上传文件的id
*  @param [in]nPercent -- 进度值
*  @return void
*/
// void CKMS::SetUploadPercent(const std::string &fileId, const int &nPercent)
// {
//     CUploadFactory::GetSingletonPtr()->SetUploadPercent(nPercent);
// }
