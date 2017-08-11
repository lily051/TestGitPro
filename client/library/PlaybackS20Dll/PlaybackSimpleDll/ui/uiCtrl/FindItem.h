/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       FindItem.h
*   Description:    �����ؼ�
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
	*    Description:   ��������
	*    Input:         hItem ���ڵ�
	*    Output:        NULl
	*    Return:	    return TRUE �ɹ��� FALSE ʧ��   
	*******************************************************************************/
	BOOL	RecursiveSearchItem(HTREEITEM hItem);

	/*******************************************************************************
	*    Function:      InitSearch
	*    Description:   ��ʼ����
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void	InitSearch(){m_clStopEvent.Reset(); };

	/*******************************************************************************
	*    Function:      SetEndofSearch
	*    Description:   ��������
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void	SetEndofSearch(){m_clStopEvent.Set(); };

	/*******************************************************************************
	*    Function:      IsEndofSearch
	*    Description:   �ж��Ƿ����
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return TRUE �ɹ��� FALSE ʧ��   
	*******************************************************************************/
	BOOL	IsEndofSearch(){ return m_clStopEvent.Check(); };

protected:

	/*******************************************************************************
	*    Function:      NeedSearchThisItem
	*    Description:   �Ƿ������˽ڵ�
	*    Input:         hItem ���ڵ�
	*    Output:        NULl
	*    Return:	    return TRUE �ɹ��� FALSE ʧ��    
	*******************************************************************************/
	virtual BOOL NeedSearchThisItem(HTREEITEM hItem);

	/*******************************************************************************
	*    Function:      ProcessItem
	*    Description:   ��������
	*    Input:         hItem ���ڵ�
	*    Output:        NULl
	*    Return:	    return TRUE �ɹ��� FALSE ʧ��  
	*******************************************************************************/
	virtual BOOL ProcessItem(HTREEITEM hItem);

	CTreeCtrl*  m_pTreeCtrl;

private:
	CAMDVDEvent	m_clStopEvent;
	
};

#endif // !defined(__FINDFILE_H__)
