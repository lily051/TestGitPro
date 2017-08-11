
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Preview_HIK.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

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
