// DownSuccessDlg.cpp : ʵ���ļ�
//
//lint -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownSuccessDlg.h"
#include "../ui/uiCtrl/BCMenu.h"

// CDownSuccessDlg �Ի���

IMPLEMENT_DYNAMIC(CDownSuccessDlg, CDialog)

CDownSuccessDlg::CDownSuccessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownSuccessDlg::IDD, pParent)
{
	m_bMove = FALSE;

	m_hBitmap = NULL;
	m_nBitmapWidth = 0;
	m_nBitmapHeight = 0;
}

CDownSuccessDlg::~CDownSuccessDlg()
{
	try
	{
		if (m_hBitmap)
		{
			DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}

		if (m_dcMem.m_hDC != NULL)
		{
			m_dcMem.DeleteDC();
		}
	}
	catch (...)
	{

	}
}

void CDownSuccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OPENFILE, m_btnOpenFile);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_LIST_DOWNLOADSUC, m_listDownloadSuc);
	DDX_Control(pDX, IDC_BUTTON_OPEN_DIR, m_btnOpenDir);
}


BEGIN_MESSAGE_MAP(CDownSuccessDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CDownSuccessDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDownSuccessDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDownSuccessDlg::OnBnClickedButtonClear)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DIR, &CDownSuccessDlg::OnBnClickedButtonOpenDir)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DOWNLOADSUC, &CDownSuccessDlg::OnNMRClickListDownloadsuc)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOWNLOADSUC, &CDownSuccessDlg::OnLvnItemchangedListDownloadsuc)
END_MESSAGE_MAP()


// CDownSuccessDlg ��Ϣ�������

BOOL CDownSuccessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrl();

	m_bMove = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDownSuccessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}

}

void CDownSuccessDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, UI_WND_BKGND);

	dc.StretchBlt(m_rcTitle.left, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height(), &m_dcMem, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);
}

//�����ӿؼ�
void CDownSuccessDlg::InitCtrl()
{
	//button;
	m_btnOpenFile.Load(IDB_PNG_STARTDOWNLOAD);
	m_btnOpenFile.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);
	m_btnOpenFile.ShowWindow(SW_HIDE);

	m_btnDelete.Load(IDB_PNG_DEL_TASK);
	m_btnDelete.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	m_btnClear.Load(IDB_PNG_ONBTN_CLEAR);
	m_btnClear.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	m_btnOpenDir.Load(IDB_PNG_ONBTN_FOLDER);
	m_btnOpenDir.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	//list
	m_listDownloadSuc.SetExtendedStyle(m_listDownloadSuc.GetExtendedStyle()
		| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_listDownloadSuc.InsertColumn(0, _T("���"), LVCFMT_LEFT, 40);
	m_listDownloadSuc.InsertColumn(1, _T("���ؽ���"), LVCFMT_LEFT, 60);
	m_listDownloadSuc.InsertColumn(2, _T("��ص�"), LVCFMT_LEFT, 120);
	m_listDownloadSuc.InsertColumn(3, _T("��ʼʱ��"), LVCFMT_LEFT, 125);
	m_listDownloadSuc.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 125);
	m_listDownloadSuc.InsertColumn(5, _T("�ļ���"), LVCFMT_LEFT, 160);
	m_listDownloadSuc.InsertColumn(6, _T("��С"), LVCFMT_LEFT, 100);
	m_listDownloadSuc.InsertColumn(7, _T("���ط�ʽ"), LVCFMT_LEFT, 80);
	m_listDownloadSuc.InsertColumn(8, _T("����Դ"), LVCFMT_LEFT, 80);
	m_listDownloadSuc.InsertColumn(9, _T("�洢·��"), LVCFMT_LEFT, 260);
	//m_listDownloadSuc.InsertColumn(10, _T("��ע"), LVCFMT_LEFT, 120);

	Bitmap * pBitmap = ::LoadBitmapFromResource(IDB_PNG_TITLE);
	pBitmap->GetHBITMAP(Color(0, 255, 0, 255), &m_hBitmap);
	m_nBitmapWidth = pBitmap->GetWidth();
	m_nBitmapHeight = pBitmap->GetHeight();
}

