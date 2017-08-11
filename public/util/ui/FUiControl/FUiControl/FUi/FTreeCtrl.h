#pragma once
#include "FuiBase.h"

/*
    1���Ҽ�ѡ��       ��
    2���Ҽ��ص�       ��
    3���༭�ڵ㡢�ص� ��
    4����ק�ڵ�
    5���Զ�������     ��
    6���Զ���������ɫ ��
    7��֧�ֳ����ؼ����ı���...��ʾ?�Ѿ��Զ��������� ��
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
    int m_nEditLimit; // �ɱ༭ʱ���Ƴ���
    BOOL m_bEditing;    // ���ڱ༭Item

    CFont m_font;
    COLORREF m_color;

    BOOL m_bDragging;   // ������ק
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
    // ��״̬��������(�����С��������)
    void SetTextFont(LOGFONT& normal);
    // ��״̬����������ɫ
    void SetTextColorEx(COLORREF color);
    // �������Ƿ�����ֱ�ӱ༭
    void EnableEditItem(BOOL bEnable, int nLimit);
    // �����Ƿ����������ק��
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
    // ��������ק
    void CompleteDrag(bool bLButtonUp);
    void SuccessfulDrag();
    void TidyUpOfDrag();

    BOOL IsAncestor(HTREEITEM hItem, HTREEITEM hCheck) const;
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    void DrawDragItem(CDC* pDC, CRect& rcClient);

    void DrawDropLine(CDC* pDC, CRect rcItem);
};


