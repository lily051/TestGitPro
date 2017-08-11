/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	31:7:2008   10:54
	filename: 	c:\guoll\Mydemo\5.2\Playback_Version3.0\Playback_Version3.0\PlayView_WndArrangeOper.cpp
	file path:	c:\guoll\Mydemo\5.2\Playback_Version3.0\Playback_Version3.0
	file base:	PlayView_WndArrangeOper
	file ext:	cpp
	author:		Angle Pig
	
	purpose:	������ͼ����ָ�ģʽ����
*********************************************************************/
//lint -library
#include "StdAfx.h"
#include "PlayView_WndArrangeOper.h"
#include "PreviewS20Dll.h"

CPlayView_WndArrangeOper::CPlayView_WndArrangeOper(void)
{
	//��Ա������ʼ��
	m_uiWndNum = MAX_LAYOUT_WND_NUM;
	m_uiShowWndNum = 0;

	//��ǰѡ�еĴ���ָ��Ϊ��
	m_pZoomWnd = NULL;

	//����ǰ��ע�ᴰ��ָ���ÿ�
	int i = 0;
	for(i = 0; i < MAX_LAYOUT_WND_NUM;i++)
	{
		m_pPlayWnd[i] = NULL;
	}


	//�������߿�Ļ���
	m_BorderPen[0].CreatePen(PS_SOLID,2,theApp.GetUIColor(UI_COLOR_TYPE_PV));
	m_BorderPen[1].CreatePen(PS_SOLID,2,theApp.GetUIColor(UI_COLOR_TYPE_RW_BORDER));
}

CPlayView_WndArrangeOper::~CPlayView_WndArrangeOper(void)
{
	//ע������ע����Ĵ���ָ��
	_PlayView_WndArrangeOper_UnAssignAllOutput();

	//�ͷŻ���
	for(int i = 0; i < 2;i++)
	{
		if(NULL != m_BorderPen[i].m_hObject)
		{
			DeleteObject(m_BorderPen[i].m_hObject);
			m_BorderPen[i].m_hObject = NULL;
		}
	}
}

/*************************************************
Function:    	PlayView_WndArrangeOper_SetMaxWndNum
Description:	������󴰿���
Input:		    uiMaxWndNum  ���Ĵ�����
Output:      	
Return:			�Ƿ����óɹ�
*************************************************/
BOOL CPlayView_WndArrangeOper::PlayView_WndArrangeOper_SetMaxWndNum(unsigned int uiMaxWndNum)
{
	if(uiMaxWndNum <= 0)
	{
		uiMaxWndNum = 1;
	}else if(uiMaxWndNum >= MAX_LAYOUT_WND_NUM)
	{
		uiMaxWndNum = MAX_LAYOUT_WND_NUM;
	}

	m_uiWndNum = uiMaxWndNum;
	return TRUE;
}
  
/*************************************************
Function:    	PlayView_WndArrangeOper_GetMaxWndNum
Description:	��ȡ��󴰿���
Input:		   
Output:      	
Return:		    ������󴰿���	
*************************************************/
unsigned int CPlayView_WndArrangeOper::PlayView_WndArrangeOper_GetMaxWndNum(void)
{
	return m_uiWndNum;
}

/*************************************************
Function:    	PlayView_WndArrangeOper_AssignPlayWnd
Description:	ע�ᴰ��ָ��
Input:		    pWnd ����ָ�� 
				uiWndIndex ��������
Output:      	
Return:			�Ƿ�ע��ɹ�
*************************************************/
BOOL CPlayView_WndArrangeOper::PlayView_WndArrangeOper_AssignPlayWnd(CWnd * pWnd, unsigned int uiWndIndex)
{
	//����ǷǷ�������ţ�����
	if(__IsValidWndIndex(uiWndIndex) != TRUE)
	{
		return FALSE;
	}

	m_pPlayWnd[uiWndIndex] = pWnd;

	return TRUE;
}

