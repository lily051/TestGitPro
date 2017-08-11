/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: streamclient_define.h
Description: �ṹ�ͺ궨��  	 
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

//�����ϰ汾����ý��
typedef HPR_INT32 (CALLBACK *pDataRec)(HPR_INT32 sid, HPR_INT32 iusrdata, HPR_INT32 idatatype, char* pdata, HPR_INT32 ilen);
typedef HPR_INT32 (CALLBACK *pMsgBack)(HPR_INT32 sid, HPR_INT32 opt, HPR_INT32 param1, HPR_INT32 param2);

class IHikClientAdviseSink
{
public:
	/******************************************************************
	��Setupʱ������,��ȡ�ܵĲ��ų���.nLengthΪ�ܵĲ��ų���,��1/64��Ϊ��λ
	*/
	virtual HPR_INT32 OnPosLength(HPR_ULONG nLength) = 0;

	/******************************************************************
     ��Setup�󱻵���,��ʾURL�Ѿ����ɹ���,sucessΪ1��ʾ�ɹ�,0��ʾʧ��
	*/
	virtual HPR_INT32 OnPresentationOpened(HPR_INT32 success) = 0;

	 /************************************************************************
     ��Player��ֹͣ���ٺ����
     */
	virtual HPR_INT32 OnPresentationClosed() = 0;

	 /************************************************************************
     δʹ��
     */
	virtual HPR_INT32 OnPreSeek(HPR_ULONG uOldTime, HPR_ULONG uNewTime) = 0;

	 /************************************************************************
     δʹ��
     */
	virtual HPR_INT32 OnPostSeek(HPR_ULONG uOldTime, HPR_ULONG uNewTime) = 0;

	 /************************************************************************
     δʹ��
	 */	
	virtual HPR_INT32 OnStop() = 0;

	 /************************************************************************
     ��Pauseʱ�����ã�uTimeĿǰ����0
     */
	virtual HPR_INT32 OnPause(HPR_ULONG uTime) = 0;

	/************************************************************************
     �ڿ�ʼ����ʱ���ã�uTimeĿǰ����0
     */
	virtual HPR_INT32 OnBegin(HPR_ULONG uTime) = 0;

     /************************************************************************
     ���������ʱ���ã�uTimeĿǰ����0
     */
	virtual HPR_INT32 OnRandomBegin(HPR_ULONG uTime) = 0;

	/************************************************************************
     ��Setupǰ���ã�pszHost��ʾ�������ӵķ�����
     */
	virtual HPR_INT32 OnContacting(const char* pszHost) = 0;
    
	/************************************************************************
	�ڷ������˷��س�����Ϣ�ǵ��ã� pError��Ϊ������Ϣ����
	*/
	virtual HPR_INT32 OnPutErrorMsg(const char* pError) = 0;
	
	/************************************************************************
    δʹ��
     */
	virtual HPR_INT32 OnBuffering(HPR_UINT32 uFlag, HPR_UINT16 uPercentComplete) = 0;

	virtual HPR_INT32 OnChangeRate(HPR_INT32 flag) = 0;

	virtual HPR_INT32 OnDisconnect() = 0;
};

//��̨���ƾֲ�����Ŵ�
typedef struct tagPOINT_FRAME
{
      HPR_INT32 xTop;				//������ʼ���x����
      HPR_INT32 yTop;				//����������y����
      HPR_INT32 xBottom;			//����������x����
      HPR_INT32 yBottom;			//����������y����
      HPR_INT32 bCounter;			//����
}POINT_FRAME, *LPPOINT_FRAME;

#endif