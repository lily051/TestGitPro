/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       FindItem.h
*   Description:    搜索控件
*   Author:         Zhengjinrong(Zhengjinrong@hikvision.com)
*   Date:           2009-11-26
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2009-11-27      Zhengjinrong   created
**************************************************************************/

#if !defined(__FINDFILE_H__)
#define __FINDFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AMThread.h"

class CFindItem  
{
public:
	CFindItem();
	virtual ~CFindItem();
		
	/*******************************************************************************
	*    Function:      RecursiveSearchItem
	*    Description:   搜索函数
	*    Input:         hItem 树节点
	*    Output:        NULl
	*    Return:	    return TRUE 成功； FALSE 失败   
	*******************************************************************************/
	BOOL	RecursiveSearchItem(HTREEITEM hItem);

	/*******************************************************************************
	*    Function:      InitSearch
	*    Description:   开始搜索
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void	InitSearch(){m_clStopEvent.Reset(); };

	/*******************************************************************************
	*    Function:      SetEndofSearch
	*    Description:   结束搜索
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void	SetEndofSearch(){m_clStopEvent.Set(); };

	/*******************************************************************************
	*    Function:      IsEndofSearch
	*    Description:   判断是否结束
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return TRUE 成功； FALSE 失败   
	*******************************************************************************/
	BOOL	IsEndofSearch(){ return m_clStopEvent.Check(); };

protected:

	/*******************************************************************************
	*    Function:      NeedSearchThisItem
	*    Description:   是否搜索此节点
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

	CTreeCtrl*  m_pTreeCtrl;

private:
	CAMDVDEvent	m_clStopEvent;
	
};

#endif // !defined(__FINDFILE_H__)
