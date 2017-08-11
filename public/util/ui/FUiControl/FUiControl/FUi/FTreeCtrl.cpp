// FTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "FTreeCtrl.h"
#include "FMemDCEx.h"

#define IDT_HOVER_TIMER  2
#define OUT_OF_CONTROL_MARGIN   5
// CFTreeCtrl

IMPLEMENT_DYNAMIC(CFTreeCtrl, CTreeCtrl)

CFTreeCtrl::CFTreeCtrl()
: m_bEditing(FALSE)
, m_bDragging(FALSE)
{

}

CFTreeCtrl::~CFTreeCtrl()
{
    SAFE_DELETE_FONT(m_font);
}

BEGIN_MESSAGE_MAP(CFTreeCtrl, CTreeCtrl)
    ON_WM_RBUTTONDOWN()
    ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CFTreeCtrl::OnTvnBeginlabeledit)
    ON_NOTIFY_REFLECT(TVN_ITEMCHANGED, &CFTreeCtrl::OnTvnItemChanged)
    ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CFTreeCtrl::OnTvnEndlabeledit)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CFTreeCtrl::OnTvnBegindrag)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
END_MESSAGE_MAP()



// 按状态设置字体
void CFTreeCtrl::SetTextFont(LOGFONT& lf)
{
    SAFE_DELETE_FONT(m_font);
    m_font.CreateFontIndirect(&lf);
    SetFont(&m_font);
}

// 按状态设置文字颜色
void CFTreeCtrl::SetTextColorEx(COLORREF color)
{
    m_color = color;
    SetTextColor(color);
}

// 设置项是否允许直接编辑
void CFTreeCtrl::EnableEditItem(BOOL bEnable, int nLimit)
{
    if (bEnable)
    {
        ModifyStyle(0, TVS_EDITLABELS);
        m_nEditLimit = nLimit;
    }
    else
    {
        ModifyStyle(TVS_EDITLABELS, 0);
    }
}

void CFTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        CompleteDrag(FALSE);
    }
    UINT uFlags = 0;
    HTREEITEM hItem = HitTest(point, &uFlags);
    SelectItem(hItem);
    ::PostMessage(GetParent()->GetSafeHwnd(), WM_FTREE_CTRL_RBUTTON_DOWN, (WPARAM)hItem, NULL);

    CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CFTreeCtrl::OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);

    ::PostMessage(GetParent()->GetSafeHwnd(), WM_FTREE_CTRL_ITEMSELECTED, 
        (WPARAM)pNMTVItemChange->hItem, 0);

    *pResult = 0;
}

void CFTreeCtrl::OnTvnBeginlabeledit(NMHDR * /*pNMHDR*/, LRESULT *pResult)
{
    //LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    
    GetEditControl()->LimitText(m_nEditLimit);

    m_bEditing = TRUE;

    *pResult = 0;
}


void CFTreeCtrl::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    
    CString strTextNew;
    GetEditControl()->GetWindowText(strTextNew);
    if (strTextNew.IsEmpty())
    {
        return ;
    }

    HTREEITEM hItem = pTVDispInfo->item.hItem;
    CString strTextOld = GetItemText(hItem);

    if (strTextOld != strTextNew)
    {
        SetItemText(hItem, strTextNew);
        ::PostMessage(GetParent()->GetSafeHwnd(), WM_FTREE_CTRL_ITEMTEXT_CHANGED, (WPARAM)hItem, NULL);
    }

    m_bEditing = FALSE;

    *pResult = 0;
}

BOOL CFTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
    if (WM_KEYDOWN == pMsg->message)
    {
        if (VK_ESCAPE == pMsg->wParam)
        {
            if (m_bEditing)
            {
                GetEditControl()->SendMessage(WM_UNDO, NULL);  //ESC撤销 
                GetEditControl()->SendMessage(WM_KEYDOWN, VK_RETURN); 
            } 
        }
        else if (VK_RETURN == pMsg->wParam)
        {
            if (m_bEditing)
            {
                GetEditControl()->SendMessage(WM_KEYDOWN, VK_RETURN);   //回车就执行
            }  
        }
        else if (VK_F2 == pMsg->wParam)
        {
            HTREEITEM hItem = GetSelectedItem();
            if (hItem && !m_bEditing)
            {
                EditLabel(hItem);
            }
        }
    }

    return CTreeCtrl::PreTranslateMessage(pMsg);
}

