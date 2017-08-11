// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// PlaybackSDK.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ
//lint -library
#include "stdafx.h"


/** @fn HANDLE Create_File
*   @brief �����ļ�����
*   @param[in] strFileName: �ļ���
*   @param null
*   @return �ļ����
*/
HANDLE Create_File(char * szFileName)
{
	try
	{
		//����·��
		if(!MakeSureDirectoryPathExists(szFileName))
        {
            PLAYBACKSDK_ERROR("MakeSureDirectoryPathExists failed, LastError:%lu, szFileName:%s",GetLastError(),szFileName);
            return NULL;
        }

		HANDLE hFileHandle = NULL;
		DWORD dwDirattr = 0;
		CFileFind tempFind;

		dwDirattr = GetFileAttributes(szFileName);
		if (INVALID_FILE_ATTRIBUTES != dwDirattr)
		{
			TRACE("@z GetFileAttributes:%lu", GetLastError());
			CFile::Remove(szFileName);
			dwDirattr = GetFileAttributes(szFileName);
		}

		if(dwDirattr == 0xFFFFFFFF)
		{
			hFileHandle = CreateFile(szFileName,
				GENERIC_WRITE,                
				FILE_SHARE_READ,             
				NULL,                        
				OPEN_ALWAYS,               
				FILE_ATTRIBUTE_NORMAL,       
				NULL	
				);

			if(INVALID_HANDLE_VALUE == hFileHandle)
			{
				return NULL;
			}
		}
		return hFileHandle;
	}
	catch (CFileException* pEx)
	{
		PLAYBACKSDK_ERROR("Create_File throw en exception, LastError:%lu, m_cause:%d", GetLastError(), pEx->m_cause);
		pEx->Delete();

		return NULL;
	}

	return NULL;
}

/** @fn AppFun_GetDllPath
*   @brief ��ȡdll·��
*   @param[in]  NULL
*   @param[out] strPathName: ·��
*   @return NULL
*/
void AppFun_GetDllPath(CString &strPathName)
{
	TCHAR szFileName[MAX_PATH];

	if (0 != GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH - 1))
	{
		szFileName[MAX_PATH - 1] = '\0';
	}

	TCHAR * p = _tcsrchr(szFileName, '\\') + 1;

	if (p)
	{
		*p = '\0';

		strPathName = szFileName;
	}
}

/**   @fn          WaitForWindowThread
 *    @brief   	   �ȴ������߳�.
 *    @param[in]   hThread:�߳̾��.
 *    @param[in]   
 *    @return      
 */
void WaitForWindowThread(HANDLE hThread)
{	
	while(WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLINPUT)) 
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);   
			DispatchMessage(&msg);   
		}
	}
}

void OutputDebugStrEx(const TCHAR *fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	TCHAR	string_buffer[1024];
	_vsntprintf_s(string_buffer, _countof(string_buffer), _TRUNCATE, fmt, vl);
	string_buffer[1023] = '\0';
	OutputDebugString(string_buffer);
	va_end(vl);
}

BOOL AppFun_GetLocalIPAddr(CString & strIPAddr)
{
	WORD   wVersionRequested;   
	WSADATA   wsaData;   
	char   name[255];   
	PHOSTENT   hostinfo;   
	wVersionRequested = MAKEWORD(2,0);   

	if(WSAStartup(wVersionRequested,&wsaData) == 0)   
	{   
		if(gethostname(name,sizeof(name)) == 0)   
		{   
			if((hostinfo = gethostbyname(name)) != NULL)   
			{   
				strIPAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}   
		}   
		WSACleanup();
	}     
	if (!strIPAddr.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}

int GetUdpPort(const CString& strLocalIP, LONG& nPort)
{
	int nPortCount = nPort;
	HPR_SOCK_T sockUdp = HPR_CreateSocket(HPR_AF_INET, SOCK_DGRAM, 0);
	if (sockUdp == HPR_INVALID_SOCKET)
	{
		return -1;
	}
	HPR_ADDR_T sockAddr; 
	sockAddr.SA.sin4.sin_addr.S_un.S_addr = inet_addr(strLocalIP);
	sockAddr.SA.sin4.sin_family = HPR_AF_INET;
	sockAddr.SA.sin4.sin_port = HPR_Htons((HPR_UINT16)nPort);

	if (HPR_ERROR == HPR_Bind(sockUdp, &sockAddr))
	{
		if (nPort - nPortCount >= 100)
		{
			return -1;
		}
		nPort += 2;
		GetUdpPort(strLocalIP, nPort);
	}
	HPR_CloseSocket(sockUdp);
	return 0;
}

BOOL IsPBTimeCorrect(const PB_TIME& pbTime)
{
    if (pbTime.dwYear  < 1970U || pbTime.dwYear > 3000U ||  
        pbTime.dwMonth < 1U    || pbTime.dwMonth> 12U   ||  
        pbTime.dwDay   < 1U    || pbTime.dwDay  > 31U   ||  
        pbTime.dwHour  < 0UL   || pbTime.dwHour > 23U   ||  
        pbTime.dwMinute< 0UL   || pbTime.dwMinute > 59U ||  
        pbTime.dwSecond< 0UL   || pbTime.dwSecond > 59U  )  //����ᱨһ������Ϊ��һ���޷�������0�Ƚϣ�
                                                       //ʵ��������Ƚϻ��Զ�ת��Ϊ�޷������ıȽϣ���coverity�ᱨ��
    {  
        return FALSE;  
    } 

    return TRUE;
    /*CTime ct(pbTime.dwYear, pbTime.dwMonth, pbTime.dwDay, pbTime.dwHour, pbTime.dwMinute, pbTime.dwSecond);  
    return (pbTime.dwYear   == (DWORD)ct.GetYear() &&  
            pbTime.dwMonth  == (DWORD)ct.GetMonth() &&  
            pbTime.dwDay    == (DWORD)ct.GetDay() &&  
            pbTime.dwHour   == (DWORD)ct.GetHour() &&  
            pbTime.dwMinute == (DWORD)ct.GetMinute() &&  
            pbTime.dwSecond == (DWORD)ct.GetSecond()); */
}