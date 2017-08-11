#include "StdAfx.h"
#include "CustomLayoutData.h"
//#include "Markup.h"

CCustomLayoutData::CCustomLayoutData(void)
{
	Fun_Init();
}

CCustomLayoutData::~CCustomLayoutData(void)
{
	try
	{
		Fun_UnInit();
	}
	catch (...)
	{
		
	}
}

/*
<?xml version="1.0"?>
<controlLayoutCfgData>
	<CustomLayout>
		<Name>
		</Name>
		<AreaNum>
		</AreaNum>
		<Wid>
		</Wid>
		<Heit>
		</Heit>
		<Layout>
			<LayoutPer>
				<LayoutPerLeft>
				</LayoutPerLeft>
				<LayoutPerTop>
				</LayoutPerTop>
				<LayoutPerRight>
				</LayoutPerRight>
				<LayoutPerBottom>
				</LayoutPerBottom>
			</LayoutPer>
			...
		</Layout>
	</CustomLayout>
	...
</controlLayoutCfgData>
*/
BOOL CCustomLayoutData::GetData(PCUSTOMLAYOUTLIST pLayoutCfg)
{

	*pLayoutCfg = m_CustomLayoutList;
	//memcpy(pLayoutCfg,&m_CustomLayoutList,sizeof(CUSTOMLAYOUTLIST));
	return TRUE;
}

void CCustomLayoutData::Fun_Init(void)
{

	m_CustomLayoutList.uiCustomLayoutNum = MAX_CUSTOMLAYOUT_COUNT;

	for(int i = 0; i < MAX_CUSTOMLAYOUT_COUNT;i++)
	{

		sprintf_s(m_CustomLayoutList.CustomLayout[i].csCustomLayoutName,"自定义画面分割%d",i + 1);
		m_CustomLayoutList.CustomLayout[i].uiWidBound = 6;
		m_CustomLayoutList.CustomLayout[i].uiHeitBound = 6;
		m_CustomLayoutList.CustomLayout[i].uiSplitWndNum = 1;
		m_CustomLayoutList.CustomLayout[i].CustomLayoutNode[0].uiLeft = 0;
		m_CustomLayoutList.CustomLayout[i].CustomLayoutNode[0].uiTop = 0;
		m_CustomLayoutList.CustomLayout[i].CustomLayoutNode[0].uiRight = 6;
		m_CustomLayoutList.CustomLayout[i].CustomLayoutNode[0].uiBottom = 6;	
	}

	//初始化缺省风格模式
	__InitDefaultMode();
}

//void CCustomLayoutData::__InitCustomMode()
//{
//	UINT iNum = 0;
//	WORD wkey = PREVIEW_ARRANGE_TYPE_CUSTOM_01;
//	PLAYOUT_PARAM pLayout = NULL; 
//
//	while(iNum < m_CustomLayoutList.uiCustomLayoutNum && wkey <= PREVIEW_ARRANGE_TYPE_CUSTOM_08)
//	{
//		if(m_CustomLayoutList.CustomLayout[iNum].uiSplitWndNum == 0)
//		{
//
//			iNum++;
//			wkey++;
//			continue;
//		}
//
//		pLayout = new LAYOUT_PARAM;
//
//		if(pLayout == NULL )
//		{
//
//			iNum++;
//			wkey++;
//			continue;
//		}
//
//		pLayout->uiWidBoundLen = m_CustomLayoutList.CustomLayout[iNum].uiWidBound;
//		pLayout->uiHeitBoundLen = m_CustomLayoutList.CustomLayout[iNum].uiHeitBound;
//		pLayout->iAreas = m_CustomLayoutList.CustomLayout[iNum].uiSplitWndNum;
//		pLayout->bCustom = TRUE;
//
//		unsigned int i = 0;
//		unsigned int j = 0;
//		unsigned int k = 0;
//
//		unsigned uiRight = 0;
//		unsigned uiBottom = 0;
//		unsigned uiLeft = 0;
//		unsigned uiTop = 0;
//
//
//		for(k = 0; k < pLayout->iAreas;k++)
//		{
//			if(m_CustomLayoutList.CustomLayout[iNum].CustomLayoutNode[k].uiBottom > 0)
//			{
//				uiLeft = m_CustomLayoutList.CustomLayout[iNum].CustomLayoutNode[k].uiLeft;
//				uiTop = m_CustomLayoutList.CustomLayout[iNum].CustomLayoutNode[k].uiTop;
//				uiRight = m_CustomLayoutList.CustomLayout[iNum].CustomLayoutNode[k].uiRight;
//				uiBottom = m_CustomLayoutList.CustomLayout[iNum].CustomLayoutNode[k].uiBottom;
//
//
//				for(j = uiTop;j < uiBottom;j++)
//				{
//
//					for(i = uiLeft; i < uiRight; i++)
//					{
//
//						pLayout->iOutIndex[j * pLayout->uiWidBoundLen + i] = k;
//					}
//				}
//			}
//		}
//
//		m_MapLoyout[wkey] = pLayout;
//
//		iNum++;
//		wkey++;
//	}
//}