/*************************************************
Function:    	PlayView_WndArrangeOper_SetZoom
Description:	���ý��㴰��
Input:			pWnd ����ָ��
Output:      	
Return:			�Ƿ����óɹ�
*************************************************/
BOOL CPlayView_WndArrangeOper::PlayView_WndArrangeOper_SetZoom(CWnd * pWnd)
{
	//���pWnd == NULL ����û��ѡ�д���
	if(pWnd == NULL)
	{
		m_pZoomWnd = NULL;
		return TRUE;
	}

	//�ж��Ƿ�ô����Ѿ�ע��
	if(__IsAddWnd(pWnd) >= 0)
	{
		if(m_pZoomWnd != pWnd)
		{
			m_pZoomWnd = pWnd;
		}

		return TRUE;
	}

	return 0;
}

/*************************************************
Function:    	PlayView_WndArrangeOper_SetZoom
Description:	���ý��㴰��
Input:			uiWndIndex ��������
Output:      	
Return:			�Ƿ����óɹ�
*************************************************/
BOOL CPlayView_WndArrangeOper::PlayView_WndArrangeOper_SetZoom(unsigned int uiWndIndex)
{//����ǷǷ�������ţ�����

	if(__IsValidWndIndex(uiWndIndex) != TRUE)
	{
		return FALSE;
	}

	if(m_pPlayWnd[uiWndIndex] != NULL)
	{
		return PlayView_WndArrangeOper_SetZoom(m_pPlayWnd[uiWndIndex]);
	}

	return FALSE;
}

int CPlayView_WndArrangeOper::PlayView_WndArrangeOper_GetZoom()
{//����ǷǷ�������ţ�����

	int iIndex = __IsAddWnd(m_pZoomWnd);
	return iIndex;
}

/*************************************************
Function:    	__IsAddWnd
Description:	�Ƿ�����ע��Ĵ���
Input:			pWnd ����ָ��
Output:      	
Return:			���ش��ڵ�����
*************************************************/
int CPlayView_WndArrangeOper::__IsAddWnd(CWnd * pWnd)
{
	if(!pWnd)
	{
		return -1;
	}

	for(UINT i = 0; i < m_uiWndNum;i++)
	{
		if(m_pPlayWnd[i] && pWnd == m_pPlayWnd[i])
		{
			return i;
		}
	}

	return -1;
}

/*************************************************
Function:    	PlayView_WndArrangeOper_UnAssignAllOutput
Description:	ע������ע����Ĵ���ָ��
Input:		
Output:      	
Return:			
*************************************************/
void CPlayView_WndArrangeOper::_PlayView_WndArrangeOper_UnAssignAllOutput(void)
{
	for(UINT i = 0; i < m_uiWndNum;i++)
	{
		m_pPlayWnd[i] = NULL;
	}

	m_pZoomWnd = NULL;

	m_uiShowWndNum = 0;
}

/*************************************************
Function:    	_PlayView_WndArrangeOper_SetWndArrMode
Description:	���ò�����ͼ�Ļ���ָ�ģʽ
Input:			nMode ����ָ����
Output:      	
Return:			�Ƿ����óɹ�
*************************************************/
BOOL CPlayView_WndArrangeOper::_PlayView_WndArrangeOper_SetWndArrMode(WORD wMode)
{
	////�ж��Ƿ����û��Զ���ģʽ


	//�Ų����ڵ�λ��
	__UIFun_IniPose_SetOutputMode(wMode);

	////���ڻ���ָ�ģʽ�仯


	return TRUE;
}

