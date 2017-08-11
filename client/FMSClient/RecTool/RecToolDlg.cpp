
// RecToolDlg.cpp : ʵ���ļ�
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


// CRecToolDlg �Ի���




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

 	this->SetDrawTitleIcon(FALSE);		//����ҪFDialog�Ի����ͼ�꣨��Ϊ���Լ��У�
 	this->SetTitleLeftMargin(5);	//������ͼ����߾�20
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


// CRecToolDlg ��Ϣ�������

BOOL CRecToolDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

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
	LoadSkin();

	CRect rcClient;
	rcClient.left = 0;
	rcClient.top = 0;
	rcClient.right = 855;
	rcClient.bottom = 420;
	MoveWindow(rcClient);

	CenterWindow();

	SetWindowText(_T("����¼��"));
	this->SetTitleFont(YAHEI_20_B);
    this->SetTitleColor(RGB(188, 188, 188));

	InitCtrl();

	MoveCtrl();

	StartPreview();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRecToolDlg::OnPaint()
{
    CPaintDC dc(this); // ���ڻ��Ƶ��豸������

//     //�ײ�����
//     CRect rc1;
//     GetDlgItem(IDC_STATIC_BLANK1)->GetWindowRect(&rc1);
//     ScreenToClient(rc1);
//     CPen linePen;
//     linePen.CreatePen(PS_SOLID, 1, RGB(0x34, 0x34, 0x34));
//     CPen * pOld = dc.SelectObject(&linePen);
//     int n = rc1.top + rc1.Height() / 2;
//     dc.MoveTo(rc1.left, n);
//     dc.LineTo(rc1.right, n);

    //���ڱ���ɫ
    CRect rect;
    GetClientRect(&rect);
    dc.FillSolidRect(rect, RGB(0, 0, 0));

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
		CFDialog::OnPaint();
	}



}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRecToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CRecToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

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
	//Ԥ������
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


	//¼�����ƴ���
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

	//״̬��
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

	//�طŴ���
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

	//�طŴ���
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


	//¼���б�
	m_listRecords.SetToolTipEnable(true);
	m_listRecords.SetHeadHeight(0);  //���ñ�ͷ�߶�(0-����ʾ)
	m_listRecords.SetHeight(25);
    m_listRecords.SetStyle(FLS_BLACK);
	//SkinWndScroll(&m_listRecords);
	m_listRecords.InsertColumn(0, _T("���"), LVCFMT_LEFT, 30);
	m_listRecords.InsertColumn(1, _T("�ļ�"), LVCFMT_LEFT, 165);
	m_listRecords.InsertColumn(2, _T("ʱ��"), LVCFMT_LEFT, 60);
	m_listRecords.InsertColumn(3, _T("��С"), LVCFMT_LEFT, 50);
	m_listRecords.InsertColumn(4, _T("����"), LVCFMT_LEFT, 80);
	m_listRecords.SetButton(4, 3, _T("edit-(video)"), _T("view"), _T("delete"));
	m_listRecords.SetButtonToolTip(3, _T("�޸�����"), _T("�鿴"), _T("ɾ��"));
	m_listRecords.SetHeadLock(TRUE);

    GetDlgItem(IDC_STATIC_BLANK1)->ShowWindow(SW_HIDE);
}

void CRecToolDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	/********************Ԥ������Start********************/
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

//     //�ײ�����
//     CRect rcStaticBlank1(rcClient);
//     rcStaticBlank1.left = rcList.left;
//     rcStaticBlank1.top = rcList.bottom;
//     rcStaticBlank1.right = rcList.right;
//     rcStaticBlank1.bottom = rcStaticBlank1.top + 1;
//     m_staticBlank1.MoveWindow(rcStaticBlank1);

}

//����¼����ʾ֪ͨ��Ϣ
void CRecToolDlg::NotifyRecordFileInfo(const RECOED_FILE_INFO &recordFile)
{
	BOOL bRet = FALSE;
	CString strFileName = recordFile.csPicPath;
	const RECOED_FILE_INFO * pRecordFileInfo = new RECOED_FILE_INFO(recordFile);
	RECOED_FILE_INFO recInfo = recordFile;

	if (0 == recordFile.nRecordStatus)  //��ʼ¼��(¼����)
	{
		InsertPic(recordFile);	//���뿪ʼ¼���ͼ
		if (NULL != m_lpRecNameInfoDlg)
		{
			bRet = ::SendMessage(m_lpRecNameInfoDlg->GetSafeHwnd(), WM_PV_STARTRECORD, WPARAM(pRecordFileInfo), 0);
		}
		//��ʼ¼����ð�ť
		m_bIsRecording = true;
	}
	else if (1 == recordFile.nRecordStatus)  //����¼��
	{
		InsertPic(recordFile);	//�������¼���ͼ
		if (NULL != m_lpRecNameInfoDlg)
		{
			bRet = ::SendMessage(m_lpRecNameInfoDlg->GetSafeHwnd(), WM_PV_STOPRECORD, WPARAM(pRecordFileInfo), 0);
			//m_lpRecNameInfoDlg->StopRecord(recInfo.strFileName);
			//CLIENT_INFO("Before Rename:%s", recordFile.strFileName);
			//CFile::Rename(recordFile.strFileName, recInfo.strFileName);
			//CLIENT_INFO("After Rename:%s", recInfo.strFileName);
		}
		InsertRec(recInfo);	//����¼���ļ�
		m_bIsRecording = false;
	}
	delete pRecordFileInfo;
}

