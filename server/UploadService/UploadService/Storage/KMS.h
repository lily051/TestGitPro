#pragma once
#include "storagebase.h"
#include "../../util/Guard.h"
#include <map>

// typedef struct _Struct_Process
// {
//     void* pThis;        //指针
//     std::string strFileId; //录像信息
//     _Struct_Process()
//     {
//         pThis = NULL;
//     }
// }Struct_Process;

class CKMS :
    public CStorageBase
{
public:
    CKMS(void);
    virtual ~CKMS(void);
public:
    /**	@fn	    Init
	*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	BOOL
	*/
    BOOL Init();

    /**	@fn	    Cleanup
	*	@brief	释放资源
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	BOOL
	*/
    BOOL Cleanup();

    /**	@fn	    Login
	*	@brief	登录
	*	@param  [in] struStorage -- 存储器信息
	*	@return	LONG -- 登录句柄
	*/
    LONG Login(const Struct_StorageConfig &struStorage);

    /**	@fn	    Logout
	*	@brief	登出
	*	@param  [in] lLoginID -- 登录句柄
	*	@return	BOOL
	*/
    BOOL Logout(LONG lLoginID);

    /**	@fn	    Upload
	*	@brief	上传录像
	*	@param  [in] struStorage -- 存储器信息
	*	@param  [in] struRecord -- 录像信息
	*	@return	BOOL
	*/
    BOOL Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord);

    /**	@fn	    GetUploadStatus
	*	@brief	获取上传状态
    *	@param  [in] lLoginID -- 登录句柄
    *	@param  [in] struRecord -- 录像信息
    *	@param  [out] struRecord.recordStatus -- 录像状态
	*	@return	BOOL
	*/
    BOOL GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord);

    /** @fn SetUploadPercent
    *  @brief  设置上传进度
    *  @param [in]fileId -- 上传文件的id
    *  @param [in]nPercent -- 进度值
    *  @return void
    */
    /*void SetUploadPercent(const std::string &fileId, const int &nPercent);*/

	/**	@fn	    GetUploadRecordInfo
    *	@brief	获取存储设备中已上传的录像信息
    *	@param  [in] lLoginID -- 登录句柄
	*	@param  [in] struRecord -- 上传录像信息
    *	@param  [out] struRecordParam -- 已上传到存储设备中的录像信息
    *	@return	BOOL
    */
    BOOL GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam);

private:
    /** @fn getHttpToken
    *  @brief  向KMS获取Token认证信息
    *  @param [in]struKMSInfo -- KMS的配置信息
    *  @param [out]strToken -- 获取到的token信息
    *  @return BOOL
    */
    BOOL getHttpToken(const Struct_StorageConfig &struKMSInfo, std::string &strToken);

    /** @fn uploadFile
    *  @brief  向KMS上传文件
    *  @param [in]strToken -- KMS的token认证信息
    *  @param [in]struKMSInfo -- KMS的配置信息
    *  @param [in]filePath -- 上传文件的路径
    *  @param [out]strFileId -- KMS返回的文件id
    *  @return BOOL
    */
    BOOL uploadFile(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &filePath, std::string &strFileId);

    /** @fn getRtspPath
    *  @brief  获取rtsp路径
    *  @param [in]strToken -- KMS的token认证信息
    *  @param [in]struKMSInfo -- KMS的配置信息
    *  @param [in]strFileId -- 上传文件的id
    *  @param [out]strRtspPath -- 该文件的RTSP播放路径
    *  @return bool
    */
	BOOL getRtspPath(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &strFileId, std::string &strRtspPath, std::string &strStartTime, std::string &strEndTime);

    /** @fn getFileSize
    *  @brief  获取文件大小
    *  @param [in]strToken -- KMS的token认证信息
    *  @param [in]struKMSInfo -- KMS的配置信息
    *  @param [in]strFileId -- 上传文件的id
    *  @param [out]nFileSize -- 该文件大小
    *  @return bool
    */
    BOOL getFileSize(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &strFileId, int &nFileSize);

private:
//     CRITICAL_SECTION m_csMapUploadPercent;          //进度锁
//     std::map<std::string, int> m_mapUploadPercent;  //录像进度信息

};
