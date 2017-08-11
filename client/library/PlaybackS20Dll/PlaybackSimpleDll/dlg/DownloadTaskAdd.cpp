// DownloadTaskAdd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadTaskAdd.h"


// CDownloadTaskAdd �Ի���

IMPLEMENT_DYNAMIC(CDownloadTaskAdd, UIDialog)

CDownloadTaskAdd::CDownloadTaskAdd(std::vector<PB_FILEINFO>* pFileList, CWnd* pParent /*=NULL*/,BOOL bShowDownTimeTab /*= TRUE*/)
	: UIDialog(CDownloadTaskAdd::IDD, pParent)
	, m_pFileList(pFileList)
	, m_bShowDownTimeTab(bShowDownTimeTab)
{
	m_pDownloadDlg = NULL;
	m_pDownloadByTime = NULL;
	m_pDownloadByFile = NULL;
	m_nChannel = -1;
	m_nCameraID = -1;
	m_dwRev = 0;
	m_strCamName = _T("");
	m_dwRight = PBSD_RIGHT_ALL;
	m_pLoginInfo = NULL;
	m_bkBrush.CreateSolidBrush(RGB(228,228,228));
	m_emPlayType = PB_TYPE_DVR;
}

CDownloadTaskAdd::~CDownloadTaskAdd()
{
	m_pDownloadDlg = NULL;
	m_pDownloadByTime = NULL;
	m_pDownloadByFile = NULL;
	try
	{
		m_bkBrush.DeleteObject();
	}
	catch(...)
	{
	}
}

void CDownloadTaskAdd::DoDataExchange(CDataExchange* pDX)
{
	UIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADDDOWNLOAD, m_btnAddDownload);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_CLOSEDLG, m_btnCloseDlg);
	DDX_Control(pDX, IDC_TAB_DOWNLOAD_TYPE, m_tabDownload);
}


BEGIN_MESSAGE_MAP(CDownloadTaskAdd, UIDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDownloadTaskAdd::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADDDOWNLOAD, &CDownloadTaskAdd::OnBnClickedButtonAdddownload)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEDLG, &CDownloadTaskAdd::OnBnClickedButtonClosedlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DOWNLOAD_TYPE, &CDownloadTaskAdd::OnTcnSelchangeTabDownload)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDownloadTaskAdd ��Ϣ�������

BOOL CDownloadTaskAdd::OnInitDialog()
{
	UIDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (-1 == m_nCameraID)
	{
		MsgBoxApi_Show(_T("û�л�ȡ����ص���Ϣ"), MSGBOX_TYPE_WARNING);
		EndDialog(IDCANCEL);
		return FALSE;
	}


	//�жϵ�ǰ��ص��Ƿ�������Ȩ��
	if (!theApp.IsCamDownloadRight(m_dwRight))
	{
		MsgBoxApi_Show(_T("��ǰ�û������д˼�ص�����Ȩ��"), MSGBOX_TYPE_WARNING);
		EndDialog(IDCANCEL);
		return FALSE;
	}

	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CDownloadTaskAdd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = UIDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

// 	if (CTLCOLOR_STATIC == nCtlColor)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		pDC->SetBkColor(RGB(228,228,228));
// 		pDC->SetTextColor(RGB(0, 0, 0));
// 		hbr = m_bkBrush;
// 	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CDownloadTaskAdd::OnBnClickedButtonAdddownload()
{
	if (m_pDownloadByTime != NULL  &&  m_pDownloadByFile != NULL)
	{
		if ((m_pDownloadByTime->GetTaskNum() + m_pDownloadByFile->GetTaskNum()) <= 0)
		{
			CString strErrMsg = _T("��ѡ��һ���ļ���ʱ���!");
			BSTR str = strErrMsg.AllocSysString();
			MsgBoxApi_Show(str, MSGBOX_TYPE_WARNING);
			SysFreeString(str);
			return;
		}
	}

	int nItem = m_tabDownload.GetCurSel();

	if (SEL_BY_FILE == nItem)
	{
		if (m_pDownloadByFile != NULL)
		{
			if (!m_pDownloadByFile->AddDownload())
			{
				return;
			}
		}
	}
	else if (SEL_BY_TIME == nItem)
	{
		if (m_pDownloadByTime != NULL)
		{
			if (!m_pDownloadByTime->AddDownload())
			{
				return;
			}
		}
	}
	
	if (m_pDownloadDlg != NULL)
	{
		m_pDownloadDlg->NotifyStartDownload();
	}

	OnOK();
}


void CDownloadTaskAdd::OnBnClickedButtonCancel()
{
	OnCancel();
}

//��ʼ���ؼ�ͼƬ
void CDownloadTaskAdd::InitCtrl()
{
	SetWindowText(_T("����"));

	if (NULL == m_pDownloadByTime)
	{
		m_pDownloadByTime = new CDownloadByTime;
		m_pDownloadByTime->SetDownloadDlgPoint(m_pDownloadDlg);
		m_pDownloadByTime->SetInitInfo(m_nCameraID, m_dwRev, m_strCamName, m_nChannel, m_emPlayType, m_pLoginInfo);
		m_pDownloadByTime->Create(CDownloadByTime::IDD, this);
		m_pDownloadByTime->ShowWindow(SW_HIDE);
	}

	if (NULL == m_pDownloadByFile)
	{
		m_pDownloadByFile = new CDownloadByFile();
		m_pDownloadByFile->SetInitInfo(m_pFileList, m_pLoginInfo, m_pDownloadDlg, m_nCameraID, m_dwRev, m_strCamName, m_nChannel);
		m_pDownloadByFile->Create(CDownloadByFile::IDD, this);
		m_pDownloadByFile->ShowWindow(SW_SHOW);
	}

	m_tabDownload.InsertItem(TAB_ITEM_DOWNLOAD_BY_FILE, _T("���ļ�����"));
	if (m_bShowDownTimeTab)
	{
		m_tabDownload.InsertItem(TAB_ITEM_DOWNLOAD_BY_TIME, _T("��ʱ������"));
	}
	m_tabDownload.SetCurSel(0);
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcTab(rcClient);
	rcTab.bottom = rcTab.top + 24;
	m_tabDownload.MoveWindow(rcTab);
	CRect rcChild(rcClient);
	CRect rcBtn;
	m_btnAddDownload.GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	rcChild.top = rcTab.bottom;
	rcChild.bottom = rcBtn.top - 10;
	if (m_pDownloadByTime != NULL)
	{
		m_pDownloadByTime->MoveWindow(rcChild);
		m_pDownloadByFile->MoveWindow(rcChild);
	}

	//button
	m_btnAddDownload.SetSkinBitmap(RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_BTNHOVERBK), RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_SELBK), RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_SELBK));
	//m_btnAddDownload.SetImage(LoadImageFromResource(IDB_PNG_ONBTN_DELETE));

	m_btnCancel.SetSkinBitmap(RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_BTNHOVERBK), RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_SELBK), RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_SELBK));
	//m_btnCancel.SetImage(LoadImageFromResource(IDB_PNG_ONBTN_DELETE));

	m_btnCloseDlg.SetSkinBitmap(RESOURCELOAD_PNG_EX_HBMPE(IDR_PNG_CUSTOMPOPUPDLG_X), RESOURCELOAD_PNG_EX_HBMPE(IDR_PNG_CUSTOMPOPUPDLG_XSEL), RESOURCELOAD_PNG_EX_HBMPE(IDR_PNG_CUSTOMPOPUPDLG_XSEL));
}

