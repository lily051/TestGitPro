/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	31:7:2008   10:54
	filename: 	c:\guoll\Mydemo\5.2\Playback_Version3.0\Playback_Version3.0\PlayView_WndArrangeOper.h
	file path:	c:\guoll\Mydemo\5.2\Playback_Version3.0\Playback_Version3.0
	file base:	PlayView_WndArrangeOper
	file ext:	h
	author:		Angle Pig
	
	purpose:	������ͼ����ָ�ģʽ����
*********************************************************************/
//#include "PlayViewStruct.h"
#include "Layoutctrl.h"
#pragma once

class CPlayView_WndArrangeOper
{
public:
	CPlayView_WndArrangeOper(void);
	virtual ~CPlayView_WndArrangeOper(void);

public:
	/*************************************************
	Function:    	PlayView_WndArrangeOper_GetMaxWndNum
	Description:	��ȡ��󴰿���
	Input:		   
	Output:      	
	Return:		    ������󴰿���	
	*************************************************/
	unsigned int PlayView_WndArrangeOper_GetMaxWndNum(void);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_SetMaxWndNum
	Description:	������󴰿���
	Input:		    uiMaxWndNum  ���Ĵ�����
	Output:      	
	Return:			�Ƿ����óɹ�
	*************************************************/
	BOOL PlayView_WndArrangeOper_SetMaxWndNum(unsigned int uiMaxWndNum);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_AssignPlayWnd
	Description:	ע�ᴰ��ָ��
	Input:		    pWnd ����ָ�� 
					uiWndIndex ��������
	Output:      	
	Return:			�Ƿ�ע��ɹ�
	*************************************************/
	BOOL PlayView_WndArrangeOper_AssignPlayWnd(CWnd * pWnd, unsigned int uiWndIndex);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_SetZoom
	Description:	���ý��㴰��
	Input:			pWnd ����ָ��
	Output:      	
	Return:			�Ƿ����óɹ�
	*************************************************/
	BOOL PlayView_WndArrangeOper_SetZoom(CWnd * pWnd);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_SetZoom
	Description:	���ý��㴰��
	Input:			uiWndIndex ��������
	Output:      	
	Return:			�Ƿ����óɹ�
	*************************************************/
	BOOL PlayView_WndArrangeOper_SetZoom(unsigned int uiWndIndex);
protected:
	BOOL _UIFun_Update_OutputPose();
	/*************************************************
	Function:    	PlayView_WndArrangeOper_UnAssignAllOutput
	Description:	ע������ע����Ĵ���ָ��
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void _PlayView_WndArrangeOper_UnAssignAllOutput(void);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_SetWndArrMode
	Description:	���ò�����ͼ�Ļ���ָ�ģʽ
	Input:			nMode ����ָ����
	Output:      	
	Return:			�Ƿ����óɹ�
	*************************************************/
	BOOL _PlayView_WndArrangeOper_SetWndArrMode(WORD wMode);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_SetSize
	Description:	���ò�����ͼ�Ĵ�С
	Input:		    rect ��С
	Output:      	
	Return:			
	*************************************************/
	void _PlayView_WndArrangeOper_SetSize(RECT rect);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_WndArrModeChange
	Description:	
	Input:		
	Output:      	
	Return:			
	*************************************************/
	virtual void _PlayView_WndArrangeOper_WndArrModeChange();

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_SetEnlargeWnd
	Description:	��ѡ�д��ڵ�����Ŵ�
	Input:		
	Output:      	
	Return:			�Ƿ�ɹ�
	*************************************************/
	BOOL _PlayView_WndArrangeOper_SetEnlargeWnd(void);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_DrawOutputBorder
	Description:	�����Ŵ��ڵı߿���
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void _PlayView_WndArrangeOper_DrawOutputBorder(CDC * pDC);
protected:
	unsigned int m_uiShowWndNum;           //��ʾ���ڵĸ���
	unsigned int m_uiWndNum;               //��󲥷Ŵ�����					
	CWnd * m_pPlayWnd[MAX_LAYOUT_WND_NUM];//��ע��Ĵ���ָ��
	CWnd * m_pZoomWnd;	                   //���㴰�ڵ�ָ��
	
	CPen m_BorderPen[2];				   //�����ڱ߿�Ļ���
	
	CLayoutctrl m_LayoutCtrl;
private:
	/*************************************************
	Function:    	__IsAddWnd
	Description:	�Ƿ�����ע��Ĵ���
	Input:			pWnd ����ָ��
	Output:      	
	Return:			���ش��ڵ�����
	*************************************************/
	int __IsAddWnd(CWnd * pWnd);

	/*************************************************
	Function:    	__UIFun_IniPose_SetOutputMode
	Description:	���û���ָ�ģʽ
	Input:		
	Output:      	
	Return:			
	*************************************************/
	BOOL __UIFun_IniPose_SetOutputMode(WORD wMode);

	/*************************************************
	Function:    	__IsValidWndIndex
	Description:	�ж��Ƿ�����Ч�Ĵ���������
	Input:			uiWndIndex ����������
	Output:      	
	Return:			�Ƿ���������Ч
	*************************************************/
	BOOL __IsValidWndIndex(unsigned int uiWndIndex);
public:
	/*************************************************
	Function:    	PlayView_WndArrangeOper_GetWndArrMode
	Description:	��ȡ��ǰ�Ļ���ָ�ģʽ
	Input:		
	Output:      	iArrangeMode ����ָ�ģʽ
	Return:			�Ƿ��ȡ�ɹ�
	*************************************************/
	BOOL PlayView_WndArrangeOper_GetWndArrMode(WORD & wArrangeMode);
	int PlayView_WndArrangeOper_GetZoom();

	UINT PlayView_WndArrangeOper_GetWndNum(void);
	void Fun_Init(void);

	public:
	CWnd* PlayView_WndArrangeOper_GetWnd(int iIndex);
	int PalyView_WndArrangeOper_GetWndIndex(CPoint &pt);
	BOOL PlayView_WndArrangeOper_ChangeWnd(int iSourceWnd, int iPurposeWnd);

};