//��ʼԤ��
BOOL CRecToolDlg::StartPreview(void)
{
// 	CConfigDlg dlg;
// 	// 	if (dlg.GetUSBCamCount() <= 0) //δ��⵽USB���
// 	// 	{
// 	// 		FMessageBox::Show(_T("δ��⵽USB����������������Ӻ��ٿ���Ԥ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
// 	// 		return FALSE;
// 	// 	}
// 	if (dlg.GetUSBCamCount() < theApp.m_nCamIndex + 1)
// 	{
// 		FMessageBox::Show(_T("��⵽ѡ���Ĭ��USB������Ѳ�����\r\n�뵽��������������ѡ��USB�����������������"), _T("����"), MB_OK | MB_ICONWARNING);
// 		return FALSE;
// 	}
	//�����ļ�·��
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

//����ͼƬ���б�
BOOL CRecToolDlg::InsertPic(const RECOED_FILE_INFO &recordFile)
{
// 	ITEMDATA * pData = new ITEMDATA;
// 	pData->strFileName = recordFile.csPicPath;
	HANDLE hFileHandle = NULL;
	//���
	int nIndex = m_listRecords.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	m_listRecords.InsertItem(nIndex, strIndex);
	//m_listRecords.SetItemData(nIndex, (DWORD)pData);
	//�ļ���
	CString strFileName = recordFile.csPicPath.Mid(recordFile.csPicPath.ReverseFind('\\') + 1);
	//m_listRecords.SetItemText(nIndex, 1, recordFile.csPicPath);
	m_listRecords.SetItemText(nIndex, 1, strFileName);
	//¼��ʱ��
	CString strFileDuration = _T("--");
	m_listRecords.SetItemText(nIndex, 2, strFileDuration);
	//�ļ���С
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

	//��¼������ļ�
	m_PicFileArray.Add(recordFile);
	m_ArrayFileList.Add(recordFile.csPicPath);

	return TRUE;
}

BOOL CRecToolDlg::InsertRec(const RECOED_FILE_INFO &recordFile)
{
// 	ITEMDATA * pData = new ITEMDATA;
// 	pData->strFileName = recordFile.strFileName;
	//���
	int nIndex = m_listRecords.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	m_listRecords.InsertItem(nIndex, strIndex);
	//m_listRecords.SetItemData(nIndex, (DWORD)pData);
	//�ļ���
	//int nPos = recordFile.strFileName.ReverseFind('\\');
	CString strFileName = recordFile.strFileName.Mid(recordFile.strFileName.ReverseFind('\\') + 1);
	m_listRecords.SetItemText(nIndex, 1, strFileName);
	//m_listRecords.SetItemText(nIndex, 1, recordFile.strFileName);
	//¼��ʱ��
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

	//�ļ���С
	CString strFileSize;
	strFileSize.Format(_T("%.2fM"),  (double)recordFile.lFileSize / 1024 / 1024);
	/*double dFileSize = (double)recordFile.lFileSize / 1024 / 1024;*/
	m_listRecords.SetItemText(nIndex, 3, strFileSize);

	//��ӵ�¼���б�
	m_RecFileArray.Add(recordFile);
	m_ArrayFileList.Add(recordFile.strFileName);

	return TRUE;
}
BOOL CRecToolDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (MSG_MULTIBTN_CLICK == wParam)  
	{
		//��ť��Ϣ�Ľṹ��
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
		if (multiBtnInfo->nNumber == 0)	// �޸��ļ�����ť
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
				if (strFileName.Find(_T("mp4")) > 0)   //��Ƶ
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
		else if (multiBtnInfo->nNumber == 1)		//�طŰ�ť
		{
			if (strFileName.Find(_T("mp4")) > 0)   //��Ƶ
			{
				m_lpPlaybackCtrlDlg->ShowWindow(SW_SHOW);
				m_lpPlaybackCtrlDlg->StartPlayBack(0, strFileName);
			}
			else		//ͼƬ
			{
				TCHAR szSystemDir[MAX_PATH];
				::GetSystemDirectory(szSystemDir, MAX_PATH);

				CString strRundll;
				strRundll.Format(_T("%s\\rundll32.exe"), szSystemDir);

				//�滻·����������ļ�ʧ��
				CString strTemp = strFileName;
				strTemp.Replace(_T("\\\\"), _T("\\"));

				CString strParam;
				strParam.Format(_T("%s\\shimgvw.dll imageview_fullscreen %s"), szSystemDir, strTemp);
				ShellExecute(NULL, _T("Open"), strRundll.GetBuffer(), strParam, NULL, SW_SHOW);
			}

		}
		else if (multiBtnInfo->nNumber == 2)  //ɾ����ť
		{
			if (IDYES == FMessageBox::Show(_T("ȷ��ɾ�����ļ���\r\nע�⣺�����ļ����ᱻɾ����"), _T("ɾ��"), MB_YESNO | MB_ICONQUESTION))
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

		//�ͷ��ڴ棬������TRUE
		delete multiBtnInfo;
		return TRUE;
	}

	return CFDialog::OnCommand(wParam, lParam);
}

BOOL CRecToolDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CRect rcClient;
    GetClientRect(rcClient);
    pDC->FillSolidRect(rcClient, RGB(0x1F, 0x1F, 0x1F));


    return CFDialog::OnEraseBkgnd(pDC);
}