//�ƶ��ؼ�
void CDownSuccessDlg::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	m_rcTitle.left = rcClient.left;
	m_rcTitle.top = rcClient.top + 1;
	m_rcTitle.right = rcClient.right;
	m_rcTitle.bottom = m_rcTitle.top + 28;

	int nBtnWidth = 82;
	int nBtnHeight = 28;
	int nSpace = 8;

	int nTop = 1;
	int nLeft = 25;
	//m_btnOpenFile.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	//nLeft = nLeft + nBtnWidth + nSpace;
	m_btnDelete.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnOpenDir.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnClear.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	

	CRect rcList(rcClient);
	rcList.top = m_rcTitle.bottom;
	m_listDownloadSuc.MoveWindow(rcList);

}

BOOL CDownSuccessDlg::OnEraseBkgnd(CDC* pDC)
{
    
	if (m_dcMem.m_hDC == NULL)
	{
		m_dcMem.CreateCompatibleDC(pDC);
		m_dcMem.SelectObject(m_hBitmap);
	}

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

/** @fn TransparentBk
*   @͸����ͼ 
*   @param[in] pDc: �ӿؼ�pDc
*   @param[in] pWnd: �ӿؼ���� 
*   @return NULL
*/
void CDownSuccessDlg::TransparentBk(CDC * pDc, CWnd * pWnd)
{
	CRect rcWnd;
	pWnd->GetClientRect(&rcWnd);

	pDc->StretchBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &m_dcMem, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);
}


void CDownSuccessDlg::OnBnClickedButtonOpenfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = NULL;
	pos = m_listDownloadSuc.GetFirstSelectedItemPosition();
	int nItem = m_listDownloadSuc.GetNextSelectedItem(pos);
	if (-1 == nItem)
	{
		return;
	}
	//֪ͨ
	CString strFilePath = m_listDownloadSuc.GetItemText(nItem, 9);
	CString* pSend = new CString(strFilePath);
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADFILE_PLAY, 0, (LPARAM)pSend);
}

void CDownSuccessDlg::OnBnClickedButtonDelete()
{
	POSITION pos = NULL;
	while (pos = m_listDownloadSuc.GetFirstSelectedItemPosition())
	{
		int nItem = m_listDownloadSuc.GetNextSelectedItem(pos);
		LONG lDownloadSeq = m_listDownloadSuc.GetItemData(nItem);
		//ɾ��map���Ӧ����
		std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.find(lDownloadSeq);
		m_mapDownInfo.erase(itor);
		//ɾ��list������
		m_listDownloadSuc.DeleteItem(nItem);
		//֪ͨ
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_DEL, lDownloadSeq, IDD_DIALOG_DOWNSUCCESS);
	}

	//�������
	int nItem = 0;
	CString strIndex;
	int nCount = m_listDownloadSuc.GetItemCount();
	while (nItem < nCount)
	{
		strIndex.Format(_T("%d"), nItem+1);
	    m_listDownloadSuc.SetItemText(nItem, 0, strIndex);

		nItem++;
	}

}

void CDownSuccessDlg::OnBnClickedButtonClear()
{
	m_listDownloadSuc.DeleteAllItems();
	std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.begin();
	while (itor != m_mapDownInfo.end())
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_DEL, itor->first, IDD_DIALOG_DOWNSUCCESS);
		itor++;
	}
	m_mapDownInfo.clear();
}

