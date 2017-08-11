#pragma once
#include "FuiBase.h"

// CFProgress

class AFX_EXT_CLASS CFProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CFProgress)

public:
	CFProgress();
	virtual ~CFProgress();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();

private:
    // 背景图片
    Gdiplus::Image *m_pImgBkg;
    // 控件名称
    CString m_strObjectName;
    // 字体颜色
    COLORREF m_clrText;
    // 显示字体
    CString m_strTextFontName;
    // 自定义显示文字
    CString m_strText;

public:
    // 设置控件名称
    void SetObjectName(LPCTSTR szObjectName);

    // 设置显示文字的颜色
    void SetTextColor(COLORREF clrText);

    // 自定义显示的文字
    void SetText(LPCTSTR szText);

    // 设置字体
    void SetTextFont(const CString& strFontName);
};


