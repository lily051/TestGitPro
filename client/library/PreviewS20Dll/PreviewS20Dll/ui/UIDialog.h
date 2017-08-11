/**	@file    UIDialog.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   这里填写本文件的摘要。
 *
 *	@author	 liying
 *	@date	 2012/12/21
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    在原UIDialog基础上调整了PNG图片的尺寸以适应新的贴图
 *  @note    自己完成PNG图片资源的加载,包括从dll或文件
 *
 *	@warning 需要初始化GDI+
 *  @warning 需要在MFC项目中使用
 *  @warning 需要修改stdafx.h文件的引用
 *  @warning 可能需要调整UIDlgDefine.h文件的部分宏
 */
// UIDialog.h : header file
//

#pragma once

class PngMetrics 
{
public:
    PngMetrics(void){
        m_hResource = NULL;
        nTotalWidth = 0;
        nTotalHeight = 0;

        nCapHeight = 0;
        nCapLeftWidth = 0;
        nCapRightWidth = 0;

        nLeftBorderWidth = 0;
        nRightBorderWidth = 0;

        nBottomBorderHeight = 0;
    };
    ~PngMetrics(void){};

    int nTotalWidth;
    int nTotalHeight;

    int nCapHeight;
    int nCapLeftWidth;
    int nCapRightWidth;

    int nLeftBorderWidth;
    int nRightBorderWidth;

    int nBottomBorderHeight;

    static CString sResFileName;

    HINSTANCE m_hResource;
    
    void SetMode(int nMode)
    {
        switch (nMode)
        {
        case 0:
            {
                nCapLeftWidth = 4;
                nCapRightWidth = 4;
                nCapHeight = 30;
                nLeftBorderWidth = 2;
                nRightBorderWidth = 2;
                nBottomBorderHeight = 1;
            }
            break;
        case 1:
            {
                nCapLeftWidth = 4;
                nCapRightWidth = 4;
                nCapHeight = 30;
                nLeftBorderWidth = 2;
                nRightBorderWidth = 2;
                nBottomBorderHeight = 1;
            }
            break;
        case 2:
            {
                nCapLeftWidth = 4;
                nCapRightWidth = 4 + 9;
                nCapHeight = 30;
                nLeftBorderWidth = 2;
                nRightBorderWidth = 2 + 9;
                nBottomBorderHeight = 1 + 8;
            }
            break;
        }        
    };

    CRect GetCapLeft(BOOL bActive = FALSE)
    {
        CRect rc(0, 0, nCapLeftWidth, nCapHeight);
        if (bActive)
        {
            rc.OffsetRect(0, nCapHeight);
        }
        return rc;
    };

    CRect GetCapMid(BOOL bActive = FALSE)
    {
        CRect rc(nCapLeftWidth
            , 0
            , nTotalWidth - nCapRightWidth
            , nCapHeight);
        if (bActive)
        {
            rc.OffsetRect(0, nCapHeight);
        }
        return rc;
    };

    CRect GetCapRight(BOOL bActive = FALSE)
    {
        CRect rc(nTotalWidth - nCapRightWidth
            , 0
            , nTotalWidth
            , nCapHeight);
        if (bActive)
        {
            rc.OffsetRect(0, nCapHeight);
        }
        return rc;
    };

    CRect GetLeftBorder(void)
    {
        CRect rc( 0
            , nCapHeight + nCapHeight
            , nLeftBorderWidth
            , nTotalHeight - nBottomBorderHeight);
        return rc;
    };

    CRect GetRightBorder(void)
    {
        CRect rc( nTotalWidth - nRightBorderWidth
            , nCapHeight + nCapHeight
            , nTotalWidth
            , nTotalHeight - nBottomBorderHeight);
        return rc;
    };

    CRect GetBottomBorder(void)
    {
        CRect rc( 0
            , nTotalHeight - nBottomBorderHeight
            , nTotalWidth
            , nTotalHeight);
        return rc;
    };

    HBITMAP LoadPNG(UINT nID, DWORD cr);
};

// UIDialog dialog
class UIDialog : public CDialog
{
private:
	UIDialog(CWnd* pParent = NULL);
// Construction
public:
	UIDialog(UINT nIDTemplate, CWnd* pParent = NULL);	// standard constructor

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);

protected:

    class SystemButton
    {
    public:
        static const int exitWidth = 17;
        static const int maxWidth = 17;
        static const int minWidth = 17;
        static const int sysHeight = 17;

        SystemButton(UINT nHit, CRect & rc, int nWidth)
            : m_nHit(nHit)
            , m_rc(rc)
            , m_bHover(FALSE)
            , m_bLBDown(FALSE)
        {
            m_nWidth = nWidth;

            SetButtonRect(rc);
        }

        ~SystemButton(){};

        UINT GetButtonHit()
        {
            return m_nHit;
        }

        void SetButtonRect(CRect & rc)
        {
            m_rc = rc;

            m_rc.left = rc.right - m_nWidth;
            m_rc.bottom = rc.top + sysHeight;
        }

        BOOL IsLBDown()
        {
            return m_bLBDown;
        }

        void SetLButtonDown(BOOL bLBDown)
        {
            m_bLBDown = bLBDown;
        }

        void SetHover(BOOL bHover)
        {
            m_bHover = bHover;
        }

        BOOL PtInRect(CPoint & pt)
        {
            return m_rc.PtInRect(pt);
        }

        BOOL DrawButton(CDC * pDC, CDC * pBufferDC, BOOL bMax);
        

        int GetLeft(void)
        {
            return m_rc.left;
        }

    private:
        SystemButton();
        UINT m_nHit;
        CRect m_rc;
        BOOL m_bHover;
        BOOL m_bLBDown;

        int m_nWidth;
    };

    typedef CList<SystemButton*,SystemButton*> CSysButtonList;

    CSysButtonList m_listSysButton;

    CRect GetCaptionRect(void);

	void LoadSysButtons();

	CBitmap m_bitmap;
	CDC m_dcBuffer;
    PngMetrics m_pngMetrics;

	BOOL m_bActive;
	CBrush m_brushBkgnd;
	CFont m_font;
	BOOL m_bCenter;

	CRect m_rcHitCaption;
	CRect m_rcHitIcon;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnPaint();
	void DrawDialog(void);
protected:
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

    static void SetResFileName(CString sResFileName);
};


