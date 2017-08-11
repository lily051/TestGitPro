
// ResPackerDlg.cpp : 实现文件
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


// CResPackerDlg 对话框

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


// CResPackerDlg 消息处理程序

BOOL CResPackerDlg::OnInitDialog()
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

	// 设置
	SetWindowText(_T("资源打包解包工具"));

	// 设置主菜单
	m_menu.LoadMenu(IDR_MENU_MAIN);
	SetMenu(&m_menu);

	// 初始化控件
	InitCtrl();

	m_bInit = TRUE;

	// 设置界面大小
	MoveWindow(0,0,1024, 600);

	// 读取密码
	m_strPassword = CPasswordDlg::LoadPassword();

	// 显示启动日志
	AddLog(_T("启动完成..."));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CResPackerDlg::OnPaint()
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
HCURSOR CResPackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CResPackerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	MoveCtrl();
}

// 初始化控件
void CResPackerDlg::InitCtrl(void)
{
	m_imageList.Create(16, 16, ILC_COLOR32, 2, 2);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON0));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));

	m_trRes.SetImageList(&m_imageList, TVSIL_NORMAL);
	m_trRes.SetBkColor(RGB(240, 240, 240));

	// 设置列表样式
	DWORD dwStyle = m_lcLog.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_FLATSB | LVS_EX_GRIDLINES;
	m_lcLog.SetExtendedStyle(dwStyle);
	m_lcLog.SetBkColor(RGB(240, 240, 240));
	m_lcLog.InsertColumn(0, _T("时间"), LVCFMT_LEFT, 140);
	m_lcLog.InsertColumn(1, _T("日志"), LVCFMT_LEFT, 600);

	// 创建预览图片的窗口
	m_dlgPicPreview.Create(CPicPreviewDlg::IDD, this);
	m_dlgPicPreview.ShowWindow(SW_SHOW);
}

// 控件位置和大小
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


// 显示日志
void CResPackerDlg::AddLog(const CString& strMsg, int nLevel)
{
	CTime tNow = CTime::GetCurrentTime();
	CString strTime = tNow.Format(_T("%Y/%m/%d %H:%M:%S"));

	int nItem = m_lcLog.InsertItem(m_lcLog.GetItemCount(), strTime);
	m_lcLog.SetItemText(nItem, 1, strMsg);
    m_lcLog.SetItemTextColor(nItem, g_mapColor[nLevel]);
	m_lcLog.EnsureVisible(nItem, FALSE);
}

// 创建资源树
void CResPackerDlg::BuildRcTree(const RC_INFO& rcInfo, HTREEITEM hItem)
{
	// 根节点
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
		
		// 递归
		BuildRcTree(subRcInfo, hItemParent);
	}

	// 展开
	m_trRes.Expand(hItemParent, TVE_EXPAND);
}

// 根据资源路径，结构化资源信息
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
					AddLog(_T("发现未支持的资源文件: ") + fileFinder.GetFilePath());
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

