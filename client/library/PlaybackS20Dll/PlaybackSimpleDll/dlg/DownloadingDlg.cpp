// DownloadingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadingDlg.h"


// CDownloadingDlg �Ի���
void CDownloadingDlg::DownloadThreadFun(LPVOID lpvoid)
{
	CDownloadingDlg* pInstance = (CDownloadingDlg* )lpvoid;
	if (pInstance == NULL)
	{
		return;
	}
	//�������ؽ��Ȼ�ȡ��ʱ��
	pInstance->SetTimer(TIMER_ID_GETDOWNLOADPOS, 1000, NULL);

	while (pInstance->m_bWorking)
	{
		if (pInstance->m_bDownloading)
		{
			Sleep(1000);
			continue;
		}
		if (pInstance->m_bWorking == FALSE)
		{
			break;
		}
        EnterCriticalSection(&(pInstance->m_csDownlist));
        std::map<LONG, DOWNLOAD_INFO> mapDownInfo = pInstance->m_mapDownInfo;
        LeaveCriticalSection(&(pInstance->m_csDownlist));
        std::map<LONG, DOWNLOAD_INFO>::iterator itor = mapDownInfo.begin();
        while (itor != mapDownInfo.end())
        {
            EnterCriticalSection(&(pInstance->m_csDownlist));
			if (itor->second.bDownload)
			{
				itor++;
				continue;
			}
			pInstance->m_lDowningSeq = itor->first;

			itor->second.bDownload = TRUE;
            LeaveCriticalSection(&(pInstance->m_csDownlist));

            EnterCriticalSection(&(pInstance->m_csDownlist));
            pInstance->m_mapDownInfo[itor->first].bDownload = TRUE;
            LeaveCriticalSection(&(pInstance->m_csDownlist));
			if (!pInstance->DownloadSingleTask(itor->first, itor->second))
			{
				pInstance->m_lDowningSeq = 0;
                EnterCriticalSection(&(pInstance->m_csDownlist));
                pInstance->m_mapDownInfo.erase(pInstance->m_mapDownInfo.find(itor->first));
                LeaveCriticalSection(&(pInstance->m_csDownlist));
                itor = mapDownInfo.erase(itor);
				continue;
			}
			else
			{
				
				break;
			}
		}
        EnterCriticalSection(&(pInstance->m_csDownlist));
        if (pInstance->m_mapDownInfo.size() <= 0)
        {
			pInstance->m_bWorking = FALSE;
            pInstance->PostMessage(MSG_UPDATEBTN,0,0);
		}
		LeaveCriticalSection(&(pInstance->m_csDownlist));
	}

	//�رն�ʱ��
	pInstance->KillTimer(TIMER_ID_GETDOWNLOADPOS);
	
	if (pInstance->m_hDownloadThread)
	{
		CloseHandle(pInstance->m_hDownloadThread);
		pInstance->m_hDownloadThread = NULL;
	}
}


IMPLEMENT_DYNAMIC(CDownloadingDlg, CDialog)

CDownloadingDlg::CDownloadingDlg(HANDLE hDlg, CWnd* pParent /*=NULL*/)
: CDialog(CDownloadingDlg::IDD, pParent)
,m_listDownloading(31, 1)
{
	m_bMove = FALSE;
	m_hDlg = hDlg;
	m_hBitmap = NULL;
	m_nBitmapWidth = 0;
	m_nBitmapHeight = 0;

	m_lDowningSeq = 0;
	m_bDownloading = FALSE;
	m_hDownloadThread = NULL;
	m_hDownload = NULL;
	m_bWorking = FALSE;
	m_nCamID = -1;
	m_dwRev = 0;
	m_dwTimeBegin = 0;
	m_dwTimeEnd = 0;
	InitializeCriticalSection(&m_csDownlist);
}

CDownloadingDlg::~CDownloadingDlg()
{
	try
	{
		//�˳��߳�
		m_bWorking = FALSE;

		DeleteCriticalSection(&m_csDownlist);

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

	m_hDownloadThread = NULL;
	m_hDownload = NULL;
	m_nCamID = -1;
	m_dwTimeBegin = 0;
	m_dwTimeEnd = 0;
	m_dwRev = 0;
}

void CDownloadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_STARTDOWNLOAD, m_btnStartDownload);
	DDX_Control(pDX, IDC_BUTTON_STOPDOWNLOAD, m_btnStopDownload);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_LIST_DOWNLOADING, m_listDownloading);
}


