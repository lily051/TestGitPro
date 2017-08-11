
// UpLoadLog.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UpLoadLog.h"
#include "UpLoadLogDlg.h"
#include "tlhelp32.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpLoadLogApp

BEGIN_MESSAGE_MAP(CUpLoadLogApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUpLoadLogApp ����

CUpLoadLogApp::CUpLoadLogApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	g_nCount = 0;
	m_url[MAX_PATH] = NULL;
	m_localpath[MAX_PATH] = NULL;
	m_cfilename[MAX_PATH] = NULL;
}


// Ψһ��һ�� CUpLoadLogApp ����

CUpLoadLogApp theApp;


// CUpLoadLogApp ��ʼ��

BOOL CUpLoadLogApp::InitInstance()
{
	
	// �������Ƿ��Ѿ������У�������ֱ���˳�
	if (IsAppRunning())
	{
		return FALSE;
	}
	if(ParseCommandLine() == FALSE)
	{
		return FALSE;
	}
	char temp[MAX_PATH] = {0};
	//�رյ��øó���Ľ���
	StringChange::CStringToChar(m_sAppName+_T(".exe"),temp);
	UPLOAD_LOG_INFO("Close Progress %s",temp);
	CloseProgram(m_sAppName+_T(".exe"));

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

	CUpLoadLogDlg dlg;
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

BOOL CUpLoadLogApp::IsAppRunning()
{
	char temp[MAX_PATH] = {0};
	//������
	CString m_strMutex = _T("UpLoadMutex");
	if ((m_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, m_strMutex)))
	{
		
		StringChange::CStringToChar(_T("UpLoadMutex"),temp);
		UPLOAD_LOG_INFO("Progress %s is exist",temp);
		return TRUE;
	}
	else
	{
		StringChange::CStringToChar(_T("UpLoadMutex"),temp);
		UPLOAD_LOG_INFO("Create Progress %s",temp);
		::CreateMutex(0, FALSE, m_strMutex);
		return FALSE;
	}
}
// ����������
// ����ֵΪ TRUE ��ʾҪ�������к���ĳ���Ϊ FALSE ��ʾ��ֹ��������
BOOL CUpLoadLogApp::ParseCommandLine()
{
	CString sParameter = _T("");
	// ��������в���
	for (int i = 1; i < __argc; i++)
	{
		sParameter = __wargv[i];
		if(sParameter.Left(wcslen(PARA_KEY_CHECLOGPATH)).CompareNoCase(PARA_KEY_CHECLOGPATH) == 0)
		{
			m_sLogPath = sParameter.Mid(wcslen(PARA_KEY_CHECLOGPATH));
		}
		else if(sParameter.Left(wcslen(PARA_KEY_CHECKURL)).CompareNoCase(PARA_KEY_CHECKURL) == 0)
		{
			m_sUrl = sParameter.Mid(wcslen(PARA_KEY_CHECKURL));
		}
		else if(sParameter.Left(wcslen(PARA_KEY_CHECFLAG)).CompareNoCase(PARA_KEY_CHECFLAG) == 0)
		{
			m_iFlag = sParameter.Mid(wcslen(PARA_KEY_CHECFLAG));
		}
		else if(sParameter.Left(wcslen(PARA_KEY_CHECAPPNAME)).CompareNoCase(PARA_KEY_CHECAPPNAME) == 0)
		{
			m_sAppName = sParameter.Mid(wcslen(PARA_KEY_CHECAPPNAME));
		}
		else
		{
			char temp[MAX_PATH] = {0};
			StringChange::CStringToChar(sParameter,temp);
			UPLOAD_LOG_ERROR("Invalid parameter : %s", temp);
			return FALSE;
		}
	}
	return TRUE;
}

void CUpLoadLogApp::CloseProgram(CString strProgram)
{
	HANDLE handle; //����CreateToolhelp32Snapshotϵͳ���վ�� 
	HANDLE handle1; //����Ҫ�������̾�� 
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//���ϵͳ���վ�� 
	PROCESSENTRY32 *info; //����PROCESSENTRY32�ṹ��ָ 
	//PROCESSENTRY32 �ṹ�� dwSize ��Ա���ó� sizeof(PROCESSENTRY32) 

	info = new PROCESSENTRY32; 
	info->dwSize = sizeof(PROCESSENTRY32); 
	//����һ�� Process32First �������ӿ����л�ȡ�����б� 
	Process32First(handle, info); 
	//�ظ����� Process32Next��ֱ���������� FALSE Ϊֹ 
	while(Process32Next(handle, info) != FALSE) 
	{   
		CString strTmp = info->szExeFile;     //ָ���������   
		//strcmp�ַ����ȽϺ���ͬҪ������ͬ   
		//if(strcmp(c, info->szExeFile) == 0 )   
		if (strProgram.CompareNoCase(info->szExeFile) == 0 )   
		{   
			//PROCESS_TERMINATE��ʾΪ����������,FALSE=�ɼ̳�,info->th32ProcessID=����ID    
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID); 
			//�������� 
			TerminateProcess(handle1, 0);    
		}   
	}
	delete info;
	CloseHandle(handle);
}