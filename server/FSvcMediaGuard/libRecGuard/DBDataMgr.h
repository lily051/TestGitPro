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

//登录结果
typedef enum _loginResult
{
    LOGIN_RESULT_FAIL = -1,//登录失败
    LOGIN_RESULT_OK = 0,//登录成功
}ENUM_LOGIN_RESULT;

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

    /**	@fn	    GetSysConfig
    *	@brief	获取中心系统信息
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetSysConfig();

    /**	@fn	    GetStorageInfo
    *	@brief	获取存储设备参数
    *	@param  [in] strStorageID -- 存储设备ID
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetStorageInfo(const HPR_INT32 &storageID);

    /**	@fn	    GetOldBzRecord
    *	@brief	获取过期录像信息
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetOldBzRecord();

    /**	@fn	    GetRecRecord
    *	@brief	获取录像信息
    *	@param  [in] nClientId -- ClientID
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetRecRecord(const int &nClientId);

    /**	@fn	    MarkOldBzRecords
    *	@brief	标记clientinput表中的过期记录
    *	@param  [in] nClientID
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int MarkOldBzRecords(const int &nClientID);

    /**	@fn	    DelClientRecords
    *	@brief	删除录像记录
    *	@param  [in] nClientId -- ClientID
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int DelClientRecords(const std::string &strClientIds);

    /**	@fn	    DelCenterRecords
    *	@brief	删除prosaverecord表中的录像记录
    *	@param  [in] nRecordID -- 中心录像ID
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int CDBDataMgr::DelCenterRecords(const int &nRecordID);
private:
    bool m_bInit;                                       //是否被初始化过
    bool m_bStart;                                      //是否启动过

    HANDLE m_hLoginRetEvent;                            //登录返回信号
    volatile bool m_bStopWorkerThread;                  //停止工作线程
public:
    cms_8120::RspServerLogin m_ServerLoginRsp;          //服务器登陆响应信息
};
