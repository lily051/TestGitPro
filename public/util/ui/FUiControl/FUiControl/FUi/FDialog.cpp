// FDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "FDialog.h"
#include "FMemDCEx.h"

// 系统按钮的ID
const int SYSBTN_CLOSE = 500;
const int SYSBTN_MAX   = 501;
const int SYSBTN_MIN   = 502;


// 初始化窗口阴影类（该类只能初始化一次）
static bool g_initShadow = false;
// CFDialog 对话框

IMPLEMENT_DYNAMIC(CFDialog, CDialog)

CFDialog::CFDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
, m_nTitleAlign(DT_LEFT)
, m_pImgBkg(NULL)
, m_clrTitle(FUi::White)
, m_bNcTracking(FALSE)
, m_nBtnHover(-1)
, m_strCaptionFont(YAHEI_20_SB)
, m_rcIcon(CRect(0,0,0,0))
, m_rcTitle(CRect(0,0,0,0))
,m_bDrawTitleIcon(TRUE)
,m_nTitleLeftMargin(10)	//默认10像素
,m_nTitleTextLeft(50)
{
    // 系统默认边框大小和标题栏高度
    int nSysCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
    int nSysBorderWidth = GetSystemMetrics(SM_CYDLGFRAME);

    m_vecMargin.push_back(nSysBorderWidth);
    m_vecMargin.push_back(nSysCaptionHeight + nSysBorderWidth);
    m_vecMargin.push_back(nSysBorderWidth);
    m_vecMargin.push_back(nSysBorderWidth);

    /*
    if (!g_initShadow)
    {
        g_initShadow = true;
        CWndShadow::Initialize(AfxGetInstanceHandle());
    }
    */
}

//lint -e1579
CFDialog::~CFDialog()
{
    try
    {
        if (m_Rgn.GetSafeHandle())
        {
            m_Rgn.DeleteObject();
        }

        DestorySysBtn();
    }
    catch (...)
    {
    }
}
//lint +e1579

void CFDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFDialog, CDialog)
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_NCACTIVATE()
    ON_WM_GETMINMAXINFO()
    ON_WM_NCHITTEST()
    ON_WM_NCCREATE()
    ON_WM_SIZE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCLBUTTONUP()
    ON_WM_NCMOUSELEAVE()
    ON_WM_NCMOUSEMOVE()
    ON_WM_SYSCOMMAND()
    //ON_WM_PAINT()
	ON_MESSAGE(WM_CTRL_TOOLTIP, &CFDialog::OnMsgCtrlToolTip)
	ON_MESSAGE(WM_CTRL_TOOLTIP_EVENT, &CFDialog::OnMsgCtrlToolTipEvent)
END_MESSAGE_MAP()

BOOL CFDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_toolTip.Create(this);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

BOOL CFDialog::PreTranslateMessage(MSG* pMsg)
{
    m_toolTip.RelayEvent(pMsg);

    return CDialog::PreTranslateMessage(pMsg);
}

// 重新设置窗口的边框（包括标题栏）的尺寸
void CFDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    if (bCalcValidRects)
    {
        CRect& rc = (CRect&)lpncsp->rgrc[0];//get the client rectangle  

        rc.left += m_vecMargin[0];
        rc.top += m_vecMargin[1]; 
        rc.right -= m_vecMargin[2]; 
        rc.bottom -= m_vecMargin[3];
    }
    // 这里不能再继续调用默认的OnNcCalcSize,否则会造成获取区域大小接口都返回异常数值
}

// 窗口最小的高度
void CFDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // 最小高度为上下边框的和
    lpMMI->ptMinTrackSize.y = m_vecMargin[1] + m_vecMargin[3];

    CDialog::OnGetMinMaxInfo(lpMMI);
}
// 处理窗口大小改变时，按钮的位置的改变
void CFDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    MoveSysBtn();

    //圆角
    CRect rcWnd;
    GetWindowRect(rcWnd);

    if (m_Rgn.GetSafeHandle())
    {
        m_Rgn.DeleteObject();
    }
    m_Rgn.CreateRoundRectRgn(0, 0, rcWnd.Width() + 1, rcWnd.Height() + 1, 1, 1); //根据窗口矩形创建一个圆角矩形
    SetWindowRgn(m_Rgn, TRUE); //根据圆角矩形指定的区域改变窗口的形状
}

