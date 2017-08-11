#pragma once

#include <vector>
#include "FButton.h"
// CFHeadCtrl

#define WM_HEADCTRL_CHECKBOX  (WM_USER + 1999)
#define WM_HEADCTRL_CHECKBOX_CHANGED (WM_USER + 2000)

// 列表排序方式
enum list_sort_mode
{
    LIST_SORT_NO = 0,       // 不支持排序
    LIST_SORT_YES,          // 支持排序
    LIST_SORT_DESCENDING,   // 降序
    LIST_SORT_ASCENDING,    // 升序
};

class CFHeadCtrl : public CHeaderCtrl
{
    DECLARE_DYNAMIC(CFHeadCtrl)

public:
    CFHeadCtrl();
    virtual ~CFHeadCtrl();

    // 设置checkbox
    void SetCheckBox(bool bCreate = false);
    // 添加加锁定
    void AddLock();
    // 设置是否锁定
    void SetLock(int nItem, bool isLock);
    // 添加排序
    void AddSort();
    // 设置排序方式
    void SetSort(int nItem, int nSortMode);
    // 设置高度
    void SetHeight(int nHeight);
    // 获取高度
    int GetHeight();
    // 设置背景颜色
    void SetBgColor(COLORREF colorBg);

protected:
    bool m_isCheckBox;       // 是否选择checkbox
    int m_height;            // 高度
    COLORREF m_colorBg;      // 背景色
    Image* m_pImgSort;       // 排序图像
    CFButton* m_pCheckBox;   // checkbox
    std::vector<bool> m_isLockVec;          // 是否锁定
    std::vector<int> m_nSortVec;            // 排序方式
    std::map<int, CString> m_mapFont;        // 字体
    std::map<int, COLORREF> m_mapTextColor;  // 颜色

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBtnClickCheckBox();
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    LRESULT OnLayout(WPARAM wParam, LPARAM lParam);
    LRESULT OnChangeCheckBox(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    BOOL m_headerResizeDisabled;
    BOOL IsOnLastColumnDivider(const CPoint& point);

};
