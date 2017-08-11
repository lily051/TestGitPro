/**************************************************************************
*   Copyright 2007-2009 Hikvision Digital Technology Co., Ltd.
*   FileName:       CriticalSectionLock.cpp
*   Description:    CriticalSection lock
*   Author:         Zhihu Xue
*   Date:           17/11/2009
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         17/11/2009      Zhihu Xue       created
**************************************************************************/
#include "StdAfx.h"
#include ".\criticalsectionlock.h"
/**************************************************************************
*   Function:		CCriticalSectionLock
*   Description:    Constructor
*   Input:          null 
*   Output:         null
*   Return:			void
**************************************************************************/
CCriticalSectionLock::CCriticalSectionLock(const CRITICAL_SECTION &csec)
:m_csection(csec)
{
	EnterCriticalSection( (LPCRITICAL_SECTION)&m_csection);
}
/**************************************************************************
*   Function:		~CCriticalSectionLock
*   Description:    Destructor
*   Input:          null 
*   Output:         null
*   Return:			void
**************************************************************************/
CCriticalSectionLock::~CCriticalSectionLock(void)
{
	LeaveCriticalSection((LPCRITICAL_SECTION)&m_csection);
}

CCritSec::CCritSec(void)
{
    ::InitializeCriticalSection(&m_critSec);
}
CCritSec::~CCritSec(void)
{
    ::DeleteCriticalSection(&m_critSec);
}
void CCritSec::Lock(void)
{
    ::EnterCriticalSection(&m_critSec);
}
void CCritSec::Unlock(void) throw()
{
    try
    {
        ::LeaveCriticalSection(&m_critSec);
    }
    catch (...)
    {
        //OutputDebugString(_T("error, wtf\n"));
    }
    
}
