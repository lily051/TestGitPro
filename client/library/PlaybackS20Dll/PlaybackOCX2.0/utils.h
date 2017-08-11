#pragma once

void AppFun_GetAppPath(CString &strPathName);

void CodeSet_Free(void* p);
int W2A_Internal(
				 UINT CodePage, 
				 DWORD dwFlags, 
				 LPCWSTR lpWideCharStr,
				 int cchWideChar, 
				 LPSTR& lpMultiByteStr, 
				 LPCSTR lpDefaultChar,    
				 LPBOOL lpUsedDefaultChar,
				 DWORD* pdwError);
bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);