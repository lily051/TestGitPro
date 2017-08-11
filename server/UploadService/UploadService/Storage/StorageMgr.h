#pragma once
#include "../stdafx.h"
#include "StorageBase.h"
#include "../../util/Singleton.h"
#include "../../util/Guard.h"
#include <time.h>
#include <map>

#define    INTERNAL_5_MINUTE  300   //登录时间间隔

//存储设备登录信息
typedef struct _Struct_LoginInfo
{
    LONG lLoginID;//有效时间30分钟
    int nCount;//任务使用计数
    time_t loginTime;//登陆时间
    int nError;//发生错误计数
    ENUM_STORAGE_TYPE nManType; //类型
    _Struct_LoginInfo()
        :lLoginID(STORAGE_LOGIN_FAILED)
        ,nCount(0)
        ,loginTime(time(0))
        ,nError(0)
        ,nManType(STORAGE_TYPE_MAX)
    {

    }
}Struct_LoginInfo;

class CStorageMgr
{
    SINGLETON_DECLARE(CStorageMgr)///<单例相关声明

private:
    CStorageMgr(void);
    virtual ~CStorageMgr(void);
public:
    /**	@fn	    Init
	*	@brief	初始化函数
	*	@param  [in] 
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Init(ENUM_STORAGE_TYPE enumStorageType);

	/**	@fn	    Fini
	*	@brief	反初始化函数，负责资源释放
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Fini();

    /**	@fn	    Login
	*	@brief	登录
	*	@param  [in] struStorage -- 存储器信息
	*	@param  [out] struStorage.lLoginID -- 登录句柄
	*	@return	LONG
	*/
    LONG Login(const Struct_StorageConfig &struStorage);

    /**	@fn	    Logout
	*	@brief	登出
	*	@param  [in] lLoginID -- 登录句柄
	*	@return	BOOL
	*/
    BOOL Logout(const ENUM_STORAGE_TYPE &storageType, const LONG &lLoginID);

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

    /**	@fn	    LoginManage
    *	@brief	登录管理
    *	@param  [in] struStorage -- 存储设备信息
    *	@return	LONG -- 登录句柄
    */
    LONG LoginManage(const Struct_StorageConfig &struStorage);

    /**	@fn	    LogoutManage
    *	@brief	登出管理
    *	@param  [in] struStorage -- 存储设备信息
    *	@param  [in] lLoginID -- 登录句柄
    *   @param  [in] bError -- 是否有错误发生
    *	@return	void
    */
    void LogoutManage(const Struct_StorageConfig &struStorage, BOOL bError);
private:
    CRITICAL_SECTION m_csMapLoginInfo;                                  //存储设备登录锁
    std::map<std::string, Struct_LoginInfo> m_mapStorageLoginInfo;  //存储设备登录信息

    CStorageBase* m_pStorageMgr[STORAGE_TYPE_MAX];
};
