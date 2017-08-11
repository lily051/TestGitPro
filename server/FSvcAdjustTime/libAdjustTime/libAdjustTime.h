/**	@file    libAdjustTime.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   时间校准dll接口
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#ifndef GUARD_LIBADJUSTTIME_H
#define GUARD_LIBADJUSTTIME_H

#include "Common.h"
#include <SvcGeneralDef.h>

#ifdef LIBADJUSTTIME_EXPORTS
#define LIBADJUSTTIME_API __declspec(dllexport)
#else
#define LIBADJUSTTIME_API __declspec(dllimport)
#endif

// 错误码定义
static const int ADJUSTTIME_SVC_NO_ERROR = 0; // 没有错误，发生错误则返回负数值，具体含义由各模块自定义
// 这里也不再定义其他错误码，上层对这些一无所知，出现错误应该由模块自己使用 hlog 进行记录

/**	@fn	    libAdjustTime_Init
 *	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
 *          上层应用不需要处理
 *          这个地方可以用来进行以前的用户名和密码验证
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，ADJUST_ERROR_SUCC 表示没有错误，其他数值表示出现了错误
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Init(void);

/**	@fn	    libAdjustTime_Fini
 *	@brief	反初始化函数，负责资源释放
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，ADJUST_ERROR_SUCC 表示没有错误，其他数值表示出现了错误
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Fini(void);

/**	@fn	    libAdjustTime_Start
 *	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
 *          此函数应该尽快返回，不应该占用太多时间，必须小于30s
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，ADJUST_ERROR_SUCC 表示没有错误，其他数值表示出现了错误
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Start(void);

/**	@fn	    libAdjustTime_ImmExec
 *	@brief	立即执行一次系统巡检服务
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，ADJUST_ERROR_SUCC 表示没有错误，其他数值表示出现了错误
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_ImmExec(void);

/**	@fn	    libAdjustTime_Stop
 *	@brief	停止函数
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，ADJUST_ERROR_SUCC 表示没有错误，其他数值表示出现了错误
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Stop(void);

/**	@fn	    libAdjustTime_GetVersion
 *	@brief	获取版本号
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	DWORD，返回版本，如0x01020304
 */
LIBADJUSTTIME_API DWORD WINAPI libAdjustTime_GetVersion(void);

/**	@fn	    libAdjustTime_GetLastError
 *	@brief	返回错误码
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，ADJUST_ERROR_SUCC 表示没有错误，其他数值表示出现了错误
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_GetLastError(void);

/**	@fn	    libAdjustTime_SetMsgCallbackEx
 *	@brief	设置DLL消息回调函数
 *	@param  [in] const pMsgBack msgfun, 即时消息回调函数
 *	@param  [out] 无
 *	@return	int，ADJUST_ERROR_SUCC 表示没有错误，其他数值表示出现了错误
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_SetMsgCallback(const pTransMsgBack msgfun);
LIBADJUSTTIME_API int WINAPI libAdjustTime_SetMsgCallbackEx(const pTransMsgBackEx msgfun);

//显示消息,注意参数为UTF-8编码
void _gShowMsgStr(const std::string  & sMsg_);

#endif //GUARD_LIBADJUSTTIME_H
