
// RecToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RecTool.h"
#include "RecToolDlg.h"

#define CAM_TYPE_IPC	1
#define CAM_TYPE_USB	2

CRecToolDlg * g_pRecMgrDlg = NULL;

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


// CRecToolDlg 对话框




CRecToolDlg::CRecToolDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CRecToolDlg::IDD, pParent)
	,m_lpPreviewCtrlDlg(NULL)
	,m_bIsRecording(FALSE)
	,m_lpRecNameInfoDlg(NULL)
	,m_lpPlaybackCtrlDlg(NULL)
	,m_lpBottomDlg(NULL)
	,m_lpListInfoDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_pRecMgrDlg = this;

 	this->SetDrawTitleIcon(FALSE);		//不需要FDialog自绘标题图标（因为我自己有）
 	this->SetTitleLeftMargin(5);	//标题栏图标左边距20
    this->SetTitleTextLeft(10);
}

CRecToolDlg::~CRecToolDlg()
{
	if (NULL != m_lpPreviewCtrlDlg)
	{
		delete m_lpPreviewCtrlDlg;
		m_lpPreviewCtrlDlg = NULL;
	}
}

void CRecToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CFDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_listRecords);
    DDX_Control(pDX, IDC_STATIC_BLANK1, m_staticBlank1);
}

BEGIN_MESSAGE_MAP(CRecToolDlg, CFDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CRecToolDlg 消息处理程序

BOOL CRecToolDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

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
	LoadSkin();

	CRect rcClient;
	rcClient.left = 0;
	rcClient.top = 0;
	rcClient.right = 855;
	rcClient.bottom = 420;
	MoveWindow(rcClient);

	CenterWindow();

	SetWindowText(_T("离线录像"));
	this->SetTitleFont(YAHEI_20_B);
    this->SetTitleColor(RGB(188, 188, 188));

	InitCtrl();

	MoveCtrl();

	StartPreview();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRecToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CFDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRecToolDlg::OnPaint()
{
    CPaintDC dc(this); // 用于绘制的设备上下文

//     //底部横线
//     CRect rc1;
//     GetDlgItem(IDC_STATIC_BLANK1)->GetWindowRect(&rc1);
//     ScreenToClient(rc1);
//     CPen linePen;
//     linePen.CreatePen(PS_SOLID, 1, RGB(0x34, 0x34, 0x34));
//     CPen * pOld = dc.SelectObject(&linePen);
//     int n = rc1.top + rc1.Height() / 2;
//     dc.MoveTo(rc1.left, n);
//     dc.LineTo(rc1.right, n);

    //窗口背景色
    CRect rect;
    GetClientRect(&rect);
    dc.FillSolidRect(rect, RGB(0, 0, 0));

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
		CFDialog::OnPaint();
	}



}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRecToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CRecToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFDialog::PreTranslateMessage(pMsg);
}


void CRecToolDlg::LoadSkin(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT);
	vecMargin.push_back(MARGIN_TOP_DLG);
	vecMargin.push_back(MARGIN_RIGHT);
	vecMargin.push_back(MARGIN_BOTTOM);
	SetMargin(vecMargin);

	SetObjectName(_T("local"));
}