// 非客户区绘制消息
void CFDialog::OnNcPaint()
{
    CDC* pWinDC = GetWindowDC();
    if (NULL == pWinDC)
    {
        return ;
    }

    // 绘制非客户区
    DrawNc(pWinDC);

    ReleaseDC(pWinDC);
}

void CFDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    CRect rcWnd;
    GetWindowRect(rcWnd);

    CPoint pt = point;
    pt.Offset(-rcWnd.left, -rcWnd.top);

    // 鼠标是否点击在按钮上
    int nBtnID = SysBtnHitTest(pt);
    if (m_mapSysBtn.find(nBtnID) != m_mapSysBtn.end())
    {
        //TRACE("鼠标是否点击在按钮上:%d\n", nBtnID);
        m_mapSysBtn[nBtnID]->LButtonDown();
        OnNcPaint();
        return ;    // 不调用系统默认，否则会绘制默认图标
    } 

    // 只处理点击图标区域和标题栏空白区域
    // 鼠标点击边框
    if(m_rcIcon.PtInRect(pt) || nHitTest == HTCAPTION ||
       (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST))
    {
        CDialog::OnNcLButtonDown(nHitTest, point);
        OnNcPaint();
        return;
    }
}

void CFDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    CRect rcWnd;
    GetWindowRect(rcWnd);

    CPoint pt = point;
    pt.Offset(-rcWnd.left, -rcWnd.top);

    // 获取鼠标弹起时所在按钮
    int nBtnID = SysBtnHitTest(pt);
    switch (nBtnID)
    {
    case SYSBTN_CLOSE:
        {
            SendMessage(WM_SYSCOMMAND, SC_CLOSE,0);
            break;
        }
    case SYSBTN_MAX:
        {
            if (IsZoomed())
            {
                SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            }
            else
            {
                SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
            }
            break;
        }
    case SYSBTN_MIN:
        {
            if (IsIconic())
            {
                SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            }
            else
            {
                SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
            }
            break;
        }
    case -1:    
        {
            // 没有在按钮区域
            CDialog::OnNcLButtonUp(nHitTest, point);
            return ;
        }
    default:
        {
            // 一般不会来这
			//m_mapSysBtn[nBtnID]->SetSelected();
            m_mapSysBtn[nBtnID]->LButtonUp();
            OnNcPaint();
            break;
        }
    }
}

// 鼠标鼠标离开非客户区
void CFDialog::OnNcMouseLeave()
{
    m_bNcTracking = FALSE;

    if(m_nBtnHover != -1)
    {
        m_mapSysBtn[m_nBtnHover]->MouseLeave();
        m_nBtnHover = -1;
		
		OnNcPaint();
    }

    //CDialog::OnNcMouseLeave();
}

// 鼠标在非客户区移动
void CFDialog::OnNcMouseMove(UINT nHitTest, CPoint point)
{
    if (!m_bNcTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = m_hWnd;
        tme.dwFlags = TME_NONCLIENT | TME_LEAVE;
        tme.dwHoverTime = 1;
        m_bNcTracking = _TrackMouseEvent(&tme);
    }

    CRect rcWnd;
    GetWindowRect(rcWnd);

    CPoint pt = point;
    pt.Offset(-rcWnd.left, -rcWnd.top);

    // 获取鼠标在哪个按钮上
    int nBtn = SysBtnHitTest(pt);
    if(nBtn != m_nBtnHover)
    {
        if(m_nBtnHover != -1)
        {
            m_mapSysBtn[m_nBtnHover]->MouseLeave();
            m_nBtnHover = -1;
        }
        if(nBtn != -1)
        {
            m_nBtnHover = nBtn;
            m_mapSysBtn[m_nBtnHover]->MouseHover();
        }

        OnNcPaint();
    }

    CDialog::OnNcMouseMove(nHitTest, point);
}

// 处理失去焦点时的状态
BOOL CFDialog::OnNcActivate(BOOL /*bActive*/)
{
    OnNcPaint();
    return TRUE;
}

