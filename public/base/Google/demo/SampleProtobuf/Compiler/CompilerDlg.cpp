
// CompilerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Compiler.h"
#include "CompilerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCompilerDlg 对话框




CCompilerDlg::CCompilerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompilerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCompilerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editSelPath);
	DDX_Control(pDX, IDC_EDIT_PROTO_FILE, m_editProtoFile);
}

BEGIN_MESSAGE_MAP(CCompilerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SELECT_PROTO_FILE, &CCompilerDlg::OnBnClickedBtnSelectProtoFile)
	ON_BN_CLICKED(IDC_BTN_SELECT_BUILD_PATH, &CCompilerDlg::OnBnClickedBtnSelectBuildPath)
	ON_BN_CLICKED(IDC_BTN_COMPILER, &CCompilerDlg::OnBnClickedBtnCompiler)
	ON_BN_CLICKED(IDC_BUTTON3, &CCompilerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CCompilerDlg 消息处理程序

BOOL CCompilerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCompilerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCompilerDlg::OnPaint()
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
HCURSOR CCompilerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCompilerDlg::OnBnClickedBtnSelectProtoFile()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	TCHAR szFilters[]= _T("MyType Files (*.proto)|*.proto|All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, _T("proto"), _T("*.proto"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if(fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName();
		CString strFileName = fileDlg.GetFileName();
	
		m_strProtoFile = pathName;
		m_strSearchPath = pathName.Left(pathName.GetLength()-strFileName.GetLength());

		m_editProtoFile.SetWindowText(m_strProtoFile);
	}
}

//获取选择的路径
CString GetDirectoryPath(HWND hWnd)
{
	CString szTemp;
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(hWnd,CSIDL_DRIVES,&pidlRoot);

	BROWSEINFO bi;
	CString strDisplayName;
	bi.hwndOwner = hWnd;
	bi.pidlRoot = pidlRoot;
	szTemp = _T("请选择需要打开的文件夹");
	bi.lpszTitle = szTemp.GetBuffer();
	szTemp.ReleaseBuffer();
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH+1); //得到缓冲区指针
	LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);
	strDisplayName.ReleaseBuffer(MAX_PATH+1);	

	char pPath[MAX_PATH];
	CString Str;
	if(lpIDList)
	{
		SHGetPathFromIDList(lpIDList,pPath);
		Str = pPath;
	}
	return Str;
}

void CCompilerDlg::OnBnClickedBtnSelectBuildPath()
{
	//选择文件路径
	CString strPath = GetDirectoryPath(this->GetSafeHwnd());
	if (strPath.IsEmpty())
	{
		return;
	}
	m_strOutputPath = strPath;
	m_editSelPath.SetWindowText(m_strOutputPath);
}

void CCompilerDlg::OnBnClickedBtnCompiler()
{
	if (m_strOutputPath.IsEmpty() 
		|| m_strProtoFile.IsEmpty())
	{
		AfxMessageBox("输入参数不能为空");
		return;
	}

	CString strExePath;//程序路径

	TCHAR buff[MAX_PATH];
	GetModuleFileName(NULL,buff,sizeof(buff));
	PathRemoveFileSpec(buff);
	CString strRunPath;
	strRunPath = buff;
	strExePath = strRunPath + _T("/protoc.exe");

	if (GetFileAttributes(strExePath) == (DWORD)INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("未找到protoc.exe!");
		return;
	}

	CString strMsg;
	strMsg.Format("@path %s",strExePath);
	OutputDebugString(strMsg);

	CString strParam;
	strParam.Format("-I=%s --cpp_out=%s %s", m_strSearchPath, m_strOutputPath,m_strProtoFile);
	ShellExecute(NULL, "open", strExePath, strParam, NULL, SW_SHOWNORMAL);
}

void CCompilerDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}
