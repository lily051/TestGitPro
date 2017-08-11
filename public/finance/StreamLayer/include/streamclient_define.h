/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: streamclient_define.h
Description: 结构和宏定义  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __STREAMCLIENT_DEFINE_H__
#define __STREAMCLIENT_DEFINE_H__
#include "streamgeneral_define.h"

typedef struct _ABS_TIME_
{
	HPR_UINT32 dwYear;
	HPR_UINT32 dwMonth;
	HPR_UINT32 dwDay;
	HPR_UINT32 dwHour;
	HPR_UINT32 dwMintes;
	HPR_UINT32 dwSeconds;
}ABS_TIME, *pABS_TIME;

typedef struct streamclient_sessioninfo
{
	char header[MAX_HEAD_LEN + 1];
	char baseurl[MAX_URL_LEN + 1];
	int headlen;
	int baseurllen;
	bool islocalfile;
	char mediaurl[MAX_SINGLERTSP_RTP_COUNT][MAX_URL_LEN + 1];
	int mediacount;
} STREAMCLIENT_SESSIONINFO, *PSTREAMCLIENT_SESSIONINFO;

#define HPR_STREAMCLIENT_HANDLE HPR_INT32
#define HPR_INVALID_STREAMCLIENTSESSION -1
typedef HPR_INT32 (CALLBACK *pStreamClientMsgFunc)(HPR_STREAMCLIENT_HANDLE sessionhandle, HPR_VOIDPTR userdata, HPR_INT32 opt, HPR_VOIDPTR param1, HPR_VOIDPTR param2, HPR_VOIDPTR param3, HPR_VOIDPTR param4);
typedef HPR_INT32 (CALLBACK *pStreamClientDataFunc)(HPR_STREAMCLIENT_HANDLE sessionhandle, HPR_VOIDPTR userdata, HPR_INT32 datatype, HPR_VOIDPTR pdata, HPR_INT32 datalen);

#if defined(OS_WINDOWS)
	#if defined(STREAMCLIENT_EXPORTS)
		#define HPR_STREAMCLIENT_DECLARE extern "C" __declspec(dllexport)
	#else
		#define HPR_STREAMCLIENT_DECLARE extern "C" __declspec(dllimport)
	#endif
#elif defined(OS_POSIX)
	#define HPR_STREAMCLIENT_DECLARE extern "C"
#else
	#error OS Not Implement Yet.
#endif

//兼容老版本的流媒体
typedef HPR_INT32 (CALLBACK *pDataRec)(HPR_INT32 sid, HPR_INT32 iusrdata, HPR_INT32 idatatype, char* pdata, HPR_INT32 ilen);
typedef HPR_INT32 (CALLBACK *pMsgBack)(HPR_INT32 sid, HPR_INT32 opt, HPR_INT32 param1, HPR_INT32 param2);

class IHikClientAdviseSink
{
public:
	/******************************************************************
	在Setup时被调用,获取总的播放长度.nLength为总的播放长度,以1/64秒为单位
	*/
	virtual HPR_INT32 OnPosLength(HPR_ULONG nLength) = 0;

	/******************************************************************
     在Setup后被调用,表示URL已经被成功打开,sucess为1表示成功,0表示失败
	*/
	virtual HPR_INT32 OnPresentationOpened(HPR_INT32 success) = 0;

	 /************************************************************************
     在Player被停止销毁后调用
     */
	virtual HPR_INT32 OnPresentationClosed() = 0;

	 /************************************************************************
     未使用
     */
	virtual HPR_INT32 OnPreSeek(HPR_ULONG uOldTime, HPR_ULONG uNewTime) = 0;

	 /************************************************************************
     未使用
     */
	virtual HPR_INT32 OnPostSeek(HPR_ULONG uOldTime, HPR_ULONG uNewTime) = 0;

	 /************************************************************************
     未使用
	 */	
	virtual HPR_INT32 OnStop() = 0;

	 /************************************************************************
     在Pause时被调用，uTime目前都是0
     */
	virtual HPR_INT32 OnPause(HPR_ULONG uTime) = 0;

	/************************************************************************
     在开始播放时调用，uTime目前都是0
     */
	virtual HPR_INT32 OnBegin(HPR_ULONG uTime) = 0;

     /************************************************************************
     在随机播放时调用，uTime目前都是0
     */
	virtual HPR_INT32 OnRandomBegin(HPR_ULONG uTime) = 0;

	/************************************************************************
     在Setup前调用，pszHost表示正在连接的服务器
     */
	virtual HPR_INT32 OnContacting(const char* pszHost) = 0;
    
	/************************************************************************
	在服务器端返回出错信息是调用， pError中为出错信息内容
	*/
	virtual HPR_INT32 OnPutErrorMsg(const char* pError) = 0;
	
	/************************************************************************
    未使用
     */
	virtual HPR_INT32 OnBuffering(HPR_UINT32 uFlag, HPR_UINT16 uPercentComplete) = 0;

	virtual HPR_INT32 OnChangeRate(HPR_INT32 flag) = 0;

	virtual HPR_INT32 OnDisconnect() = 0;
};

//云台控制局部区域放大
typedef struct tagPOINT_FRAME
{
      HPR_INT32 xTop;				//方框起始点的x坐标
      HPR_INT32 yTop;				//方框结束点的y坐标
      HPR_INT32 xBottom;			//方框结束点的x坐标
      HPR_INT32 yBottom;			//方框结束点的y坐标
      HPR_INT32 bCounter;			//保留
}POINT_FRAME, *LPPOINT_FRAME;

#endif