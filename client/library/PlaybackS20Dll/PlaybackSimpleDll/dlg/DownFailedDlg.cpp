// DownFailedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownFailedDlg.h"


// CDownFailedDlg 对话框

IMPLEMENT_DYNAMIC(CDownFailedDlg, CDialog)

CDownFailedDlg::CDownFailedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownFailedDlg::IDD, pParent)
{
	m_bMove = FALSE;

	m_hBitmap = NULL;
	m_nBitmapWidth = 0;
	m_nBitmapHeight = 0;
}

CDownFailedDlg::~CDownFailedDlg()
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

void CDownFailedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_BUTTON_REDOWN, m_btnReDownload);
	DDX_Control(pDX, IDC_LIST_DOWNLOADFAILED, m_listDownFailed);
}


BEGIN_MESSAGE_MAP(CDownFailedDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_REDOWN, &CDownFailedDlg::OnBnClickedButtonRedown)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDownFailedDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDownFailedDlg::OnBnClickedButtonClear)
	ON_WM_PAINT()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOWNLOADFAILED, &CDownFailedDlg::OnLvnItemchangedListDownloadfailed)
END_MESSAGE_MAP()


// CDownFailedDlg 消息处理程序

BOOL CDownFailedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();

	m_bMove = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDownFailedDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}

}

void CDownFailedDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, UI_WND_BKGND);

	dc.StretchBlt(m_rcTitle.left, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height(), &m_dcMem, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);
}

//创建子控件
void CDownFailedDlg::InitCtrl()
{
	//button;
	m_btnReDownload.Load(IDB_PNG_AGAIN);
	m_btnReDownload.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	m_btnDelete.Load(IDB_PNG_DEL_TASK);
	m_btnDelete.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);

	m_btnClear.Load(IDB_PNG_ONBTN_CLEAR);
	m_btnClear.LoadSelectBkgnd(IDR_PNG_DLG_BTN, IDR_PNG_DLG_BTN);
 

	//list
	m_listDownFailed.SetExtendedStyle(m_listDownFailed.GetExtendedStyle()
		| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_listDownFailed.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_listDownFailed.InsertColumn(1, _T("下载进度"), LVCFMT_LEFT, 60);
	m_listDownFailed.InsertColumn(2, _T("监控点"), LVCFMT_LEFT, 120);
	m_listDownFailed.InsertColumn(3, _T("开始时间"), LVCFMT_LEFT, 125);
	m_listDownFailed.InsertColumn(4, _T("结束时间"), LVCFMT_LEFT, 125);
	m_listDownFailed.InsertColumn(5, _T("文件名"), LVCFMT_LEFT, 160);
	m_listDownFailed.InsertColumn(6, _T("大小"), LVCFMT_LEFT, 100);
	m_listDownFailed.InsertColumn(7, _T("下载方式"), LVCFMT_LEFT, 80);
	m_listDownFailed.InsertColumn(8, _T("数据源"), LVCFMT_LEFT, 80);
	m_listDownFailed.InsertColumn(9, _T("存储路径"), LVCFMT_LEFT, 260);
	//m_listDownFailed.InsertColumn(10, _T("错误原因"), LVCFMT_LEFT, 120);

	Bitmap * pBitmap = ::LoadBitmapFromResource(IDB_PNG_TITLE);
	pBitmap->GetHBITMAP(Color(0, 255, 0, 255), &m_hBitmap);
	m_nBitmapWidth = pBitmap->GetWidth();
	m_nBitmapHeight = pBitmap->GetHeight();

}

//移动控件
void CDownFailedDlg::MoveCtrl()
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
	m_btnReDownload.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnDelete.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnClear.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);

	CRect rcList(rcClient);
	rcList.top = m_rcTitle.bottom;
	m_listDownFailed.MoveWindow(rcList);

}

BOOL CDownFailedDlg::OnEraseBkgnd(CDC* pDC)
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
*   @透明绘图 
*   @param[in] pDc: 子控件pDc
*   @param[in] pWnd: 子控件句柄 
*   @return NULL
*/
void CDownFailedDlg::TransparentBk(CDC * pDc, CWnd * pWnd)
{
	CRect rcWnd;
	pWnd->GetClientRect(&rcWnd);

	pDc->StretchBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &m_dcMem, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);
}


void CDownFailedDlg::OnBnClickedButtonRedown()
{
	POSITION pos = NULL;
	while (pos = m_listDownFailed.GetFirstSelectedItemPosition())
	{
		int nItem = m_listDownFailed.GetNextSelectedItem(pos);
		LONG lDownloadSeq = m_listDownFailed.GetItemData(nItem);
		//删除map表对应数据
		std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.find(lDownloadSeq);
		m_mapDownInfo.erase(itor);
		//删除list表数据
		m_listDownFailed.DeleteItem(nItem);
		//通知
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_REDOWN, lDownloadSeq, 0);
	}

	//序号重排
	int nItem = 0;
	CString strIndex;
	int nCount = m_listDownFailed.GetItemCount();
	while (nItem < nCount)
	{
		strIndex.Format(_T("%d"), nItem+1);
		m_listDownFailed.SetItemText(nItem, 0, strIndex);

		nItem++;
	}

	//发送通知下载消息
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_RECORD_DOWNLOAD_NOTIFY, 0, 0);
}

