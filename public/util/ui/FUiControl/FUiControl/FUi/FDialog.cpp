// FDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FDialog.h"
#include "FMemDCEx.h"

// ϵͳ��ť��ID
const int SYSBTN_CLOSE = 500;
const int SYSBTN_MAX   = 501;
const int SYSBTN_MIN   = 502;


// ��ʼ��������Ӱ�ࣨ����ֻ�ܳ�ʼ��һ�Σ�
static bool g_initShadow = false;
// CFDialog �Ի���

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
,m_nTitleLeftMargin(10)	//Ĭ��10����
,m_nTitleTextLeft(50)
{
    // ϵͳĬ�ϱ߿��С�ͱ������߶�
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
    // �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CFDialog::PreTranslateMessage(MSG* pMsg)
{
    m_toolTip.RelayEvent(pMsg);

    return CDialog::PreTranslateMessage(pMsg);
}

// �������ô��ڵı߿򣨰������������ĳߴ�
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
    // ���ﲻ���ټ�������Ĭ�ϵ�OnNcCalcSize,�������ɻ�ȡ�����С�ӿڶ������쳣��ֵ
}

// ������С�ĸ߶�
void CFDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // ��С�߶�Ϊ���±߿�ĺ�
    lpMMI->ptMinTrackSize.y = m_vecMargin[1] + m_vecMargin[3];

    CDialog::OnGetMinMaxInfo(lpMMI);
}
// �����ڴ�С�ı�ʱ����ť��λ�õĸı�
void CFDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    MoveSysBtn();

    //Բ��
    CRect rcWnd;
    GetWindowRect(rcWnd);

    if (m_Rgn.GetSafeHandle())
    {
        m_Rgn.DeleteObject();
    }
    m_Rgn.CreateRoundRectRgn(0, 0, rcWnd.Width() + 1, rcWnd.Height() + 1, 1, 1); //���ݴ��ھ��δ���һ��Բ�Ǿ���
    SetWindowRgn(m_Rgn, TRUE); //����Բ�Ǿ���ָ��������ı䴰�ڵ���״
}

// �ǿͻ���������Ϣ
void CFDialog::OnNcPaint()
{
    CDC* pWinDC = GetWindowDC();
    if (NULL == pWinDC)
    {
        return ;
    }

    // ���Ʒǿͻ���
    DrawNc(pWinDC);

    ReleaseDC(pWinDC);
}

void CFDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    CRect rcWnd;
    GetWindowRect(rcWnd);

    CPoint pt = point;
    pt.Offset(-rcWnd.left, -rcWnd.top);

    // ����Ƿ����ڰ�ť��
    int nBtnID = SysBtnHitTest(pt);
    if (m_mapSysBtn.find(nBtnID) != m_mapSysBtn.end())
    {
        //TRACE("����Ƿ����ڰ�ť��:%d\n", nBtnID);
        m_mapSysBtn[nBtnID]->LButtonDown();
        OnNcPaint();
        return ;    // ������ϵͳĬ�ϣ���������Ĭ��ͼ��
    } 

    // ֻ������ͼ������ͱ������հ�����
    // ������߿�
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

    // ��ȡ��굯��ʱ���ڰ�ť
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
            // û���ڰ�ť����
            CDialog::OnNcLButtonUp(nHitTest, point);
            return ;
        }
    default:
        {
            // һ�㲻������
			//m_mapSysBtn[nBtnID]->SetSelected();
            m_mapSysBtn[nBtnID]->LButtonUp();
            OnNcPaint();
            break;
        }
    }
}

// �������뿪�ǿͻ���
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

// ����ڷǿͻ����ƶ�
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

    // ��ȡ������ĸ���ť��
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

// ����ʧȥ����ʱ��״̬
BOOL CFDialog::OnNcActivate(BOOL /*bActive*/)
{
    OnNcPaint();
    return TRUE;
}

// �����ǿͻ�������
BOOL CFDialog::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (!CDialog::OnNcCreate(lpCreateStruct))
        return FALSE;

    // ��ʼ��ϵͳ��ť
    InitSysBtn();

    return TRUE;
}

// �ǿͻ����������
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

	// �����ϵͳĬ�ϵİ�ť���� -> ������
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
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

    //DrawDialog(&dc);
}

// ϵͳ�˵�
void CFDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
    if (nID != SC_CLOSE)
    {
        OnNcPaint();
    }
    PostMessage(WM_NCMOUSEMOVE,nID,lParam);

    CDialog::OnSysCommand(nID, lParam);
}