// 保存资源文件
BOOL CResPackerDlg::SaveRc(const int nSkinType /*= 0*/)
{
	m_strZipRootPath = m_strResPath;
	m_strZipRootPath = m_strZipRootPath.Left(m_strZipRootPath.ReverseFind(_T('\\')));
	CString strZipPath;
	strZipPath.Format(_T("%s\\%d.rc"), (LPCTSTR)m_strZipRootPath, nSkinType);

	if (PathFileExists(strZipPath))
	{
		if (IDYES == MessageBox(_T("指定的文件已经存在，是否覆盖？"), _T("提示"), MB_YESNO))
		{
			if (0 == DeleteFile(strZipPath))
			{
				CString strMsg;
				strMsg.Format(_T("删除文件%s失败，请手动删除后重试！"), (LPCTSTR)strZipPath);
				MessageBox(strMsg);
				return FALSE;
			}
		}
	}

	// 保存为压缩文件
	m_zlib.ClearFile();
	AddRes2Zip(m_zlib, m_rcInfoList);

	std::string strPasswordA = fcT2A(m_strPassword.GetBuffer());
	if (m_zlib.ZipFile(fcT2A(strZipPath.GetBuffer()), strPasswordA.c_str()))
	{
		AddLog(_T("【成功】保存资源到: ") + strZipPath);
	}
	else
	{
		AddLog(_T("【失败】保存资源到: ") + strZipPath);
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

		// 相对路径的起始位置
		int nPos1 = strFullPath.Find(m_strResPath) + m_strResPath.GetLength() + 1;
		CString strHead = strFullPath.Right(strFullPath.GetLength() - nPos1);
		if (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(strFullPath))
		{
			strHead += _T("\\");
			AddLog(_T("跳过目录资源: ") + strHead);
		}
		else
		{
			RES_FILE resFile;
			resFile.strFileName = fcT2A(strHead);
			resFile.strFilePath = fcT2A(rcInfo.strFullPath);
			BOOL bRet = zlib.AddFile(resFile);
			if (bRet)
			{
				AddLog(_T("添加资源成功: ") + strHead);
			}
			else if (resFile.strFileName.length() > 34)
			{
				AddLog(_T("资源路径太长添加失败: ") + strHead, 1);
			}
            else
            {
                AddLog(_T("添加资源失败: ") + strHead, 1);
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

// 打开资源文件
LRESULT CResPackerDlg::OnMsgOpenRc(WPARAM, LPARAM)
{
    if (1)
    {
        /*先解压缩到本地磁盘*/

        // zip 文件所在的路径
        CString strPath = m_strResPath.Left(m_strResPath.ReverseFind(_T('\\')) + 1);

        // 随机一个18位的字符串作为临时目录
        const int nLen = 18;
        char szTemp[nLen] = {0};
        GenRandomString(szTemp, nLen);
        CString strTemp = fcA2T(szTemp);
        strPath = strPath + strTemp;
        // 创建临时目录
        SuperMkDirs(strPath.GetBuffer());

        m_vecTempResPath.push_back(strPath);

        AddLog(_T("解压资源到临时目录: ") + strPath);

        if (m_zlib.UnzipFile(m_strResPath, strPath, fcT2A(m_strPassword.GetBuffer())))
        {
            m_strResPath = strPath;
            PostMessage(WM_ADD_RES, 0, 0);
        }
    }
    else
    {
        AddLog(_T("打开资源文件."));
        // rc 文件 解析文件结构，传出，生成资源树
        BOOL bRet = m_zlib.OpenRc(m_strResPath);
        if (!bRet)
        {
            AddLog(_T("打开文件失败."));
            return -1;
        }
        AddLog(_T("打开文件成功."));

        m_rcInfoList.clear();
        bRet = m_zlib.GetRcInfoList(m_rcInfoList);
        if (!bRet)
        {
            AddLog(_T("解析资源文件失败."));
            return -2;
        }
        AddLog(_T("解析资源文件成功."));

        m_trRes.DeleteAllItems();
        BuildRcTree(m_rcInfoList);
        AddLog(_T("创建资源树."));
    }

	return 0;
}

// 添加资源文件夹
LRESULT CResPackerDlg::OnMsgAddRes(WPARAM, LPARAM)
{
	m_rcInfoList.clear();

	m_rcInfoList.strName = DEFAULT_RC_NAME;
	m_rcInfoList.nRcType = TYPE_FOLDER;
	m_rcInfoList.strFullPath = m_strResPath;
	AddLog(_T("打开文件夹成功."));

	ListRes(m_rcInfoList);
	AddLog(_T("结构化资源文件."));

	m_trRes.DeleteAllItems();
	BuildRcTree(m_rcInfoList);
	AddLog(_T("创建资源树."));

	return 0;
}

// 处理拖放的消息
void CResPackerDlg::OnDropFiles(HDROP hDropInfo)
{
	int nDropCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//取得被拖动文件的数目 
	if (nDropCount != 1)
	{
		return ;
	}
	// 获取路径
	TCHAR szPath[MAX_PATH] = {0};
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);
	DragFinish(hDropInfo); //拖放结束后,释放内存

	m_strResPath = szPath;
	if (FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes(szPath))
	{
		// 打开资源文件
		CString strPath(szPath);
		CString strExt = strPath.Right(strPath.GetLength() - strPath.ReverseFind(_T('.')));
		if (strExt == _T(".rc"))
		{
			PostMessage(WM_OPEN_RC, 0, 0);
		}
	}
	else
	{
		// 添加资源目录
		PostMessage(WM_ADD_RES, 0, 0);
	}

	CDialog::OnDropFiles(hDropInfo);
}


// 点击打开菜单
void CResPackerDlg::OnMenuOpen()
{
	CString strFilter = _T("资源文件 (*.rc)|*.rc||");   //文件过虑的类型  
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, strFilter);
	if (IDOK == dlgFile.DoModal())
	{
		m_strResPath = dlgFile.GetPathName();
		PostMessage(WM_OPEN_RC, 0, 0);
	}
}
// 新建一个资源
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
        if(SUCCEEDED(SHGetMalloc(&pMalloc)))    //pidl指向的对象用完应该释放，之前忽略了
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

// 点击保存菜单
void CResPackerDlg::OnMenuSaveRed()
{
	SaveRc(0);
}

// 点击保存菜单
void CResPackerDlg::OnMenuSaveBlue()
{
	SaveRc(1);
}

void CResPackerDlg::OnMenuExtract()
{
    // m_strResPath 改名
    int nPos = m_strResPath.ReverseFind(_T('\\'));
    CString strNewPath = m_strResPath.Left(nPos);
    strNewPath += _T("\\Res");
    myCopyDirectory(m_strResPath, strNewPath);
    AddLog(_T("解压资源到:") + strNewPath);
}

// 点击设置密码菜单
void CResPackerDlg::OnMenuPassword()
{
	CPasswordDlg dlg;
	dlg.DoModal();
	m_strPassword = CPasswordDlg::LoadPassword();
	AddLog(_T("设置密码:") + m_strPassword);
}

// 点击资源树节点
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

	// 资源的名称
	CString strName = m_trRes.GetItemText(hItem);

	// 获取资源在zip中的完整的名称
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
		// zip包中读取资源数据
		char* pFileBuffer = NULL;
		int nFileLen = 0;
		if (!m_zlib.GetFile(strName, m_strPassword, &pFileBuffer, nFileLen))
		{
			AddLog(_T("读取文件错误."));
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
	// 显示在界面上
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
