//lint -library
#ifndef __GUARD_VIDEOSVCCLENTAPI_H_
#define __GUARD_VIDEOSVCCLENTAPI_H_

#ifdef VIDEOSVCCLENTAPI_EXPORTS
#define VIDEOSVCCLENT_API __declspec(dllexport)
#else
#define VIDEOSVCCLENT_API __declspec(dllimport)
#endif

// 错误码
enum evcErrorType {
	EVC_ERROR_SUCCESS = 0, // 没错误
	EVC_ERROR_CONNECTION,  // 连接错误
	EVC_ERROR_START,       // 开始错误
	EVC_ERROR_PLAY,        // 播放过程错误
	EVC_ERROR_RESERVE_SAPCE_FULL, //磁盘满
	EVC_ERROR_FILE_OPERATION_FAIL, //文件操作失败
	EVC_ERROR_UNKNOW,      // 未知错误
};

/**	@fn	    evcErrorNotify
*	@brief	错误通知原型
*	@param  HANDLE hLogin,登录句柄
*	@param  HANDLE userdata,用户自定义数据
*  @param  int nError,错误内容
*	@return	void
*/
typedef void (CALLBACK * VSClient_ErrorNotify)(HANDLE hLogin, HANDLE userdata, 
										 int nError);

/**	@fn	    evcStreamDataCallback
*	@brief	数据回调
*	@param  HANDLE hLogin,登录句柄
*	@param  HANDLE userdata,用户自定义数据
*  @param  long datatype,数据类型
*  @param  void * pBuffer,数据缓冲
*  @param  DWORD dwSize,数据大小
*	@return	void
*/
typedef void (CALLBACK * VSClient_StreamDataCallback)(HANDLE hLogin, HANDLE userdata, 
												long datatype, void * pBuffer, DWORD dwSize);


#ifdef __cplusplus
extern "C"
{
#endif

	/**	@fn	    evcStartup
	*	@brief	初始化
	*	@param  null
	*	@return	BOOL
	*/
	VIDEOSVCCLENT_API BOOL VSClient_Startup(void);

	/**	@fn	    evcCleanup
	*	@brief	反初始化
	*	@param  null
	*	@return	BOOL
	*/
	VIDEOSVCCLENT_API BOOL VSClient_Cleanup(void);

	/**	@fn	    evcLogin
	*	@brief	登录服务器
	*	@param  PCSTR szIP, 服务器ip
	*  @param  UINT16 nPort, 服务器数据监听端口
	*  @param  DWORD nID, 服务器id
	*  @param  evcStreamDataCallback pFunStream,数据回调函数
	*  @param  evcErrorNotify pFunError,错误回调函数
	*	@return	HANDLE, NULL for fail
	*/
	VIDEOSVCCLENT_API HANDLE VSClient_Login(PCSTR szIP, USHORT nPort, int nUserID, HANDLE hUserData, VSClient_StreamDataCallback pFunStream = NULL, VSClient_ErrorNotify pFunError = NULL);

	/**	@fn	    evcLogout
	*	@brief	退出服务
	*	@param  HANDLE hLogin，登录返回的句柄
	*	@return	BOOL
	*/
	VIDEOSVCCLENT_API BOOL VSClient_Logout(HANDLE hLogin);


	/**	@fn	    VSClient_StartPreview
	 *	@brief	
	 *	@param  HANDLE hLogin
	 *	@param  const VIDEO_REQ_PREVIEW_MSG & struStartPreview
	 *	@return	VIDEOSVCCLENT_API BOOL
	 */
	VIDEOSVCCLENT_API BOOL VSClient_StartPreview(HANDLE hLogin, int nCameraID);


	/**	@fn	    VSClient_StopPreview
	 *	@brief	
	 *	@param  HANDLE hLogin
	 *	@param  const VIDEO_REQ_PREVIEW_MSG & struStartPreview
	 *	@return	VIDEOSVCCLENT_API BOOL
	 */
	VIDEOSVCCLENT_API BOOL VSClient_StopPreview(HANDLE hLogin, int nCameraID);


	/**	@fn	    VSClient_StartRecord
	 *	@brief	
	 *	@param  HANDLE hLogin
	 *	@param  int nCameraID
	 *	@return	VIDEOSVCCLENT_API BOOL
	 */
	VIDEOSVCCLENT_API BOOL VSClient_StartRecord(HANDLE hLogin, int nCameraID);


	/**	@fn	    VSClient_StopRecord
	 *	@brief	
	 *	@param  HANDLE hLogin
	 *	@param  int nCameraID
	 *	@return	VIDEOSVCCLENT_API BOOL
	 */
	VIDEOSVCCLENT_API BOOL VSClient_StopRecord(HANDLE hLogin, int nCameraID);

#ifdef __cplusplus
}
#endif

#endif //end if __GUARD_VIDEOSVCCLENT_API_H_
