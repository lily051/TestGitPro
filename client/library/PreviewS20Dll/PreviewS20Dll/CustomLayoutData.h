#pragma once
//#include "CustomLoyoutWnd.h"
#define MAX_CUSTOMLAYOUT_COUNT 8
#define MIN_BOUND_LEN 1
#define MAX_BOUND_LEN 16

#define MAX_LAYOUT_WND_NUM MAX_BOUND_LEN * MAX_BOUND_LEN

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

#define PREVIEW_ARRANGE_TYPE_9_9_A	    0x24
#define PREVIEW_ARRANGE_TYPE_10_10_A	0x25
#define PREVIEW_ARRANGE_TYPE_11_11_A	0x26
#define PREVIEW_ARRANGE_TYPE_12_12_A	0x27
#define PREVIEW_ARRANGE_TYPE_13_13_A	0x28
#define PREVIEW_ARRANGE_TYPE_14_14_A	0x29
#define PREVIEW_ARRANGE_TYPE_15_15_A	0x2a
#define PREVIEW_ARRANGE_TYPE_16_16_A	0x2b

#define PREVIEW_ARRANGE_TYPE_CUSTOM_01 0x80
#define PREVIEW_ARRANGE_TYPE_CUSTOM_02 0x81
#define PREVIEW_ARRANGE_TYPE_CUSTOM_03 0x82
#define PREVIEW_ARRANGE_TYPE_CUSTOM_04 0x83
#define PREVIEW_ARRANGE_TYPE_CUSTOM_05 0x84
#define PREVIEW_ARRANGE_TYPE_CUSTOM_06 0x85
#define PREVIEW_ARRANGE_TYPE_CUSTOM_07 0x86
#define PREVIEW_ARRANGE_TYPE_CUSTOM_08 0x87


//#ifdef FUN_WIDSCREEN  //wide screen

//宽屏下的画面分割模式  wide screen
#define PREVIEW_ARRANGE_TYPE_3_4_A_EX 0x2c//4
#define PREVIEW_ARRANGE_TYPE_3_4_B_EX 0x2d //6
#define PREVIEW_ARRANGE_TYPE_3_4_C_EX 0x30 //9
#define PREVIEW_ARRANGE_TYPE_3_4_D_EX 0x31 //12
#define PREVIEW_ARRANGE_TYPE_6_8_A_EX 0x32 //8
#define PREVIEW_ARRANGE_TYPE_6_8_B_EX 0x33 //16
#define PREVIEW_ARRANGE_TYPE_6_8_C_EX 0x34 //24
#define PREVIEW_ARRANGE_TYPE_6_8_D_EX 0x35 //36
#define PREVIEW_ARRANGE_TYPE_6_8_E_EX 0x36 //48

//#endif
typedef struct tagCustomLayoutNode
{
	unsigned int uiLeft;
	unsigned int uiTop;
	unsigned int uiRight;
	unsigned int uiBottom;

	tagCustomLayoutNode()
	{
		uiLeft = 0;
		uiTop = 0;
		uiRight = 1;
		uiBottom = 1;
	}
}CUSTOMLAYOUT_NODE,*PCUSTOMLAYOUT_NODE;

typedef struct tagCustomLayout
{
	char csCustomLayoutName[32];
	CUSTOMLAYOUT_NODE CustomLayoutNode[256];
	unsigned int uiWidBound;
	unsigned int uiHeitBound;
	unsigned int uiSplitWndNum;
	tagCustomLayout()
	{
		csCustomLayoutName[0] = '\0';
		uiSplitWndNum = 0;
		uiWidBound = 1;
		uiHeitBound = 1;
	}
}CUSTOMLAYOUT,*PCUSTOMLAYOUT;

typedef struct tagCustomLayoutList
{
	CUSTOMLAYOUT CustomLayout[MAX_CUSTOMLAYOUT_COUNT];
	unsigned int uiCustomLayoutNum;
	tagCustomLayoutList()
	{
		uiCustomLayoutNum = MAX_CUSTOMLAYOUT_COUNT;
	}
}CUSTOMLAYOUTLIST,*PCUSTOMLAYOUTLIST;

typedef struct tagLayoutInfo
{
	BOOL bCustom;
	unsigned int uiWidBoundLen;        
	unsigned int uiHeitBoundLen;        
	int iOutIndex[MAX_LAYOUT_WND_NUM];
	unsigned int iAreas;
	tagLayoutInfo()
	{
		bCustom = FALSE;
		uiWidBoundLen = MIN_BOUND_LEN;
		uiHeitBoundLen = MIN_BOUND_LEN;
		int i = 0;

		for(i = 0; i < MAX_LAYOUT_WND_NUM;i++)
		{
			iOutIndex[i] = -1;
		}

		iAreas = 0;
	}
}LAYOUT_PARAM,*PLAYOUT_PARAM;

class CCustomLayoutData
{
public:
	CCustomLayoutData(void);
	~CCustomLayoutData(void);
	BOOL GetData(PCUSTOMLAYOUTLIST pLayoutCfg);

	BOOL GetLayoutData(WORD wLayoutMode,PLAYOUT_PARAM pLayoutParam);
private:
	//char m_szFileName[MAX_PATH];
	CUSTOMLAYOUTLIST m_CustomLayoutList;
	CMapWordToPtr m_MapLoyout;

private:
	void __InitDefaultMode();
	//void __InitCustomMode();

public:
	void Fun_Init(void);
	void Fun_UnInit(void);
};
