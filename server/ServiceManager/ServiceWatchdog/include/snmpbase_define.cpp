/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: snmpbase_define.cpp
Description: 结构和宏实现
Author: guanguochen       
Date: 2009.09.14    
*****************************************************/
#include "stdafx.h"
#include "snmpbase_define.h"
#include <stdio.h>
#include <string.h>

static const STUENTITYTYPE g_stuentitytype[] =
{
	{ENTITY_TYPE_UNKNOWN, "unknown"},
	{ENTITY_TYPE_SERVER, "server"},
	{ENTITY_TYPE_NOSNMP_DEVICE, "no snmp"},
	{ENTITY_TYPE_SNMP_DEVICE, "snmp device"}
};

static const STUENTITYSUBTYPE g_stuentitysubtype[] =
{
	{ENTITY_SUBTYPE_UNKNOWN, "unknown"},
	{ENTITY_SUBTYPE_CMS_SERVER, "cms"},
	{ENTITY_SUBTYPE_ALARM_SERVER, "alarm"},
	{ENTITY_SUBTYPE_RECORD_SERVER, "record"},
	{ENTITY_SUBTYPE_STREAM_SERVER, "stream"},
	{ENTITY_SUBTYPE_DECODE_SERVER, "decode"},
	{ENTITY_SUBTYPE_VRM_SERVER, "vrm"},
	{ENTITY_SUBTYPE_PCNVR_SERVER, "pcnvr"},
	{ENTITY_SUBTYPE_PCDVR_SERVER, "pcdvr"},
	{ENTITY_SUBTYPE_VIDEOWALL_SERVER, "videowall"},
	{ENTITY_SUBTYPE_PTZ_SERVER, "ptz"},
	{ENTITY_SUBTYPE_LB, "lb"},
	{ENTITY_SUBTYPE_HIK_DSHC, "hik-dshc"},
	{ENTITY_SUBTYPE_HIK_CARD, "hik-card"},
	{ENTITY_SUBTYPE_HIK_DSME, "hik-dsme"},
	{ENTITY_SUBTYPE_DECODE_DEVICE, "hik-decode"},
	{ENTITY_SUBTYPE_BNRECORD_DEVICE, "brainaire-record"},
	{ENTITY_SUBTYPE_PAG_SERVER, "pag"},
	{ENTITY_SUBTYPE_CAG_SERVER, "cag"},
	{ENTITY_SUBTYPE_VTM_SERVER, "vtm"}
};

