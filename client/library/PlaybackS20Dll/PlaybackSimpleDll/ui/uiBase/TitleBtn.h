#pragma once
/**	@file    TitleBtn.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ���ⰴť
*
*	@author	 yudan
*	@date	 2012/08/20
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/08/20 by yudan
*/


class CTitleButton : public CButton
{
	DECLARE_DYNAMIC(CTitleButton)

public:
	CTitleButton();
	~CTitleButton();

private:
	BOOL m_bInit;
	Image * m_pImage;
	CBitmap m_bitmapBk;
	int m_nFontSize;
	int m_bCenter;    //�����Ƿ����

public:
	Gdiplus::Font* m_pFontText;  //����
	SolidBrush* m_pSolidBrush;  //ˢ��

public:

	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	//����ͼƬ������
	void Load(UINT nIDResource);

	//���־���
	void CenterText(BOOL bCenter = TRUE);

protected:
	virtual void PreSubclassWindow();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