BOOL CPlayView_WndArrangeOper::_UIFun_Update_OutputPose()
{
	WORD wMode = 0;

	int iAreas = m_LayoutCtrl.GetArrangeMode(wMode);
	m_uiShowWndNum = iAreas;
	RECT cRectOutput;
	int i = 0;

	BOOL bFullScreen = m_LayoutCtrl.IsZoom();

	if(iAreas > 0)
	{
		for(i = 0; i < iAreas;i++)
		{
			if(m_pPlayWnd[i] != NULL)
			{
				if(bFullScreen)
				{

					if(m_pPlayWnd[i] == m_pZoomWnd)
					{
						if(m_LayoutCtrl.GetRects(0,&cRectOutput) >= 0)
						{
							m_pPlayWnd[i]->MoveWindow(&cRectOutput);
							m_pPlayWnd[i]->ShowWindow(SW_SHOW);
						}else
						{
							m_pPlayWnd[i]->ShowWindow(SW_HIDE);
						}
					}else
					{

						m_pPlayWnd[i]->ShowWindow(SW_HIDE);
					}
				}else
				{
					if(m_LayoutCtrl.GetRects(i,&cRectOutput) >= 0)
					{
						m_pPlayWnd[i]->MoveWindow(&cRectOutput);
						m_pPlayWnd[i]->ShowWindow(SW_SHOW);
					}else
					{
						m_pPlayWnd[i]->ShowWindow(SW_HIDE);
					}
				}
			}
		}

		for(i = iAreas;i < m_uiWndNum;i++)
		{
			if(m_pPlayWnd[i] != NULL)
			{
				m_pPlayWnd[i]->ShowWindow(SW_HIDE);
			}
		}
	}

	_PlayView_WndArrangeOper_WndArrModeChange();

	return TRUE;
}

/*************************************************
Function:    	__UIFun_IniPose_SetOutputMode
Description:	���û���ָ�ģʽ
Input:		
Output:      	
Return:			
*************************************************/
BOOL CPlayView_WndArrangeOper::__UIFun_IniPose_SetOutputMode(WORD wMode)
{
	if(m_uiWndNum <= 0)
	{
		return FALSE;
	}

	int iAreas = 0;


	if(m_LayoutCtrl.IsZoom())
	{

		m_LayoutCtrl.Zoom();
	}

	iAreas = m_LayoutCtrl.SetArrangeMode(wMode);

	if(iAreas > 0)
	{
		m_uiShowWndNum = iAreas;
		_UIFun_Update_OutputPose();

		return TRUE;
	}

	return FALSE;
}

/*************************************************
Function:    	_PlayView_WndArrangeOper_SetSize
Description:	���ò�����ͼ�Ĵ�С
Input:		    rect ��С
Output:      	
Return:			
*************************************************/
void CPlayView_WndArrangeOper::_PlayView_WndArrangeOper_SetSize(RECT rect)
{
	if(m_uiWndNum <= 0)
	{
		return;
	}

	//��ʼ������ָ��ģ
	m_LayoutCtrl.SetInfoFirst(rect);

	_UIFun_Update_OutputPose();
}

/*************************************************
Function:    	_PlayView_WndArrangeOper_WndArrModeChange
Description:	
Input:		
Output:      	
Return:			
*************************************************/
void CPlayView_WndArrangeOper::_PlayView_WndArrangeOper_WndArrModeChange()
{

}


/*************************************************
Function:    	_PlayView_WndArrangeOper_SetEnlargeWnd
Description:	��ѡ�д��ڵ�����Ŵ�
Input:		
Output:      	
Return:			�Ƿ�ɹ�
*************************************************/
BOOL CPlayView_WndArrangeOper::_PlayView_WndArrangeOper_SetEnlargeWnd(void)
{
	ASSERT(m_pZoomWnd);

	UINT nSelectIndex = 0;
	CRect cRectOutput(0,0,0,0);

	//����������Ѿ�ע��Ĵ���
	if(__IsAddWnd(m_pZoomWnd) >= 0)
	{	
		m_LayoutCtrl.Zoom();

		_UIFun_Update_OutputPose();

		return TRUE;
	}

	return FALSE;
}


