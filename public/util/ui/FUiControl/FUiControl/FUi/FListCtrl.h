#pragma once

#include <list>
#include "FHeadCtrl.h"
#include "FScrollBar.h"
// CFListCtrl

#define MSG_MULTIBTN_CLICK (WM_USER + 200) // �ఴť�����Ϣ

// ��ť��Ϣ�Ľṹ��
typedef struct multibtn_info{
    int nItem;              // �ఴť������ 
    int nNumber;            // �ఴť�еĵڼ���
}STRU_MULTIBTN_INFO;

enum flist_style
{
    FLS_ZEBRA = 0,  // ������
    FLS_LINE  = 1,  // ����
	FLS_BLACK = 2,	//��ɫ��
};

class AFX_EXT_CLASS CFListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(CFListCtrl)

    enum{
        IDC_LIST_CHECKBOX_BEGIN  = 1000 , // Checkbox����ʼID
        IDC_LIST_BUTTON_BEGIN    = 5000 , // ��ť����ʼID
        IDC_LIST_BUTTON_END      = 50000, // ��ť�Ľ���ID

        BUTTON_MAXNUM            = 5000 , // ��ť�������
        CHECKBOX_SIZE            = 12   , // Checkbox��С
        ITEM_CHECKBOX_WIDTH      = 32   , // Checkbox�����еĿ��
        BUTTON_WIDTH             = 60   , // ��ť���
        BUTTON_HEIGHT            = 25   , // ��ť�߶�
        MULTI_BUTTON_HEIGHT      = 20   , // �ఴť�ĸ߶�
        SUBITEM_MARGIN           = 10   , // ���ֱ߾�
        BMPIMAGE_SIZE            = 16   , // ����ͼ��Ĵ�С
        HEAD_WIDTH_MIN           = 30   , // ��ͷ��С���
    };

public:
    CFListCtrl();
    virtual ~CFListCtrl();

	void SetToolTipEnable(bool bValue);
    // �����ͷ
    int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
    // ������
    int InsertItem(const LVITEM* pItem);
    int InsertItem(int nItem, LPCTSTR lpszItem);
    int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
    // ɾ����
    int DeleteItem(int nItem);
    // ɾ��������
    BOOL DeleteAllItems();
    // ����checkbox
    void SetCheckBox(bool bCreate = true);
    // �����Ƿ�ѡ��checkbox, 1-ѡ�У� 0-ȡ��ѡ��
    void SetItemChecked(int nItem, int nChecked);
    // CheckBox�Ƿ�ѡ��
    bool IsItemChecked(int nItem);
    // ���ñ�ͷ�Ƿ�����
    void SetHeadLock(int nItem, bool isLock = true);
    // ���ñ�ͷ�ܷ�����
    void SetHeadSort(int nItem, bool bSort = true);
    // ����ͼ���б�
    int SetImageList(LPCTSTR lpszImagePath);
    // �����б���
    BOOL SetItem(const LVITEM* pItem);
    BOOL SetItem(int nItem, int nSubItem, LPCTSTR lpszItem, int nImage);
    // ���ð�ť
    BOOL SetButton(int nItem, int nSubItem, LPCTSTR lpszButton, UINT nID);
    // �ఴť�����֣�ÿ����ͬ, ��һ������Ϊ��ť��������Ϊ����������Ϊzip�е�ͼƬ��
    BOOL SetButton(int nSubItem, int nBtnNum, LPCTSTR lpszImage, ...);

    BOOL SetButtonText(int nItem, int nSubItem, LPCTSTR lpszButton);

	BOOL SetButtonToolTip(int nBtnNum, LPCTSTR strToolTip, ...);
    // ������ʽ,�б���ʽ: 0-�����ƣ�1-����
    void SetStyle(int nStyle);
    // �����б�ͷ�߶�
    void SetHeadHeight(int nHeight);
    // ��ȡ�б�ͷ�߶�
    int GetHeadHeight();
    // ��ȡ�б�ͷ������ɫ
    void SetHeadBgColor(COLORREF colorBg);
    // �����и�
    void SetHeight(int nHeight);

	void SetMutibtnSize(int nHeight, int nWidth = -1);

    int GetHoverItem() const { return m_nHoverItem; }

    // ���ù������󸸴��ڸı䣬����ʵ�ʵĸ����ڣ��ؼ�����ʹ�õ�������ָ�����Ҫ���ã�
    void SetActrueParent(CWnd* pParent){ m_pActrueParent = pParent; }

protected:

	int m_nMultiBtnHeight;
	int m_nMultiBtnWidth;

	int m_nItem;
	int m_nSubItem;
	CToolTipCtrl m_toolTip;
	bool m_bEnableToolTip;
	CPoint m_ptLastHoverBtn;

    bool m_isDataBegin;      // �Ƿ�������ݵ�������
    bool m_isCheckBox;       // �Ƿ�ѡ��checkbox
    bool m_isHeadCreateBox;  // �б�ͷ�Ƿ��Ѿ����CheckBox
    int m_nStyle;            // �б���ʽ
    int m_nHoverItem;        // �����ͣ����
    int m_nPreHoverItem;     // ��һ�������ͣ����
    int m_nScrollPos;        // ������λ��
    int m_nScrollPage;       // ������������ʾ���������
    int m_nHeight;           // �б���߶�
    std::vector<int> m_nSortVector;          // ����ʽ
    std::list<CFButton*> m_listCheckBox;                     // checkbox�б�
    std::map<std::pair<int, int>, CFButton*> m_mapButton;    // ��ť����
    int m_nMultiBtnSub;                                      // �ఴť������
    int m_nMultiBtnShow;                                     // �ఴť��ʾ�ĸ���
    CPoint m_ptMultiBtnHover;                                // �ఴť��ͣ����
    CPoint m_ptMultiBtnClick;                                // �ఴť�������
    std::vector<Image*> m_vecBtnImage;                       // �ఴťͼƬ
	std::vector<CString> m_vecBtnToolTip;                       // �ఴť��ʾ��Ϣ
    std::map<int, CString> m_mapFont;        // ����
    std::map<int, COLORREF> m_mapTextColor;  // ��ɫ
    CImageList m_imgList;                    // ͼ���б�
    CFHeadCtrl m_listHeader;	// �б�ͷ
	Image* m_pImgNoData;		// û������ʱ��ʾͼʾ
    CWnd* m_pActrueParent;      // ʵ�ʵĸ����ڣ����ù������󸸴��ڸı�

    // ����checkbox
    void InsertCheckBox(int nItem);
    // ������
    void DrawBg(CDC* pDC, CRect& rcItem, LPDRAWITEMSTRUCT lpDrawItemStruct);
    // ���ذ�ť
    void HideBtn();
    // ���б���
    void DrawListItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct);
    // ����ť
    virtual void DrawBtn(CDC* pDC, LV_COLUMN& lvColumn, LV_ITEM& lvItem, CRect& rcSubItem);
    // ����ǰ�Ĳ�����ˢ�������
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
