/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SearchEditCtrl.h
*   Description:    搜索控件
*   Author:         Zhengjinrong(Zhengjinrong@hikvision.com)
*   Date:           2009-11-26
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2009-11-26      Zhengjinrong   created
**************************************************************************/

//lint -library
#include "StdAfx.h"
#include "SearchEditCtrl.h"
#include "deelx.h"
#include ".\searcheditctrl.h"
#include "./memdcex.h"



#define   EDITCTRL_CLASSNAME     _T("SearchEditCtrl")

#define	  FONT_COLOR			RGB(166, 166, 166)
#define   ID_BTN_SEARCH			8000
#define   ID_BTN_NEXT			ID_BTN_SEARCH + 1
#define   ID_BTN_PER			ID_BTN_SEARCH + 2
#define   ID_EDIT_SEARCH		ID_BTN_SEARCH + 3
#define   ID_STATIC_DISP		ID_BTN_SEARCH + 4
#define   WM_SEARCH_END			WM_USER + 8000

#define   BTN_WIDTH				17
#define   BTN_HEIGHT			19
#define   BTN_WIDTH_SEARCH		15
#define   BTN_HEIGHT_SEARCH		16

IMPLEMENT_DYNAMIC(CSearchEditCtrl, CWnd)

CSearchEditCtrl::CSearchEditCtrl():
m_nIndex(0),
m_clThreadActive(TRUE),
m_pCallBack(NULL),
m_lParam(NULL)
{
	WNDCLASS wndcls;

	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, EDITCTRL_CLASSNAME, &wndcls)))
	{
		wndcls.style			= CS_SAVEBITS;
		wndcls.lpfnWndProc		= ::DefWindowProc;
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
		wndcls.hInstance		= hInst;
		wndcls.hIcon			= NULL;
		wndcls.hCursor			= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= NULL;
		wndcls.lpszMenuName		= NULL;
		wndcls.lpszClassName	= EDITCTRL_CLASSNAME;

		BOOL ix=AfxRegisterClass(&wndcls);

		if (!ix)	
			AfxThrowResourceException();
	} 

	m_bEmapSearch = FALSE;
	m_dqTreeItem.clear();

	ZeroMemory(&m_szKey, sizeof(TCHAR)*MAX_PATH);
}

CSearchEditCtrl::~CSearchEditCtrl()
{
	if (m_clFont.m_hObject)
	{
		DeleteObject(m_clFont.m_hObject);
		m_clFont.m_hObject = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSearchEditCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_BTN_SEARCH, ID_BTN_PER, OnClkEventBtn)
	ON_MESSAGE(WM_SEARCH_END, OnSearchEnd)
	ON_WM_ERASEBKGND()
	ON_EN_SETFOCUS(ID_EDIT_SEARCH, OnSetfocusEdit)
	ON_EN_KILLFOCUS(ID_EDIT_SEARCH, OnKillfocusEdit)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// OnCreate
int CSearchEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);

	CreateChildCtrl();

	InitUI();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnSize
void CSearchEditCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	MovePos();
}

