#pragma once
#include "FuiBase.h"
#include "FNcButton.h"

#define MSG_CHECKBOX_CLICK (WM_USER + 102) // checkbox点击消息

/*
    1' 按状态（normal, hover, pressed, disable）设置图片
    2‘ 文字前设置图标
    3’ 按状态自定义字体
    4' 支持超出控件的文本用...显示
    5' 支持设置鼠标悬浮时的指针样式
*/

// FButton

class AFX_EXT_CLASS CFButton : public CButton
{
	DECLARE_DYNAMIC(CFButton)

public:
	CFButton();
	virtual ~CFButton();

     BOOL CornerBtnHit(CPoint pt);
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	// 按钮背景图：允许设置索引号指向竖排拼接图片的编号，用于不变状态的按钮，m_nIconIndex为第几个，m_nIconTotal为ICON总个数（竖排）
	int m_nIconIndex;
	int m_nIconTotal;
    // 按钮背景图：必须有4中状态，图片竖排拼接
    Image *m_pImgBkg;
    // 按钮左侧的图标
    Image *m_pImgIcon;
    // 按钮字体：普通，悬停，按下，置灰
    std::map<int, CString> m_mapFont;
    // 鼠标在按钮上的指针
    HCURSOR m_hCursor;
    // 文字颜色
    std::map<int, COLORREF> m_mapTextColor;
    // 文字对齐方式
    int m_nTextAlign;
    // 文字边距
    int m_nTextMargin;
    // 文字上边距
    int m_nTextTopMargin;
    // 说明文字上边距
    int m_nSubNameMargin;
    // icon左边距
    int m_nIconLeftMargin;
    // icon上边距
    int m_nIconTopMargin;
    // 按钮背景色，暂不支持背景透明，所以需要设置一个颜色
    COLORREF m_bgColor;
    // 提示文字
    TCHAR m_szTipText[MAX_LEN_TOOLTIP];

    /* checkbox样式属性 */
    Image *m_pImgCheckBox;
    BOOL m_bCheckBox;
    BOOL m_isInListCtrl;    // 是否在列表中使用
    int m_nChecked;

    /* radio 样式属性*/

    // 控件名称
    CString m_strObjectName;
    // 捕获鼠标悬停状态
    BOOL m_bMouseTrack, m_bMouseHover;

    // Icon 
    CString m_strIconName;

    std::tr1::shared_ptr<CFNcButton> m_ptrCBtn;

    // 说明文字
    CString m_strSubName;
    std::map<int, CString> m_mapSubNameFont;
    std::map<int, COLORREF> m_mapSubNameColor;

	// 是否始终Hover样式
	BOOL m_bAlwHover;

	// 是否用户手动设置边距
	BOOL m_bUserMargin;	
public:

	//设置显示Iocn图标编号
	void SetIconIndex(int nIndex);
    // 设置控件名称
	void SetObjectName(LPCTSTR szObjectName);
    // 文字前设置图标
    void SetIcon(LPCTSTR szIconName, int nIconTotal = 4);
    // 设置图标的左边距和上编剧也
    void SetIconMargin(int nLeftMargin, int nTopMargin);
    // 设置鼠标指针
    void SetHoverCursor(HCURSOR hCursor);
    // 按状态设置字体  参见：ctrl_state
    void SetTextFont(int nState, const CString& strFontName);
    // 设置字体颜色
    void SetTextColor(int nState, COLORREF clrText);
    // 设置文字对齐方式：DT_LEFT, DT_CENTER, DT_RIGHT
    void SetTextAlign(int nAlign);
    // 设置文字边距
    void SetTextMargin(int nMargin);
    // 设置文字上边距, 说明文字上边距
    void SetTextTopMargin(int nTopMargin, int nSubMargin);

    // 单独设置控件的提示文字
    void SetTipText(LPCTSTR szTipText);

    // 设置固定宽度
    void SetFixedWidth(const int nWidth);

    // 设置按钮背景色，目前使用双缓冲和GDI+的方式绘图，
    // 无法把图片中的透明部分画出来，需要上层设置一个颜色相似的背景色
    void SetBgColor(COLORREF color);

    // 添加右上角按钮
    void AddCornerBtn(const int nBtnID, const CString& strCornerBtnName);
    // 移除右上角按钮
    void RemoveCornerBtn();

    /* checkbox 样式*/
    // 设置为checkbox, 一定要在 SetObjectName 之后调用
    void SetCheckBox(BOOL bSet = TRUE);
    // 设置为在列表中
    void SetInListCtrl(BOOL bSet = TRUE);
    // 设置选中
    void SetCheck(int nCheck);
    // 获取选中状态
    int GetCheck();

    // 设置说明文字
    void SetSubName(const CString& strSubName);
    void SetSubNameFont(int nState, const CString& strFontName);
    void SetSubNameColor(int nState, COLORREF clrText);

	// 设置始终Hover样式
	void LockHover(BOOL bLock = TRUE);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


