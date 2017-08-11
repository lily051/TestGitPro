/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SearchEditCtrl.h
*   Description:    �����ؼ�
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
*    Description:   ��Ϣ�ص�����
*    Input:         nType ��Ϣ����
					lpszText ��Ϣ����
					lParam �ص�������ָ��
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
	BOOL m_bEmapSearch;//�Ƿ��ǵ��ӵ�ͼ����

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
	*    Description:   ��������
	*    Input:         pParentWnd ������ָ��
						pCallBack ��Ϣ�ص�����
						lParam �ص�������ָ��
	*    Output:        NULl
	*    Return:	    return TRUE �ɹ��� FALSE ʧ��  
	*******************************************************************************/
	BOOL CreateCtrl(CWnd* pParentWnd, MsgCallBack pCallBack = NULL, LPARAM lParam = NULL);


	/*******************************************************************************
	*    Function:      Attach
	*    Description:   �󶨿ؼ�
	*    Input:         pTreeCtrl ���ؼ�
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void Attach(CTreeCtrl* pTreeCtrl);

	//����
	BOOL Emap_Search(CString strKey);

protected:
	/*******************************************************************************
	*    Function:      ThreadProc
	*    Description:   �̺߳���
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return DWORD  
	*******************************************************************************/
	virtual DWORD ThreadProc();

	/*******************************************************************************
	*    Function:      NeedSearchThisItem
	*    Description:   �Ƿ���Ҫ�����ڵ�
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

private:
	//UI

	/*******************************************************************************
	*    Function:      CreateChildCtrl
	*    Description:   �����ӿؼ�
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void CreateChildCtrl();

	/*******************************************************************************
	*    Function:      InitUI
	*    Description:   ��ʼ���ؼ�
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void InitUI();

	/*******************************************************************************
	*    Function:      MovePos
	*    Description:   �������µ���
	*    Input:         NULl 
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void MovePos();

	/*******************************************************************************
	*    Function:      DarwBK
	*    Description:   �ػ汳��
	*    Input:         pDC DC���
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void DarwBK(CDC* pDC);

	/*******************************************************************************
	*    Function:      DarwEditBK
	*    Description:   �ػ�༭�򱳾�
	*    Input:         pDC DC���
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void DarwEditBK(CDC* pDC);

	//Event

	/*******************************************************************************
	*    Function:      OnBtnClickSearch
	*    Description:   ������Ϣ��Ӧ����
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void OnBtnClickSearch();

	/*******************************************************************************
	*    Function:      OnBtnClickNext
	*    Description:   ������һ������
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void OnBtnClickNext();

	/*******************************************************************************
	*    Function:      OnBtnClickPer
	*    Description:   ����ǰһ������
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void OnBtnClickPer();
	
	//Search

	/*******************************************************************************
	*    Function:      InitSerach
	*    Description:   ��ʼ������
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return TRUE �ɹ��� FALSE ʧ�� 
	*******************************************************************************/
	BOOL InitSerach();

	/*******************************************************************************
	*    Function:      DoSearch
	*    Description:   ��ʼ����
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return TRUE �ɹ��� FALSE ʧ��  
	*******************************************************************************/
	BOOL DoSearch();

	/*******************************************************************************
	*    Function:      EndOfSearch
	*    Description:   ��������
	*    Input:         NULl
	*    Output:        NULl
	*    Return:	    return NULl  
	*******************************************************************************/
	void EndOfSearch();

	/*******************************************************************************
	*    Function:      MessagCallBack
	*    Description:   ��Ϣ�ص�����
	*    Input:         nType ��Ϣ����
						lpszText ��Ϣ����
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