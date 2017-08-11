#pragma once
/**	@file    TransButton.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ͸��������ť
*
*	@author	 yudan
*	@date	 2012/08/20
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/08/20 by yudan
*/


class CTransButton : public CButton
{
	DECLARE_DYNAMIC(CTransButton)

public:
	CTransButton();
	~CTransButton();

private:
	BOOL m_bMouseTrack;
	BOOL m_bHover;
	BOOL m_bLock;
	BOOL m_bSelect;

	BOOL m_bInit;

	Image * m_pImage;
	Image * m_pImageHoverBk;
	Image * m_pImageSelBk;

	CBitmap m_bitmapBk;


public:
	Gdiplus::Font* m_pFontText;  //����
	SolidBrush* m_pSolidBrush;  //ˢ��
	int m_nBtnID;  //��ť�ı�ʾID����������Ӧ

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	//����ͼƬ������
	void Load(UINT nIDResource);
	//����ѡ�б���ͼƬ
	void LoadSelectBkgnd(UINT nIDHover, UINT nIDSelect);
	//����״̬
	void LockState();
	//���ð�ťѡ��״̬
	void SetSelect(BOOL bSelect);

protected:
	virtual void PreSubclassWindow();

public:
	afx_msg void OnBnClicked();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
