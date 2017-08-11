/**   @file		FNcButton.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    自定义按钮类，只能用在标题栏上
 *
 *    @author	jddd
 *    @date		2015/08/11
 *
 *    @note 	
 *    @note 	
 *    @note 	
 *
 *    @warning 	
 */

#pragma once

class CFNcButton
{
public:
	CFNcButton(void);
	~CFNcButton(void);

private:
    // 父窗口指针
	CWnd *m_pParent;
    // 鼠标是否Hover
	BOOL m_bHover;
    // 按钮是否按下
	BOOL m_bPressed;
    // 按钮是否不可用
	BOOL m_bDisabled;
    // 按钮文字
	CString m_strText;
    // 文字颜色
	COLORREF m_clrText;
    // 文字对其方式
	int m_nTextAlign;
    // 文字的字体
	CString m_strTextFont;

    // 按钮ID
	int m_nID;
//     // 按钮的位置
// 	CRect m_rcRect;
    // 控件的名称
	CString m_strObjectName;
    // 背景图片
	Image* m_pImgBkg;
    // 鼠标提示文字
    CString m_strToolTip;

public:

	// 是否始终Hover样式
	BOOL m_bAlwHover;
	// 按钮的位置
	CRect m_rcRect;

public:
	void DrawButton(CDC* pDC);
	void SetObjectName(const CString& strObjectName);
	BOOL CreateButton(const CString& strText, CRect rcRect, CWnd* pParent, int nID);
	void EnableButton(BOOL bEnable);
	int GetControlID();
	void SetTextFont(const CString& strFontName);
	void GetRect(CRect& rcRect);
	void MoveWindow(CRect rcRect);

    void LButtonDown();
    void LButtonUp();
    void MouseHover();
    void MouseLeave();

    void SetToolTip(const CString& strToolTip);
    CString GetToolTip();

    void SetFixedWidth(const int nWidth);

	// 设置始终Hover样式
	void LockHover(BOOL bLock = TRUE);
};
