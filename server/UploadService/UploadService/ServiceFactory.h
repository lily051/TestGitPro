/**	@file ServiceFactory.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 包含所有服务的类。该类中配置提供哪些服务、维护启动的服务等；
*	@author		chenbin5
*	@date		2016-3-3
*	@note 
*/
#pragma once
#include "../util/Noncopyable.h"
#include "../util/Singleton.h"
#include "../util/IService.h"
#include "GlobalParam.h"


class CServiceFactory:public Noncopyable, public IService
{
    SINGLETON_DECLARE(CServiceFactory)///<单例相关声明

public:
	/**	@fn	    Init
	*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
	*	@param  [in] 无
	*	@param  [out] 无
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

	/**	@fn	    GetVersion
	*	@brief	获取版本号
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	DWORD，返回版本，如0x01020304
	*/
	int GetVersion();

	/**	@fn	    GetLastError
	*	@brief	返回错误码
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int GetLastError();
private:
	bool m_bInit;//<是否已经初始化
	bool m_bStart;//<是否已经启动
private:
    CServiceFactory(void);
    virtual ~CServiceFactory(void);
};
