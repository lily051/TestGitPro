// AreaDivision.cpp: implementation of the CAreaDivision class.
//
//////////////////////////////////////////////////////////////////////
//lint -library
#include "stdafx.h"
#include "AreaDivision.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//	PREVIEW_ARRANGE_TYPE_1_1_A:(areas:1)
int	g_iArrangeDraw_1_1_A[1]	
=	{0};

//	PREVIEW_ARRANGE_TYPE_2_2_A:(areas:4)
int	g_iArrangeDraw_2_2_A[2*2] 
=	{
	0,	1,
	2,	3
	};

//	PREVIEW_ARRANGE_TYPE_3_3_A:(areas:9)	
int	g_iArrangeDraw_3_3_A[3*3]
=	{
	0,	1,	2,
	3,	4,	5,
	6,	7,	8
	};

//	PREVIEW_ARRANGE_TYPE_3_3_B:(areas:6)	
int	g_iArrangeDraw_3_3_B[3*3]
=	{
	0,	0,	1,
	0,	0,	2,
	3,	4,	5
	};

//	PREVIEW_ARRANGE_TYPE_4_4_A:(areas:16)
int	g_iArrangeDraw_4_4_A[4*4]
=	{
	0,	1,	2,	3,
	4,	5,	6,	7,	
	8,	9,	10,	11,
	12,	13,	14,	15
	};

//	PREVIEW_ARRANGE_TYPE_4_4_B:(areas:13)
int	g_iArrangeDraw_4_4_B[4*4]
=	{
	0,	0,	1,	2,
	0,	0,	3,	4,	
	5,	6,	7,	8,
	9,	10,	11,	12
	};

//	PREVIEW_ARRANGE_TYPE_4_4_C:(areas:10)
int	g_iArrangeDraw_4_4_C[4*4]
=	{
	0,	0,	1,	1,
	0,	0,	1,	1,	
	2,	3,	4,	5,
	6,	7,	8,	9
	};

//	PREVIEW_ARRANGE_TYPE_4_4_D:(areas:8)
int	g_iArrangeDraw_4_4_D[4*4]
=	{
	0,	0,	0,	1,
	0,	0,	0,	2,	
	0,	0,	0,	3,
	4,	5,	6,	7
	};

//	PREVIEW_ARRANGE_TYPE_4_4_E:(areas:13)
int	g_iArrangeDraw_4_4_E[4*4]
=	{
	0,	1,	2,	3,
	4,	5,	5,	6,	
	7,	5,	5,	8,
	9,	10,	11,	12
	};

//	PREVIEW_ARRANGE_TYPE_5_5_A:(areas:25)
int	g_iArrangeDraw_5_5_A[5*5]
=	{
	0,	1,	2,	3,  4,
	5,	6,	7,	8,  9,	
	10,	11,	12,	13, 14,
	15,	16,	17,	18, 19,
	20, 21, 22, 23, 24
	};

//	PREVIEW_ARRANGE_TYPE_5_5_B:(areas:22)
int	g_iArrangeDraw_5_5_B[5*5]
=	{
	0,	0,	1,	2,  3,
	0,	0,	4,	5,  6,	
	7,	8,	9,	10, 11,
	12,	13,	14,	15, 16,
	17, 18, 19, 20, 21
	};

//	PREVIEW_ARRANGE_TYPE_5_5_C:(areas:17)
int	g_iArrangeDraw_5_5_C[5*5]
=	{
	0,	0,	0,	1,  2,
	0,	0,	0,	3,  4,	
	0,	0,	0,	5,  6,
	7,	8,	9,	10, 11,
	12, 13, 14, 15, 16
	};

//	PREVIEW_ARRANGE_TYPE_5_5_D:(areas:14)
int	g_iArrangeDraw_5_5_D[5*5]
=	{
	0,	0,	0,	1,  1,
	0,	0,	0,	1,  1,	
	0,	0,	0,	2,  3,
	4,	5,	6,	7,  8,
	9, 10, 11, 12, 13
	};

//	PREVIEW_ARRANGE_TYPE_5_5_E:(areas:10)
int	g_iArrangeDraw_5_5_E[5*5]
=	{
	0,	0,	0,	0,  1,
	0,	0,	0,	0,  2,	
	0,	0,	0,	0,  3,
	0,	0,	0,	0,  4,
	5,  6,  7,  8,  9
	};

