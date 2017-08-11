#pragma once
/**	@file    IconStatic.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ͼ��static,��Ӧ����¼�
*
*	@author	 yudan
*	@date	 2012/08/27
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/08/27 by yudan
*/



class CIconStatic : 
	public CStatic
{
	DECLARE_DYNAMIC(CIconStatic)

public:
	CIconStatic();
	virtual ~CIconStatic();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void DrawItem(CDC * pDC);

public:
	Image * m_pImage;

	BOOL m_bMouseTrack;
	BOOL m_bHove;
	BOOL m_bSelect;

	CFont m_font;
	CString m_strText;

public:
	void Load(UINT nID);
	void LoadImage(Image * pImage);

	//����static�Ƿ���ѡ��״̬
	void SetStaticSel(BOOL bSelect);
	// ��������.
	void SetText(const CString& strText);

	afx_msg void OnPaint();
	LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

};
