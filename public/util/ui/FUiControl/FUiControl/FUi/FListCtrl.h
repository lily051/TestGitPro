#pragma once

#include <list>
#include "FHeadCtrl.h"
#include "FScrollBar.h"
// CFListCtrl

#define MSG_MULTIBTN_CLICK (WM_USER + 200) // 多按钮点击消息

// 按钮信息的结构体
typedef struct multibtn_info{
    int nItem;              // 多按钮所在行 
    int nNumber;            // 多按钮中的第几个
}STRU_MULTIBTN_INFO;

enum flist_style
{
    FLS_ZEBRA = 0,  // 斑马纹
    FLS_LINE  = 1,  // 线性
	FLS_BLACK = 2,	//黑色调
};

class AFX_EXT_CLASS CFListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(CFListCtrl)

    enum{
        IDC_LIST_CHECKBOX_BEGIN  = 1000 , // Checkbox的起始ID
        IDC_LIST_BUTTON_BEGIN    = 5000 , // 按钮的起始ID
        IDC_LIST_BUTTON_END      = 50000, // 按钮的结束ID

        BUTTON_MAXNUM            = 5000 , // 按钮最大数量
        CHECKBOX_SIZE            = 12   , // Checkbox大小
        ITEM_CHECKBOX_WIDTH      = 32   , // Checkbox所在列的宽度
        BUTTON_WIDTH             = 60   , // 按钮宽度
        BUTTON_HEIGHT            = 25   , // 按钮高度
        MULTI_BUTTON_HEIGHT      = 20   , // 多按钮的高度
        SUBITEM_MARGIN           = 10   , // 文字边距
        BMPIMAGE_SIZE            = 16   , // 插入图标的大小
        HEAD_WIDTH_MIN           = 30   , // 表头最小宽度
    };

public:
    CFListCtrl();
    virtual ~CFListCtrl();

	void SetToolTipEnable(bool bValue);
    // 插入表头
    int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
    // 插入项
    int InsertItem(const LVITEM* pItem);
    int InsertItem(int nItem, LPCTSTR lpszItem);
    int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
    // 删除项
    int DeleteItem(int nItem);
    // 删除所有项
    BOOL DeleteAllItems();
    // 设置checkbox
    void SetCheckBox(bool bCreate = true);
    // 设置是否选中checkbox, 1-选中， 0-取消选中
    void SetItemChecked(int nItem, int nChecked);
    // CheckBox是否选中
    bool IsItemChecked(int nItem);
    // 设置表头是否锁定
    void SetHeadLock(int nItem, bool isLock = true);
    // 设置表头能否排序
    void SetHeadSort(int nItem, bool bSort = true);
    // 设置图标列表
    int SetImageList(LPCTSTR lpszImagePath);
    // 设置列表项
    BOOL SetItem(const LVITEM* pItem);
    BOOL SetItem(int nItem, int nSubItem, LPCTSTR lpszItem, int nImage);
    // 设置按钮
    BOOL SetButton(int nItem, int nSubItem, LPCTSTR lpszButton, UINT nID);
    // 多按钮无文字，每项相同, 第一个参数为按钮数，后面为不定参数，为zip中的图片名
    BOOL SetButton(int nSubItem, int nBtnNum, LPCTSTR lpszImage, ...);

    BOOL SetButtonText(int nItem, int nSubItem, LPCTSTR lpszButton);

	BOOL SetButtonToolTip(int nBtnNum, LPCTSTR strToolTip, ...);
    // 设置样式,列表样式: 0-斑马纹，1-线性
    void SetStyle(int nStyle);
    // 设置列表头高度
    void SetHeadHeight(int nHeight);
    // 获取列表头高度
    int GetHeadHeight();
    // 获取列表头背景颜色
    void SetHeadBgColor(COLORREF colorBg);
    // 设置行高
    void SetHeight(int nHeight);

	void SetMutibtnSize(int nHeight, int nWidth = -1);

    int GetHoverItem() const { return m_nHoverItem; }

    // 启用滚动条后父窗口改变，设置实际的父窗口（控件中有使用到父窗口指针才需要调用）
    void SetActrueParent(CWnd* pParent){ m_pActrueParent = pParent; }

protected:

	int m_nMultiBtnHeight;
	int m_nMultiBtnWidth;

	int m_nItem;
	int m_nSubItem;
	CToolTipCtrl m_toolTip;
	bool m_bEnableToolTip;
	CPoint m_ptLastHoverBtn;

    bool m_isDataBegin;      // 是否从无数据到有数据
    bool m_isCheckBox;       // 是否选择checkbox
    bool m_isHeadCreateBox;  // 列表头是否已经添加CheckBox
    int m_nStyle;            // 列表样式
    int m_nHoverItem;        // 鼠标悬停的项
    int m_nPreHoverItem;     // 上一个鼠标悬停的项
    int m_nScrollPos;        // 滑动条位置
    int m_nScrollPage;       // 滑动条区域显示的最大项数
    int m_nHeight;           // 列表项高度
    std::vector<int> m_nSortVector;          // 排序方式
    std::list<CFButton*> m_listCheckBox;                     // checkbox列表
    std::map<std::pair<int, int>, CFButton*> m_mapButton;    // 按钮索引
    int m_nMultiBtnSub;                                      // 多按钮所在列
    int m_nMultiBtnShow;                                     // 多按钮显示的个数
    CPoint m_ptMultiBtnHover;                                // 多按钮悬停坐标
    CPoint m_ptMultiBtnClick;                                // 多按钮点击坐标
    std::vector<Image*> m_vecBtnImage;                       // 多按钮图片
	std::vector<CString> m_vecBtnToolTip;                       // 多按钮提示信息
    std::map<int, CString> m_mapFont;        // 字体
    std::map<int, COLORREF> m_mapTextColor;  // 颜色
    CImageList m_imgList;                    // 图标列表
    CFHeadCtrl m_listHeader;	// 列表头
	Image* m_pImgNoData;		// 没有数据时显示图示
    CWnd* m_pActrueParent;      // 实际的父窗口，启用滚动条后父窗口改变

    // 插入checkbox
    void InsertCheckBox(int nItem);
    // 画背景
    void DrawBg(CDC* pDC, CRect& rcItem, LPDRAWITEMSTRUCT lpDrawItemStruct);
    // 隐藏按钮
    void HideBtn();
    // 画列表项
    void DrawListItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct);
    // 画按钮
    virtual void DrawBtn(CDC* pDC, LV_COLUMN& lvColumn, LV_ITEM& lvItem, CRect& rcSubItem);
    // 插入前的操作，刷新区域等
    void BeforeInsert();

    afx_msg LRESULT OnHeadCtrlCheckBox(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnListItemCheckBox(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBtnClick(UINT nID);

    void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void PreSubclassWindow();
    afx_msg void OnMouseLeave();
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnNMHover(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	

    DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    BOOL m_trackingHeaderDivider;
    int m_oldColumnWidth;

    int GetLastColumnIndex() { return GetHeaderCtrl()->GetItemCount() - 1; }
};
