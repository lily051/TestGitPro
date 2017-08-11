#ifndef GUARD_MYUTILS_H
#define GUARD_MYUTILS_H
#pragma once

#include "TCharExpand.h"
#include <vector>
namespace myutils{

    //bool GetAppFullName(LPTSTR lpFileName, DWORD nSize);
    //bool GetAppFullPath(LPTSTR lpFilePath, DWORD nSize);

    // unicode utf8 cp936 字符编码和解码相关的函数---begin
    bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
    bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
    bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
    bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
    void CodeSet_Free(void* p);
    std::wstring CP936_A2W(const std::string& strA);
    std::string CP936_W2A(const std::wstring& strW);
    std::string CP936_T2A(const std::_tstring& strT);
    //added by ly
    //std::string UTF8_MFCSTRW2A(const CStringW& strW);
    //CStringW UTF8_A2MFCSTRW(const std::string& strA);
    std::string UTF8_W2A(const std::wstring& strW);
    std::wstring UTF8_A2W(const std::string& strA);
    void StdWStrFormat(std::wstring& ws, const wchar_t *_format, ...);
    // unicode utf8 cp936 字符编码和解码相关的函数-----end

    void GetAllLocalIPAddr(std::vector<std::wstring> & vIPs);

    /*************************************************
    Function:      GetProcessIDByName
    Description:  根据进程的名称获取进程的ID
    Input:        pName进程名称
    Output:        
    Return:       -1获取失败，非-1获取成功
    *************************************************/
    DWORD GetProcessIDByName(LPCTSTR pName);

    std::string DecipherStr(std::string sCipher);
    std::string CipherStr(std::string sPlain);

}

namespace myutils_macro{

    /************************ macros begin[*******************************/

#ifndef myDebugBreak
#ifdef _X86_    
#define myDebugBreak()    _asm { int 3 }
#else
#define myDebugBreak()    DebugBreak()
#endif
#endif


    /************************ macros end  ]*******************************/

}



#endif // end of GUARD_MYUTILS_H