BEGIN_MESSAGE_MAP(CDownloadingDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_STARTDOWNLOAD, &CDownloadingDlg::OnBnClickedButtonStartdownload)
	ON_BN_CLICKED(IDC_BUTTON_STOPDOWNLOAD, &CDownloadingDlg::OnBnClickedButtonStopdownload)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDownloadingDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDownloadingDlg::OnBnClickedButtonClear)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(MSG_CHECK_SPACE_NOTENOUGH, &CDownloadingDlg::OnMsgCheckSpaceNotEnough)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOWNLOADING, &CDownloadingDlg::OnLvnItemchangedListDownloading)
    ON_MESSAGE(MSG_UPDATEBTN, &CDownloadingDlg::UpdateMenuButtonState)
    ON_MESSAGE(MSG_DOWNLOADTASK_FAIL, &CDownloadingDlg::OnMsgDownloadFailed)
END_MESSAGE_MAP()


// CDownloadingDlg ��Ϣ�������

BOOL CDownloadingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrl();

	m_bMove = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDownloadingDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	StopDownload();
	OutputDebugString(_T("@z stop download"));
}

void CDownloadingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_bMove)
	{
		MoveCtrl();
	}
}

//�����ӿؼ�
void CDownloadingDlg::InitCtrl()
{
	//button;
	m_btnStartDownload.Load(IDB_PNG_STARTDOWNLOAD);
	m_btnStartDownload.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	m_btnStopDownload.Load(IDB_PNG_STOPDOWNLOAD);
	m_btnStopDownload.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	m_btnDelete.Load(IDB_PNG_DEL_TASK);
	m_btnDelete.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	m_btnClear.Load(IDB_PNG_ONBTN_CLEAR);
	m_btnClear.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	//list
	m_listDownloading.SetExtendedStyle(m_listDownloading.GetExtendedStyle()
		| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_listDownloading.InsertColumn(0, _T("���"), LVCFMT_LEFT, 40);
	m_listDownloading.InsertColumn(1, _T("���ؽ���"), LVCFMT_LEFT, 120);
	m_listDownloading.InsertColumn(2, _T("��ص�"), LVCFMT_LEFT, 120);
	m_listDownloading.InsertColumn(3, _T("��ʼʱ��"), LVCFMT_LEFT, 125);
	m_listDownloading.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 125);
	m_listDownloading.InsertColumn(5, _T("�ļ���"), LVCFMT_LEFT, 160);
	m_listDownloading.InsertColumn(6, _T("��С"), LVCFMT_LEFT, 100);
	m_listDownloading.InsertColumn(7, _T("���ط�ʽ"), LVCFMT_LEFT, 80);
	m_listDownloading.InsertColumn(8, _T("����Դ"), LVCFMT_LEFT, 80);
	m_listDownloading.InsertColumn(9, _T("�洢·��"), LVCFMT_LEFT, 260);
	//m_listDownloading.InsertColumn(10, _T("��ע"), LVCFMT_LEFT, 120);

	Bitmap * pBitmap = ::LoadBitmapFromResource(IDB_PNG_TITLE);
	pBitmap->GetHBITMAP(Color(0, 255, 0, 255), &m_hBitmap);
	m_nBitmapWidth = pBitmap->GetWidth();
	m_nBitmapHeight = pBitmap->GetHeight();

}

//�ƶ��ؼ�
void CDownloadingDlg::MoveCtrl()
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
	m_btnStartDownload.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnStopDownload.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnDelete.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnClear.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);

	CRect rcList(rcClient);
	rcList.top = m_rcTitle.bottom;
	m_listDownloading.MoveWindow(rcList);

}


BOOL CDownloadingDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
void CDownloadingDlg::TransparentBk(CDC * pDc, CWnd * pWnd)
{
	CRect rcWnd;
	pWnd->GetClientRect(&rcWnd);

	pDc->StretchBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &m_dcMem, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);
}

void CDownloadingDlg::OnBnClickedButtonStartdownload()
{
	StartDownload();
}

