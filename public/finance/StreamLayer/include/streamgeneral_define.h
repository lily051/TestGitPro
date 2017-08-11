/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: streamgeneral_define.h
Description: �ṹ�ͺ궨��  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __STREAMGENERAL_DEFINE_H__
#define __STREAMGENERAL_DEFINE_H__

#include "rtspserver_define.h"
#include "rtspclient_define.h"

#define MAX_SHAREDBUFF_BLOCK 101			//һ�����������е������������
#define MAX_SHAREDBUFF_LENGTH 4072			//һ������������󳤶�

#define MAX_SDP_LEN 2048						//���SDP����
#define MAX_ENCRYPTDES_LEN 64					//�����ܷ�ʽ��������

//���ؽ�����־
#define HEADER_FLAG    0x68696b24
#define HEADER_FLAG_EX 0x01020304
typedef struct _DOWNLOAD_BYE_
{
	int iByte;
	int iByteEx;
}DOWNLOAD_BYE;

#endif