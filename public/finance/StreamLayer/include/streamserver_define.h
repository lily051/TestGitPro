/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: streamserver_define.h
Description: �ṹ�ͺ궨��  	 
Author: guanguochen       
Date: 2009.03.06    
*****************************************************/
#pragma once
#ifndef __STREAMSERVER_DEFINE_H__
#define __STREAMSERVER_DEFINE_H__

#include "streamgeneral_define.h"

#define MAX_SHAREDBUFF_BLOCK 101				//һ�����������е������������
#define STREAMSERVER_MSG_CLIENTINFO 0x0101		//��ý�����˿ͻ�����Ϣ֪ͨ
#define MAX_RELATIVE_PATH_COUNT 10				//֧�����·���������Ŀ
#define MAX_RELATIVEPATH_LEN 256				//������·������

#define HPR_STREAMSERVER_HANDLE HPR_INT32		//��ý��������Ự������Ͷ���
#define HPR_INVALID_STREAMSERVERSESSION -1		//��Ч����ý��������Ự���ֵ����

typedef HPR_INT32 (CALLBACK *pStreamServerMsgFunc)(HPR_STREAMSERVER_HANDLE sessionhandle, HPR_INT32 opt,\
						HPR_VOIDPTR param1, HPR_VOIDPTR param2, HPR_VOIDPTR param3, HPR_VOIDPTR param4);

#if defined(OS_WINDOWS)
	#if defined(STREAMSERVER_EXPORTS)
		#define HPR_STREAMSERVER_DECLARE extern "C" __declspec(dllexport)
	#else
		#define HPR_STREAMSERVER_DECLARE extern "C" __declspec(dllimport)
	#endif
#elif defined(OS_POSIX)
	#define HPR_STREAMSERVER_DECLARE extern "C"
#else
	#error OS Not Implement Yet.
#endif

#endif