//����һ����¼
BOOL CDownSuccessDlg::AddDownloadInfo(LONG lSeq, DOWNLOAD_INFO& stDownloadInfo)
{
	if (m_mapDownInfo.find(lSeq) != m_mapDownInfo.end())
	{
		return FALSE;
	}

	m_mapDownInfo[lSeq] = stDownloadInfo;

	int nCount = m_listDownloadSuc.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nCount+1);
	int nItem = m_listDownloadSuc.InsertItem(nCount, strIndex);
	CString strCameraName, strStartTime, strStopTime, strFileName, strFileSize, strDownloadType, strSavePath;
	wchar_t* wszCameraName = NULL;
	wchar_t* wszFileName = NULL;
	wchar_t* wszSavePath = NULL;
	int nLength;
	CP936_A2W(stDownloadInfo.szCameraName, strlen(stDownloadInfo.szCameraName)+1, wszCameraName, nLength);
	strCameraName.Format(_T("%s"), wszCameraName);
	CodeSet_Free(wszCameraName);
	PB_TIME startTime, stopTime;
	if (stDownloadInfo.bByTime)
	{
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadTimeParam.szFileName, strlen(stDownloadInfo.unDownloadParam.downloadTimeParam.szFileName)+1, 
			wszFileName, nLength);
		strFileName.Format(_T("%s"), wszFileName);
		CodeSet_Free((void*)wszFileName);
		//��ȡ�����ļ�ʵ�ʴ�С
		{
			CFile cfile;  
			CFileStatus rStatus;
			wchar_t* wszSavePath = NULL;
			int nLength = 0;
			CP936_A2W(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile,
				strlen(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile)+1, 
				wszSavePath, nLength);
			if(CFile::GetStatus(wszSavePath,rStatus))
			{
				CodeSet_Free(wszSavePath);
				strFileSize = GetFileSize(rStatus.m_size);
			}
		}
		strFileSize = GetFileSize(stDownloadInfo.unDownloadParam.downloadTimeParam.dwFileSize);
		startTime = stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime;
		stopTime = stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime;
		strDownloadType = _T("ʱ������");
		nLength = strlen(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile);
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile, strlen(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile)+1, 
			wszSavePath, nLength);
		strSavePath.Format(_T("%s"), wszSavePath);
		CodeSet_Free((void*)wszSavePath);
	}
	else
	{
		startTime = stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime;
		stopTime = stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime;
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.szFileName, strlen(stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.szFileName)+1, 
			wszFileName, nLength);
		strFileName.Format(_T("%s"), wszFileName);
		CodeSet_Free((void*)wszFileName);
		//��ȡ�����ļ�ʵ�ʴ�С
		{
			CFile cfile;  
			CFileStatus rStatus;
			wchar_t* wszSavePath = NULL;
			int nLength = 0;
			CP936_A2W(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile,
				strlen(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile)+1, 
				wszSavePath, nLength);
			if (CFile::GetStatus(wszSavePath,rStatus))
			{
				CodeSet_Free(wszSavePath);
				strFileSize = GetFileSize(rStatus.m_size);
			}
		}
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile, strlen(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile)+1, 
			wszSavePath, nLength);
		strSavePath.Format(_T("%s"), wszSavePath);
		CodeSet_Free((void*)wszSavePath);
		strDownloadType = _T("�ļ�����");
	}
	strStartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), startTime.dwYear, startTime.dwMonth, startTime.dwDay, startTime.dwHour, startTime.dwMinute, startTime.dwSecond);
	strStopTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), stopTime.dwYear, stopTime.dwMonth, stopTime.dwDay, stopTime.dwHour, stopTime.dwMinute, stopTime.dwSecond);
	m_listDownloadSuc.SetItemText(nItem, 1, _T("�������"));
	m_listDownloadSuc.SetItemText(nItem, 2, strCameraName);
	m_listDownloadSuc.SetItemText(nItem, 3, strStartTime);
	m_listDownloadSuc.SetItemText(nItem, 4, strStopTime);
	m_listDownloadSuc.SetItemText(nItem, 5, strFileName);
	m_listDownloadSuc.SetItemText(nItem, 6, strFileSize);
	m_listDownloadSuc.SetItemText(nItem, 7, strDownloadType);
	CString strDataSrc;
	switch (stDownloadInfo.emDownType)
	{
	case PB_TYPE_DVR:
		strDataSrc = _T("DVR");
		break;
	case PB_TYPE_CVR:
		strDataSrc = _T("CVR");
		break;
	case PB_TYPE_CLOUD:
		strDataSrc = _T("�ƴ洢");
		break;
    case PB_TYPE_KMS:
        strDataSrc = _T("KMS");
        break;
	case PB_TYPE_OBJECTCLOUD:
		strDataSrc = _T("����洢");
		break;
	default:
		break;
	}
	m_listDownloadSuc.SetItemText(nItem, 8, strDataSrc);
	m_listDownloadSuc.SetItemText(nItem, 9, strSavePath);
	m_listDownloadSuc.SetItemData(nItem, lSeq);

	// ����Ӧ���.
	int nColumnWidth = m_listDownloadSuc.GetColumnWidth(9);
	m_listDownloadSuc.SetColumnWidth(9, LVSCW_AUTOSIZE_USEHEADER);
	int nHeaderWidth = m_listDownloadSuc.GetColumnWidth(9); 
	m_listDownloadSuc.SetColumnWidth(9, max(nColumnWidth, nHeaderWidth));

	return TRUE;
}

