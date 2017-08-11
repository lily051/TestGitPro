

#include "stdafx.h"
#include "utils.h"
#include <regex>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

// 获取本机的IP地址
void GetLocalIP(std::vector<CString>& vecIP)
{
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    pIPAddrTable = (MIB_IPADDRTABLE*) GlobalAlloc(GPTR, sizeof(MIB_IPADDRTABLE));
    if ( pIPAddrTable ) 
    {
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
        {
            GlobalFree( pIPAddrTable );
            pIPAddrTable = (MIB_IPADDRTABLE *) GlobalAlloc (GPTR, dwSize);
        }
    }
    else
    {
        OutputDebugString(_T("@ljg-Memory allocation failed."));
    }

    if ( pIPAddrTable ) 
    {
        // Make a second call to GetIpAddrTable to get the
        // actual data we want
        if ( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) == NO_ERROR ) 
        { 
            for (unsigned int i=0;i<dwSize/sizeof(MIB_IPADDRTABLE);++i)
            {
                if (pIPAddrTable->table[i].dwAddr != 0)
                {
                    in_addr in;
                    in.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
                    CString tempIP;
                    tempIP = ::inet_ntoa(in);
                    vecIP.push_back(tempIP);
                }
            }
        }
        else 
        {
            OutputDebugString(_T("@ljg-Call to GetIpAddrTable failed."));
        }
    }

    if (pIPAddrTable)
    {
        GlobalFree( pIPAddrTable );
    }
}

// 查看CString是否合法(% ％　')
ENUM_CHECKPATH_LEAGL StringLegality(CString str, int nMaxLen)
{
    str.Trim();
    if (0 == _tcscmp(str, L""))
    {
        return CHECKPATH_EMPTY;
    }

    if (str.Find(_T("\\"))>=0 || str.Find(_T("/"))>=0 || str.Find(_T(":"))>=0 ||
        str.Find(_T("*"))>=0 || str.Find(_T("*"))>=0 || str.Find(_T("&"))>=0 ||
        str.Find(_T("?"))>=0 ||  str.Find(_T("\""))>=0 || str.Find(_T("<"))>=0 ||
        str.Find(_T("|"))>=0 || str.Find(_T("'"))>=0 || str.Find(_T("%"))>=0 ||
        str.Find(_T("％"))>=0 ||str.Find(_T(">"))>=0)
    {
        return CHECKPATH_ILLEAGLCHAR;
    }

    if (0 == _tcscmp(str, _T(" ")))
    {
        return CHECKPATH_EMPTY;
    }

    if (str.GetLength() > nMaxLen)
    {
        return CHECKPATH_OVERLENGTH;
    }

    return CHECKPATH_SUCCESS;
}

// 检查是否是IP地址
bool ip_valid(const char* ip)
{
    std::tr1::regex reg("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
    bool ret = std::tr1::regex_match(ip, reg);
    return ret;
}

// 整形转string
std::string My_itoa(int i)
{
    std::stringstream ss;
    ss << i;
    std::string strTemp = ss.str();
    return strTemp;
}

// 整形转CString
CString My_itot(int i)
{
    CString strTemp;
    strTemp.Format(_T("%d"), i);
    return strTemp;
}

//lint -efunc(438, DebugMsg)
void DebugMsg(LPCTSTR pszFormat, ...) throw()
{
    TCHAR buf[4096] = {0};
    va_list arglist;
    va_start(arglist, pszFormat);
    _vsntprintf_s(buf, sizeof(buf) / sizeof(*buf) - 1, pszFormat, arglist); //lint !e534
    va_end(arglist);

    OutputDebugString(buf);
}
//lint +efunc(438, DebugMsg)

void CodeSet_Free(void* p)
{
	if (p)
	{
		::free(p);
	}
}

int A2W_Internal(
				 UINT CodePage, 
				 DWORD dwFlags,         
				 LPCSTR lpMultiByteStr, 
				 int cbMultiByte,       
				 LPWSTR& lpWideCharStr,
				 DWORD* pdwError)
{
	int nRequired = ::MultiByteToWideChar(CodePage, dwFlags,
		lpMultiByteStr, cbMultiByte, lpWideCharStr, 0);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		}
		return 0;
	}

	lpWideCharStr = (LPWSTR)::malloc(nRequired * sizeof(WCHAR)/* + 1*/);
	if (!lpWideCharStr)
	{		
		if (pdwError)
		{
			*pdwError = ERROR_INSUFFICIENT_BUFFER;
		} 
		return 0;
	}

	nRequired = ::MultiByteToWideChar(CodePage, dwFlags,
		lpMultiByteStr, cbMultiByte, lpWideCharStr, nRequired);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		} 
		::free(lpWideCharStr);
		lpWideCharStr = NULL;
		return 0;
	}

	//lpWideCharStr[nRequired * sizeof(WCHAR)] = '\0';

	return nRequired;
}

//lint -e413
int W2A_Internal(
				 UINT CodePage, 
				 DWORD dwFlags, 
				 LPCWSTR lpWideCharStr,
				 int cchWideChar, 
				 LPSTR& lpMultiByteStr, 
				 LPCSTR lpDefaultChar,    
				 LPBOOL lpUsedDefaultChar,
				 DWORD* pdwError)
{
	int nRequired = ::WideCharToMultiByte(CodePage, dwFlags,
		lpWideCharStr, cchWideChar, lpMultiByteStr, 0, lpDefaultChar, lpUsedDefaultChar);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		} 
		return 0;
	}

	lpMultiByteStr = (LPSTR)::malloc(nRequired * sizeof(CHAR)/* + 1*/);
	if (!lpMultiByteStr)
	{	
		if (pdwError)
		{
			*pdwError = ERROR_INSUFFICIENT_BUFFER;
		} 
		return 0;
	}

	nRequired = ::WideCharToMultiByte(CodePage, dwFlags,
		lpWideCharStr, cchWideChar, lpMultiByteStr, nRequired, lpDefaultChar, lpUsedDefaultChar);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		} 
		::free(lpMultiByteStr);
		lpMultiByteStr = NULL;
		return 0;
	}

	//lpMultiByteStr[nRequired * sizeof(CHAR)] = '\0';

	return nRequired;
}

bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError)
{
	cbMultiByte = W2A_Internal(CP_UTF8, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, NULL, NULL, pdwError);
	if (!cbMultiByte)
	{
		return false;
	}

	return true;
}

bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError)
{
	cchWideChar = A2W_Internal(CP_UTF8, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, pdwError);
	if (!cchWideChar)
	{
		return false;
	}

	return true;
}

string UTF8_MFCSTRW2A(const CStringW& strW)
{
	string strA;
	CHAR* pTemp = 0;
	int cchar = 0;
	if (UTF8_W2A((LPCWSTR)strW, strW.GetLength() + 1, pTemp, cchar))
	{
		strA = pTemp;
		CodeSet_Free(pTemp);
	}
	else
	{
	}

	return strA;
}

CStringW UTF8_A2MFCSTRW(const string& strA)
{
	CStringW strW;
	WCHAR* pTemp = 0;
	int cwchar = 0;
	if (UTF8_A2W(strA.c_str(), strA.size() + 1, pTemp, cwchar))
	{
		strW = pTemp;
		CodeSet_Free(pTemp);
	}
	else
	{
	}

	return strW;
}

