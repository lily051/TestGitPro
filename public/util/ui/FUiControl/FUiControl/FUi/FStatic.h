/**   @file		FStatic.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    1
 *
 *    @author	Zhangyy
 *    @date		2015/07/27
 *
 *    @note 	2
 *    @note 	3
 *    @note 	4
 *
 *    @warning 	5
 */
#pragma once
#include "FuiBase.h"
/*
 1、支持自定义字体
 2、支持超出控件的文本用...显示
 3、支持设置鼠标悬浮时的指针样式
 4、支持设置背景色
*/

// CFStatic

class AFX_EXT_CLASS CFStatic : public CStatic
{
	DECLARE_DYNAMIC(CFStatic)

public:
	CFStatic();
	virtual ~CFStatic();

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);    // 
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);  // 设置鼠标指针
    virtual void PreSubclassWindow();   // 修改窗口自绘风格
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

    void UpdateFace();
private:
    // 是否设置背景色
    BOOL m_bTransparent;
    // 背景色
    COLORREF m_bgColor;
    // 文字颜色
    COLORREF m_clrText;
    // 文字字体样式
    CString m_strTextFontName;
    // 鼠标悬停的指针样式
    HCURSOR m_hCursor;
    // 文字对齐方式
    int m_nTextAlign;

	// 提示文字
	TCHAR m_szTipText[MAX_LEN_TOOLTIP];
public:	
    /**
    * @brief 设置鼠标悬浮时的指针样式
    * @param [in]hCursor 设置的鼠标指针
    * @return void
    * @author JdDd
    * @version v1.0.0.0
    * @date 2015-7-27
    * @since 
    * @bug 
    * @warning 
    */
    void SetHoverCursor(HCURSOR hCursor);
    
    /**
    * @brief	设置文字样式
    * @param	
    * @return	
    * @author	zhangyy
    * @version	v0.1
    * @date		2015/07/27
    * @since	
    * @bug		
    * @warning	
    */
    void SetTextFont(const CString& strFontName);
    
    /**   @brief		设置控件的背景色
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetBgcolor(BOOL bTransparent, COLORREF color);
    
    /**   @brief		设置文字颜色
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetTextColor(COLORREF color);
    
    /**   @brief		设置文字
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetText(LPCTSTR szText);
   
    /**   @brief		设置文字对其方式
     *    @brief	
     *    @param[in] nAlign 复用系统定义好的DT_LEFT、DT_CENTER等等
     *    @param	
     *    @return	
     */
    void SetTextAlign(int nAlign);

	// 单独设置控件的提示文字
	void SetTipText(LPCTSTR szTipText);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