void CDownSuccessDlg::OnBnClickedButtonOpenDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = NULL;
	pos = m_listDownloadSuc.GetFirstSelectedItemPosition();
	int nItem = m_listDownloadSuc.GetNextSelectedItem(pos);

	TCHAR szDownloadPath[MAX_PATH];
	MByteToWChar(theApp.m_stPlaybackParam.szDownloadPath, szDownloadPath, MAX_PATH);
	if (-1 == nItem)
	{
		// ��Ĭ��Ŀ¼.
		ShellExecute(NULL, _T("open"), szDownloadPath, NULL, NULL, SW_SHOW);
		return;
	}
	// �ļ�·��.
	CString strFilePath = m_listDownloadSuc.GetItemText(nItem, 9);
	if (strFilePath.IsEmpty())
	{
		// ��Ĭ��Ŀ¼.
		ShellExecute(NULL, _T("open"), szDownloadPath, NULL, NULL, SW_SHOW);
		return;
	}

	CString strParam;
	strParam.Format(_T("/select,%s"),  (LPCTSTR)strFilePath);

	CString strMsg;
	strMsg.Format(_T("@z Open Dir: %s"), (LPCTSTR)strParam);
	OutputDebugString(strMsg);

	// ��ָ���ļ�Ŀ¼��ѡ���ļ�.
	ShellExecute(NULL, _T("open"), _T("Explorer.exe"), strParam.GetBuffer(0), NULL, SW_SHOW);
}

void CDownSuccessDlg::OnNMRClickListDownloadsuc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BCMenu bcMenu;
	bcMenu.CreatePopupMenu();
	//bcMenu.AppendMenu(MF_STRING, WM_MENU_OPENFILE, _T("���ļ�"), 11);
	bcMenu.AppendMenu(MF_STRING, WM_MENU_DELETETASK, _T("ɾ������"), 11);
	bcMenu.AppendMenu(MF_STRING, WM_MENU_OPENDIR, _T("��Ŀ¼"), 11);

	POINT curPos;
	GetCursorPos(&curPos);
	int nCmd = ( int ) bcMenu.TrackPopupMenu( \
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD, \
		curPos.x, curPos.y, this);

	switch (nCmd)
	{
// 	case WM_MENU_OPENFILE:
// 		{
// 			OnBnClickedButtonOpenfile();
// 		}
// 		break;

	case WM_MENU_DELETETASK:
		{
			OnBnClickedButtonDelete();
		}
		break;

	case WM_MENU_OPENDIR:
		{
			OnBnClickedButtonOpenDir();
		}
		break;

	default:
		return;
	}


	*pResult = 0;
}

void CDownSuccessDlg::OnLvnItemchangedListDownloadsuc(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    if (m_listDownloadSuc.GetSelectedCount() <= 0)
    {
        m_btnOpenFile.EnableWindow(FALSE);
        m_btnDelete.EnableWindow(FALSE);
    }
    else
    {
        m_btnOpenFile.EnableWindow(TRUE);
        m_btnDelete.EnableWindow(TRUE);
    }
    *pResult = 0;
}