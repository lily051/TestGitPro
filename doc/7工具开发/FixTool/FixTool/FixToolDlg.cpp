
// FixToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FixTool.h"
#include "FixToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFixToolDlg 对话框




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


// CFixToolDlg 消息处理程序

BOOL CFixToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化产品列表
	std::string sConfigFile("productline.xml");
	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile(sConfigFile.c_str());
	TiXmlHandle docHandle(&xmlDoc);

	//读取文件信息
	TiXmlElement* fileNode = docHandle.FirstChild("ProductNames").FirstChild("Service").ToElement();
	for(; NULL != fileNode; fileNode=(TiXmlElement*)fileNode->NextSiblingElement() )
	{
		std::string sType = fileNode->Attribute("ServiceName");
		CString csType(sType.c_str());
		m_comProduct.AddString(csType);
	}
	m_comProduct.SetCurSel(0);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFixToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFixToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFixToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString sParm;
	CString sPackFolder;
	int Which = m_sPackPath.ReverseFind('.');
	if (Which!=-1)
	{  
		sPackFolder=m_sPackPath.Left(Which);
	}
	// 解压缩的命令
	sParm.Format(_T("x %s -o%s -aoa"), m_sPackPath, sPackFolder);
	HINSTANCE hret = ShellExecute(NULL,_T("open"),_T("7z.exe"), sParm, NULL, SW_SHOW);
	if (SE_ERR_DLLNOTFOUND > int(hret))
	{
		CString sMsg;
		sMsg.Format(_T("解压缩修复包失败！执行返回值：%d！"), hret);
		AfxMessageBox(sMsg, MB_OK|MB_ICONSTOP);
		return;
	}

	// 等待解压缩成功
	Sleep(2000);
	//获取服务所在目录
	CString sTomcat;
	CString csWebType;
	GetDlgItemText(IDC_CBX_PRODUCT, csWebType);

	if (!GetProcessPath(csWebType, sTomcat))
	{
		CString sMsg;
		sMsg.Format(_T("获取WEB服务路径失败！"));
		AfxMessageBox(sMsg, MB_OK|MB_ICONSTOP);
		return;
	}

	CString csWebPath;
	Which = sTomcat.ReverseFind('\\');
	if (Which!=-1)
	{  
		csWebPath=sTomcat.Left(Which+1);
	}
	
	// 更新文件列表
	CString csXMLFile = sPackFolder + _T("\\updata.xml");
	USES_CONVERSION;  
	std::string sXMLFile(W2A(csXMLFile));  
	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile(sXMLFile.c_str());
	TiXmlHandle docHandle(&xmlDoc);

	//读取文件信息
	TiXmlElement* fileNode = docHandle.FirstChild("updatafiles").FirstChild("node").ToElement();
	if (NULL == fileNode)
	{
		CString sMsg;
		sMsg.Format(_T("升级失败，解析配置文件出错！"));
		AfxMessageBox(sMsg, MB_OK|MB_ICONSTOP);
		return;
	}

	// 停止WEB服务
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
			sMsg.Format(_T("升级失败！新文件%s拷贝失败，错误码：%ld！"), csnewfilename, GetLastError());
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
			//sMsg.Format(_T("原文件%s删除失败，可能已被删除，错误码：%ld！"), csoldfilename, GetLastError());
			//AfxMessageBox(sMsg, MB_OK|MB_ICONINFORMATION);
		}
		nindex++;
	}

	CString sMsg;
	sMsg.Format(_T("升级成功，共处理%d个文件！\r\n注意：即将重启WEB服务！"), nindex);
	AfxMessageBox(sMsg, MB_OK|MB_ICONINFORMATION);

	// 开启WEB服务
	std::string sStart = "net start " + std::string(W2A(csWebType));
	system(sStart.c_str());
}

void CFixToolDlg::OnBnClickedBtnSelectPack()
{
	// TODO: 在此添加控件通知处理程序代码
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

	// 获取 WinSvc handle
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

	// 获取当前服务的状态。
	SERVICE_STATUS_PROCESS ssStatus;
	DWORD dwBytesNeeded= 0;
	DWORD dwProcessId  = 0;
	if (!QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)(&ssStatus),
		sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
	{
		CString sMsg;
		sMsg.Format(_T("获取WEB服务路径失败，错误信息:%ld！"), GetLastError());
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
		sMsg.Format(_T("获取WEB服务路径失败，错误信息:%ld！"), GetLastError());
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
		sMsg.Format(_T("获取WEB服务路径失败，错误信息:%ld！"), GetLastError());
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
		sMsg.Format(_T("获取WEB服务路径失败，错误信息:%ld！"), GetLastError());
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
