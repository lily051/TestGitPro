/**	@file ClientConnSessionMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 客户端连接会话管理
*	@author		liangjungao
*	@date		2012-4-26 16:01:53
*	@note       
*/
#pragma once
#include "../util/Singleton.h"
#include <string>
#include <map>
#include <hpp_hpp.h>//<HPP协议栈库头文件
#include "ConnSessionInfo.h"
#include <Type/cmd.pb.h>
#include "AlarmMsg.pb.h"

#define HPP_WORKER_THREAD	 10	//HPP工作线程数
#define HPP_TIMEOUT			 10	//HPP超时时间.单位秒

#define RECONNECT_OVER_TIME     5 //重连时间

class CConnSessionMgr
{
    SINGLETON_DECLARE(CConnSessionMgr)///<单例相关声明

public:
    /**	@fn	    Init
    *	@brief	初始化函数
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

    /**	@fn	    Connect
    *	@brief	连接
    *	@param  [in] ip_ ip
    *	@param  [in] port_ 端口
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int Connect(const std::string & ip_, int port_);

    /**	@fn	    Disconnect
    *	@brief	断开连接
    *	@param  [in] ip_ ip
    *	@param  [in] port_ 端口
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int Disconnect(const std::string & ip_, int port_);

    /**	@fn	    IsConnect
    *	@brief	判断是否在连接
    *	@param  [in] ip_ ip
    *	@param  [in] port_ 端口
    *	@param  [out] 无
    *	@return	true连接，false没有连接
    */
    bool IsConnect(const std::string & ip_, int port_);
    
    /**	@fn	    SendMsg
    *	@brief	发送消息
    *	@param  [in] ip_ ip
    *	@param  [in] port_ 端口
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SendMsg(const std::string & ip_, int port_, cms_8120::CMD cmd_, void * pMsg);

    /**	@fn	    SendAlarmMsg
    *	@brief	发送报警消息
    *	@param  [in] ip_ ip
    *	@param  [in] port_ 端口
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SendAlarmMsg(const std::string & ip_, int port_, AlarmMsg::AlarmId cmd_, void * pMsg);

    /**	@fn	    StartListen
    *	@brief	启动监听
    *	@param  [in] ip_ ip
    *	@param  [in] port_ 端口
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int StartListen(const std::string & ip_, int port_);

    /**	@fn	    StopListen
    *	@brief	停止监听
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int StopListen();
public:
    /**	@fn	    AddConnSession
    *	@brief	添加会话Session
    *	@param  [in] pConnSession_ 
    *	@param  [out] 无
    *	@return	std::string 主键
    */
    void AddConnSession(ConnSession * pConnSession_);

    /**	@fn	    CloseConnSession
    *	@brief	关闭会话
    *	@param  [in] pConnSession_ 
    *	@param  [out] 无
    *	@return	std::string 主键
    */
    void CloseConnSession(ConnSession * pConnSession_);

    /**	@fn	    RunCycleReconnect
    *	@brief	重连线程函数
    *	@param  [in]  
    *	@param  [out] 无
    *	@return	
    */
    void RunCycleReconnect();
private:
    /**	@fn	    FormatKey
    *	@brief	形成主键
    *	@param  [in] ip_ ip
    *	@param  [in] port_ 端口
    *	@param  [out] 无
    *	@return	std::string 主键
    */
    inline std::string FormatKey(const std::string & ip_, int port_);

    /**	@fn	    ClearConnect
    *	@brief	清除所有的连接信息
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	std::string 主键
    */
    void ClearConnect();
public:
    CConnSessionMgr(void);
    virtual ~CConnSessionMgr(void);
private:
    std::map<std::string,CConnSessionInfo> m_mapConnSessionInfo;//所有的连接会话信息
    typedef std::map<std::string,CConnSessionInfo>::iterator ConnSessionInfoIterator;
    CRITICAL_SECTION m_csConnSesssionLock;
    bool m_bInit;//是否初始化
    bool m_bStart;//是否启动
    HPP_HANDLE m_hHppHandle;//HPP库初始化返回值
    HANDLE m_hReconnectThread;//重连线程
    volatile bool m_bStopWorkerThread;//停止工作线程
    HPP_SERVER_HANDLE m_hServerHandle;//本地服务
};

