
// ResPackerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResPacker.h"
#include "ResPackerDlg.h"
#include "PasswordDlg.h"
#include "../FUiControl/FUi/fuilib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::map<int, COLORREF> g_mapColor;

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


// CResPackerDlg �Ի���

CResPackerDlg::CResPackerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResPackerDlg::IDD, pParent)
	, m_bInit(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    g_mapColor[0] = RGB(0, 0, 0);
    g_mapColor[1] = RGB(255, 0, 0);
}

void CResPackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_RES, m_trRes);
	DDX_Control(pDX, IDC_LIST_LOG, m_lcLog);
}

BEGIN_MESSAGE_MAP(CResPackerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_MESSAGE(WM_OPEN_RC, &CResPackerDlg::OnMsgOpenRc)
	ON_MESSAGE(WM_ADD_RES, &CResPackerDlg::OnMsgAddRes)
	ON_COMMAND(ID_MENU_OPEN, &CResPackerDlg::OnMenuOpen)
	ON_COMMAND(ID_MENU_SAVE_RED, &CResPackerDlg::OnMenuSaveRed)
	ON_COMMAND(ID_MENU_SAVE_BLUE, &CResPackerDlg::OnMenuSaveBlue)
	ON_COMMAND(ID_MENU_PASSWORD, &CResPackerDlg::OnMenuPassword)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_RES, &CResPackerDlg::OnTvnSelchangedTreeRes)
    ON_COMMAND(ID_MENU_NEW, &CResPackerDlg::OnMenuNew)
    ON_WM_CLOSE()
    ON_COMMAND(ID_MENU_EXTRACT, &CResPackerDlg::OnMenuExtract)
END_MESSAGE_MAP()


// CResPackerDlg ��Ϣ�������

BOOL CResPackerDlg::OnInitDialog()
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

	// ����
	SetWindowText(_T("��Դ����������"));

	// �������˵�
	m_menu.LoadMenu(IDR_MENU_MAIN);
	SetMenu(&m_menu);

	// ��ʼ���ؼ�
	InitCtrl();

	m_bInit = TRUE;

	// ���ý����С
	MoveWindow(0,0,1024, 600);

	// ��ȡ����
	m_strPassword = CPasswordDlg::LoadPassword();

	// ��ʾ������־
	AddLog(_T("�������..."));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CResPackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CResPackerDlg::OnPaint()
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
HCURSOR CResPackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CResPackerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	MoveCtrl();
}

// ��ʼ���ؼ�
void CResPackerDlg::InitCtrl(void)
{
	m_imageList.Create(16, 16, ILC_COLOR32, 2, 2);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON0));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));

	m_trRes.SetImageList(&m_imageList, TVSIL_NORMAL);
	m_trRes.SetBkColor(RGB(240, 240, 240));

	// �����б���ʽ
	DWORD dwStyle = m_lcLog.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_FLATSB | LVS_EX_GRIDLINES;
	m_lcLog.SetExtendedStyle(dwStyle);
	m_lcLog.SetBkColor(RGB(240, 240, 240));
	m_lcLog.InsertColumn(0, _T("ʱ��"), LVCFMT_LEFT, 140);
	m_lcLog.InsertColumn(1, _T("��־"), LVCFMT_LEFT, 600);

	// ����Ԥ��ͼƬ�Ĵ���
	m_dlgPicPreview.Create(CPicPreviewDlg::IDD, this);
	m_dlgPicPreview.ShowWindow(SW_SHOW);
}