/*************************************************
Function:    	_PlayView_WndArrangeOper_DrawOutputBorder
Description:	�����Ŵ��ڵı߿���
Input:		
Output:      	
Return:			
*************************************************/
void CPlayView_WndArrangeOper::_PlayView_WndArrangeOper_DrawOutputBorder(CDC * pDC)
{
	ASSERT(pDC);

	CPen * pOldPen = NULL;

	CRect cRectClient(0,0,0,0);

	pDC->SelectStockObject(NULL_BRUSH);

	for(UINT i = 0; i < m_uiWndNum;i++)
	{
		if(m_pPlayWnd[i] && m_pPlayWnd[i]->IsWindowVisible())
		{
			//�ж��Ƿ��ǵ�ǰѡ�еĴ���
			if(m_pZoomWnd == m_pPlayWnd[i])
			{
				//����ǵ�ǰѡ�д���
				pOldPen = pDC->SelectObject(&m_BorderPen[1]);

			}else
			{
				//������ǵ�ǰѡ�д���
				pOldPen = pDC->SelectObject(&m_BorderPen[0]);
			}

			m_pPlayWnd[i]->GetWindowRect(&cRectClient);

			m_pPlayWnd[i]->GetParent()->ScreenToClient(&cRectClient);

			cRectClient.InflateRect(1,1,2,2);
			pDC->Rectangle(cRectClient);

			if(pOldPen)
			{
				pDC->SelectObject(pOldPen);
				pOldPen = NULL;
			}
		}
	}
}

/*************************************************
Function:    	__IsValidWndIndex
Description:	�ж��Ƿ�����Ч�Ĵ���������
Input:			uiWndIndex ����������
Output:      	
Return:			�Ƿ���������Ч
*************************************************/
BOOL CPlayView_WndArrangeOper::__IsValidWndIndex(unsigned int uiWndIndex)
{
	if(uiWndIndex < 0u || uiWndIndex >= m_uiWndNum)
	{
		return FALSE;
	}

	return TRUE;
}

/*************************************************
Function:    	PlayView_WndArrangeOper_GetWndArrMode
Description:	��ȡ��ǰ�Ļ���ָ�ģʽ
Input:		
Output:      	iArrangeMode ����ָ�ģʽ
Return:			�Ƿ��ȡ�ɹ�
*************************************************/
BOOL CPlayView_WndArrangeOper::PlayView_WndArrangeOper_GetWndArrMode(WORD & wArrangeMode)
{
	if(m_LayoutCtrl.GetArrangeMode(wArrangeMode) > 0)
	{

		return TRUE;
	}

	return FALSE;
}

UINT CPlayView_WndArrangeOper::PlayView_WndArrangeOper_GetWndNum(void)
{

	return m_uiShowWndNum;
}

void CPlayView_WndArrangeOper::Fun_Init(void)
{

	/*m_LayoutCtrl.Fun_Init();*/
}

CWnd* CPlayView_WndArrangeOper::PlayView_WndArrangeOper_GetWnd(int iIndex)
{
	if(__IsValidWndIndex(iIndex) != TRUE)
	{
		return FALSE;
	}
	return m_pPlayWnd[iIndex];
}

int CPlayView_WndArrangeOper::PalyView_WndArrangeOper_GetWndIndex(CPoint &pt)
{
	CRect rc;
	for (UINT i = 0; i < m_uiWndNum; i ++)
	{
		if (m_pPlayWnd[i] != NULL)
		{
			m_pPlayWnd[i]->GetWindowRect(&rc);
			if (rc.PtInRect(pt))
			{
				return i;
			}
		}
	}

	return -1;
}

BOOL CPlayView_WndArrangeOper::PlayView_WndArrangeOper_ChangeWnd(int iSourceWnd, int iPurposeWnd)
{	
	if(!__IsValidWndIndex(iSourceWnd)  || !__IsValidWndIndex(iPurposeWnd))
	{
		return FALSE;
	}

	if (iSourceWnd == iPurposeWnd)
	{
		return TRUE;
	}

	CRect sourceRC,PurposeRC;
	CWnd* pTemp;

	m_LayoutCtrl.GetRects(iSourceWnd,&sourceRC);
	m_LayoutCtrl.GetRects(iPurposeWnd,&PurposeRC);

	m_pPlayWnd[iSourceWnd]->MoveWindow(&PurposeRC);
	m_pPlayWnd[iPurposeWnd]->MoveWindow(&sourceRC);

	pTemp = m_pPlayWnd[iSourceWnd];
	m_pPlayWnd[iSourceWnd] = m_pPlayWnd[iPurposeWnd];
	m_pPlayWnd[iPurposeWnd] = pTemp;
	return TRUE;

}