//	PREVIEW_ARRANGE_TYPE_5_5_F:(areas:19)
int	g_iArrangeDraw_5_5_F[5*5]
=	{
	0,	0,	1,	1,  2,
	0,	0,	1,	1,  3,	
	4,	5,	6,	7,  8,
	9,	10,	11,	12, 13,
	14, 15, 16, 17, 18
	};

//	PREVIEW_ARRANGE_TYPE_5_5_G:(areas:17)
int	g_iArrangeDraw_5_5_G[5*5]
=	{
	0,	1,	2,	3,  4,
	5,	6,	6,	6,  7,	
	8,	6,	6,	6,  9,
	10,	6,	6,	6,  11,
	12, 13, 14, 15, 16
	};

//	PREVIEW_ARRANGE_TYPE_5_5_H:(areas:13)
int	g_iArrangeDraw_5_5_H[5*5]
=	{
	0,	0,	1,	1,  2,
	0,	0,	1,	1,  3,	
	4,	4,	5,	5,  6,
	4,	4,	5,	5,  7,
	8,  9,  10, 11, 12
	};

//	PREVIEW_ARRANGE_TYPE_6_6_A:(areas:36)
int	g_iArrangeDraw_6_6_A[6*6]
=	{
	0,	1,	2,	3,  4,  5,
	6,	7,	8,	9,  10,	11,
	12,	13,	14,	15, 16, 17,
	18,	19,	20,	21, 22, 23,
	24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35
	};

//	PREVIEW_ARRANGE_TYPE_6_6_B:(areas:28)
int	g_iArrangeDraw_6_6_B[6*6]
=	{
	0,	0,	0,	1,  2,  3,
	0,	0,	0,	4,  5,	6,
	0,	0,	0,	7,  8,  9,
	10,	11,	12,	13, 14, 15,
	16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 26, 27
	};

//	PREVIEW_ARRANGE_TYPE_6_6_C:(areas:20)
int	g_iArrangeDraw_6_6_C[6*6]
=	{
	0,	0,	0,	1,  1,  1,
	0,	0,	0,	1,  1,	1,
	0,	0,	0,	1,  1,  1,
	2,	3,	4,	5,  6,  7,
	8,  9, 10, 11, 12, 13,
	14, 15,16, 17, 18, 19
	};

//	PREVIEW_ARRANGE_TYPE_6_6_D:(areas:21)
int	g_iArrangeDraw_6_6_D[6*6]
=	{
	0,	0,	0,	0,  1,  2,
	0,	0,	0,	0,  3,	4,
	0,	0,	0,	0,  5,  6,
	0,	0,	0,	0,  7,  8,
	9, 10, 11, 12, 13, 14,
	15,16, 17, 18, 19, 20
	};

//	PREVIEW_ARRANGE_TYPE_6_6_E:(areas:18)
int	g_iArrangeDraw_6_6_E[6*6]
=	{
	0,	0,	0,	0,  1,  1,
	0,	0,	0,	0,  1,	1,
	0,	0,	0,	0,  2,  3,
	0,	0,	0,	0,  4,  5,
	6,  7,  8,  9, 10, 11,
	12,13, 14, 15, 16, 17
	};

//	PREVIEW_ARRANGE_TYPE_6_6_F:(areas:15)
int	g_iArrangeDraw_6_6_F[6*6]
=	{
	0,	0,	0,	0,  1,  1,
	0,	0,	0,	0,  1,	1,
	0,	0,	0,	0,  2,  2,
	0,	0,	0,	0,  2,  2,
	3,  4,  5,  6,  7,  8,
	9,  10, 11, 12, 13, 14
	};

//	PREVIEW_ARRANGE_TYPE_6_6_G:(areas:12)
int	g_iArrangeDraw_6_6_G[6*6]
=	{
	0,	0,	0,	0,  0,  1,
	0,	0,	0,	0,  0,	2,
	0,	0,	0,	0,  0,  3,
	0,	0,	0,	0,  0,  4,
	0,  0,  0,  0,  0,  5,
	6,  7,  8,  9, 10, 11
	};

