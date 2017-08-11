/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       FindItem.cpp
*   Description:    搜索控件
*   Author:         Zhengjinrong(Zhengjinrong@hikvision.com)
*   Date:           2009-11-26
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2009-11-27      Zhengjinrong   created
**************************************************************************/

//lint -library
#include "stdafx.h"
#include "FindItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFindItem::CFindItem():
m_pTreeCtrl(NULL)
{
}

CFindItem::~CFindItem()
{
}

/*******************************************************************************
*    Function:      RecursiveSearchItem
*    Description:   搜索函数
*    Input:         hItem 树节点
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败   
*******************************************************************************/
BOOL CFindItem::RecursiveSearchItem(HTREEITEM hItem)
{
	if (!NeedSearchThisItem(hItem))
	{
		return TRUE;
	}

	BOOL bBreak = FALSE;

	ASSERT(m_pTreeCtrl);

	HTREEITEM hChildItem = m_pTreeCtrl->GetChildItem(hItem);

	while (hChildItem)
	{	
		if (m_clStopEvent.Check())
		{
			bBreak = TRUE;
			break;
		}

		ProcessItem(hChildItem);

		if (!RecursiveSearchItem(hChildItem))
		{
			bBreak = TRUE;
			break;
		}

		hChildItem = m_pTreeCtrl->GetNextSiblingItem(hChildItem);
	}

	return !bBreak;
}


/*******************************************************************************
*    Function:      NeedSearchThisItem
*    Description:   是否搜索此节点
*    Input:         hItem 树节点
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败    
*******************************************************************************/
BOOL CFindItem::NeedSearchThisItem(HTREEITEM hItem)
{
	return TRUE;
}

/*******************************************************************************
*    Function:      ProcessItem
*    Description:   搜索函数
*    Input:         hItem 树节点
*    Output:        NULl
*    Return:	    return TRUE 成功； FALSE 失败  
*******************************************************************************/
BOOL CFindItem::ProcessItem(HTREEITEM hItem)
{
	return FALSE;
}

