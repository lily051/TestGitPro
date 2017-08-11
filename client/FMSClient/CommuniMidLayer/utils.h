/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       utils.h
*   Description:    实用函数
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/
//lint -library
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
///////////////////////////// chBEGINTHREADEX Macro ///////////////////////////


// This macro function calls the C runtime's _beginthreadex function. 
// The C runtime library doesn't want to have any reliance on Windows' data 
// types such as HANDLE. This means that a Windows programmer needs to cast
// values when using _beginthreadex. Since this is terribly inconvenient, 
// I created this macro to perform the casting.
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#include <process.h>


#include <TlHelp32.h> //获取进程ID函数中需要用到

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
#define _countof(x) (sizeof(x)/sizeof(x[0]))
#endif

void SetRegion(CWnd * pWnd, CBitmap & bitmap);

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

                if (VerQueryValue(m_pVerInfo, L"\\", &lpvi, &uLen))
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
			query.Format(L"\\StringFileInfo\\%04x%04x\\%s", m_translation.langID, m_translation.charset, lpszKeyName);

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





//ljg 2010/03/22 add [[
//加载位图
HBITMAP ResourceLoad_Bmp(UINT nID);
int GetFontSize(int nPixelSize);
int GetFontSize0();
CString GetFontName();
int GetFontCharset();

/*************************************************
Function:    	CheckStrLegality
Description:	查看CString是否合法(% ％　')
Input:          CString str,被检查的字符串
Output:      	
Return:			无
祁梦星 2009年10月26日8:55:25 添加
*************************************************/
//检查密码输入
ENUM_CHECKPATH_LEAGL AppFun_CheckStrLegality(CString csString, int nMaxLen);

BOOL AppFun_CheckStrLegality(CString str);

//获取程序运行的路径
void AppFun_GetAppPath(CString &strPathName);

//根据进程的名称获取进程的ID
DWORD AppFun_GetProcessIDByName(LPCTSTR pName);

//获取选择的路径
CString AppFun_GetDirectoryPath(HWND hWnd, CString strPath=NULL);

//获取硬盘可用信息
CString AppFun_GetDiskAvailableInfo();

/** @fn AppFun_GeLocalIPAddr
*   @brief 或者本机IP
*   @param[out] strIPAddr: ip
*   @param NULL
*   @return NULL
*/
BOOL AppFun_GetLocalIPAddr(CString & strIPAddr);
void AppFun_GetAllLocalIPAddr(std::vector<CString> & vecIP);




// dsx unicode utf8 cp936 字符编码和解码相关的函数---begin
int A2W_Internal(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cbMultiByte,LPWSTR& lpWideCharStr,DWORD* pdwError);
int W2A_Internal(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,LPSTR& lpMultiByteStr,LPCSTR lpDefaultChar,LPBOOL lpUsedDefaultChar,DWORD* pdwError);
bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
std::wstring CP936_A2W(const std::string& strA);
std::string CP936_W2A(const std::wstring& strW);
CStringA CStringW2A(CStringW strText);
std::string UTF8_MFCSTRW2A(const CStringW& strW);
CStringW UTF8_A2MFCSTRW(const std::string& strA);
void CodeSet_Free(void* p);
int My_atoi(CString strNum);

bool ip_valid(const char* ip);

std::string My_itoa(int i);
CString My_itot(int i);
//]]end


void DebugMsg(LPCTSTR pszFormat, ...);
