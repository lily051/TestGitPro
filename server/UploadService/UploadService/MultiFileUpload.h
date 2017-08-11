#pragma once
#include "GlobalParam.h"
#include "../util/Singleton.h"
#include "../util/Guard.h"
#include <list>
#include <map>
#include "hcs_sdk.h"
#include "hcs_object_api.h"

const int WRITE_LENGTH   = 512 * 1024; //512K字节
const int MAX_THREAD_NUM = 100; //最大并发上传数

class CMultiFileUpload 
{
	SINGLETON_DECLARE(CMultiFileUpload)///<单例相关声明
public:
    CMultiFileUpload(void);
    virtual ~CMultiFileUpload(void);
public:
    /**	@fn	    Init
	*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	BOOL
	*/
    BOOL Init();

    /**	@fn	    Fini
	*	@brief	反初始化函数，释放资源等
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	BOOL
	*/
    BOOL Fini();

	/**	@fn	    Start
	*	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
	*          此函数应该尽快返回，不应该占用太多时间，必须小于30s
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Start();

	/**	@fn	    Stop
	*	@brief	停止函数
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Stop();

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

	/**	@fn	    MultiFileUpload
	*	@brief	上传录像
	*	@param  [in] 无
	*	@return	void
	*/
    void MultiFileUpload();

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
private:

	/**	@fn	    UploadStreamFile
	*	@brief	流式上传
	*	@param  [in] struRecord -- 录像信息
	*	@param  [out] struRecord.strCameraID -- 文件key
	*	@return	BOOL
	*/
	BOOL UploadStreamFile(Struct_RecordInfo &struRecord);

	/**	@fn	    UploadNormalFile
	*	@brief	普通上传
	*	@param  [in] struRecord -- 录像信息
	*	@param  [out] struRecord.strCameraID -- 文件key
	*	@return	BOOL
	*/
	BOOL UploadNormalFile(Struct_RecordInfo &struRecord);
private:
	bool m_bInit;  //<是否已经初始化
	bool m_bStart; //<是否已经启动

private:
//     CRITICAL_SECTION m_csMapUploadPercent;          //进度锁
//     std::map<std::string, int> m_mapUploadPercent;  //录像进度信息
	std::list<Struct_RecordInfo> m_lstFile;         //文件信息队列
	volatile bool m_bStopWorker;  //停止工作线程
	HCSClient * m_client;
	char  fileData[WRITE_LENGTH + 1];
	HANDLE hThread[MAX_THREAD_NUM];  //线程句柄
	CRITICAL_SECTION m_csFile;       //文件访问锁    
};
