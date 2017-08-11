
// MakeINIToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MakeINITool.h"
#include "MakeINIToolDlg.h"
#include <windows.h>
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


// CMakeINIToolDlg 对话框




CMakeINIToolDlg::CMakeINIToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeINIToolDlg::IDD, pParent)
	, m_strUrl(_T("http://10.10.48.55:80/download/UploadClient/"))
	//, m_strVersion(_T("1.0"))
	, m_strAppName(_T("FSvcUpload"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakeINIToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strUrl);
	//DDX_Text(pDX, IDC_EDIT_VERSION, m_strVersion);
	DDX_Text(pDX, IDC_EDIT_APPNAME, m_strAppName);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_Search);
	DDX_Control(pDX, IDC_EDIT_PATH, m_Url);
	DDX_Control(pDX, IDC_EDIT_APPNAME, m_App);
}

BEGIN_MESSAGE_MAP(CMakeINIToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CMakeINIToolDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_WRITEINI, &CMakeINIToolDlg::OnBnClickedBtnWriteini)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CMakeINIToolDlg::OnNMDblclkList)
	ON_EN_KILLFOCUS(IDC_EDIT, &CMakeINIToolDlg::OnEnKillfocusEdit)
END_MESSAGE_MAP()


// CMakeINIToolDlg 消息处理程序

