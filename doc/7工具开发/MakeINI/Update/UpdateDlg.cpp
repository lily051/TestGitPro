
// UpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "UpdateDlg.h"
#include "md5.h"
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


// CUpdateDlg 对话框




CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_StoreItems);
}

BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CUpdateDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, &CUpdateDlg::OnBnClickedBtnDownload)
	//ON_BN_CLICKED(IDC_BUTTON_CON, &CUpdateDlg::OnBnClickedButtonCon)
END_MESSAGE_MAP()


// CUpdateDlg 消息处理程序

BOOL CUpdateDlg::OnInitDialog()
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

	InitCtr();
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUpdateDlg::OnPaint()
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
HCURSOR CUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUpdateDlg::InitCtr(void)
{
	CRect rect;
	m_StoreItems.GetWindowRect(rect);
	m_StoreItems.DeleteAllItems();

	LVCOLUMN lvColumn;
	int nCol;
	m_StoreItems.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH; //说明此结构中哪些成员是有效的
	lvColumn.fmt = LVCFMT_CENTER;                      //列的对齐方式
	lvColumn.cx =rect.right-20;                                  //列的长度
	lvColumn.pszText = TEXT("文件名称");                  //列的标题
	nCol = m_StoreItems.InsertColumn(0, &lvColumn);    //添加列,索引为0
	m_StoreItems.SortItems(0,0);
}
size_t DownloadCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)  
{  
	FILE* fp = (FILE*)pParam;  
	size_t nWrite = fwrite(pBuffer, nSize, nMemByte, fp);  

	return nWrite;  
}  
void CUpdateDlg::OnBnClickedBtnSearch()
{
	m_StoreItems.DeleteAllItems();
	TCHAR strDirName[MAX_PATH]={0};
	char DirName[MAX_PATH]={0};
	//CString szIniPath;
	//选择文件夹
	BROWSEINFO bi;
	CString szString = TEXT("选择一个源文件子文件夹");
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
	int iLength ;   
	//获取字节长度    
	iLength = WideCharToMultiByte(CP_ACP, 0, strDirName, -1, NULL, 0, NULL, NULL);   
	//将tchar值赋给_char     
	WideCharToMultiByte(CP_ACP, 0, strDirName, -1, DirName, iLength, NULL, NULL); 

	CString szIniPath=_T("");
	Char2CString(DirName,szIniPath);
	
	theApp.szIniPath = szIniPath;
	szIniPath += "\\";  
	szIniPath += "*.*"; 


	CFileFind ff; 
	BOOL res = ff.FindFile(szIniPath); 
	while (res) 
	{ 
		res = ff.FindNextFile(); 
		if (!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			LVITEM lvItem;
			int nItem;
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = 0;
			lvItem.iSubItem = 0;
			wchar_t szBuffer[100] = {0};
			wcscpy(szBuffer,ff.GetFileName());
			LPWSTR lpwStr = szBuffer;
			lvItem.pszText = lpwStr;
			nItem = m_StoreItems.InsertItem(&lvItem);
		} 
	} 
	ff.Close(); 

}

void CUpdateDlg::OnBnClickedBtnDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	//删除配置文件
	DeleteConfig();
	//选中的文件入队列
	PushList();
	//文件总信息,个数,文件名
	GetAllFileInfo();
    //写Common
	WriteCommon();
	//把选中的文件名和文件夹里面的比较,如果相等,就把文件的信息写入ini
	FindFiles();
	
}

void CUpdateDlg::PushList(void)
{
	for(int i = 0;i < m_StoreItems.GetItemCount();i++)
	{
		//被选中的并且大小不为空
		if(m_StoreItems.GetCheck(i))
		{
			//队列
			CString filename = m_StoreItems.GetItemText(i,0);
			theApp.download_list.push(filename);
		}
	}
}


void CUpdateDlg::WriteConfig(int i,fileStruct &fileInfo)
{ 
	CString szIniPath = SetConfig(NULL,_T("UpdateConfig.ini"));
	//文件信息
	CString filenumber,filesize;
	filenumber.Format(_T("%d"),i);
	filesize.Format(_T("%d"),fileInfo.filesize);
	CString CommonFile = L"CommonFile" + filenumber;

	// 计算文件的Hash
	CString sHash;
	unsigned char acMD5Digest[16];
	CString url =theApp.szIniPath;
	
	if(url.Right(1) != L"/"){
		url = url+ "/" + fileInfo.Name;
	}
	else
	{
		url = url+ fileInfo.Name;
	}
	CalculateMD5(CSCT::Tstring2Ansi(url.GetString()).c_str(),acMD5Digest);
	CString strMD5 = MD5toString(acMD5Digest);
	fileInfo.Hash = strMD5;

	::WritePrivateProfileString(CommonFile, TEXT("Name"), fileInfo.Name, szIniPath);  
	::WritePrivateProfileString(CommonFile, TEXT("Size"), filesize, szIniPath);  
	::WritePrivateProfileString(CommonFile, TEXT("URL"), fileInfo.URL, szIniPath);  
	::WritePrivateProfileString(CommonFile, TEXT("CompressMethod"), TEXT("None"), szIniPath);  
	::WritePrivateProfileString(CommonFile, TEXT("Hash"), fileInfo.Hash, szIniPath);  
	::WritePrivateProfileString(CommonFile, TEXT("Subcatalog"), fileInfo.Subcatalog, szIniPath);  
}
void CUpdateDlg::DeleteConfig()
{
    // 得到exe执行路径.  
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	// 设置ini路径到exe同一目录下  
	#ifndef CONFIG_FILE  
	#define CONFIG_FILE     (TEXT("UpdateConfig.ini"))  
	#endif  
  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
	if (pFind == NULL)  
	{  

		AfxMessageBox(L"找不到配置文件,请先配置数据库!");
		return ;  
	}  
	*pFind = '\0';  

	CString szIniPath = tcExePath;  
	szIniPath += "\\";  
	szIniPath += CONFIG_FILE; 
	DeleteFile(szIniPath);  
}

