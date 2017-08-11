#pragma once
/**	@file    VolumnSetWnd.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ����������ʾ
*
*	@author	 yudan
*	@date	 2012/07/24
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/07/24 by yudan
*/

#include "../ui/uiCtrl/CustomDrawBlackWnd.h"
#include "../ui/uiCtrl/BitmapSlider.h"
#include "../ui/uiBase/DrawStripFun.h"


class CVolumnSetWnd : public CWnd
{
public:
	CVolumnSetWnd();
	virtual ~CVolumnSetWnd();

	DECLARE_MESSAGE_MAP()

	
private:
	BOOL m_bMove;
	CBrush m_bkBrush;
	CFont m_font;
	UINT m_uiCloseTimer;//ʱ�Ӿ��
	UINT m_uiIdleTime;
	BOOL m_bVoice;      //��Ƶ�Ƿ��
	
	CWnd * m_pParentWnd;  //������
	CDC m_dcMem;

	CDrawStripFun m_DrawBakeFun;
	HBITMAP m_hBitmap;
	int m_nBitmapWidth;  //����ͼƬ���
	int m_nBitmapHeight;  //����ͼƬ�߶�

	CBitmapSlider * m_pSliderVolumn;
private:
	//��ʼ���ؼ�
	void InitCtrl();
	//�ƶ��ؼ�
	void MoveCtrl();
	//������ʱ��
	BOOL SetCloseTimer(void);
	//�ر�ʱ��
	void KillColseTimer(void);

public:
	//����
	BOOL CreateFloatWindow(CWnd *pParentWnd);
	//���ó�ʼ������
	void SetInitPos(int nVolumn);
	//���������
	LRESULT OnMsgSliderClick(WPARAM wParam, LPARAM lParam);
	//͸����ͼ
	void TransparentBk(CDC * pDc, CWnd * pWnd);

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
