#ifndef GUARD_LIBRECGUARD_H
#define GUARD_LIBRECGUARD_H

#ifdef LIBRECGUARD_EXPORTS
#define LIBRECGUARD_API __declspec(dllexport)
#else
#define LIBRECGUARD_API __declspec(dllimport)
#endif

#include <SvcGeneralDef.h>

#ifdef __cplusplus
extern "C" {
#endif

// �����붨��
static const int libRecGuard_NO_ERROR = 0; // û�д��󣬷��������򷵻ظ���ֵ�����庬���ɸ�ģ���Զ���
// ����Ҳ���ٶ������������룬�ϲ����Щһ����֪�����ִ���Ӧ����ģ���Լ�ʹ�� hlog ���м�¼

LIBRECGUARD_API int fnlibRecGuard(void);

/**	@fn	    libRecGuard_Init
 *	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
 *          �ϲ�Ӧ�ò���Ҫ����
 *          ����ط���������������ǰ���û�����������֤
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��libRecGuard_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBRECGUARD_API int WINAPI libRecGuard_Init(void);

/**	@fn	    libRecGuard_Fini
 *	@brief	����ʼ��������������Դ�ͷ�
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��libRecGuard_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBRECGUARD_API int WINAPI libRecGuard_Fini(void);

/**	@fn	    libRecGuard_Start
 *	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
 *          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��libRecGuard_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBRECGUARD_API int WINAPI libRecGuard_Start(void);

/**	@fn	    libRecGuard_Stop
 *	@brief	ֹͣ����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��libRecGuard_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBRECGUARD_API int WINAPI libRecGuard_Stop(void);

/**	@fn	    libRecGuard_GetVersion
 *	@brief	��ȡ�汾��
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	DWORD�����ذ汾����0x01020304
 */
LIBRECGUARD_API DWORD WINAPI libRecGuard_GetVersion(void);

/**	@fn	    libRecGuard_GetLastError
 *	@brief	���ش�����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��libRecGuard_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBRECGUARD_API int WINAPI libRecGuard_GetLastError(void);

/**	@fn	    libRecGuard_SetMsgCallback
 *	@brief	����DLL��Ϣ�ص�����
 *	@param  [in] const pMsgBack msgfun, ��ʱ��Ϣ�ص�����
 *	@param  [out] ��
 *	@return	int��libRecGuard_NO_ERROR ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBRECGUARD_API int WINAPI libRecGuard_SetMsgCallback(const pTransMsgBack msgfun);
LIBRECGUARD_API int WINAPI libRecGuard_SetMsgCallbackEx(const pTransMsgBackEx msgfun);

//��ʾ��Ϣ,ע�����ΪUTF-8����
void _gShowMsgStr(const std::string  & sMsg_);

#ifdef __cplusplus
}
#endif

#endif // end of GUARD_LIBRECGUARD_H
