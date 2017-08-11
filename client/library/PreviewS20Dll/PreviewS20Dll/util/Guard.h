#pragma once

class CGuard
{
public:
	CGuard(CRITICAL_SECTION* pMutex);
	~CGuard(void);
	
private:
	CRITICAL_SECTION* m_pLock;
	
};
