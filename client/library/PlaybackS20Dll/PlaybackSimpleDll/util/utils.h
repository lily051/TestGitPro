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

#define SAFEDELETE(ptr) { if (ptr != NULL) { delete ptr; ptr = NULL; } }

//
#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof(x[0]))
#endif

Image * LoadImageFromResource(UINT uID);
Bitmap * LoadBitmapFromResource(UINT uID);

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
BOOL AppFun_GetLocalIPAddr(CString& strIPAddr);

/** @fn SaveBmpTofile
*   @brief 保存bmp数据到文件中
*   @param[in]  hbmp: bmp内存数据
*   @param[in]  path: 保存到地址
*   @return TRUE成功
*/
BOOL SaveBmpTofile(HBITMAP hbmp, CString path);

// dsx unicode utf8 cp936 字符编码和解码相关的函数---begin
int A2W_Internal(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cbMultiByte,LPWSTR& lpWideCharStr,DWORD* pdwError);
int W2A_Internal(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,LPSTR& lpMultiByteStr,LPCSTR lpDefaultChar,    				 LPBOOL lpUsedDefaultChar,DWORD* pdwError);
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


//按钮提示符
HWND MyCreateToolTip(int toolID, HWND hDlg, TCHAR* pText);
//]]end


//字符转换

#if (defined _UNICODE) || (defined UNICODE)
#define _tstring wstring
#else
#define _tstring string
#endif

static std::wstring Ansi2Unicode(std::string szAnsi) 
{
	//calc block size to be returned
	int len = MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), NULL, 0);
	//malloc and fill the returned block
	wchar_t* szUnicode = new wchar_t[len+1];
	MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), szUnicode, len);
	szUnicode[len] = 0;
	std::wstring rs = szUnicode;
	delete[] szUnicode;

	return rs;
}
static std::string Unicode2Ansi(std::wstring szUnicode) 
{
	//calc block size to be returned
	int len = WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
	//malloc and fill the returned block
	char* szAnsi = new char[len + 1];
	WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), szAnsi, len, NULL, NULL);
	szAnsi[len] = 0;
	std::string rs = szAnsi;
	delete[] szAnsi;

	return rs;
}

static std::wstring Utf82Unicode(std::string szUtf8) 
{
	//calc block size to be returned
	int len = MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), NULL, 0);
	//malloc and fill the returned block
	wchar_t* szUnicode = new wchar_t[len+1];
	MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), szUnicode, len);
	szUnicode[len] = 0;
	std::wstring rs = szUnicode;
	delete[] szUnicode;

	return rs;
}

static std::string Unicode2Utf8(std::wstring szUnicode) 
{
	//calc block size to be returned
	int len = WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
	//malloc and fill the returned block
	char* szUtf8 = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), szUtf8, len, NULL, NULL);
	szUtf8[len] = 0;
	std::string rs = szUtf8;
	delete[] szUtf8;

	return rs;
}

static std::string Ansi2Utf8(std::string szAnsi) 
{
	return Unicode2Utf8(Ansi2Unicode(szAnsi));
}

static std::string Utf82Ansi(std::string szUtf8) 
{
	return Unicode2Ansi(Utf82Unicode(szUtf8));
}

static std::_tstring Ansi2Tstring(std::string szAnsi)
{
#if (defined _UNICODE) || (defined UNICODE)
	return Ansi2Unicode(szAnsi);
#else
	return szAnsi;
#endif
}

static std::_tstring Unicode2Tstring(std::wstring szUnicode)
{
#if (defined _UNICODE) || (defined UNICODE)
	return szUnicode;
#else
	return Unicode2Ansi(szUnicode);
#endif
}

static std::string Tstring2Ansi(std::_tstring _tstr)
{
#if (defined _UNICODE) || (defined UNICODE)
	return Unicode2Ansi(_tstr);
#else
	return _tstr;
#endif
}

static std::wstring Tstring2Unicode(std::_tstring _tstr)
{
#if (defined _UNICODE) || (defined UNICODE)
	return _tstr;
#else
	return Ansi2Unicode(_tstr);
#endif
}

void WaitForWindowThread(HANDLE hThread);
BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);
BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);
/**   @fn          CheckFilePathSpace
 *    @brief   	   检查磁盘空间.
 *    @param[in]   strFilePath:文件路径.
 *    @param[in]   uMinSpace:最小磁盘空间.
 *    @return      
 */
BOOL CheckFilePathSpace(CString strFilePath,ULONGLONG uMinSpace = 50LL * 1024LL * 1024LL);

/**   @fn          GetFileSize
 *    @brief   	   文件大小换算.
 *    @param[in]   dwFileSize:大小换算.
 *    @param[in]   
 *    @return      文件大小字符串.
 */
CString GetFileSize(DWORD dwFileSize);

void GetErrorMsg(DWORD dwError, CString& strMsg);