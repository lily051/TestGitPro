/***********************************************************************************
Copyright:				Hangzhou Hikvision System Technology Co.,Ltd, All Right Reserved.
FileName:				CascadeClient.h
Description:			CascadeClient
Author:					gaocong 
Date:					2011-11-26	
Contact:						
Modification History :		
************************************************************************************/

#ifndef __CASCADECLIENT_H__
#define __CASCADECLIENT_H__

#include "CascadeClient_Ext.h"    /**< �������õ��Ľṹ���ڸ�ͷ�ļ��ж����˵��*/

#ifdef __cplusplus
extern "C"
{
#endif

	#define CASC_API(type) extern "C" __declspec(dllexport)  type __stdcall


/**	@fn	int CASC_Init(const CASC_REQ_INIT* stReqInit)
 *	@brief	<CascadeClient���ʼ��>
 *	@param stReqInit ��ʼ���ṹ�壬�������������������Ϣ.
 *	@return	����0����ʼ���ɹ���-1����ʼ��ʧ��.
 */
CASC_API(int) CASC_Init(const CASC_REQ_INIT* stReqInit);


/**	@fn	void CASC_Destroy(void)
 *	@brief	<CascadeClient�����٣�����ʹ��CascadeClient��ʱ���ã���CASC_Init�ɶ�ʹ��>
 *	@param stReqInit ��ʼ���ṹ�壬�������������������Ϣ.
 */
CASC_API(void) CASC_Destroy(void);


/**	@fn	int CASC_StartRealStreamEx(const CASC_REALSTREAM_PARA* stRealStreamPara)
 *	@brief	<����ʵʱ��Ƶ������Ƶ���ڻص������з���>
 *	@param CASC_REALSTREAM_PARA ʵʱ��Ƶ���ṹ�壬��������ͨ�������Ϣ.
 *	@return	����0������ɹ�������ֵ���ڻص����������ֲ�ͬ�������ڹر���Ƶ����-1������ʧ�ܻ����.
 */
CASC_API(int) CASC_StartRealStreamEx(const CASC_REALSTREAM_PARA* stRealStreamPara);

/**	@fn	int CASC_StopRealStreamEx(const int iSessionID)
 *	@brief	<�ر�ʵʱ��Ƶ��>
 *	@param iSessionID ΪCASC_StartRealStream()����ֵ.
 *	@return	0����������ɹ���-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_StopRealStreamEx(const int iSessionID);


/**	@fn	int CASC_PTZCtrl(const CASC_REQ_PTZ* stReqPTZ)
 *	@brief	<������̨��������>
 *	@param CASC_REQ_PTZ ��̨��������ṹ�壬������̨�������������Ϣ.
 *	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ����-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_PTZCtrl(const CASC_REQ_PTZ* stReqPTZ);


/**	@fn	int CASC_GetHistoryFile(const CASC_REQ_FILELIST* stReqFileList)
 *	@brief	<��ѯ¼���ļ��б��ļ��б��ڻص������з���>
 *	@param CASC_REQ_FILELIST ��ѯ¼���ļ��ṹ�壬������ز�ѯ����.
 *	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ����-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_GetHistoryFile(const CASC_REQ_FILELIST* stReqFileList);


/**	@fn	int CASC_StartPlayBackEx(const CASC_REQ_PLAYBACK* stReqPlayBack)
*	@brief	<������ʷ��Ƶ������Ƶ���ڻص������з���>
*	@param CASC_PLAYBACKSTREAM_PARA ��ʷ��Ƶ���ṹ�壬��������ͨ�������Ϣ.
*	@return	����0������ɹ�������ֵ���ڻص����������ֲ�ͬ�������ڹر���Ƶ����-1������ʧ�ܻ����.
*/
CASC_API(int) CASC_StartPlayBackEx(const CASC_PLAYBACKSTREAM_PARA * stPlayBackStreamPara);

/**	@fn	int CASC_StopPlayBackEx(const int iSessionID)
*	@brief	<�رջط���Ƶ��>
*	@param iSessionID ΪCASC_StartPlayBack()����ֵ.
*	@return	0����������ɹ���-1����������ʧ�ܻ����.
*/
CASC_API(int) CASC_StopPlayBackEx(const int iSessionID);


/**	@fn	int CASC_StartPlayBackControlEx(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl)
*	@brief	<��ʷ��Ƶ���طſ���>
*	@param CASC_REQ_PLAYBACK_CONTROL �������������Ϣ.
*	@return	0����������ɹ�,����ʧ��.
*/
CASC_API(int) CASC_StartPlayBackControlEx(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl);


//���½ӿ���ȡ����ʽΪUDP����ʹ��SDK�����������Ч
CASC_API(int) CASC_PictureCapture(const int iSessionID, const char* pPictureName, const int iPicMode);
CASC_API(int) CASC_StartRecord(const int iSessionID, const char* pFileName);
CASC_API(int) CASC_StopRecord(const int iSessionID);
CASC_API(int) CASC_OpenSound(const int iSessionID);
CASC_API(int) CASC_CloseSound(const int iSessionID);
CASC_API(int) CASC_SetVolume(const int iSessionID, const int iVolume);



// ������ 2.3.6�汾֮ǰ���Ͻӿڡ�2.3.6��2.3.6�Ժ�汾��ʹ������ӿڡ�

/**	@fn	int CASC_GetHistoryFileUrl(const CASC_REQ_FILEURL* stReqFileUrl)
 *	@brief	<��ȡ¼���ļ��㲥URL��URL�ڻص������з���>
 *	@param CASC_REQ_FILEURL �㲥URL�ṹ�壬������ز�ѯ����.
 *	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ����-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_GetHistoryFileUrl(const CASC_REQ_FILEURL* stReqFileUrl);


/**	@fn	int CASC_StartRealStream(const CASC_REQ_REALSTREAM* stReqRealStream)
 *	@brief	<����ʵʱ��Ƶ������Ƶ���ڻص������з���>
 *	@param CASC_REQ_REALSTREAM ʵʱ��Ƶ���ṹ�壬��������ͨ�������Ϣ.
 *	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ�������ڹر���Ƶ����-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_StartRealStream(const CASC_REQ_REALSTREAM* stReqRealStream);

/**	@fn	int CASC_StopRealStream(const int iSessionID)
 *	@brief	<�ر�ʵʱ��Ƶ��>
 *	@param iSessionID ΪCASC_StartRealStream()����ֵ.
 *	@return	0����������ɹ���-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_StopRealStream(const int iSessionID);

/**	@fn	int CASC_StartPlayBack(const CASC_REQ_PLAYBACK* stReqPlayBack)
*	@brief	<������ʷ��Ƶ������Ƶ���ڻص������з���>
*	@param CASC_REQ_PLAYBACK ��ʷ��Ƶ���ṹ�壬��������ͨ�������Ϣ.
*	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ�������ڹر���Ƶ����-1����������ʧ�ܻ����.
*/
CASC_API(int) CASC_StartPlayBack(const CASC_REQ_PLAYBACK* stReqPlayBack);


/**	@fn	int CASC_StopPlayBack(const int iSessionID)
*	@brief	<�ر�ʵʱ��Ƶ��>
*	@param iSessionID ΪCASC_StartPlayBack()����ֵ.
*	@return	0����������ɹ���-1����������ʧ�ܻ����.
*/
CASC_API(int) CASC_StopPlayBack(const int iSessionID);

/**	@fn	int CASC_StartPlayBackControl(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl)
*	@brief	<��ʷ��Ƶ���طſ���>
*	@param CASC_REQ_PLAYBACK_CONTROL �������������Ϣ.
*	@return	0����������ɹ�,����ʧ��.
*/
CASC_API(int) CASC_StartPlayBackControl(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl);


/**	@fn	int CASC_GetRealMediaUrl(const CASC_REQ_REALURL* stReqRealUrl)
 *	@brief	<��ȡʵʱ��ƵURL��URL�ڻص������з���>
 *	@param CASC_REQ_REALURL ʵʱURL�ṹ�壬��������ͨ�������Ϣ.
 *	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ����-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_GetRealMediaUrl(const CASC_REQ_REALURL* stReqRealUrl);

/**	@fn	int CASC_GetPagVoiceUrl(const CASC_REQ_PAGVOICEURL* stReqPagVoiceUrl)
 *	@brief	<��ȡ�����Խ�URL��URL�ڻص������з���>
 *	@param CASC_REQ_PAGVOICEURL �����Խ�URL�ṹ�壬��������ͨ�������Ϣ.
 *	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ����-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_GetPagVoiceUrl(const CASC_REQ_PAGVOICEURL* stReqPagVoiceUrl);

/**	@fn	int CASC_CatalogQuery(const CASC_REQ_CATALOGQUERY* stReqCatalogQuery)
 *	@brief	<Ŀ¼��ѯ>
 *	@param CASC_REQ_CATALOGQUERY Ŀ¼��ѯ�ṹ�壬����Ŀ¼�����Ϣ.
 *	@return	����0����������ɹ�������ֵ���ڻص����������ֲ�ͬ����-1����������ʧ�ܻ����.
 */
CASC_API(int) CASC_CatalogQuery(const CASC_REQ_CATALOGQUERY* stReqCatalogQuery);

#ifdef __cplusplus
}
#endif

#endif