///////////////////////////////////////////////////////////////////////////////
// OnClkEventBtn
void CSearchEditCtrl::OnClkEventBtn(UINT nID) 
{
	switch( nID)
	{
	case ID_BTN_SEARCH:
		OnBtnClickSearch();
		break;
	case ID_BTN_NEXT:
		OnBtnClickNext();
		break;
	case ID_BTN_PER:
		OnBtnClickPer();
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnSearchEnd
LRESULT CSearchEditCtrl::OnSearchEnd(WPARAM wparam, LPARAM lparam)
{
	if (m_dqTreeItem.size())
	{
		HTREEITEM hItem = m_dqTreeItem[0];

		ASSERT(m_pTreeCtrl != NULL);

		if (hItem)
		{
			m_pTreeCtrl->SelectItem(hItem);
			//ljg设置被选中状态2010-7-29 14:45:11
			if (m_bEmapSearch)
			{
				m_pTreeCtrl->SetCheck(hItem,TRUE);
			}
		}

		m_nIndex = 0;

		if (m_dqTreeItem.size() > 1)
		{
			m_btnNext.EnableWindow(TRUE);
		}
	}
	else
	{
		MessagCallBack(MB_OK|MB_ICONWARNING, _T("没有找到符合的节点!"));
		m_btnNext.EnableWindow(FALSE);
		m_btnPer.EnableWindow(FALSE);
	}

	m_bEmapSearch = FALSE;

	return 0L;
}

/*******************************************************************************
*    Function:      Create
*    Description:   创建窗口
*    Input:         pParentWnd 父窗口指针
pCallBack 消息回调函数
lParam 回调函数类指针
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败  
*******************************************************************************/
BOOL CSearchEditCtrl::CreateCtrl(CWnd * pParentWnd, MsgCallBack pCallBack/* = NULL*/, LPARAM lParam /*=0*/)
{
	ASSERT_VALID(pParentWnd);

	m_pCallBack = pCallBack;
	m_lParam = lParam;
	DWORD dwStyle = WS_CHILD |WS_VISIBLE; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW;

	BOOL ix = CreateEx(dwExStyle, EDITCTRL_CLASSNAME, NULL, dwStyle, 0, 0, 10, 10, pParentWnd->GetSafeHwnd(), NULL, NULL);
	//BOOL ix = CreateEx(dwExStyle, EDITCTRL_CLASSNAME, NULL, dwStyle, CRect(0,0,0,0), pParentWnd, 2700, NULL);
	//BOOL ix = Create(NULL, 0, WS_CHILD |WS_VISIBLE, CRect(0,0,0,0), this, 2700);

	return ix;
}

/*******************************************************************************
*    Function:      Attach
*    Description:   绑定控件
*    Input:         pTreeCtrl 树控件
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::Attach(CTreeCtrl* pTreeCtrl)
{
	m_pTreeCtrl = pTreeCtrl;
}

/*******************************************************************************
*    Function:      ThreadProc
*    Description:   线程函数
*    Input:         NULl
*    Output:        NULl
*    Return:	    return DWORD  
*******************************************************************************/
DWORD CSearchEditCtrl::ThreadProc()
{
	m_clThreadActive.Reset();

	BOOL bSearchFinished = DoSearch();

	if(bSearchFinished)
	{
		PostMessage(WM_SEARCH_END, 0, 0);
	}

	m_clThreadActive.Set();

	return 0;
}

/*******************************************************************************
*    Function:      NeedSearchThisItem
*    Description:   是否需要搜索节点
*    Input:         hItem 树节点
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败  
*******************************************************************************/
BOOL CSearchEditCtrl::NeedSearchThisItem(HTREEITEM hItem)
{
	return TRUE;
}

/*******************************************************************************
*    Function:      ProcessItem
*    Description:   搜索函数
*    Input:         hItem 树节点
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败  
*******************************************************************************/
BOOL CSearchEditCtrl::ProcessItem(HTREEITEM hItem)
{
	ASSERT(hItem != NULL);

	CString strText = m_pTreeCtrl->GetItemText(hItem);

	CRegexpT<TCHAR> Regexp(m_szKey, IGNORECASE);
	MatchResult Result = Regexp.Match(strText);
	int nMatchResult = Result.IsMatched();

	if (nMatchResult != 0)
	{
		m_dqTreeItem.push_back(hItem);
	}

	return TRUE;
}

/*******************************************************************************
*    Function:      CreateChildCtrl
*    Description:   创建子控件
*    Input:         NULl
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::CreateChildCtrl()
{
	m_btnSearch.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,1,1), this, ID_BTN_SEARCH);

	m_btnNext.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,1,1), this, ID_BTN_NEXT);

	m_btnPer.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
		CRect(0,0,1,1), this, ID_BTN_PER);
	
	m_clEdit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			CRect(0,0,1,1), this, ID_EDIT_SEARCH);

	m_clStcText.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		CRect(0,0,1,1), this, ID_STATIC_DISP);

	m_tooltip.Create(this);   
	m_tooltip.Activate(TRUE);   
}

/*******************************************************************************
*    Function:      InitUI
*    Description:   初始化控件
*    Input:         NULl
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::InitUI()
{
	if (m_clFont.m_hObject == NULL)
	{
		int nHeight = GetFontSize(GetFontSize0());
		m_clFont.CreateFont(nHeight , 0, 0,0, FW_NORMAL, 0, 0, 0, GetFontCharset(),
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH|FF_DONTCARE, GetFontName());
	}

	m_btnNext.Load(RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SNEXT),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SNEXTSEL),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SNEXTDIS));
	m_btnNext.EnableWindow(FALSE);

	m_btnPer.Load(RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SPRE),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SPRESEL),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SPREDIS));
	m_btnPer.EnableWindow(FALSE);

	m_btnSearch.Load(RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SEARCH),
		RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CTRL_SEARCHSEL));

	m_clEdit.SetFont(&m_clFont);
	m_clStcText.SetFont(&m_clFont);
	m_clStcText.SetWindowText(_T("输入要查找的内容"));

	m_tooltip.AddTool(&m_btnNext, _T("查找下一个"));   
	m_tooltip.AddTool(&m_btnPer, _T("查找上一个"));   
	m_tooltip.AddTool(&m_btnSearch, _T("查找"));   

	MovePos();
}

/*******************************************************************************
*    Function:      MovePos
*    Description:   窗口重新调整
*    Input:         NULl 
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::MovePos()
{
	CRect rcClient, rcTmp;
	GetClientRect(&rcClient);
	
	rcClient.DeflateRect(4, 4);
	rcTmp = rcClient;

	if (IsWindow(m_btnNext.m_hWnd))
	{
		rcTmp.left = rcTmp.right - BTN_WIDTH;
		rcTmp.bottom = rcTmp.top + BTN_HEIGHT;

		m_btnNext.MoveWindow(rcTmp);
	}

	if (IsWindow(m_btnPer.m_hWnd))
	{
		rcTmp.OffsetRect(-(2 + BTN_WIDTH), 0);
		m_btnPer.MoveWindow(rcTmp);
	}

	if (IsWindow(m_btnSearch.m_hWnd))
	{
		rcTmp.OffsetRect(-(12 + BTN_WIDTH_SEARCH), 3);
		rcTmp.DeflateRect(0, 0, 2, 3);
		m_btnSearch.MoveWindow(rcTmp);
	}

	if (IsWindow(m_clEdit.m_hWnd))
	{
		rcTmp.right = rcTmp.left - 2;
		rcTmp.left = rcClient.left + 10;
		m_clEdit.MoveWindow(rcTmp);
	}

	if (IsWindow(m_clStcText.m_hWnd))
	{
		m_clStcText.MoveWindow(rcTmp);
	}
}

/*******************************************************************************
*    Function:      OnBtnClickSearch
*    Description:   搜索消息响应函数
*    Input:         NULl
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::OnBtnClickSearch()
{
	ASSERT(m_pTreeCtrl != NULL);
	if (m_pTreeCtrl == NULL)
		return;
	
	SetEndofSearch();

	CloseHandle(m_hThread);
	m_hThread = NULL;

	CAMDVDThread::Create();
}

/*******************************************************************************
*    Function:      OnBtnClickNext
*    Description:   搜索下一个函数
*    Input:         NULl
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::OnBtnClickNext()
{
	ASSERT(m_pTreeCtrl != NULL);
	if (m_pTreeCtrl == NULL)
		return;

	if (m_dqTreeItem.size() == 0)
	{
		return;
	}

	if (m_nIndex >= ((int)m_dqTreeItem.size()-1))
	{
		m_btnNext.EnableWindow(FALSE);
		MessagCallBack(MB_OK|MB_ICONWARNING, _T("已经是最后一个节点!"));
		return;
	}

	++m_nIndex;

	HTREEITEM hItem = m_dqTreeItem[m_nIndex];

	ASSERT(hItem != NULL);

	if (hItem)
	{
		ASSERT(m_pTreeCtrl != NULL);

		m_pTreeCtrl->SelectItem(hItem);
	}

	m_btnPer.EnableWindow(TRUE);
}

/*******************************************************************************
*    Function:      OnBtnClickNext
*    Description:   搜索下一个函数
*    Input:         NULl
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::OnBtnClickPer()
{
	ASSERT(m_pTreeCtrl != NULL);
	if (m_pTreeCtrl == NULL)
		return;

	if (m_dqTreeItem.size() == 0)
	{
		return;
	}

	if (m_nIndex == 0)
	{
		m_btnPer.EnableWindow(FALSE);
		MessagCallBack(MB_OK|MB_ICONWARNING, _T("已经是第一个节点!"));
		return;
	}

	--m_nIndex;

	HTREEITEM hItem = m_dqTreeItem[m_nIndex];

	ASSERT(hItem != NULL);

	if (hItem)
	{
		ASSERT(m_pTreeCtrl != NULL);
		
		m_pTreeCtrl->SelectItem(hItem);
	}

	m_btnNext.EnableWindow(TRUE);
}


/*******************************************************************************
*    Function:      InitSerach
*    Description:   初始化搜索
*    Input:         NULl
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败 
*******************************************************************************/
BOOL CSearchEditCtrl::InitSerach()
{
	CFindItem::InitSearch();
	
	m_nIndex = 0;
	m_dqTreeItem.clear();

	m_clEdit.GetWindowText(m_szKey, MAX_PATH);

	if (m_szKey[0] == '\0')
	{
		return FALSE;
	}
	return TRUE;
}

/*******************************************************************************
*    Function:      DoSearch
*    Description:   开始搜索
*    Input:         NULl
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败  
*******************************************************************************/
BOOL CSearchEditCtrl::DoSearch()
{
	if (!InitSerach())
	{
		return TRUE;
	}

	BOOL bFinish = FALSE;

	HTREEITEM hRoot = m_pTreeCtrl->GetRootItem();

	while(hRoot)
	{
		 ProcessItem(hRoot);

		 bFinish = RecursiveSearchItem(hRoot);

		 hRoot = m_pTreeCtrl->GetNextSiblingItem(hRoot);
	}

	return bFinish;
}

/*******************************************************************************
*    Function:      EndOfSearch
*    Description:   结束搜索
*    Input:         NULl
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::EndOfSearch()
{
	SetEndofSearch();
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CSearchEditCtrl::OnEraseBkgnd(CDC* pDC)
{ 
	// TODO: Add your message handler code here and/or call default

	CRect clientRC;
	GetClientRect(&clientRC);
	CMemDCEX memDC(pDC,clientRC);
  
	CBrush br(UI_WNDSPACE_BKGND);   
	memDC.FillRect(clientRC, &br);   

	DarwBK(&memDC);

	DarwEditBK(&memDC);

	return TRUE;

	//return CWnd::OnEraseBkgnd(pDC);
}

/*******************************************************************************
*    Function:      DarwBK
*    Description:   重绘背景
*    Input:         pDC DC句柄
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::DarwBK(CDC* pDC)
{
	ASSERT(pDC != NULL);

	CRect clientRC;
	GetClientRect(&clientRC);

	//CMemDCEX memDC(pDC,clientRC);

	CDC compatibleDC;
	compatibleDC.CreateCompatibleDC(pDC);

	BITMAP btLeft,btMid,btRight;
	CRect leftRC, midRC,rightRC;

	CBitmap bkLeft,bkMid,bkRight;   
	bkLeft.Attach(RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CTRL_SBKLEFT, Color(255,255,255,0)));
	bkLeft.GetBitmap(&btLeft);
	bkMid.Attach(RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CTRL_SBKMIDDLE, Color(255,255,255,0)));
	bkMid.GetBitmap(&btMid);
	bkRight.Attach(RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CTRL_SBKRIGHT, Color(255,255,255,0)));
	bkRight.GetBitmap(&btRight);

// 	//左边
// 	leftRC.left = clientRC.left;
// 	leftRC.right = leftRC.left + btLeft.bmWidth;
// 	leftRC.top = clientRC.top;
// 	leftRC.bottom = leftRC.top + btLeft.bmHeight;
// 
// 	CBitmap *pOldBmp = compatibleDC.SelectObject(&bkLeft);
// 	pDC->StretchBlt(leftRC.left,leftRC.top,leftRC.Width(),leftRC.Height(),&compatibleDC,0,0,btLeft.bmWidth,btLeft.bmHeight,SRCCOPY);
// 
// 	//右边 
// 	rightRC.right = clientRC.right;
// 	rightRC.left = rightRC.right - btRight.bmWidth;
// 	rightRC.top = clientRC.top;
// 	rightRC.bottom = rightRC.top + btLeft.bmHeight;
// 
// 	compatibleDC.SelectObject(&bkRight);
// 	pDC->StretchBlt(rightRC.left,rightRC.top,rightRC.Width(),rightRC.Height(),&compatibleDC,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);

	//中间
	midRC.left = clientRC.right;
	midRC.right = clientRC.left;
	midRC.top = clientRC.top;
	midRC.bottom = midRC.top + btMid.bmHeight;

	CBitmap *pOldBmp = compatibleDC.SelectObject(&bkMid);
	pDC->StretchBlt(midRC.left,midRC.top,midRC.Width(),midRC.Height(),&compatibleDC,0,0,btMid.bmWidth,btMid.bmHeight,SRCCOPY);

	compatibleDC.SelectObject(pOldBmp);

	compatibleDC.DeleteDC(); 

	DeleteObject(bkLeft.Detach());
	DeleteObject(bkMid.Detach());
	DeleteObject(bkRight.Detach());
}

/*******************************************************************************
*    Function:      DarwEditBK
*    Description:   重绘编辑框背景
*    Input:         pDC DC句柄
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::DarwEditBK(CDC* pDC)
{
	ASSERT(pDC != NULL);

	CRect clientRC;
	GetClientRect(&clientRC);

	int nOffsetX = 6;
	int noffSetY = 1;
	clientRC.DeflateRect(nOffsetX, 0, 45, 0);

	CDC compatibleDC;
	compatibleDC.CreateCompatibleDC(pDC);

	BITMAP btLeft,btMid,btRight;
	CRect leftRC, midRC,rightRC;

	CBitmap bkLeft,bkMid,bkRight;   
	bkLeft.Attach(RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CTRL_SEDITLEFT, Color(255,255,255,0)));
	bkLeft.GetBitmap(&btLeft);
	bkMid.Attach(RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CTRL_SEDITMIDDLE, Color(255,255,255,0)));
	bkMid.GetBitmap(&btMid);
	bkRight.Attach(RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CTRL_SEDITRIGHT, Color(255,255,255,0)));
	bkRight.GetBitmap(&btRight);

	//左边
	leftRC.left = clientRC.left;
	leftRC.right = leftRC.left + btLeft.bmWidth;
	leftRC.top = clientRC.top;
	leftRC.bottom = leftRC.top + btLeft.bmHeight;

	CBitmap *pOldBmp = compatibleDC.SelectObject(&bkLeft);
	pDC->StretchBlt(leftRC.left,leftRC.top,leftRC.Width(),leftRC.Height(),&compatibleDC,0,0,btLeft.bmWidth,btLeft.bmHeight,SRCCOPY);

	//右边 
	rightRC.right = clientRC.right;
	rightRC.left = rightRC.right - btRight.bmWidth;
	rightRC.top = clientRC.top;
	rightRC.bottom = rightRC.top + btLeft.bmHeight;

	compatibleDC.SelectObject(&bkRight);
	pDC->StretchBlt(rightRC.left,rightRC.top,rightRC.Width(),rightRC.Height(),&compatibleDC,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);

	//中间
	midRC.left = leftRC.right;
	midRC.right = rightRC.left;
	midRC.top = clientRC.top;
	midRC.bottom = midRC.top + btMid.bmHeight;

	compatibleDC.SelectObject(&bkMid);
	pDC->StretchBlt(midRC.left, midRC.top, midRC.Width(), midRC.Height(), &compatibleDC,0,0,btMid.bmWidth,btMid.bmHeight,SRCCOPY);

	compatibleDC.SelectObject(pOldBmp);

	compatibleDC.DeleteDC(); 

	DeleteObject(bkLeft.Detach());
	DeleteObject(bkMid.Detach());
	DeleteObject(bkRight.Detach());
}

/*******************************************************************************
*    Function:      MessagCallBack
*    Description:   消息回调函数
*    Input:         nType 消息类型
lpszText 消息内容
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
void CSearchEditCtrl::MessagCallBack(UINT nType, LPCTSTR lpszText)
{
	if (m_pCallBack && m_lParam)
	{
		m_pCallBack(nType, lpszText, m_lParam);
	}
}

BOOL CSearchEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBtnClickSearch();
	}

	m_tooltip.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

void CSearchEditCtrl::OnSetfocusEdit() 
{
	// TODO: Add your control notification handler code here
	if (m_clStcText.IsWindowVisible())
	{
		m_clStcText.ShowWindow(SW_HIDE);
	}
	
	CString strText;
	m_clEdit.GetWindowText(strText);
	m_clEdit.PostMessage(EM_SETSEL, 0, -1);
}

void CSearchEditCtrl::OnKillfocusEdit() 
{
	// TODO: Add your control notification handler code here
	CString strText;
	m_clEdit.GetWindowText(strText);
	if (strText.IsEmpty())
	{
		m_clStcText.ShowWindow(SW_SHOW);
	}
}
HBRUSH CSearchEditCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	static CBrush clBrush(UI_WNDSPACE_BKGND);
	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == ID_STATIC_DISP)
	{
		pDC->SetTextColor(FONT_COLOR);
		pDC->SetBkMode(TRANSPARENT);
		return clBrush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

//电子地图查找
BOOL CSearchEditCtrl::Emap_Search(CString strKey)
{
	m_bEmapSearch = TRUE;
	m_clEdit.SetWindowText(strKey);//设置输入框的内容
	m_clStcText.ShowWindow(SW_HIDE);//隐藏输入提示
	OnClkEventBtn(ID_BTN_SEARCH); //按查找按钮
	return TRUE;
}