void CRecToolDlg::InitCtrl(void)
{
	//预览窗口
	if (NULL == m_lpPreviewCtrlDlg)
	{
		try
		{
			m_lpPreviewCtrlDlg = new CPreviewCtrlDlg(this);
		}
		catch (...)
		{
			return;
		}
		m_lpPreviewCtrlDlg->Create(IDD_DLG_PREVIEW_CRTL, this);
		m_lpPreviewCtrlDlg->ShowWindow(SW_SHOW);
	}


	//录像名称窗口
	if (NULL == m_lpRecNameInfoDlg)
	{
		try
		{
			m_lpRecNameInfoDlg = new CRecNameInfoDlg(this);
		}
		catch(...)
		{
			return;
		}
		m_lpRecNameInfoDlg->Create(IDD_DLG_RECNAMEINFO, this);
		m_lpRecNameInfoDlg->ShowWindow(SW_SHOW);
	}

	//状态栏
	if (NULL == m_lpBottomDlg)
	{
		m_lpBottomDlg = new CBottomDlg;
		if (NULL == m_lpBottomDlg)
		{
			return;
		}
		m_lpBottomDlg->Create(IDD_DLG_BOTTOM);
		m_lpBottomDlg->ShowWindow(SW_SHOW);
	}

	//回放窗口
	if (NULL == m_lpPlaybackCtrlDlg)
	{
		try
		{
			m_lpPlaybackCtrlDlg = new CPlaybackCtrlDlg(this);
		}
		catch (...)
		{
			return;	
		}
		m_lpPlaybackCtrlDlg->Create(IDD_DLG_PLAYBACK_CTRL, GetDesktopWindow());
		m_lpPlaybackCtrlDlg->ShowWindow(SW_HIDE);
	}

	//回放窗口
	if (NULL == m_lpListInfoDlg)
	{
		try
		{
			m_lpListInfoDlg = new CListInfoDlg(this);
		}
		catch (...)
		{
			return;	
		}
		m_lpListInfoDlg->Create(IDD_DLG_LISTINFO, this);
		m_lpListInfoDlg->ShowWindow(SW_SHOW);
	}


	//录像列表
	m_listRecords.SetToolTipEnable(true);
	m_listRecords.SetHeadHeight(0);  //设置表头高度(0-不显示)
	m_listRecords.SetHeight(25);
    m_listRecords.SetStyle(FLS_BLACK);
	//SkinWndScroll(&m_listRecords);
	m_listRecords.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 30);
	m_listRecords.InsertColumn(1, _T("文件"), LVCFMT_LEFT, 165);
	m_listRecords.InsertColumn(2, _T("时长"), LVCFMT_LEFT, 60);
	m_listRecords.InsertColumn(3, _T("大小"), LVCFMT_LEFT, 50);
	m_listRecords.InsertColumn(4, _T("操作"), LVCFMT_LEFT, 80);
	m_listRecords.SetButton(4, 3, _T("edit-(video)"), _T("view"), _T("delete"));
	m_listRecords.SetButtonToolTip(3, _T("修改名称"), _T("查看"), _T("删除"));
	m_listRecords.SetHeadLock(TRUE);

    GetDlgItem(IDC_STATIC_BLANK1)->ShowWindow(SW_HIDE);
}

void CRecToolDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	/********************预览窗口Start********************/
	CRect rcDlgRecName(rcClient);
	rcDlgRecName.right = rcDlgRecName.left + 468;
	rcDlgRecName.bottom = rcDlgRecName.top + 30;
	if (NULL != m_lpRecNameInfoDlg)
	{
		m_lpRecNameInfoDlg->MoveWindow(rcDlgRecName);
	}

	CRect rcPreview(rcClient);
	rcPreview.top = rcDlgRecName.bottom;
	rcPreview.right = rcClient.left + 468;
	rcPreview.bottom = rcPreview.top + 318;
	if (NULL != m_lpPreviewCtrlDlg)
	{
		m_lpPreviewCtrlDlg->MoveWindow(rcPreview);
	}

	CRect rcBottom(rcClient);
	rcBottom.top = rcPreview.bottom;
	if (NULL != m_lpBottomDlg)
	{
		m_lpBottomDlg->MoveWindow(rcBottom);
	}

	 CRect rcListInfo(rcClient);
	rcListInfo.left = rcPreview.right - 1;
	 rcListInfo.bottom = rcListInfo.top + 30;
	 if (NULL != m_lpListInfoDlg)
	 {
		 m_lpListInfoDlg->MoveWindow(rcListInfo);
	 }


	CRect rcList(rcClient);
	rcList.left = rcPreview.right;
	rcList.top = rcListInfo.bottom;
	rcList.bottom = rcBottom.top;
	m_listRecords.MoveWindow(rcList);

