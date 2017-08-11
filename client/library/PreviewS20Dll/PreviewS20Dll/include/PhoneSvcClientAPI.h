//lint -library
#ifndef __GUARD_PHONESVCCLIENTAPI_H_
#define __GUARD_PHONESVCCLIENTAPI_H_

#ifdef PHONESVCCLIENTAPI_EXPORTS
#define PHONESVCCLENT_API __declspec(dllexport)
#else
#define PHONESVCCLENT_API __declspec(dllimport)
#endif

// 流数据类型 用于数据回调函数
enum phoneDataStreamType {
	PHONE_DT_STREAM_START = 0,       // 录音开始
	PHONE_DT_STREAM_DATA = 1,        // 录音数据
	PHONE_DT_STREAM_STOP = 2,        // 录音停止
	PHONE_DT_STREAM_END = 500,       // 数据结束
	PHONE_DT_STREAM_EXCEPTION = 501, // 501
};

// 错误码
enum phoneErrorType {
	PHONE_ERROR_SUCCESS = 0, // 没错误
	PHONE_ERROR_CONNECTION,  // 连接错误
	PHONE_ERROR_START,       // 开始错误
	PHONE_ERROR_PLAY,        // 播放过程错误
	PHONE_ERROR_UNKNOW,      // 未知错误
};

/**	@fn	    PSClient_ErrorNotify
*	@brief	错误通知原型
*	@param  HANDLE hLogin,登录句柄
*	@param  HANDLE userdata,用户自定义数据
*   @param  int nError,错误内容
*	@return	void
*/
typedef void (CALLBACK * PSClient_ErrorNotify)(HANDLE hLogin, HANDLE userdata, 
											   int nError);

/**	@fn	    PSClient_StreamDataCallback
*	@brief	数据回调
*	@param  HANDLE hLogin,登录句柄
*	@param  HANDLE userdata,用户自定义数据
*   @param  long datatype,数据类型
*   @param  void * pBuffer,数据缓冲
*   @param  DWORD dwSize,数据大小
*	@return	void
*/
typedef void (CALLBACK * PSClient_StreamDataCallback)(HANDLE hLogin, HANDLE userdata, 
													  long datatype, void * pBuffer, DWORD dwSize);

#ifdef __cplusplus
extern "C"
{
#endif

	/**	@fn	    PSClient_Startup
	*	@brief	初始化
	*	@param  null
	*	@return	BOOL
	*/
	PHONESVCCLENT_API BOOL PSClient_Startup(void);

	/**	@fn	    PSClient_Cleanup
	*	@brief	反初始化
	*	@param  null
	*	@return	BOOL
	*/
	PHONESVCCLENT_API BOOL PSClient_Cleanup(void);

	/**	@fn	    PSClient_Login
	*	@brief	登录录像服务模块
	*	@param  PCSTR szIP, 服务器ip
	*   @param  UINT16 nPort, 服务器数据监听端口
	*   @param  DWORD nID, 服务器id
	*   @param  evcStreamDataCallback pFunStream,数据回调函数
	*   @param  evcErrorNotify pFunError,错误回调函数
	*	@return	HANDLE, NULL for fail
	*/
	PHONESVCCLENT_API HANDLE PSClient_Login(PCSTR szIP, USHORT nPort, int nUserID, int nPChannel, HANDLE hUserData, PSClient_StreamDataCallback pFunStream = NULL, PSClient_ErrorNotify pFunError = NULL);

	/**	@fn	    PSClient_Logout
	*	@brief	登出录像服务模块
	*	@param  HANDLE hLogin，登录返回的句柄
	*	@return	BOOL
	*/
	PHONESVCCLENT_API BOOL PSClient_Logout(HANDLE hLogin);

#ifdef __cplusplus
}
#endif

#endif