BOOL CFTreeCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

void CFTreeCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rcClient;
    GetClientRect(rcClient);

    FUi::CMemDCEX memDC(&dc, rcClient);

    CRect rcClip;
    dc.GetClipBox(rcClip);

    CRgn rgn;
    rgn.CreateRectRgnIndirect(&rcClip);
    memDC.SelectClipRgn(&rgn);
    rgn.DeleteObject();

    // 调用默认函数绘图
    CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.GetSafeHdc(), 0);

    // 处理拖拽绘图
    DrawDragItem(&memDC, rcClient);
}

void CFTreeCtrl::DrawDragItem(CDC* pDC, CRect& rcClient)
{
    if ((NULL == pDC) || !m_bDragging)
    {
        return ;
    }

    DWORD dwFormat = DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_END_ELLIPSIS | DT_NOCLIP;

    CRect rcItem;
    HTREEITEM hItem = GetFirstVisibleItem();
    while (hItem)
    {
        GetItemRect(hItem, rcItem, TRUE);

        // 背景色
        CRect rcTemp(rcItem);
        rcTemp.right = rcClient.right;
        CBrush brBg(GetSysColor(COLOR_WINDOW));
        pDC->FillRect(rcTemp, &brBg);

        // 文字
        CString strText = GetItemText(hItem);
        CFont* pOldFont = pDC->SelectObject(GetFont());
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(m_color);

        if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
        {
            pDC->DrawFocusRect(rcItem);
        }

        pDC->DrawText(strText, rcItem, dwFormat);

        if (pOldFont)
        {
            pDC->SelectObject(pOldFont);
        }

        hItem = GetNextVisibleItem(hItem);
    }

    hItem = GetFirstVisibleItem();
    while (hItem)
    {
        if (hItem == m_hDragDstItem)
        {
            GetItemRect(hItem, rcItem, TRUE);
            rcItem.right = rcClient.right;
            DrawDropLine(pDC, rcItem);
        }
        hItem = GetNextVisibleItem(hItem);
    }
}

void CFTreeCtrl::DrawDropLine(CDC* pDC, CRect rcItem)
{
    if (DROP_NODROP == m_nDropHint)
    {
        return ;
    }

    CPoint ptStart(rcItem.left - 8, rcItem.bottom), ptEnd(rcItem.right, rcItem.bottom);
    if (DROP_ABOVE == m_nDropHint)
    {
        TRACE("hint:above\n");
        ptStart.y = ptEnd.y = rcItem.top;
    }
    else
    {
        TRACE("hint:below\n");
    }

    CPen pen, *pOld;
    pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    pOld = pDC->SelectObject(&pen);

    pDC->Ellipse(ptStart.x, ptStart.y - 3, ptStart.x + 6, ptStart.y + 3);

    pDC->MoveTo(ptStart.x + 6, ptStart.y);
    pDC->LineTo(ptEnd.x, ptEnd.y);

}

// 设置是否允许鼠标拖拽项
void CFTreeCtrl::EnableDragItem(bool bEnable)
{
    if (bEnable)
    {
        ModifyStyle(TVS_DISABLEDRAGDROP, 0);
    }
    else
    {
        ModifyStyle(0, TVS_DISABLEDRAGDROP);
    }
}

void CFTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    
    m_hDragItem = pNMTreeView->itemNew.hItem;
    if (NULL == m_hDragItem)
    {
        return ;
    }
    SelectItem(m_hDragItem);

    m_pDragImgList = CreateDragImage(m_hDragItem);
    if (NULL == m_pDragImgList)
    {
        return ;
    }
    m_pDragImgList->BeginDrag(0, CPoint(0, 5));
    m_pDragImgList->DragEnter(this, pNMTreeView->ptDrag);   // 锁定不刷新界面
    m_hDragDstItem = NULL;
    m_bDragging = TRUE;
    SetCapture();
    m_nDragTimerId = SetTimer(1, 25, NULL);  // 

    *pResult = 0;
}

void CFTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // 结束之前设置的timer
    if (m_nHoverTimerId > 0)
    {
        KillTimer(m_nHoverTimerId);
        m_nHoverTimerId = 0;
    }

    if (m_bDragging && m_pDragImgList)
    {
        m_pDragImgList->DragMove(point);

        m_ptHoverPoint = point;
        m_nHoverTimerId = SetTimer(IDT_HOVER_TIMER, 500, NULL);

        UINT uFlags = 0;
        m_hDragDstItem = HitTest(point, &uFlags);   // 当前指向的item
        if (m_hDragDstItem)
        {
            m_pDragImgList->DragLeave(this);    // 释放，刷新界面

            // 计算目标位置
            SelectDropTarget(m_hDragDstItem);
            m_nDropHint = GetDropHint(point);
            EnsureVisible(m_hDragDstItem);
            RedrawWindow();
        }
        m_pDragImgList->DragEnter(this, point);
    }

    CTreeCtrl::OnMouseMove(nFlags, point);
}

void CFTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
       CompleteDrag(TRUE);
    }

    CTreeCtrl::OnLButtonUp(nFlags, point);
}

// 完成鼠标拖拽
void CFTreeCtrl::CompleteDrag(bool bLButtonUp)
{
    TidyUpOfDrag();
    if (bLButtonUp)
    {
        if (m_hDragDstItem)
        {
            SuccessfulDrag();
        }
    }
    else
    {
        // 取消拖拽
        EnsureVisible(m_hDragItem);
        Invalidate();
    }
}

// 检查hCheck是否为hItem的子节点
BOOL CFTreeCtrl::IsAncestor(HTREEITEM hItem, HTREEITEM hCheck) const
{
    for(HTREEITEM hParent = hCheck; hParent != NULL; hParent = GetParentItem(hParent))
    {
        if(hParent == hItem)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 返回插入位置：上面、下面、子节点
CFTreeCtrl::DROP_HINT CFTreeCtrl::GetDropHint(CPoint point)
{
    DROP_HINT hint = DROP_NODROP;

    CRect rcItem;
    if(GetItemRect(GetDropHilightItem(), rcItem, FALSE)) 
    {
        if(2 * point.y > rcItem.top + rcItem.bottom)
        {
            hint = DROP_BELOW;
        }
        else
        {
            hint = DROP_ABOVE;
        }
    } 
    else
    {
        hint = DROP_ABOVE;
    }

    return hint;
}

// 定时器
void CFTreeCtrl::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == (UINT)m_nHoverTimerId)
    {
        KillTimer(m_nHoverTimerId);
        m_nHoverTimerId = 0;

        UINT uFlags = 0;
        HTREEITEM hItem = HitTest(m_ptHoverPoint, &uFlags);
        if (hItem)
        {
            m_pDragImgList->DragShowNolock(FALSE);
            Expand(hItem, TVE_TOGGLE);
            TRACE("TVE_TOGGLE   *\n");
            RedrawWindow();
            m_pDragImgList->DragShowNolock(TRUE);
        }
    }
    else if (m_bDragging)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(&point);

    }
    CTreeCtrl::OnTimer(nIDEvent);
}


void CFTreeCtrl::TidyUpOfDrag()
{
    m_bDragging = FALSE;

    KillTimer(m_nDragTimerId);
    m_nDragTimerId = 0;
    KillTimer(m_nHoverTimerId);
    m_nHoverTimerId = 0;

    ReleaseCapture();
    //ShowCursor(TRUE);

    if (m_pDragImgList)
    {
        m_pDragImgList->DragLeave(this);
        m_pDragImgList->EndDrag();

        delete m_pDragImgList;
        m_pDragImgList = NULL;
    }

    SelectDropTarget(NULL);
}

void CFTreeCtrl::SuccessfulDrag()
{

}