//	PREVIEW_ARRANGE_TYPE_7_7_A:(areas:49)
int	g_iArrangeDraw_7_7_A[7*7]
=	{
	0,	1,	2,	3,  4,  5,  6,
	7,	8,	9,	10, 11,	12, 13,
	14,	15,	16,	17, 18, 19, 20,
	21,	22,	23,	24, 25, 26, 27,
	28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41,
	42, 43, 44, 45, 46, 47, 48 
	};

//	PREVIEW_ARRANGE_TYPE_7_7_B:(areas:41)
int	g_iArrangeDraw_7_7_B[7*7]
=	{
	0,	0,	0,	1,  2,  3,  4,
	0,	0,	0,	5,  6,	7,  8,
	0,	0,	0,	9, 10, 11, 12,
	13,	14,	15,	16,17, 18, 19,
	20, 21, 22, 23, 24,25, 26,
	27, 28, 29, 30, 31,32, 33,
	34, 35, 36, 37, 38,39, 40 
	};

//	PREVIEW_ARRANGE_TYPE_7_7_C:(areas:33)
int	g_iArrangeDraw_7_7_C[7*7]
=	{
	0,	0,	0,	1,  1,  1,  2,
	0,	0,	0,	1,  1,	1,  3,
	0,	0,	0,	1,  1,  1,  4,
	5,	6,	7,	8,  9, 10, 11,
	12, 13,14, 15, 16, 17, 18,
	19, 20,21, 22, 23, 24, 25,
	26, 27,28, 29, 30, 31, 32 
	};

//	PREVIEW_ARRANGE_TYPE_7_7_D:(areas:34)
int	g_iArrangeDraw_7_7_D[7*7]
=	{
	0,	0,	0,	0,  1,  2,  3,
	0,	0,	0,	0,  4,	5,  6,
	0,	0,	0,	0,  7,  8,  9,
	0,	0,	0,	0, 10, 11, 12,
	13, 14,15, 16, 17, 18, 19,
	20, 21,22, 23, 24, 25, 26,
	27, 28,29, 30, 31, 32, 33 
	};

//	PREVIEW_ARRANGE_TYPE_7_7_E:(areas:26)
int	g_iArrangeDraw_7_7_E[7*7]
=	{
	0,	0,	0,	0,  1,  1,  1,
	0,	0,	0,	0,  1,	1,  1,
	0,	0,	0,	0,  1,  1,  1,
	0,	0,	0,	0,  2,  3,  4,
	5,  6,  7,  8,  9, 10, 11,
   12, 13, 14, 15, 16, 17, 18,
   19, 20, 21, 22, 23, 24, 25 
	};

//	PREVIEW_ARRANGE_TYPE_7_7_F:(areas:25)
int	g_iArrangeDraw_7_7_F[7*7]
=	{
	0,	0,	0,	0,  0,  1,  2,
	0,	0,	0,	0,  0,	3,  4,
	0,	0,	0,	0,  0,  5,  6,
	0,	0,	0,	0,  0,  7,  8,
	0,  0,  0,  0,  0,  9, 10,
   11, 12, 13, 14, 15, 16, 17,
   18, 19, 20, 21, 22, 23, 24 
	};

//	PREVIEW_ARRANGE_TYPE_8_8_A:(areas:64)
int	g_iArrangeDraw_8_8_A[8*8]
=	{
	0,	1,	2,	3,  4,  5,  6,  7,
	8,	9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23,
   24, 25, 26, 27, 28, 29, 30, 31,
   32, 33, 34, 35, 36, 37, 38, 39,
   40, 41, 42, 43, 44, 45, 46, 47,
   48, 49, 50, 51, 52, 53, 54, 55, 
   56, 57, 58, 59, 60, 61, 62, 63
	};

//	PREVIEW_ARRANGE_TYPE_8_8_B:(areas:49)
int	g_iArrangeDraw_8_8_B[8*8]
=	{
	0,	0,	0,	0,  1,  2,  3,  4,
	0,	0,  0,  0,  5,  6,  7,  8,
    0,  0,  0,  0,  9, 10, 11, 12,
    0,  0,  0,  0, 13, 14, 15, 16,
   17, 18, 19, 20, 21, 22, 23, 24,
   25, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 40, 
   41, 42, 43, 44, 45, 46, 47, 48
	};

