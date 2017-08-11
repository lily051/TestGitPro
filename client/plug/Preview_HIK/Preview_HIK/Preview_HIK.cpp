
// Preview_HIK.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Preview_HIK.h"
#include "Preview_HIKDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPreview_HIKApp

BEGIN_MESSAGE_MAP(CPreview_HIKApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPreview_HIKApp ����

CPreview_HIKApp::CPreview_HIKApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPreview_HIKApp ����

CPreview_HIKApp theApp;


// CPreview_HIKApp ��ʼ��

BOOL CPreview_HIKApp::InitInstance()
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

    if (!GetCommandLineParam())
    {
        AfxMessageBox(_T("��������ȷ"));
        return FALSE;
    }

    AppFun_GetAppPath(m_strAppPath);
    m_strPicPath = m_strAppPath + _T("pic\\");
    SHCreateDirectoryEx(NULL,m_strPicPath,NULL);

    NET_DVR_Init();

	CPreview_HIKDlg dlg;
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

int CPreview_HIKApp::ExitInstance()
{
    NET_DVR_Cleanup();

    return CWinAppEx::ExitInstance();
}

//��ȡ�����в���
BOOL CPreview_HIKApp::GetCommandLineParam()
{
    LPWSTR *szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (NULL == szArglist)
    {
        return FALSE;
    }

    BOOL bResult = TRUE;
    if (6 == nArgs)//IP PORT USER PWD CHANNEL-->��ͨ��Ԥ��
    {
        m_cmdParam.nParam = nArgs-1;
        m_cmdParam.sIP = szArglist[1];
        m_cmdParam.nPort = _ttoi(szArglist[2]);
        m_cmdParam.sUser = szArglist[3];
        m_cmdParam.sPWD = szArglist[4];
        m_cmdParam.nChannel = _ttoi(szArglist[5]);
        
    }
    else if (5 == nArgs)//IP PORT USER PWD-->���豸Ԥ��
    {
        m_cmdParam.nParam = nArgs-1;
        m_cmdParam.sIP = szArglist[1];
        m_cmdParam.nPort = _ttoi(szArglist[2]);
        m_cmdParam.sUser = szArglist[3];
        m_cmdParam.sPWD = szArglist[4];
    }
    else
    {
        bResult = FALSE;
    }

    LocalFree(szArglist);
    return bResult;
}