// �ؼ�λ�úʹ�С
void CResPackerDlg::MoveCtrl()
{
	if (!m_bInit)
	{
		return ;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcTemp(rcClient);
	rcTemp.right = 240;
	m_trRes.MoveWindow(rcTemp);

	rcTemp.CopyRect(rcClient);
	rcTemp.left = 243;
	rcTemp.bottom -= 180;
	m_dlgPicPreview.MoveWindow(rcTemp);

	rcTemp.CopyRect(rcClient);
	rcTemp.top = rcClient.bottom - 177;
	rcTemp.left = 243;
	m_lcLog.MoveWindow(rcTemp);
}


// ��ʾ��־
void CResPackerDlg::AddLog(const CString& strMsg, int nLevel)
{
	CTime tNow = CTime::GetCurrentTime();
	CString strTime = tNow.Format(_T("%Y/%m/%d %H:%M:%S"));

	int nItem = m_lcLog.InsertItem(m_lcLog.GetItemCount(), strTime);
	m_lcLog.SetItemText(nItem, 1, strMsg);
    m_lcLog.SetItemTextColor(nItem, g_mapColor[nLevel]);
	m_lcLog.EnsureVisible(nItem, FALSE);
}

// ������Դ��
void CResPackerDlg::BuildRcTree(const RC_INFO& rcInfo, HTREEITEM hItem)
{
	// ���ڵ�
	int nImage = 0;
	if (hItem == TVI_ROOT)
	{
		nImage = 0;
	}
	else if (rcInfo.nRcType == 0)
	{
		nImage = 1;
	}
	else if (rcInfo.nRcType == 1)
	{
		nImage = 2;
	}
	HTREEITEM hItemParent = m_trRes.InsertItem(rcInfo.strName, nImage, nImage, hItem);
	m_trRes.SetItemData(hItemParent, rcInfo.nRcType);
	
	for (std::list<RC_INFO>::const_iterator citr = rcInfo.subRc.begin();
		citr != rcInfo.subRc.end(); ++citr)
	{
		const RC_INFO& subRcInfo = *citr;
		
		// �ݹ�
		BuildRcTree(subRcInfo, hItemParent);
	}

	// չ��
	m_trRes.Expand(hItemParent, TVE_EXPAND);
}

// ������Դ·�����ṹ����Դ��Ϣ
void CResPackerDlg::ListRes(RC_INFO& rcInfoList)
{
	CFileFind fileFinder;
	CString strWildcard(rcInfoList.strFullPath);
	strWildcard += _T("\\*.*");
	BOOL bWorking = fileFinder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = fileFinder.FindNextFile();
		// skip . and .. files; 
		if (fileFinder.IsDots())
		{
			continue;
		}

		// if it's a directory, recursively search it
		else if (fileFinder.IsDirectory())
		{
			// exclude system folder
			if (!fileFinder.IsSystem())
			{
				RC_INFO rcInfo;
				rcInfo.strName = fileFinder.GetFileName();
				rcInfo.nRcType = TYPE_FOLDER;
				rcInfo.strFullPath = fileFinder.GetFilePath();

				ListRes(rcInfo);

				rcInfoList.subRc.push_back(rcInfo);
	
			}
		}
		else
		{
			if (!fileFinder.IsSystem())
			{
				CString strName = fileFinder.GetFileName();
				CString strExt = strName.Right(strName.GetLength() - strName.ReverseFind(_T('.')));
				if (strExt != _T(".png"))
				{
					AddLog(_T("����δ֧�ֵ���Դ�ļ�: ") + fileFinder.GetFilePath());
				}

				RC_INFO rcInfo;
				rcInfo.strName = fileFinder.GetFileName();
				rcInfo.nRcType = TYPE_IMAGE;
				rcInfo.strFullPath = fileFinder.GetFilePath();
				rcInfoList.subRc.push_back(rcInfo);
			}
		}
	}
	fileFinder.Close();
}

// ������Դ�ļ�
BOOL CResPackerDlg::SaveRc(const int nSkinType /*= 0*/)
{
	m_strZipRootPath = m_strResPath;
	m_strZipRootPath = m_strZipRootPath.Left(m_strZipRootPath.ReverseFind(_T('\\')));
	CString strZipPath;
	strZipPath.Format(_T("%s\\%d.rc"), (LPCTSTR)m_strZipRootPath, nSkinType);

	if (PathFileExists(strZipPath))
	{
		if (IDYES == MessageBox(_T("ָ�����ļ��Ѿ����ڣ��Ƿ񸲸ǣ�"), _T("��ʾ"), MB_YESNO))
		{
			if (0 == DeleteFile(strZipPath))
			{
				CString strMsg;
				strMsg.Format(_T("ɾ���ļ�%sʧ�ܣ����ֶ�ɾ�������ԣ�"), (LPCTSTR)strZipPath);
				MessageBox(strMsg);
				return FALSE;
			}
		}
	}

	// ����Ϊѹ���ļ�
	m_zlib.ClearFile();
	AddRes2Zip(m_zlib, m_rcInfoList);

	std::string strPasswordA = fcT2A(m_strPassword.GetBuffer());
	if (m_zlib.ZipFile(fcT2A(strZipPath.GetBuffer()), strPasswordA.c_str()))
	{
		AddLog(_T("���ɹ���������Դ��: ") + strZipPath);
	}
	else
	{
		AddLog(_T("��ʧ�ܡ�������Դ��: ") + strZipPath);
	}

	CString strRootName;
	strRootName.Format(_T("%d.rc"), nSkinType);
	m_trRes.SetItemText(m_trRes.GetRootItem(), strRootName);
	return TRUE;
}