void CUpdateDlg::FindFiles(void)
{
	CString download_path,directpath;
	download_path =theApp.szIniPath;
	directpath = download_path;
	download_path += _T("\\"); 
	download_path += _T("*.*"); 

	//查找要下载的文件
	int i = 0;
	CFileFind ff; 
	BOOL res = ff.FindFile(download_path); 
	while (res && theApp.download_list.size() != 0) 
	{ 
		res = ff.FindNextFile(); 
		if (!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			CString filename = theApp.download_list.front();
			if(ff.GetFileName() == filename)
			{
				//MessageBox(L"相等");
				fileStruct fileInfo;
				
				//文件路径
				CString tmp = directpath;
				GetDlgItemText(IDC_EDIT_SAVE__PATH,tmp);
				if (tmp == L"")
				{
					AfxMessageBox(L"填写文件路径");
					return;
				}
				CString strFilePath = L"";
				if(tmp.Right(1) != L"/"){
				     strFilePath = tmp + "/" + filename;
				}
				else
				{
					  strFilePath = tmp + filename;
				}
				fileInfo.URL = strFilePath;
				if (fileInfo.URL == L"")
				{
					AfxMessageBox(L"填写文件路径");
					return;
				}
				
				CFileStatus fileStatus;
				//文件名
				fileInfo.Name = filename;
				//文件大小
				ULONGLONG size;
				CString temp = directpath + "\\" +filename;
				if (CFile::GetStatus(temp, fileStatus))
				{
					size = fileStatus.m_size;
					fileInfo.filesize = size;
				}
				//压缩方式
				fileInfo.CompressMethod = TEXT("None");
				//哈希码
				fileInfo.Hash = L"";
				//保存路径
				GetDlgItemText(IDC_EDIT_DOWNLOAD_PATH,fileInfo.Subcatalog);
				WriteConfig(i+1,fileInfo);
				i++;
				//出队列
				theApp.download_list.pop();
				//从文件夹再次遍历查找文件
				res = ff.FindFile(directpath + "\\" +"*.*");
			}
		} 
	}
	ff.Close(); 
}

void CUpdateDlg::GetAllFileInfo(void)
{
	download_list = theApp.download_list;
	theApp.file_number = theApp.download_list.size();
}

void CUpdateDlg::WriteCommon(void)
{
	CString szIniPath = SetConfig(NULL,_T("UpdateConfig.ini"));
	CString strVersion,filename,description,CommonFile(_T("UPDATE"));
	GetDlgItemText(IDC_EDIT_NAME,filename);
	GetDlgItemText(IDC_EDIT_NUMBER,strVersion);
	::WritePrivateProfileString(CommonFile, TEXT("AppName"), filename, szIniPath); 
	::WritePrivateProfileString(CommonFile, TEXT("Version"), strVersion, szIniPath);
	::WritePrivateProfileString(CommonFile, TEXT("RunAfterDownload"), filename, szIniPath);
	//更新文件个数和文件名
	CString str = TEXT("COMMON");
	CString tmp;
	tmp.Format(_T("%d"),theApp.file_number);
	::WritePrivateProfileString(str, TEXT("FileCount"), tmp, szIniPath); 
	for(int j = 0;j < theApp.file_number;j++)
	{
		CString num;
		num.Format(_T("%d"),j+1);
		tmp = TEXT("File") + num;

		::WritePrivateProfileString(str, tmp, download_list.front(), szIniPath); 
		download_list.pop();
	}
}

BOOL CUpdateDlg::GetFileVersion(LPCTSTR strFile, CString& strVersion,CString &filename,CString &description)    
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


//void CUpdateDlg::OnBnClickedButtonCon()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	//char filepath[MAX_PATH]={"Hello.txt"};
//	char myurl[100] = {"http://127.0.0.1/download"};
//	//char myurl[100] = "http://127.0.0.1/download/AutoUpdate";
//	//char myurl[100] = "http://www.baidu.com/";
//	CURL *curl = curl_easy_init();  
//
//	//设置接收数据的回调 
//	FILE* file = fopen("D:\\Hello.txt", "wb");
//	if (file == NULL)
//	{
//		AfxMessageBox(L"打开文件失败");
//		return;
//	}
//	curl_easy_setopt(curl, CURLOPT_URL, myurl);
//	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
//	curl_easy_setopt(curl, CURLOPT_FTPLISTONLY,1);
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownloadCallback);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA,file); 
//
//	CURLcode retcCode = curl_easy_perform(curl);
//	const char* pError = curl_easy_strerror(retcCode);
//	CString cserror;
//	cserror.Format(_T("%s"),pError);
//	fclose(file);
//	curl_easy_cleanup(curl);
//}

CString CUpdateDlg::GeTPath(void)
{
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	// 设置ini路径到exe同一目录下  
	#ifndef CONFIG_FILE  
	#define CONFIG_FILE     (TEXT("UpdateConfig.ini"))  
	#endif  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
	if (pFind == NULL)  
	{  

		AfxMessageBox(L"找不到");
		return L"";  
	}  
	*pFind = '\0';  
	CString szIniPath = tcExePath;  
	//szIniPath += "\\";  
	return CString(szIniPath);
}
