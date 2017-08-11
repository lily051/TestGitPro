/**	@file StreamClient.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 流媒体客户端。
 *
 *	@author		zhaojian
 *	@date		2012/07/30
 *
 *	@note 历史记录：
                    2013/06/10 升级流媒体客户端版本至4.1.0，增加详细日志和注释，重构代码
 *	@note 
 *
 *	@warning 
 */
#ifndef __STREAMCLIENT_H__
#define __STREAMCLIENT_H__

// 传输方式
#define RTPRTSP_TRANSMODE  0x9000			// RTP over RTSP
#define RTPUDP_TRANSMODE   0x9002			// RTP over UDP
#define RTPMCAST_TRANSMODE 0x9004			// RTP over multicast

// 流数据类型
#define STREAM_HEAD         0x0001          // 系统头数据
#define STREAM_DATA         0x0002          // 流数据
#define STREAM_PLAYBACK_FINISH 0x0064       // 回放、下载或倒放结束标识

/**	@struct _ABS_TIME_
 *  @brief 绝对时间回放时间参数结构体。
 *
 */
typedef struct _ABS_TIME_
{
    unsigned int dwYear;    ///< 年
    unsigned int dwMonth;   ///< 月
    unsigned int dwDay;     ///< 日
    unsigned int dwHour;    ///< 时
    unsigned int dwMintes;  ///< 分
    unsigned int dwSeconds; ///< 秒
}ABS_TIME, *pABS_TIME;

/**	@struct tagPOINT_FRAME
 *  @brief 云台控制局部区域放大。
 *
 * 为兼容老版本，未使用
 */
typedef struct tagPOINT_FRAME
{
    int xTop;
    int yTop;				
    int xBottom;			
    int yBottom;			
    int bCounter;			
}POINT_FRAME, *LPPOINT_FRAME;

// 兼容老版本的流媒体（未使用）
class IHikClientAdviseSink;

#if (defined(WIN32) || defined(WIN64))
    #if defined(STREAMCLIENT_EXPORTS)
        #define HPR_STREAMCLIENT_DECLARE extern "C" __declspec(dllexport)
    #else
        #define HPR_STREAMCLIENT_DECLARE extern "C" __declspec(dllimport)
    #endif
    #ifndef CALLBACK
        #define CALLBACK __stdcall
    #endif
#else
    #define HPR_STREAMCLIENT_DECLARE extern "C"
    #ifndef CALLBACK
        #define CALLBACK
    #endif
#endif

/**************************/
/*消息回调函数参数定义*/
/**********************************************
sessionhandle 会话句柄
userdata      用户自定义数据
errCode       错误码
param1        暂时未定义
param2        暂时未定义
param3        暂时未定义
param4        暂时未定义
成功返回0 失败返回-1
************************************************/
typedef int (CALLBACK *pStreamClientMsgFunc)(int sessionhandle, void* userdata, int errCode, void* param1, void* param2, void* param3, void* param4);

/**************************/
/*原始数据（从流媒体服务器接收的数据）回调函数参数定义*/
/***********************************************
sessionhandle 会话句柄
userdata      用户自定义数据
pdata         码流数据（STREAM_HEAD 系统头数据，STREAM_DATA 流数据，STREAM_PLAYBACK_FINISH 回放、下载或倒放结束）
datalen       码流数据长度
成功返回0 失败返回-1
***********************************************/
typedef int (CALLBACK *pStreamClientDataFunc)(int sessionhandle, void* userdata, int datatype, void* pdata, int datalen);

/**************************/
/*PS封装数据回调函数参数定义，当接收的码流支持转封装成PS，此数据回调会有数据回调。*/
/***********************************************
sessionhandle 会话句柄
userdata      用户自定义数据
pdata         码流数据（STREAM_HEAD 系统头数据，STREAM_DATA 流数据，STREAM_PLAYBACK_FINISH 回放、下载或倒放结束）
datalen       码流数据长度
成功返回0 失败返回-1
***********************************************/
typedef int (CALLBACK *pStreamClientPsDataFunc)(int sessionhandle, void* userdata, int datatype, void* pdata, int datalen);

//兼容老版本的流媒体（未使用）
typedef int (CALLBACK *pDataRec)(int sid, int iusrdata, int idatatype, char* pdata, int ilen);
typedef int (CALLBACK *pMsgBack)(int sid, int opt, int param1, int param2);

