#include "stdafx.h"
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include "TCharExpand.h"
#include "myutils.h"
#include "../hlogconfig.h"
#include <string>
#include "../include/hak.h"

using namespace std;

#pragma warning(push)
#pragma warning(disable:4996)


namespace myutils{
    int A2W_Internal(
        UINT CodePage, 
        DWORD dwFlags,         
        LPCSTR lpMultiByteStr, 
        int cbMultiByte,       
        LPWSTR& lpWideCharStr,       
        DWORD* pdwError);
    int W2A_Internal(
        UINT CodePage, 
        DWORD dwFlags, 
        LPCWSTR lpWideCharStr,
        int cchWideChar, 
        LPSTR& lpMultiByteStr, 
        LPCSTR lpDefaultChar,    
        LPBOOL lpUsedDefaultChar,
        DWORD* pdwError);

// added by ly
#if 0
    bool GetAppFullName(LPTSTR lpFileName, DWORD nSize)
    {
        TCHAR szAppPath[MAX_PATH + 1];
        DWORD dwSize = ::GetModuleFileName(NULL, szAppPath, _countof(szAppPath)); 
        if (0 == dwSize || _countof(szAppPath) == dwSize
            || nSize < dwSize + 1)
        {
            MYUTILS_ERROR("gmfn failed, dwSize=%u,nSize=%u,Error=%u", ::GetLastError());

            return false;
        }
        
        _tcscpy(lpFileName, szAppPath);

        return true;
    }

    bool GetAppFullPath(LPTSTR lpFilePath, DWORD nSize)
    {
        TCHAR szAppPath[MAX_PATH + 1];
        if (!GetAppFullName(szAppPath, _countof(szAppPath)))
        {
            MYUTILS_ERROR("gafn failed");

            return false;
        }

        TCHAR* pSplash = _tcsrchr(szAppPath, '\\');
        if (!pSplash)
        {
            MYUTILS_ERROR("rchr failed, szAppPath=%s", szAppPath);

            return false;
        }
        *(++pSplash) = 0;

        if (_tcslen(szAppPath) + 1 > nSize)
        {
            MYUTILS_ERROR("AppPath is too long, nSize=%d", nSize);

            return false;
        }

        _tcscpy(lpFilePath, szAppPath);

        return true;
    }
#endif

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
            MYUTILS_ERROR("mbtwc failed, Error=%u", dwError);

            if (NULL != pdwError)
            {
                *pdwError = dwError;
            }            
            return 0;
        }

        lpWideCharStr = (LPWSTR)::malloc(nRequired * sizeof(WCHAR));
        if (!lpWideCharStr)
        {
            MYUTILS_ERROR("malloc failed");

            if (NULL != pdwError)
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
            MYUTILS_ERROR("mbtwc failed, Error=%u", dwError);

            if (NULL != pdwError)
            {
                *pdwError = dwError;
            } 
            ::free(lpWideCharStr);
            lpWideCharStr = NULL;
            return 0;
        }

        return nRequired;
    }

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
            MYUTILS_ERROR("wctmb failed, Error=%u", dwError);

            if (NULL != pdwError)
            {
                *pdwError = dwError;
            } 
            return 0;
        }

        lpMultiByteStr = (LPSTR)::malloc(nRequired * sizeof(CHAR));
        if (!lpMultiByteStr)
        {
            MYUTILS_ERROR("malloc failed");

            if (NULL != pdwError)
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
            MYUTILS_ERROR("wctmb failed, Error=%u", dwError);

            if (NULL != pdwError)
            {
                *pdwError = dwError;
            } 
            ::free(lpMultiByteStr);
            lpMultiByteStr = NULL;
            return 0;
        }

        return nRequired;
    }

    bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError)
    {
        cchWideChar = A2W_Internal(936, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, pdwError);
        if (!cchWideChar)
        {
            MYUTILS_ERROR("a2wi failed");

            return false;
        }

        return true;
    }

    bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError)
    {
        cbMultiByte = W2A_Internal(936, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, NULL, NULL, pdwError);
        if (!cbMultiByte)
        {
            MYUTILS_ERROR("w2ai failed");

            return false;
        }

        return true;
    }

    bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError)
    {
        cchWideChar = A2W_Internal(CP_UTF8, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, pdwError);
        if (!cchWideChar)
        {
            MYUTILS_ERROR("a2wi failed");

            return false;
        }

        return true;
    }

    bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError)
    {
        cbMultiByte = W2A_Internal(CP_UTF8, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, NULL, NULL, pdwError);
        if (!cbMultiByte)
        {
            MYUTILS_ERROR("w2ai failed");

            return false;
        }

        return true;
    }

    void CodeSet_Free(void* p)
    {
        ::free(p);
    }

    wstring CP936_A2W(const string& strA)
    {
        wstring strW;
        WCHAR* pTemp = 0;
        int cwchar = 0;
        if (CP936_A2W(strA.c_str(), strA.size() + 1, pTemp, cwchar))
        {
            strW = pTemp;
            CodeSet_Free(pTemp);
        }
        else
        {
            MYUTILS_ERROR("ca2w failed");
        }

        return strW;
    }
    string CP936_W2A(const wstring& strW)
    {
        string strA;
        CHAR* pTemp = 0;
        int cchar = 0;
        if (CP936_W2A(strW.c_str(), strW.size() + 1, pTemp, cchar))
        {
            strA = pTemp;
            CodeSet_Free(pTemp);
        }
        else
        {
            MYUTILS_ERROR("cw2a failed");
        }

        return strA;
    }

    std::string CP936_T2A(const std::_tstring& strT)
    {
#ifdef UNICODE
        return CP936_W2A(strT);
#else
        return strT;
#endif
    }

    //added by ly
