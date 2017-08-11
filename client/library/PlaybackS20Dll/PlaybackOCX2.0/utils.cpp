//lint -library
#include "stdafx.h"
#include "utils.h"

void AppFun_GetAppPath(CString &strPathName)
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

void CodeSet_Free(void* p)
{
	if (p)
	{
		::free(p);
	}
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

		if (NULL != pdwError)
		{
			*pdwError = dwError;
		} 
		return 0;
	}

	lpMultiByteStr = (LPSTR)::malloc(nRequired * sizeof(CHAR)/* + 1*/);
	if (!lpMultiByteStr)
	{	
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

		if (NULL != pdwError)
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

bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError)
{
	cbMultiByte = W2A_Internal(936, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, NULL, NULL, pdwError);
	if (!cbMultiByte)
	{
		//MYUTILS_ERROR("w2ai failed");

		return false;
	}

	return true;
}