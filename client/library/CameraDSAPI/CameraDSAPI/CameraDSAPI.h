/**	@file    CameraDSAPI.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ��ȡusb����ͷ����ps��
 *
 *	@author	 chenxiaoyan3
 *	@date	 2015/07/23
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */
#ifndef GUARD_CAMERADSAPI_H
#define GUARD_CAMERADSAPI_H

#include <list>
#include <string>
#include "GeneralDef.h"

/**	@fn	    CameraDSAPI_Init
 *	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
 *          �ϲ�Ӧ�ò���Ҫ����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
  */
int WINAPI CameraDSAPI_Init(void **pCameraDS);

/**	@fn	    CameraDSAPI_Fini
 *	@brief	����ʼ��������������Դ�ͷ�
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_Fini(void **pCameraDS);

/**	@fn	    CameraDSAPI_InitCamera
 *	@brief	��ȡusb����ͷ�����Լ�����
 *	@param  [out] lstCameraInfo ����ͷ��Ϣ�б�
 *	@param  [out] lstMicInfo ��˷���Ϣ�б�
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_GetAllCameraInfo(std::list<CAMERA_INFO_ST> &lstCameraInfo, std::list<MICOR_INFO_ST> &lstMicInfo);

/**	@fn	    CameraDSAPI_InitCamera
 *	@brief	��ʼ������ͷ����
 *	@param  [in] uiCamIndex ����ͷ������
 *	@param  [in] uiMicIndex ��˷�������
 *	@param  [in] strMicName ��˷�����
 *	@param  [in] stProperty ͼ������  ��ǰ֧�ֱַ���ֻ��1920*1080 1280*720 640*480 320*240
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_InitCamera(void *pCameraDS,
                                  unsigned int uiCamIndex,
                                  unsigned int uiMicIndex,
                                  const std::string &strMicName,
                                  const PICTURE_PROPERTY_ST &stProperty
                                  );

/**	@fn	    CameraDSAPI_FiniCamera
 *	@brief	����ʼ������ͷ����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_FiniCamera(void *pCameraDS);

/**	@fn	    CameraDSAPI_Start
 *	@brief	��ʼȡ������
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_Start(void *pCameraDS);

/**	@fn	    CameraDSAPI_Pause
 *	@brief	��ͣȡ������
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_Pause(void *pCameraDS);

/**	@fn	    CameraDSAPI_Stop
 *	@brief	ֹͣȡ������
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_Stop(void *pCameraDS);

/**	@fn	    CameraDSAPI_GetRatio
 *	@brief	��ȡ���ͼ��ֱ���
 *	@param  [in] stRatioInfo ͼ��ֱ���
 *	@param  [out] stRatioInfo ͼ��ֱ���
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_GetPictureInfo(void *pCameraDS, PICTURE_PROPERTY_ST &stProperty);

/**	@fn	    CameraDSAPI_SetDataCallback
 *	@brief	���ݻص�����
 *	@param  [in] const pCameraDSDataBack datafun, ���ݻص�����
 *	@param  [in] pUser, �û�����ָ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_SetDataCallback(void *pCameraDS, const pCameraDSDataBack datafun, void *pUser);

#endif //GUARD_CAMERADSAPI_H

