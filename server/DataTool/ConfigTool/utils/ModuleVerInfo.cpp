#include "stdafx.h"
#include "ModuleVerInfo.h"
#include "myutils.h"

#pragma comment(lib, "version.lib")


CModuleVerInfo::CModuleVerInfo(LPCTSTR lpszMoudleName)
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
                if (VerQueryValue(m_pVerInfo, L"\\VarFileInfo\\Translation", &lpvi, &uLen) && uLen >= 4) 
                {
                    m_translation = *(TRANSLATION *)lpvi;
                }
            }
        }
    }
}

CModuleVerInfo::~CModuleVerInfo()
{
    if (m_pVerInfo)
    {
        free(m_pVerInfo);
    }
}

////
void CModuleVerInfo::GetVerValue(LPCTSTR lpszKeyName, std::wstring & wsVerValue)
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
        std::wstring query;
        myutils::StdWStrFormat(query
                                , L"\\StringFileInfo\\%04x%04x\\%s"
                                , m_translation.langID
                                , m_translation.charset
                                , lpszKeyName);
        
        LPCTSTR pVal;
        UINT iLenVal;

        if (VerQueryValue(m_pVerInfo, query.c_str(), (LPVOID*)&pVal, &iLenVal)) 
        {
            wsVerValue = pVal;
        }
    }
}