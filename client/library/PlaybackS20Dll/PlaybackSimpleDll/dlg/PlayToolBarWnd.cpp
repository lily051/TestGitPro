// PlayToolBarDlg.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "PlayToolBarWnd.h"
#include "../ui/uiCtrl/memdcex.h"
#include "./PlaybackWnd/PlaybackWnd.h"



// CPlayToolBarDlg 对话框

IMPLEMENT_DYNAMIC(CPlayToolBarWnd, CWnd)

CPlayToolBarWnd::CPlayToolBarWnd(CWnd* pParent /*=NULL*/)
{
    m_bMove = FALSE;
	m_bPlayState = FALSE;
}

CPlayToolBarWnd::~CPlayToolBarWnd()
{

}

BEGIN_MESSAGE_MAP(CPlayToolBarWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(TOOL_GROUPCFG_ID, OnBnClickPlayGroupCfg)
	ON_BN_CLICKED(TOOL_STOPALL_ID, OnBnClickStopAll)
	ON_BN_CLICKED(TOOL_VOLUMN_ID, OnBnClickVolumn)
	ON_BN_CLICKED(TOOL_FULLSCREEN_ID, OnBnClickFullScreen)
	ON_BN_CLICKED(TOOL_PLAY_ID, OnBnClickPlay)
	ON_BN_CLICKED(TOOL_SOUND_ID, OnBnClickSound)
    ON_BN_CLICKED(TOOL_CAPTURE_ID, OnBnClickAllCapture)
END_MESSAGE_MAP()


int CPlayToolBarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	CreateChildCtrl();

	InitCtrl();

	m_bMove = TRUE;

	return 0;
}

void CPlayToolBarWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CPlayToolBarWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bMove)
	{
		MoveCtrl();
	}
}

//创建子控件
void CPlayToolBarWnd::CreateChildCtrl()
{
	CRect rcBtn(0, 0, 34, 34);

	m_btnStopAll.Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcBtn, this, TOOL_STOPALL_ID);
	m_btnSound.Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcBtn, this, TOOL_SOUND_ID);
	m_btnGroupCfg.Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcBtn, this, TOOL_GROUPCFG_ID);
	m_btnPlay.Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcBtn, this, TOOL_PLAY_ID);
	if (theApp.m_stRights.bCapture)
	{
		m_btnAllCapture.Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcBtn, this, TOOL_CAPTURE_ID);
	}
	m_btnFullScreen.Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcBtn, this, TOOL_FULLSCREEN_ID);
}

//初始化子控件
void CPlayToolBarWnd::InitCtrl()
{
	m_DrawBakeFun.DrawStrip_Loadbmp(
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_TOOLBARBAKE,Color(0,0,0,255)),
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_TOOLBARBAKE,Color(0,0,0,255)),
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_TOOLBARBAKE,Color(0,0,0,255)));

	m_btnGroupCfg.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_PLAYGROUPCFG),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
	m_btnGroupCfg.ShowWindow(SW_HIDE);

	if (theApp.m_emLayoutStyle == LAYOUT_WND_1)
	{
		m_btnStopAll.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_ALLSTART),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
	}
	else
	{
		m_btnStopAll.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_STOP),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
	}

	m_btnSound.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_SOUND),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));

    m_btnAllCapture.LoadbakeImage(
        RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
        RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_CAPTURE),
        RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));

	m_btnPlay.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_PLAY),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
    m_btnPlay.ShowWindow(SW_HIDE);

	m_btnFullScreen.LoadbakeImage(
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_FULLSCREEN),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));

	SetButtonTips();

	m_btnSound.EnableWindow(FALSE);
	m_btnPlay.EnableWindow(FALSE);
}

//移动子控件
void CPlayToolBarWnd::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nBtnWidth = 34;			//通用按钮宽度
	int nBtnHeight = 34;		//按钮高度
	int nTop = 5;               //top
	int nSpace = 4;             //按钮间距

	//left
	int nLeft = rcClient.left + 12;
	//m_btnPlay.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
	
	//nLeft = nLeft + nBtnWidth + nSpace * 5;
	m_btnStopAll.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);

	nLeft = nLeft + nBtnWidth + nSpace;
	m_btnAllCapture.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);

	//right
	nLeft = rcClient.right - nBtnHeight - nSpace;
 	m_btnFullScreen.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);

    nLeft = nLeft - nBtnHeight - nSpace;
    m_btnSound.MoveWindow(nLeft, nTop, nBtnWidth, nBtnHeight);
}

BOOL CPlayToolBarWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);

	CMemDCEX memDC(pDC,rectClient);
	m_DrawBakeFun.DrawStrip_DrawFun(&memDC,&rectClient);

	return TRUE;
    //return CWnd::OnEraseBkgnd(pDC);
}


