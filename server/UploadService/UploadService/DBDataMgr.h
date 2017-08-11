/**	@file DBDataMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief DC数据交互类
*	@author		chenbin5
*	@date		2016-3-4
*	@note 
*/
#pragma once
#include "../util/Singleton.h"
#include "verify/RspServerLogin.pb.h"
#include "Center/Collectconfig.pb.h"

class CDBDataMgr
{
    SINGLETON_DECLARE(CDBDataMgr)///<单例相关声明

public:
    CDBDataMgr(void);
    virtual ~CDBDataMgr(void);
public:
    /**	@fn	    Init
	*	@brief	初始化函数
	*	@param  [in] 
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Init();

	/**	@fn	    Fini
	*	@brief	反初始化函数，负责资源释放
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Fini();

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
public:
    /**	@fn	    Login
    *	@brief	登录
    *	@param  [in] 无
    *	@return	ENUM_LOGIN_RESULT，LOGIN_RESULT_OK,LOGIN_RESULT_FAIL
    */
    ENUM_LOGIN_RESULT Login();

    /**	@fn	    Logout
    *	@brief	登出
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int Logout();

    /**	@fn	    AddLoginRsp
    *	@brief	添加登陆响应消息
    *	@param  [in] serLoginRsp 登陆响应信息
    *	@return	
    */
    void AddLoginRsp(const cms_8120::RspServerLogin & serLoginRsp);

    /**	@fn	    SendLoginReq
    *	@brief	发送登录消息
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SendLoginReq();

    /**	@fn	    SendLogoutMsg
    *	@brief	发送退出消息
    *	@param  [in]无
    *	@return	
    */
    void SendLogoutMsg();

    /**	@fn	    SendHeartBeat
    *	@brief	发送心跳请求
    *	@param  [in]无
    *	@return	
    */
    void SendHeartBeat();

    /**	@fn	    GetRecordByStatus
    *	@brief	根据录像状态获取录像信息
    *	@param  [in] nStatus -- 录像状态
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetRecordByStatus(/*ENUM_RECORD_STATUS nStatus*/);

	/**	@fn	    GetUploadedRecords
    *	@brief	获取某一时间段（web配置）的已上传录像信息
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetUploadedRecords();

    /**	@fn	    SetRecordStatus
    *	@brief	设置录像状态
    *	@param  [in] nID -- 录像ID
    *	@param  [in] nStatus -- 录像状态
    *	@param  [in] nPercent -- 上传进度
	*	@param  [in] strTip -- 提示信息
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SetRecordStatus(const int &nID, const ENUM_RECORD_STATUS nStatus, const int &nTaskId = -1, const int &nPercent = 0, std::string strTip = "");

    /**	@fn	    SetRecordFail
    *	@brief	设置录像失败状态
    *	@param  [in] nID -- 录像ID
    *	@param  [in] strErrorMsg -- 错误信息
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SetRecordFail(const int &nID, const std::string& strErrorMsg);

    /**	@fn	    SetKmsRecordStatus(KMS专用)
    *	@brief	设置录像状态(KMS专用)
    *	@param  [in] nID -- 录像ID
    *	@param  [in] nStatus -- 录像状态
    *	@param  [in] strFiledID -- 文件ID
    *	@param  [in] strRtspPath -- 文件在KMS的RTSP路径
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SetKmsRecordStatus(const int &nID, const ENUM_RECORD_STATUS nStatus, const std::string &strFiledID, 
		const std::string &strRtspPath, const int &nFileSize, const std::string &strStartTime, const std::string &strEndTime);

	 /**	@fn	    SetObjectCloudRecordStatus(对象云存储专用)
    *	@brief	设置录像状态(对象云存储专用)
    *	@param  [in] nID -- 录像ID
    *	@param  [in] nStatus -- 录像状态
    *	@param  [in] strFiledID -- 文件ID
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SetObjectCloudRecordStatus(const int &nID, const ENUM_RECORD_STATUS nStatus, const std::string &strFiledID, const std::string &strStartTime, const std::string &strEndTime);

    /**	@fn	    GetStorageInfo
    *	@brief	获取存储设备参数
    *	@param  [in] strIP -- 采集器IP
    *                nPort -- 采集器端口
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetStorageInfo(const std::string &strIP, const int &nPort);

private:
    bool m_bInit;                                       //是否被初始化过
    bool m_bStart;                                      //是否启动过

    HANDLE m_hLoginRetEvent;                            //登录返回信号
    volatile bool m_bStopWorkerThread;                  //停止工作线程
public:
    cms_8120::RspServerLogin m_ServerLoginRsp;          //服务器登陆响应信息
};
