/*****************************************************
Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.  
FileName	:	AreaDivision.h
Description	:  	 
Author		:   YanChun
Date		:   2007

Modification History:      
    <version > <time> <author> <desc>
*****************************************************/


#if !defined(AFX_AREADIVISION_H__6A218D62_167F_49E7_A262_955AD59E38BC__INCLUDED_)
#define AFX_AREADIVISION_H__6A218D62_167F_49E7_A262_955AD59E38BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//画面分割模式宏定义

#define	PREVIEW_ARRANGE_TYPE_1_1_A	0x00
#define	PREVIEW_ARRANGE_TYPE_2_2_A	0x01
#define	PREVIEW_ARRANGE_TYPE_3_3_A	0x02
#define	PREVIEW_ARRANGE_TYPE_3_3_B	0x03

#define	PREVIEW_ARRANGE_TYPE_4_4_A	0x04
#define	PREVIEW_ARRANGE_TYPE_4_4_B	0x05
#define	PREVIEW_ARRANGE_TYPE_4_4_C	0x06
#define	PREVIEW_ARRANGE_TYPE_4_4_D	0x07
#define	PREVIEW_ARRANGE_TYPE_4_4_E	0x08

#define	PREVIEW_ARRANGE_TYPE_5_5_A	0x09
#define	PREVIEW_ARRANGE_TYPE_5_5_B	0x0a
#define	PREVIEW_ARRANGE_TYPE_5_5_C	0x0b
#define	PREVIEW_ARRANGE_TYPE_5_5_D	0x0c
#define	PREVIEW_ARRANGE_TYPE_5_5_E	0x0d
#define	PREVIEW_ARRANGE_TYPE_5_5_F	0x0e
#define	PREVIEW_ARRANGE_TYPE_5_5_G	0x0f
#define	PREVIEW_ARRANGE_TYPE_5_5_H	0x10

#define PREVIEW_ARRANGE_TYPE_6_6_A	0x11
#define PREVIEW_ARRANGE_TYPE_6_6_B	0x12
#define PREVIEW_ARRANGE_TYPE_6_6_C	0x13
#define PREVIEW_ARRANGE_TYPE_6_6_D	0x14
#define PREVIEW_ARRANGE_TYPE_6_6_E	0x15
#define PREVIEW_ARRANGE_TYPE_6_6_F	0x16
#define PREVIEW_ARRANGE_TYPE_6_6_G	0x17

#define PREVIEW_ARRANGE_TYPE_7_7_A	0x18
#define PREVIEW_ARRANGE_TYPE_7_7_B	0x19
#define PREVIEW_ARRANGE_TYPE_7_7_C	0x1a
#define PREVIEW_ARRANGE_TYPE_7_7_D	0x1b
#define PREVIEW_ARRANGE_TYPE_7_7_E	0x1c
#define PREVIEW_ARRANGE_TYPE_7_7_F	0x1d

#define PREVIEW_ARRANGE_TYPE_8_8_A	0x1e
#define PREVIEW_ARRANGE_TYPE_8_8_B	0x1f
#define PREVIEW_ARRANGE_TYPE_8_8_C	0x20
#define PREVIEW_ARRANGE_TYPE_8_8_D	0x21
#define PREVIEW_ARRANGE_TYPE_8_8_E	0x22
#define PREVIEW_ARRANGE_TYPE_8_8_F	0x23


#define PREVEEW_ARRANGE_MAX_MODES	0x23+1	/*当前支持的模式数量*/

class CAreaDivision  
{
public:
	CAreaDivision();
	virtual ~CAreaDivision();

private:
	int		m_iMode;
	RECT	m_stRect;

	int	*	m_piMode[PREVEEW_ARRANGE_MAX_MODES];
	int		m_iSects[PREVEEW_ARRANGE_MAX_MODES];
public:

	/*************************************************
	  Function:    	SetDivision()
	  Description:	设置画面分割模式及总的区域大小
	  Input:        imode	-	画面分割模式
					stRect	-	总的区域大小
	  Output:      	
	  Return:		TRUE/FALSE
	*************************************************/

	BOOL SetDivision(int iMode, RECT stRect);


	/*************************************************
	  Function:    	GetDivision()
	  Description:	获取特定分割模式下各子区域的位置及大小
	  Input:        iIndex	-	子区域序号,从0开始
	  Output:      	pstRect	-	子区域位置及大小
	  Return:		TRUE/FALSE
	*************************************************/

	BOOL GetDivision(int iIndex, PRECT pstRect);

};

#endif // !defined(AFX_AREADIVISION_H__6A218D62_167F_49E7_A262_955AD59E38BC__INCLUDED_)