void CDownFailedDlg::OnBnClickedButtonDelete()
{
	POSITION pos = NULL;
	while (pos = m_listDownFailed.GetFirstSelectedItemPosition())
	{
		int nItem = m_listDownFailed.GetNextSelectedItem(pos);
		LONG lDownloadSeq = m_listDownFailed.GetItemData(nItem);
		//删除map表对应数据
		std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.find(lDownloadSeq);
		m_mapDownInfo.erase(itor);
		//删除list表数据
		m_listDownFailed.DeleteItem(nItem);
		//通知
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_DEL, lDownloadSeq, IDD_DIALOG_DOWNFAILED);
	}

	//序号重排
	int nItem = 0;
	CString strIndex;
	int nCount = m_listDownFailed.GetItemCount();
	while (nItem < nCount)
	{
		strIndex.Format(_T("%d"), nItem+1);
		m_listDownFailed.SetItemText(nItem, 0, strIndex);

		nItem++;
	}
}

void CDownFailedDlg::OnBnClickedButtonClear()
{
	m_listDownFailed.DeleteAllItems();
	std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownInfo.begin();
	while (itor != m_mapDownInfo.end())
	{
		::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADTASK_DEL, itor->first, IDD_DIALOG_DOWNFAILED);
		itor++;
	}
	m_mapDownInfo.clear();
}

//增加一条记录
BOOL CDownFailedDlg::AddDownloadInfo(LONG lSeq, DOWNLOAD_INFO& stDownloadInfo, DWORD dwError)
{
	if (m_mapDownInfo.find(lSeq) != m_mapDownInfo.end())
	{
		return FALSE;
	}

	m_mapDownInfo[lSeq] = stDownloadInfo;

	int nCount = m_listDownFailed.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nCount+1);
	int nItem = m_listDownFailed.InsertItem(nCount, strIndex);
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
		strFileSize = GetFileSize(stDownloadInfo.unDownloadParam.downloadTimeParam.dwFileSize);
		startTime = stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime;
		stopTime = stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime;
		strDownloadType = _T("时间下载");
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
		strFileSize = GetFileSize(stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.dwFileSize);
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile, strlen(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile)+1, 
			wszSavePath, nLength);
		strSavePath.Format(_T("%s"), wszSavePath);
		CodeSet_Free((void*)wszSavePath);
		strDownloadType = _T("文件下载");
	}
	strStartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), startTime.dwYear, startTime.dwMonth, startTime.dwDay, startTime.dwHour, startTime.dwMinute, startTime.dwSecond);
	strStopTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), stopTime.dwYear, stopTime.dwMonth, stopTime.dwDay, stopTime.dwHour, stopTime.dwMinute, stopTime.dwSecond);
	m_listDownFailed.SetItemText(nItem, 1, _T("下载失败"));
	m_listDownFailed.SetItemText(nItem, 2, strCameraName);
	m_listDownFailed.SetItemText(nItem, 3, strStartTime);
	m_listDownFailed.SetItemText(nItem, 4, strStopTime);
	m_listDownFailed.SetItemText(nItem, 5, strFileName);
	m_listDownFailed.SetItemText(nItem, 6, strFileSize);
	m_listDownFailed.SetItemText(nItem, 7, strDownloadType);
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
		strDataSrc = _T("云存储");
		break;
    case PB_TYPE_KMS:
        strDataSrc = _T("KMS");
        break;
	case PB_TYPE_OBJECTCLOUD:
		strDataSrc = _T("对象存储");
		break;
	default:
		break;
	}
	m_listDownFailed.SetItemText(nItem, 8, strDataSrc);
	m_listDownFailed.SetItemText(nItem, 9, strSavePath);
	CString strErr;
	GetErrorMsg(dwError, strErr);
	//m_listDownFailed.SetItemText(nItem, 10, strErr);
	m_listDownFailed.SetItemData(nItem, lSeq);

	// 自适应宽度.
	int nColumnWidth = m_listDownFailed.GetColumnWidth(9);
	m_listDownFailed.SetColumnWidth(9, LVSCW_AUTOSIZE_USEHEADER);
	int nHeaderWidth = m_listDownFailed.GetColumnWidth(9); 
	m_listDownFailed.SetColumnWidth(9, max(nColumnWidth, nHeaderWidth));

	return TRUE;
}

//lint -e438
//lint -e529
void CDownFailedDlg::OnLvnItemchangedListDownloadfailed(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    if (m_listDownFailed.GetSelectedCount() <= 0)
    {
        m_btnReDownload.EnableWindow(FALSE);
        m_btnDelete.EnableWindow(FALSE);
    }
    else
    {
        m_btnReDownload.EnableWindow(TRUE);
        m_btnDelete.EnableWindow(TRUE);
    }
    *pResult = 0;
}
//lint +e529
//lint +e438