//	PREVIEW_ARRANGE_TYPE_8_8_C:(areas:34)
int	g_iArrangeDraw_8_8_C[8*8]
=	{
	0,	0,	0,	0,  1,  1,  1,  1,
	0,	0,  0,  0,  1,  1,  1,  1,
    0,  0,  0,  0,  1,  1,  1,  1,
    0,  0,  0,  0,  1,  1,  1,  1,
    2,  3,  4,  5,  6,  7,  8,  9,
   10, 11, 12, 13, 14, 15, 16, 17,
   18, 19, 20, 21, 22, 23, 24, 25, 
   26, 27, 28, 29, 30, 31, 32, 33
	};

//	PREVIEW_ARRANGE_TYPE_8_8_D:(areas:40)
int	g_iArrangeDraw_8_8_D[8*8]
=	{
	0,	0,	0,	0,  0,  1,  2,  3,
	0,	0,  0,  0,  0,  4,  5,  6,
    0,  0,  0,  0,  0,  7,  8,  9,
    0,  0,  0,  0,  0, 10, 11, 12,
    0,  0,  0,  0,  0, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23,
   24, 25, 26, 27, 28, 29, 30, 31, 
   32, 33, 34, 35, 36, 37, 38, 39
	};

//	PREVIEW_ARRANGE_TYPE_8_8_E:(areas:32)
int	g_iArrangeDraw_8_8_E[8*8]
=	{
	0,	0,	0,	0,  0,  1,  1,  1,
	0,	0,  0,  0,  0,  1,  1,  1,
    0,  0,  0,  0,  0,  1,  1,  1,
    0,  0,  0,  0,  0,  2,  3,  4,
    0,  0,  0,  0,  0,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23, 
   24, 25, 26, 27, 28, 29, 30, 31
	};

//	PREVIEW_ARRANGE_TYPE_8_8_F:(areas:29)
int	g_iArrangeDraw_8_8_F[8*8]
=	{
	0,	0,	0,	0,  0,  0,  1,  2,
	0,	0,  0,  0,  0,  0,  3,  4,
    0,  0,  0,  0,  0,  0,  5,  6,
    0,  0,  0,  0,  0,  0,  7,  8,
    0,  0,  0,  0,  0,  0,  9, 10,
    0,  0,  0,  0,  0,  0, 11, 12,
   13, 14, 15, 16, 17, 18, 19, 20, 
   21, 22, 23, 24, 25, 26, 27, 28
	};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAreaDivision::CAreaDivision()
{
	m_iMode = -1;
    ZeroMemory(&m_stRect, sizeof(RECT));
	//initialze mode talbe
	

	m_piMode[0]		=	g_iArrangeDraw_1_1_A;	m_iSects[0]   = 1;
	m_piMode[1]		=	g_iArrangeDraw_2_2_A;	m_iSects[1]   = 2;
	m_piMode[2]		=	g_iArrangeDraw_3_3_A;	m_iSects[2]   = 3;
	m_piMode[3]		=	g_iArrangeDraw_3_3_B;	m_iSects[3]   = 3;
	m_piMode[4]		=	g_iArrangeDraw_4_4_A;	m_iSects[4]   = 4;
	m_piMode[5]		=	g_iArrangeDraw_4_4_B;	m_iSects[5]   = 4;
	m_piMode[6]		=	g_iArrangeDraw_4_4_C;	m_iSects[6]   = 4;
	m_piMode[7]		=	g_iArrangeDraw_4_4_D;	m_iSects[7]   = 4;
	m_piMode[8]		=	g_iArrangeDraw_4_4_E;	m_iSects[8]   = 4;
	m_piMode[9]		=	g_iArrangeDraw_5_5_A;	m_iSects[9]   = 5;
	m_piMode[10]	=	g_iArrangeDraw_5_5_B;	m_iSects[10]  = 5;
	m_piMode[11]	=	g_iArrangeDraw_5_5_C;	m_iSects[11]  = 5;
	m_piMode[12]	=	g_iArrangeDraw_5_5_D;	m_iSects[12]  = 5;
	m_piMode[13]	=	g_iArrangeDraw_5_5_E;	m_iSects[13]  = 5;
	m_piMode[14]	=	g_iArrangeDraw_5_5_F;	m_iSects[14]  = 5;
	m_piMode[15]	=	g_iArrangeDraw_5_5_G;	m_iSects[15]  = 5;
	m_piMode[16]	=	g_iArrangeDraw_5_5_H;	m_iSects[16]  = 5;
	m_piMode[17]	=	g_iArrangeDraw_6_6_A;	m_iSects[17]  = 6;
	m_piMode[18]	=	g_iArrangeDraw_6_6_B;	m_iSects[18]  = 6;
	m_piMode[19]	=	g_iArrangeDraw_6_6_C;	m_iSects[19]  = 6;
	m_piMode[20]	=	g_iArrangeDraw_6_6_D;	m_iSects[20]  = 6;
	m_piMode[21]	=	g_iArrangeDraw_6_6_E;	m_iSects[21]  = 6;
	m_piMode[22]	=	g_iArrangeDraw_6_6_F;	m_iSects[22]  = 6;
	m_piMode[23]	=	g_iArrangeDraw_6_6_G;	m_iSects[23]  = 6;
	m_piMode[24]	=	g_iArrangeDraw_7_7_A;	m_iSects[24]  = 7;
	m_piMode[25]	=	g_iArrangeDraw_7_7_B;	m_iSects[25]  = 7;
	m_piMode[26]	=	g_iArrangeDraw_7_7_C;	m_iSects[26]  = 7;
	m_piMode[27]	=	g_iArrangeDraw_7_7_D;	m_iSects[27]  = 7;
	m_piMode[28]	=	g_iArrangeDraw_7_7_E;	m_iSects[28]  = 7;
	m_piMode[29]	=	g_iArrangeDraw_7_7_F;	m_iSects[29]  = 7;
	m_piMode[30]	=	g_iArrangeDraw_8_8_A;	m_iSects[30]  = 8;
	m_piMode[31]	=	g_iArrangeDraw_8_8_B;	m_iSects[31]  = 8;
	m_piMode[32]	=	g_iArrangeDraw_8_8_C;	m_iSects[32]  = 8;
	m_piMode[33]	=	g_iArrangeDraw_8_8_D;	m_iSects[33]  = 8;
	m_piMode[34]	=	g_iArrangeDraw_8_8_E;	m_iSects[34]  = 8;
	m_piMode[35]	=	g_iArrangeDraw_8_8_F;	m_iSects[35]  = 8;
}

