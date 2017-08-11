#pragma once
/**	@file    BtnStatic
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   static�Ի�ؼ�
*
*	@author	 yudan
*	@date	 2012/03/29
*
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/03/29 by yudan
*/

//lint -library

class CBtnStatic : 
    public CStatic
{
   // DECLARE_DYNAMIC(CBtnStatic)

public:
    CBtnStatic();
    virtual ~CBtnStatic();

protected:
    DECLARE_MESSAGE_MAP()

protected:
    void DrawItem(CDC * pDC);

protected:
    virtual void PreSubclassWindow();

public:
    Image * m_pImage;
    Image * m_pImageSel;

    BOOL m_bMouseTrack;
    BOOL m_bHove;
    BOOL m_bSelect;

    CFont m_font;
    CFont m_fontSel;

public:
    void Load(UINT nID, UINT nIDSel);
    void LoadImage(Image * pImage, Image * pImageSel);

    //����static�Ƿ���ѡ��״̬
    void SetStaticSel(BOOL bSelect);

    afx_msg void OnPaint();
    LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
    LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

};