// 创建非客户区过程
BOOL CFDialog::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (!CDialog::OnNcCreate(lpCreateStruct))
        return FALSE;

    // 初始化系统按钮
    InitSysBtn();

    return TRUE;
}

// 非客户区点击处理
LRESULT CFDialog::OnNcHitTest(CPoint point)
{
    CRect rcWnd;
    GetWindowRect(rcWnd);

    CPoint pt = point;
    pt.Offset(-rcWnd.left, -rcWnd.top);

	UINT hitTest = HTNOWHERE;
    if (m_rcIcon.PtInRect(pt))	// icon
    {
        return HTSYSMENU;
    }
	else if (0)				// caption text
	{
	}
    else
	{
		int nBtnID = SysBtnHitTest(pt);
		switch (nBtnID)
		{
		case SYSBTN_CLOSE:
			{
				hitTest = HTCLOSE;
				break;
			}
		case SYSBTN_MAX:
			{
				hitTest = HTMAXBUTTON;
				break;
			}
		case SYSBTN_MIN:
			{
				hitTest = HTMINBUTTON;
				break;
			}
		default:
			{
                hitTest = HTNOWHERE;
				break;
			}
		}
	}

    CString strDbg;
    strDbg.Format(_T("NcHitTest point(%d,%d);hitTest=%d"), point.x, point.y, hitTest);
    OutputDebugString(strDbg);

	if (hitTest != HTNOWHERE)
	{
        OnNcPaint();
		return hitTest;
	}

	// 点击了系统默认的按钮区域 -> 不处理
	LRESULT result = CDialog::OnNcHitTest(point);
	if (result == HTCLOSE || 
		result == HTMAXBUTTON 
		|| result == HTMINBUTTON)
	{
        OnNcPaint();
		return HTNOWHERE;
	}

    OnNcPaint();
	return CDialog::OnNcHitTest(point);
}

void CFDialog::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialog::OnPaint()

    //DrawDialog(&dc);
}

// 系统菜单
void CFDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
    if (nID != SC_CLOSE)
    {
        OnNcPaint();
    }
    PostMessage(WM_NCMOUSEMOVE,nID,lParam);

    CDialog::OnSysCommand(nID, lParam);
}

//  自绘非客户区
void CFDialog::DrawNc(CDC* pDC)
{
	//TRACE("CFDialog::DrawNc\n");
    if (NULL == pDC)
    {
        return ;
    }

    CRect rcWnd;
    GetWindowRect(rcWnd);
    rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);

    CRect rcClient;
    GetClientRect(rcClient);
    rcClient.OffsetRect(m_vecMargin[0], m_vecMargin[1]);

    // 排除客户区
    pDC->ExcludeClipRect(rcClient);

    // 双缓冲绘图
    FUi::CMemDCEX memDC(pDC, rcWnd);

    std::vector<RectF> vecRcfWnd;
    SepRect(rcWnd, vecRcfWnd, m_vecMargin);

    // 背景
    for (size_t i = 0; i < m_vecRcfBkg.size(); ++i)
    {
        if (i == 4)
        {	// 跳过客户区
            continue;
        }
        RenderEngine->DrawImageX(&memDC, m_pImgBkg, vecRcfWnd[i], m_vecRcfBkg[i].X, m_vecRcfBkg[i].Y, 
			m_vecRcfBkg[i].Width, m_vecRcfBkg[i].Height, RENDER_GDI);
    }

    // 最大化时见到的标题栏尺寸会有变化
    // 是否最大化(需要偏移)
    int nOffset = IsZoomed()?GetSystemMetrics(SM_CXDLGFRAME):0;
    // 标题栏（图标+文字）
    CString strTitle;
    GetWindowText(strTitle);
    CRect rcTitle(m_nTitleLeftMargin, nOffset, rcWnd.Width() - 96, IsIconic()?rcWnd.Height():m_vecMargin[1]); // 右侧预留4个按钮的空间

    // 图标
    HICON hIcon = GetIcon(TRUE);
    if (hIcon)
    {
        int nIconSize = IsIconic()?24:32;
        int nLeft = IsIconic()?3:m_nTitleLeftMargin;
        int nTop = rcTitle.top + (rcTitle.Height() - nIconSize) / 2;
        IsIconic()?(nTop += 2):0;

        m_rcIcon = CRect(nLeft, nTop, nLeft + nIconSize, nTop + nIconSize);

		//绘制标题栏图标
		if (m_bDrawTitleIcon)
		{
			DrawIconEx(memDC->GetSafeHdc(), nLeft, nTop, hIcon, nIconSize, nIconSize, 0, NULL, DI_NORMAL | DI_COMPAT);

			rcTitle.left = rcTitle.left +  nIconSize  + 5;
		}
		else		//不需要绘制标题栏图标
		{
            DrawIconEx(memDC->GetSafeHdc(), 0, 0, hIcon, 1, 1, 0, NULL, DI_NORMAL | DI_COMPAT);
			rcTitle.left = m_nTitleTextLeft;
		}
    
    }

    // 标题文字
    CFont *pOld = memDC->SelectObject(RenderEngine->GetFont(m_strCaptionFont));
    int nFormat = m_nTitleAlign | DT_VCENTER| DT_SINGLELINE | DT_WORD_ELLIPSIS;
    RenderEngine->DrawTextX(&memDC, strTitle, rcTitle, m_clrTitle, nFormat);
    memDC->SelectObject(pOld);

	m_rcTitle = rcTitle;
    // 系统按钮
    for (std::map<int, CFNcButton*>::iterator itr = m_mapSysBtn.begin(); 
        itr != m_mapSysBtn.end(); ++itr)
    {
        CFNcButton* pBtn = itr->second;
        if (pBtn)
        {
            pBtn->DrawButton(&memDC);
        }		
    }
}

