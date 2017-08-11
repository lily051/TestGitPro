#pragma once
#include "CustomLayoutData.h"


class CLayoutctrl
{
public:
	CLayoutctrl(void);
	~CLayoutctrl(void);
	BOOL GetArrangeModeParam(WORD wMode,PLAYOUT_PARAM pLayout);

	int	SetInfoFirst(RECT sRect);
	/*************************************************
	Function:    SetArrangeMode
	Description������Ԥ��ģʽ 
	Input:       iMode��Ԥ��ģʽ
	Output:      ��
	Return:		 ��ģʽ�µ�������
	*************************************************/
	int	SetArrangeMode(WORD wMode);

	/*************************************************
	Function:    GetArrangeMode
	Description����ȡԤ��ģʽ 
	Input:       
	Output:      wMode��Ԥ��ģʽ
	Return:		 ��ģʽ�µ�������
	*************************************************/
	int GetArrangeMode(WORD &wMode);

	/*************************************************
	Function:    GetRects
	Description����ȡ��ǰģʽ��Ԥ�����������С 
	Input:       iIndex��������
	����
	  Output:      pRect�����ص������С
	  Return:		 0
	  *************************************************/
	int	GetRects(int iIndex, RECT * pRect);
	/*************************************************
	Function:    Zoom
	Description��ȫ��ģʽ�л� 
	Input:       ��
	����
	  Output:      ��
	  Return:		 0
	  *************************************************/
	BOOL Zoom();

	BOOL IsZoom();
private:
private:
	BOOL	m_bInitialize; 			/*��ģ���Ƿ��ʼ��	*/
	RECT	m_sShowRect;			/*��ʾ�����λ�ô�С*/
	WORD m_wCurLoyoutMode;
	BOOL m_bFullScreen;
	int m_iZoomIndex;
public:
	void Fun_Init(void);
	void Fun_UnInit(void);
};


