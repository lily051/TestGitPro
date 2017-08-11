// FHeadCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "FHeadCtrl.h"
#include "FMemDCEx.h"

#define IDC_HEAD_CHECK_BOX     1999   // Checkbox的ID

#define SEPARATE_LINE_LENGTH   12     // 分隔线长度
#define CHECKBOX_SIZE          12     // checkbox大小
#define TEXT_MARGIN            10     // 文字边距
// CFHeadCtrl

IMPLEMENT_DYNAMIC(CFHeadCtrl, CHeaderCtrl)

CFHeadCtrl::CFHeadCtrl()
{
    m_isCheckBox = false;
    m_height = 34;
    m_colorBg = FUi::LISTHEAD_BG;
    m_pCheckBox = NULL;
    m_pImgSort = NULL;
    m_mapFont[FUi::Normal] = YAHEI_18_SB;
    m_mapTextColor[FUi::Normal] = FUi::White;
    m_headerResizeDisabled = FALSE;
}

//lint -e1551
CFHeadCtrl::~CFHeadCtrl()
{
    if (m_pCheckBox)
    {
        delete m_pCheckBox;
        m_pCheckBox = NULL;
    }

    std::vector<bool>().swap(m_isLockVec);
    std::vector<int>().swap(m_nSortVec);
}
//lint +e1551

//lint -e648
BEGIN_MESSAGE_MAP(CFHeadCtrl, CHeaderCtrl)
    ON_WM_NCHITTEST()
    ON_WM_SETCURSOR()
    ON_WM_PAINT()
    ON_MESSAGE(HDM_LAYOUT, &CFHeadCtrl::OnLayout)
    ON_NOTIFY_REFLECT(HDN_BEGINTRACK, &CFHeadCtrl::OnHdnBegintrack)
    ON_BN_CLICKED(IDC_HEAD_CHECK_BOX, &CFHeadCtrl::OnBtnClickCheckBox)
    ON_MESSAGE(WM_HEADCTRL_CHECKBOX_CHANGED, &CFHeadCtrl::OnChangeCheckBox)
END_MESSAGE_MAP()
//lint +e648

// CFHeadCtrl 消息处理程序

BOOL CFHeadCtrl::PreTranslateMessage(MSG* pMsg)
{
    // 预处理所有鼠标消息
    if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
    {
        int itemCount = GetItemCount();
        for (int i = 0; i < itemCount; i++)
        {
            CRect itemRect;
            GetItemRect(i, &itemRect);
            itemRect.left = itemRect.right - 12;
            itemRect.right = itemRect.right + 12;

            // 当鼠标在itemRect内且这一项锁定时，将鼠标设置为普通样式
            ClientToScreen(itemRect);
            if (itemRect.PtInRect(pMsg->pt) && m_isLockVec[i])
            {
                HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);
                ::SetCursor(hCur);
                return TRUE;
            }
        }
    }

    return CHeaderCtrl::PreTranslateMessage(pMsg);
}

void CFHeadCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rcClient, rcItem;
    GetClientRect(&rcClient);
    FUi::CMemDCEX memDC(&dc, rcClient);

    // 画出所有字段
    int nItem = GetItemCount(); // 得到单元数量
    int i = 0;
    while (i < nItem)
    {
        // 获取表头的文字和对齐方式
        HD_ITEM hditem;
        WCHAR lpBuffer[256] = {0};
        ZeroMemory(&hditem, sizeof(hditem));
        hditem.mask = HDI_TEXT | HDI_FORMAT;
        hditem.pszText = lpBuffer;
        hditem.cchTextMax = sizeof(lpBuffer);
        GetItem(i, &hditem);

        // 画背景，分隔线
        GetItemRect(i, &rcItem);
        rcItem.right -= 1;
        RenderEngine->DrawColor(&memDC, rcItem, m_colorBg);

        int margin = (m_height - SEPARATE_LINE_LENGTH) / 2;
        rcItem.left = rcItem.right;
        RenderEngine->DrawLine(&memDC, rcItem, 1, m_colorBg);
        rcItem.top += margin;
        rcItem.bottom -= margin;
        RenderEngine->DrawLine(&memDC, rcItem, 1, FUi::White);

        // 画Checkbox
        if (m_pCheckBox && (0 == i))
        {
            GetItemRect(0, &rcItem);
            int width = rcItem.Width();
            m_pCheckBox->MoveWindow((width - CHECKBOX_SIZE) / 2, margin, CHECKBOX_SIZE, CHECKBOX_SIZE);
            i++;
            continue;
        }

        // 将表头对齐转换为文字对齐
        int textAlign = DT_LEFT;
        if (hditem.fmt & LVCFMT_CENTER)
            textAlign = DT_CENTER;
        else if (hditem.fmt & LVCFMT_RIGHT)
            textAlign = DT_RIGHT;

        // 画表头文字
        CString strText(lpBuffer);
        GetItemRect(i, &rcItem);
        CRect rcText(rcItem);
        rcText.left += TEXT_MARGIN;
        UINT uFormat = DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL | DT_END_ELLIPSIS | textAlign;
        CFont *pOld = memDC->SelectObject(RenderEngine->GetFont(m_mapFont[FUi::Normal]));
        RenderEngine->DrawTextX(&memDC, strText, rcText, m_mapTextColor[FUi::Normal], uFormat);
        memDC->SelectObject(pOld);

        // 画排序图标
        if (m_pImgSort && m_nSortVec[i] > LIST_SORT_NO)
        {
            RectF rcfDst((REAL)rcItem.right - 5 - (REAL)m_pImgSort->GetWidth(),
                (REAL)rcItem.bottom / 2 - (REAL)m_pImgSort->GetHeight() / 4,
                (REAL)m_pImgSort->GetWidth(), (REAL)m_pImgSort->GetHeight() / 2);
            if (LIST_SORT_YES == m_nSortVec[i] || LIST_SORT_DESCENDING == m_nSortVec[i])
            {
                RenderEngine->DrawImageX(&memDC, m_pImgSort, rcfDst, 0, (REAL)m_pImgSort->GetHeight() / 2,
                    (REAL)m_pImgSort->GetWidth(), (REAL)m_pImgSort->GetHeight() / 2);
            }
            else if (LIST_SORT_ASCENDING == m_nSortVec[i])
            {
                RenderEngine->DrawImageX(&memDC, m_pImgSort, rcfDst, 0, 0,
                    (REAL)m_pImgSort->GetWidth(), (REAL)m_pImgSort->GetHeight() / 2);
            }
        }

        i++;
    }

    // 画表头剩余部分
    CRect lastRect;
    GetClientRect(&lastRect);
    lastRect.left = rcItem.right;
    RenderEngine->DrawColor(&memDC, lastRect, m_colorBg);
}