void CDownloadingDlg::OnBnClickedButtonStopdownload()
{
	StopDownload();
}

void CDownloadingDlg::OnBnClickedButtonDelete()
{
	DelSelectInfo();
}

void CDownloadingDlg::OnBnClickedButtonClear()
{
	ClearList();
}

void CDownloadingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, UI_WND_BKGND);

	dc.StretchBlt(m_rcTitle.left, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height(), &m_dcMem, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);
}

//��ʼ����
BOOL CDownloadingDlg::StartDownload()
{
	if (m_bWorking)
	{
		MsgBoxApi_Show(_T("����������"), MSGBOX_TYPE_WARNING);
		return FALSE;
	}
	if (m_mapDownInfo.size() == 0)
	{
		MsgBoxApi_Show(_T("û����������"), MSGBOX_TYPE_WARNING);
		return FALSE;
	}

	m_bDownloading = FALSE;

	//���������߳�
	m_bWorking = TRUE;
	DWORD dwThreadId;
	m_hDownloadThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DownloadThreadFun, (LPVOID)this, 0, &dwThreadId);
	if (m_hDownloadThread == NULL)
	{
		m_bWorking = FALSE;
		MsgBoxApi_Show(_T("���������߳�ʧ��"), MSGBOX_TYPE_WARNING);
		return FALSE;
	}
    UpdateMenuButtonState(0,0);
	return TRUE;
}

//�¼�֪ͨ����
BOOL CDownloadingDlg::NotifyStartDownload()
{
	if (m_bWorking)
	{
		return FALSE;
	}
	if (m_mapDownInfo.size() == 0)
	{
		return FALSE;
	}

	m_bDownloading = FALSE;

	//���������߳�
	m_bWorking = TRUE;
	DWORD dwThreadId;
	m_hDownloadThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DownloadThreadFun, (LPVOID)this, 0, &dwThreadId);
	if (m_hDownloadThread == NULL)
	{
		m_bWorking = FALSE;
		MsgBoxApi_Show(_T("���������߳�ʧ��"), MSGBOX_TYPE_WARNING);
		return FALSE;
	}
    UpdateMenuButtonState(0,0);
	return TRUE;
}

//���ص�������
BOOL CDownloadingDlg::DownloadSingleTask(LONG lSeq, DOWNLOAD_INFO& stDownloadInfo)
{
	//��ȡ��¼��Ϣ
	/*std::string strCamName;
	if (!theApp.GetLoginInfo(stDownloadInfo.nCameraID, stDownloadInfo.emDownType, stLoginInfo, nChannel, strCamName))
	{
		int nCount = m_listDownloading.GetItemCount();
		for (int i=0; i<nCount; i++)
		{
			LONG lDownSeq = m_listDownloading.GetItemData(i);
			if (lDownSeq == m_lDowningSeq)
			{
				m_listDownloading.DeleteItem(i);
				::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_FAIL, lDownSeq, 0);
				return FALSE;
			}
		}

		return FALSE;
	}*/

	//����
	if (stDownloadInfo.bByTime)
	{
		m_hDownload = m_opeDownload.DownloadTime(
			m_hDlg, 
			stDownloadInfo.stLoginInfo, 
			stDownloadInfo.unDownloadParam.downloadTimeParam, 
			stDownloadInfo.nCameraID,
			stDownloadInfo.dwRev);
		m_nCamID = stDownloadInfo.nCameraID;
		m_dwRev = stDownloadInfo.dwRev;
		// ��ʼʱ��.
		CTime ctBegin(
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime.dwYear,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime.dwMonth,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime.dwDay,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime.dwHour,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime.dwMinute,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime.dwSecond);
		m_dwTimeBegin = ctBegin.GetTime();
		// ����ʱ��.
		CTime ctEnd(
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime.dwYear,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime.dwMonth,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime.dwDay,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime.dwHour,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime.dwMinute,
			stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime.dwSecond);
		m_dwTimeEnd = ctEnd.GetTime();
	}
	else
	{
		m_hDownload = m_opeDownload.DownloadFile(
			m_hDlg, 
			stDownloadInfo.stLoginInfo, 
			stDownloadInfo.unDownloadParam.downloadFileParam, 
			stDownloadInfo.nCameraID,
			stDownloadInfo.dwRev);
		m_nCamID = stDownloadInfo.nCameraID;
		m_dwRev = stDownloadInfo.dwRev;
		// ��ʼʱ��.
		CTime ctBegin(
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime.dwYear,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime.dwMonth,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime.dwDay,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime.dwHour,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime.dwMinute,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime.dwSecond);
		m_dwTimeBegin = ctBegin.GetTime();
		// ����ʱ��.
		CTime ctEnd(
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime.dwYear,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime.dwMonth,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime.dwDay,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime.dwHour,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime.dwMinute,
			stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime.dwSecond);
		m_dwTimeEnd = ctEnd.GetTime();
	}
	if (m_hDownload == NULL)
	{
        PostMessage(MSG_DOWNLOADTASK_FAIL, (WPARAM)m_lDowningSeq ,0);
        DWORD dwError = m_opeDownload.GetSDKLastError();
        ::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_FAIL, (WPARAM)m_lDowningSeq, (LPARAM)dwError);
        return FALSE;
	}

	m_bDownloading = TRUE;

	return TRUE;
}

