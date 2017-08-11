#pragma once

#define DRAG_DELAY   60

typedef struct CONTROLCENTER_TreenodePtr TreeNode,*PTreeNode;

class AFX_EXT_CLASS CFTreeCtrlEx : public CTreeCtrl
{
	typedef CTreeCtrl Base;
	DECLARE_DYNAMIC(CFTreeCtrlEx)

public:
	enum
	{
		TREE_MARGIN_5       = 5,
		TREE_MARGIN_10      = 10,
		TREE_IMAGE_WIDTH    = 16,
	};
	enum
	{
		TREE_IMAGE_ICON     = 0,
		TREE_IMAGE_DICON    = 1,
		TREE_IMAGE_ALL
	};

public:
	CFTreeCtrlEx();
	virtual ~CFTreeCtrlEx();

	void LoadImageRes(Image * pImageIcon, Image * pImageDIcon );
	void LoadImageRes(LPCTSTR lpszIconPath, LPCTSTR lpszDIconPath);
	void SetDragAble(BOOL bVale);
	void ClearTreeData();
	
protected:
	virtual void OnItemIconClick(HTREEITEM hItem);
	virtual void DrawIcon(CDC *pDC, CRect &rcItem, HTREEITEM hItem);

	HTREEITEM GetNextEnableItem(HTREEITEM hItem = NULL);

private:

	LRESULT AddItemIcon(NMTVCUSTOMDRAW* pNMCD);
	void InitCtrl();
	void DrawRect(CDC *pDC, CRect &rcTag);
	void MouseMoveOfDrag(UINT nFlags, CPoint point);
	void SetCurrentCursor(HCURSOR curSor, HTREEITEM hotItem);

	HTREEITEM CopyBranch(HTREEITEM htiBranch,HTREEITEM htiNewParent,HTREEITEM htiAfter);  
	HTREEITEM CopyItem(HTREEITEM hItem,HTREEITEM htiNewParent,HTREEITEM htiAfter);

//UI设计变量
protected:
	std::vector<PTreeNode> m_vcTreeDatas;
	COLORREF m_clOrange;
	COLORREF m_clGray;
	COLORREF m_clWhite;
	COLORREF m_clBlack;
	CRect m_rcClientRect;
	CRect m_rcOldHover;
	CPoint m_ptHover;
	BOOL m_bHover;

	BOOL m_bHoverHot;

	BOOL m_bTracking;
	//CFont m_fontText;
	Image * m_pImageArray[TREE_IMAGE_ALL];

	HTREEITEM m_hCurHotItem;

	HCURSOR m_hCSHand;
	HCURSOR m_hCSArrow;
	HCURSOR m_hCSCurrent;
//可拖动变量
protected:
	UINT          m_TimerTicks;      //处理滚动的定时器所经过的时间  
	UINT          m_nScrollTimerID;  //处理滚动的定时器  
	CPoint        m_HoverPoint;      //鼠标位置  
	UINT          m_nHoverTimerID;   //鼠标敏感定时器  
	DWORD         m_dwDragStart;     //按下鼠标左键那一刻的时间  
	BOOL          m_bDragging;       //标识是否正在拖动过程中  
	CImageList*   m_pDragImage;      //拖动时显示的图象列表  
	HTREEITEM     m_hItemDragS;      //被拖动的标签  
	HTREEITEM     m_hItemDragD;      //接受拖动的标签
	HTREEITEM	  m_hInsertAfter;	 //拖动的标签插在这个标签之后

	BOOL		  m_bDragAble;		//是否允许拖动
	BOOL		  m_bHadIcon;		//是否有按钮图标，有的话允许点击
	WORD		  m_wLastKayDown;
protected:
	DECLARE_MESSAGE_MAP()

public:

	BOOL SetItemData(HTREEITEM hItem, int nUserData);
	int GetItemData(HTREEITEM hItem);

	void SetItemFavState(HTREEITEM hItem, BOOL bFav);
	void SetItemEnable(HTREEITEM hItem, BOOL bEnable);

	BOOL IsItemEnable(HTREEITEM hItem);

	void SetHoverHot(BOOL bValue);	//鼠标覆盖项是否为热区

	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMKillfocus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
