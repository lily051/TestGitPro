#pragma once  
/**	@file    CTransCheckBox.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ͸��checkBox
*
*	@author	 yudan
*	@date	 2012/08/11
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/08/11 by yudan
*/


class CTransCheckBox : public CButton
{
	DECLARE_DYNAMIC(CTransCheckBox)

public:
	CTransCheckBox();
	~CTransCheckBox();


	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	Gdiplus::Font* m_pFontText;  //����
	SolidBrush* m_pSolidBrush;  //ˢ��

	BOOL m_bCheck;

protected:
	virtual void PreSubclassWindow();

public:
	//����ѡ��״̬
	void SetCheck(BOOL bCheck);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
