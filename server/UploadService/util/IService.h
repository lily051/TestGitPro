/**	@file IService.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 每个服务类需要实现的接口
*	@author		liangjungao
*	@date		2012-3-27
*	@note		每个单例类最好将其构造函数和虚析构函数设置为private
*/

#pragma once

/**	@class IService
*	@brief IService接口定义类。
*/
class IService
{
public:
	/**	@fn	    Init
	*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	virtual int Init() = 0;


	/**	@fn	    Fini
	*	@brief	反初始化函数，负责资源释放
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	virtual int Fini() = 0;

	/**	@fn	    Start
	*	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
	*          此函数应该尽快返回，不应该占用太多时间，必须小于30s
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	virtual int Start() = 0;

	/**	@fn	    Stop
	*	@brief	停止函数
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	virtual int Stop() = 0;

	/**	@fn	    GetVersion
	*	@brief	获取版本号
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	DWORD，返回版本，如0x01020304
	*/
	virtual int GetVersion() = 0;

	/**	@fn	    GetLastError
	*	@brief	返回错误码
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	virtual int GetLastError() = 0;
public:
	IService(void){};
	virtual ~IService(void){};
};