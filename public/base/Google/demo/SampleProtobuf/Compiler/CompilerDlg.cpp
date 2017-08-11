
// CompilerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Compiler.h"
#include "CompilerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCompilerDlg �Ի���




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


// CCompilerDlg ��Ϣ�������

BOOL CCompilerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCompilerDlg::OnPaint()
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

//��ȡѡ���·��
CString GetDirectoryPath(HWND hWnd)
{
	CString szTemp;
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(hWnd,CSIDL_DRIVES,&pidlRoot);

	BROWSEINFO bi;
	CString strDisplayName;
	bi.hwndOwner = hWnd;
	bi.pidlRoot = pidlRoot;
	szTemp = _T("��ѡ����Ҫ�򿪵��ļ���");
	bi.lpszTitle = szTemp.GetBuffer();
	szTemp.ReleaseBuffer();
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH+1); //�õ�������ָ��
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
	//ѡ���ļ�·��
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
		AfxMessageBox("�����������Ϊ��");
		return;
	}

	CString strExePath;//����·��

	TCHAR buff[MAX_PATH];
	GetModuleFileName(NULL,buff,sizeof(buff));
	PathRemoveFileSpec(buff);
	CString strRunPath;
	strRunPath = buff;
	strExePath = strRunPath + _T("/protoc.exe");

	if (GetFileAttributes(strExePath) == (DWORD)INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("δ�ҵ�protoc.exe!");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
