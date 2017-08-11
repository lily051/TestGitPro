#include "StdAfx.h"
#include "Layoutctrl.h"
#include "PreviewS20Dll.h"

//lint -library
CLayoutctrl::CLayoutctrl(void)
{
	m_bInitialize = FALSE;
	m_bFullScreen = FALSE;
	m_sShowRect = CRect(0,0,0,0);

	m_wCurLoyoutMode = PREVIEW_ARRANGE_TYPE_2_2_A;
	m_iZoomIndex = 0;
}

CLayoutctrl::~CLayoutctrl(void)
{
	try
	{
		Fun_UnInit();
	}
	catch (...)
	{
	}
}

/*************************************************
Function:    SetInfoFirst
Description: ����Ԥ������Ĵ�С
Input:       sRect��Ԥ���ľ�������
Output:      ��
Return:		 0
*************************************************/
int	CLayoutctrl::SetInfoFirst(RECT sRect)
{
	m_sShowRect		= sRect;

	return 0;
}

/*************************************************
Function:    SetArrangeMode
Description������Ԥ��ģʽ 
Input:       iMode��Ԥ��ģʽ
Output:      ��
Return:		 ��ģʽ�µ�������
*************************************************/
int	CLayoutctrl::SetArrangeMode(WORD wMode)
{
	//ASSERT((iMode>=0)&&(iMode<=PREVEEW_ARRANGE_MAX_MODES));	

	//default value

	LAYOUT_PARAM LayoutMode;

	if(theApp.m_customLayoutData.GetLayoutData(wMode,&LayoutMode) == FALSE)
	{
		return 0;
	}

	m_wCurLoyoutMode	= wMode;
	m_bFullScreen		= FALSE;

	return LayoutMode.iAreas;
}

BOOL CLayoutctrl::GetArrangeModeParam(WORD wMode,PLAYOUT_PARAM pLayout)
{
	//ASSERT((iMode>=0)&&(iMode<=PREVEEW_ARRANGE_MAX_MODES));	

	//default value
	LAYOUT_PARAM LayoutMode;

	if(theApp.m_customLayoutData.GetLayoutData(wMode,&LayoutMode) == FALSE)
	{

		return 0;
	}


	memcpy(pLayout,&LayoutMode,sizeof(LAYOUT_PARAM));


	return 1;
}
/*************************************************
Function:    Zoom
Description��ȫ��ģʽ�л� 
Input:       ��
����
Output:      ��
Return:		 0
*************************************************/
BOOL CLayoutctrl::Zoom()
{
	m_bFullScreen = !m_bFullScreen;

	return m_bFullScreen;
}

BOOL CLayoutctrl::IsZoom()
{

	return m_bFullScreen;
}

void CLayoutctrl::Fun_Init(void)
{
	if(m_bInitialize == FALSE)
	{
		m_bInitialize = TRUE;
	}
}

void CLayoutctrl::Fun_UnInit(void)
{
	m_bInitialize = FALSE;
}

/*************************************************
Function:    GetRects
Description����ȡ��ǰģʽ��Ԥ�����������С 
Input:       iIndex��������
����
Output:      pRect�����ص������С
Return:		 0
*************************************************/
int	CLayoutctrl::GetRects(int iIndex, RECT * pRect)
{
	//ASSERT(m_bInitialize);
	ASSERT(iIndex >= 0 && iIndex < MAX_LAYOUT_WND_NUM);
	CRect rect;
	rect.CopyRect(&m_sShowRect);

	int iLinWeit = 2;

	if(m_bFullScreen)
	{
		if(iIndex!=0) return -1;

		rect.DeflateRect(iLinWeit,iLinWeit);

		*pRect = rect;

		return 0;
	}
	LAYOUT_PARAM LayoutMode;

	if(theApp.m_customLayoutData.GetLayoutData(m_wCurLoyoutMode,&LayoutMode) == FALSE)
	{

		return -1;
	}

	UINT i,j,istartx = 0,istarty = 0,iendx = 0,iendy = 0;
	int iareas;
	int isectw,isecth;
	BOOL bBingo;


	iareas = LayoutMode.iAreas;
	//	*iCount = iareas;
	if(iIndex>=iareas) 
	{
		return -1;
	}

	bBingo = FALSE;

	for(j=0;j<LayoutMode.uiHeitBoundLen;j++)
	{
		for(i=0;i<LayoutMode.uiWidBoundLen;i++)
		{
			if(LayoutMode.iOutIndex[j*LayoutMode.uiWidBoundLen+i] == iIndex)
			{
				if(!bBingo)
				{
					istartx = i;
					istarty = j;
					bBingo = TRUE;
				}

				iendx = i;
				iendy = j;
			}
		}
	}

	isectw = rect.Width()/LayoutMode.uiWidBoundLen;
	isecth = rect.Height() /LayoutMode.uiHeitBoundLen;


	pRect->left		=	rect.left + isectw*istartx + iLinWeit;
	pRect->top		=	rect.top + isecth*istarty + iLinWeit;
	//pRect->bottom	=	pRect->top+(iendy-istarty+1)*isecth;

	if(iendx+1==LayoutMode.uiWidBoundLen)
	{//ÿһ�е����һ��,����!
		pRect->right	=	rect.right - iLinWeit;
	}else
	{
		pRect->right	=	rect.left + isectw*(iendx + 1) - iLinWeit;
	}

	if(iendy+1==LayoutMode.uiHeitBoundLen)
	{//ÿһ�е����һ��,����!
		pRect->bottom	=	rect.bottom  - iLinWeit;	
	}else
	{
		pRect->bottom	=	rect.top + isecth*(iendy + 1) - iLinWeit;
	}


	return 0;
}
/*************************************************
Function:    GetArrangeMode
Description����ȡԤ��ģʽ 
Input:       
Output:      wMode��Ԥ��ģʽ
Return:		 ��ģʽ�µ�������
*************************************************/
int CLayoutctrl::GetArrangeMode(WORD &wMode)
{
	LAYOUT_PARAM LayoutMode;

	if(theApp.m_customLayoutData.GetLayoutData(m_wCurLoyoutMode,&LayoutMode))
	{
		wMode = m_wCurLoyoutMode;

		return LayoutMode.iAreas;

	}
	return 0;
}
/*
<?xml version="1.0"?>
	<LayoutData>
<Database>
<IPHistory>
</IPHistory>
<UserHistory>
</UserHistory>
<Password>
</Password>
</Database>
<Login>
<UserHistory>
</UserHistory>
<Password>
</Password>
<Login>
</StreamServerData>
*/