BOOL CResPackerDlg::AddRes2Zip(zlibHelper& zlib, RC_INFO& rcInfo)
{
	if (rcInfo.strName != DEFAULT_RC_NAME)
	{
		CString strFullPath(rcInfo.strFullPath);

		// ���·������ʼλ��
		int nPos1 = strFullPath.Find(m_strResPath) + m_strResPath.GetLength() + 1;
		CString strHead = strFullPath.Right(strFullPath.GetLength() - nPos1);
		if (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(strFullPath))
		{
			strHead += _T("\\");
			AddLog(_T("����Ŀ¼��Դ: ") + strHead);
		}
		else
		{
			RES_FILE resFile;
			resFile.strFileName = fcT2A(strHead);
			resFile.strFilePath = fcT2A(rcInfo.strFullPath);
			BOOL bRet = zlib.AddFile(resFile);
			if (bRet)
			{
				AddLog(_T("�����Դ�ɹ�: ") + strHead);
			}
			else if (resFile.strFileName.length() > 34)
			{
				AddLog(_T("��Դ·��̫�����ʧ��: ") + strHead, 1);
			}
            else
            {
                AddLog(_T("�����Դʧ��: ") + strHead, 1);
            }
		}
	}

	for (std::list<RC_INFO>::iterator itr = rcInfo.subRc.begin();
		itr != rcInfo.subRc.end(); ++itr)
	{
		RC_INFO& subRcInfo = *itr;
		AddRes2Zip(zlib, subRcInfo);
	}

	return TRUE;
}

// ����Դ�ļ�
LRESULT CResPackerDlg::OnMsgOpenRc(WPARAM, LPARAM)
{
    if (1)
    {
        /*�Ƚ�ѹ�������ش���*/

        // zip �ļ����ڵ�·��
        CString strPath = m_strResPath.Left(m_strResPath.ReverseFind(_T('\\')) + 1);

        // ���һ��18λ���ַ�����Ϊ��ʱĿ¼
        const int nLen = 18;
        char szTemp[nLen] = {0};
        GenRandomString(szTemp, nLen);
        CString strTemp = fcA2T(szTemp);
        strPath = strPath + strTemp;
        // ������ʱĿ¼
        SuperMkDirs(strPath.GetBuffer());

        m_vecTempResPath.push_back(strPath);

        AddLog(_T("��ѹ��Դ����ʱĿ¼: ") + strPath);

        if (m_zlib.UnzipFile(m_strResPath, strPath, fcT2A(m_strPassword.GetBuffer())))
        {
            m_strResPath = strPath;
            PostMessage(WM_ADD_RES, 0, 0);
        }
    }
    else
    {
        AddLog(_T("����Դ�ļ�."));
        // rc �ļ� �����ļ��ṹ��������������Դ��
        BOOL bRet = m_zlib.OpenRc(m_strResPath);
        if (!bRet)
        {
            AddLog(_T("���ļ�ʧ��."));
            return -1;
        }
        AddLog(_T("���ļ��ɹ�."));

        m_rcInfoList.clear();
        bRet = m_zlib.GetRcInfoList(m_rcInfoList);
        if (!bRet)
        {
            AddLog(_T("������Դ�ļ�ʧ��."));
            return -2;
        }
        AddLog(_T("������Դ�ļ��ɹ�."));

        m_trRes.DeleteAllItems();
        BuildRcTree(m_rcInfoList);
        AddLog(_T("������Դ��."));
    }

	return 0;
}

// �����Դ�ļ���
LRESULT CResPackerDlg::OnMsgAddRes(WPARAM, LPARAM)
{
	m_rcInfoList.clear();

	m_rcInfoList.strName = DEFAULT_RC_NAME;
	m_rcInfoList.nRcType = TYPE_FOLDER;
	m_rcInfoList.strFullPath = m_strResPath;
	AddLog(_T("���ļ��гɹ�."));

	ListRes(m_rcInfoList);
	AddLog(_T("�ṹ����Դ�ļ�."));

	m_trRes.DeleteAllItems();
	BuildRcTree(m_rcInfoList);
	AddLog(_T("������Դ��."));

	return 0;
}

