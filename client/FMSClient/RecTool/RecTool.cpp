
// RecTool.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RecTool.h"
#include "RecToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* CLIENT_CONFIG_FILENAME = _T("FMSClient.ini");
const TCHAR* MODULE_CONFIG_PATH  = _T("Config\\ModuleConfig.xml");      // ģ�������ļ�λ��
const TCHAR* FUI_RES_PASSWORD = _T("Hik_Financial_8100");               // Fui��Դ����

// CRecToolApp

BEGIN_MESSAGE_MAP(CRecToolApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRecToolApp ����

CRecToolApp::CRecToolApp()
:m_nSkinType(1)
,m_nLanguage(0)
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRecToolApp ����

CRecToolApp theApp;


// CRecToolApp ��ʼ��

BOOL CRecToolApp::InitInstance()
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

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strAppName = szPath;

	//��ȡ�汾
	//GetVerValue(_T("FileVersion"), m_strAppVer);

	//����Ŀ¼
	(_tcsrchr(szPath, _T('\\')))[0] = 0;
	m_strAppPath = szPath;
	m_strDefaultCapPath = m_strAppPath + _T("\\RecordFiles\\");

	 FUiInit();

	ReadConfig(m_strAppCapPath);

	 GetConfigInfo();

	CRecToolDlg dlg;
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

int CRecToolApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	FUiCleanup();

	return CWinAppEx::ExitInstance();
}


void CRecToolApp::GetConfigInfo()
{
	//�����ļ�·��
	CString strConfigPath = m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(CLIENT_CONFIG_FILENAME);

	//��ʼ��Ƥ�����
	m_nSkinType = GetPrivateProfileInt(_T("FMSClient"), _T("SkinScheme"), 1, strConfigPath);
	CString strImagePath;
	strImagePath.Format(_T("\\image\\%d.rc"), m_nSkinType);
	FUiSetRes(m_strAppPath + strImagePath, FUI_RES_PASSWORD);

	TCHAR szCustomTitle[MAX_PATH] = {0};
	GetPrivateProfileString(_T("FMSClient"), _T("CustomTitle"), _T("��Ʒ��չ���ϵͳ"), szCustomTitle, MAX_PATH, strConfigPath);
	m_sCustomTitle = szCustomTitle;

	TCHAR szCopyright[MAX_PATH] = {0};
	GetPrivateProfileString(_T("FMSClient"), _T("Copyright"), _T(""), szCopyright, MAX_PATH, strConfigPath);
	m_sCopyright = szCopyright;
}

BOOL CRecToolApp::ReadConfig(CString& m_strAppCapPath)
{
	TCHAR szDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szDir, MAX_PATH);
	PathRemoveFileSpec(szDir); //��·��ĩβ���ļ����ͷ�б��ȥ��
	TCHAR szPath[MAX_PATH] = {0};
	PathCombine(szPath, szDir, CLIENT_CONFIG_FILENAME);
	TCHAR szBuf[1024] = {0};
	GetPrivateProfileString(_T("LocalConfig"), _T("CapPath"), theApp.m_strDefaultCapPath, szBuf, 1024, szPath);
	m_strAppCapPath.Format(_T("%s"), szBuf);

	return TRUE;
}