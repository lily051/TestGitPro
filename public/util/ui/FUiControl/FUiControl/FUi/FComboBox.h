#pragma once
#include "FuiBase.h"
#include "FEdit.h"

// CFComboBox

class AFX_EXT_CLASS CFComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CFComboBox)

public:
	CFComboBox();
	virtual ~CFComboBox();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnCbnKillfocus();
    afx_msg void OnCbnSetfocus();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
    // ���໯�������
    CFEdit m_edit;
    // ��������
    std::map<int, CString> m_mapFont;
    // �߿���ɫ����̬���н���״̬
    std::map<int, COLORREF> m_mapBorderColor;
    // ������ɫ
    std::map<int, COLORREF> m_mapTextColor;
    // ������ť��ͼƬ��Ŀǰû��ʵ�֣�
    Image *m_pImgArrow;
    // ������ť�Ŀ�Ⱥ͸߶�
    int m_nArrowWidth, m_nArrowHeight;
    // �ؼ�Ϊ��������״̬
    BOOL m_bError;
    // �ؼ��Ƿ��н���
    BOOL m_bFocus;
    // �ؼ�����
    CString m_strObjectName;

    //
    CRect m_rcArrow;
    // 
    BOOL m_bMouseTrack;
    // ���hover��ť
    BOOL m_bHoverBtn;

public:
    // ��갴�°�ť
    BOOL m_bPressBtn;

	//�Ƿ񵯳��Ի���
	BOOL m_bPopupDlg;

	// ��ʾ����
	TCHAR m_szTipText[MAX_LEN_TOOLTIP];
private:
    // �ؼ��߿�
    void DrawBorder(CDC* pDC, CRect& rcWnd);
    // ��������ť
    void DrawArrowBtn(CDC* pDC, CRect& rcArrow);

public:

    void SetObjectName(LPCTSTR szObjectName);
    // ������ʾ����
    // SetItemHeight ��Ҫ���ڸýӿ�֮����ã�����������������߶ȱ�����
    void SetTextFont(int nState, const CString& strFontName);
    // ���ñ߿���ɫ
    void SetBoderColor(int nState, COLORREF color);
    // ������ʾ����
    void SetDefaultText(LPCTSTR szDefaultText);
    // ������ʾ������ɫ
    void SetTextColor(int nState, COLORREF color);
    // ��ȡ���֣��ؼ��Դ��Ĳ����ٵ��ã�
    void GetText(LPTSTR szText);
    // �������֣��ؼ��Դ��Ĳ����ٵ��ã�
    void SetText(LPCTSTR szText);
    // �����Ƿ�Ϊ�������״̬
    void SetState(BOOL bError = FALSE);
    // �������
    void ClearText();
	// ����Ϊ���ܱ༭ģʽ
	void SetReadOnly(BOOL bReadOnly = TRUE);

	void SetPopupDlg(BOOL bPopup = TRUE);

	void SetTipText(LPCTSTR szTipText);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