//ֹͣ����
void CDownloadingDlg::StopDownload()
{
	if (m_hDownload)
	{
		m_opeDownload.StopDownload(m_hDlg, m_hDownload, m_nCamID, m_dwRev, m_dwTimeBegin, m_dwTimeEnd, FALSE);
		m_hDownload = NULL;
		m_nCamID = -1;
		m_dwTimeBegin = 0;
		m_dwTimeEnd = 0;
		m_dwRev = 0;
	}

	//��ǰ�������ص��̱߳�־λ����
	EnterCriticalSection(&m_csDownlist);
	std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.find(m_lDowningSeq);
	if (itor != m_mapDownInfo.end())
	{
		itor->second.bDownload = FALSE;

	}
    LeaveCriticalSection(&m_csDownlist);

	int nCount = m_listDownloading.GetItemCount();
	for (int i=0; i<nCount; i++)
	{
		LONG lDownSeq = m_listDownloading.GetItemData(i);
		if (lDownSeq == m_lDowningSeq)
		{
			m_listDownloading.SetItemText(i, 1, _T(""));
			break;
		}
	}

	//ֹͣ�ط��߳�, ���߳���ֹͣ��ʱ���ͽ����߳̾��
	m_bWorking = FALSE;
	m_bDownloading = FALSE;
	WaitForSingleObject(m_hDownloadThread, INFINITE);
	m_lDowningSeq = 0;
    PostMessage(MSG_UPDATEBTN,0,0);
}


