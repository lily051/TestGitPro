#ifndef GUARD_UPLOADSERVICE_H
#define GUARD_UPLOADSERVICE_H

#ifdef UPLOADSERVICE_EXPORTS
#define UPLOADSERVICE_API __declspec(dllexport)
#else
#define UPLOADSERVICE_API __declspec(dllimport)
#endif

#include <SvcGeneralDef.h>

#ifdef __cplusplus
extern "C" {
#endif

// 错误码定义
static const int UploadSvc_NO_ERROR = 0; // 没有错误，发生错误则返回负数值，具体含义由各模块自定义
// 这里也不再定义其他错误码，上层对这些一无所知，出现错误应该由模块自己使用 hlog 进行记录

UPLOADSERVICE_API int fnUploadSvc(void);

/**	@fn	    UploadSvc_Init
 *	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
 *          上层应用不需要处理
 *          这个地方可以用来进行以前的用户名和密码验证
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，UploadSvc_NO_ERROR 表示没有错误，其他数值表示出现了错误
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Init(void);

/**	@fn	    UploadSvc_Fini
 *	@brief	反初始化函数，负责资源释放
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，UploadSvc_NO_ERROR 表示没有错误，其他数值表示出现了错误
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Fini(void);

/**	@fn	    UploadSvc_Start
 *	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
 *          此函数应该尽快返回，不应该占用太多时间，必须小于30s
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，UploadSvc_NO_ERROR 表示没有错误，其他数值表示出现了错误
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Start(void);

/**	@fn	    UploadSvc_Stop
 *	@brief	停止函数
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，UploadSvc_NO_ERROR 表示没有错误，其他数值表示出现了错误
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Stop(void);

/**	@fn	    UploadSvc_GetVersion
 *	@brief	获取版本号
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	DWORD，返回版本，如0x01020304
 */
UPLOADSERVICE_API DWORD WINAPI UploadSvc_GetVersion(void);

/**	@fn	    UploadSvc_GetLastError
 *	@brief	返回错误码
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，UploadSvc_NO_ERROR 表示没有错误，其他数值表示出现了错误
 */
UPLOADSERVICE_API int WINAPI UploadSvc_GetLastError(void);

/**	@fn	    UploadSvc_SetMsgCallback
 *	@brief	设置DLL消息回调函数
 *	@param  [in] const pTransMsgBack msgfun, 即时消息回调函数
 *	@param  [out] 无
 *	@return	int，UploadSvc_NO_ERROR 表示没有错误，其他数值表示出现了错误
 */
UPLOADSERVICE_API int WINAPI UploadSvc_SetMsgCallback(const pTransMsgBack msgfun);
//UPLOADSERVICE_API int WINAPI UploadSvc_SetMsgCallbackEx(const pTransMsgBackEx msgfun);

//显示消息,注意参数为UTF-8编码
void _gShowMsgStr(const std::string  & sMsg_);

#ifdef __cplusplus
}
#endif

#endif // end of GUARD_UPLOADSERVICE_H
