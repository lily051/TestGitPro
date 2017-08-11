// PlayMiniToolBarExpandWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "PlayMiniToolBarExpandWnd.h"


// CPlayMiniToolBarExpandWnd

IMPLEMENT_DYNAMIC(CPlayMiniToolBarExpandWnd, CWnd)

CPlayMiniToolBarExpandWnd::CPlayMiniToolBarExpandWnd(BOOL bOpenSound, BOOL bSaveData)
: m_nBtnNum(5)
, m_nBtnWidth(20)
, m_nBtnHeight(24)
, m_nBtnSpace(4)
, m_uiIdleTime(0)
, m_uiCloseTimer(0)
, m_pParentWnd(NULL)
, m_bOpenSound(bOpenSound)
, m_bSaveData(bSaveData)
{

}

CPlayMiniToolBarExpandWnd::~CPlayMiniToolBarExpandWnd()
{
	m_pParentWnd = NULL;
}


BEGIN_MESSAGE_MAP(CPlayMiniToolBarExpandWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(TOOL_CAPTURE_ID, &CPlayMiniToolBarExpandWnd::OnToolBarBtnClkCapture)
	ON_BN_CLICKED(TOOL_SAVEDATA_ID, &CPlayMiniToolBarExpandWnd::OnToolBarBtnClkSave)
	ON_BN_CLICKED(TOOL_SOUND_ID, &CPlayMiniToolBarExpandWnd::OnToolBarBtnClkSound)
	ON_BN_CLICKED(TOOL_DOWNLOAD_ID, &CPlayMiniToolBarExpandWnd::OnToolBarBtnClkDownload)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPlayMiniToolBarExpandWnd ��Ϣ�������
//�߿����
void CPlayMiniToolBarExpandWnd::DrawBorder(CDC* pDC)
{
	CRect rcWin;
	CPen pen;
	CPen* pOldPen = NULL;
	CBrush *pOldBrush = NULL;

	GetClientRect(&rcWin);
	pen.CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(rcWin);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();
}


BOOL CPlayMiniToolBarExpandWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(0, 0, 0));

	DrawBorder(pDC);
	return TRUE;

	//return CWnd::OnEraseBkgnd(pDC);
}

int CPlayMiniToolBarExpandWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CreateChildWnd();
	InitCtrl();
	InitPos();
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);
	return 0;
}

void CPlayMiniToolBarExpandWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_penBorder.DeleteObject();
}

void CPlayMiniToolBarExpandWnd::CreateChildWnd()
{
	CRect rcSmallBtn(0, 0, 20, 28);

	m_btnDownload.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcSmallBtn, this, TOOL_DOWNLOAD_ID);
	if (theApp.HasDownloadModule())
	{
		m_btnDownload.ShowWindow(SW_SHOW);
	}

	m_btnCapture.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcSmallBtn, this, TOOL_CAPTURE_ID);
	m_btnCapture.ShowWindow(SW_SHOW);

	m_btnSave.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcSmallBtn, this, TOOL_SAVEDATA_ID);
	m_btnSave.ShowWindow(SW_SHOW);

	m_btnSound.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP, 
		rcSmallBtn, this, TOOL_SOUND_ID);
	m_btnSound.ShowWindow(SW_SHOW);
}

void CPlayMiniToolBarExpandWnd::InitCtrl()
{
	m_penBorder.CreatePen(PS_SOLID, 2, RGB(128, 128, 128));

	m_btnDownload.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_DOWNLOAD),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_DOWNLOAD_HOVER));

	m_btnCapture.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CAPTURE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CAP_HOVER));

	SetToolBarBtnTips();

	UpdateBtnState(m_bOpenSound, m_bSaveData);
}

void CPlayMiniToolBarExpandWnd::InitPos()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcBtn(rcClient);
	rcBtn.right = rcClient.right - m_nBtnSpace;
	rcBtn.left = rcBtn.right - m_nBtnWidth;
	rcBtn.top = 2;
	rcBtn.bottom = rcBtn.top + m_nBtnHeight;
	m_btnSound.MoveWindow(rcBtn);

	rcBtn.OffsetRect(-(m_nBtnWidth + m_nBtnSpace), 0);
	m_btnSave.MoveWindow(rcBtn);

	rcBtn.OffsetRect(-(m_nBtnWidth + m_nBtnSpace), 0);
	m_btnCapture.MoveWindow(rcBtn);

	rcBtn.OffsetRect(-(m_nBtnWidth + m_nBtnSpace), 0);
	m_btnDownload.MoveWindow(rcBtn);
}

void CPlayMiniToolBarExpandWnd::SetToolBarBtnTips()
{
	m_btnDownload.SetTooltipText(_T("����¼��"));
	m_btnCapture.SetTooltipText(_T("ץͼ"));
	m_btnSave.SetTooltipText(_T("����"));
	m_btnSound.SetTooltipText(_T("������"));
}

/**   @fn          GetWndWidth
 *    @brief   	   ��ȡ���ڿ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      ���.
 */
int CPlayMiniToolBarExpandWnd::GetWndWidth()
{
	return (m_nBtnNum * m_nBtnWidth) + ((m_nBtnNum + 1) * m_nBtnSpace);
}

/**   @fn          GetWndHeight
 *    @brief   	   ��ȡ���ڸ߶�.
 *    @param[in]   
 *    @param[in]   
 *    @return      �߶�.
 */
int CPlayMiniToolBarExpandWnd::GetWndHeight()
{
	return m_nBtnHeight + 4;
}

void CPlayMiniToolBarExpandWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDT_VOLUMNWNDCLOSE_TIMER == nIDEvent)
	{
		m_uiIdleTime += 1;

		if(m_uiIdleTime >= FLOATWND_CLOSE_TIME)
		{
			POINT pt;
			GetCursorPos(&pt);

			CRect rectWindow(0,0,0,0);

			GetWindowRect(&rectWindow);

			if(rectWindow.PtInRect(pt))
			{
				m_uiIdleTime = 0;
			}
			else
			{
				ShowWindow(SW_HIDE);
				m_uiIdleTime = 0;
			}
		}

	}
	
	CWnd::OnTimer(nIDEvent);
}

void CPlayMiniToolBarExpandWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_uiIdleTime = 0;
	CWnd::OnMouseMove(nFlags, point);
}


//������ʱ��
BOOL CPlayMiniToolBarExpandWnd::SetCloseTimer(void)
{
	if(m_uiCloseTimer != 0)
	{
		KillColseTimer();
	}

	m_uiIdleTime = 0;
	m_uiCloseTimer = SetTimer(IDT_VOLUMNWNDCLOSE_TIMER, 1000, NULL);

	return true;
}

//ֹͣʱ��
void CPlayMiniToolBarExpandWnd::KillColseTimer(void)
{
	if(m_uiCloseTimer != 0)
	{
		KillTimer(IDT_VOLUMNWNDCLOSE_TIMER);
		m_uiCloseTimer = 0;
	}

	m_uiIdleTime = 0;
}

LRESULT CPlayMiniToolBarExpandWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if((message == WM_ACTIVATE)&&(wParam == WA_INACTIVE))
	{
		ShowWindow(SW_HIDE);
	}

	return CWnd::DefWindowProc(message, wParam, lParam);
}

void CPlayMiniToolBarExpandWnd::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CWnd::PostNcDestroy();
}

void CPlayMiniToolBarExpandWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if(bShow)
	{
		//��������ʾʱ�����ô��ڽ���
		SetFocus();
		//���ô����Զ��ر�ʱ��
		SetCloseTimer();
	}
	else
	{
		//ȡ�������Զ��ر�ʱ��
		KillColseTimer();
		AnimateWindow(200, AW_HIDE|AW_BLEND);
		::PostMessage(m_pParentWnd->GetSafeHwnd(), MSG_MINITOOLBAR_EXPAND_CLOSE, NULL, NULL);
	}
}

//����
BOOL CPlayMiniToolBarExpandWnd::CreateFloatWindow(CWnd *pParentWnd)
{
	m_pParentWnd = pParentWnd;

	BOOL bRet =  CreateEx(WS_EX_TOPMOST,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW),(HBRUSH)GetStockObject(BLACK_BRUSH)),
		NULL, WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0,0,0,0), 0, 0, NULL);

	return bRet;
}

void CPlayMiniToolBarExpandWnd::OnBtnClick(UINT nID)
{
	if (m_pParentWnd != NULL)
	{
		::PostMessage(m_pParentWnd->GetSafeHwnd(), MSG_TOOLBAR_BTN_CLICK, nID, NULL);
	}
}

void CPlayMiniToolBarExpandWnd::OnToolBarBtnClkCapture()
{
	OnBtnClick(TOOL_CAPTURE_ID);
}

void CPlayMiniToolBarExpandWnd::OnToolBarBtnClkSave()
{
	OnBtnClick(TOOL_SAVEDATA_ID);
}

void CPlayMiniToolBarExpandWnd::OnToolBarBtnClkSound()
{
	OnBtnClick(TOOL_SOUND_ID);
}

void CPlayMiniToolBarExpandWnd::OnToolBarBtnClkDownload()
{
	KillColseTimer();
	AnimateWindow(200, AW_HIDE|AW_BLEND);
	::PostMessage(m_pParentWnd->GetSafeHwnd(), MSG_MINITOOLBAR_EXPAND_CLOSE, NULL, NULL);
	OnBtnClick(TOOL_DOWNLOAD_ID);
}

void CPlayMiniToolBarExpandWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	InitPos();
}


/**   @fn          UpdateBtnState
 *    @brief   	   ���°�ť״̬.
 *    @param[in]   bPlay:�Ƿ��ǲ���״̬.
 *    @param[in]   bOpenSound:�Ƿ������Ƶ.
 *    @param[in]   bSave:�Ƿ��Ǳ�������״̬.
 *    @return      
 */
void CPlayMiniToolBarExpandWnd::UpdateBtnState(BOOL bOpenSound, BOOL bSave)
{
	// ��������. ͼ����ʾ��һ���Ĳ��������ǵ�ǰ����״̬
	if (bOpenSound)
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_VOLUMN),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSEVOLUMN_HOVER));
		m_btnSound.SetTooltipText(_T("�ر�����"));
	}
	// û�д�����.
	else
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN_HOVER));
		m_btnSound.SetTooltipText(_T("������"));
	}

	// ���ڼ�����.
	if (bSave)
	{
		m_btnSave.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE_HOVER),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE));
		m_btnSave.SetTooltipText(_T("ֹͣ����"));
	}
	// û�м���.
	else
	{
		m_btnSave.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE_HOVER));
		m_btnSave.SetTooltipText(_T("����"));
	}
}

/**   @fn          UpdateButtonTips
 *    @brief   	   ���°�ťtips.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayMiniToolBarExpandWnd::UpdateButtonTips()
{
	m_btnDownload.UpdateTooltip();
	m_btnSound.UpdateTooltip();
	m_btnSave.UpdateTooltip();
	m_btnCapture.UpdateTooltip();
}