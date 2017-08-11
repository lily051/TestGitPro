// LocalVideoFile.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalVideoFile.h"

// CLocalVideoFile 对话框

IMPLEMENT_DYNAMIC(CLocalVideoFile, CDialog)

CLocalVideoFile::CLocalVideoFile(CWnd* pParent /*=NULL*/)
	: CDialog(CLocalVideoFile::IDD, pParent)
    , m_strVEditPath(0)
{
	m_csFilePath = _T("");
	m_nVideoType = 0;
}

CLocalVideoFile::~CLocalVideoFile()
{
	try
	{
		m_csFilePath = _T("");
		m_nVideoType = 0;
	}
	catch (...)
	{
		
	}
}

void CLocalVideoFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIDEOPATH, m_stVideoPath);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editVideoPath);
	DDX_Control(pDX, IDC_BUTTON_OPENPATH, m_btnVideoPath);
	DDX_Control(pDX, IDC_STATIC_VIDEOTYPE, m_stVideoType);
	DDX_Control(pDX, IDC_COMBO_VIDEOTYPE, m_cmbVideoType);
	DDX_Control(pDX, IDC_TREE_VIDEOLIST, m_treeVideoList);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
}


BEGIN_MESSAGE_MAP(CLocalVideoFile, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_OPENPATH, &CLocalVideoFile::OnBnClickedButtonOpenpath)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_VIDEOLIST, &CLocalVideoFile::OnNMDblclkTreeVideolist)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CLocalVideoFile::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CLocalVideoFile 消息处理程序

BOOL CLocalVideoFile::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	MoveCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLocalVideoFile::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CLocalVideoFile::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveCtrl();
}

HBRUSH CLocalVideoFile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

// 初始化控件
void CLocalVideoFile::InitCtrl()
{
    //隐藏文件路径
    m_editVideoPath.ShowWindow(SW_HIDE);

	//文件路径
	m_btnVideoPath.SetWindowText(_T(""));
	m_btnVideoPath.SetBitmaps(IDB_BITMAP_OPENFILEOFF, RGB(250, 0, 250), IDB_BITMAP_OPENFILEON, RGB(250, 0, 250));
    m_btnVideoPath.ShowWindow(SW_HIDE);

	//录像类型
	m_cmbVideoType.InsertString(0, _T("海康威视"));
	m_cmbVideoType.SetItemData(0, 0);
	m_cmbVideoType.SetCurSel(0);
    m_cmbVideoType.ShowWindow(SW_HIDE);

	//资源树
	m_ImageList.Create(16,16,ILC_COLOR32|ILC_MASK,1,1);
	CBitmap bmp;
	bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_VEDIOFOLDER, Color(0,0,0,180)));
	m_ImageList.Add(&bmp, RGB(0,0,180));
	DeleteObject(bmp.Detach());

	bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_VEDIOFILE, Color(0,0,0,180)));
	m_ImageList.Add(&bmp, RGB(0,0,180));
	DeleteObject(bmp.Detach());

	bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_VEDIOFILE_PLAY, Color(0,0,0,180)));
	m_ImageList.Add(&bmp, RGB(0,0,180));
	DeleteObject(bmp.Detach());

	m_treeVideoList.SetTextColor(UI_TEXT_COLOR);
	m_treeVideoList.SetBkColor(UI_TREE_BKCOLOR);
	m_treeVideoList.SetImageList(&m_ImageList, LVSIL_NORMAL);
	m_treeVideoList.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_treeVideoList.SetItemHeight(20);

	//刷新按钮
	m_btnRefresh.SetSkinBitmap(RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_BTN_REFRESH), RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_BTN_REFRESH_OVER), RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_BTN_REFRESH_OVER));
    m_btnRefresh.ShowWindow(SW_HIDE);
}

//移动控件
void CLocalVideoFile::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nSpace = 4;
	int nLeft = rcClient.left;
	int nTop = rcClient.top + nSpace * 2;
	int nStaWidth = 100;
	int nStaHeight = 16;
	int nEditWidth = rcClient.Width() - 26;
	int nEditHeight = 18;
    int ntopOnlyTree = nTop;

	//录像路径
	m_stVideoPath.MoveWindow(nLeft, nTop, nStaWidth, nStaHeight);
	nTop = nTop + nStaHeight;
	m_editVideoPath.MoveWindow(nLeft, nTop, nEditWidth, nEditHeight);
	nLeft = nLeft + nEditWidth + nSpace/2;
	m_btnVideoPath.MoveWindow(nLeft, nTop-nSpace, 24, 24);

	//录像类型
	nLeft = rcClient.left;
	nTop = nTop + nEditHeight + nSpace*2;
	m_stVideoType.MoveWindow(nLeft, nTop, nStaWidth, nStaHeight);
	nTop = nTop + nStaHeight;
	m_cmbVideoType.MoveWindow(nLeft, nTop, rcClient.Width(), 100);

	//资源树
    /*nTop = nTop + nStaHeight + nSpace*3;*/
	nTop = ntopOnlyTree; //使树列表顶头显示
	m_treeVideoList.MoveWindow(rcClient.left, nTop, rcClient.Width(), rcClient.bottom/* - nTop - nEditHeight - nSpace*3*/);

	//刷新按钮
	nLeft = rcClient.left+1;
	nTop = nTop + (rcClient.bottom - nTop - nEditHeight - nSpace*2 - 1);
	m_btnRefresh.MoveWindow(nLeft, nTop, rcClient.Width()-nSpace, nStaHeight + 2*nSpace);
}