CAreaDivision::~CAreaDivision()
{

}

BOOL CAreaDivision::SetDivision(int iMode, RECT stRect)
{
	m_iMode	=	iMode;
	m_stRect=	stRect;
	
	return TRUE;
}

BOOL CAreaDivision::GetDivision(int iIndex, PRECT pRect)
{
	if((m_iMode<0)||(m_iMode>=PREVEEW_ARRANGE_MAX_MODES))
	{
		return FALSE;
	}

	if((iIndex<0)||(iIndex>=m_iSects[m_iMode]*m_iSects[m_iMode]))
	{
		return FALSE;
	}


	int isects;

	isects = m_iSects[m_iMode];
	

	int i,j;
	int istartx,istarty,iendx,iendy;
	int isectw,isecth;
	BOOL bbingo;

	bbingo = FALSE;

	
	for(j=0;j<isects;j++)
	{
		for(i=0;i<isects;i++)
		{
			if(m_piMode[m_iMode][j*isects+i] == iIndex)
			{
				if(!bbingo)
				{
					istartx = i;
					istarty = j;
					bbingo = TRUE;
				}

				iendx = i;
				iendy = j;
			}
		}
	}

	//区域号越界了!!
	if(!bbingo) return FALSE;

	isectw = (m_stRect.right-m_stRect.left)/isects;
	isecth = (m_stRect.bottom-m_stRect.top)/isects;

	pRect->left	= m_stRect.left + isectw*istartx;
	pRect->top	= m_stRect.top + isecth*istarty;


	if(iendx+1==isects)
	{//每一行的最后一个,补齐!
		pRect->right = m_stRect.right;
	}else
	{
		pRect->right = pRect->left+(iendx-istartx+1)*isectw;
	}

	if(iendy+1==isects)
	{//每一列的最后一个,补齐!
		pRect->bottom = m_stRect.bottom;	
	}else
	{
		pRect->bottom = pRect->top+(iendy-istarty+1)*isecth;
	}
	
	//增大间距
	pRect->top = pRect->top +1;
	pRect->left = pRect->left +1;
	pRect->right = pRect->right - 1;
	pRect->bottom = pRect->bottom-1;
	
	
	return TRUE;
}