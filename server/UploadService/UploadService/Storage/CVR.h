#pragma once
#include "storagebase.h"
#include "HCNetSDK/include/HCNetSDK.h"

class CCVR :
    public CStorageBase
{
public:
    CCVR(void);
    virtual ~CCVR(void);
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
    *	@param  [in] lLoginID -- 登录句柄
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

	/**	@fn	    GetUploadRecordInfo
    *	@brief	获取存储设备中已上传的录像信息
    *	@param  [in] lLoginID -- 登录句柄
	*	@param  [in] struRecord -- 上传录像信息
    *	@param  [out] struRecordParam -- 已上传到存储设备中的录像信息
    *	@return	BOOL
    */
    BOOL GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam);

private:
    /**	@fn	    setRecordTime
	*	@brief	设置时间信息
    *	@param  [in] nTime -- 时间戳
    *	@param  [out] struTime -- 时间信息
	*	@return	BOOL
	*/
    BOOL setRecordTime(const int &nTime, NET_DVR_TIME_EX &struTime);

	/**	@fn	    SetSearchTime
	*	@brief	设置搜索时间
	*	@param  [in] struRecordTime -- 录像时间
	*	@param  [out] struSearchTime -- 搜索时间
	*	@return	BOOL
	*/
	BOOL SetSearchTime(const Struct_RecordTime &struRecordTime, NET_DVR_TIME &struSearchTime);

	/**	@fn	    SetFileTime
	*	@brief	设置文件时间
	*	@param  [in] struRecordTime -- 录像时间
	*	@param  [out] struFileTime -- 文件时间
	*	@return	BOOL
	*/
	BOOL SetFileTime(const NET_DVR_TIME &struRecordTime, Struct_RecordTime &struFileTime);
};
