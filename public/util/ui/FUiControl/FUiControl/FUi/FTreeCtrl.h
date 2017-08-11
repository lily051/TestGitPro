#pragma once
#include "FuiBase.h"

/*
    1、右键选中       √
    2、右键回调       √
    3、编辑节点、回调 √
    4、拖拽节点
    5、自定义字体     √
    6、自定义文字颜色 √
    7、支持超出控件的文本用...显示?已经自动滚动条了 ×
*/
// CFTreeCtrl

#define WM_FTREE_CTRL_RBUTTON_DOWN     (WM_USER + 8000)
#define WM_FTREE_CTRL_ITEMTEXT_CHANGED (WM_USER + 8001)
#define WM_FTREE_CTRL_ITEMSELECTED     (WM_USER + 8002)
#define WM_FTREE_CTRL_ITEM_DRAGGED     (WM_USER + 8003)


class CFTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CFTreeCtrl)

public:
	CFTreeCtrl();
	virtual ~CFTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()

private:
    int m_nEditLimit; // 可编辑时限制长度
    BOOL m_bEditing;    // 正在编辑Item

    CFont m_font;
    COLORREF m_color;

    BOOL m_bDragging;   // 正在拖拽
    HTREEITEM m_hDragItem, m_hDragDstItem;
    CImageList* m_pDragImgList;

    int m_nDragTimerId;
    int m_nHoverTimerId;
    CPoint m_ptHoverPoint;


    BOOL IsDragging() {return m_bDragging; }


    typedef enum
    {
        DROP_ABOVE = 100,
        DROP_BELOW,
        DROP_NODROP,
    }DROP_HINT;
    DROP_HINT m_nDropHint;

    DROP_HINT GetDropHint(CPoint point);

public:
    // 按状态设置字体(字体大小相关项被忽略)
    void SetTextFont(LOGFONT& normal);
    // 按状态设置文字颜色
    void SetTextColorEx(COLORREF color);
    // 设置项是否允许直接编辑
    void EnableEditItem(BOOL bEnable, int nLimit);
    // 设置是否允许鼠标拖拽项
    void EnableDragItem(bool bEnable);

    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    // 完成鼠标拖拽
    void CompleteDrag(bool bLButtonUp);
    void SuccessfulDrag();
    void TidyUpOfDrag();

    BOOL IsAncestor(HTREEITEM hItem, HTREEITEM hCheck) const;
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    void DrawDragItem(CDC* pDC, CRect& rcClient);

    void DrawDropLine(CDC* pDC, CRect rcItem);
};


