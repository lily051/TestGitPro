#pragma once
#include "fuilib.h"

// CFMenu

class AFX_EXT_CLASS CFMenu : public CMenu
{
	DECLARE_DYNAMIC(CFMenu)

public:
	CFMenu();
	virtual ~CFMenu();

    virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

private:
    CString m_strObjectName;

    // 菜单项的高度
    int m_nItemHeight;

    // 背景图片
    Image* m_pImgBkg;

    // 每个菜单项的参数
    typedef struct _item_data
    {
        CString strText;        // 菜单文字
        CString strIcon;        // 图标名称
        int nFlags;             // 

        _item_data()
        {
            nFlags = 0;
        }
    }ITEM_DATA;

    // 菜单项参数
    std::map<UINT_PTR, ITEM_DATA*> m_mapItemData;

    // 字体
    CString m_strFontName;

    // 字体颜色、置灰的字体颜色
    COLORREF m_clrText, m_clrGrayText, m_clrHoverText;
    
    // 边框颜色
    COLORREF m_clrBorder;

    // 获取分隔符的个数
    int GetSeparatorCount();

public:

    void SetObjectName(LPCTSTR szObjectName);

    void SetItemHeight(int nItemHeight);

    BOOL CreatePopupMenu();

    BOOL DestoryMenu();

    BOOL AddMenu(UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem = NULL, LPCTSTR lpszIcon = NULL);

    void SetTextFont(const CString& strFontName);

	void SetTextColor(COLORREF clrText, COLORREF clrGrayText, COLORREF clrHoverText = RGB(255, 255, 255));

    void SetBorderColor(COLORREF clrBorder);
};


