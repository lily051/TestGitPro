#pragma once

#include <vector>
#include "FButton.h"
// CFHeadCtrl

#define WM_HEADCTRL_CHECKBOX  (WM_USER + 1999)
#define WM_HEADCTRL_CHECKBOX_CHANGED (WM_USER + 2000)

// �б�����ʽ
enum list_sort_mode
{
    LIST_SORT_NO = 0,       // ��֧������
    LIST_SORT_YES,          // ֧������
    LIST_SORT_DESCENDING,   // ����
    LIST_SORT_ASCENDING,    // ����
};

class CFHeadCtrl : public CHeaderCtrl
{
    DECLARE_DYNAMIC(CFHeadCtrl)

public:
    CFHeadCtrl();
    virtual ~CFHeadCtrl();

    // ����checkbox
    void SetCheckBox(bool bCreate = false);
    // ��Ӽ�����
    void AddLock();
    // �����Ƿ�����
    void SetLock(int nItem, bool isLock);
    // �������
    void AddSort();
    // ��������ʽ
    void SetSort(int nItem, int nSortMode);
    // ���ø߶�
    void SetHeight(int nHeight);
    // ��ȡ�߶�
    int GetHeight();
    // ���ñ�����ɫ
    void SetBgColor(COLORREF colorBg);

protected:
    bool m_isCheckBox;       // �Ƿ�ѡ��checkbox
    int m_height;            // �߶�
    COLORREF m_colorBg;      // ����ɫ
    Image* m_pImgSort;       // ����ͼ��
    CFButton* m_pCheckBox;   // checkbox
    std::vector<bool> m_isLockVec;          // �Ƿ�����
    std::vector<int> m_nSortVec;            // ����ʽ
    std::map<int, CString> m_mapFont;        // ����
    std::map<int, COLORREF> m_mapTextColor;  // ��ɫ

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