//����һ����¼
BOOL CDownloadingDlg::AddDownloadInfo(LONG lSeq, DOWNLOAD_INFO &stDownloadInfo)
{
	EnterCriticalSection(&m_csDownlist);
	if (m_mapDownInfo.find(lSeq) != m_mapDownInfo.end())
	{
		LeaveCriticalSection(&m_csDownlist);
		return FALSE;
	}
    LeaveCriticalSection(&m_csDownlist);

	int nCount = m_listDownloading.GetItemCount();
	CString strIndex;
	// ����������������.
	for (int i = 0; i < nCount; i++)
	{
		strIndex.Format(_T("%d"), i+1);
		m_listDownloading.SetItemText(i, 0, strIndex);
	}
	
	strIndex.Format(_T("%d"), nCount+1);
	int nItem = m_listDownloading.InsertItem(nCount, strIndex);
    PostMessage(MSG_UPDATEBTN,0,0);
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
		strFileName = _T("��");

		strFileSize = GetFileSize(stDownloadInfo.unDownloadParam.downloadTimeParam.dwFileSize);
		startTime = stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime;
		stopTime = stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime;
		strDownloadType = _T("ʱ������");
		nLength = strlen(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile);
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile, strlen(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile)+1, 
			wszSavePath, nLength);
		strSavePath.Format(_T("%s"), wszSavePath);
		strFileName = strSavePath.Mid(strSavePath.ReverseFind('\\') + 1); 
		sprintf_s(stDownloadInfo.unDownloadParam.downloadTimeParam.szFileName, sizeof(stDownloadInfo.unDownloadParam.downloadTimeParam.szFileName),
			CStringW2A(strFileName).GetBuffer());
		CodeSet_Free((void*)wszSavePath);
		sprintf_s(stDownloadInfo.unDownloadParam.downloadTimeParam.szFilePath, sizeof(stDownloadInfo.unDownloadParam.downloadTimeParam.szFilePath),
			CStringW2A(strSavePath).GetBuffer());
	}
	else
	{
		startTime = stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stStartTime;
		stopTime = stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.stEndTime;
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.szFileName, strlen(stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.szFileName)+1, 
			wszFileName, nLength);
		strFileName.Format(_T("%s"), wszFileName);
		CodeSet_Free((void*)wszFileName);
		strFileSize = GetFileSize(stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.dwFileSize);
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile, strlen(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile)+1, 
			wszSavePath, nLength);
		strSavePath.Format(_T("%s"), wszSavePath);
		CodeSet_Free((void*)wszSavePath);
		strDownloadType = _T("�ļ�����");
	}
	strStartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), startTime.dwYear, startTime.dwMonth, startTime.dwDay, startTime.dwHour, startTime.dwMinute, startTime.dwSecond);
	strStopTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), stopTime.dwYear, stopTime.dwMonth, stopTime.dwDay, stopTime.dwHour, stopTime.dwMinute, stopTime.dwSecond);
	m_listDownloading.SetItemText(nItem, 2, strCameraName);
	m_listDownloading.SetItemText(nItem, 3, strStartTime);
	m_listDownloading.SetItemText(nItem, 4, strStopTime);
	m_listDownloading.SetItemText(nItem, 5, strFileName);
	m_listDownloading.SetItemText(nItem, 6, strFileSize);
	m_listDownloading.SetItemText(nItem, 7, strDownloadType);
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
	m_listDownloading.SetItemText(nItem, 8, strDataSrc);
	m_listDownloading.SetItemText(nItem, 9, strSavePath);

	m_listDownloading.SetItemData(nItem, lSeq);

	// ����Ӧ���.
	int nColumnWidth = m_listDownloading.GetColumnWidth(9);
	m_listDownloading.SetColumnWidth(9, LVSCW_AUTOSIZE_USEHEADER);
	int nHeaderWidth = m_listDownloading.GetColumnWidth(9); 
	m_listDownloading.SetColumnWidth(9, max(nColumnWidth, nHeaderWidth));
    EnterCriticalSection(&m_csDownlist);
	m_mapDownInfo[lSeq] = stDownloadInfo;
	LeaveCriticalSection(&m_csDownlist);

	return TRUE;
}

//ɾ��ѡ�еļ�¼
void CDownloadingDlg::DelSelectInfo()
{
	POSITION pos = NULL;
	while (pos = m_listDownloading.GetFirstSelectedItemPosition())
	{
		int nItem = m_listDownloading.GetNextSelectedItem(pos);
		LONG lDownloadSeq = m_listDownloading.GetItemData(nItem);
		if (m_lDowningSeq == lDownloadSeq)
		{
			//ֹͣ��ǰ����
			m_opeDownload.StopDownload(m_hDlg, m_hDownload, m_nCamID, m_dwRev, m_dwTimeBegin, m_dwTimeEnd);
			m_hDownload = NULL;
			m_nCamID = -1;
			m_dwTimeBegin = 0;
			m_dwTimeEnd = 0;
			m_dwRev = 0;
			m_bDownloading = FALSE;
			m_lDowningSeq = 0;
		}
		//ɾ��map���Ӧ����
        EnterCriticalSection(&m_csDownlist);
		std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.find(lDownloadSeq);
		m_mapDownInfo.erase(itor);
        LeaveCriticalSection(&m_csDownlist);
		//ɾ��list������
		m_listDownloading.DeleteItem(nItem);
        PostMessage(MSG_UPDATEBTN,0,0);
		//֪ͨ
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_DEL, lDownloadSeq, IDD_DIALOG_DOWNLOADING);
	}

    //�������
    ReSetListIndex();
}

