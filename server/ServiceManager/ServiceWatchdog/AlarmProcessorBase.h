#ifndef __GUARD_ALARMPROCESSOR_BASE_H_
#define __GUARD_ALARMPROCESSOR_BASE_H_

#pragma once

#include "GenDef.h"

class CAlarmProcessorBase
{
public:
	CAlarmProcessorBase(void);
	virtual ~CAlarmProcessorBase(void);

public:
	virtual HPR_BOOL InitProcessor() = 0;
	virtual HPR_VOID FiniProcessor() = 0;
	virtual void AddAlarmMsg(const pALARM_MSG pAlarm) = 0; 
};

#endif
