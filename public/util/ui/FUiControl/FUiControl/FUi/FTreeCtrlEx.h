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

//UI��Ʊ���
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
//���϶�����
protected:
	UINT          m_TimerTicks;      //��������Ķ�ʱ����������ʱ��  
	UINT          m_nScrollTimerID;  //��������Ķ�ʱ��  
	CPoint        m_HoverPoint;      //���λ��  
	UINT          m_nHoverTimerID;   //������ж�ʱ��  
	DWORD         m_dwDragStart;     //������������һ�̵�ʱ��  
	BOOL          m_bDragging;       //��ʶ�Ƿ������϶�������  
	CImageList*   m_pDragImage;      //�϶�ʱ��ʾ��ͼ���б�  
	HTREEITEM     m_hItemDragS;      //���϶��ı�ǩ  
	HTREEITEM     m_hItemDragD;      //�����϶��ı�ǩ
	HTREEITEM	  m_hInsertAfter;	 //�϶��ı�ǩ���������ǩ֮��

	BOOL		  m_bDragAble;		//�Ƿ������϶�
	BOOL		  m_bHadIcon;		//�Ƿ��а�ťͼ�꣬�еĻ�������
	WORD		  m_wLastKayDown;
protected:
	DECLARE_MESSAGE_MAP()

public:

	BOOL SetItemData(HTREEITEM hItem, int nUserData);
	int GetItemData(HTREEITEM hItem);

	void SetItemFavState(HTREEITEM hItem, BOOL bFav);
	void SetItemEnable(HTREEITEM hItem, BOOL bEnable);

	BOOL IsItemEnable(HTREEITEM hItem);

	void SetHoverHot(BOOL bValue);	//��긲�����Ƿ�Ϊ����

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
