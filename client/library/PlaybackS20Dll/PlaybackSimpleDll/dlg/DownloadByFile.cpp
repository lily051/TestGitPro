// DownloadByFile.cpp : 实现文件
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadByFile.h"


// CDownloadByFile 对话框

IMPLEMENT_DYNAMIC(CDownloadByFile, CDialog)

CDownloadByFile::CDownloadByFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadByFile::IDD, pParent)
{
	m_pFileList = NULL;
	m_pDownloadDlg = NULL;
	m_pLoginInfo = NULL;
	m_nCameraID = -1;
	m_dwRev = 0;
	m_strCamName = _T("");
	m_nChannel = -1;
	m_nCheckNum = 0;
}

CDownloadByFile::~CDownloadByFile()
{
	m_pFileList = NULL;
	m_pDownloadDlg = NULL;
	m_pLoginInfo = NULL;
}

void CDownloadByFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Control(pDX, IDC_CHECK_ALL, m_chkAll);
}


BEGIN_MESSAGE_MAP(CDownloadByFile, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_ALL, &CDownloadByFile::OnBnClickedCheckAll)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDownloadByFile 消息处理程序

void CDownloadByFile::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, RGB(0xf5, 0xf2, 0xf2));
}

//初始化控件图片
void CDownloadByFile::InitCtrl()
{
	m_listFile.SetExtendedStyle(m_listFile.GetExtendedStyle()
		|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listFile.SetBkColor(RGB(235, 235, 235));
	m_listFile.DeleteAllItems();
	m_listFile.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_listFile.InsertColumn(1, _T("开始时间"), LVCFMT_LEFT, 120);
	m_listFile.InsertColumn(2, _T("结束时间"), LVCFMT_LEFT, 120);
	m_listFile.InsertColumn(3, _T("文件名"), LVCFMT_LEFT, 160);
	m_listFile.InsertColumn(4, _T("文件大小"), LVCFMT_LEFT, 100);

	if (NULL == m_pFileList)
	{
		return;
	}
	// 插入数据.
	CString strTemp;
	int nItem = 0;
	for (std::vector<PB_FILEINFO>::iterator ite = m_pFileList->begin(); ite != m_pFileList->end(); ite++)
	{
		strTemp.Format(_T("%d"), nItem + 1);
		m_listFile.InsertItem(nItem, strTemp);
		// 开始时间.
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			(*ite).stStartTime.dwYear, (*ite).stStartTime.dwMonth, (*ite).stStartTime.dwDay, 
			(*ite).stStartTime.dwHour, (*ite).stStartTime.dwMinute, (*ite).stStartTime.dwSecond);
		m_listFile.SetItemText(nItem, 1, strTemp);
		// 结束时间.
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			(*ite).stEndTime.dwYear, (*ite).stEndTime.dwMonth, (*ite).stEndTime.dwDay, 
			(*ite).stEndTime.dwHour, (*ite).stEndTime.dwMinute, (*ite).stEndTime.dwSecond);
		m_listFile.SetItemText(nItem, 2, strTemp);
		// 文件名.
		TCHAR szFileName[128];
		MByteToWChar((*ite).szFileName, szFileName, sizeof(szFileName));
		strTemp.Format(_T("%s"), szFileName);
		m_listFile.SetItemText(nItem, 3, strTemp);
		// 文件大小.
		m_listFile.SetItemText(nItem, 4, GetFileSize((*ite).dwFileSize));

		// 设置背景色.
		if (nItem % 2 == 0)
		{
			m_listFile.SetItemBkColor(nItem, -1, UI_LISTITEMF_COLOR);
		}
		else
		{
			m_listFile.SetItemBkColor(nItem, -1, UI_LISTITEMS_COLOR);
		}

		nItem++;
	}
	m_listFile.SetCheck(0);
}

BOOL CDownloadByFile::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**   @fn          GetFileSize
 *    @brief   	   文件大小换算.
 *    @param[in]   dwFileSize:大小换算.
 *    @param[in]   
 *    @return      文件大小字符串.
 */
CString CDownloadByFile::GetFileSize(DWORD dwFileSize)
{
	TRACE("@z filesize:%ld", dwFileSize);
	CString strFileSize = _T("");

	if (dwFileSize <= 0)
	{
		strFileSize = _T("0");
		return strFileSize;
	}

	// MB.
	float fMB = dwFileSize / ((float)1024 * (float)1024);
	if (fMB > 1)
	{
		strFileSize.Format(_T("%.02fMB"), fMB);
		return strFileSize;
	}

	// KB.
	float fKB = dwFileSize / ((float)1024);
	if (fKB > 1)
	{
		strFileSize.Format(_T("%.02fKB"), fKB);
		return strFileSize;
	}

	strFileSize.Format(_T("%ldB"), dwFileSize);
	return strFileSize;

}

void CDownloadByFile::OnBnClickedCheckAll()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheck = FALSE;
	if (m_chkAll.GetCheck())
	{
		bCheck = TRUE;
	}
	else
	{
		bCheck = FALSE;
	}

	int nCount = m_listFile.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_listFile.SetCheck(i, bCheck);
	}
}

/**   @fn          AddDownload
 *    @brief   	   加入下载.	
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CDownloadByFile::AddDownload()
{
	int nCount = m_listFile.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_listFile.GetCheck(i))
		{
			m_pDownloadDlg->AddFileDownloadInfo(m_nCameraID, m_dwRev, m_strCamName, m_nChannel, *m_pLoginInfo, (*m_pFileList)[i]);
		}
	}

	return TRUE;
}

/**   @fn          SetInitInfo
 *    @brief   	   设置初始化信息.	
 *    @param[in]   pFileList:文件列表
 *    @param[in]   pDownloadDlg:下载对话框指针.
 *    @param[in]   nCameraID:监控点ID.
 *    @param[in]   dwRev:保留字段.
 *    @param[in]   nChannel:通道号.
 *    @return      
 */
void CDownloadByFile::SetInitInfo(
								  std::vector<PB_FILEINFO>* pFileList, 
								  PB_LOGININFO* pLoginInfo, 
								  CDownloadDlg* pDownloadDlg, 
								  int nCameraID, 
								  DWORD dwRev,
								  const CString& strCamName, 
								  int nChannel)
{
	m_pFileList = pFileList;
	m_pLoginInfo = pLoginInfo;
	m_pDownloadDlg = pDownloadDlg;
	m_nCameraID = nCameraID;
	m_dwRev = dwRev;
	m_strCamName = strCamName;
	m_nChannel = nChannel;
}
HBRUSH CDownloadByFile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(0xf5, 0xf2, 0xf2));
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(RGB(0xf5, 0xf2, 0xf2));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

int CDownloadByFile::GetTaskNum()
{
	m_nCheckNum = 0;

	int nCount = m_listFile.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_listFile.GetCheck(i))
		{
			m_nCheckNum++;
		}
	}

	return m_nCheckNum;
}