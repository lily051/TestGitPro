/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: snmpManager.h
Description: APIº¯Êý¶¨Òå     
Author: guanguochen       
Date: 2009.09.14
*****************************************************/
#if defined(WIN32)
#pragma once
#endif

#ifndef __SNMPMANAGER_H__
#define __SNMPMANAGER_H__

#include "snmpManager_define.h"
#include "HPR_Addr.h"
#include "HPR_Types.h"
#include "HPR_String.h"

SNMPMANAGER_DECLARE int CALLBACK SnmpManager_InitLib(void);
SNMPMANAGER_DECLARE int CALLBACK SnmpManager_FiniLib(void);

SNMPMANAGER_DECLARE int CALLBACK SnmpManager_StartService(unsigned short managerport = DEFAULT_MANAGER_PORT, unsigned short family_type = AF_INET);
SNMPMANAGER_DECLARE int CALLBACK SnmpManager_StopService(void);

SNMPMANAGER_DECLARE int CALLBACK SnmpManager_GetRemoteAddr(HPR_ADDR_T*);

SNMPMANAGER_DECLARE int CALLBACK SnmpManager_SetMsgCallBack(SnmpManager_Msg_CB msgCB, void *userdata);

SNMPMANAGER_DECLARE int CALLBACK SnmpManager_Get(int requestid, unsigned int vbcount, char **oidarray,\
                                                 char *agentaddr, unsigned short agentport = DEFAULT_AGENT_PORT,\
                                                 int iversion = DEFAULT_SNMP_VERSION);
SNMPMANAGER_DECLARE int CALLBACK SnmpManager_GetNext(int requestid, unsigned int vbcount, char **oidarray,\
                                                 char *agentaddr, unsigned short agentport = DEFAULT_AGENT_PORT,\
                                                 int iversion = DEFAULT_SNMP_VERSION);
SNMPMANAGER_DECLARE int CALLBACK SnmpManager_Set(int requestid, unsigned int vbcount, char **oidarray,\
                                                 SmiLPVALUE valuearray, char *agentaddr,\
                                                 unsigned short agentport = DEFAULT_AGENT_PORT,\
                                                 int iversion = DEFAULT_SNMP_VERSION);
SNMPMANAGER_DECLARE int CALLBACK SnmpManager_Inform(int requestid, unsigned int vbcount, char **oidarray,\
                                                 SmiLPVALUE valuearray, char *agentaddr,\
                                                 unsigned short agentport = DEFAULT_AGENT_PORT,\
                                                 int iversion = DEFAULT_SNMP_VERSION);
SNMPMANAGER_DECLARE int CALLBACK SnmpManager_GetBulk(int requestid, unsigned int vbcount, char **oidarray,\
                                                 char *agentaddr,\
                                                 int nonrepeaters = 0, int maxrepetitions = 0,\
                                                 unsigned short agentport = DEFAULT_AGENT_PORT,\
                                                 int iversion = DEFAULT_SNMP_VERSION);

#endif