//     //底部横线
//     CRect rcStaticBlank1(rcClient);
//     rcStaticBlank1.left = rcList.left;
//     rcStaticBlank1.top = rcList.bottom;
//     rcStaticBlank1.right = rcList.right;
//     rcStaticBlank1.bottom = rcStaticBlank1.top + 1;
//     m_staticBlank1.MoveWindow(rcStaticBlank1);

}

//处理录像提示通知信息
void CRecToolDlg::NotifyRecordFileInfo(const RECOED_FILE_INFO &recordFile)
{
	BOOL bRet = FALSE;
	CString strFileName = recordFile.csPicPath;
	const RECOED_FILE_INFO * pRecordFileInfo = new RECOED_FILE_INFO(recordFile);
	RECOED_FILE_INFO recInfo = recordFile;

	if (0 == recordFile.nRecordStatus)  //开始录像(录像中)
	{
		InsertPic(recordFile);	//插入开始录像截图
		if (NULL != m_lpRecNameInfoDlg)
		{
			bRet = ::SendMessage(m_lpRecNameInfoDlg->GetSafeHwnd(), WM_PV_STARTRECORD, WPARAM(pRecordFileInfo), 0);
		}
		//开始录像禁用按钮
		m_bIsRecording = true;
	}
	else if (1 == recordFile.nRecordStatus)  //结束录像
	{
		InsertPic(recordFile);	//插入结束录像截图
		if (NULL != m_lpRecNameInfoDlg)
		{
			bRet = ::SendMessage(m_lpRecNameInfoDlg->GetSafeHwnd(), WM_PV_STOPRECORD, WPARAM(pRecordFileInfo), 0);
			//m_lpRecNameInfoDlg->StopRecord(recInfo.strFileName);
			//CLIENT_INFO("Before Rename:%s", recordFile.strFileName);
			//CFile::Rename(recordFile.strFileName, recInfo.strFileName);
			//CLIENT_INFO("After Rename:%s", recInfo.strFileName);
		}
		InsertRec(recInfo);	//插入录像文件
		m_bIsRecording = false;
	}
	delete pRecordFileInfo;
}

//开始预览
BOOL CRecToolDlg::StartPreview(void)
{
// 	CConfigDlg dlg;
// 	// 	if (dlg.GetUSBCamCount() <= 0) //未检测到USB相机
// 	// 	{
// 	// 		FMessageBox::Show(_T("未检测到USB摄像机，请重新连接后再开启预览。"), _T("提示"), MB_OK | MB_ICONWARNING);
// 	// 		return FALSE;
// 	// 	}
// 	if (dlg.GetUSBCamCount() < theApp.m_nCamIndex + 1)
// 	{
// 		FMessageBox::Show(_T("检测到选择的默认USB摄像机已不存在\r\n请到参数配置中重新选择USB摄像机并重启本程序。"), _T("错误"), MB_OK | MB_ICONWARNING);
// 		return FALSE;
// 	}
	//配置文件路径
	CString strConfigPath = theApp.m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(_T("FMSClient.ini"));

	int nCamIndex = GetPrivateProfileInt(_T("LocalConfig"), _T("CamIndex"), 0, strConfigPath);
	CString strUSBCamInfo= _T("");

	PREVIEW_CAMINFO stCameraInfo;
	stCameraInfo.nCamType = CAM_TYPE_USB;
	stCameraInfo.strCamName = _T("USB Camera00");
	stCameraInfo.strMicName = _T("USB Mic");
	stCameraInfo.lCamIndex = nCamIndex;
	stCameraInfo.lMicIndex = nCamIndex;
	stCameraInfo.nTimeout = 30;
	stCameraInfo.nDelayTime = 30;

	if (NULL !=m_lpPreviewCtrlDlg)
	{
		m_lpPreviewCtrlDlg->StartPreview(stCameraInfo);
	}



	return TRUE;
}

