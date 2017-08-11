
// MakeINIToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MakeINITool.h"
#include "MakeINIToolDlg.h"
#include <windows.h>
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


// CMakeINIToolDlg �Ի���




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


// CMakeINIToolDlg ��Ϣ�������

BOOL CMakeINIToolDlg::OnInitDialog()
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
	InitCtr();
	GetExePath();
	ReadIni();
	CString temp = _T("");
	FindFile(m_LocalPath, temp);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMakeINIToolDlg::OnPaint()
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
	lStyle = GetWindowLong(m_List.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_List.m_hWnd, GWL_STYLE, lStyle);//����style
	DWORD dwStyle = m_List.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_List.SetExtendedStyle(dwStyle); //������չ���


	m_List.InsertColumn(0,_T("�ļ�����"),LVCFMT_CENTER,100);
	m_List.InsertColumn(1,_T("��װ��Ŀ¼"),LVCFMT_CENTER,100);
	m_List.InsertColumn(2,_T("�ļ���С(B)"),LVCFMT_CENTER,100);
	m_List.InsertColumn(3,_T("�ļ�·��"),LVCFMT_LEFT,200);
	m_List.InsertColumn(4,_T("����·��"),LVCFMT_LEFT,200);
	UpdateData(FALSE);
}
void CMakeINIToolDlg::GetExePath()
{
	// �õ�exeִ��·��.  
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	//_tcsrchr() ��������������һ��'\\'��λ�ã������ظ�λ�õ�ָ��  
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


	//��ȡ������ʱ��
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
	//�ļ���С
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
			//�������dll��exe���˳�����ѭ��
			//if(GetFileType(ff.GetFileName()) == FALSE)
			//	continue;

			//����UpdateConfig.ini
			if(0 == ff.GetFileName().CompareNoCase(_T("UpdateConfig.ini")))
				continue;

			strFile = ff.GetFilePath();
			//ff.GetFileName();
			GetFileVersion(strFile,strVersion,strFilename,strDescription);

			strFilename = ff.GetFileName();
			int nRow = m_List.InsertItem(0, strFilename);  //�ļ���
			//m_List.SetItemText(nRow,1,strVersion);         //

			size = 0;
			if (CFile::GetStatus(ff.GetFilePath(), fileStatus))
			{
				size = fileStatus.m_size;
			}
			temp.Format(_T("%d"),size);
			m_List.SetItemText(nRow,2,temp);                 //�ļ���С
			
			strUrl = _T("");
			strUrl +=m_strUrl;
			if(strFilelod != _T(""))
			{
				strUrl +=strFilelod;
				strUrl +=_T("/");
			}
			strUrl += strFilename;
			m_List.SetItemText(nRow,1, strFilelod);         //�汾��
			m_List.SetItemText(nRow,3,strUrl);               //�ļ���ַ

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_List.DeleteAllItems();
	UpdateData(TRUE);
	TCHAR strDirName[MAX_PATH]={0};
	BROWSEINFO bi;
	CString szString = TEXT("ѡ���ļ���");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �õ�exeִ��·��.
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
	//ɾ���Ѿ����ڵ������ļ�
	DeleteFile(_T("UpdateConfig.ini")); 
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	// ����ini·����exeͬһĿ¼��  
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

	CString str; //��ȡϵͳʱ��
	CTime tm; 
	tm=CTime::GetCurrentTime(); 
	str.Format(_T("%d%02d%02d%02d%02d%02d"),tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	::WritePrivateProfileString(TEXT("UPDATE"), TEXT("Time"), str, szIniPath);




	//д�����ļ���Ϣ
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
	//дÿ���ļ���Ϣ
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
	// TODO: �ڴ����ר�ô����/����û���
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			//����Enter��
			if (pMsg->wParam == VK_RETURN)
			{
				return TRUE;
			}
			//����Esc��
			if (pMsg->wParam == VK_ESCAPE)
			{
				return TRUE;
			}
		}
		break;
	case WM_SYSKEYDOWN:
		{
			//����Alt + F4
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	TCHAR * ach = new TCHAR [MAX_LENGTH];
	CRect cRectItem;
	CRect cRectList;

	NM_LISTVIEW* pNMListView = ( NM_LISTVIEW* )pNMHDR;

	if ( -1 != pNMListView->iItem )
	{
		m_row = pNMListView->iItem; 
		m_column = pNMListView->iSubItem; 

		//����key��key�������޸�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_Edit.GetWindowText(str);//ȡ�ñ༭�������
	m_List.SetItemText(m_row,m_column,str);//�������ݸ��µ�CListCtrl��
	m_Edit.ShowWindow(SW_HIDE);//���ر༭��

}