void CCustomLayoutData::__InitDefaultMode()
{
	PLAYOUT_PARAM pLayout = NULL;
	WORD wKey = 0;
	int i = 0;
	int j = 0;
	int iBoundLen = 0;
	int iWndNum = 0;

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	//0
	wKey = PREVIEW_ARRANGE_TYPE_1_1_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0;i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 1;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	//0 1
	//2 3
	wKey = PREVIEW_ARRANGE_TYPE_2_2_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0;i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 4;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	// 0 1 2
	// 3 4 5
	// 6 7 8
	wKey = PREVIEW_ARRANGE_TYPE_3_3_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0;i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 9;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0 0 1
	// 0 0 2
	// 3 4 5
	wKey = PREVIEW_ARRANGE_TYPE_3_3_B;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[4] = 0;

		j = 1;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 6;

		m_MapLoyout[wKey] = pLayout;
	}


	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	// 0  1  2  3
	// 4  5  6  7
	// 8  9 10 11
	//12 13 14 15 
	wKey = PREVIEW_ARRANGE_TYPE_4_4_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0;i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 16;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  1  2 
	// 0  0  3  4
	// 5  6  7  8
	// 9 10 11 12
	wKey = PREVIEW_ARRANGE_TYPE_4_4_B;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[5] = 0;

		j = 1;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 13;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  1  1
	// 0  0  1  1
	// 2  3  4  5
	// 6  7  8  9
	wKey = PREVIEW_ARRANGE_TYPE_4_4_C;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 1;
		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 1;
		pLayout->iOutIndex[7] = 1;

		j= 2;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}
		pLayout->iAreas = 10;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  1
	// 0  0  0  2
	// 0  0  0  3
	// 4  5  6  7
	wKey = PREVIEW_ARRANGE_TYPE_4_4_D;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;

		j= 1;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 )
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 8;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  1  2  3
	// 4  5  5  6
	// 7  5  5  8
	// 9 10 11 12
	wKey = PREVIEW_ARRANGE_TYPE_4_4_E;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[5] = 5;
		pLayout->iOutIndex[6] = 5;
		pLayout->iOutIndex[9] = 5;
		pLayout->iOutIndex[10] = 5;

		j = 0;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 5 )
			{
				pLayout->iOutIndex[i] = j;
				j++;

				if(j == 5)
				{
					j++;
				}
			}
		}
		pLayout->iAreas = 13;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	// 0  1  2  3  4
	// 5  6  7  8  9
	//10 11 12 13 14
	//15 16 17 18 19
	//20 21 22 23 24
	wKey = PREVIEW_ARRANGE_TYPE_5_5_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0;i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 25;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  1  2  3
	// 0  0  4  5  6
	// 7  8  9 10 11
	//12 13 14 15 16
	//17 18 19 20 21
	wKey = PREVIEW_ARRANGE_TYPE_5_5_B;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;

		j= 1;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 )
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 22;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0  0  0  1  2
	// 0  0  0  3  4
	// 0  0  0  5  6
	// 7  8  9 10 11
	//12 13 14 15 16
	wKey = PREVIEW_ARRANGE_TYPE_5_5_C;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;
		pLayout->iOutIndex[12] = 0;

		j= 1;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 )
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 17;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  1  1
	// 0  0  0  1  1
	// 0  0  0  2  3
	// 4  5  6  7  8 
	// 9 10 11 12 13
	wKey = PREVIEW_ARRANGE_TYPE_5_5_D;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 1;
		pLayout->iOutIndex[9] = 1;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;
		pLayout->iOutIndex[12] = 0;

		j= 2;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 14;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  1
	// 0  0  0  0  2
	// 0  0  0  0  3
	// 0  0  0  0  4 
	// 5  6  7  8  9
	wKey = PREVIEW_ARRANGE_TYPE_5_5_E;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;

		j= 1;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 10;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  1  1  2
	// 0  0  1  1  3
	// 4  5  6  7  8
	// 9 10 11 12 13
	//14 15 16 17 18
	wKey = PREVIEW_ARRANGE_TYPE_5_5_F;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 1;
		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 1;
		pLayout->iOutIndex[8] = 1;

		j= 2;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 19;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  1  2  3  4
	// 5  6  6  6  7
	// 8  6  6  6  9
	//10  6  6  6 11
	//12 13 14 15 16
	wKey = PREVIEW_ARRANGE_TYPE_5_5_G;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[6] = 6;
		pLayout->iOutIndex[7] = 6;
		pLayout->iOutIndex[8] = 6;
		pLayout->iOutIndex[11] = 6;
		pLayout->iOutIndex[12] = 6;
		pLayout->iOutIndex[13] = 6;
		pLayout->iOutIndex[16] = 6;
		pLayout->iOutIndex[17] = 6;
		pLayout->iOutIndex[18] = 6;

		j = 0;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 6 )
			{
				pLayout->iOutIndex[i] = j;
				j++;

				if(j == 6)
				{
					j++;
				}
			}
		}

		pLayout->iAreas = 17;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  1  1  2
	// 0  0  1  1  3
	// 4  4  5  5  6
	// 4  4  5  5  7
	// 8  9 10 11 12
	wKey = PREVIEW_ARRANGE_TYPE_5_5_H;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 1;
		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 1;
		pLayout->iOutIndex[8] = 1;
		pLayout->iOutIndex[10] = 4;
		pLayout->iOutIndex[11] = 4;
		pLayout->iOutIndex[12] = 5;
		pLayout->iOutIndex[13] = 5;
		pLayout->iOutIndex[15] = 4;
		pLayout->iOutIndex[16] = 4;
		pLayout->iOutIndex[17] = 5;
		pLayout->iOutIndex[18] = 5;

		j = 2;
		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1\
				&& pLayout->iOutIndex[i] != 4 && pLayout->iOutIndex[i] != 5 )
			{
				if(j == 4 || j == 5)
				{
					j++;
				}
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 13;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	// 0  1  2  3  4  5
	// 6  7  8  9 10 11 
	//12 13 14 15 16 17
	//18 19 20 21 22 23
	//24 25 26 27 28 29
	//30 31 32 33 34 35
	wKey = PREVIEW_ARRANGE_TYPE_6_6_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 36;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  1  2  3
	// 0  0  0  4  5  6
	// 0  0  0  7  8  9
	//10 11 12 13 14 15
	//16 17 18 19 20 21
	//22 23 24 25 26 27
	wKey = PREVIEW_ARRANGE_TYPE_6_6_B;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;
		pLayout->iOutIndex[14] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 28;

		m_MapLoyout[wKey] = pLayout;
	}

	wKey = PREVIEW_ARRANGE_TYPE_6_6_C;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	// 0  0  0  1  1  1
	// 0  0  0  1  1  1
	// 0  0  0  1  1  1
	// 2  3  4  5  6  7
	// 8  9 10 11 12 13
	//14 15 16 17 18 19
	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;
		pLayout->iOutIndex[14] = 0;

		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;
		pLayout->iOutIndex[9] = 1;
		pLayout->iOutIndex[10] = 1;
		pLayout->iOutIndex[11] = 1;
		pLayout->iOutIndex[15] = 1;
		pLayout->iOutIndex[16] = 1;
		pLayout->iOutIndex[17] = 1;

		j = 2;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 20;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  1  2
	// 0  0  0  0  3  4
	// 0  0  0  0  5  6
	// 0  0  0  0  7  8
	// 9 10 11 12 13 14
	//15 16 17 18 19 20
	wKey = PREVIEW_ARRANGE_TYPE_6_6_D;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;
		pLayout->iOutIndex[20] = 0;
		pLayout->iOutIndex[21] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 21;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  1  1
	// 0  0  0  0  1  1
	// 0  0  0  0  2  3
	// 0  0  0  0  4  5
	// 6  7  8  9 10 11
	//12 13 14 15 16 17
	wKey = PREVIEW_ARRANGE_TYPE_6_6_E;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;
		pLayout->iOutIndex[20] = 0;
		pLayout->iOutIndex[21] = 0;

		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;
		pLayout->iOutIndex[10] = 1;
		pLayout->iOutIndex[11] = 1;

		j = 2;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 18;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  1  1
	// 0  0  0  0  1  1
	// 0  0  0  0  2  2
	// 0  0  0  0  2  2
	// 3  4  5  6  7  8
	// 9 10 11 12 13 14
	wKey = PREVIEW_ARRANGE_TYPE_6_6_F;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;
		pLayout->iOutIndex[20] = 0;
		pLayout->iOutIndex[21] = 0;

		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;
		pLayout->iOutIndex[10] = 1;
		pLayout->iOutIndex[11] = 1;

		pLayout->iOutIndex[16] = 2;
		pLayout->iOutIndex[17] = 2;
		pLayout->iOutIndex[22] = 2;
		pLayout->iOutIndex[23] = 2;

		j = 3;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1\
				&& pLayout->iOutIndex[i] != 2)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 15;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  0  1
	// 0  0  0  0  0  2
	// 0  0  0  0  0  3
	// 0  0  0  0  0  4
	// 0  0  0  0  0  5
	// 6  7  8  9 10 11
	wKey = PREVIEW_ARRANGE_TYPE_6_6_G;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[6] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;
		pLayout->iOutIndex[20] = 0;
		pLayout->iOutIndex[21] = 0;
		pLayout->iOutIndex[22] = 0;
		pLayout->iOutIndex[24] = 0;
		pLayout->iOutIndex[25] = 0;
		pLayout->iOutIndex[26] = 0;
		pLayout->iOutIndex[27] = 0;
		pLayout->iOutIndex[28] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas =12;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	// 0  1  2  3  4  5  6
	// 7  8  9 10 11 12 13
	//14 15 16 17 18 19 20 
	//21 22 23 24 25 26 27 
	//28 29 30 31 32 33 34
	//35 36 37 38 39 40 41
	//42 43 44 45 46 47 48
	wKey = PREVIEW_ARRANGE_TYPE_7_7_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}


	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;


		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 49;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  1  2  3  4
	// 0  0  0  5  6  7  8
	// 0  0  0  9 10 11 12
	//13 14 15 16 17 18 19 
	//20 21 22 23 24 25 26 
	//27 28 29 30 31 32 33 
	//34 35 36 37 38 39 40
	wKey = PREVIEW_ARRANGE_TYPE_7_7_B;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}


	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[16] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 41;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0  0  0  1  1  1  2
	// 0  0  0  1  1  1  3
	// 0  0  0  1  1  1  4
	// 5  6  7  8  9 10 11 
	//12 13 14 15 16 17 18
	//19 20 21 22 23 24 25 
	//26 27 28 29 30 31 32
	wKey = PREVIEW_ARRANGE_TYPE_7_7_C;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}


	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[16] = 0;

		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;
		pLayout->iOutIndex[10] = 1;
		pLayout->iOutIndex[11] = 1;
		pLayout->iOutIndex[12] = 1;
		pLayout->iOutIndex[17] = 1;
		pLayout->iOutIndex[18] = 1;
		pLayout->iOutIndex[19] = 1;

		j = 2;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 33;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  1  2  3
	// 0  0  0  0  4  5  6
	// 0  0  0  0  7  8  9
	// 0  0  0  0 10 11 12 
	//13 14 15 16 17 18 19
	//20 21 22 23 24 25 26
	//27 28 29 30 31 32 33
	wKey = PREVIEW_ARRANGE_TYPE_7_7_D;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}


	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[21] = 0;
		pLayout->iOutIndex[22] = 0;
		pLayout->iOutIndex[23] = 0;
		pLayout->iOutIndex[24] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 34;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  1  1  1
	// 0  0  0  0  1  1  1
	// 0  0  0  0  1  1  1
	// 0  0  0  0  2  3  4 
	// 5  6  7  8  9 10 11 
	//12 13 14 15 16 17 18
	//19 20 21 22 23 24 25
	wKey = PREVIEW_ARRANGE_TYPE_7_7_E;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[21] = 0;
		pLayout->iOutIndex[22] = 0;
		pLayout->iOutIndex[23] = 0;
		pLayout->iOutIndex[24] = 0;

		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;
		pLayout->iOutIndex[6] = 1;
		pLayout->iOutIndex[11] = 1;
		pLayout->iOutIndex[12] = 1;
		pLayout->iOutIndex[13] = 1;
		pLayout->iOutIndex[18] = 1;
		pLayout->iOutIndex[19] = 1;
		pLayout->iOutIndex[20] = 1;

		j = 2;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 26;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0  0  0  0  0  1  2  
	// 0  0  0  0  0  3  4
	// 0  0  0  0  0  5  6
	// 0  0  0  0  0  7  8
	// 0  0  0  0  0  9 10
	//11 12 13 14 15 16 17 
	//18 19 20 21 22 23 24
	wKey = PREVIEW_ARRANGE_TYPE_7_7_F;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[7] = 0;
		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;
		pLayout->iOutIndex[14] = 0;
		pLayout->iOutIndex[15] = 0;
		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[21] = 0;
		pLayout->iOutIndex[22] = 0;
		pLayout->iOutIndex[23] = 0;
		pLayout->iOutIndex[24] = 0;
		pLayout->iOutIndex[25] = 0;
		pLayout->iOutIndex[28] = 0;
		pLayout->iOutIndex[29] = 0;
		pLayout->iOutIndex[30] = 0;
		pLayout->iOutIndex[31] = 0;
		pLayout->iOutIndex[32] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 25;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	// 0  1  2  3  4  5  6  7
	// 8  9 10 11 12 13 14 15
	//16 17 18 19 20 21 22 23
	//24 25 26 27 28 29 30 31
	//32 33 34 35 36 37 38 39
	//40 41 42 43 44 45 46 47
	//48 49 50 51 52 53 54 55
	//56 57 58 59 60 61 62 63
	wKey = PREVIEW_ARRANGE_TYPE_8_8_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;		
		}

		pLayout->iAreas = 64;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0  0  0  0  1  2  3  4 
	// 0  0  0  0  5  6  7  8
	// 0  0  0  0  9 10 11 12
	// 0  0  0  0 13 14 15 16
	//17 18 19 20 21 22 23 24
	//25 26 27 28 29 30 31 32
	//33 34 35 36 37 38 39 40
	//41 42 43 44 45 46 47 48
	wKey = PREVIEW_ARRANGE_TYPE_8_8_B;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;

		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;


		pLayout->iOutIndex[24] = 0;
		pLayout->iOutIndex[25] = 0;
		pLayout->iOutIndex[26] = 0;
		pLayout->iOutIndex[27] = 0;	

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 49;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0  0  0  0  1  1  1  1  
	// 0  0  0  0  1  1  1  1
	// 0  0  0  0  1  1  1  1
	// 0  0  0  0  1  1  1  1
	// 2  3  4  5  6  7  8  9
	//10 11 12 13 14 15 16 17
	//18 19 20 21 22 23 24 25
	//26 27 28 29 30 31 32 33
	wKey = PREVIEW_ARRANGE_TYPE_8_8_C;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;

		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;


		pLayout->iOutIndex[24] = 0;
		pLayout->iOutIndex[25] = 0;
		pLayout->iOutIndex[26] = 0;
		pLayout->iOutIndex[27] = 0;	

		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;
		pLayout->iOutIndex[6] = 1;
		pLayout->iOutIndex[7] = 1;

		pLayout->iOutIndex[12] = 1;
		pLayout->iOutIndex[13] = 1;
		pLayout->iOutIndex[14] = 1;
		pLayout->iOutIndex[15] = 1;

		pLayout->iOutIndex[20] = 1;
		pLayout->iOutIndex[21] = 1;
		pLayout->iOutIndex[22] = 1;
		pLayout->iOutIndex[23] = 1;


		pLayout->iOutIndex[28] = 1;
		pLayout->iOutIndex[29] = 1;
		pLayout->iOutIndex[30] = 1;
		pLayout->iOutIndex[31] = 1;	

		j = 2;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 && pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 34;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0  0  0  0  0  1  2  3
	// 0  0  0  0  0  4  5  6
	// 0  0  0  0  0  7  8  9
	// 0  0  0  0  0 10 11 12
	// 0  0  0  0  0 13 14 15
	//16 17 18 19 20 21 22 23
	//24 25 26 27 28 29 30 31
	//32 33 34 35 36 37 38 39
	wKey = PREVIEW_ARRANGE_TYPE_8_8_D;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[4] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;
		pLayout->iOutIndex[12] = 0;

		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;
		pLayout->iOutIndex[20] = 0;

		pLayout->iOutIndex[24] = 0;
		pLayout->iOutIndex[25] = 0;
		pLayout->iOutIndex[26] = 0;
		pLayout->iOutIndex[27] = 0;	
		pLayout->iOutIndex[28] = 0;	

		pLayout->iOutIndex[32] = 0;
		pLayout->iOutIndex[33] = 0;
		pLayout->iOutIndex[34] = 0;
		pLayout->iOutIndex[35] = 0;	
		pLayout->iOutIndex[36] = 0;	

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 40;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0  0  0  0  0  1  1  1
	// 0  0  0  0  0  1  1  1
	// 0  0  0  0  0  1  1  1
	// 0  0  0  0  0  2  3  4
	// 0  0  0  0  0  5  6  7
	// 8  9 10 11 12 13 14 15
	//16 17 18 19 20 21 22 23
	//24 25 26 27 28 29 30 31
	wKey = PREVIEW_ARRANGE_TYPE_8_8_E;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[4] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;
		pLayout->iOutIndex[12] = 0;

		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;
		pLayout->iOutIndex[20] = 0;

		pLayout->iOutIndex[24] = 0;
		pLayout->iOutIndex[25] = 0;
		pLayout->iOutIndex[26] = 0;
		pLayout->iOutIndex[27] = 0;	
		pLayout->iOutIndex[28] = 0;	

		pLayout->iOutIndex[32] = 0;
		pLayout->iOutIndex[33] = 0;
		pLayout->iOutIndex[34] = 0;
		pLayout->iOutIndex[35] = 0;	
		pLayout->iOutIndex[36] = 0;	

		pLayout->iOutIndex[5] = 1;
		pLayout->iOutIndex[6] = 1;
		pLayout->iOutIndex[7] = 1;

		pLayout->iOutIndex[13] = 1;
		pLayout->iOutIndex[14] = 1;
		pLayout->iOutIndex[15] = 1;

		pLayout->iOutIndex[21] = 1;
		pLayout->iOutIndex[22] = 1;
		pLayout->iOutIndex[23] = 1;

		j = 2;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0 \
				&& pLayout->iOutIndex[i] != 1)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 32;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  0  0  0  0  1  2
	// 0  0  0  0  0  0  3  4
	// 0  0  0  0  0  0  5  6
	// 0  0  0  0  0  0  7  8
	// 0  0  0  0  0  0  9 10
	// 0  0  0  0  0  0 11 12
	//13 14 15 16 17 18 19 20
	//21 22 23 24 25 26 27 28
	wKey = PREVIEW_ARRANGE_TYPE_8_8_F;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;
		pLayout->iOutIndex[3] = 0;
		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[5] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;
		pLayout->iOutIndex[11] = 0;
		pLayout->iOutIndex[12] = 0;
		pLayout->iOutIndex[13] = 0;

		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;
		pLayout->iOutIndex[19] = 0;
		pLayout->iOutIndex[20] = 0;
		pLayout->iOutIndex[21] = 0;

		pLayout->iOutIndex[24] = 0;
		pLayout->iOutIndex[25] = 0;
		pLayout->iOutIndex[26] = 0;
		pLayout->iOutIndex[27] = 0;	
		pLayout->iOutIndex[28] = 0;	
		pLayout->iOutIndex[29] = 0;	

		pLayout->iOutIndex[32] = 0;
		pLayout->iOutIndex[33] = 0;
		pLayout->iOutIndex[34] = 0;
		pLayout->iOutIndex[35] = 0;	
		pLayout->iOutIndex[36] = 0;	
		pLayout->iOutIndex[37] = 0;	

		pLayout->iOutIndex[40] = 0;
		pLayout->iOutIndex[41] = 0;
		pLayout->iOutIndex[42] = 0;
		pLayout->iOutIndex[43] = 0;	
		pLayout->iOutIndex[44] = 0;	
		pLayout->iOutIndex[45] = 0;	

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] != 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = 29;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	// 0  1  2  3  4  5  6  7  8
	// 9 10 11 12 13 14 15 16 17
	//18 19 20 21 22 23 24 25 26
	//27 28 29 30 31 32 33 34 35
	//36 37 38 39 40 41 42 43 44
	//45 46 47 48 49 50 51 52 53
	//54 55 56 57 58 59 60 61 62
	//63 64 65 66 67 68 69 70 71
	//72 73 74 75 76 77 78 79 80
	wKey = PREVIEW_ARRANGE_TYPE_9_9_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 81;

		m_MapLoyout[wKey] = pLayout;
	}


	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	wKey = PREVIEW_ARRANGE_TYPE_10_10_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 100;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	wKey = PREVIEW_ARRANGE_TYPE_11_11_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 121;

		m_MapLoyout[wKey] = pLayout;
	}


	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	wKey = PREVIEW_ARRANGE_TYPE_12_12_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 144;

		m_MapLoyout[wKey] = pLayout;
	}


	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	wKey = PREVIEW_ARRANGE_TYPE_13_13_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 169;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	wKey = PREVIEW_ARRANGE_TYPE_14_14_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 196;

		m_MapLoyout[wKey] = pLayout;
	}
	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	wKey = PREVIEW_ARRANGE_TYPE_15_15_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 225;

		m_MapLoyout[wKey] = pLayout;
	}

	iBoundLen++;
	iWndNum = iBoundLen * iBoundLen;

	wKey = PREVIEW_ARRANGE_TYPE_16_16_A;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{
		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = iBoundLen;
		pLayout->uiHeitBoundLen = iBoundLen;
		pLayout->bCustom = FALSE;

		for(i = 0; i < iWndNum;i++)
		{
			pLayout->iOutIndex[i] = i;
		}

		pLayout->iAreas = 256;

		m_MapLoyout[wKey] = pLayout;
	}

	// 添加宽屏模板begin [11/27/2009 GuoLinlin]
