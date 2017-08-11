//lint -library
#ifndef __GUARD_VIDEOSVCCLENTAPI_H_
#define __GUARD_VIDEOSVCCLENTAPI_H_

#ifdef VIDEOSVCCLENTAPI_EXPORTS
#define VIDEOSVCCLENT_API __declspec(dllexport)
#else
#define VIDEOSVCCLENT_API __declspec(dllimport)
#endif

// ������
enum evcErrorType {
	EVC_ERROR_SUCCESS = 0, // û����
	EVC_ERROR_CONNECTION,  // ���Ӵ���
	EVC_ERROR_START,       // ��ʼ����
	EVC_ERROR_PLAY,        // ���Ź��̴���
	EVC_ERROR_RESERVE_SAPCE_FULL, //������
	EVC_ERROR_FILE_OPERATION_FAIL, //�ļ�����ʧ��
	EVC_ERROR_UNKNOW,      // δ֪����
};

/**	@fn	    evcErrorNotify
*	@brief	����֪ͨԭ��
*	@param  HANDLE hLogin,��¼���
*	@param  HANDLE userdata,�û��Զ�������
*  @param  int nError,��������
*	@return	void
*/
typedef void (CALLBACK * VSClient_ErrorNotify)(HANDLE hLogin, HANDLE userdata, 
										 int nError);

/**	@fn	    evcStreamDataCallback
*	@brief	���ݻص�
*	@param  HANDLE hLogin,��¼���
*	@param  HANDLE userdata,�û��Զ�������
*  @param  long datatype,��������
*  @param  void * pBuffer,���ݻ���
*  @param  DWORD dwSize,���ݴ�С
*	@return	void
*/
typedef void (CALLBACK * VSClient_StreamDataCallback)(HANDLE hLogin, HANDLE userdata, 
												long datatype, void * pBuffer, DWORD dwSize);


#ifdef __cplusplus
extern "C"
{
#endif

	/**	@fn	    evcStartup
	*	@brief	��ʼ��
	*	@param  null
	*	@return	BOOL
	*/
	VIDEOSVCCLENT_API BOOL VSClient_Startup(void);

	/**	@fn	    evcCleanup
	*	@brief	����ʼ��
	*	@param  null
	*	@return	BOOL
	*/
	VIDEOSVCCLENT_API BOOL VSClient_Cleanup(void);

	/**	@fn	    evcLogin
	*	@brief	��¼������
	*	@param  PCSTR szIP, ������ip
	*  @param  UINT16 nPort, ���������ݼ����˿�
	*  @param  DWORD nID, ������id
	*  @param  evcStreamDataCallback pFunStream,���ݻص�����
	*  @param  evcErrorNotify pFunError,����ص�����
	*	@return	HANDLE, NULL for fail
	*/
	VIDEOSVCCLENT_API HANDLE VSClient_Login(PCSTR szIP, USHORT nPort, int nUserID, HANDLE hUserData, VSClient_StreamDataCallback pFunStream = NULL, VSClient_ErrorNotify pFunError = NULL);

	/**	@fn	    evcLogout
	*	@brief	�˳�����
	*	@param  HANDLE hLogin����¼���صľ��
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
