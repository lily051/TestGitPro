/**   @file		FEdit.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 
 *
 *    @author	jddd
 *    @date		2015/08/13
 *
 *    @note 	
 *    @note 	
 *    @note 	
 *
 *    @warning 	1、单行文本模式下，如果需要竖直居中，程序需要对该空间改变一下大小才能达到效果
                2、要设置为多行文本，需要设置 ES_MULTILINE | ES_AUTOHSCROLL 两个属性
                3、设置为多行是，暂时只支持没有滚动条的模式
 */

#pragma once
#include "FuiBase.h"

// CFEdit

class AFX_EXT_CLASS CFEdit : public CEdit
{
	DECLARE_DYNAMIC(CFEdit)

public:
	CFEdit();
	virtual ~CFEdit();

    BOOL IsFocused() const { return m_bFocus; }
    BOOL IsEmpty() const { return m_bIsEmpty; }

protected:
	DECLARE_MESSAGE_MAP()

    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    virtual afx_msg void OnEnChange();
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	virtual void PreSubclassWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
    // 输入文字的字体
    std::map<int, CString> m_mapFont;
    // 文字颜色
    std::map<int, COLORREF> m_mapTextColor;
    // 边框颜色
    std::map<int, COLORREF> m_mapBorderColor;
    // 背景提示文字的字体
    CString m_strDefaultText;
    // 输入框是否提示输入错误
    BOOL m_bError;
    // 输入框是否有焦点
    BOOL m_bFocus;
    // 输入框是否输入为空
    BOOL m_bIsEmpty;
    // 是否在输入
    //BOOL m_bInput; 
    // 控件是否为密码样式
    BOOL m_bPassword;
    // 客户区相对位置偏移
    int m_nClientOffsetX;
    int m_nClientOffsetY;
    // 是否为多行
    BOOL m_bMultiLine;
	// 是否为ComboBox中的Edit组合控件
	BOOL m_bIsComboBox;

    //
	CBrush m_brTextBkg;
	CBrush m_brWhiteBkg;

	BOOL m_bCanGetText; // 是否允许获取文本

	// 提示文字
	TCHAR m_szTipText[MAX_LEN_TOOLTIP];
private:
    // 控件边框
    void DrawBorder(CDC* pDC, CRect& rcWnd);    
    // 设置输入竖直居中
    void VCenterEdit(BOOL bFlag, NCCALCSIZE_PARAMS* lpncsp);
public:
    /**   @brief		设置显示字体
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetTextFont(int nState, const CString& strFontName);
    /**   @brief		设置显示文字颜色
     *    @param	
     *    @param	
     *    @return	
     */
    void SetTextColor(int nState, COLORREF color);
    /**   @brief		设置提示文字
     *    @param	
     *    @param	
     *    @return	
     */
    void SetDefaultText(LPCTSTR szDefaultText);
    /**   @brief		设置边框颜色：常态和有焦点状态
     *    @param	
     *    @param	
     *    @return	
     */
    void SetBorderColor(int nState, COLORREF color);
    /**   @brief		设置是否为输入错误状态
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetErrorState(BOOL bError = FALSE);

    /**   @brief		设置为密码样式
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetPasswordMode(BOOL bSet);

     /**   @brief   获取密码文本
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void GetPasswordText(CString& strText);

	/*** ComboBox使用的接口 ***/
	/**   @brief		设置Edit为ComboBox组合控件
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
	void SetComboEdit(BOOL bSet){m_bIsComboBox = bSet;}
	void UpdateCtrl(){OnEnChange();}
	CString GetDefaultText() {return m_strDefaultText;}

	// 单独设置控件的提示文字
	void SetTipText(LPCTSTR szTipText);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

};