//  �Ի�ǿͻ���
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

    // �ų��ͻ���
    pDC->ExcludeClipRect(rcClient);

    // ˫�����ͼ
    FUi::CMemDCEX memDC(pDC, rcWnd);

    std::vector<RectF> vecRcfWnd;
    SepRect(rcWnd, vecRcfWnd, m_vecMargin);

    // ����
    for (size_t i = 0; i < m_vecRcfBkg.size(); ++i)
    {
        if (i == 4)
        {	// �����ͻ���
            continue;
        }
        RenderEngine->DrawImageX(&memDC, m_pImgBkg, vecRcfWnd[i], m_vecRcfBkg[i].X, m_vecRcfBkg[i].Y, 
			m_vecRcfBkg[i].Width, m_vecRcfBkg[i].Height, RENDER_GDI);
    }

    // ���ʱ�����ı������ߴ���б仯
    // �Ƿ����(��Ҫƫ��)
    int nOffset = IsZoomed()?GetSystemMetrics(SM_CXDLGFRAME):0;
    // ��������ͼ��+���֣�
    CString strTitle;
    GetWindowText(strTitle);
    CRect rcTitle(m_nTitleLeftMargin, nOffset, rcWnd.Width() - 96, IsIconic()?rcWnd.Height():m_vecMargin[1]); // �Ҳ�Ԥ��4����ť�Ŀռ�

    // ͼ��
    HICON hIcon = GetIcon(TRUE);
    if (hIcon)
    {
        int nIconSize = IsIconic()?24:32;
        int nLeft = IsIconic()?3:m_nTitleLeftMargin;
        int nTop = rcTitle.top + (rcTitle.Height() - nIconSize) / 2;
        IsIconic()?(nTop += 2):0;

        m_rcIcon = CRect(nLeft, nTop, nLeft + nIconSize, nTop + nIconSize);

		//���Ʊ�����ͼ��
		if (m_bDrawTitleIcon)
		{
			DrawIconEx(memDC->GetSafeHdc(), nLeft, nTop, hIcon, nIconSize, nIconSize, 0, NULL, DI_NORMAL | DI_COMPAT);

			rcTitle.left = rcTitle.left +  nIconSize  + 5;
		}
		else		//����Ҫ���Ʊ�����ͼ��
		{
            DrawIconEx(memDC->GetSafeHdc(), 0, 0, hIcon, 1, 1, 0, NULL, DI_NORMAL | DI_COMPAT);
			rcTitle.left = m_nTitleTextLeft;
		}
    
    }

    // ��������
    CFont *pOld = memDC->SelectObject(RenderEngine->GetFont(m_strCaptionFont));
    int nFormat = m_nTitleAlign | DT_VCENTER| DT_SINGLELINE | DT_WORD_ELLIPSIS;
    RenderEngine->DrawTextX(&memDC, strTitle, rcTitle, m_clrTitle, nFormat);
    memDC->SelectObject(pOld);

	m_rcTitle = rcTitle;
    // ϵͳ��ť
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

// �Ի�ͻ���
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

// ��ʼ��ϵͳ��ť
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

// �Ի���ı��С���ƶ�ϵͳ��ť��λ��
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
        rcBtnPos.top = rcWnd.top + GetSystemMetrics(SM_CXDLGFRAME) + 4;  //��ť�ڱ�������λ��
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

// �ж�������ĸ���ť��
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


// ���öԻ���ı���ͼƬ
BOOL CFDialog::SetMargin(std::vector<int>& vecMargin)
{
    if (m_pImgBkg)
    {
        CRect rcImg(0, 0, m_pImgBkg->GetWidth(), m_pImgBkg->GetHeight());
        SepRect(rcImg, m_vecRcfBkg, vecMargin);
    }

    m_vecMargin = vecMargin;

    // ���� OnNcCalcSize
    CRect rcWnd;
    GetWindowRect(rcWnd);
    SetWindowPos(NULL, 0, 0, rcWnd.Width() + 1, rcWnd.Height(), SWP_NOMOVE);
    SetWindowPos(NULL, 0, 0, rcWnd.Width(), rcWnd.Height(), SWP_NOMOVE);

    return TRUE;
}

// �Ի������Ķ��뷽ʽ
void CFDialog::SetTitleAlign(int nAlign)
{
    m_nTitleAlign = nAlign;
    OnNcPaint();
}

// �����������Զ��尴ť
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

    // ������ť
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

// �ӱ�����ɾ���Զ���İ�ť
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
            // �Ƴ�tooltip
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

        // ���·ָ�
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

//�Ƿ���Ҫ���Ʊ�����ͼ��
//Ĭ�ϻ��ƣ���ĳЩ�������ΪFALSE��ʾ����Ҫ�Ի������ͼ��
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