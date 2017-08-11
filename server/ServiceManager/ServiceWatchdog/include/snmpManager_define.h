/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: snmpManager_define.h
Description: 结构和宏定义    
Author: guanguochen       
Date: 2009.09.14    
*****************************************************/
#if defined(WIN32)
#pragma once
#endif

#ifndef __SNMPMANAGER_DEFINE_H__
#define __SNMPMANAGER_DEFINE_H__

#if defined(WIN32)
    #if defined(SNMPMANAGER_EXPORTS)
        #define SNMPMANAGER_DECLARE extern "C" __declspec(dllexport)
    #else
        #define SNMPMANAGER_DECLARE extern "C" __declspec(dllimport)
    #endif

    #ifndef CALLBACK
    #define CALLBACK __stdcall
    #endif
#else
    #define SNMPMANAGER_DECLARE extern "C"

    #ifndef CALLBACK
        #define CALLBACK
    #else
        #undef CALLBACK
        #define CALLBACK
    #endif
#endif

enum SnmpManager_IO_TYPE            //IO操作类型
{
    SNMPMANAGER_SEND,           
    SNMPMANAGER_RECV
};

typedef struct SnmpManager_AsyncIO_Info
{
    SnmpManager_IO_TYPE iotype;
    void *callclass;
    void *pbuffer;
} SNMPMANAGER_ASYNCIO_INFO, *PSNMPMANAGER_ASYNCIO_INFO;

#include "smival.h"

typedef struct PduInfo
{
    unsigned long requestid;
    int errorstatus;
    int errorindex;
    int vbcount;
    char **stroidarray;
    unsigned short type;
    SmiVALUE *objvaluearray;
} PDUINFO, *PPDUINFO;

typedef void (CALLBACK *SnmpManager_Msg_CB)(PPDUINFO ppdu, void *userdata);

//#define SNMPMANAGER_SYNC
#define MAX_SYNCSENDSOCK_NUM 1000

#endif
