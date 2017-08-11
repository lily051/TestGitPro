#pragma once

class CCriticalSectionLock
{
public:
	CCriticalSectionLock(const CRITICAL_SECTION &csec);
	~CCriticalSectionLock(void);
private:
	const CRITICAL_SECTION &m_csection;
    CCriticalSectionLock& operator=(const CCriticalSectionLock &);
};

template <typename TLock>
class CScopeLock
{
public:
    CScopeLock(TLock& lock)
        : m_lock(lock)
    {
        m_lock.Lock();
    }
    ~CScopeLock(void)
    {
        m_lock.Unlock();
    }
private:
    TLock &m_lock;
    CScopeLock(const CScopeLock&);
    CScopeLock& operator=(const CScopeLock&);
};

class CCritSec
{
public:
    CCritSec(void);
    ~CCritSec(void);
    void Lock(void);
    void Unlock(void) throw();
private:
    CRITICAL_SECTION m_critSec;
};
