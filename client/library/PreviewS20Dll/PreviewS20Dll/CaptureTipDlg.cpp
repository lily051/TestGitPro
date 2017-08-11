// CaptureTipDlg.cpp : ʵ���ļ�
//
//lint -library
#include "stdafx.h"

#include "CaptureTipDlg.h"
#include "ui/memdcex.h"
#include "PreviewS20Dll.h"

// CCaptureTipDlg �Ի���
#define DISAPPEARTIME (5 * 1000)
#define DISAP_TOTALTIMES 50
#define TOTAL_ALPHA 255

IMPLEMENT_DYNAMIC(CCaptureTipDlg, UIDialog)

CCaptureTipDlg::CCaptureTipDlg(CString& strPath, CWnd* pParent /*=NULL*/)
	: UIDialog(CCaptureTipDlg::IDD, pParent)
	, m_hDisappear(NULL)
	, m_nDisAppearCount(0)
	, m_bMouseHover(FALSE)
	, m_bMouseTrack(TRUE)
{
    m_strPath = strPath;
    m_bIsInit = FALSE;
    m_brushBK.CreateSolidBrush(theApp.GetUIColor(UI_COLOR_TYPE_UIDIALOG));
	InitializeCriticalSection(&m_csDisAppearCount);
}

CCaptureTipDlg::~CCaptureTipDlg()
{
    try
    {
        m_brushBK.DeleteObject();
    }
    catch (...)
    {
    }
}

void CCaptureTipDlg::DoDataExchange(CDataExchange* pDX)
{
	UIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PATH, m_sttPath);
}


BEGIN_MESSAGE_MAP(CCaptureTipDlg, UIDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCaptureTipDlg ��Ϣ�������

BOOL CCaptureTipDlg::OnInitDialog()
{
	UIDialog::OnInitDialog();

	m_nDisAppearCount = DISAP_TOTALTIMES;

	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,  
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);  

    SetWindowText(_T("ץͼ"));

	InitPos();

	m_bIsInit = TRUE;

	m_hDisappear = CreateThread(NULL, 0, AutoDisappear, this, 0, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CCaptureTipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = UIDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����

    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    if(CTLCOLOR_DLG == nCtlColor || CTLCOLOR_STATIC == nCtlColor)
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetBkColor(theApp.GetUIColor(UI_COLOR_TYPE_UIDIALOG));
        hbr = m_brushBK;
    }

    return hbr;
}

void CCaptureTipDlg::OnDestroy()
{
	UIDialog::OnDestroy();

	m_pic.FreePicture();
	if (m_hDisappear != NULL)
	{
		CloseHandle(m_hDisappear);
	}

	DeleteCriticalSection(&m_csDisAppearCount);

}

BOOL CCaptureTipDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CMemDCEX memDC(pDC,m_rcPic);
	
	m_pic.DrawPicture(memDC->m_hDC,0, 0,m_rcPic.Width(),m_rcPic.Height());

	return UIDialog::OnEraseBkgnd(pDC);
}

void CCaptureTipDlg::LoadPic(PBYTE pImage,DWORD dwImageSize)
{
	m_pic.LoadPicture(pImage,dwImageSize);

	m_sttPath.SetWindowText(m_strPath);
}


void CCaptureTipDlg::InitPos()
{
	CRect rcWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);

	CRect rcClient;
	GetClientRect(rcClient);

	MoveWindow(rcWorkArea.right - rcClient.Width(),
		rcWorkArea.bottom - rcClient.Height(),rcClient.Width(),rcClient.Height());

	GetClientRect(rcClient);
	m_rcPic = rcClient;
	m_rcPic.DeflateRect(10,10);
	//m_rcPic.top += 15;
	m_rcPic.bottom -= 30;

	m_rcPath = m_rcPic;
	m_rcPath.top = m_rcPic.bottom + 10;
	m_rcPath.bottom = rcClient.bottom - 10;
	m_sttPath.MoveWindow(m_rcPath);
}

void CCaptureTipDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_bMouseTrack)     // ������ ׷�٣���
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;// ָ��Ҫ ׷�� �Ĵ���
		csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧��


		m_bMouseTrack=FALSE ;   // ���Ѿ� ׷�� ����ֹͣ ׷��
	}

	CString strMsg;
	strMsg.Format(_T("@mouse left:%d, top:%d, right:%d, bottom:%d, ptx:%d, ptY:%d"),
		m_rcPath.left, m_rcPath.top, m_rcPath.right, m_rcPath.bottom,
		point.x, point.y);
	OutputDebugString(strMsg);

	if (m_rcPath.PtInRect(point))
	{
		SetCursor(LoadCursor(NULL,IDC_HAND)); 	
	}

	ResetTimeInterVal();
	m_bMouseHover = TRUE;
	UIDialog::OnMouseMove(nFlags, point);
}

void CCaptureTipDlg::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMouseHover = FALSE;
	m_bMouseTrack = TRUE;
	UIDialog::OnMouseLeave();
}

void CCaptureTipDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	ShellExecute(NULL, _T("open"), m_strPath, NULL, NULL, SW_SHOW);

	UIDialog::OnLButtonDown(nFlags, point);
}

DWORD WINAPI CCaptureTipDlg::AutoDisappear(LPVOID pParam)
{
	CCaptureTipDlg* pMan = (CCaptureTipDlg*)pParam;
	if (!pMan)
	{
		return -1;
	}
	BYTE btAlpha = TOTAL_ALPHA;
	float fDisappear = 1;
	int nTimeCount = DISAPPEARTIME;
	int nInterval = nTimeCount / pMan->m_nDisAppearCount;
	while(pMan->m_nDisAppearCount != 0)
	{
		if (pMan->m_bMouseHover)
		{
			pMan->ResetTimeInterVal();
		}
		else
		{
			CGuard guard(&pMan->m_csDisAppearCount);
			-- pMan->m_nDisAppearCount;
		}

		fDisappear = pMan->m_nDisAppearCount * 1.0 / DISAP_TOTALTIMES;
		btAlpha = TOTAL_ALPHA * fDisappear;
		::SetLayeredWindowAttributes(pMan->GetSafeHwnd(), 0, btAlpha, LWA_ALPHA);
		Sleep(nInterval);
	}

	return 0;
}

void CCaptureTipDlg::ResetTimeInterVal()
{
	CGuard guard(&m_csDisAppearCount);
	m_nDisAppearCount = DISAP_TOTALTIMES;
}