void CLocalVideoFile::OnBnClickedButtonOpenpath()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSelPath = AppFun_GetDirectoryPath(this->GetSafeHwnd(),m_strEditPath);
	if (strSelPath.IsEmpty())
	{
		return;
	}
	//检查路径的合法性
	if (!AppFun_CheckStrLegality(strSelPath))
	{
		MsgBoxApi_Show(_T("选择的路径不能含有特殊字符%、％、'"), MSGBOX_TYPE_WARNING);
		return;
	}

	if (strSelPath.GetAt(strSelPath.GetLength()-1) == _T('\\'))
	{
		strSelPath = strSelPath.Left(strSelPath.GetLength() - 1);
	}

	//保存选择的路径
	m_strEditPath = strSelPath;
	m_editVideoPath.SetWindowText(strSelPath);
	GetPathItem(strSelPath);
}

//创建资源树
void CLocalVideoFile::GetPathItem(CString strPath, HTREEITEM hRegionItem)
{
	PLAYBACKDLL_INFO(_T("local create tree start"));
	if (strPath == "")
	{
		PLAYBACKDLL_ERROR(_T("local create tree path is empty"));
		return;
	}

	if(hRegionItem == NULL)
	{
		m_treeVideoList.DeleteAllItems();
	}

	// 去掉最后的\符号.
	if (strPath.GetAt(strPath.GetLength()-1) == _T('\\'))
	{
		strPath = strPath.Left(strPath.GetLength() - 1);
	}

	CString strFolderName(strPath);

	//获取文件
	//给路径名后面加上“\*.*”
	strPath.AppendFormat(_T("%s"), _T("\\*.*")); //找所有文件 


	strPath.TrimLeft();   
	strPath.TrimRight(); 
	TRACE(_T("@z strPath:%s"), (LPCTSTR)strPath);

	//开始搜索文件夹，保存到内部数组中
	WIN32_FIND_DATA FindedFolderInfo;
	HANDLE hFind = ::FindFirstFile( strPath.GetBuffer(), &FindedFolderInfo );
	strPath.ReleaseBuffer();

	//查找失败
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return;
	}
	std::vector<CString> vecCurFolders;
	std::vector<CString> vecVedioFiles;
	// 遍历获取所有文件夹的名字
	do 
	{
		if( FindedFolderInfo.cFileName[0] == _T('.') ) //过滤这个目录
		{
			continue; 
		}
		//添加 不遍历系统文件夹，如回收站的文件夹
		if (FindedFolderInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		{
			continue;
		}

		if( FindedFolderInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)   // 记录文件名 和 文件夹数目
		{   
			vecCurFolders.push_back(FindedFolderInfo.cFileName);
		}

		CString strFileName;
		strFileName.Format(_T("%s"), FindedFolderInfo.cFileName);
		if (strFileName.ReverseFind('.') > 0)
		{
			CString strTypeName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.'));

			if ( strTypeName == _T(".264")|| strTypeName == _T(".mp4") || strTypeName == _T(".dav"))
			{
				vecVedioFiles.push_back(FindedFolderInfo.cFileName);
			}
		}

	}while(::FindNextFile( hFind, &FindedFolderInfo ) && ((int)vecCurFolders.size() < LOCAL_MAX_FILENUM) ); // 只显示前MAX_FILENUM个文件夹
	::FindClose(hFind);

    
	//文件夹加入树结构中
	for (int i = 0; i < (int)vecCurFolders.size(); i++)
	{
		HTREEITEM hCellItem;
		if (hRegionItem == NULL)
		{
			hCellItem = m_treeVideoList.InsertItem(vecCurFolders[i], 0, 0, TVI_ROOT);
			m_treeVideoList.SetItemData(hCellItem,typeDirectory);
			CString strSubPath(strFolderName);
			strSubPath += _T("\\");
			strSubPath += vecCurFolders[i];
			GetPathItem(strSubPath,hCellItem);
			m_treeVideoList.Expand(hCellItem,TVE_EXPAND);
		}
		else
		{
			hCellItem = m_treeVideoList.InsertItem(vecCurFolders[i], 0, 0, hRegionItem);
			m_treeVideoList.SetItemData(hCellItem,typeDirectory);
			CString strSubPath(strFolderName);
			strSubPath += _T("\\");
			strSubPath += vecCurFolders[i];
			GetPathItem(strSubPath,hCellItem);
		}
	}

	//文件加入树结构中
	for (int i = 0; i < (int)vecVedioFiles.size(); i++)
	{
		if (hRegionItem == NULL)
		{
			HTREEITEM hCellItem = m_treeVideoList.InsertItem(vecVedioFiles[i], 1, 1, TVI_ROOT);
			m_treeVideoList.SetItemData(hCellItem,typeFile);
		}
		else
		{
			HTREEITEM hCellItem = m_treeVideoList.InsertItem(vecVedioFiles[i], 1, 1, hRegionItem);
			m_treeVideoList.SetItemData(hCellItem,typeFile);
		}
	}
	PLAYBACKDLL_INFO(_T("local create tree end"));
}

