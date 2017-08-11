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

#include "CascadeClient_Ext.h"    /**< 以下所用到的结构体在该头文件中定义和说明*/

#ifdef __cplusplus
extern "C"
{
#endif

	#define CASC_API(type) extern "C" __declspec(dllexport)  type __stdcall


/**	@fn	int CASC_Init(const CASC_REQ_INIT* stReqInit)
 *	@brief	<CascadeClient库初始化>
 *	@param stReqInit 初始化结构体，包含级联服务器相关信息.
 *	@return	返回0，初始化成功；-1，初始化失败.
 */
CASC_API(int) CASC_Init(const CASC_REQ_INIT* stReqInit);


/**	@fn	void CASC_Destroy(void)
 *	@brief	<CascadeClient库销毁，不再使用CascadeClient库时调用，与CASC_Init成对使用>
 *	@param stReqInit 初始化结构体，包含级联服务器相关信息.
 */
CASC_API(void) CASC_Destroy(void);


/**	@fn	int CASC_StartRealStreamEx(const CASC_REALSTREAM_PARA* stRealStreamPara)
 *	@brief	<请求实时视频流，视频流在回调函数中返回>
 *	@param CASC_REALSTREAM_PARA 实时视频流结构体，包含请求通道相关信息.
 *	@return	大于0，请求成功，返回值可在回调函数中区分不同请求，用于关闭视频流；-1，请求失败或错误.
 */
CASC_API(int) CASC_StartRealStreamEx(const CASC_REALSTREAM_PARA* stRealStreamPara);

/**	@fn	int CASC_StopRealStreamEx(const int iSessionID)
 *	@brief	<关闭实时视频流>
 *	@param iSessionID 为CASC_StartRealStream()返回值.
 *	@return	0，发送请求成功；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_StopRealStreamEx(const int iSessionID);


/**	@fn	int CASC_PTZCtrl(const CASC_REQ_PTZ* stReqPTZ)
 *	@brief	<发送云台控制命令>
 *	@param CASC_REQ_PTZ 云台控制命令结构体，包含云台控制命令相关信息.
 *	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_PTZCtrl(const CASC_REQ_PTZ* stReqPTZ);


/**	@fn	int CASC_GetHistoryFile(const CASC_REQ_FILELIST* stReqFileList)
 *	@brief	<查询录像文件列表，文件列表在回调函数中返回>
 *	@param CASC_REQ_FILELIST 查询录像文件结构体，包含相关查询条件.
 *	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_GetHistoryFile(const CASC_REQ_FILELIST* stReqFileList);


/**	@fn	int CASC_StartPlayBackEx(const CASC_REQ_PLAYBACK* stReqPlayBack)
*	@brief	<请求历史视频流，视频流在回调函数中返回>
*	@param CASC_PLAYBACKSTREAM_PARA 历史视频流结构体，包含请求通道相关信息.
*	@return	大于0，请求成功，返回值可在回调函数中区分不同请求，用于关闭视频流；-1，请求失败或错误.
*/
CASC_API(int) CASC_StartPlayBackEx(const CASC_PLAYBACKSTREAM_PARA * stPlayBackStreamPara);

/**	@fn	int CASC_StopPlayBackEx(const int iSessionID)
*	@brief	<关闭回放视频流>
*	@param iSessionID 为CASC_StartPlayBack()返回值.
*	@return	0，发送请求成功；-1，发送请求失败或错误.
*/
CASC_API(int) CASC_StopPlayBackEx(const int iSessionID);


/**	@fn	int CASC_StartPlayBackControlEx(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl)
*	@brief	<历史视频流回放控制>
*	@param CASC_REQ_PLAYBACK_CONTROL 包含请求相关信息.
*	@return	0，发送请求成功,其他失败.
*/
CASC_API(int) CASC_StartPlayBackControlEx(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl);


//以下接口在取流方式为UDP，并使用SDK解码情况下有效
CASC_API(int) CASC_PictureCapture(const int iSessionID, const char* pPictureName, const int iPicMode);
CASC_API(int) CASC_StartRecord(const int iSessionID, const char* pFileName);
CASC_API(int) CASC_StopRecord(const int iSessionID);
CASC_API(int) CASC_OpenSound(const int iSessionID);
CASC_API(int) CASC_CloseSound(const int iSessionID);
CASC_API(int) CASC_SetVolume(const int iSessionID, const int iVolume);



// 下面是 2.3.6版本之前的老接口。2.3.6及2.3.6以后版本请使用上面接口。

/**	@fn	int CASC_GetHistoryFileUrl(const CASC_REQ_FILEURL* stReqFileUrl)
 *	@brief	<获取录像文件点播URL，URL在回调函数中返回>
 *	@param CASC_REQ_FILEURL 点播URL结构体，包含相关查询条件.
 *	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_GetHistoryFileUrl(const CASC_REQ_FILEURL* stReqFileUrl);


/**	@fn	int CASC_StartRealStream(const CASC_REQ_REALSTREAM* stReqRealStream)
 *	@brief	<请求实时视频流，视频流在回调函数中返回>
 *	@param CASC_REQ_REALSTREAM 实时视频流结构体，包含请求通道相关信息.
 *	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求，用于关闭视频流；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_StartRealStream(const CASC_REQ_REALSTREAM* stReqRealStream);

/**	@fn	int CASC_StopRealStream(const int iSessionID)
 *	@brief	<关闭实时视频流>
 *	@param iSessionID 为CASC_StartRealStream()返回值.
 *	@return	0，发送请求成功；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_StopRealStream(const int iSessionID);

/**	@fn	int CASC_StartPlayBack(const CASC_REQ_PLAYBACK* stReqPlayBack)
*	@brief	<请求历史视频流，视频流在回调函数中返回>
*	@param CASC_REQ_PLAYBACK 历史视频流结构体，包含请求通道相关信息.
*	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求，用于关闭视频流；-1，发送请求失败或错误.
*/
CASC_API(int) CASC_StartPlayBack(const CASC_REQ_PLAYBACK* stReqPlayBack);


/**	@fn	int CASC_StopPlayBack(const int iSessionID)
*	@brief	<关闭实时视频流>
*	@param iSessionID 为CASC_StartPlayBack()返回值.
*	@return	0，发送请求成功；-1，发送请求失败或错误.
*/
CASC_API(int) CASC_StopPlayBack(const int iSessionID);

/**	@fn	int CASC_StartPlayBackControl(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl)
*	@brief	<历史视频流回放控制>
*	@param CASC_REQ_PLAYBACK_CONTROL 包含请求相关信息.
*	@return	0，发送请求成功,其他失败.
*/
CASC_API(int) CASC_StartPlayBackControl(const CASC_REQ_PLAYBACK_CONTROL* stReqPlayBackControl);


/**	@fn	int CASC_GetRealMediaUrl(const CASC_REQ_REALURL* stReqRealUrl)
 *	@brief	<获取实时视频URL，URL在回调函数中返回>
 *	@param CASC_REQ_REALURL 实时URL结构体，包含请求通道相关信息.
 *	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_GetRealMediaUrl(const CASC_REQ_REALURL* stReqRealUrl);

/**	@fn	int CASC_GetPagVoiceUrl(const CASC_REQ_PAGVOICEURL* stReqPagVoiceUrl)
 *	@brief	<获取语音对讲URL，URL在回调函数中返回>
 *	@param CASC_REQ_PAGVOICEURL 语音对讲URL结构体，包含请求通道相关信息.
 *	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_GetPagVoiceUrl(const CASC_REQ_PAGVOICEURL* stReqPagVoiceUrl);

/**	@fn	int CASC_CatalogQuery(const CASC_REQ_CATALOGQUERY* stReqCatalogQuery)
 *	@brief	<目录查询>
 *	@param CASC_REQ_CATALOGQUERY 目录查询结构体，包含目录相关信息.
 *	@return	大于0，发送请求成功，返回值可在回调函数中区分不同请求；-1，发送请求失败或错误.
 */
CASC_API(int) CASC_CatalogQuery(const CASC_REQ_CATALOGQUERY* stReqCatalogQuery);

#ifdef __cplusplus
}
#endif

#endif
