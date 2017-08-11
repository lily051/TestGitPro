
// UpdateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Update.h"
#include "UpdateDlg.h"
#include "md5.h"
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


// CUpdateDlg �Ի���




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


// CUpdateDlg ��Ϣ�������

BOOL CUpdateDlg::OnInitDialog()
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

	InitCtr();
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpdateDlg::OnPaint()
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
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH; //˵���˽ṹ����Щ��Ա����Ч��
	lvColumn.fmt = LVCFMT_CENTER;                      //�еĶ��뷽ʽ
	lvColumn.cx =rect.right-20;                                  //�еĳ���
	lvColumn.pszText = TEXT("�ļ�����");                  //�еı���
	nCol = m_StoreItems.InsertColumn(0, &lvColumn);    //�����,����Ϊ0
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
	//ѡ���ļ���
	BROWSEINFO bi;
	CString szString = TEXT("ѡ��һ��Դ�ļ����ļ���");
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
	//��ȡ�ֽڳ���    
	iLength = WideCharToMultiByte(CP_ACP, 0, strDirName, -1, NULL, 0, NULL, NULL);   
	//��tcharֵ����_char     
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ɾ�������ļ�
	DeleteConfig();
	//ѡ�е��ļ������
	PushList();
	//�ļ�����Ϣ,����,�ļ���
	GetAllFileInfo();
    //дCommon
	WriteCommon();
	//��ѡ�е��ļ������ļ�������ıȽ�,������,�Ͱ��ļ�����Ϣд��ini
	FindFiles();
	
}

void CUpdateDlg::PushList(void)
{
	for(int i = 0;i < m_StoreItems.GetItemCount();i++)
	{
		//��ѡ�еĲ��Ҵ�С��Ϊ��
		if(m_StoreItems.GetCheck(i))
		{
			//����
			CString filename = m_StoreItems.GetItemText(i,0);
			theApp.download_list.push(filename);
		}
	}
}


void CUpdateDlg::WriteConfig(int i,fileStruct &fileInfo)
{ 
	CString szIniPath = SetConfig(NULL,_T("UpdateConfig.ini"));
	//�ļ���Ϣ
	CString filenumber,filesize;
	filenumber.Format(_T("%d"),i);
	filesize.Format(_T("%d"),fileInfo.filesize);
	CString CommonFile = L"CommonFile" + filenumber;

	// �����ļ���Hash
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
    // �õ�exeִ��·��.  
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	// ����ini·����exeͬһĿ¼��  
	#ifndef CONFIG_FILE  
	#define CONFIG_FILE     (TEXT("UpdateConfig.ini"))  
	#endif  
  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
	if (pFind == NULL)  
	{  

		AfxMessageBox(L"�Ҳ��������ļ�,�����������ݿ�!");
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

	//����Ҫ���ص��ļ�
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
				//MessageBox(L"���");
				fileStruct fileInfo;
				
				//�ļ�·��
				CString tmp = directpath;
				GetDlgItemText(IDC_EDIT_SAVE__PATH,tmp);
				if (tmp == L"")
				{
					AfxMessageBox(L"��д�ļ�·��");
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
					AfxMessageBox(L"��д�ļ�·��");
					return;
				}
				
				CFileStatus fileStatus;
				//�ļ���
				fileInfo.Name = filename;
				//�ļ���С
				ULONGLONG size;
				CString temp = directpath + "\\" +filename;
				if (CFile::GetStatus(temp, fileStatus))
				{
					size = fileStatus.m_size;
					fileInfo.filesize = size;
				}
				//ѹ����ʽ
				fileInfo.CompressMethod = TEXT("None");
				//��ϣ��
				fileInfo.Hash = L"";
				//����·��
				GetDlgItemText(IDC_EDIT_DOWNLOAD_PATH,fileInfo.Subcatalog);
				WriteConfig(i+1,fileInfo);
				i++;
				//������
				theApp.download_list.pop();
				//���ļ����ٴα��������ļ�
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
	//�����ļ��������ļ���
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
			//�汾��
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//
//	//char filepath[MAX_PATH]={"Hello.txt"};
//	char myurl[100] = {"http://127.0.0.1/download"};
//	//char myurl[100] = "http://127.0.0.1/download/AutoUpdate";
//	//char myurl[100] = "http://www.baidu.com/";
//	CURL *curl = curl_easy_init();  
//
//	//���ý������ݵĻص� 
//	FILE* file = fopen("D:\\Hello.txt", "wb");
//	if (file == NULL)
//	{
//		AfxMessageBox(L"���ļ�ʧ��");
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
	// ����ini·����exeͬһĿ¼��  
	#ifndef CONFIG_FILE  
	#define CONFIG_FILE     (TEXT("UpdateConfig.ini"))  
	#endif  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
	if (pFind == NULL)  
	{  

		AfxMessageBox(L"�Ҳ���");
		return L"";  
	}  
	*pFind = '\0';  
	CString szIniPath = tcExePath;  
	//szIniPath += "\\";  
	return CString(szIniPath);
}
