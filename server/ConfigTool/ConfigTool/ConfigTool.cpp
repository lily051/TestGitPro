
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
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CConfigToolApp ����

CConfigToolApp theApp;


// CConfigToolApp ��ʼ��

BOOL CConfigToolApp::InitInstance()
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
	SetRegistryKey(_T("The application wizard generates a local application"));

	InitWorkPath();

	CConfigToolDlg dlg;
/*lint -e1414*/
	m_pMainWnd = &dlg;
/*lint +e1414*/ 
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
