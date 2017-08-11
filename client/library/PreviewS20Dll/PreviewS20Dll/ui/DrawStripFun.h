/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	5:4:2008   13:53
	filename: 	DrawStripFun.h
	file base:	DrawStripFun
	file ext:	h
	author:		GuoLinlin
	purpose:	�����ε���
*********************************************************************/
#pragma once
#define PASTEBMP_LEFT  0 //���
#define PASTEBMP_MID   1 //�м�
#define PASTEBMP_RIGHT 2 //�ұ�

class CDrawStripFun
{
public:
	CDrawStripFun(void);
	~CDrawStripFun(void);
	/*************************************************
	Function:    	DrawStrip_Loadbmp
	Description:	LoadͼƬ
	Input:			nLeft  ���ͼƬID
					nMid   �м�ͼƬID
					nRight �ұ�ͼƬID
	Output:      	
	Return:			
	*************************************************/
	void DrawStrip_Loadbmp(UINT nLeft, UINT nMid, UINT nRight);

	/*************************************************
	Function:    	DrawStrip_DrawFun
	Description:	��ָ������
	Input:			pDC ��ͼ��DC
					rectBoundary ָ���ľ�������
	Output:      	
	Return:			
	*************************************************/
	void DrawStrip_DrawFun(CDC * pDC, LPRECT rectBoundary);

	/*************************************************
	Function:    	DrawStrip_Loadbmp
	Description:	LoadͼƬ
	Input:			hLeft  ���ͼƬ���
					hMid   �м�ͼƬ���
					hRight �ұ�ͼƬ���
	Output:      	
	Return:			
	*************************************************/
	void DrawStrip_Loadbmp(HBITMAP hLeft, HBITMAP hMid, HBITMAP hRight);
private:
	CBitmap m_gPasteBmp[3];
};
