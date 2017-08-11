#pragma once


// CPopupDlgOwerDraw dialog

class CPopupDlgOwerDraw : public CDialog
{
	DECLARE_DYNAMIC(CPopupDlgOwerDraw)

public:
	//CPopupDlgOwerDraw(CWnd* pParent = NULL);   // standard constructor
	CPopupDlgOwerDraw(UINT nIDTemplate, CWnd* pParentWnd = NULL );   // standard constructor
	virtual ~CPopupDlgOwerDraw();

// Dialog Data
	//enum { IDD = IDD_POPUPDLGOWERDRAW };
	void MinDlg();                                                         //��С������
	void MaxDlg();                                                         //��󻯴���

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void _DrawDialog(CDC * pDC);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void _DrawCustom(CDC * pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	void SetSkinImage(HBITMAP hTopLeft,HBITMAP hTopMid,HBITMAP hTopRight,HBITMAP hMidLeft,\
		HBITMAP hMidRight,HBITMAP hBottomLeft,HBITMAP hBottomMid,HBITMAP hBottomRight);

private:
	HBITMAP m_hTopLeft;
	HBITMAP m_hTopMid;
	HBITMAP m_hTopRight;
	HBITMAP m_hMidLeft;
	HBITMAP m_hMidRight;
	HBITMAP m_hBottomLeft;
	HBITMAP m_hBottomMid;
	HBITMAP m_hBottomRight;

	CBrush m_bakeBrush;
	BOOL m_bSkinMode;
public:

	/*******************************************************************************
	*    Function:      FreeImage
	*    Description:   �ͷ�ͼƬ
	*    Input:         (NULL) 
	*    Output:        (NULL)
	*    Return:	    return void
	*******************************************************************************/
	void FreeImage(void);

	/*******************************************************************************
	*    Function:      FreeBrush
	*    Description:   �ͷŻ�ˢ
	*    Input:         (NULL) 
	*    Output:        (NULL)
	*    Return:	    return void
	*******************************************************************************/
	void FreeBrush(void);

	/*************************************************
	Function:    	SetBakeColor
	Description:	���ô��ڵı�����ɫ
	Input:			crBakeColor ���ñ�����ɫ
	Output:      	
	Return:			
	*************************************************/
	void SetBakeColor(COLORREF crBakeColor);

protected:
	virtual void _DrawDialog_Skin(CDC * pDC);	
};
