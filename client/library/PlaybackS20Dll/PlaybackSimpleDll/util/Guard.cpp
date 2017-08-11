#include "StdAfx.h"
#include ".\guard.h"

CGuard::CGuard(CRITICAL_SECTION* pMutex)
{
	m_pLock = pMutex;
	EnterCriticalSection(m_pLock);
}

CGuard::~CGuard(void)
{
	LeaveCriticalSection(m_pLock);	
}
