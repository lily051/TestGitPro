#pragma once

/*
	1.调用SubclassWindow或者使用DDX_Control子类化树控件

	2.初始化 OnInitDialog():
		CBitmap bmpTemp;
		bmpTemp.LoadBitmap(IDB_BMP_STATE3);
		m_imglistState.Create(15, 15, ILC_COLOR24, 4, 1);
		m_imglistState.Add(&bmpTemp, RGB(0,0,0));
		m_tree.SetImageList(&m_imglistState, TVSIL_STATE);
*/

// CCheck3TreeCtrl

class CCheck3TreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CCheck3TreeCtrl)

public:
	CCheck3TreeCtrl();
	virtual ~CCheck3TreeCtrl();

	//nCheck: 0 check, 1 uncheck, else xcheck
	BOOL SetCheck(HTREEITEM hItem, int nCheck = 1);

	//result: 0 check, 1 uncheck, 2 xcheck
	int  GetCheck(HTREEITEM hItem) const;

	//update parent and children, recursive.
	void UpdateParentState(HTREEITEM hItem, int nCheck);
	void UpdateChildrenState(HTREEITEM hItem, int nCheck);

private:
	UINT      m_nHitFlag;
	HTREEITEM m_hHitItem;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};