// 自绘客户区
void CFDialog::DrawDialog(CDC* pDC)
{
	TRACE("CFDialog::DrawDialog\n");
    CRect rcClient;
    GetClientRect(rcClient);

    FUi::CMemDCEX memDC(pDC, rcClient);

    RectF rcfWnd((REAL)rcClient.left, (REAL)rcClient.top, 
        (REAL)rcClient.Width(), (REAL)rcClient.Height());

    if (m_vecRcfBkg.size() > 4)
    {
        RenderEngine->DrawImageX(&memDC, m_pImgBkg, rcfWnd, m_vecRcfBkg[4].X, m_vecRcfBkg[4].Y, 
            m_vecRcfBkg[4].Width, m_vecRcfBkg[4].Height);
    }
}

// 初始化系统按钮
void CFDialog::InitSysBtn()
{
    DWORD dwStyle = GetStyle(); 
    CRect rcBtn(0, 0, 24, 24);

    // close
    CFNcButton *pBtnClose = new CFNcButton;
    if (pBtnClose)
    {
        pBtnClose->CreateButton(_T(""), rcBtn, this, SYSBTN_CLOSE);
        pBtnClose->SetObjectName(_T("close"));
    }
    m_mapSysBtn[SYSBTN_CLOSE] = pBtnClose;

    // max
    CFNcButton *pBtnMax = NULL;
    if (dwStyle & WS_MAXIMIZEBOX)
    {
        pBtnMax = new CFNcButton;
        if (pBtnMax)
        {
            pBtnMax->CreateButton(_T(""), rcBtn, this, SYSBTN_MAX);
            pBtnMax->SetObjectName(_T("max"));
        }
    }
    m_mapSysBtn[SYSBTN_MAX] = pBtnMax;

    // min 
    CFNcButton *pBtnMin = NULL;
    if (dwStyle & WS_MINIMIZEBOX)
    {
        pBtnMin = new CFNcButton;

        if (pBtnMin)
        {
            pBtnMin->CreateButton(_T(""), rcBtn, this, SYSBTN_MIN);
            pBtnMin->SetObjectName(_T("min"));
        }
    }
    m_mapSysBtn[SYSBTN_MIN] = pBtnMin;
}

void CFDialog::DestorySysBtn()
{
    for (std::map<int, CFNcButton*>::iterator itr = m_mapSysBtn.begin();
        itr != m_mapSysBtn.end(); ++itr)
    {
        CFNcButton* pBtn = itr->second;
        if (pBtn)
        {
            delete pBtn;
        }
    }
    m_mapSysBtn.clear();
}

