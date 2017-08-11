
// ConfigTool.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ConfigTool.h"
#include "ConfigToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void InitWorkPath()
{
	TCHAR path[256] = {0};
	GetModuleFileName(NULL, path, 256);
	for (int i = (int)_tcslen(path); i >= 0; --i)
	{
		if (path[i] == _T('\\'))
		{
			path[i]=0;
			break;
		}
	}
	SetCurrentDirectory(path);
}


// CConfigToolApp

BEGIN_MESSAGE_MAP(CConfigToolApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CConfigToolApp ����

CConfigToolApp::CConfigToolApp()
{
	m_hMutex = NULL;
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CConfigToolApp ����

CConfigToolApp theApp;


// CConfigToolApp ��ʼ��

BOOL CConfigToolApp::InitInstance()
{
	// ͬһ������ֻ����һ��ʵ��
	CString appMutexName(_T("Global\\Upload Client"));
	m_hMutex = CreateMutex(NULL, FALSE, appMutexName);
	if (NULL == m_hMutex)
	{
		MessageBox(NULL, _T("����ϴ��ͻ�������ʧ�ܣ�"), _T("��ʾ"), MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, _T("����ϴ��ͻ���ֻ������һ����"), _T("��ʾ"), MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}

	CString moduleFileDir;
	GetModuleFileName(NULL, moduleFileDir.GetBuffer(MAX_PATH), MAX_PATH - 1);
	moduleFileDir.ReleaseBuffer(MAX_PATH - 1);
	int dotPos = moduleFileDir.ReverseFind('\\');
	if (dotPos > 0)
	{
		moduleFileDir = moduleFileDir.Left(dotPos + 1);
		// ���õ�ǰ����Ŀ¼Ϊ���Ź���ִ�г�������Ŀ¼��
		SetCurrentDirectory(moduleFileDir);
	}
	else
	{
		moduleFileDir.Empty();
	}

	// д��ע�������������
	WCHAR szAppName[MAX_PATH];
	GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);
	CString strAppPath(szAppName);
	LPCTSTR lpszKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	CRegKey regeditKey;
	if (ERROR_SUCCESS == regeditKey.Open(HKEY_LOCAL_MACHINE, lpszKey))
	{
		// LONG lrtn = 
		regeditKey.SetStringValue(_T("Upload Client"), strAppPath);
		regeditKey.Close();
	}
	else
	{
		MessageBox(NULL, _T("ע����ʧ�ܣ�"), _T("��ʾ"), MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
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
	SetRegistryKey(_T("The application wizard generates a local application"));

	InitWorkPath();

	CConfigToolDlg *dlg = new CConfigToolDlg;
/*lint -e1414*/
	m_pMainWnd = dlg;
/*lint +e1414*/ 
	INT_PTR nResponse = dlg->DoModal();
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
	delete dlg;
	return FALSE;
}
