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

// �����붨��
static const int UploadSvc_NO_ERROR = 0; // û�д��󣬷��������򷵻ظ���ֵ�����庬���ɸ�ģ���Զ���
// ����Ҳ���ٶ������������룬�ϲ����Щһ����֪�����ִ���Ӧ����ģ���Լ�ʹ�� hlog ���м�¼

UPLOADSERVICE_API int fnUploadSvc(void);

/**	@fn	    UploadSvc_Init
 *	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
 *          �ϲ�Ӧ�ò���Ҫ����
 *          ����ط���������������ǰ���û�����������֤
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��UploadSvc_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Init(void);

/**	@fn	    UploadSvc_Fini
 *	@brief	����ʼ��������������Դ�ͷ�
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��UploadSvc_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Fini(void);

/**	@fn	    UploadSvc_Start
 *	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
 *          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��UploadSvc_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Start(void);

/**	@fn	    UploadSvc_Stop
 *	@brief	ֹͣ����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��UploadSvc_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
UPLOADSERVICE_API int WINAPI UploadSvc_Stop(void);

/**	@fn	    UploadSvc_GetVersion
 *	@brief	��ȡ�汾��
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	DWORD�����ذ汾����0x01020304
 */
UPLOADSERVICE_API DWORD WINAPI UploadSvc_GetVersion(void);

/**	@fn	    UploadSvc_GetLastError
 *	@brief	���ش�����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��UploadSvc_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
UPLOADSERVICE_API int WINAPI UploadSvc_GetLastError(void);

/**	@fn	    UploadSvc_SetMsgCallback
 *	@brief	����DLL��Ϣ�ص�����
 *	@param  [in] const pTransMsgBack msgfun, ��ʱ��Ϣ�ص�����
 *	@param  [out] ��
 *	@return	int��UploadSvc_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
UPLOADSERVICE_API int WINAPI UploadSvc_SetMsgCallback(const pTransMsgBack msgfun);
//UPLOADSERVICE_API int WINAPI UploadSvc_SetMsgCallbackEx(const pTransMsgBackEx msgfun);

//��ʾ��Ϣ,ע�����ΪUTF-8����
void _gShowMsgStr(const std::string  & sMsg_);

#ifdef __cplusplus
}
#endif

#endif // end of GUARD_UPLOADSERVICE_H
