#pragma once
#include "FuiBase.h"
#include "FNcButton.h"

#define MSG_CHECKBOX_CLICK (WM_USER + 102) // checkbox�����Ϣ

/*
    1' ��״̬��normal, hover, pressed, disable������ͼƬ
    2�� ����ǰ����ͼ��
    3�� ��״̬�Զ�������
    4' ֧�ֳ����ؼ����ı���...��ʾ
    5' ֧�������������ʱ��ָ����ʽ
*/

// FButton

class AFX_EXT_CLASS CFButton : public CButton
{
	DECLARE_DYNAMIC(CFButton)

public:
	CFButton();
	virtual ~CFButton();

     BOOL CornerBtnHit(CPoint pt);
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	// ��ť����ͼ����������������ָ������ƴ��ͼƬ�ı�ţ����ڲ���״̬�İ�ť��m_nIconIndexΪ�ڼ�����m_nIconTotalΪICON�ܸ��������ţ�
	int m_nIconIndex;
	int m_nIconTotal;
    // ��ť����ͼ��������4��״̬��ͼƬ����ƴ��
    Image *m_pImgBkg;
    // ��ť����ͼ��
    Image *m_pImgIcon;
    // ��ť���壺��ͨ����ͣ�����£��û�
    std::map<int, CString> m_mapFont;
    // ����ڰ�ť�ϵ�ָ��
    HCURSOR m_hCursor;
    // ������ɫ
    std::map<int, COLORREF> m_mapTextColor;
    // ���ֶ��뷽ʽ
    int m_nTextAlign;
    // ���ֱ߾�
    int m_nTextMargin;
    // �����ϱ߾�
    int m_nTextTopMargin;
    // ˵�������ϱ߾�
    int m_nSubNameMargin;
    // icon��߾�
    int m_nIconLeftMargin;
    // icon�ϱ߾�
    int m_nIconTopMargin;
    // ��ť����ɫ���ݲ�֧�ֱ���͸����������Ҫ����һ����ɫ
    COLORREF m_bgColor;
    // ��ʾ����
    TCHAR m_szTipText[MAX_LEN_TOOLTIP];

    /* checkbox��ʽ���� */
    Image *m_pImgCheckBox;
    BOOL m_bCheckBox;
    BOOL m_isInListCtrl;    // �Ƿ����б���ʹ��
    int m_nChecked;

    /* radio ��ʽ����*/

    // �ؼ�����
    CString m_strObjectName;
    // ���������ͣ״̬
    BOOL m_bMouseTrack, m_bMouseHover;

    // Icon 
    CString m_strIconName;

    std::tr1::shared_ptr<CFNcButton> m_ptrCBtn;

    // ˵������
    CString m_strSubName;
    std::map<int, CString> m_mapSubNameFont;
    std::map<int, COLORREF> m_mapSubNameColor;

	// �Ƿ�ʼ��Hover��ʽ
	BOOL m_bAlwHover;

	// �Ƿ��û��ֶ����ñ߾�
	BOOL m_bUserMargin;	
public:

	//������ʾIocnͼ����
	void SetIconIndex(int nIndex);
    // ���ÿؼ�����
	void SetObjectName(LPCTSTR szObjectName);
    // ����ǰ����ͼ��
    void SetIcon(LPCTSTR szIconName, int nIconTotal = 4);
    // ����ͼ�����߾���ϱ��Ҳ
    void SetIconMargin(int nLeftMargin, int nTopMargin);
    // �������ָ��
    void SetHoverCursor(HCURSOR hCursor);
    // ��״̬��������  �μ���ctrl_state
    void SetTextFont(int nState, const CString& strFontName);
    // ����������ɫ
    void SetTextColor(int nState, COLORREF clrText);
    // �������ֶ��뷽ʽ��DT_LEFT, DT_CENTER, DT_RIGHT
    void SetTextAlign(int nAlign);
    // �������ֱ߾�
    void SetTextMargin(int nMargin);
    // ���������ϱ߾�, ˵�������ϱ߾�
    void SetTextTopMargin(int nTopMargin, int nSubMargin);

    // �������ÿؼ�����ʾ����
    void SetTipText(LPCTSTR szTipText);

    // ���ù̶����
    void SetFixedWidth(const int nWidth);

    // ���ð�ť����ɫ��Ŀǰʹ��˫�����GDI+�ķ�ʽ��ͼ��
    // �޷���ͼƬ�е�͸�����ֻ���������Ҫ�ϲ�����һ����ɫ���Ƶı���ɫ
    void SetBgColor(COLORREF color);

    // ������Ͻǰ�ť
    void AddCornerBtn(const int nBtnID, const CString& strCornerBtnName);
    // �Ƴ����Ͻǰ�ť
    void RemoveCornerBtn();

    /* checkbox ��ʽ*/
    // ����Ϊcheckbox, һ��Ҫ�� SetObjectName ֮�����
    void SetCheckBox(BOOL bSet = TRUE);
    // ����Ϊ���б���
    void SetInListCtrl(BOOL bSet = TRUE);
    // ����ѡ��
    void SetCheck(int nCheck);
    // ��ȡѡ��״̬
    int GetCheck();

    // ����˵������
    void SetSubName(const CString& strSubName);
    void SetSubNameFont(int nState, const CString& strFontName);
    void SetSubNameColor(int nState, COLORREF clrText);

	// ����ʼ��Hover��ʽ
	void LockHover(BOOL bLock = TRUE);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


