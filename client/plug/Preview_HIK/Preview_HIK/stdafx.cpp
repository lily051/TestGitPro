
// stdafx.cpp : 只包括标准包含文件的源文件
// Preview_HIK.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

void AppFun_GetAppPath(CString &strPathName)
{
    TCHAR szFileName[MAX_PATH];
    if (0 != GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH - 1))
    {
        szFileName[MAX_PATH - 1] = _T( '\0');
    }
    TCHAR * p = _tcsrchr(szFileName, _T('\\')) + 1;
    if (p)
    {
        *p = _T('\0');
        strPathName = szFileName;
    }
}
