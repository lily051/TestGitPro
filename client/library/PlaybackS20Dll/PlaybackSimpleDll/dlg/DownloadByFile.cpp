// DownloadByFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadByFile.h"


// CDownloadByFile �Ի���

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


// CDownloadByFile ��Ϣ�������

void CDownloadByFile::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, RGB(0xf5, 0xf2, 0xf2));
}

//��ʼ���ؼ�ͼƬ
void CDownloadByFile::InitCtrl()
{
	m_listFile.SetExtendedStyle(m_listFile.GetExtendedStyle()
		|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listFile.SetBkColor(RGB(235, 235, 235));
	m_listFile.DeleteAllItems();
	m_listFile.InsertColumn(0, _T("���"), LVCFMT_LEFT, 40);
	m_listFile.InsertColumn(1, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_listFile.InsertColumn(2, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listFile.InsertColumn(3, _T("�ļ���"), LVCFMT_LEFT, 160);
	m_listFile.InsertColumn(4, _T("�ļ���С"), LVCFMT_LEFT, 100);

	if (NULL == m_pFileList)
	{
		return;
	}
	// ��������.
	CString strTemp;
	int nItem = 0;
	for (std::vector<PB_FILEINFO>::iterator ite = m_pFileList->begin(); ite != m_pFileList->end(); ite++)
	{
		strTemp.Format(_T("%d"), nItem + 1);
		m_listFile.InsertItem(nItem, strTemp);
		// ��ʼʱ��.
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			(*ite).stStartTime.dwYear, (*ite).stStartTime.dwMonth, (*ite).stStartTime.dwDay, 
			(*ite).stStartTime.dwHour, (*ite).stStartTime.dwMinute, (*ite).stStartTime.dwSecond);
		m_listFile.SetItemText(nItem, 1, strTemp);
		// ����ʱ��.
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			(*ite).stEndTime.dwYear, (*ite).stEndTime.dwMonth, (*ite).stEndTime.dwDay, 
			(*ite).stEndTime.dwHour, (*ite).stEndTime.dwMinute, (*ite).stEndTime.dwSecond);
		m_listFile.SetItemText(nItem, 2, strTemp);
		// �ļ���.
		TCHAR szFileName[128];
		MByteToWChar((*ite).szFileName, szFileName, sizeof(szFileName));
		strTemp.Format(_T("%s"), szFileName);
		m_listFile.SetItemText(nItem, 3, strTemp);
		// �ļ���С.
		m_listFile.SetItemText(nItem, 4, GetFileSize((*ite).dwFileSize));

		// ���ñ���ɫ.
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/**   @fn          GetFileSize
 *    @brief   	   �ļ���С����.
 *    @param[in]   dwFileSize:��С����.
 *    @param[in]   
 *    @return      �ļ���С�ַ���.
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
 *    @brief   	   ��������.	
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
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
 *    @brief   	   ���ó�ʼ����Ϣ.	
 *    @param[in]   pFileList:�ļ��б�
 *    @param[in]   pDownloadDlg:���ضԻ���ָ��.
 *    @param[in]   nCameraID:��ص�ID.
 *    @param[in]   dwRev:�����ֶ�.
 *    @param[in]   nChannel:ͨ����.
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

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(0xf5, 0xf2, 0xf2));
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(RGB(0xf5, 0xf2, 0xf2));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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