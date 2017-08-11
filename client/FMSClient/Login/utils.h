
#pragma once

#include <string>
#include <vector>
using namespace std;

enum ENUM_CHECKPATH_LEAGL
{
    CHECKPATH_SUCCESS = 0,
    CHECKPATH_EMPTY,
    CHECKPATH_ILLEAGLCHAR,
    CHECKPATH_OVERLENGTH
};

/**
* @brief	获取本机的IP地址
* @param	
* @return	
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
void GetLocalIP(std::vector<CString>& vecIP);

/**
* @brief	查看CString是否合法(% ％　')
* @param	
* @return	
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
ENUM_CHECKPATH_LEAGL StringLegality(CString str, int nMaxLen);

/**
* @brief	检查是否是IP地址
* @param	
* @return	
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
bool ip_valid(const char* ip);

/**
* @brief	整形转string
* @param	
* @return	
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
std::string My_itoa(int i);

/**
* @brief	整形转CString
* @param	
* @return	
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
CString My_itot(int i);

void DebugMsg(LPCTSTR pszFormat, ...);

void CodeSet_Free(void* p);
int A2W_Internal(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cbMultiByte,LPWSTR& lpWideCharStr,DWORD* pdwError);
int W2A_Internal(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,LPSTR& lpMultiByteStr,LPCSTR lpDefaultChar,LPBOOL lpUsedDefaultChar,DWORD* pdwError);
bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
CStringW UTF8_A2MFCSTRW(const std::string& strA);
std::string UTF8_MFCSTRW2A(const CStringW& strW);