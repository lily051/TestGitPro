// CaptureTipDlg.cpp : ʵ���ļ�
//
//lint -library
#include "stdafx.h"
#include "SaveDataDlg.h"
#include "../ui/uiCtrl/memdcex.h"

// CSaveDataDlg �Ի���

IMPLEMENT_DYNAMIC(CSaveDataDlg, UIDialog)

CSaveDataDlg::CSaveDataDlg(CWnd* pParent /*=NULL*/)
	: UIDialog(CSaveDataDlg::IDD, pParent)
{
	m_bIsInit = FALSE;

	m_bTimer = FALSE;
	m_bCount = TRUE;
	m_nLeaveTime = 0;

	m_bkBrush.CreateSolidBrush(UI_TREE_BKCOLOR);
}

CSaveDataDlg::~CSaveDataDlg()
{
	try
	{
		m_bkBrush.DeleteObject();
	}
	catch (...)
	{

	}
}

void CSaveDataDlg::DoDataExchange(CDataExchange* pDX)
{
	UIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PATH, m_sttPath);
}


BEGIN_MESSAGE_MAP(CSaveDataDlg, UIDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CSaveDataDlg ��Ϣ�������

BOOL CSaveDataDlg::OnInitDialog()
{
	UIDialog::OnInitDialog();

	SetWindowText(_T("�طż���"));

	InitPos();

	m_bIsInit = TRUE;

	SetTimer(IDT_CAPTUREDLGCLOSE_TIME, 1000, NULL);
	m_bTimer = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSaveDataDlg::OnDestroy()
{
	UIDialog::OnDestroy();

	if (m_bTimer)
	{
		KillTimer(IDT_CAPTUREDLGCLOSE_TIME);
		m_bTimer = FALSE;
	}
}

void CSaveDataDlg::LoadFile(TCHAR * szPicPath)
{
    m_strPath.Format(_T("%s"),szPicPath);
}


void CSaveDataDlg::InitPos()
{
	CRect rcWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);

	CRect rcClient;
	GetClientRect(rcClient);

	int nWidth = 250;
	int nHeight = 150;
	MoveWindow(rcWorkArea.right - nWidth,
		rcWorkArea.bottom - nHeight, nWidth, nHeight);

    rcClient.left = 0;
    rcClient.right = 250;
	m_rcPath.CopyRect(rcClient);
	m_sttPath.ShowWindow(SW_HIDE);
}

void CSaveDataDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_rcPath.PtInRect(point))
	{
		SetCursor(LoadCursor(NULL,IDC_HAND)); 	
	}

	TRACKMOUSEEVENT mouseEvent;
	mouseEvent.cbSize = sizeof(mouseEvent);
	mouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
	mouseEvent.hwndTrack = m_hWnd;
	mouseEvent.dwHoverTime = 200;
	::TrackMouseEvent(&mouseEvent);

	UIDialog::OnMouseMove(nFlags, point);
}

void CSaveDataDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcPath.PtInRect(point))
	{
		CString strParam;
		strParam.Format(_T("/select,%s"),  (LPCTSTR)m_strPath);
		ShellExecute(NULL, _T("open"), _T("Explorer.exe"), strParam.GetBuffer(), NULL, SW_SHOW);
	}

	UIDialog::OnLButtonDown(nFlags, point);
}

void CSaveDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	UIDialog::OnTimer(nIDEvent);

	if (IDT_CAPTUREDLGCLOSE_TIME == nIDEvent)
	{
		if (!m_bCount)
		{
			return;
		}

		if (m_nLeaveTime >= 10)
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			m_nLeaveTime++;
		}
	}
}

void CSaveDataDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bCount = FALSE;
	m_nLeaveTime = 0;

	UIDialog::OnMouseHover(nFlags, point);
}

void CSaveDataDlg::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_bCount = TRUE;

	UIDialog::OnMouseLeave();
}

HBRUSH CSaveDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = UIDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(UI_TREE_BKCOLOR);
		pDC->SetTextColor(UI_TEXT_COLOR);
	}
    hbr = m_bkBrush;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CSaveDataDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowWindow(SW_HIDE);
	if (m_bTimer)
	{
		KillTimer(IDT_CAPTUREDLGCLOSE_TIME);
		m_bTimer = FALSE;
	}

	//UIDialog::OnClose();
}

void CSaveDataDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
 
    DrawTextInRect(m_strPath,m_rcPath);
}

void CSaveDataDlg::DrawTextInRect(CString strText, CRect rcTemp)
{
    CDC *pDC = GetDC();
    if (NULL == pDC)
    {
        return;
    }
    LOGFONT stLF;
    memset(&stLF, 0, sizeof(LOGFONT));
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(stLF), &stLF);
    stLF.lfWeight = 500;
    HFONT hFontNew, hFontOld;
    hFontNew = CreateFontIndirect(&stLF);
    hFontOld = (HFONT)pDC->SelectObject(hFontNew);
    pDC->SetTextColor(UI_TEXT_COLOR);
    pDC->SetBkColor(UI_TREE_BKCOLOR);
    // DrawText���������DT_WORDBREAK|DT_EDITCONTROL���ض�ʡ����ʾ��DT_END_ELLIPSIS
    pDC->DrawText(strText, strText.GetLength(), rcTemp, DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS);
    pDC->SelectObject(hFontOld);

    DeleteObject(hFontNew);
    ReleaseDC(pDC);
}