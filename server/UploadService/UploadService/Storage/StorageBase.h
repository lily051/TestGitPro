#pragma once
#define STORAGE_LOGIN_FAILED -1

class CStorageBase
{
public:
    CStorageBase(void);
    virtual ~CStorageBase(void);
public:
    /**	@fn	    Init
	*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	BOOL
	*/
    virtual BOOL Init() = 0;

    /**	@fn	    Cleanup
	*	@brief	释放资源
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	BOOL
	*/
    virtual BOOL Cleanup() = 0;

    /**	@fn	    Login
	*	@brief	登录
	*	@param  [in] struStorage -- 存储器信息
	*	@return	LONG -- 登录句柄
	*/
    virtual LONG Login(const Struct_StorageConfig &struStorage) = 0;

    /**	@fn	    Logout
	*	@brief	登出
	*	@param  [in] lLoginID -- 登录句柄
	*	@return	BOOL
	*/
    virtual BOOL Logout(LONG lLoginID) = 0;

    /**	@fn	    Upload
	*	@brief	上传录像
    *	@param  [in] lLoginID -- 登录句柄
	*	@param  [in] struRecord -- 录像信息
	*	@return	BOOL
	*/
    virtual BOOL Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord) = 0;

    /**	@fn	    GetUploadStatus
	*	@brief	获取上传状态
    *	@param  [in] lLoginID -- 登录句柄
    *	@param  [in] struRecord -- 录像信息
    *	@param  [out] struRecord.recordStatus -- 录像状态
	*	@return	BOOL
	*/
    virtual BOOL GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord) = 0;

	/**	@fn	    GetUploadRecordInfo
    *	@brief	获取存储设备中已上传的录像信息
    *	@param  [in] lLoginID -- 登录句柄
	*	@param  [in] struRecord -- 上传录像信息
    *	@param  [out] struRecordParam -- 已上传到存储设备中的录像信息
    *	@return	BOOL
    */
    virtual BOOL GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam) = 0;
};