void CPlayToolBarWnd::OnBnClickPlayGroupCfg()
{
// 	CPlayGroupCfg dlgPlayGroup;
// 	if (IDOK == dlgPlayGroup.DoModal())
// 	{
// 		// 如果分组配置有改变.
// 		if (dlgPlayGroup.IsGroupCfgChange())
// 		{
// 			// 自动刷新回放分组.
// 			::PostMessage(GetParent()->GetSafeHwnd(), MSG_GROUPCFG_CHANGE, 0, 0);
// 		}
// 	}
}



void CPlayToolBarWnd::OnBnClickStopAll()
{
	if (theApp.m_emLayoutStyle == LAYOUT_WND_1)
	{
		if (m_bPlayState)
		{
			OnClkToolBarBtn(TOOL_STOPALL_ID, NULL);
		}
		else
		{
			OnClkToolBarBtn(TOOL_PLAY_ID, NULL);
		}
	}
	else
	{
		OnClkToolBarBtn(TOOL_STOPALL_ID, NULL);
	}
}

void CPlayToolBarWnd::OnBnClickVolumn()
{
	OnClkToolBarBtn(TOOL_VOLUMN_ID, NULL);
}

void CPlayToolBarWnd::OnBnClickFullScreen()
{
	OnClkToolBarBtn(TOOL_FULLSCREEN_ID, NULL);
}

void CPlayToolBarWnd::OnBnClickPlay()
{
	OnClkToolBarBtn(TOOL_PLAY_ID, NULL);
}

void CPlayToolBarWnd::OnBnClickSound()
{
	OnClkToolBarBtn(TOOL_SOUND_ID, NULL);
}

void CPlayToolBarWnd::OnBnClickAllCapture()
{
    OnClkToolBarBtn(TOOL_CAPTURE_ID, NULL);
}

//控制全屏按钮状态
void CPlayToolBarWnd::SetFullScreenOn(BOOL bFull)
{
	if (bFull)
	{
		m_btnFullScreen.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_ESCFULLSCREEN),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
	}
	else
	{
		m_btnFullScreen.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_FULLSCREEN),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
	}
}

void CPlayToolBarWnd::UpdatePlayState(BOOL bState)
{
	if (bState)
	{
		m_btnStopAll.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_STOP),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
		m_btnStopAll.SetTooltipText(_T("停止播放"));
	}
	else
	{
		m_btnStopAll.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDB_PNG_ALLSTART),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
		m_btnStopAll.SetTooltipText(_T("重新播放"));
	}
	m_bPlayState = bState;
}


//按钮点击消息传出
void CPlayToolBarWnd::OnClkToolBarBtn(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_TOOLBAR_BTN_CLICK, wParam, lParam);
}

//设置按钮提示符
void CPlayToolBarWnd::SetButtonTips()
{
	if (theApp.m_emLayoutStyle == LAYOUT_WND_1)
	{
		m_btnStopAll.SetTooltipText(_T("开始播放"));
		m_btnAllCapture.SetTooltipText(_T("回放抓图"));
	}
	else
	{
		m_btnStopAll.SetTooltipText(_T("停止全部"));
		m_btnAllCapture.SetTooltipText(_T("全部抓图"));
	}
	m_btnSound.SetTooltipText(_T("音量"));
	m_btnPlay.SetTooltipText(_T("重新播放"));
	m_btnGroupCfg.SetTooltipText(_T("分组配置"));
	m_btnFullScreen.SetTooltipText(_T("全屏切换"));
}

//刷新按钮状态
void CPlayToolBarWnd::UpdateBtnState(LPVOID lpPlayWnd)
{
	//获取刷新的窗口指针
	CPlaybackWnd * pPlayWnd = (CPlaybackWnd*)lpPlayWnd;
	if (pPlayWnd == NULL)
	{
		return;
	}

	//刷新
	if (pPlayWnd->m_hPlayback)
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
	}
	else
	{
		m_btnSound.LoadbakeImage(
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKE),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_SOUND),
			RESOURCELOAD_PNG_EX_IMG(IDR_PNG_TOOL_OTHERBAKESEL));
	}
	if (pPlayWnd->m_bSoundOpen)
	{
		m_btnSound.EnableWindow(TRUE);
	}
	else
	{
		m_btnSound.EnableWindow(FALSE);
	}

	if (pPlayWnd->IsPlayback()  ||  !pPlayWnd->IsUsed())
	{
		m_btnPlay.EnableWindow(FALSE);
	}
	else
	{
		m_btnPlay.EnableWindow(TRUE);
	}
}

BOOL CPlayToolBarWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::PreTranslateMessage(pMsg);
}

/**   @fn          UpdateButtonTips
 *    @brief   	   更新tooltip.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayToolBarWnd::UpdateButtonTips()
{
	m_btnStopAll.UpdateTooltip();
	m_btnSound.UpdateTooltip();
	m_btnPlay.UpdateTooltip();
	m_btnGroupCfg.UpdateTooltip();
	m_btnFullScreen.UpdateTooltip();
    m_btnAllCapture.UpdateTooltip();
}	