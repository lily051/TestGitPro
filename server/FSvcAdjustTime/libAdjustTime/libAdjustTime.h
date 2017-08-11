/**	@file    libAdjustTime.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ʱ��У׼dll�ӿ�
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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

// �����붨��
static const int ADJUSTTIME_SVC_NO_ERROR = 0; // û�д��󣬷��������򷵻ظ���ֵ�����庬���ɸ�ģ���Զ���
// ����Ҳ���ٶ������������룬�ϲ����Щһ����֪�����ִ���Ӧ����ģ���Լ�ʹ�� hlog ���м�¼

/**	@fn	    libAdjustTime_Init
 *	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
 *          �ϲ�Ӧ�ò���Ҫ����
 *          ����ط���������������ǰ���û�����������֤
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��ADJUST_ERROR_SUCC ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Init(void);

/**	@fn	    libAdjustTime_Fini
 *	@brief	����ʼ��������������Դ�ͷ�
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��ADJUST_ERROR_SUCC ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Fini(void);

/**	@fn	    libAdjustTime_Start
 *	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
 *          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��ADJUST_ERROR_SUCC ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Start(void);

/**	@fn	    libAdjustTime_ImmExec
 *	@brief	����ִ��һ��ϵͳѲ�����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��ADJUST_ERROR_SUCC ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_ImmExec(void);

/**	@fn	    libAdjustTime_Stop
 *	@brief	ֹͣ����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��ADJUST_ERROR_SUCC ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_Stop(void);

/**	@fn	    libAdjustTime_GetVersion
 *	@brief	��ȡ�汾��
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	DWORD�����ذ汾����0x01020304
 */
LIBADJUSTTIME_API DWORD WINAPI libAdjustTime_GetVersion(void);

/**	@fn	    libAdjustTime_GetLastError
 *	@brief	���ش�����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int��ADJUST_ERROR_SUCC ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_GetLastError(void);

/**	@fn	    libAdjustTime_SetMsgCallbackEx
 *	@brief	����DLL��Ϣ�ص�����
 *	@param  [in] const pMsgBack msgfun, ��ʱ��Ϣ�ص�����
 *	@param  [out] ��
 *	@return	int��ADJUST_ERROR_SUCC ��ʾû�д���������ֵ��ʾ�����˴���
 */
LIBADJUSTTIME_API int WINAPI libAdjustTime_SetMsgCallback(const pTransMsgBack msgfun);
LIBADJUSTTIME_API int WINAPI libAdjustTime_SetMsgCallbackEx(const pTransMsgBackEx msgfun);

//��ʾ��Ϣ,ע�����ΪUTF-8����
void _gShowMsgStr(const std::string  & sMsg_);

#endif //GUARD_LIBADJUSTTIME_H
