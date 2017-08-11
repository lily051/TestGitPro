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
	void MinDlg();                                                         //最小化窗口
	void MaxDlg();                                                         //最大化窗口

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
	*    Description:   释放图片
	*    Input:         (NULL) 
	*    Output:        (NULL)
	*    Return:	    return void
	*******************************************************************************/
	void FreeImage(void);

	/*******************************************************************************
	*    Function:      FreeBrush
	*    Description:   释放画刷
	*    Input:         (NULL) 
	*    Output:        (NULL)
	*    Return:	    return void
	*******************************************************************************/
	void FreeBrush(void);

	/*************************************************
	Function:    	SetBakeColor
	Description:	设置窗口的背景颜色
	Input:			crBakeColor 设置背景颜色
	Output:      	
	Return:			
	*************************************************/
	void SetBakeColor(COLORREF crBakeColor);

protected:
	virtual void _DrawDialog_Skin(CDC * pDC);	
};