// 对话框改变大小后移动系统按钮的位置
void CFDialog::MoveSysBtn()
{
    if (m_mapSysBtn.size() <= 0)
    {
        return ;
    }

    CRect rcWnd;
    GetWindowRect(rcWnd);
    rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
    rcWnd.right -= 1;

    CRect rcBtnPos(rcWnd.right, 0, rcWnd.right, rcWnd.bottom);

    CFNcButton *pBtn = NULL; 
    for (std::map<int, CFNcButton*>::const_iterator citr = m_mapSysBtn.begin();
        citr != m_mapSysBtn.end(); ++citr)
    {
        pBtn = citr->second;
        if (NULL == pBtn)
        {
            continue;
        }

        CRect rcBtn;
        pBtn->GetRect(rcBtn);

        rcBtnPos.right = rcBtnPos.left - 3;
        rcBtnPos.left = rcBtnPos.right - rcBtn.Width();
        rcBtnPos.top = rcWnd.top + GetSystemMetrics(SM_CXDLGFRAME) + 4;  //按钮在标题栏的位置
        rcBtnPos.bottom = rcBtnPos.top + rcBtn.Height();
        pBtn->MoveWindow(rcBtnPos);

        //m_toolTip 
        if (pBtn->GetControlID() != SYSBTN_CLOSE && 
            pBtn->GetControlID() != SYSBTN_MAX && 
            pBtn->GetControlID() != SYSBTN_MIN)
        {
            rcBtnPos.OffsetRect(0, -m_vecMargin[1]);
            m_toolTip.SetToolRect(this, pBtn->GetControlID(), rcBtnPos);
        }
    }
    OnNcPaint();
}

// 判断鼠标在哪个按钮上
int CFDialog::SysBtnHitTest(CPoint pt)
{
    for (std::map<int, CFNcButton*>::const_iterator citr = m_mapSysBtn.begin();
        citr != m_mapSysBtn.end(); ++citr)
    {
        CFNcButton* pBtn = citr->second;
        if (NULL == pBtn)
        {
            continue;
        }

        CRect rcBtn;
        pBtn->GetRect(rcBtn);

        if (rcBtn.PtInRect(pt))
        {
            return pBtn->GetControlID();
        }
    }
    return -1;
}


// 设置对话框的背景图片
BOOL CFDialog::SetMargin(std::vector<int>& vecMargin)
{
    if (m_pImgBkg)
    {
        CRect rcImg(0, 0, m_pImgBkg->GetWidth(), m_pImgBkg->GetHeight());
        SepRect(rcImg, m_vecRcfBkg, vecMargin);
    }

    m_vecMargin = vecMargin;

    // 触发 OnNcCalcSize
    CRect rcWnd;
    GetWindowRect(rcWnd);
    SetWindowPos(NULL, 0, 0, rcWnd.Width() + 1, rcWnd.Height(), SWP_NOMOVE);
    SetWindowPos(NULL, 0, 0, rcWnd.Width(), rcWnd.Height(), SWP_NOMOVE);

    return TRUE;
}

// 对话框标题的对齐方式
void CFDialog::SetTitleAlign(int nAlign)
{
    m_nTitleAlign = nAlign;
    OnNcPaint();
}

// 标题栏增加自定义按钮
CFNcButton * CFDialog::AddCustomBtnToSysBtn(LPCTSTR szObjectName, LPCTSTR szText, LPCTSTR szBtnTip, 
                                    const int nID, const int nFixedWidth /* = 0 */)
{
    for (std::map<int, CFNcButton*>::const_iterator citr = m_mapSysBtn.begin();
        citr != m_mapSysBtn.end(); ++citr)
    {
        CFNcButton *pBtn = citr->second;
        if (NULL == pBtn)
        {
            continue;
        }
        if (pBtn->GetControlID() == nID)
        {
            return NULL;
        }
    }

    // 创建按钮
    CFNcButton *pBtn = new CFNcButton;
    if (pBtn)
    {
        CRect rcBtn(0, 0, 2, 2);
        pBtn->CreateButton(szText, rcBtn, this, nID);
        pBtn->SetObjectName(szObjectName);
		//pBtn->SetTextFont(YAHEI_26_SB);
        if (0 != nFixedWidth)
        {
            pBtn->SetFixedWidth(nFixedWidth);
        }
        m_toolTip.AddTool(this, szBtnTip, rcBtn, nID);
    }
    m_mapSysBtn[nID] = pBtn;

    PostMessage(WM_SIZE, 0, 0);

	return pBtn;
}

