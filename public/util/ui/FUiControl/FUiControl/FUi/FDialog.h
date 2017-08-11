/**   @file		FDialog.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    �Ի�Ի���,
 *
 *    @author	jddd
 *    @date		2015/07/28
 *
 *    @note 	
 *    @note 	
 *    @note 	
 *
 *    @warning 	1������֧�ֿ�����ק�ı��С
                2��XP��û�о�������
                3��Win7 basic ��δ����
 */


#pragma once

#include "FuiBase.h"
#include "FNcButton.h"
#include "fuilib.h"
#include "WndShadow.h"
#include <map>

// CFDialog �Ի���

class AFX_EXT_CLASS CFDialog : public CDialog
{
	DECLARE_DYNAMIC(CFDialog)

public:
    CFDialog(UINT nIDTemplate, CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CFDialog();

protected:
    DECLARE_MESSAGE_MAP()

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog();

protected:	
    // �������ô��ڵı߿򣨰������������ĳߴ�
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    // ���ô�����С�ߴ�
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    // �����ڴ�С�ı�ʱ����ť��λ�õĸı�
    afx_msg void OnSize(UINT nType, int cx, int cy);
    // �ǿͻ���������Ϣ
    afx_msg void OnNcPaint();
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    afx_msg void OnNcMouseLeave();
    afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
    // ���ڼ�����Ϣ
    afx_msg BOOL OnNcActivate(BOOL bActive);
    afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
    // �ǿͻ������
    afx_msg LRESULT OnNcHitTest(CPoint point);

    // �ͻ���������Ϣ
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

    afx_msg void OnPaint();

	// ���ӿؼ������ʾ��Ϣ
	LRESULT OnMsgCtrlToolTip(WPARAM, LPARAM);
	LRESULT OnMsgCtrlToolTipEvent(WPARAM, LPARAM);

	void SetToolTipRect(DWORD dwControlID, CRect rcControl);
private:
    // �Ի�ǿͻ���
    void DrawNc(CDC* pDC);
    // ���ƶԻ��򱳾�
    void DrawDialog(CDC* pDC);
    // ��ʼ��ϵͳ��ť
    void InitSysBtn();
    // ����ϵͳ��ť
    void DestorySysBtn();
    // �Ի���ı��С���ƶ�ϵͳ��ť��λ��
    void MoveSysBtn();

    // �ж����ָ���Ƿ��ڰ�ť��
    int SysBtnHitTest(CPoint pt);

private:
    // �ؼ�����
    CString m_strObjectName;
    // �������ֵ�����
    CString m_strCaptionFont;
    // �������ֵ���ɫ
    COLORREF m_clrTitle;
    // �������ֵĶ��뷽ʽ��DT_LEFT��DT_CENTER
    int m_nTitleAlign;

    // ����ͼƬ
    Image *m_pImgBkg;
    // ����ͼƬ�и�ߴ磨9����
    std::vector<int> m_vecMargin;
    // ����ͼƬ�и�������
    std::vector<RectF> m_vecRcfBkg;
  
    // �Զ��尴ť<ID, ptr>
    std::map<int, CFNcButton*> m_mapSysBtn;
    CPPToolTip m_toolTip;

    // ͼ������
    CRect m_rcIcon;

    // ����mouseleave��Ϣ
    BOOL m_bNcTracking;
    // ��ǰ����hover״̬�İ�ť
    int m_nBtnHover;

    // ���ڱ߿򴦵���Ӱ
    CWndShadow m_shadow;

    // Բ��
    CRgn m_Rgn;
public:
    // ���ÿؼ�����
	void SetObjectName(LPCTSTR szObjectName);

    // �Ի������Ķ��뷽ʽ
    void SetTitleAlign(int nAlign);

    // ���ñ��������
    void SetTitleFont(const CString& strFontName);

    // ���ñ������ֵ���ɫ
    void SetTitleColor(COLORREF clrTitle);

    // ���öԻ���ı���ͼƬ
    BOOL SetMargin(std::vector<int>& vecMargin);

    // �����������Զ��尴ť
    /*
       ��ν����Զ����������ť�ĵ����Ϣ��
       �ڶԻ������У����� OnCommand ��ʵ�֣�wParam �����ľ����Զ��尴ť��ID
       ����İ�ťIDӦ��> 505
       ��ť��˳���Ǹ���ID����ģ��������С������
    */
     CFNcButton * AddCustomBtnToSysBtn(LPCTSTR szObjectName, LPCTSTR szText, 
        LPCTSTR szBtnTip, const int nID, const int nFixedWidth = 0);

    // �ӱ�����ɾ���Զ���İ�ť
    // �ýӿ�ֻ�ǲ������Զ���İ�ť����ť�����ٺ�ɾ��������Ҫ�ϲ��Լ��㶨
    void DeleteCustomBtn(const int nID);

	LRESULT  ProcMsg(MSG msg);

	void SetDrawTitleIcon(BOOL bDraw = TRUE);

	void SetTitleLeftMargin(int nLeftMargin);

    void SetTitleTextLeft(int nLeft);
    //void SetShadow();
protected:
	CRect m_rcTitle;

	//�Ƿ���Ҫ���Ʊ�����ͼ��
	//Ĭ�ϻ��ƣ���ĳЩ�������ΪFALSE��ʾ����Ҫ�Ի������ͼ��
	BOOL m_bDrawTitleIcon;

	//������ͼ����߾�
	int m_nTitleLeftMargin;
    //����������������߾�
    int m_nTitleTextLeft;

};
