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

    // �˵���ĸ߶�
    int m_nItemHeight;

    // ����ͼƬ
    Image* m_pImgBkg;

    // ÿ���˵���Ĳ���
    typedef struct _item_data
    {
        CString strText;        // �˵�����
        CString strIcon;        // ͼ������
        int nFlags;             // 

        _item_data()
        {
            nFlags = 0;
        }
    }ITEM_DATA;

    // �˵������
    std::map<UINT_PTR, ITEM_DATA*> m_mapItemData;

    // ����
    CString m_strFontName;

    // ������ɫ���ûҵ�������ɫ
    COLORREF m_clrText, m_clrGrayText, m_clrHoverText;
    
    // �߿���ɫ
    COLORREF m_clrBorder;

    // ��ȡ�ָ����ĸ���
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


