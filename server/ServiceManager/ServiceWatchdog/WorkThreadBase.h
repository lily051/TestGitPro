//lint -library
#pragma once

class CWorkThreadBase
{
public:
	CWorkThreadBase(void) {};
	virtual ~CWorkThreadBase(void){};

	DWORD ThreadProc()
	{
		do 
		{
			if (m_bShutdown)
			{
				break;
			}

			DoWork();

		} while (!m_bShutdown && (WaitForSingleObject(m_hEvent, m_nWaitTime) != WAIT_OBJECT_0));

		return 1;
	}

	virtual void DoWork() {return ;};

	static DWORD WINAPI WorkerThreadProc(LPVOID pv)
	{
		CWorkThreadBase* pThis = reinterpret_cast< CWorkThreadBase* >(pv); 

		return pThis->ThreadProc();
	} 

	BOOL workStart(int nWaitTime)
	{
		m_nWaitTime = nWaitTime;
		
		m_bShutdown = FALSE;

		m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		m_hThread = CreateThread(NULL, 0, WorkerThreadProc, this, 0, NULL);

		return TRUE;
	}

	BOOL StopWork()
	{
		m_bShutdown = TRUE;

		SetEvent(m_hEvent);

		WaitForSingleObject(m_hThread, INFINITE);

		CloseHandle(m_hEvent);
		CloseHandle(m_hThread);

		return TRUE;
	}

private:

	HANDLE m_hThread;

	HANDLE m_hEvent;

	BOOL m_bShutdown;

	int m_nWaitTime;
};