// �����Ϸŵ���Ϣ
void CResPackerDlg::OnDropFiles(HDROP hDropInfo)
{
	int nDropCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//ȡ�ñ��϶��ļ�����Ŀ 
	if (nDropCount != 1)
	{
		return ;
	}
	// ��ȡ·��
	TCHAR szPath[MAX_PATH] = {0};
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);
	DragFinish(hDropInfo); //�ϷŽ�����,�ͷ��ڴ�

	m_strResPath = szPath;
	if (FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes(szPath))
	{
		// ����Դ�ļ�
		CString strPath(szPath);
		CString strExt = strPath.Right(strPath.GetLength() - strPath.ReverseFind(_T('.')));
		if (strExt == _T(".rc"))
		{
			PostMessage(WM_OPEN_RC, 0, 0);
		}
	}
	else
	{
		// �����ԴĿ¼
		PostMessage(WM_ADD_RES, 0, 0);
	}

	CDialog::OnDropFiles(hDropInfo);
}


// ����򿪲˵�
void CResPackerDlg::OnMenuOpen()
{
	CString strFilter = _T("��Դ�ļ� (*.rc)|*.rc||");   //�ļ����ǵ�����  
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, strFilter);
	if (IDOK == dlgFile.DoModal())
	{
		m_strResPath = dlgFile.GetPathName();
		PostMessage(WM_OPEN_RC, 0, 0);
	}
}
// �½�һ����Դ
void CResPackerDlg::OnMenuNew()
{
    BROWSEINFO bi;
    ZeroMemory(&bi,sizeof(BROWSEINFO));
    LPMALLOC pMalloc;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    TCHAR szResPath[MAX_PATH] = {0};
    if(pidl != NULL)
    {
        SHGetPathFromIDList(pidl, szResPath);
        if(SUCCEEDED(SHGetMalloc(&pMalloc)))    //pidlָ��Ķ�������Ӧ���ͷţ�֮ǰ������
        {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
    }

    if (PathFileExists(szResPath))
    {
        m_strResPath = szResPath;
        PostMessage(WM_ADD_RES, 0, 0);
    }
}

// �������˵�
void CResPackerDlg::OnMenuSaveRed()
{
	SaveRc(0);
}

// �������˵�
void CResPackerDlg::OnMenuSaveBlue()
{
	SaveRc(1);
}

void CResPackerDlg::OnMenuExtract()
{
    // m_strResPath ����
    int nPos = m_strResPath.ReverseFind(_T('\\'));
    CString strNewPath = m_strResPath.Left(nPos);
    strNewPath += _T("\\Res");
    myCopyDirectory(m_strResPath, strNewPath);
    AddLog(_T("��ѹ��Դ��:") + strNewPath);
}

// �����������˵�
void CResPackerDlg::OnMenuPassword()
{
	CPasswordDlg dlg;
	dlg.DoModal();
	m_strPassword = CPasswordDlg::LoadPassword();
	AddLog(_T("��������:") + m_strPassword);
}

// �����Դ���ڵ�
void CResPackerDlg::OnTvnSelchangedTreeRes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	if (hItem == NULL)
	{
		return ;
	}

	int nRcType = m_trRes.GetItemData(hItem);
	if (nRcType != TYPE_IMAGE)
	{
		m_dlgPicPreview.m_pImage = NULL;
		m_dlgPicPreview.Invalidate();
		return ;
	}

	// ��Դ������
	CString strName = m_trRes.GetItemText(hItem);

	// ��ȡ��Դ��zip�е�����������
	HTREEITEM hRoot = m_trRes.GetRootItem();
	HTREEITEM hParentItem = m_trRes.GetParentItem(hItem);
	while (hParentItem != hRoot)
	{
		CString strParentText = m_trRes.GetItemText(hParentItem);
		strName = strParentText + _T("/") + strName;
		hParentItem = m_trRes.GetParentItem(hParentItem);
	}

	Image* pImage = NULL;
	if (FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes(m_strResPath))
	{
		// zip���ж�ȡ��Դ����
		char* pFileBuffer = NULL;
		int nFileLen = 0;
		if (!m_zlib.GetFile(strName, m_strPassword, &pFileBuffer, nFileLen))
		{
			AddLog(_T("��ȡ�ļ�����."));
			return ;
		}

		pImage = mi_from_memory(pFileBuffer, nFileLen);
	}
	else
	{
		CString strImagePath = m_strResPath + _T("\\") + strName;
		strImagePath.Replace(_T('/'), _T('\\'));
		pImage = Image::FromFile(strImagePath);
	}
	// ��ʾ�ڽ�����
	m_dlgPicPreview.m_pImage = pImage;
	m_dlgPicPreview.Invalidate();

	*pResult = 0;
}

void CResPackerDlg::OnClose()
{
    for (size_t t = 0; t < m_vecTempResPath.size(); ++t)
    {
        myDeleteDirectory(m_vecTempResPath[t]);
    }

    CDialog::OnClose();
}