// 从标题栏删除自定义的按钮
void CFDialog::DeleteCustomBtn(const int nID)
{
    for (std::map<int, CFNcButton*>::const_iterator citr = m_mapSysBtn.begin();
        citr != m_mapSysBtn.end(); ++citr)
    {
        CFNcButton *pBtn = citr->second;
        if (NULL == pBtn)
        {
            continue;
        }
        if (pBtn->GetControlID() == nID)
        {
            m_mapSysBtn.erase(citr);
			delete pBtn;
            // 移除tooltip
            m_toolTip.DelTool(this, nID);

            PostMessage(WM_SIZE, 0, 0);
            return ;
        }
    }
}

void CFDialog::SetTitleFont(const CString& strFontName)
{
    m_strCaptionFont = strFontName;
    OnNcPaint();
}

void CFDialog::SetTitleColor(COLORREF clrTitle)
{
    m_clrTitle = clrTitle;
    OnNcPaint();
}

void CFDialog::SetObjectName(LPCTSTR szObjectName)
{
    Image* pImg = CResMgr::GetInstance()->GetRes(szObjectName);
    if (pImg)
    {
        m_pImgBkg = pImg;

        // 重新分割
        CRect rcImg(0, 0, m_pImgBkg->GetWidth(), m_pImgBkg->GetHeight());
        SepRect(rcImg, m_vecRcfBkg, m_vecMargin);

        m_strObjectName = szObjectName;
    }
}

//void CFDialog::SetShadow(BOOL bSet)
//{
//    m_shadow.Create(GetSafeHwnd());
//
//    m_shadow.SetSize(6);
//    m_shadow.SetSharpness(15);
//    m_shadow.SetDarkness(75);
//    m_shadow.SetPosition(0, 2);
//    m_shadow.SetColor(FUi::Black);
//}

LRESULT CFDialog::OnMsgCtrlToolTip(WPARAM wParam, LPARAM lParam)
{
	CWnd* pWnd = (CWnd*)wParam;
	LPCTSTR pszToolTip = (LPCTSTR)lParam;
	if (NULL != pWnd)
	{
		m_toolTip.RemoveTool(pWnd);
		m_toolTip.AddTool(pWnd, pszToolTip);
	}

	return 0;
}

void CFDialog::SetToolTipRect(DWORD dwControlID, CRect rcControl)
{
	m_toolTip.SetToolRect(this, dwControlID, rcControl);
}

LRESULT CFDialog::ProcMsg(MSG msg)
{
	if (msg.message >= WM_MOUSEFIRST &&msg.message <= WM_MOUSELAST ) //VM_MOUSEMOVE
	{
		m_toolTip.RelayEvent(&msg);
	}
	return 0 ;
}

LRESULT CFDialog::OnMsgCtrlToolTipEvent(WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	msg.hwnd = (HWND)wParam;
	msg.message = lParam;
	POINT curPt;
	GetCursorPos(&curPt);
	
	msg.pt = curPt;
	msg.wParam = curPt.x;
	msg.lParam = curPt.y;
	return ProcMsg(msg);
}

//是否需要绘制标题栏图标
//默认绘制，在某些情况下设为FALSE表示不需要对话框绘制图标
void CFDialog::SetDrawTitleIcon(BOOL bDraw)
{
	m_bDrawTitleIcon = bDraw;
}

void CFDialog::SetTitleLeftMargin(int nLeftMargin)
{
	m_nTitleLeftMargin = nLeftMargin;
}

void CFDialog::SetTitleTextLeft(int nLeft)
{
    m_nTitleTextLeft = nLeft;
}