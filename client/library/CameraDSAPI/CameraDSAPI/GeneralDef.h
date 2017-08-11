/**	@file    GeneralDef.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   DLL���干��ͷ�ļ�
 *
 *	@author	 chenxiaoyan3
 *	@date	 2015/7/23
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V1.0.0  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */
#ifndef GUARD_GENERALDEF_H
#define GUARD_GENERALDEF_H
#pragma once

#include <windows.h>
#include <string>
#include "SystemTransform.h"
// usb����ͷ��Ϣ
typedef struct camera_info_st{
    unsigned int  uiIndex; //����ͷ������
    std::string   strName; //����ͷ����
} CAMERA_INFO_ST, *pCAMERA_INFO_ST;

// ��˷���Ϣ
typedef struct micor_info_st{
    unsigned int  uiIndex; //��˷�������
    std::string   strName; //��˷�����
} MICOR_INFO_ST, *pMICOR_INFO_ST;

// usb����ͷ��Ϣ
typedef struct picture_property_st{
    unsigned int  uiWidth; //��
    unsigned int  uiHeight; //��
} PICTURE_PROPERTY_ST, *pPICTURE_PROPERTY_ST;

//typedef struct OUTPUTDATA_INFO 
//{
//    unsigned char* pData;          //�ص����ݻ��棬��ָ�����������첽�Ĵ���
//    unsigned long dwDataLen;       //�ص����ݳ���
//    unsigned long dwDataType;      //�������ͣ���TRANS_SYSHEAD,TRANS_STREAMDATA
//} OUTPUTDATA_INFO;

typedef void (__stdcall *pCameraDSDataBack)(OUTPUTDATA_INFO *pDataInfo, void * pUser);
typedef void (__stdcall *pMediaDataBack)(BYTE *pDataInfo, long BufferLen, void *pUser);
#endif    //GUARD_GENERALDEF_H
