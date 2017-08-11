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
*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CKMS::Init()
{
    //��ʼ��curl��
    curl_global_init(CURL_GLOBAL_ALL);
    return TRUE;
}

/**	@fn	    Cleanup
*	@brief	�ͷ���Դ
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CKMS::Cleanup()
{
    //����ʼ��curl��
    curl_global_cleanup();
    return TRUE;
}

/**	@fn	    Login
*	@brief	��¼
*	@param  [in] struStorage -- �洢����Ϣ
*	@return	BOOL -- ��¼���
*/
LONG CKMS::Login(const Struct_StorageConfig &struStorage)
{
    return 0;
}

/**	@fn	    Logout
*	@brief	�ǳ�
*	@param  [in] lLoginID -- ��¼���
*	@return	BOOL
*/
BOOL CKMS::Logout(LONG lLoginID)
{
    return TRUE;
}

//�ص���������ȡ���ؽ��
size_t OnCallBackGetHttpInfo(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{
    //���ؽ����lpVoid����
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

//�ص���������ȡ�ļ��ϴ�����
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
*	@brief	�ϴ�¼��
*	@param  [in] struStorage -- �洢����Ϣ
*	@param  [in] struRecord -- ¼����Ϣ
*	@return	BOOL
*/
BOOL CKMS::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    //��ȡKMS��Token��Ϣ
    std::string strToken;
    if (!getHttpToken(struRecord.struStorage, strToken))
    {
        KMS_LOG_ERROR("getHttpToken failed.kms ip:%s, port:%d", struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort);
        return FALSE;
    }
    //�ϴ��ļ�
    std::string strFileID;
    if (!uploadFile(strToken, struRecord.struStorage, struRecord.strFilePath, strFileID))
    {
        KMS_LOG_ERROR("uploadFile failed.kms ip:%s, port:%d.filepath:%s", 
            struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort, struRecord.strFilePath.c_str());
        return FALSE;
    }
    struRecord.strCameraID = strFileID;
    //��ȡ�ļ��ϴ����RTSP·��
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
    //��ȡ���ļ��ϴ�����KMS�洢�Ĵ�С
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
*	@brief	��ȡ�ϴ�״̬
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- ¼����Ϣ
*	@param  [out] struRecord.recordStatus -- ¼��״̬
*	@return	BOOL
*/
BOOL CKMS::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
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
BOOL CKMS::GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam)
{
	return TRUE;
}

/** @fn getHttpToken
*  @brief  ��KMS��ȡToken��֤��Ϣ
*  @param [in]struKMSInfo -- KMS��������Ϣ
*  @param [out]strToken -- ��ȡ����token��Ϣ
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
    //���ûص�
    //���÷���ֵ���ɻص���������
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);      
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

    //���ó�ʱʱ��
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
    //{"node":"10.16.38.27:8080","ret":0,"msg":"�����ɹ�","tokenError":false,"token":"807d6bf90000014f60ecfe434f42a54555"}
    //����jsoncpp��������ص�����
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
*  @brief  ��KMS�ϴ��ļ�
*  @param [in]strToken -- KMS��token��֤��Ϣ
*  @param [in]struKMSInfo -- KMS��������Ϣ
*  @param [in]filePath -- �ϴ��ļ���·��
*  @param [out]strFileId -- KMS���ص��ļ�id
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
        CURLFORM_COPYCONTENTS, "�ύ",
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

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);//��Ϊfalse ����������ý�����Ӧ����
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)this);//���ݴ���Ķ���
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, assetsManagerProgressFunc);//������Ӧ����


    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        //�������
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
    //             "msg": "�����ɹ�",
    //             "id": "a946e65a56a3406585ad1547f7df6fbd",
    //             "docMd5": "499527a2e3f6c2ebcccb0fc52336c279"��
    //             "url": "/kms/services/rest/dataInfoService/downloadFile?id=ae431c644eaf4024aa0ad92908317f45",
    //             "thumbUrl": "/kms/services/rest/CRL/getThumbnail?id=ae431c644eaf4024aa0ad92908317f45",
    //             "beginTime": 1311131696000,
    //             "endTime": 1311131701000
    //     }

    //����jsoncpp��������ص�����
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
*  @brief  ��KMS�ϴ��ļ�
*  @param [in]strToken -- KMS��token��֤��Ϣ
*  @param [in]struKMSInfo -- KMS��������Ϣ
*  @param [in]strFileId -- �ϴ��ļ���id
*  @param [out]strRtspPath -- ���ļ���RTSP����·��
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
    //             "msg": "�����ɹ�",
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
*  @brief  ��ȡ�ļ���С
*  @param [in]strToken -- KMS��token��֤��Ϣ
*  @param [in]struKMSInfo -- KMS��������Ϣ
*  @param [in]strFileId -- �ϴ��ļ���id
*  @param [out]nFileSize -- ���ļ���С
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
    //             "msg": "�����ɹ�",
    //     }
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            //��ȡ�ļ���С��ֻȡ��һ������
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
*  @brief  �����ϴ�����
*  @param [in]fileId -- �ϴ��ļ���id
*  @param [in]nPercent -- ����ֵ
*  @return void
*/
// void CKMS::SetUploadPercent(const std::string &fileId, const int &nPercent)
// {
//     CUploadFactory::GetSingletonPtr()->SetUploadPercent(nPercent);
// }