void CDownloadingDlg::ReSetListIndex()
{
    int nItem = 0;
    CString strIndex;
    int nCount = m_listDownloading.GetItemCount();
    while (nItem < nCount)
    {
        strIndex.Format(_T("%d"), nItem+1);
        m_listDownloading.SetItemText(nItem, 0, strIndex);

        nItem++;
    }
}

//����б�
void CDownloadingDlg::ClearList()
{
	//ֹͣ��ǰ����
	StopDownload();
	m_listDownloading.DeleteAllItems();
    PostMessage(MSG_UPDATEBTN,0,0);
    EnterCriticalSection(&m_csDownlist);
	std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.begin();
	while (itor != m_mapDownInfo.end())
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_DEL, itor->first, IDD_DIALOG_DOWNLOADING);
		itor++;
	}
	m_mapDownInfo.clear();
	LeaveCriticalSection(&m_csDownlist);
}

//lint -e524
void CDownloadingDlg::OnTimer(UINT_PTR nIDEvent)
{

	if (TIMER_ID_GETDOWNLOADPOS == nIDEvent)
	{
		if (m_hDownload == NULL)
		{
			return;
		}
		// �����̿ռ�.
		CString strFilePath = _T("");
		int nCount = m_listDownloading.GetItemCount();
		for (int i=0; i<nCount; i++)
		{
			LONG lDownSeq = m_listDownloading.GetItemData(i);
			if (lDownSeq == m_lDowningSeq)
			{
				// �ļ�·��.
				strFilePath = m_listDownloading.GetItemText(i, 9);
			}
		}

		if (!strFilePath.IsEmpty())
		{
			TRACE("@z checking download space");
			if (!CheckFilePathSpace(strFilePath))
			{
				// ���̿ռ䲻��.
				StopDownload();
				PostMessage(MSG_CHECK_SPACE_NOTENOUGH, 0, 0);
			}
		}
		//��ȡ���ؽ���
		int nPos = 0;
		if (!m_opeDownload.GetDownloadPos(m_hDownload, nPos))
		{
			return;
		}
		//�����쳣
		if (nPos < 0 || nPos > 100)
		{
			//ֹͣ��ǰ����
			PLAYBACKDLL_ERROR(_T("���ؽ����쳣,ֹͣ����, nPos:%d"), nPos);
			m_opeDownload.StopDownload(m_hDlg, m_hDownload, m_nCamID, m_dwRev, m_dwTimeBegin, m_dwTimeEnd);
			m_hDownload = NULL;
			m_nCamID = -1;
			m_dwTimeBegin = 0;
			m_dwTimeEnd = 0;
			m_dwRev = 0;
			m_bDownloading = FALSE;
			for (int i=0; i<nCount; i++)
			{
				LONG lDownSeq = m_listDownloading.GetItemData(i);
				if (lDownSeq == m_lDowningSeq)
				{
					m_listDownloading.DeleteItem(i);
                    ReSetListIndex();
                    EnterCriticalSection(&m_csDownlist);
					m_mapDownInfo.erase(lDownSeq);
                    LeaveCriticalSection(&m_csDownlist);
                    PostMessage(MSG_UPDATEBTN,0,0);
					::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_FAIL, lDownSeq, 1);
					break;
				}
			}
			m_lDowningSeq = 0;
		}
		//�������
		else if (nPos == 100)
		{
			m_opeDownload.StopDownload(m_hDlg, m_hDownload, m_nCamID, m_dwRev, m_dwTimeBegin, m_dwTimeEnd);
			m_hDownload = NULL;
			m_nCamID = -1;
			m_dwTimeBegin = 0;
			m_dwTimeEnd = 0;
			m_dwRev = 0;
			m_bDownloading = FALSE;
			for (int i=0; i<nCount; i++)
			{
				LONG lDownSeq = m_listDownloading.GetItemData(i);
				if (lDownSeq == m_lDowningSeq)
				{
					m_listDownloading.DeleteItem(i);
                    ReSetListIndex();
                    EnterCriticalSection(&m_csDownlist);
					m_mapDownInfo.erase(lDownSeq);
                    LeaveCriticalSection(&m_csDownlist);
                    PostMessage(MSG_UPDATEBTN,0,0);
					::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_SUC, lDownSeq, 0);
					break;
				}
			}
			m_lDowningSeq = 0;
		}
		else
		{
			for (int i=0; i<nCount; i++)
			{
				LONG lDownSeq = m_listDownloading.GetItemData(i);
				if (lDownSeq == m_lDowningSeq)
				{
                    EnterCriticalSection(&m_csDownlist);
                    DOWNLOAD_INFO downInfo = m_mapDownInfo[lDownSeq];
                    LeaveCriticalSection(&m_csDownlist);
					if (downInfo.bByTime && 0 == nPos)
					{
						//��ȡ��ǰ���ص��ļ��Ĵ�С
						CFile cfile;  
						CFileStatus rStatus;
						wchar_t* wszSavePath = NULL;
						int nLength = 0;
						CP936_A2W(downInfo.unDownloadParam.downloadTimeParam.szFilePath,
							strlen(downInfo.unDownloadParam.downloadTimeParam.szFilePath)+1, 
							wszSavePath, nLength);
						CFile::GetStatus(wszSavePath,rStatus);
						CodeSet_Free(wszSavePath);
						DWORD fileSize = (DWORD)rStatus.m_size;
						DWORD dwFilePos = (double)fileSize*100.0 / downInfo.unDownloadParam.downloadTimeParam.dwFileSize;
						if (dwFilePos >= 100)
						{
							dwFilePos = 100;
						}
						nPos = dwFilePos;
					}
					CString strPos;
					strPos.Format(_T("%d%%"), nPos);
					m_listDownloading.SetItemText(i, 1, strPos);
					break;
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}
//lint +e524

/**   @fn          GetPlaybackInfo
 *    @brief   	   ��ȡ����״̬��Ϣ.
 *    @param[in]   stData:״̬��Ϣ.
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CDownloadingDlg::GetPlaybackInfo(PlaybackStatData& stData)
{
	if (stData.nCamID == m_nCamID  &&  stData.dwRev == m_dwRev  &&  m_bDownloading)
	{
		stData.bIsDownloading = TRUE;
	}
	else
	{
		stData.bIsDownloading = FALSE;
	}

	return TRUE;
}

/**   @fn          OnMsgCheckSpaceNotEnough
 *    @brief   	   ���̿ռ䲻����Ӧ��Ϣ.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
HRESULT CDownloadingDlg::OnMsgCheckSpaceNotEnough(WPARAM wParam, LPARAM lParam)
{	
	MsgBoxApi_Show(_T("���̿ռ䲻��,����ʧ��!"), MSGBOX_TYPE_WARNING);

	return 0;
}

LRESULT CDownloadingDlg::UpdateMenuButtonState(WPARAM wParam, LPARAM lParam)
{
    if (m_listDownloading.GetItemCount() <= 0)
    {
        m_btnDelete.EnableWindow(FALSE);
        m_btnStartDownload.EnableWindow(FALSE);
        m_btnStopDownload.EnableWindow(FALSE);
        return 0;
    }

    if (m_listDownloading.GetSelectedCount() <= 0)
    {
        m_btnDelete.EnableWindow(FALSE);
    }
    else
    {
        m_btnDelete.EnableWindow(TRUE);
    }

    if (m_bWorking)
    {
        m_btnStartDownload.EnableWindow(FALSE);
        m_btnStopDownload.EnableWindow(TRUE);
    }
    else
    {
        m_btnStartDownload.EnableWindow(TRUE);
        m_btnStopDownload.EnableWindow(FALSE);
    }
    return 0;
}

//lint -e438
//lint -e529
void CDownloadingDlg::OnLvnItemchangedListDownloading(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    PostMessage(MSG_UPDATEBTN,0,0);

    *pResult = 0;
}
//lint +e529
//lint +e438

LRESULT CDownloadingDlg::OnMsgDownloadFailed( WPARAM wParam, LPARAM lParam )
{
    LONG lDowningSeq = (LONG)wParam;
    int nCount = m_listDownloading.GetItemCount();
    for (int i=0; i<nCount; i++)
    {
        LONG lDownSeq = m_listDownloading.GetItemData(i);
        if (lDownSeq == lDowningSeq)
        {
            m_listDownloading.DeleteItem(i);
            ReSetListIndex();
            PostMessage(MSG_UPDATEBTN,0,0);
            return 0;
        }
    }
    return 0;
}