//插入图片到列表
BOOL CRecToolDlg::InsertPic(const RECOED_FILE_INFO &recordFile)
{
// 	ITEMDATA * pData = new ITEMDATA;
// 	pData->strFileName = recordFile.csPicPath;
	HANDLE hFileHandle = NULL;
	//序号
	int nIndex = m_listRecords.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	m_listRecords.InsertItem(nIndex, strIndex);
	//m_listRecords.SetItemData(nIndex, (DWORD)pData);
	//文件名
	CString strFileName = recordFile.csPicPath.Mid(recordFile.csPicPath.ReverseFind('\\') + 1);
	//m_listRecords.SetItemText(nIndex, 1, recordFile.csPicPath);
	m_listRecords.SetItemText(nIndex, 1, strFileName);
	//录像时长
	CString strFileDuration = _T("--");
	m_listRecords.SetItemText(nIndex, 2, strFileDuration);
	//文件大小
	hFileHandle = CreateFile(recordFile.csPicPath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFileHandle)
	{
		//FMSCLIENT_ERROR("CreateFile Failed![Error:%d]", GetLastError());
		return FALSE;
	}

	int nFileSize = (long)GetFileSize(hFileHandle, NULL);
	CloseHandle(hFileHandle);
	hFileHandle = NULL;
	CString strFileSize;
	strFileSize.Format(_T("%.2fM"),  (double)nFileSize / 1024 / 1024);
	m_listRecords.SetItemText(nIndex, 3, strFileSize);

	//记录插入的文件
	m_PicFileArray.Add(recordFile);
	m_ArrayFileList.Add(recordFile.csPicPath);

	return TRUE;
}

