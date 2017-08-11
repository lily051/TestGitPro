#pragma once
#include <atlstr.h>
#pragma comment(lib, "version.lib")

class CModuleVerInfo
{
public:
	CModuleVerInfo(LPCTSTR lpszMoudleName)
		: m_pVerInfo(NULL)
		, m_ptranslation(NULL)
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
					// Get translation info
					if (VerQueryValue(m_pVerInfo, _T("\\VarFileInfo\\Translation"), &lpvi, &uLen) && uLen >= 4) 
					{
						m_ptranslation = (PTRANSLATION)lpvi;
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
		if (m_pVerInfo && m_ptranslation)
		{
			CString query;
			query.Format(_T("\\StringFileInfo\\%04x%04x\\%s"), m_ptranslation->langID, m_ptranslation->charset, lpszKeyName);

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

	PTRANSLATION m_ptranslation;
};