// 兼容老版本的流媒体（未实现）
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetOcxFlag(bool bOcx);

/**	@fn	HPR_STREAMCLIENT_DECLARE HPR_INT32 CALLBACK StreamClient_InitLib(HPR_VOID)
 *	@brief 初始化流媒体客户端，有计数功能。
 *	@param 无.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_InitLib(void);

/**	@fn	HPR_STREAMCLIENT_DECLARE HPR_INT32 CALLBACK StreamClient_FiniLib(HPR_VOID)
 *	@brief 反初始化流媒体客户端，有计数功能。
 *	@param 无.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_FiniLib(void);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_CreateSession(void)
 *	@brief 创建流媒体客户端会话句柄
 *	@param 无.
 *	@return	成功返回会话句柄大于等于零，失败返回负数
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_CreateSession(void);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_DestroySession(int sessionhandle)
 *	@brief 销毁会话
 *	@param sessionhandle[in]  需要销毁的会话句柄（StreamClient_CreateSession函数返回值）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_DestroySession(int sessionhandle);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Start(int sessionhandle, void* windowhandle, char *url, char *useragent, int transmethod,\
                                                                char *deviceusername, char *devicepasswd)
 *	@brief 开启实时预览
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param windowhandle[in]     窗口句柄（未使用，传NULL）.
 *	@param url[in]              实时预览url（url格式参见流媒体4.0客户端软件组件使用说明书.doc）.
 *	@param useragent[in]        客户端名称（比如：StreamClient）.
 *	@param transmethod[in]      传输方式（RTPRTSP_TRANSMODE、RTPUDP_TRANSMODE）.
 *	@param deviceusername[in]   实时预览设备的用户名.
 *	@param devicepasswd[in]     实时预览设备的密码.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Start(int sessionhandle, void* windowhandle, char *url, char *useragent, int transmethod,\
												  char *deviceusername, char *devicepasswd);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_DownLoad(int sessionhandle, void* windowhandle, char* url, char* useragent, int transmethod, \
                                                                    char *deviceusername, char *devicepasswd, pABS_TIME from = NULL, pABS_TIME to = NULL)
 *	@brief 开启下载功能
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param windowhandle[in]     窗口句柄（未使用，传NULL）.
 *	@param url[in]              下载url（url格式参见流媒体4.0客户端软件组件使用说明书.doc）.
 *	@param useragent[in]        客户端名称（比如：StreamClient）.
 *	@param transmethod[in]      传输方式（RTPRTSP_TRANSMODE、RTPUDP_TRANSMODE）.
 *	@param deviceusername[in]   下载录像对应设备的用户名.
 *	@param devicepasswd[in]     下载录像对应设备的密码.
 *	@param from[in]             下载录像文件的开始日期.
 *	@param to[in]               下载录像文件的结束日期.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_DownLoad(int sessionhandle, void* windowhandle, char* url, char* useragent, int transmethod, \
												  char *deviceusername, char *devicepasswd, pABS_TIME from = NULL, pABS_TIME to = NULL);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PlayBackByTime(int sessionhandle, void* windowhandle, char* url, char* useragent, int transmethod, \
                                                                          char *deviceusername, char *devicepasswd, pABS_TIME from, pABS_TIME to)
 *	@brief 按绝对时间开启回放
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param windowhandle[in]     窗口句柄（未使用，传NULL）.
 *	@param url[in]              绝对时间回放url（url格式参见流媒体4.0客户端软件组件使用说明书.doc）.
 *	@param useragent[in]        客户端名称（比如：StreamClient）.
 *	@param transmethod[in]      传输方式（RTPRTSP_TRANSMODE、RTPUDP_TRANSMODE）.
 *	@param deviceusername[in]   绝对时间回放对应设备的用户名.
 *	@param devicepasswd[in]     绝对时间回放对应设备的密码.
 *	@param from[in]             绝对时间回放录像文件的开始日期.
 *	@param to[in]               绝对时间回放录像文件的结束日期.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PlayBackByTime(int sessionhandle, void* windowhandle, char* url, char* useragent, int transmethod, \
												  char *deviceusername, char *devicepasswd, pABS_TIME from, pABS_TIME to);

// 兼容老版本的流媒体（未实现）
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Describe(int sessionhandle, void* windowhandle, char *url, char *useragent, int transmethod,\
                                                                  char *deviceusername, char *devicepasswd);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Setup(int sessionhandle);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Play(int sessionhandle);
// end

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Stop(int sessionhandle)
 *	@brief 停止取流操作（实时预览、下载、回放等）
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Stop(int sessionhandle);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Pause(int sessionhandle)
 *	@brief 暂停取流操作（下载、回放等）
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Pause(int sessionhandle);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Resume(int sessionhandle)
 *	@brief 恢复取流操作（下载、回放等）
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Resume(int sessionhandle);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_RandomPlay(int sessionhandle, int from, int to)
 *	@brief 相对时间回放seek操作
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param from[in]             相对时间回放seek的开始时间.
 *	@param to[in]               相对时间回放seek的结束时间.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_RandomPlay(int sessionhandle, int from, int to);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_RandomPlayByAbs(int sessionhandle, pABS_TIME from, pABS_TIME to)
 *	@brief 绝对时间回放seek操作
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param from[in]             绝对时间回放seek的开始时间.
 *	@param to[in]               绝对时间回放seek的结束时间.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_RandomPlayByAbs(int sessionhandle, pABS_TIME from, pABS_TIME to);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_ChangeRate(int sessionhandle, float scale)
 *	@brief 修改回放或下载的倍率
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param scale[in]            倍率值（倍率有效值参见流媒体4.0客户端软件组件使用说明书.doc）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_ChangeRate(int sessionhandle, float scale);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetMsgCallBack(int sessionhandle, pStreamClientMsgFunc pMsgFunc, void* userdata)
 *	@brief 设置消息回调
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param pMsgFunc[in]         消息回调函数指针.
 *	@param userdata[in]         用户自定义数据.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetMsgCallBack(int sessionhandle, pStreamClientMsgFunc pMsgFunc, void* userdata);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetDataCallBack(int sessionhandle, pStreamClientDataFunc pDataFunc, void* userdata)
 *	@brief 设置数据回调
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param pDataFunc[in]        数据回调函数指针.
 *	@param userdata[in]         用户自定义数据.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetDataCallBack(int sessionhandle, pStreamClientDataFunc pDataFunc, void* userdata);

// 兼容老版本的流媒体（未实现）
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Set_DevicePushdata(int sessionhandle, bool buse = false);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Set_StandardStream(int sessionhandle, bool bstandardstream = false);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetRTPTCPPortRange(unsigned short count,\
                                                                            unsigned short baseport);
// end

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetRTPUDPPortRange(unsigned short count,\
                                                                              unsigned short baseport)
 *	@brief 设置UDP端口范围
 *	@param count[in]    端口对数.
 *	@param baseport[in] 起始端口.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetRTPUDPPortRange(unsigned short count,\
																	  unsigned short baseport);

// 兼容老版本的流媒体（未实现）
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetRTPUDPRetrans(int sessionhandle, bool buse);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetPortShare(int level);
// end

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Backward(int sessionhandle, pABS_TIME pfrom, pABS_TIME pto = NULL)
 *	@brief 倒放
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param pfrom[in]            倒放开始时间.
 *	@param pto[in]              倒放开始时间.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_Backward(int sessionhandle, pABS_TIME pfrom, pABS_TIME pto = NULL);

// 兼容老版本的流媒体（未实现）
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_GetCurTime(int sessionhandle, unsigned long *utime);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetVolume(int sessionhandle, unsigned short volume);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_OpenSound(int sessionhandle, bool bExclusive = false);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_CloseSound(int sessionhandle);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_ThrowBFrameNum(int sessionhandle, unsigned int nNum);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_GrabPic(int sessionhandle, const char* szPicFileName,\
                                                                 unsigned short byPicType);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_ResetWndHandle(int sessionhandle, void* pWndSiteHandle);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_RefreshPlay(int sessionhandle);
//HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_RegisterDrawFun(int sessionhandle,\
//                                                                         void(CALLBACK *DrawFun)(long nPort, HDC hDC, long nUser), long nUser);
// end

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_GetVideoParams(int sessionhandle, int *ibri, int *icon, int *isat, int *ihue)
 *	@brief 获取视频参数
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param ibri[out]            亮度指针，取值范围[1,10].
 *	@param icon[out]            对比度指针，取值范围[1,10].
 *	@param isat[out]            饱和度指针，取值范围[1,10].
 *	@param ihue[out]            色度指针，取值范围[1,10].
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_GetVideoParams(int sessionhandle, int *ibri, int *icon, int *isat, int *ihue);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetVideoParams(int sessionhandle, int ibri, int icon, int isat, int ihue)
 *	@brief 设置视频参数
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param ibri[in]             亮度，取值范围[1,10].
 *	@param icon[in]             对比度，取值范围[1,10].
 *	@param isat[in]             饱和度，取值范围[1,10].
 *	@param ihue[in]             色度，取值范围[1,10].
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetVideoParams(int sessionhandle, int ibri, int icon, int isat, int ihue);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PTZControl(int sessionhandle, unsigned int ucommand, int iparam1, int iparam2, int iparam3, int iparam4)
 *	@brief 云台控制
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param ucommand[in]         云台控制命令.
 *	@param iparam1[in]          开始停止命令（0：开始，1：停止）.
 *	@param iparam2[in]          未定义.
 *	@param iparam3[in]          未定义.
 *	@param iparam4[in]          未定义.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PTZControl(int sessionhandle, unsigned int ucommand, int iparam1, int iparam2, int iparam3, int iparam4);

// 兼容老版本的流媒体（未实现）
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PTZSelZoomIn(int sessionhandle, LPPOINT_FRAME pStruPointFrame);
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_OneByOne(int sessionhandle);
// end

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_ForceIFrame(int sessionhandle)
 *	@brief 强制I帧
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_ForceIFrame(int sessionhandle);

// 兼容老版本的流媒体（未实现）
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_InputAudioData(int sessionhandle, char* pDataBuf, int iDataSize);

HPR_STREAMCLIENT_DECLARE int CALLBACK InitStreamClientLib(void);
HPR_STREAMCLIENT_DECLARE int CALLBACK FiniStreamClientLib(void);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_CreatePlayer(IHikClientAdviseSink* pSink, void *pWndSiteHandle, pDataRec pRecFunc, pMsgBack pMsgFunc = 0, int TransMethod = 0);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_OpenURL(int hSession, const char* pszURL, int iusrdata);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_Play(int hSession);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_RandomPlay(int hSession, unsigned long timepos);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_Pause(int hSession);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_Resume(int hSession);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_Stop(int hSession);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_ChangeRate(int hSession, int scale);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_Destroy(int hSession);

HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_GetVideoParams(int hSession, int *ibri, int *icon, int *isat, int *ihue);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_SetVideoParams(int hSession, int ibri, int icon, int isat, int ihue);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_PTZControl(int hSession, unsigned int ucommand, int iparam1, int iparam2, int iparam3, int iparam4);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_PTZSelZoomIn(int hSession, LPPOINT_FRAME pStruPointFrame);

HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_GetCurTime(int hSession, unsigned long* utime);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_SetVolume(int hSession, unsigned short volume);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_OpenSound(int hSession, bool bExclusive = false);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_CloseSound(int hSession);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_ThrowBFrameNum(int hSession, unsigned int nNum);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_GrabPic(int hSession, const char* szPicFileName, unsigned short byPicType);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_ResetWndHandle(int hSession, void *pWndSiteHandle);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_ResetDataCallBack(int hSession, pDataRec pRecFunc);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_GetHeader(int hSession, char* buf, int len);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_RefreshPlay(int hSession);
HPR_STREAMCLIENT_DECLARE bool CALLBACK HIKS_GetPlayPort(int hSession, unsigned long* port);
HPR_STREAMCLIENT_DECLARE bool CALLBACK HIKS_SetDDrawDevice(int hSession, unsigned long nDeviceNum);
HPR_STREAMCLIENT_DECLARE bool CALLBACK HIKS_InitDDrawDevice(void);
HPR_STREAMCLIENT_DECLARE void CALLBACK HIKS_ReleaseDDrawDevice(void);

HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_SetOcxFlag(bool bOcx);
HPR_STREAMCLIENT_DECLARE int CALLBACK HIKS_ForceIFrame(int hSession);
// end

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetClusterAddr(char* rootaddrs)
 *	@brief 设置集群地址
 *	@param rootaddrs[in]    集群地址.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetClusterAddr(char* rootaddrs);
//end

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PlayBackByOppositeTime(int sessionhandle, char* url, char* useragent, int transmethod, \
                                                                                  char *deviceusername, char *devicepasswd, int from, int to)
 *	@brief 按相对时间开启回放
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param windowhandle[in]     窗口句柄（未使用，传NULL）.
 *	@param url[in]              相对时间回放url（url格式参见流媒体4.0客户端软件组件使用说明书.doc）.
 *	@param useragent[in]        客户端名称（比如：StreamClient）.
 *	@param transmethod[in]      传输方式（RTPRTSP_TRANSMODE、RTPUDP_TRANSMODE）.
 *	@param deviceusername[in]   相对时间回放对应设备的用户名.
 *	@param devicepasswd[in]     相对时间回放对应设备的密码.
 *	@param from[in]             相对时间回放录像文件的开始日期.
 *	@param to[in]               相对时间回放录像文件的结束日期（-1表示播放到文件结束）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PlayBackByOppositeTime(int sessionhandle, char* url, char* useragent, int transmethod, \
                                                                          char *deviceusername, char *devicepasswd, int from, int to);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetPsDataCallBack(int sessionhandle, pStreamClientDataFunc pDataFunc, void* userdata)
 *	@brief 设置PS封装数据回调，若码流可以转成PS，此函数指针会有码流回调。
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param pDataFunc[in]        数据回调函数指针.
 *	@param userdata[in]         用户自定义数据.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetPsDataCallBack(int sessionhandle, pStreamClientPsDataFunc pDataFunc, void* userdata);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_GetErrMsgByErrCode(int errCode)
 *	@brief 根据错误码获取错误描述信息
 *	@param errCode[in]  错误码.
 *	@return	错误描述信息
 */
