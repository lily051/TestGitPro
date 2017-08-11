/**	@file    GeneralDef.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   DLL定义共用头文件
 *
 *	@author	 chenxiaoyan3
 *	@date	 2015/7/23
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V1.0.0  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#ifndef GUARD_GENERALDEF_H
#define GUARD_GENERALDEF_H
#pragma once

#include <windows.h>
#include <string>
#include "SystemTransform.h"
// usb摄像头信息
typedef struct camera_info_st{
    unsigned int  uiIndex; //摄像头索引号
    std::string   strName; //摄像头名称
} CAMERA_INFO_ST, *pCAMERA_INFO_ST;

// 麦克风信息
typedef struct micor_info_st{
    unsigned int  uiIndex; //麦克风索引号
    std::string   strName; //麦克风名称
} MICOR_INFO_ST, *pMICOR_INFO_ST;

// usb摄像头信息
typedef struct picture_property_st{
    unsigned int  uiWidth; //宽
    unsigned int  uiHeight; //高
} PICTURE_PROPERTY_ST, *pPICTURE_PROPERTY_ST;

//typedef struct OUTPUTDATA_INFO 
//{
//    unsigned char* pData;          //回调数据缓存，该指针请勿用于异步的传递
//    unsigned long dwDataLen;       //回掉数据长度
//    unsigned long dwDataType;      //数据类型，如TRANS_SYSHEAD,TRANS_STREAMDATA
//} OUTPUTDATA_INFO;

typedef void (__stdcall *pCameraDSDataBack)(OUTPUTDATA_INFO *pDataInfo, void * pUser);
typedef void (__stdcall *pMediaDataBack)(BYTE *pDataInfo, long BufferLen, void *pUser);
#endif    //GUARD_GENERALDEF_H