LRESULT CFHeadCtrl::OnLayout(WPARAM /*wParam*/, LPARAM lParam)
{
    LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
    HD_LAYOUT &hdl = *(HD_LAYOUT*)lParam;
    RECT *prc = hdl.prc; 
    WINDOWPOS *pwpos = hdl.pwpos;

    // 设置表头高度
    pwpos->cy = m_height;
    prc->top = m_height;

    Invalidate();

    return lResult;
}

LRESULT CFHeadCtrl::OnChangeCheckBox(WPARAM wParam, LPARAM lParam)
{
    if (m_pCheckBox)
    {
        m_pCheckBox->SetCheck(int(wParam));
    }
   
    return TRUE;
}

void CFHeadCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

    // 当一项锁定时，无法拖动
    *pResult = m_isLockVec[phdr->iItem];
}

void CFHeadCtrl::OnBtnClickCheckBox()
{
    // 同步设置列表项的checkbox
    if (m_pCheckBox)
    {
        ::PostMessage(GetParent()->GetSafeHwnd(), WM_HEADCTRL_CHECKBOX, m_pCheckBox->GetCheck(), NULL);
    }
}

void CFHeadCtrl::SetCheckBox(bool bCreate /*= false*/)
{
    if (bCreate && !m_pCheckBox)
    {
        m_pCheckBox = new CFButton;
        m_pCheckBox ->Create(_T(""), WS_CHILD | WS_VISIBLE| BS_AUTOCHECKBOX, CRect(0,0,0,0), this, IDC_HEAD_CHECK_BOX);
        m_pCheckBox->SetObjectName(_T("cbtn"));
        m_pCheckBox->SetCheckBox();
        m_pCheckBox->SetInListCtrl(TRUE);
    }
    m_isCheckBox = bCreate;
}

void CFHeadCtrl::AddLock()
{
    m_isLockVec.push_back(false);
}

void CFHeadCtrl::SetLock(int nItem, bool isLock)
{
    if (-1 == nItem)
    {
        for (int i = 0; i < GetItemCount(); i++)
        {
            m_isLockVec[i] = isLock;
        }
        return ;
    }

    m_isLockVec[nItem] = isLock;
}

void CFHeadCtrl::AddSort()
{
    m_nSortVec.push_back(LIST_SORT_NO);
}

void CFHeadCtrl::SetSort(int nItem, int nSortMode)
{
    if (NULL == m_pImgSort && 0 != nSortMode)
    {
        m_pImgSort = CResMgr::GetInstance()->GetRes(_T("header_sort"));
    }
    m_nSortVec[nItem] = nSortMode;
}

void CFHeadCtrl::SetHeight(int nHeight)
{
    m_height = nHeight;
}

int CFHeadCtrl::GetHeight()
{
    return m_height;
}

void CFHeadCtrl::SetBgColor(COLORREF colorBg)
{
    m_colorBg = colorBg;
}

LRESULT CFHeadCtrl::OnNcHitTest(CPoint point) 
{
    POINT clientPoint = point;
    ScreenToClient(&clientPoint);
    m_headerResizeDisabled = IsOnLastColumnDivider(clientPoint);
    return CHeaderCtrl::OnNcHitTest(point);
}

// prevents changing cursor to double-pointing arrow if cursor is above last header divider
BOOL CFHeadCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_headerResizeDisabled)
        return TRUE;
    return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

// chacks if point is above last column divider dragging area
BOOL CFHeadCtrl::IsOnLastColumnDivider(const CPoint& point) 
{
    // width of the area above header divider in which cursor
    // changes its shape to double-pointing east-west arrow
    int dragWidth = GetSystemMetrics(SM_CXCURSOR);
    // last column's header rectangle
    RECT rect;
    GetItemRect(GetItemCount() - 1, &rect);
    return point.x > rect.right - dragWidth / 2;
}
