#ifndef __CAMTHREAD_H__
#define __CAMTHREAD_H__
#define EXECUTE_ASSERT(_x_) ((void)(_x_))
// wrapper for event objects
// wrapper for whatever critical section we have
class CCritSec {

	// make copy constructor and assignment operator inaccessible

	CCritSec(const CCritSec &refCritSec);
	CCritSec &operator=(const CCritSec &refCritSec);

	CRITICAL_SECTION m_CritSec;

public:
	CCritSec() {
		InitializeCriticalSection(&m_CritSec);
	};

	~CCritSec() {
		DeleteCriticalSection(&m_CritSec);
	};

	void Lock() {
		EnterCriticalSection(&m_CritSec);
	};

	void Unlock() {
		LeaveCriticalSection(&m_CritSec);
	};
};

class CAutoLock {

	// make copy constructor and assignment operator inaccessible

	CAutoLock(const CAutoLock &refAutoLock);
	CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
	CCritSec * m_pLock;

public:
	CAutoLock(CCritSec * plock)
	{
		m_pLock = plock;
		m_pLock->Lock();
	};

	~CAutoLock() {
		m_pLock->Unlock();
	};
};

class CAMDVDEvent
{

	// make copy constructor and assignment operator inaccessible

	CAMDVDEvent(const CAMDVDEvent &refEvent);
	CAMDVDEvent &operator=(const CAMDVDEvent &refEvent);

protected:
	HANDLE m_hEvent;
public:
	CAMDVDEvent(BOOL fManualReset = FALSE);
	~CAMDVDEvent();

	// Cast to HANDLE - we don't support this as an lvalue
	operator HANDLE () const { return m_hEvent; };

	void Set() {EXECUTE_ASSERT(SetEvent(m_hEvent));};
	BOOL Wait(DWORD dwTimeout = INFINITE) {
		return (WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_OBJECT_0);
	};
	void Reset() { ResetEvent(m_hEvent); };
	BOOL Check() { return Wait(0); };
};
#define AM_NOVTABLE 

class AM_NOVTABLE CAMDVDThread {

	// make copy constructor and assignment operator inaccessible

	CAMDVDThread(const CAMDVDThread &refThread);
	CAMDVDThread &operator=(const CAMDVDThread &refThread);

	CAMDVDEvent m_EventSend;
	CAMDVDEvent m_EventComplete;

	DWORD m_dwParam;
	DWORD m_dwReturnVal;

protected:
	HANDLE m_hThread;

	// thread will run this function on startup
	// must be supplied by derived class
	virtual DWORD ThreadProc() = 0;

public:
	CAMDVDThread();
	virtual ~CAMDVDThread();

	CCritSec m_AccessLock;	// locks access by client threads
	CCritSec m_WorkerLock;	// locks access to shared objects

	// thread initially runs this. param is actually 'this'. function
	// just gets this and calls ThreadProc
	static DWORD WINAPI InitialThreadProc(LPVOID pv);

	// start thread running  - error if already running
	BOOL Create();

	// signal the thread, and block for a response
	//
	DWORD CallWorker(DWORD);

	// accessor thread calls this when done with thread (having told thread
	// to exit)
	void Close() {
		HANDLE hThread = (HANDLE)InterlockedExchangePointer(&m_hThread, 0);
		if (hThread) {
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = NULL;
		}
	};

	// ThreadExists
	// Return TRUE if the thread exists. FALSE otherwise
	BOOL ThreadExists(void) const
	{
		if (m_hThread == 0) {
			return FALSE;
		} else {
			return TRUE;
		}
	}

	// return the thread handle, we don't want to operator HANDLE
	HANDLE GetHandle() { return m_hThread; }
	// wait for the next request
	DWORD GetRequest();

	// is there a request?
	BOOL CheckRequest(DWORD * pParam);

	// reply to the request
	void Reply(DWORD);

	// If you want to do WaitForMultipleObjects you'll need to include
	// this handle in your wait list or you won't be responsive
	HANDLE GetRequestHandle() const { return m_EventSend; };

	// Find out what the request was
	DWORD GetRequestParam() const { return m_dwParam; };

	// call CoInitializeEx (COINIT_DISABLE_OLE1DDE) if
	// available. S_FALSE means it's not available.
	static HRESULT CoInitializeHelper();
};


#endif // __CAMTHREAD_H__