#if 0
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
            MYUTILS_ERROR("UTF8_W2A failed");
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
            MYUTILS_ERROR("UTF8_A2W failed");
        }

        return strW;
    }
#endif

    string UTF8_W2A(const wstring& strW)
    {
        string strA;
        CHAR* pTemp = 0;
        int cchar = 0;
        if (UTF8_W2A(strW.c_str(), strW.size() + 1, pTemp, cchar))
        {
            strA = pTemp;
            CodeSet_Free(pTemp);
        }
        else
        {
            MYUTILS_ERROR("UTF8_W2A failed");
        }

        return strA;
    }

    wstring UTF8_A2W(const string& strA)
    {
        wstring strW;
        WCHAR* pTemp = 0;
        int cwchar = 0;
        if (UTF8_A2W(strA.c_str(), strA.size() + 1, pTemp, cwchar))
        {
            strW = pTemp;
            CodeSet_Free(pTemp);
        }
        else
        {
            MYUTILS_ERROR("UTF8_A2W failed");
        }

        return strW;
    }

    void StdWStrFormat(std::wstring& wsFoo, const wchar_t *_format, ...)
    {
        va_list ap;
        va_start(ap, _format);
        int nLen = _vscwprintf(_format, ap) + 1;
        wchar_t* buffer = (wchar_t*) malloc(nLen * sizeof(wchar_t));
        vswprintf_s(buffer, nLen, _format, ap);    
        //PC-Lint error 438
        //va_end(ap);
        wsFoo = buffer;
        free(buffer);
    }

#pragma comment(lib,"Iphlpapi.lib")

    void GetAllLocalIPAddr(std::vector<std::wstring> & vIPs)
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
                        char* pAddr = ::inet_ntoa(in);
                        if (NULL != pAddr)
                        {
                            string sTempIp(pAddr);
                            wstring wsTempIp = CP936_A2W(sTempIp);
                            vIPs.push_back(wsTempIp);
                        }
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

    /*************************************************
    Function:      GetProcessIDByName
    Description:  根据进程的名称获取进程的ID
    Input:        pName进程名称
    Output:        
    Return:       -1获取失败，非-1获取成功
    *************************************************/
    DWORD GetProcessIDByName(LPCTSTR pName)
    {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        DWORD dwPID = -1;
        if (INVALID_HANDLE_VALUE == hSnapshot) 
        {
            return -1;
        }
        PROCESSENTRY32 pe = { sizeof(pe) };
        BOOL fOk;
        for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) 
        {
            if (!_tcscmp(pe.szExeFile, pName)) 
            {
                CloseHandle(hSnapshot);
                dwPID = pe.th32ProcessID;
                return dwPID;
            }
        }
        CloseHandle(hSnapshot);
        return -1;
    }

    std::string DecipherStr(std::string sCipher)
    {
        // 缓冲区的大小应该至少大于等于 p2密文字符串的长度 / 32 * 16
        int nLen = sCipher.size() / 32 * 16 + 1;    // 保证至少为1
        vector<char> vPlain(nLen, '\0');
        if(0 != Hak7(&vPlain[0], (char*)sCipher.c_str(), (char*)&nLen))
        {
            CMS_ERROR("decipher failed");
        }
        return string(&vPlain[0]);
    }

    std::string CipherStr(std::string sPlain)
    {
        // 其缓冲区的大小应该至少大于等于(p2明文字符串的长度 / 16 + 1) * 32 + 1
        int nLen = (sPlain.size() / 16 + 1) * 32 + 1;   // 保证至少为1
        vector<char> vCipher(nLen, '\0');
        if(0 != Hak5(&vCipher[0], (char*)sPlain.c_str(), (char*)&nLen))
        {
            CMS_ERROR("cipher failed");
        }
        return string(&vCipher[0]);
    }

}

#pragma warning(pop)