#ifdef FUN_WIDSCREEN
	//0 0 0 1
	//0 0 0 2
	//0 0 0 3
	wKey = PREVIEW_ARRANGE_TYPE_3_4_A_EX;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{

		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = 4;
		pLayout->uiHeitBoundLen = 3;

		iWndNum = pLayout->uiWidBoundLen * pLayout->uiHeitBoundLen;	
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;

		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[5] = 0;
		pLayout->iOutIndex[6] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;	
		pLayout->iOutIndex[10] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i]< 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = j;

		m_MapLoyout[wKey] = pLayout;
	}

	wKey = PREVIEW_ARRANGE_TYPE_3_4_B_EX;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{

		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = 4;
		pLayout->uiHeitBoundLen = 3;

		iWndNum = pLayout->uiWidBoundLen * pLayout->uiHeitBoundLen;	
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;

		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[5] = 0;

		pLayout->iOutIndex[2] = 1;
		pLayout->iOutIndex[3] = 1;

		pLayout->iOutIndex[6] = 1;
		pLayout->iOutIndex[7] = 1;

		j = 2;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] < 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = j;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0 0 1 2
	// 0 0 3 4
	// 5 6 7 8
	wKey = PREVIEW_ARRANGE_TYPE_3_4_C_EX;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{

		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = 4;
		pLayout->uiHeitBoundLen = 3;

		iWndNum = pLayout->uiWidBoundLen * pLayout->uiHeitBoundLen;	
		pLayout->bCustom = FALSE;

		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;

		pLayout->iOutIndex[4] = 0;
		pLayout->iOutIndex[5] = 0;

		j = 1;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] < 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = j;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0 1 2 3
	// 4 5 6 7
	// 8 9 10 11
	wKey = PREVIEW_ARRANGE_TYPE_3_4_D_EX;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{

		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = 4;
		pLayout->uiHeitBoundLen = 3;

		iWndNum = pLayout->uiWidBoundLen * pLayout->uiHeitBoundLen;	
		pLayout->bCustom = FALSE;

		j = 0;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] < 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = j;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0 0 0 1 1 1 4 4
	// 0 0 0 1 1 1 4 4
	// 0 0 0 1 1 1 5 5
	// 2 2 2 3 3 3 5 5
	// 2 2 2 3 3 3 6 6
	// 2 2 2 3 3 3 6 6
	wKey = PREVIEW_ARRANGE_TYPE_6_8_A_EX;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{

		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = 8;
		pLayout->uiHeitBoundLen = 6;

		iWndNum = pLayout->uiWidBoundLen * pLayout->uiHeitBoundLen;	
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;

		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;

		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;

		pLayout->iOutIndex[11] = 1;
		pLayout->iOutIndex[12] = 1;
		pLayout->iOutIndex[13] = 1;

		pLayout->iOutIndex[19] = 1;
		pLayout->iOutIndex[20] = 1;
		pLayout->iOutIndex[21] = 1;

		pLayout->iOutIndex[24] = 2;
		pLayout->iOutIndex[25] = 2;
		pLayout->iOutIndex[26] = 2;

		pLayout->iOutIndex[32] = 2;
		pLayout->iOutIndex[33] = 2;
		pLayout->iOutIndex[34] = 2;

		pLayout->iOutIndex[40] = 2;
		pLayout->iOutIndex[41] = 2;
		pLayout->iOutIndex[42] = 2;

		pLayout->iOutIndex[27] = 3;
		pLayout->iOutIndex[28] = 3;
		pLayout->iOutIndex[29] = 3;

		pLayout->iOutIndex[35] = 3;
		pLayout->iOutIndex[36] = 3;
		pLayout->iOutIndex[37] = 3;

		pLayout->iOutIndex[43] = 3;
		pLayout->iOutIndex[44] = 3;
		pLayout->iOutIndex[45] = 3;

		pLayout->iOutIndex[6] = 4;
		pLayout->iOutIndex[7] = 4;

		pLayout->iOutIndex[14] = 4;
		pLayout->iOutIndex[15] = 4;

		pLayout->iOutIndex[22] = 5;
		pLayout->iOutIndex[23] = 5;

		pLayout->iOutIndex[30] = 5;
		pLayout->iOutIndex[31] = 5;

		pLayout->iOutIndex[38] = 6;
		pLayout->iOutIndex[39] = 6;

		pLayout->iOutIndex[46] = 6;
		pLayout->iOutIndex[47] = 6;

		pLayout->iAreas = 7;

		m_MapLoyout[wKey] = pLayout;
	}


	// 0 0 0 1 1 1 4  5
	// 0 0 0 1 1 1 6  7
	// 0 0 0 1 1 1 8  9
	// 2 2 2 3 3 3 10 11
	// 2 2 2 3 3 3 12 13
	// 2 2 2 3 3 3 14 15
	wKey = PREVIEW_ARRANGE_TYPE_6_8_B_EX;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{

		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = 8;
		pLayout->uiHeitBoundLen = 6;

		iWndNum = pLayout->uiWidBoundLen * pLayout->uiHeitBoundLen;	
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;
		pLayout->iOutIndex[2] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;
		pLayout->iOutIndex[10] = 0;

		pLayout->iOutIndex[16] = 0;
		pLayout->iOutIndex[17] = 0;
		pLayout->iOutIndex[18] = 0;

		pLayout->iOutIndex[3] = 1;
		pLayout->iOutIndex[4] = 1;
		pLayout->iOutIndex[5] = 1;

		pLayout->iOutIndex[11] = 1;
		pLayout->iOutIndex[12] = 1;
		pLayout->iOutIndex[13] = 1;

		pLayout->iOutIndex[19] = 1;
		pLayout->iOutIndex[20] = 1;
		pLayout->iOutIndex[21] = 1;

		pLayout->iOutIndex[24] = 2;
		pLayout->iOutIndex[25] = 2;
		pLayout->iOutIndex[26] = 2;

		pLayout->iOutIndex[32] = 2;
		pLayout->iOutIndex[33] = 2;
		pLayout->iOutIndex[34] = 2;

		pLayout->iOutIndex[40] = 2;
		pLayout->iOutIndex[41] = 2;
		pLayout->iOutIndex[42] = 2;

		pLayout->iOutIndex[27] = 3;
		pLayout->iOutIndex[28] = 3;
		pLayout->iOutIndex[29] = 3;

		pLayout->iOutIndex[35] = 3;
		pLayout->iOutIndex[36] = 3;
		pLayout->iOutIndex[37] = 3;

		pLayout->iOutIndex[43] = 3;
		pLayout->iOutIndex[44] = 3;
		pLayout->iOutIndex[45] = 3;

		j = 4;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] < 0)
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = j;

		m_MapLoyout[wKey] = pLayout;
	}

	// 0  0  1  1  2  2  3  3
	// 0  0  1  1  2  2  3  3
	// 4  4  5  5  6  6  7  7
	// 4  4  5  5  6  6  7  7
	// 8  9 10 11 12 13 14 15
	//16 17 18 19 10 21 22 23
	wKey = PREVIEW_ARRANGE_TYPE_6_8_C_EX;

	if(m_MapLoyout.Lookup(wKey,(void * &)pLayout))
	{

		ASSERT(pLayout);
		delete pLayout;
		pLayout = NULL;
	}

	pLayout = new LAYOUT_PARAM;

	if(pLayout)
	{
		pLayout->uiWidBoundLen = 8;
		pLayout->uiHeitBoundLen = 6;

		iWndNum = pLayout->uiWidBoundLen * pLayout->uiHeitBoundLen;	
		pLayout->bCustom = FALSE;


		pLayout->iOutIndex[0] = 0;
		pLayout->iOutIndex[1] = 0;

		pLayout->iOutIndex[8] = 0;
		pLayout->iOutIndex[9] = 0;


		pLayout->iOutIndex[2] = 1;
		pLayout->iOutIndex[3] = 1;

		pLayout->iOutIndex[10] = 1;
		pLayout->iOutIndex[11] = 1;


		pLayout->iOutIndex[4] = 2;
		pLayout->iOutIndex[5] = 2;

		pLayout->iOutIndex[12] = 2;
		pLayout->iOutIndex[13] = 2;


		pLayout->iOutIndex[6] = 3;
		pLayout->iOutIndex[7] = 3;

		pLayout->iOutIndex[14] = 3;
		pLayout->iOutIndex[15] = 3;

		pLayout->iOutIndex[16] = 4;
		pLayout->iOutIndex[17] = 4;

		pLayout->iOutIndex[24] = 4;
		pLayout->iOutIndex[25] = 4;


		pLayout->iOutIndex[18] = 5;
		pLayout->iOutIndex[19] = 5;

		pLayout->iOutIndex[26] = 5;
		pLayout->iOutIndex[27] = 5;


		pLayout->iOutIndex[20] = 6;
		pLayout->iOutIndex[21] = 6;

		pLayout->iOutIndex[28] = 6;
		pLayout->iOutIndex[29] = 6;


		pLayout->iOutIndex[22] = 7;
		pLayout->iOutIndex[23] = 7;

		pLayout->iOutIndex[30] = 7;
		pLayout->iOutIndex[31] = 7;

		j = 8;

		for(i = 0; i < iWndNum;i++)
		{
			if(pLayout->iOutIndex[i] < 0 )
			{
				pLayout->iOutIndex[i] = j;
				j++;
			}
		}

		pLayout->iAreas = j;

		m_MapLoyout[wKey] = pLayout;
	}