HPR_STREAMCLIENT_DECLARE const char* CALLBACK StreamClient_GetErrMsgByErrCode(int errCode);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_BackwardEx(int sessionhandle, const char* url, const char* useragent, int transmethod, \
                                                                      const char *deviceusername, const char *devicepasswd, pABS_TIME pfrom, pABS_TIME pto)
 *	@brief 开启倒放
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param url[in]              绝对时间倒放url（url格式参见流媒体4.0客户端软件组件使用说明书.doc）.
 *	@param useragent[in]        客户端名称（比如：StreamClient）.
 *	@param transmethod[in]      传输方式（RTPRTSP_TRANSMODE、RTPUDP_TRANSMODE）.
 *	@param deviceusername[in]   倒放对应设备的用户名.
 *	@param devicepasswd[in]     倒放对应设备的密码.
 *	@param pfrom[in]            倒放录像文件的开始日期.
 *	@param pto[in]              倒放录像文件的结束日期.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_BackwardEx(int sessionhandle, const char* url, const char* useragent, int transmethod, \
                                                              const char *deviceusername, const char *devicepasswd, pABS_TIME pfrom, pABS_TIME pto);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_BackwardRandomPlay(int sessionhandle, pABS_TIME from, pABS_TIME to)
 *	@brief 倒放seek操作
 *	@param sessionhandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param pfrom[in]             倒放seek的开始时间.
 *	@param pto[in]               倒放seek的结束时间.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_BackwardRandomPlay(int sessionhandle, pABS_TIME pfrom, pABS_TIME pto);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PushData(int sessionHandle, const char *url, const char *userAgent, \
                                                                    const char *deviceUsername, const char *devicePasswd, const char* destIp, \
                                                                    unsigned short destPort, int transMethod = RTPUDP_TRANSMODE)
 *	@brief 发送推流请求
 *	@param sessionHandle[in]    会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param url[in]              实时预览URL.
 *	@param userAgent[in]        客户端名称（比如：StreamClient）.
 *	@param deviceUsername[in]   用户名.
 *	@param devicePasswd[in]     密码.
 *	@param destIp[in]           目标IP.
 *	@param destPort[in]         目标端口.
 *	@param transMethod[in]      传输方式（仅支持RTPUDP_TRANSMODE）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_PushData(int sessionHandle, const char* url, const char* userAgent, \
                                                            const char* deviceUsername, const char* devicePasswd, const char* destIp, \
                                                            unsigned short destPort, int transMethod = RTPUDP_TRANSMODE);

