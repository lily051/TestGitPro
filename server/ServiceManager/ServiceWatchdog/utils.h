/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       utils.h
*   Description:    ÊµÓÃº¯Êý
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/

//lint -library

///////////////////////////// chBEGINTHREADEX Macro ///////////////////////////


// This macro function calls the C runtime's _beginthreadex function. 
// The C runtime library doesn't want to have any reliance on Windows' data 
// types such as HANDLE. This means that a Windows programmer needs to cast
// values when using _beginthreadex. Since this is terribly inconvenient, 
// I created this macro to perform the casting.
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#include <process.h>

#define chBEGINTHREADEX(psa, cbStackSize, pfnStartAddr, \
    pvParam, dwCreateFlags, pdwThreadId)                 \
    ((HANDLE)_beginthreadex(                          \
    (void *)        (psa),                         \
    (unsigned)      (cbStackSize),                 \
    (PTHREAD_START) (pfnStartAddr),                \
    (void *)        (pvParam),                     \
    (unsigned)      (dwCreateFlags),               \
    (unsigned *)    (pdwThreadId)))

//
#ifndef _countof
#  define _countof(x) (sizeof(x)/sizeof(x[0]))
#endif

Image * LoadImageFromResource(UINT uID);
Bitmap * LoadBitmapFromResource(UINT uID);

void SetRegion(CWnd * pWnd, CBitmap & bitmap);

/**************************************************************************
*   Function:		TestRemoteHost
*   Description:    test whether the remote host is online
*   Input:          const char *szIP, remote host IP
*                   USHORT nPort, remote host port
*   Output:         (null)
*   Return:			BOOL, return TRUE if online
**************************************************************************/
BOOL TestRemoteHost(const char *szIP, USHORT nPort);

//////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "version.lib")

class CModuleVerInfo
{
public:
	CModuleVerInfo(LPCTSTR lpszMoudleName)
		: m_pVerInfo(NULL)
	{
		// read file version info
		DWORD dwHandle; // a variable that the function sets to zero. 
		DWORD dwSize = GetFileVersionInfoSize(lpszMoudleName, &dwHandle);
		
		if (dwSize > 0)
		{
			m_pVerInfo = malloc(dwSize); // allocate version info

			if (::GetFileVersionInfo(lpszMoudleName, 0, dwSize, m_pVerInfo))
			{
				LPVOID lpvi;
				UINT uLen;

				if (VerQueryValue(m_pVerInfo, _T("\\"), &lpvi, &uLen))
				{
					//*(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)lpvi;

					// Get translation info
					if (VerQueryValue(m_pVerInfo, "\\VarFileInfo\\Translation", &lpvi, &uLen) && uLen >= 4) 
					{
						m_translation = *(TRANSLATION *)lpvi;
					}
				}
			}
		}
	}
	~CModuleVerInfo()
	{
		if (m_pVerInfo)
		{
			free(m_pVerInfo);
		}
	}

	////
	void GetVerValue(LPCTSTR lpszKeyName, CString & strVerValue)
	{
		if (m_pVerInfo)
		{
			// To get a string value must pass query in the form
			//
			//    "\StringFileInfo\<langID><codepage>\keyname"
			//
			// where <lang-codepage> is the languageID concatenated with the
			// code page, in hex.
			//
			CString query;
			query.Format(_T("\\StringFileInfo\\%04x%04x\\%s"), m_translation.langID, m_translation.charset, lpszKeyName);

			LPCTSTR pVal;
			UINT iLenVal;
			
			if (VerQueryValue(m_pVerInfo, (LPTSTR)(LPCTSTR)query, (LPVOID*)&pVal, &iLenVal)) 
			{
				strVerValue = pVal;
			}
		}
	}

protected:
	void * m_pVerInfo;		// version information buffer

	typedef struct tagTranslation
	{
		WORD langID;			// language ID
		WORD charset;			// character set (code page)
	} TRANSLATION, *PTRANSLATION;

	TRANSLATION m_translation;
};