BOOL CMakeINIToolDlg::OnInitDialog()
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
	InitCtr();
	GetExePath();
	ReadIni();
	CString temp = _T("");
	FindFile(m_LocalPath, temp);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMakeINIToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMakeINIToolDlg::OnPaint()
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
HCURSOR CMakeINIToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMakeINIToolDlg::InitCtr()
{
	m_Edit.ShowWindow(SW_HIDE);
	m_Search.ShowWindow(SW_HIDE);
	m_App.ShowWindow(SW_HIDE);

	LONG lStyle;
	lStyle = GetWindowLong(m_List.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_List.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_List.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_List.SetExtendedStyle(dwStyle); //设置扩展风格


	m_List.InsertColumn(0,_T("文件名称"),LVCFMT_CENTER,100);
	m_List.InsertColumn(1,_T("安装子目录"),LVCFMT_CENTER,100);
	m_List.InsertColumn(2,_T("文件大小(B)"),LVCFMT_CENTER,100);
	m_List.InsertColumn(3,_T("文件路径"),LVCFMT_LEFT,200);
	m_List.InsertColumn(4,_T("本地路径"),LVCFMT_LEFT,200);
	UpdateData(FALSE);
}
void CMakeINIToolDlg::GetExePath()
{
	// 得到exe执行路径.  
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
	*pFind = '\0';  
	m_LocalPath = tcExePath;
}

void CMakeINIToolDlg::ReadIni()
{
	CString szIniPath = m_LocalPath;  
	szIniPath += "\\";  
	szIniPath += CONFIG_FILE; 

	CString strSection;
	CString strKey;
	const int BUFFER_SIZE = 512;
	wchar_t acBuffer[BUFFER_SIZE];


	//读取升级包时间
	strKey = _T("URL");
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(_T("TOOL"), strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
	m_strUrl = acBuffer;
}
BOOL CMakeINIToolDlg::GetFileVersion(LPCTSTR strFile, CString& strVersion,CString &filename,CString &description)    
{    
	TCHAR szVersionBuffer[8192] = _T("");    
	DWORD dwVerSize;    
	DWORD dwHandle;    

	dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);    
	if (dwVerSize == 0)    
		return FALSE;    

	if (GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))    
	{    
		VS_FIXEDFILEINFO * pInfo;    
		unsigned int nInfoLen; 

		if (VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))   
		{   
			//版本号
			strVersion.Format(_T("%d.%d.%d.%d"),    
				HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS),    
				HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));    
		} 
		CString language;
		CString szsubBlock;
		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		}*lpTranslate;

		VerQueryValue(szVersionBuffer, 
			TEXT("\\VarFileInfo\\Translation"),
			(LPVOID *)&lpTranslate,
			&nInfoLen);

		szsubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\OriginalFileName"),
			lpTranslate->wLanguage,lpTranslate->wCodePage);

		if(VerQueryValue(szVersionBuffer, szsubBlock, (LPVOID*)&pInfo, &nInfoLen))
		{
			filename.Format(_T("%s"),pInfo);	
		}      

		szsubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\FileDescription"),
			lpTranslate->wLanguage,lpTranslate->wCodePage);

		if(VerQueryValue(szVersionBuffer, szsubBlock, (LPVOID*)&pInfo, &nInfoLen))
		{
			description.Format(_T("%s"),pInfo);
		}
		return TRUE;   

	}    

	return FALSE;    
}   
BOOL CMakeINIToolDlg::GetFileType(CString strFileName)
{
	char type[20]={0};
	char temp[MAX_PATH] = {0};
	StringChange::CStringToChar(strFileName,temp);
	int i = 0;
	while(temp[i] != '.')i++;
	for(int k = 0;temp[i] != '\0';k++,i++)
	{
		type[k] = temp[i];
	}
	/*if(strcmp(type,".dll") == 0 || strcmp(type,".exe") == 0)
		return TRUE;*/
	if(strcmp(type,".ini") == 0)
		return FALSE;
	return TRUE;

}
void CMakeINIToolDlg::FindFile(CString strPath,CString& strFilelod)
{
	strPath += _T("\\*.*");
	CString m_strSaveFlod = _T("");
	m_strSaveFlod = m_strFlod;
	//文件大小
	ULONGLONG size;
	CFileStatus fileStatus;
	CString temp;
	CString strFile,strVersion,strNumber,strUrl;
	CString strDescription,strFilename;
	CFileFind ff; 
	BOOL res = ff.FindFile(strPath); 
	while (res) 
	{ 
		res = ff.FindNextFile(); 
		if (!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//如果不是dll和exe就退出本次循环
			//if(GetFileType(ff.GetFileName()) == FALSE)
			//	continue;

			//过滤UpdateConfig.ini
			if(0 == ff.GetFileName().CompareNoCase(_T("UpdateConfig.ini")))
				continue;

			strFile = ff.GetFilePath();
			//ff.GetFileName();
			GetFileVersion(strFile,strVersion,strFilename,strDescription);

			strFilename = ff.GetFileName();
			int nRow = m_List.InsertItem(0, strFilename);  //文件名
			//m_List.SetItemText(nRow,1,strVersion);         //

			size = 0;
			if (CFile::GetStatus(ff.GetFilePath(), fileStatus))
			{
				size = fileStatus.m_size;
			}
			temp.Format(_T("%d"),size);
			m_List.SetItemText(nRow,2,temp);                 //文件大小
			
			strUrl = _T("");
			strUrl +=m_strUrl;
			if(strFilelod != _T(""))
			{
				strUrl +=strFilelod;
				strUrl +=_T("/");
			}
			strUrl += strFilename;
			m_List.SetItemText(nRow,1, strFilelod);         //版本号
			m_List.SetItemText(nRow,3,strUrl);               //文件地址

			m_List.SetItemText(nRow,4,strFile); 

			Sleep(100);
			UpdateData(FALSE);
		} 
		else if (ff.IsDirectory() && !ff.IsDots()) 
		{
			CString temp  = ff.GetFileName();
			if (_T("") != m_strFlod)
			{
				m_strFlod += _T("/");
				m_strFlod += temp;
			}
			else
			{
				m_strFlod += temp;
			}
			CString strTemp = ff.GetFilePath();
			FindFile(strTemp, m_strFlod);	

			m_strFlod = m_strSaveFlod;
			//FindFile(strTemp);
		}
	} 
	ff.Close(); 
}
void CMakeINIToolDlg::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	m_List.DeleteAllItems();
	UpdateData(TRUE);
	TCHAR strDirName[MAX_PATH]={0};
	BROWSEINFO bi;
	CString szString = TEXT("选择文件夹");
	bi.hwndOwner = ::GetFocus();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
	if(pItemIDList == NULL)
	{
		return ;
	}
	::SHGetPathFromIDList(pItemIDList, strDirName);
	m_LocalPath.Format(_T("%s"),strDirName);
	
	CString temp = _T("");
	FindFile(m_LocalPath , temp);
}
void CMakeINIToolDlg::OnBnClickedBtnWriteini()
{
	// TODO: 在此添加控件通知处理程序代码

	// 得到exe执行路径.
	CString strOldUrl = m_strUrl;
	UpdateData(TRUE);
	CString temp = _T("");
	if(m_strUrl.CompareNoCase(strOldUrl) == 0)
	{

	}
	else
	{
		m_List.DeleteAllItems();
		FindFile(m_LocalPath, temp);
	}
	//删除已经存在的配置文件
	DeleteFile(_T("UpdateConfig.ini")); 
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	// 设置ini路径到exe同一目录下  
	#ifndef CONFIG_FILE  
	#define CONFIG_FILE     (TEXT("UpdateConfig.ini"))  
	#endif   
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
	if (pFind == NULL)  
	{  
		return;  
	}  
	*pFind = '\0';  

	CString szIniPath = tcExePath;  
	szIniPath += "\\";  
	szIniPath += CONFIG_FILE;  

	::WritePrivateProfileString(TEXT("TOOL"), TEXT("URL"), m_strUrl, szIniPath); 
	::WritePrivateProfileString(TEXT("UPDATE"), TEXT("AppName"), m_strAppName, szIniPath); 
	//::WritePrivateProfileString(TEXT("UPDATE"), TEXT("Version"), m_strVersion, szIniPath);
	::WritePrivateProfileString(TEXT("UPDATE"), TEXT("RunAfterDownload"), m_strAppName+_T(".exe"), szIniPath);

	CString str; //获取系统时间
	CTime tm; 
	tm=CTime::GetCurrentTime(); 
	str.Format(_T("%d%02d%02d%02d%02d%02d"),tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	::WritePrivateProfileString(TEXT("UPDATE"), TEXT("Time"), str, szIniPath);




	//写整体文件信息
	CString tmp;
	tmp.Format(_T("%d"),m_List.GetItemCount());
	::WritePrivateProfileString(TEXT("COMMON"), TEXT("FileCount"), tmp, szIniPath); 
	for(int j = 0;j < m_List.GetItemCount();j++)
	{
		CString num;
		num.Format(_T("%d"),j+1);
		tmp = TEXT("File") + num;
		::WritePrivateProfileString(TEXT("COMMON"), tmp, m_List.GetItemText(j,0),szIniPath); 
	} 
	//写每个文件信息
	for(int i = 0;i < m_List.GetItemCount(); i++)
	{
		CString tmp,strTmp;
		tmp.Format(_T("CommonFile%d"),i+1);

		strTmp = m_List.GetItemText(i,3);
		::WritePrivateProfileString(tmp, TEXT("URL"), strTmp, szIniPath);
		
		strTmp = m_List.GetItemText(i,0);
		::WritePrivateProfileString(tmp, TEXT("Name"), strTmp, szIniPath); 

		strTmp = m_List.GetItemText(i,4);
		unsigned char acMD5Digest[16];
		CalculateMD5(CSCT::Tstring2Ansi(strTmp.GetString()).c_str(),acMD5Digest);
		strTmp = MD5toString(acMD5Digest);
		::WritePrivateProfileString(tmp, TEXT("Hash"), strTmp, szIniPath);
		
		strTmp = m_List.GetItemText(i,1);
		::WritePrivateProfileString(tmp, TEXT("Subcatalog"), strTmp, szIniPath); 

		strTmp = m_List.GetItemText(i,2);
		::WritePrivateProfileString(tmp, TEXT("Size"), strTmp, szIniPath); 
		
	}
}
BOOL CMakeINIToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			//屏蔽Enter键
			if (pMsg->wParam == VK_RETURN)
			{
				return TRUE;
			}
			//屏蔽Esc键
			if (pMsg->wParam == VK_ESCAPE)
			{
				return TRUE;
			}
		}
		break;
	case WM_SYSKEYDOWN:
		{
			//屏蔽Alt + F4
			if (pMsg->wParam == VK_F4)
			{
				return TRUE;
			}
		}
		break;
	default:
		break;
	}

	return CWnd::PreTranslateMessage(pMsg);
}
void CMakeINIToolDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	TCHAR * ach = new TCHAR [MAX_LENGTH];
	CRect cRectItem;
	CRect cRectList;

	NM_LISTVIEW* pNMListView = ( NM_LISTVIEW* )pNMHDR;

	if ( -1 != pNMListView->iItem )
	{
		m_row = pNMListView->iItem; 
		m_column = pNMListView->iSubItem; 

		//过滤key，key不允许修改
		/*if ( 1 != m_column)
		{
			*pResult = 0;
			return;
		}*/
		return;

		m_List.GetSubItemRect( pNMListView->iItem, pNMListView->iSubItem, LVIR_LABEL, cRectItem ); 
		m_List.GetWindowRect( &cRectList );
		ScreenToClient( &cRectList );

		cRectItem.left   += cRectList.left + 3;
		cRectItem.top    += cRectList.top;
		cRectItem.right  += cRectList.left + 3;
		cRectItem.bottom += cRectList.top + 3;

		m_List.GetItemText( pNMListView->iItem, pNMListView->iSubItem, ach, MAX_LENGTH); 
		m_Edit.SetWindowText( ach ); 
		m_Edit.ShowWindow( SW_SHOW ); 
		m_Edit.MoveWindow( &cRectItem ); 
		m_Edit.SetFocus(); 
		m_Edit.CreateSolidCaret( 1, cRectItem.Height() - 5 );
		m_Edit.ShowCaret(); 
		m_Edit.SetSel( -1 ); 

	}

	*pResult = 0;
}

void CMakeINIToolDlg::OnEnKillfocusEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_Edit.GetWindowText(str);//取得编辑框的内容
	m_List.SetItemText(m_row,m_column,str);//将该内容更新到CListCtrl中
	m_Edit.ShowWindow(SW_HIDE);//隐藏编辑框

}