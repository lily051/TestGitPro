
// FixToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FixTool.h"
#include "FixToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFixToolDlg �Ի���




CFixToolDlg::CFixToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFixToolDlg::IDD, pParent)
{
	m_sPackPath = _T("");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFixToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBX_PRODUCT, m_comProduct);
	DDX_Control(pDX, IDC_EDIT_XML_PATH, m_editPackPath);
	DDX_Text(pDX, IDC_EDIT_XML_PATH, m_sPackPath);
}

BEGIN_MESSAGE_MAP(CFixToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CFixToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SELECT_PACK, &CFixToolDlg::OnBnClickedBtnSelectPack)
END_MESSAGE_MAP()


// CFixToolDlg ��Ϣ�������

BOOL CFixToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ʼ����Ʒ�б�
	std::string sConfigFile("productline.xml");
	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile(sConfigFile.c_str());
	TiXmlHandle docHandle(&xmlDoc);

	//��ȡ�ļ���Ϣ
	TiXmlElement* fileNode = docHandle.FirstChild("ProductNames").FirstChild("Service").ToElement();
	for(; NULL != fileNode; fileNode=(TiXmlElement*)fileNode->NextSiblingElement() )
	{
		std::string sType = fileNode->Attribute("ServiceName");
		CString csType(sType.c_str());
		m_comProduct.AddString(csType);
	}
	m_comProduct.SetCurSel(0);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFixToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFixToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFixToolDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString sParm;
	CString sPackFolder;
	int Which = m_sPackPath.ReverseFind('.');
	if (Which!=-1)
	{  
		sPackFolder=m_sPackPath.Left(Which);
	}
	// ��ѹ��������
	sParm.Format(_T("x %s -o%s -aoa"), m_sPackPath, sPackFolder);
	HINSTANCE hret = ShellExecute(NULL,_T("open"),_T("7z.exe"), sParm, NULL, SW_SHOW);
	if (SE_ERR_DLLNOTFOUND > int(hret))
	{
		CString sMsg;
		sMsg.Format(_T("��ѹ���޸���ʧ�ܣ�ִ�з���ֵ��%d��"), hret);
		AfxMessageBox(sMsg, MB_OK|MB_ICONSTOP);
		return;
	}

	// �ȴ���ѹ���ɹ�
	Sleep(2000);
	//��ȡ��������Ŀ¼
	CString sTomcat;
	CString csWebType;
	GetDlgItemText(IDC_CBX_PRODUCT, csWebType);

	if (!GetProcessPath(csWebType, sTomcat))
	{
		CString sMsg;
		sMsg.Format(_T("��ȡWEB����·��ʧ�ܣ�"));
		AfxMessageBox(sMsg, MB_OK|MB_ICONSTOP);
		return;
	}

	CString csWebPath;
	Which = sTomcat.ReverseFind('\\');
	if (Which!=-1)
	{  
		csWebPath=sTomcat.Left(Which+1);
	}
	
	// �����ļ��б�
	CString csXMLFile = sPackFolder + _T("\\updata.xml");
	USES_CONVERSION;  
	std::string sXMLFile(W2A(csXMLFile));  
	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile(sXMLFile.c_str());
	TiXmlHandle docHandle(&xmlDoc);

	//��ȡ�ļ���Ϣ
	TiXmlElement* fileNode = docHandle.FirstChild("updatafiles").FirstChild("node").ToElement();
	if (NULL == fileNode)
	{
		CString sMsg;
		sMsg.Format(_T("����ʧ�ܣ����������ļ�����"));
		AfxMessageBox(sMsg, MB_OK|MB_ICONSTOP);
		return;
	}

	// ֹͣWEB����
	std::string sStop = "net stop " + std::string(W2A(csWebType));
	system(sStop.c_str());
	int nindex = 0;
	for(; NULL != fileNode; fileNode=(TiXmlElement*)fileNode->NextSiblingElement() )
	{
		std::string snewfilename = fileNode->Attribute("newfile");
		CString csnewfilename(snewfilename.c_str());
		std::string srelative = fileNode->Attribute("filepath");
		CString csrelative(srelative.c_str());
		
		// copy new file
		CString csNewPath = csWebPath+csrelative+_T("\\")+csnewfilename;
		if (!csnewfilename.IsEmpty() && !CopyFile(sPackFolder+_T("\\")+csnewfilename, csNewPath, FALSE))
		{
			CString sMsg;
			sMsg.Format(_T("����ʧ�ܣ����ļ�%s����ʧ�ܣ������룺%ld��"), csnewfilename, GetLastError());
			AfxMessageBox(sMsg, MB_OK|MB_ICONSTOP);
			return;
		}

		// del old file
		std::string soldfilename = fileNode->Attribute("oldfile");
		CString csoldfilename(soldfilename.c_str());
		CString csoldfilePath = csWebPath+csrelative+_T("\\")+csoldfilename;
		if (!csoldfilename.IsEmpty() && !DeleteFile(csoldfilePath))
		{
			//CString sMsg;
			//sMsg.Format(_T("ԭ�ļ�%sɾ��ʧ�ܣ������ѱ�ɾ���������룺%ld��"), csoldfilename, GetLastError());
			//AfxMessageBox(sMsg, MB_OK|MB_ICONINFORMATION);
		}
		nindex++;
	}

	CString sMsg;
	sMsg.Format(_T("�����ɹ���������%d���ļ���\r\nע�⣺��������WEB����"), nindex);
	AfxMessageBox(sMsg, MB_OK|MB_ICONINFORMATION);

	// ����WEB����
	std::string sStart = "net start " + std::string(W2A(csWebType));
	system(sStart.c_str());
}

