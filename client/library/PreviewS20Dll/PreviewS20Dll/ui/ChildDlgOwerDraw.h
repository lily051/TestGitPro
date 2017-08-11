#pragma once

#define CUSTOMDLG_V1_CLOSE  (WM_USER + 5040)
#define CUSTOMDLG_V1_MINI  (CUSTOMDLG_V1_CLOSE + 1)
#define CUSTOMDLG_V1_RESORE (CUSTOMDLG_V1_CLOSE + 2)
#define CUSTOMDLG_V1_LOCK (CUSTOMDLG_V1_CLOSE + 3)
// CChildDlgOwerDraw dialog

class CChildDlgOwerDraw : public CDialog
{
	DECLARE_DYNAMIC(CChildDlgOwerDraw)

public:
	//CChildDlgOwerDraw(CWnd* pParent = NULL);   // standard constructor
	CChildDlgOwerDraw(UINT nIDTemplate, CWnd* pParentWnd);
	virtual ~CChildDlgOwerDraw();

// Dialog Data
	//enum { IDD = IDD_POPUPDLGOWERDRAW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetBakeColor(COLORREF crBakeColor);
	void SetMiniSize(BOOL bMini);
	void SetTitleBar_Image(Image * pMid);
	void SetTitleBar_ExitBtnImage(Image * pExitImage , Image * pExitImageSel);
	void SetTitleBar_MinBtnImage(Image * pMinImage , Image * pRestoreImage,Image * pBakeImage);
	void SetTitleBar_LockImage(Image * pLockImage , Image * pUnLockImage);
public:
	CBrush m_bakeBrush;

	Image * m_pTitle;
	Image * m_pMinImage;
	Image * m_pRestoreImage;
	Image * m_pBakeImage;
	Image * m_pExitImage;
	Image * m_pExitImageSel;
	      

	CRect m_crMinBtn;
	CRect m_crLockBtn;
	CRect m_crExitBtn;
	CRect m_crCaption;

	BOOL m_bMinBtnDown;
	BOOL m_bExitBtnDown;

	BOOL m_bMiniDlg;

	BOOL m_bHoverExitBtn;
	BOOL m_bHoverMiniBtn;
	BOOL m_bDownExitBtn;
	BOOL m_bDownMiniBtn;



	BOOL m_bMouseTrack;


	//Ëø¶¨Í¼±ê
	Image * m_pLockImage;          
	Image * m_pUnLockImage; 
	BOOL m_bLock;
	BOOL m_bDownLockBtn;
	BOOL m_bHoverLockBtn;
	

	void __UnInitCtrl(void);
	void  __DrawFrame(CDC *pDC);
public:
	void Fun_SetLock(BOOL bLock);

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnPaint();
protected:
	void _DrawDialog(CDC * pDC);
	void __DrawTitle(CDC * pDC);
private:
	void __InitPose(void);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