#endif
	// 添加宽屏模板end [11/27/2009 GuoLinlin]
}

void CCustomLayoutData::Fun_UnInit(void)
{
	if(m_MapLoyout.GetCount() <= 0)
	{
		return;
	}

	POSITION pos = m_MapLoyout.GetStartPosition();
	WORD wKey = 0;
	PLAYOUT_PARAM pLoyoutInfo = NULL; 

	while(pos != NULL)
	{
		m_MapLoyout.GetNextAssoc(pos,wKey,(void * &)pLoyoutInfo);

		ASSERT(pLoyoutInfo);

		if(pLoyoutInfo)
		{
			delete pLoyoutInfo;
			pLoyoutInfo = NULL;
		}
	}

	m_MapLoyout.RemoveAll();
}

BOOL CCustomLayoutData::GetLayoutData(WORD wLayoutMode,PLAYOUT_PARAM pLayoutParam)
{
	PLAYOUT_PARAM pLayoutTemp = NULL;

	ASSERT(pLayoutParam); 
	if(m_MapLoyout.Lookup(wLayoutMode,(void * &)pLayoutTemp))
	{

		*pLayoutParam = *pLayoutTemp;
		//memcpy(pLayoutParam,pLayoutTemp,sizeof(LAYOUT_PARAM));

		return TRUE;
	}

	return FALSE;
}