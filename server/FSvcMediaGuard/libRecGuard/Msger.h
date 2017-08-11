#ifndef GUARD_MSGER_H
#define GUARD_MSGER_H
#pragma once
#include "SvcGeneralDef.h"

class CMsger
{
public:
    CMsger(CMsger *pMsger = NULL);
    virtual ~CMsger(void);
    void AddMsg(LPCTSTR pszFormat, ...);
    void AddMsgV(LPCTSTR pszFormat, va_list args);

private:
    inline void Action(LPCTSTR lpMsg)
    {
        doAction(lpMsg);
        if (NULL != m_pOtherMsger)
        {
            m_pOtherMsger->Action(lpMsg);
        }
    }
    virtual void doAction(LPCTSTR lpMsg) = 0;

private:
    CMsger *m_pOtherMsger;
    static size_t const BUFF_SIZE = 4 * 1024;
};

void Msger_Init(void);
void Msger_AddMsg(LPCTSTR pszFormat, ...);
//void Msger_UploadMsg(const DWORD dwRecordID, const DWORD dwUploadPercent);
void Msger_NotifyMsg(const DWORD userid, LPCTSTR pszFormat, ...);
void Msger_AlarmMsg(const DWORD alarmType,
					const DWORD userid, 
					const DWORD channel, 
					const DWORD alarmStatus,
					const DWORD dwRev,
					LPCTSTR pszFormat, ...);
void Msger_Fini(void);

#endif // end of GUARD_MSGER_H
