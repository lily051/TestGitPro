// LocalWndView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalWndView.h"


// CLocalWndView

IMPLEMENT_DYNAMIC(CLocalWndView, CWnd)

CLocalWndView::CLocalWndView()
{
	m_clrBk = UI_PLAYWND_COLOR;
	m_bIsPlaying = FALSE;
	m_bIsPausing = FALSE;
	m_bIsSounding = FALSE;
	m_bIsFullScreening = FALSE;
}

CLocalWndView::~CLocalWndView()
{
}


BEGIN_MESSAGE_MAP(CLocalWndView, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CLocalWndView ��Ϣ�������



BOOL CLocalWndView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrBk);

	return TRUE;
}

int CLocalWndView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

void CLocalWndView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

void CLocalWndView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

void CLocalWndView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PopupMenu();

	CWnd::OnRButtonDown(nFlags, point);
}

void CLocalWndView::PopupMenu(BOOL bJustClose/* = FALSE*/)
{

	BCMenu menuPopup;
	menuPopup.CreatePopupMenu();

	if (!bJustClose)
	{
		if (m_bIsPlaying)
		{
			menuPopup.AppendMenu(MF_STRING, MENU_ID_CAPTURE, _T("ץͼ"));
			menuPopup.AppendMenu(MF_STRING, MENU_ID_SLOW, _T("����"));
			if (!m_bIsPausing)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("��ͣ"));
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_PLAY, _T("����"));
			}
			
			menuPopup.AppendMenu(MF_STRING, MENU_ID_FAST, _T("����"));
			if (!m_bIsSounding)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("������Ƶ"));
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_OPENSOUND, _T("�ر���Ƶ"));
			}
			menuPopup.AppendMenu(MF_SEPARATOR);
			if (!m_bIsFullScreening)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_FULLSCREEN, _T("ȫ����ʾ"));
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_FULLSCREEN, _T("�˳�ȫ��"));
			}
			menuPopup.AppendMenu(MF_STRING, MENU_ID_STOP, _T("ֹͣ"));
		}
	}

	CPoint point;
	GetCursorPos(&point);
	int nCmd = menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, point.x, point.y, this);

	ExcuteSelectOption(nCmd);
}

void CLocalWndView::ExcuteSelectOption(int nCmd)
{
	switch(nCmd)
	{
	case MENU_ID_CAPTURE:       //ץͼ
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_CAPTURE, 0);
		}
		break;
	case MENU_ID_SLOW:          //����
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SLOW, 0);
		}
		break;
	case MENU_ID_PLAY:          //���š���ͣ
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_PLAY, 0);
		}
		break;
	case MENU_ID_FAST:          //����
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_FAST, 0);
		}
		break;
	case MENU_ID_OPENSOUND:     //��Ƶ
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_SOUND, 0);
		}
		break;
	case MENU_ID_FULLSCREEN:    //ȫ��
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_FULLSCREEN, 0);
		}
		break;
	case MENU_ID_STOP:          //ֹͣ
		{
			::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_OPERATOR, OPERA_ID_STOP, 0);
		}
		break;
	}
}

void CLocalWndView::UpdatePlayStatus(BOOL bPlay)
{
	m_bIsPlaying = bPlay;
}

void CLocalWndView::UpdateMenuStatus(int nBtnType, BOOL bStatus)
{
	switch(nBtnType)
	{
	case OPERA_ID_PLAY:
		m_bIsPausing = bStatus;
		break;
	case OPERA_ID_SOUND:
		m_bIsSounding = bStatus;
		break;
	case OPERA_ID_FULLSCREEN:
		m_bIsFullScreening = bStatus;
		break;
	default:
		break;
	}
}
