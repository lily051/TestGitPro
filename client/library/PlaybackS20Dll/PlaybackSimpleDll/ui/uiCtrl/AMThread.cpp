//lint -library
#include "stdafx.h"
#include "AMThread.h"

// --- CAMDVDEvent -----------------------
CAMDVDEvent::CAMDVDEvent(BOOL fManualReset)
{
    m_hEvent = CreateEvent(NULL, fManualReset, FALSE, NULL);
}

CAMDVDEvent::~CAMDVDEvent()
{
    if (m_hEvent) {
		EXECUTE_ASSERT(CloseHandle(m_hEvent));
    }
}

CAMDVDThread::CAMDVDThread()
    : m_EventSend(TRUE)     // must be manual-reset for CheckRequest()
{
    m_hThread = NULL;
    m_dwParam = 0;
    m_dwReturnVal = 0;
}

CAMDVDThread::~CAMDVDThread() {
    Close();
}


// when the thread starts, it calls this function. We unwrap the 'this'
//pointer and call ThreadProc.
DWORD WINAPI
CAMDVDThread::InitialThreadProc(LPVOID pv)
{
    CAMDVDThread * pThread = (CAMDVDThread *) pv;

	::CoInitialize(NULL);
    DWORD dwRet = pThread->ThreadProc();
	::CoUninitialize();

	return dwRet;
}

BOOL
CAMDVDThread::Create()
{
    DWORD threadid;

    CAutoLock lock(&m_AccessLock);

    if (ThreadExists()) {
	return FALSE;
    }

    m_hThread = CreateThread(
		    NULL,
		    0,
		    CAMDVDThread::InitialThreadProc,
		    this,
		    0,
		    &threadid);

    if (!m_hThread) {
	return FALSE;
    }

    return TRUE;
}

DWORD
CAMDVDThread::CallWorker(DWORD dwParam)
{
    // lock access to the worker thread for scope of this object
    CAutoLock lock(&m_AccessLock);

    if (!ThreadExists()) {
	return (DWORD) E_FAIL;
    }

    // set the parameter
    m_dwParam = dwParam;

    // signal the worker thread
    m_EventSend.Set();

    // wait for the completion to be signalled
    m_EventComplete.Wait();

    // done - this is the thread's return value
    return m_dwReturnVal; //what?  there return this value for waht,kid me?
}

// Wait for a request from the client
DWORD
CAMDVDThread::GetRequest()
{
    m_EventSend.Wait();
    return m_dwParam;
}

// is there a request?
BOOL
CAMDVDThread::CheckRequest(DWORD * pParam)
{
    if (!m_EventSend.Check()) {
	return FALSE;
    } else {
	if (pParam) {
	    *pParam = m_dwParam;
	}
	return TRUE;
    }
}

// reply to the request
void
CAMDVDThread::Reply(DWORD dw)
{
    m_dwReturnVal = dw;

    // The request is now complete so CheckRequest should fail from
    // now on
    //
    // This event should be reset BEFORE we signal the client or
    // the client may Set it before we reset it and we'll then
    // reset it (!)

    m_EventSend.Reset();

    // Tell the client we're finished

    m_EventComplete.Set();
}

HRESULT CAMDVDThread::CoInitializeHelper()
{
	HRESULT hr = CoInitialize(NULL);
    return hr;
}