static const STUBNOID g_stubnoid[] =
{
	{hikbnRecordDeviceFCServiceStatus,       "1.3.6.1.4.1.2021.13.4249.1.1.2.0"},
	{hikbnRecordDeviceISCSIServiceStatus,    "1.3.6.1.4.1.2021.13.4249.1.1.3.0"},
	{hikbnRecordDeviceRaidStatus,            "1.3.6.1.4.1.2021.13.4249.1.1.10.0"},
	{hikbnRecordDevicePvStatus,              "1.3.6.1.4.1.2021.13.4249.1.1.11.0"},
	{hikbnRecordDeviceDiskArrayStatus,       "1.3.6.1.4.1.2021.13.4249.1.1.12.0"},
	{hikbnRecordDeviceFlashFullStatus,       "1.3.6.1.4.1.2021.13.4249.1.1.13.0"},
	{hikbnRecordDeviceDiskChannelStatus,     "1.3.6.1.4.1.2021.13.4249.1.1.14.0"},
	{hikbnRecordDeviceDiskSmartAlarmStatus,  "1.3.6.1.4.1.2021.13.4249.1.1.15.0"},
	{hikbnRecordDeviceFlashStatus,           "1.3.6.1.4.1.2021.13.4249.1.1.16.0"},
	{hikbnRecordDeviceNASServiceStatus,      "1.3.6.1.4.1.2021.13.4249.1.1.18.0"},
	{hikbnRecordDeviceCPUStatus,             "1.3.6.1.4.1.2021.13.4249.1.1.19.0"},
	{hikbnRecordDeviceFanStatus,             "1.3.6.1.4.1.2021.13.4249.1.1.20.0"},
	{hikbnRecordDeviceStoragePoolCapacity,   "1.3.6.1.4.1.2021.13.4249.1.1.21.0"},
	{hikbnRecordDeviceStoragePoolFreeCapacity, "1.3.6.1.4.1.2021.13.4249.1.1.22.0"},
	{hikbnRecordDeviceISCSILUNCapacity,      "1.3.6.1.4.1.2021.13.4249.1.1.23.0"},
	{hikbnRecordDeviceNASCapacity,           "1.3.6.1.4.1.2021.13.4249.1.1.24.0"},
	{hikbnRecordDeviceNASSnapShotsCapacity,  "1.3.6.1.4.1.2021.13.4249.1.1.25.0"},
	{hikbnRecordDeviceWORMCapacity,          "1.3.6.1.4.1.2021.13.4249.1.1.26.0"},
	{hikbnRecordDeviceNASFreeCapacity,       "1.3.6.1.4.1.2021.13.4249.1.1.28.0"},
	{hikbnRecordDevicePowerStatus,           "1.3.6.1.4.1.2021.13.4249.1.1.29.0"},
    /* add bn trap list */
	{hikTrapRaidState,              "1.3.6.1.4.1.2021.13.4249.2.1.10.0"},
	{hikTrapChannelState,           "1.3.6.1.4.1.2021.13.4249.2.1.14.0"},
	{hikTrapCPUAlarm,               "1.3.6.1.4.1.2021.13.4249.2.1.19.0"},
	{hikTrapFanStatus,              "1.3.6.1.4.1.2021.13.4249.2.1.20.0"},
	{hikTrapPowerStatus,            "1.3.6.1.4.1.2021.13.4249.2.1.29.0"},
};

static const STUHIKDES g_stuhikdes[] =
{
	{hikTrapRaidState,              "raid state is bad.\n"},
	{hikTrapChannelState,           "disk was plugged from the chassis.\n"},
	{hikTrapCPUAlarm,               "cpu temperature is over the threshold,the default threshold is 70℃.\n"},
	{hikTrapFanStatus,              "fan  does not work or speed is low.\n"},
	{hikTrapPowerStatus,            "power supply is not operational.\n"},
};

char *GetEntityTypeString(unsigned int type)
{
	int count = sizeof(g_stuentitytype) / sizeof(g_stuentitytype[0]);
	for (int i = 0; i < count; i++)
	{
		if (type == g_stuentitytype[i].itype)
		{
			return g_stuentitytype[i].strtype;
		}
	}
	return NULL;
}

char *GetEntitySubTypeString(unsigned int subtype)
{
	int count = sizeof(g_stuentitysubtype) / sizeof(g_stuentitysubtype[0]);
	for (int i = 0; i < count; i++)
	{
		if (subtype == g_stuentitysubtype[i].isubtype)
		{
			return g_stuentitysubtype[i].strsubtype;
		}
	}
	return NULL;
}

char *GetBnOidString(char *oid)
{
	int count = sizeof(g_stubnoid) / sizeof(g_stubnoid[0]);
	for (int i = 0; i < count; i++)
	{
		if (strcmp(oid, g_stubnoid[i].hikbnoid) == 0)
		{
			return g_stubnoid[i].bnoid;
		}
	}
	return NULL;
}

char *GetTrapdesc(char *oid)
{
	int count = sizeof(g_stuhikdes) / sizeof(g_stuhikdes[0]);
	for (int i = 0; i < count; i++)
	{
		if (strcmp(oid, g_stuhikdes[i].hikbnoid) == 0)
		{
			return g_stuhikdes[i].desc;
		}
	}
	return NULL;
}