BOOL CRecToolDlg::InsertRec(const RECOED_FILE_INFO &recordFile)
{
// 	ITEMDATA * pData = new ITEMDATA;
// 	pData->strFileName = recordFile.strFileName;
	//序号
	int nIndex = m_listRecords.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	m_listRecords.InsertItem(nIndex, strIndex);
	//m_listRecords.SetItemData(nIndex, (DWORD)pData);
	//文件名
	//int nPos = recordFile.strFileName.ReverseFind('\\');
	CString strFileName = recordFile.strFileName.Mid(recordFile.strFileName.ReverseFind('\\') + 1);
	m_listRecords.SetItemText(nIndex, 1, strFileName);
	//m_listRecords.SetItemText(nIndex, 1, recordFile.strFileName);
	//录像时长
	CString strFileDuration;
	int nFileDuration = recordFile.lEndTime - recordFile.lStartTime;
	if (0 == nFileDuration)
	{
		strFileDuration = _T("--");
		m_listRecords.SetItemText(nIndex, 2, strFileDuration);
	}
	else
	{
		strFileDuration.Format(_T("%02d:%02d:%02d"), nFileDuration / 3600, nFileDuration % 3600 / 60, nFileDuration % 3600 % 60);		
		m_listRecords.SetItemText(nIndex, 2, strFileDuration);
	}

	//文件大小
	CString strFileSize;
	strFileSize.Format(_T("%.2fM"),  (double)recordFile.lFileSize / 1024 / 1024);
	/*double dFileSize = (double)recordFile.lFileSize / 1024 / 1024;*/
	m_listRecords.SetItemText(nIndex, 3, strFileSize);

	//添加到录像列表
	m_RecFileArray.Add(recordFile);
	m_ArrayFileList.Add(recordFile.strFileName);

	return TRUE;
}
BOOL CRecToolDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (MSG_MULTIBTN_CLICK == wParam)  
	{
		//按钮信息的结构体
		STRU_MULTIBTN_INFO * multiBtnInfo = (STRU_MULTIBTN_INFO*)lParam;
		//CString strFileName = m_listRecords.GetItemText(multiBtnInfo->nItem, 1);
// 		ITEMDATA * pData = (ITEMDATA *)m_listRecords.GetItemData(multiBtnInfo->nItem);
// 		CString strFileName = pData->strFileName;
		int nItem = multiBtnInfo->nItem;
		if (-1 == nItem)
		{
			//return FALSE;
		}

		CString strFileName = m_ArrayFileList.GetAt(nItem);
		if (multiBtnInfo->nNumber == 0)	// 修改文件名按钮
		{
			CEditFileNameDlg dlg;
			dlg.SetDlgData(strFileName);
			if (IDOK == dlg.DoModal())
			{
				m_listRecords.SetItemText(nItem, 1, theApp.m_strNewFileName);
				CString strText = strFileName.Mid(0, strFileName.ReverseFind('\\')) + _T('\\') + theApp.m_strNewFileName;
// 				pData->strFileName = strText;
// 				m_listRecords.SetItemData(nItem, (DWORD)pData);
				m_ArrayFileList.SetAt(nItem, strText);
				if (strFileName.Find(_T("mp4")) > 0)   //视频
				{
					for (int i = 0; i < m_RecFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_RecFileArray.GetAt(i);
						if (recInfo.strFileName.Find(strFileName) >= 0)
						{
							RECOED_FILE_INFO info = recInfo;
							info.strFileName = strText;
							m_RecFileArray.SetAt(i, info);
						}
					}
				}
				else
				{
					for (int i = 0; i < m_PicFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_PicFileArray.GetAt(i);
						if (recInfo.csPicPath.Find(strFileName) >= 0)
						{
							RECOED_FILE_INFO info = recInfo;
							info.csPicPath = strText;
							m_PicFileArray.SetAt(i, info);
						}
					}
				}
			}
		}
		else if (multiBtnInfo->nNumber == 1)		//回放按钮
		{
			if (strFileName.Find(_T("mp4")) > 0)   //视频
			{
				m_lpPlaybackCtrlDlg->ShowWindow(SW_SHOW);
				m_lpPlaybackCtrlDlg->StartPlayBack(0, strFileName);
			}
			else		//图片
			{
				TCHAR szSystemDir[MAX_PATH];
				::GetSystemDirectory(szSystemDir, MAX_PATH);

				CString strRundll;
				strRundll.Format(_T("%s\\rundll32.exe"), szSystemDir);

				//替换路径，否则打开文件失败
				CString strTemp = strFileName;
				strTemp.Replace(_T("\\\\"), _T("\\"));

				CString strParam;
				strParam.Format(_T("%s\\shimgvw.dll imageview_fullscreen %s"), szSystemDir, strTemp);
				ShellExecute(NULL, _T("Open"), strRundll.GetBuffer(), strParam, NULL, SW_SHOW);
			}

		}
		else if (multiBtnInfo->nNumber == 2)  //删除按钮
		{
			if (IDYES == FMessageBox::Show(_T("确定删除该文件吗？\r\n注意：本地文件将会被删除。"), _T("删除"), MB_YESNO | MB_ICONQUESTION))
			{
				if (strFileName.Find(_T(".mp4")) > 0)
				{
					for (int i = 0; i < m_RecFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_RecFileArray.GetAt(i);
						if (recInfo.strFileName.Find(strFileName) >= 0)
						{
							m_RecFileArray.RemoveAt(i);
							m_listRecords.DeleteItem(nItem);
						}
					}
				}
				else
				{
					for (int i = 0; i < m_PicFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_PicFileArray.GetAt(i);
						if (recInfo.csPicPath.Find(strFileName) >= 0)
						{
							m_PicFileArray.RemoveAt(i);
							m_listRecords.DeleteItem(nItem);
						}
					}
				}

				m_ArrayFileList.RemoveAt(nItem);

				if (!::DeleteFile(strFileName))
				{
					delete multiBtnInfo;
					return FALSE;
				}
			}
		}

		//释放内存，并返回TRUE
		delete multiBtnInfo;
		return TRUE;
	}

	return CFDialog::OnCommand(wParam, lParam);
}

BOOL CRecToolDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CRect rcClient;
    GetClientRect(rcClient);
    pDC->FillSolidRect(rcClient, RGB(0x1F, 0x1F, 0x1F));


    return CFDialog::OnEraseBkgnd(pDC);
}
