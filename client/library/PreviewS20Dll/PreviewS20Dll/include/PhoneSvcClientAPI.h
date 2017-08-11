//lint -library
#ifndef __GUARD_PHONESVCCLIENTAPI_H_
#define __GUARD_PHONESVCCLIENTAPI_H_

#ifdef PHONESVCCLIENTAPI_EXPORTS
#define PHONESVCCLENT_API __declspec(dllexport)
#else
#define PHONESVCCLENT_API __declspec(dllimport)
#endif

// ���������� �������ݻص�����
enum phoneDataStreamType {
	PHONE_DT_STREAM_START = 0,       // ¼����ʼ
	PHONE_DT_STREAM_DATA = 1,        // ¼������
	PHONE_DT_STREAM_STOP = 2,        // ¼��ֹͣ
	PHONE_DT_STREAM_END = 500,       // ���ݽ���
	PHONE_DT_STREAM_EXCEPTION = 501, // 501
};

// ������
enum phoneErrorType {
	PHONE_ERROR_SUCCESS = 0, // û����
	PHONE_ERROR_CONNECTION,  // ���Ӵ���
	PHONE_ERROR_START,       // ��ʼ����
	PHONE_ERROR_PLAY,        // ���Ź��̴���
	PHONE_ERROR_UNKNOW,      // δ֪����
};

/**	@fn	    PSClient_ErrorNotify
*	@brief	����֪ͨԭ��
*	@param  HANDLE hLogin,��¼���
*	@param  HANDLE userdata,�û��Զ�������
*   @param  int nError,��������
*	@return	void
*/
typedef void (CALLBACK * PSClient_ErrorNotify)(HANDLE hLogin, HANDLE userdata, 
											   int nError);

/**	@fn	    PSClient_StreamDataCallback
*	@brief	���ݻص�
*	@param  HANDLE hLogin,��¼���
*	@param  HANDLE userdata,�û��Զ�������
*   @param  long datatype,��������
*   @param  void * pBuffer,���ݻ���
*   @param  DWORD dwSize,���ݴ�С
*	@return	void
*/
typedef void (CALLBACK * PSClient_StreamDataCallback)(HANDLE hLogin, HANDLE userdata, 
													  long datatype, void * pBuffer, DWORD dwSize);

#ifdef __cplusplus
extern "C"
{
#endif

	/**	@fn	    PSClient_Startup
	*	@brief	��ʼ��
	*	@param  null
	*	@return	BOOL
	*/
	PHONESVCCLENT_API BOOL PSClient_Startup(void);

	/**	@fn	    PSClient_Cleanup
	*	@brief	����ʼ��
	*	@param  null
	*	@return	BOOL
	*/
	PHONESVCCLENT_API BOOL PSClient_Cleanup(void);

	/**	@fn	    PSClient_Login
	*	@brief	��¼¼�����ģ��
	*	@param  PCSTR szIP, ������ip
	*   @param  UINT16 nPort, ���������ݼ����˿�
	*   @param  DWORD nID, ������id
	*   @param  evcStreamDataCallback pFunStream,���ݻص�����
	*   @param  evcErrorNotify pFunError,����ص�����
	*	@return	HANDLE, NULL for fail
	*/
	PHONESVCCLENT_API HANDLE PSClient_Login(PCSTR szIP, USHORT nPort, int nUserID, int nPChannel, HANDLE hUserData, PSClient_StreamDataCallback pFunStream = NULL, PSClient_ErrorNotify pFunError = NULL);

	/**	@fn	    PSClient_Logout
	*	@brief	�ǳ�¼�����ģ��
	*	@param  HANDLE hLogin����¼���صľ��
	*	@return	BOOL
	*/
	PHONESVCCLENT_API BOOL PSClient_Logout(HANDLE hLogin);

#ifdef __cplusplus
}
#endif

#endif