/**	@fn	HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetExtractFrame(int sessionhandle, int iExtractFrame)
 *	@brief 抽帧/不抽帧操作（用于回放）
 *	@param sessionhandle[in]  会话句柄（StreamClient_CreateSession函数返回值）.
 *	@param iExtractFrame[in]  抽帧标识符（0：不抽帧，1：抽帧）.
 *	@return	成功返回0，失败返回错误码（详细解释参数错误码文档）
 */
HPR_STREAMCLIENT_DECLARE int CALLBACK StreamClient_SetExtractFrame(int sessionhandle, int iExtractFrame);

// 函数返回值错误码定义
#define STREAM_CLIENT_NO_INIT                       -2  ///< 流媒体客户端未初始化
#define STREAM_CLIENT_ERR_NO_DEF                    -1  ///< 错误未定义
#define STREAM_CLIENT_NOERROR                       0   ///< 没有错误
#define STREAM_CLIENT_SAESSION_INVALID              1   ///< 会话无效
#define STREAM_CLIENT_OVER_MAX_CONN                 2   ///< 超出流媒体用户个数超过最大
#define STREAM_CLIENT_NETWORK_FAIL_CONNECT          3   ///< 连接设备失败。设备不在线或网络原因引起的连接超时等。
#define STREAM_CLIENT_DEVICE_OFF_LIEN               4   ///< 设备掉线
#define STREAM_CLIENT_DEVICE_OVER_MAX_CONN          5   ///< 设备超过最大连接数
#define STREAM_CLIENT_RECV_ERROR                    6   ///< 从流媒体服务器接收数据失败
#define STREAM_CLIENT_RECV_TIMEOUT                  7   ///< 从流媒体服务器接收数据超时
#define STREAM_CLIENT_SEND_ERROR                    8   ///< 向流媒体服务器发送数据失败
#define STREAM_CLIENT_TRANSMETHOD_INVALID           9   ///< 传输方式无效（不是RTP/RTSP，RTP/UDP，RTP/MCAST）
#define STREAM_CLIENT_CREATESOCKET_ERROR            10  ///< 创建SOCKET失败
#define STREAM_CLIENT_SETSOCKET_ERROR               11  ///< 设置SOCKET失败
#define STREAM_CLIENT_BINDSOCKET_ERROR              12  ///< 绑定SOCKET失败
#define STREAM_CLIENT_LISTENSOCKET_ERROR            13  ///< 监听SOCKET失败
#define STREAM_CLIENT_URL_FORMAT_ERROR              14  ///< URL格式错误
#define STREAM_CLIENT_RTSP_STATE_INVALID            15  ///< RTSP状态无效
#define STREAM_CLIENT_RTSP_RSP_ERROR                16  ///< RTSP回应错误（语法错误，未包含必须的字段）
#define STREAM_CLIENT_RTSP_RSP_STATE_ERROR          17  ///< RTSP返回状态失败，不等于200或302
#define STREAM_CLIENT_PARSE_SDP_FAIL                18  ///< 解析SDP失败
#define STREAM_CLIENT_PARSE_RTSP_FAIL               19  ///< 解析RTSP信令失败（比如：获取RTSP某字段失败，RTSP）
#define STREAM_CLIENT_MEDIACOUNT_LESS_ZERO          20  ///< 解析SDP时，未解析到traceID
#define STREAM_CLIENT_SEND_DESCRIBE_FAIL            21  ///< 发送describe信令失败
#define STREAM_CLIENT_SEND_SETUP_FAIL               22  ///< 发送setup信令失败
#define STREAM_CLIENT_SEND_PLAY_FAIL                23  ///< 发送play信令失败
#define STREAM_CLIENT_SEND_PAUSE_FAIL               24  ///< 发送pause信令失败
#define STREAM_CLIENT_SEND_TRERDOWN_FAIL            25  ///< 发送teardown信令失败
#define STREAM_CLIENT_RECV_DESCRIBE_TIMEOUT         26  ///< 接收describe超时
#define STREAM_CLIENT_RECV_SETUP_TIMEOUT            27  ///< 接收setup超时
#define STREAM_CLIENT_RECV_PLAY_TIMEOUT             28  ///< 接收play超时
#define STREAM_CLIENT_RECV_PAUSE_TIMEOUT            29  ///< 接收pause超时
#define STREAM_CLIENT_RECV_TEARDOWN_TIMEOUT         30  ///< 接收teardown超时
#define STREAM_CLIENT_DESCRIBE_RSP_ERROR            31  ///< describe响应错误
#define STREAM_CLIENT_SETUP_RSP_ERROR               32  ///< setup响应错误
#define STREAM_CLIENT_PLAY_RSP_ERROR                33  ///< play响应错误
#define STREAM_CLIENT_PAUSE_RSP_ERROR               34  ///< pause响应错误
#define STREAM_CLIENT_TEARDOWN_RSP_ERROR            35  ///< teardown响应错误
#define STREAM_CLIENT_REDIRECT_ERROR                36  ///< 重定向失败
#define STREAM_CLIENT_UDP_GET_SERVER_PROT_FAIL	    37  ///< 从RTSP的setup信令解析服务器端口失败
#define STREAM_CLIENT_CREATE_UPD_CONNECT_FAIL	    38  ///< 创建UDP异步网络连接失败
#define STREAM_CLIENT_OPEN_UDP_CONNECT_FAIL	        39  ///< 打开UDP异步网络连接失败
#define STREAM_CLIENT_UDP_ASYNC_RECV_FAIL	        40  ///< UDP投递异步接收请求失败
#define STREAM_CLIENT_OPT_BACK_DATE_ERROR	        41	 ///< 相对时间回放时间错误
#define STREAM_CLIENT_ABS_BACK_DATE_ERROR	        42	 ///< 绝对时间回放时间错误
#define STREAM_CLIENT_MSG_CB_INVALID                43  ///< 消息回调设置错误
#define STREAM_CLIENT_SEND_PTZ_FAILED               44  ///< 发送云台控制信令失败
#define STREAM_CLIENT_SEND_FORCEIFRAM_FAILED        45  ///< 发送强制I帧信令失败
#define STREAM_CLIENT_SEND_GETVEDIOPARAM_FAILED     46  ///< 发送获取视频参数信令失败
#define STREAM_CLIENT_SEND_SETVEDIOPARAM_FAILED     47  ///< 发送设置视频参数信令失败
#define STREAM_CLIENT_RECV_PTZ_TIMEOUT              48  ///< 接收云台控制信令超时
#define STREAM_CLIENT_RECV_FORCEIFRAM_TIMEOUT       49  ///< 接收强制I帧信令超时
#define STREAM_CLIENT_RECV_GETVEDIOPARAM_TIMEOUT    50  ///< 接收获取视频参数信令超时
#define STREAM_CLIENT_RECV_SETVEDIOPARAM_TIMEOUT    51  ///< 接收设置视频参数信令超时
#define STREAM_CLIENT_FUNCTION_NO_ACHIEVE           52  ///< 函数未实现
#define STREAM_CLIENT_CONFIG_RTSP_SESSION_FAILED    53  ///< 配置RTSP会话时，某参数无效
#define STREAM_CLIENT_FUNC_PARAM_INVALID            54  ///< 函数参数无效
#define STREAM_CLIENT_SESSION_POINTER_INVALID       55  ///< 会话指针无效
#define STREAM_CLIENT_MEMORY_LACK                   56  ///< 内存不足或申请内存失败
#define STREAM_CLIENT_SEND_SETPARAMETER_FAILED      57  ///< 发送设置参数信令失败
#define STREAM_CLIENT_RECV_SETPARAMETER_TIMEOUT     58  ///< 接收设置参数信令超时
#define STREAM_CLIENT_SEND_HEARTBEAT_FAILED         59  ///< 发送心跳信令失败
#define STREAM_CLIENT_RECV_HEARTBEAT_TIMEOUT        60  ///< 接收心跳信令超时
#define STREAM_CLIENT_PUSHDATA_TRANSMETHOD_INVALID  61  ///< 推流传输方式无效（仅支持RTPUDP_TRANSMODE）

// 消息回调错误定义
#define STREAM_CLIENT_SEND_HEARTBEAT_FAIL   100  ///< 发送心跳失败（未使用）
#define STREAM_CLIENT_HEARTBEAT_TIMEOUT     101  ///< 心跳超时（未使用）
#define STREAM_CLIENT_NOT_SUPPORT_PS_STREAM 102  ///< 不支持转封装成PS码流标识，消息数据回调中使用

#endif
