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


#ifndef __SEARCHEDITCTRL_H_
#define __SEARCHEDITCTRL_H_

#include "FindItem.h"
#include "SkinButton_V3.h"
#include <deque>


/*******************************************************************************
*    Function:      MsgCallBack
*    Description:   消息回调函数
*    Input:         nType 消息类型
					lpszText 消息内容
					lParam 回调函数类指针
*    Output:        NULl
*    Return:	    return NULl  
*******************************************************************************/
typedef void (CALLBACK *MsgCallBack)(UINT nType, LPCTSTR lpszText, LPARAM lParam);


class CSearchEditCtrl : public CWnd, public CAMDVDThread, public CFindItem
{
	DECLARE_DYNAMIC(CSearchEditCtrl)

public:
	CSearchEditCtrl(void);
	virtual ~CSearchEditCtrl(void);

private:
	BOOL m_bEmapSearch;//是否是电子地图查找

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClkEventBtn(UINT nID);
	afx_msg LRESULT OnSearchEnd(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:

	/*******************************************************************************
	*    Function:      Create
	*    Description:   创建窗口
	*    Input:         pParentWnd 父窗口指针
						pCallBack 消息回调函数
						lParam 回调函数类指针
	*    Output:        NULl
	*    Return:	    return TRUE 成功； FALSE 失败  
	*******************************************************************************/
	BOOL CreateCtrl(CWnd* pParentWnd, MsgCallBack pCallBack = NULL, LPARAM lParam = NULL);


	/*******************************************************************************
	*    Function:      Attach
	*    Description:   绑定控件
	*    Input:         pTreeCtrl 树控件
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void Attach(CTreeCtrl* pTreeCtrl);

	//查找
	BOOL Emap_Search(CString strKey);

protected:
	/*******************************************************************************
	*    Function:      ThreadProc
	*    Description:   线程函数
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return DWORD  
	*******************************************************************************/
	virtual DWORD ThreadProc();

	/*******************************************************************************
	*    Function:      NeedSearchThisItem
	*    Description:   是否需要搜索节点
	*    Input:         hItem 树节点
	*    Output:        NULl
	*    Return:	    return TRUE 成功； FALSE 失败  
	*******************************************************************************/
	virtual BOOL NeedSearchThisItem(HTREEITEM hItem);

	/*******************************************************************************
	*    Function:      ProcessItem
	*    Description:   搜索函数
	*    Input:         hItem 树节点
	*    Output:        NULl
	*    Return:	    return TRUE 成功； FALSE 失败  
	*******************************************************************************/
	virtual BOOL ProcessItem(HTREEITEM hItem);

private:
	//UI

	/*******************************************************************************
	*    Function:      CreateChildCtrl
	*    Description:   创建子控件
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void CreateChildCtrl();

	/*******************************************************************************
	*    Function:      InitUI
	*    Description:   初始化控件
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void InitUI();

	/*******************************************************************************
	*    Function:      MovePos
	*    Description:   窗口重新调整
	*    Input:         NULl 
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void MovePos();

	/*******************************************************************************
	*    Function:      DarwBK
	*    Description:   重绘背景
	*    Input:         pDC DC句柄
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void DarwBK(CDC* pDC);

	/*******************************************************************************
	*    Function:      DarwEditBK
	*    Description:   重绘编辑框背景
	*    Input:         pDC DC句柄
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void DarwEditBK(CDC* pDC);

	//Event

	/*******************************************************************************
	*    Function:      OnBtnClickSearch
	*    Description:   搜索消息响应函数
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void OnBtnClickSearch();

	/*******************************************************************************
	*    Function:      OnBtnClickNext
	*    Description:   搜索下一个函数
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void OnBtnClickNext();

	/*******************************************************************************
	*    Function:      OnBtnClickPer
	*    Description:   搜索前一个函数
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void OnBtnClickPer();
	
	//Search

	/*******************************************************************************
	*    Function:      InitSerach
	*    Description:   初始化搜索
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return TRUE 成功； FALSE 失败 
	*******************************************************************************/
	BOOL InitSerach();

	/*******************************************************************************
	*    Function:      DoSearch
	*    Description:   开始搜索
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return TRUE 成功； FALSE 失败  
	*******************************************************************************/
	BOOL DoSearch();

	/*******************************************************************************
	*    Function:      EndOfSearch
	*    Description:   结束搜索
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void EndOfSearch();

	/*******************************************************************************
	*    Function:      MessagCallBack
	*    Description:   消息回调函数
	*    Input:         nType 消息类型
						lpszText 消息内容
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void MessagCallBack(UINT nType, LPCTSTR lpszText);

private:
	CSkinButton_V3 m_btnSearch;
	CSkinButton_V3 m_btnNext;
	CSkinButton_V3 m_btnPer;
	CEdit		   m_clEdit;
	CFont	       m_clFont;
	CToolTipCtrl   m_tooltip;  
	CStatic		   m_clStcText;

	int			   m_nIndex;
	TCHAR		   m_szKey[MAX_PATH];

	CAMDVDEvent    m_clThreadActive;

	std::deque<HTREEITEM> m_dqTreeItem;

	MsgCallBack    m_pCallBack;
	LPARAM		   m_lParam;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetfocusEdit();
	afx_msg void OnKillfocusEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


#endif  //__SEARCHEDITCTRL_H_