void CLocalVideoFile::OnNMDblclkTreeVideolist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelItem = m_treeVideoList.GetSelectedItem();
	if (NULL == hSelItem)
	{
		return;
	}
	int nItemData = m_treeVideoList.GetItemData(hSelItem);
	CString strFileName;
	strFileName = m_treeVideoList.GetItemText(hSelItem);

	switch (nItemData)
	{
	case typeDirectory:
		break;
	case typeFile:
		{
			//m_csFilePath = m_strEditPath;
            //不同路径下文件的兼容
            for (int i = 0; i < m_strVEditPath.size(); i++)
            {
                if (-1 != m_strVEditPath[i].Find(strFileName))
                {
                    m_csFilePath = m_strVEditPath[i];
                    break;
                }
            }
			HTREEITEM hParentItem = m_treeVideoList.GetParentItem(hSelItem);
			std::map<int, CString> vDirName;
			int nKey = 0;
			while (hParentItem != NULL)
			{
				CString strParentFolder = m_treeVideoList.GetItemText(hParentItem);
				vDirName[nKey++] = strParentFolder;
				hParentItem = m_treeVideoList.GetParentItem(hParentItem);
			}
// 			for (int i = nKey - 1; i >= 0 ; i--)
// 			{
// 				m_csFilePath += _T("\\");
// 				m_csFilePath += vDirName[i];
// 			}
			vDirName.clear();

			//m_csFilePath = m_csFilePath + _T("\\") + strFileName;
			m_nVideoType = 0;

			::PostMessage(this->GetParent()->GetSafeHwnd(), MSG_LOCALVEDIO_PLAY, 0, 0);
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}

void CLocalVideoFile::GetFileInfo(int &nVideoType, CString &csFilePath)
{
	nVideoType = m_nVideoType;
	csFilePath = m_csFilePath;
}

// 本地回放.
void CLocalVideoFile::LocalPlayback(const int nVideoType, const CString csFilePath)
{
    std::vector<CString> sttVFilePath;
	InitRecoredPath(csFilePath, sttVFilePath);
    m_nVideoType = 0;
	m_csFilePath = sttVFilePath.front();
	::PostMessage(this->GetParent()->GetSafeHwnd(), MSG_LOCALVEDIO_PLAY, 0, 0);
}

//刷新按钮
void CLocalVideoFile::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	GetPathItem(m_strEditPath);
}

void CLocalVideoFile::InitRecoredPath(const CString strFilePath, std::vector<CString>& sttVFilePath)
{
    CString  ch = _T("|");
    int intiPos = 0;
    CString strTmp;
    strTmp = strFilePath.Tokenize(ch, intiPos);
    while(strTmp.Trim() != _T(""))
    {
        strTmp.Replace(_T("\\\\"), _T("\\"));
        sttVFilePath.push_back(strTmp);
		PLAYBACKDLL_INFO(_T("OnMsgLocalVideoPlay:%s"), strFilePath);
        strTmp = strFilePath.Tokenize(ch, intiPos);
    }
    
    m_strVEditPath = sttVFilePath;
//     for (int i = 0; i < sttVFilePath.size(); i++)
//     {
//         CString strPath = sttVFilePath[i];
//         // 去掉最后的\符号.
//         if (strPath.GetAt(strPath.GetLength()-1) != _T('\\'))
//         {
//             int pos = strPath.ReverseFind(_T('\\'));
//             strPath = strPath.Left(pos -1);
//         }
//         m_strVEditPath.push_back(strPath);
//     }

    SetPathItem(sttVFilePath);
}

//设置树列表内容，插入树
void CLocalVideoFile::SetPathItem(std::vector<CString> strPath)
{
    PLAYBACKDLL_INFO(_T("local create tree start"));
    if (strPath.size() == 0)
    {
        PLAYBACKDLL_ERROR(_T("local create tree path is empty"));
        return;
    }

    m_treeVideoList.DeleteAllItems();
 
    // 获取路径中的文件名.
    CString strFileName = _T("");
    std::vector<CString> stVFileName;
    for (int i = 0; i < strPath.size(); i++)
    {
        if (strPath[i].GetAt(strPath[i].GetLength() - 1) != _T('\\'))
        {
            strFileName = strPath[i].Mid(strPath[i].ReverseFind('\\') + 1);
            stVFileName.push_back(strFileName);
        }
    }

    for (int i = 0; i < (int)stVFileName.size(); i++)
    {
            HTREEITEM hCellItem = m_treeVideoList.InsertItem(stVFileName[i], 1, 1, TVI_ROOT);
            m_treeVideoList.SetItemData(hCellItem, typeFile);
    }
}
