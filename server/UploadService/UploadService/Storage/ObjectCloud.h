#pragma once
#include "storagebase.h"
#include "../../util/Guard.h"
#include <map>
#include "hcs_sdk.h"
#include "hcs_object_api.h"
#include "../../util/NetSpeedControl.h"

// typedef struct _Struct_Process
// {
//     void* pThis;        //指针
//     std::string strFileId; //录像信息
//     _Struct_Process()
//     {
//         pThis = NULL;
//     }
// }Struct_Process;

const int WRITE_LENGTH = 512 * 1024; //512K字节

class CObjectCloud :
    public CStorageBase
{
public:
    CObjectCloud(void);
    virtual ~CObjectCloud(void);
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

	/**	@fn	    UploadStreamFile
	*	@brief	流式上传
	*	@param  [in] pClient    -- 云存储初始化句柄
	*	@param  [in] struRecord -- 录像信息
	*	@param  [out] struRecord.strCameraID -- 文件key
	*	@return	BOOL
	*/
	BOOL UploadStreamFile(HCSClient *pClient, Struct_RecordInfo &struRecord);

	/**	@fn	    UploadNormalFile
	*	@brief	普通上传
	*	@param  [in] pClient    -- 云存储初始化句柄
	*	@param  [in] struRecord -- 录像信息
	*	@param  [out] struRecord.strCameraID -- 文件key
	*	@return	BOOL
	*/
	BOOL UploadNormalFile(HCSClient *pClient, Struct_RecordInfo &struRecord);

	/**	@fn	    GetRecordTime
	*	@brief	获取已上传录像的开始/结束时间
	*	@param  [in] pClient    -- 云存储初始化句柄
	*	@param  [in] strBucket -- 资源池
	*	@param  [in] strObjectKey -- 文件key
	*	@param  [out] strStartTime -- 录像开始时间
	*	@param  [out] strEndTime -- 录像结束时间
	*	@return	BOOL
	*/
	BOOL GetRecordTime(HCSClient *pClient, std::string strBucket, std::string strObjectKey, std::string &strStartTime, std::string &strEndTime);

	/**	@fn	    GetTimeStamp
	*	@brief	得到时间戳
	*	@param  [in] strTime -- 时间字串
	*	@return	time_t
	*/
	time_t GetTimeStamp(std::string strTime);

private:
//     CRITICAL_SECTION m_csMapUploadPercent;          //进度锁
//     std::map<std::string, int> m_mapUploadPercent;  //录像进度信息
	std::map<std::string, HCSClient *> m_mapHCSClient;    //云存储初始化句柄map，key：云存储IP
    char  fileData[WRITE_LENGTH + 1];
	CNetSpeedControl m_oSpeedControl;
};
