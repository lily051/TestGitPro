
// ResPacker.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ResPacker.h"
#include "ResPackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CResPackerApp

BEGIN_MESSAGE_MAP(CResPackerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CResPackerApp ����

CResPackerApp::CResPackerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CResPackerApp ����

CResPackerApp theApp;

static ULONG_PTR fui_token = NULL;
// CResPackerApp ��ʼ��

BOOL CResPackerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	GdiplusStartupInput input;
	GdiplusStartup(&fui_token, &input, NULL);

	CResPackerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int SuperMkDirs(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    ASSERT(lpPathName);
    int nResult = -1;
    if (!lpPathName || _tcslen(lpPathName) <= 3)
    {
        OutputDebugStringA("invalid parameter\n");
        return nResult;
    }
    CString strPath = lpPathName;
    int nPos = strPath.Find(_T(":\\"));
    if (nPos == -1)
    {
        OutputDebugStringA("Expect a absolute path\n");
        return nResult;
    }
    if (strPath.Right(1) != _T('\\'))
    {
        strPath += _T("\\");
    }
    ++nPos;
    while ((nPos = strPath.Find(_T('\\'), nPos+1)) != -1)
    {
        TRACE(strPath.Left(nPos) + _T("\n"));
        if (!CreateDirectory(strPath.Left(nPos+1), lpSecurityAttributes))
        {
            if (GetLastError() != ERROR_ALREADY_EXISTS)
            {
                return -1;
            }
            else
            {
                nResult = 1;
            }
        }  
        else
        {
            nResult = 0;
        }
    }

    return nResult;
}

// ����һ����������ɵ�����ַ���
char *GenRandomString(char *pStr, int nLen)
{
    nLen -= 1;
    int *pInt32 = (int*) pStr;

    int i = 0;
    for (; i < nLen / 4; ++i)
    {
        pInt32[i] = (rand() | 0x30303030) & 0x39393939;
    }
    i *= 4;

    for (; i < nLen; ++i)
    {
        pStr[i] = rand() % (0x39 - 0x30) + 0x30;
    }
    pStr[i] = '\0';

    return pStr;
}

//ɾ��һ���ļ����µ���������  
void myDeleteDirectory(CString strDelPath)
{
    CFileFind finder;
    CString strPath;
    strPath.Format(_T("%s\\*.*"), (LPCTSTR)strDelPath);
    BOOL bWorking = finder.FindFile(strPath);
    while(bWorking)
    {  
        bWorking = finder.FindNextFile();
        if(finder.IsDirectory() && !finder.IsDots())
        {
            //�����ļ���  
            myDeleteDirectory(finder.GetFilePath()); //�ݹ�ɾ���ļ���
            RemoveDirectory(finder.GetFilePath());
        }
        else
        {//�����ļ�
            DeleteFile(finder.GetFilePath());
        }
    }
    RemoveDirectory(strDelPath); 
}

void myCopyDirectory(CString strSrc, CString strDst)
{
    // ����Ŀ���ļ���
    if (PathFileExists(strDst))
    {
        myDeleteDirectory(strDst);
    }
    SuperMkDirs(strDst);

    CFileFind finder;
    CString strPath(strSrc);
    strPath.AppendFormat(_T("\\*.*"), (LPCTSTR)strSrc);
    BOOL bWorking = finder.FindFile(strPath);
    while(bWorking)
    {  
        bWorking = finder.FindNextFile();
        if (!finder.IsDots() && !finder.IsSystem())
        {
            if(finder.IsDirectory())
            {
                //���ļ��� ���� ���Ʋ��� . �� ..  
                myCopyDirectory(finder.GetFilePath(), strDst + _T("\\") + finder.GetFileName()); //�ݹ鴴���ļ���+"/"+finder.GetFileName()  
            }  
            else
            {
                CString s = finder.GetFilePath();
                //���ļ� ��ֱ�Ӹ���  
                CopyFile(finder.GetFilePath(), strDst + _T("\\") + finder.GetFileName(), FALSE);
            }
        }
    }
}