//�������ش���ָ��
void CDownloadTaskAdd::SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg)
{
	m_pDownloadDlg = pDownloadDlg;
}

void CDownloadTaskAdd::OnBnClickedButtonClosedlg()
{
	OnCancel();
}

/**   @fn          SetInitInfo
 *    @brief   	   ����������Ϣ.
 *    @param[in]   nCameraID:��ص�ID.
 *    @param[in]   dwRev:�����ֶ�.
 *    @param[in]   nChannel:ͨ����.
 *    @param[in]   emPlayType:�ط�����.
 *    @return      
 */
void CDownloadTaskAdd::SetInitInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, DWORD dwRight, ENUM_PLAY_TYPE emPlayType, PB_LOGININFO* pLoginInfo)
{
	m_nCameraID = nCameraID;
	m_dwRev = dwRev;
	m_strCamName = strCamName;
	m_nChannel = nChannel;
	m_emPlayType = emPlayType;
	m_pLoginInfo = pLoginInfo;
	m_dwRight = dwRight;
}

void CDownloadTaskAdd::OnDestroy()
{
	UIDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pDownloadByTime != NULL)
	{
		m_pDownloadByTime->DestroyWindow();
		delete m_pDownloadByTime;
		m_pDownloadByTime = NULL;
	}

	if (m_pDownloadByFile != NULL)
	{
		m_pDownloadByFile->DestroyWindow();
		delete m_pDownloadByFile;
		m_pDownloadByFile = NULL;
	}
}

void CDownloadTaskAdd::OnTcnSelchangeTabDownload(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = m_tabDownload.GetCurSel();

	switch (nItem)
	{
	case SEL_BY_FILE:
		{
			if (m_pDownloadByFile != NULL)
			{
				m_pDownloadByFile->ShowWindow(SW_SHOW);
			}
			if (m_pDownloadByTime != NULL)
			{
				m_pDownloadByTime->ShowWindow(SW_HIDE);
			}
		}
		break;

	case SEL_BY_TIME:
		{
			if (m_pDownloadByFile != NULL)
			{
				m_pDownloadByFile->ShowWindow(SW_HIDE);
			}
			if (m_pDownloadByTime != NULL)
			{
				m_pDownloadByTime->ShowWindow(SW_SHOW);
			}
		}
		break;
	}

	*pResult = 0;
}

BOOL CDownloadTaskAdd::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    switch (pMsg->message)
    {
    case WM_KEYDOWN:
        {
            //����Enter��
            if (pMsg->wParam == VK_RETURN)
            {
                return TRUE;
            }
            //����Esc��
            if (pMsg->wParam == VK_ESCAPE)
            {
                return TRUE;
            }
        }
        break;
    case WM_SYSKEYDOWN:
        {
            //����Alt + F4
            if (pMsg->wParam == VK_F4)
            {
                return TRUE;
            }
        }
        break;
    default:
        break;
    }

    return CWnd::PreTranslateMessage(pMsg);
}