void CFixToolDlg::OnBnClickedBtnSelectPack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	m_sPackPath = dlg.GetPathName();
	m_sPackPath.Replace(_T("\\"),_T("\\\\"));
	UpdateData(FALSE);
}

BOOL CFixToolDlg::GetProcessPath(const CString & ProcessName, CString & ProcessPath)
{
	if (ProcessName.IsEmpty())
	{
		return FALSE;
	}

	// ��ȡ WinSvc handle
	SC_HANDLE schSCManager = NULL;//Windows SC Manager database handle .
	SC_HANDLE schService = NULL; //Windows Service handle .

	// Get a handle to the SCM database. 
	schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) 
	{
		return false;
	}

	// Get a handle to the service
	schService = OpenService( 
		schSCManager,         // SCM database 
		ProcessName,      // name of service 
		SERVICE_ALL_ACCESS);    // full access 

	if (NULL == schService)
	{
		CloseServiceHandle(schSCManager);
		return FALSE;
	}

	// ��ȡ��ǰ�����״̬��
	SERVICE_STATUS_PROCESS ssStatus;
	DWORD dwBytesNeeded= 0;
	DWORD dwProcessId  = 0;
	if (!QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)(&ssStatus),
		sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
	{
		CString sMsg;
		sMsg.Format(_T("��ȡWEB����·��ʧ�ܣ�������Ϣ:%ld��"), GetLastError());
		AfxMessageBox(sMsg, MB_OK|MB_ICONHAND);
		CloseServiceHandle(schSCManager);
		CloseServiceHandle(schService);
		return FALSE;
	}

	dwProcessId = ssStatus.dwProcessId;
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, dwProcessId );

	// Get the process path.
	if (NULL == hProcess )
	{
		CString sMsg;
		sMsg.Format(_T("��ȡWEB����·��ʧ�ܣ�������Ϣ:%ld��"), GetLastError());
		AfxMessageBox(sMsg, MB_OK|MB_ICONHAND);
		CloseServiceHandle(schSCManager);
		CloseServiceHandle(schService);
		return FALSE;
	}

	HMODULE hMod;
	DWORD cbNeeded;
	if ( !EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
		&cbNeeded) )
	{
		CString sMsg;
		sMsg.Format(_T("��ȡWEB����·��ʧ�ܣ�������Ϣ:%ld��"), GetLastError());
		AfxMessageBox(sMsg, MB_OK|MB_ICONHAND);
		CloseServiceHandle(schSCManager);
		CloseServiceHandle(schService);
		CloseHandle(hProcess);
		return FALSE;
	}

	TCHAR szProcessName[MAX_PATH] = TEXT("");
	if(!GetModuleFileNameEx(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR)))
	{
		CString sMsg;
		sMsg.Format(_T("��ȡWEB����·��ʧ�ܣ�������Ϣ:%ld��"), GetLastError());
		AfxMessageBox(sMsg, MB_OK|MB_ICONHAND);
		CloseServiceHandle(schSCManager);
		CloseServiceHandle(schService);
		CloseHandle(hProcess);
		return FALSE;
	}

	CString sProcessName(szProcessName);
	ProcessPath = sProcessName;
	//int idx = sProcessName.ReverseFind('.');
	//if (idx > 0)
	//{
	//	rtn.Truncate(idx);
	//}

	CloseServiceHandle(schSCManager);
	CloseServiceHandle(schService);
	CloseHandle(hProcess);
	return TRUE;
}
