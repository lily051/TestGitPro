// PlayMiniToolBarExpandWnd.cpp : 实现文件
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



// CPlayMiniToolBarExpandWnd 消息处理程序
//边框绘制
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	// TODO:  在此添加您专用的创建代码
	CreateChildWnd();
	InitCtrl();
	InitPos();
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);
	return 0;
}

void CPlayMiniToolBarExpandWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
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
	m_btnDownload.SetTooltipText(_T("下载录像"));
	m_btnCapture.SetTooltipText(_T("抓图"));
	m_btnSave.SetTooltipText(_T("剪辑"));
	m_btnSound.SetTooltipText(_T("打开声音"));
}

/**   @fn          GetWndWidth
 *    @brief   	   获取窗口宽度.
 *    @param[in]   
 *    @param[in]   
 *    @return      宽度.
 */
int CPlayMiniToolBarExpandWnd::GetWndWidth()
{
	return (m_nBtnNum * m_nBtnWidth) + ((m_nBtnNum + 1) * m_nBtnSpace);
}

/**   @fn          GetWndHeight
 *    @brief   	   获取窗口高度.
 *    @param[in]   
 *    @param[in]   
 *    @return      高度.
 */
int CPlayMiniToolBarExpandWnd::GetWndHeight()
{
	return m_nBtnHeight + 4;
}

void CPlayMiniToolBarExpandWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_uiIdleTime = 0;
	CWnd::OnMouseMove(nFlags, point);
}


//开启定时器
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

//停止时钟
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
	// TODO: 在此添加专用代码和/或调用基类
	if((message == WM_ACTIVATE)&&(wParam == WA_INACTIVE))
	{
		ShowWindow(SW_HIDE);
	}

	return CWnd::DefWindowProc(message, wParam, lParam);
}

void CPlayMiniToolBarExpandWnd::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CWnd::PostNcDestroy();
}

void CPlayMiniToolBarExpandWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if(bShow)
	{
		//当窗口显示时，设置窗口焦点
		SetFocus();
		//设置窗口自动关闭时钟
		SetCloseTimer();
	}
	else
	{
		//取消窗口自动关闭时钟
		KillColseTimer();
		AnimateWindow(200, AW_HIDE|AW_BLEND);
		::PostMessage(m_pParentWnd->GetSafeHwnd(), MSG_MINITOOLBAR_EXPAND_CLOSE, NULL, NULL);
	}
}

//创建
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

	// TODO: 在此处添加消息处理程序代码
	InitPos();
}


/**   @fn          UpdateBtnState
 *    @brief   	   更新按钮状态.
 *    @param[in]   bPlay:是否是播放状态.
 *    @param[in]   bOpenSound:是否打开了音频.
 *    @param[in]   bSave:是否是保存数据状态.
 *    @return      
 */
void CPlayMiniToolBarExpandWnd::UpdateBtnState(BOOL bOpenSound, BOOL bSave)
{
	// 打开了声音. 图标显示下一步的操作，不是当前声音状态
	if (bOpenSound)
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_VOLUMN),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSEVOLUMN_HOVER));
		m_btnSound.SetTooltipText(_T("关闭声音"));
	}
	// 没有打开声音.
	else
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_VOLUMN_HOVER));
		m_btnSound.SetTooltipText(_T("打开声音"));
	}

	// 正在剪辑中.
	if (bSave)
	{
		m_btnSave.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE_HOVER),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE));
		m_btnSave.SetTooltipText(_T("停止剪辑"));
	}
	// 没有剪辑.
	else
	{
		m_btnSave.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_MINIEXPAND_BTNBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SAVE_HOVER));
		m_btnSave.SetTooltipText(_T("剪辑"));
	}
}

/**   @fn          UpdateButtonTips
 *    @brief   	   更新按钮tips.
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