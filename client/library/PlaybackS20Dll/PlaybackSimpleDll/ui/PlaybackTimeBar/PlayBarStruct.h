#pragma once
//#include "memdcex.h"
#define SPAN_THREEDAY  5
#define SPAN_ONEDAY    6
#define SPAN_HALFOFDAY 7
#define SPAN_SIXHOUR   8
#define SPAN_FOURHOUR  9
#define SPAN_TWOHOUR   10
#define SPAN_ONEHOUR   11
#define SPAN_HALFHOUR  12

#define CELLLENTYPE_1 1
#define CELLLENTYPE_2 2
#define CELLLENTYPE_3 3
#define CELLLENTYPE_4 4
#define CELLLENTYPE_5 5

#define COLOR_NORTITLETEXT   RGB(0,0,0)
#define COLOR_NORTITLESTART RGB(0xff,0xff,0xff)
#define COLOR_NORTITLEEND   RGB(30,90,150)
#define COLOR_NORBAKE   RGB(195,221,246)
#define COLOR_NOR6   RGB(57,215,189)
#define COLOR_NOR5_PLAYBAR  RGB(247,65,123)
#define COLOR_NOR7_PLAYBAR   RGB(255,207,0)
#define COLOR_NOR8   RGB(99,121,0XFF)
#define COLOR_NOR9_PLAYBAR   RGB(255,100,0x00)

// yxn20090918
#define PLAYBARCTRL_COMMAND_TIMECHANGE 201

#define TIMESPAN_MIN 424800000  //1983-6-19 0:0:0
#define TIMESPAN_MAX 1939564800 //2031-6-19 0:0:0

//消息回调
typedef BOOL (*FCHK_CALLBACK_PLAYBAR)(UINT nCommandID,long lParam,void* lUserID);  // yxn20090921

#define MAX_ELEMENTNAME_LEN 64   // yxn20091017 将32改为64
typedef struct _tagPLAYBAR_ElementInfo
{
	int iElementID;
	TCHAR czElementName[MAX_ELEMENTNAME_LEN];	
	int iElementTemp1;
	int iElementTemp2;
	TCHAR czElementTemp1[MAX_ELEMENTNAME_LEN];
	TCHAR czElementTemp2[MAX_ELEMENTNAME_LEN];
	_tagPLAYBAR_ElementInfo *pNext;
	_tagPLAYBAR_ElementInfo()
	{
		iElementID = -1;
		memset(czElementName,0,MAX_ELEMENTNAME_LEN);
		iElementTemp1 = -1;
		iElementTemp2 = -1;
		memset(czElementTemp1,0,MAX_ELEMENTNAME_LEN);
		memset(czElementTemp2,0,MAX_ELEMENTNAME_LEN);
		pNext = NULL;
	}
}PLAYBAR_ELEMENT_INFO,*PPLAYBAR_ELEMENT_INFO;

typedef struct _tagSectionInfoFor9000{
	int	iElementID;
	/*unsigned int*/time_t	tTimeStart;
	/*unsigned int*/time_t	tTimeEnd;
	int	iDataType;
	bool bLocked;
	unsigned int videoDataLen;
	_tagSectionInfoFor9000* pNext;
	_tagSectionInfoFor9000()
	{
		iElementID = -1;
		tTimeStart = 0;
		tTimeEnd = 0;
		iDataType = -1;
		videoDataLen = 0;
		bLocked = false;
		pNext = NULL;
	}
}SECTIONINFO_FOR9000,*PSECTIONINFO_FOR9000;

// 通道的锁定片段信息 yxn20090917
typedef struct _tagPLAYBAR_LockSection{
	time_t	timeStart;
	time_t	timeEnd;
	_tagPLAYBAR_LockSection* pNext;
	_tagPLAYBAR_LockSection()
	{
		timeStart = TIMESPAN_MIN;
		timeEnd = TIMESPAN_MAX;
		pNext = NULL;
	}
}PLAYBAR_LOCK_SECTION,*PPLAYBAR_LOCK_SECTION;

// 通道的标签片段信息 yxn20090917
typedef struct _tagPLAYBAR_RemarkSection{
	time_t	timeStart;
	time_t	timeEnd;
	CString strRemark;
	_tagPLAYBAR_RemarkSection* pNext;
	_tagPLAYBAR_RemarkSection()
	{
		timeStart = TIMESPAN_MIN;
		timeEnd = TIMESPAN_MAX;
		strRemark = "";
		pNext = NULL;
	}
}PLAYBAR_REMARK_SECTION,*PPLAYBAR_REMARK_SECTION;

// 通道的其他详细信息 yxn20090917
typedef struct _tagPLAYBAR_ElementMoreInfo
{
	int iElemID;
	PPLAYBAR_LOCK_SECTION pLockSection;
	PPLAYBAR_REMARK_SECTION pRemarkSection;
	_tagPLAYBAR_ElementMoreInfo *pNext;
	_tagPLAYBAR_ElementMoreInfo()
	{
		iElemID = 0;
		pLockSection = NULL;
		pRemarkSection = NULL;
		pNext = NULL;
	}
}PLAYBAR_ELEMENT_MOREINFO,*PPLAYBAR_ELEMENT_MOREINFO;

typedef struct _tagPLAYBAR_ElementFileLis
{
	PLAYBAR_ELEMENT_INFO ElementInfo;
	PSECTIONINFO_FOR9000 pSectionList;
	_tagPLAYBAR_ElementFileLis *pNext;
	_tagPLAYBAR_ElementFileLis()
	{
		pSectionList = NULL;
		pNext = NULL;
	}
}PLAYBAR_ELEMENT_FILELIST,*PPLAYBAR_ELEMENT_FILELIST;

typedef struct _tagPLAYBAR_FileTypeColor
{
	int iDataType;
	COLORREF crfileColor;
	_tagPLAYBAR_FileTypeColor()
	{
		iDataType = -1;
		crfileColor = RGB(0,0,0);
	}
}PLAYBAR_FILETYPE_COLOR,*PPLAYBAR_FILETYPE_COLOR;

typedef struct _tagPLATBAR_TimePos
{
	unsigned int uiTimePos;
	time_t  ctTime;
	_tagPLATBAR_TimePos()
	{
		uiTimePos = 0;
		ctTime = TIMESPAN_MIN;
	}
}PLATBAR_TIMEPOS,*PPLATBAR_TIMEPOS;

// 选中的片段信息 yxn20090917
typedef struct _tagPLAYBAR_SelectSection
{
	int iElementID;
	time_t timeStart;
	time_t timeEnd;
	TCHAR szElementName[64];
	unsigned int iTotalDataLen;
	PSECTIONINFO_FOR9000 pFileSections;  // 相关文件信息
	_tagPLAYBAR_SelectSection *pNext;
	_tagPLAYBAR_SelectSection()
	{
		iElementID = -1;
		timeStart = TIMESPAN_MIN;
		timeEnd   = TIMESPAN_MAX;
		memset(szElementName,0,64);
		iTotalDataLen = 0;
		pFileSections = NULL;
		pNext = NULL;
	}

}PLAYBAR_SELECT_SECTION,*PPLAYBAR_SELECT_SECTION;

