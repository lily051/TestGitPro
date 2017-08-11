// PlaybackOCX3.0PropPage.cpp : CPlaybackOCX30PropPage 属性页类的实现。

#include "stdafx.h"
#include "PlaybackOCX2.0.h"
#include "PlaybackOCX2.0PropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPlaybackOCX30PropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CPlaybackOCX30PropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid
#ifndef _FORCLIENT
IMPLEMENT_OLECREATE_EX(CPlaybackOCX30PropPage, "PLAYBACKOCX30.PlaybackOCX30PropPage.1",
	0x3d7c7840, 0x87f5, 0x4623, 0x88, 0xb3, 0xc8, 0xe2, 0x28, 0x44, 0x8c, 0x69)
#else
IMPLEMENT_OLECREATE_EX(CPlaybackOCX30PropPage, "PLAYBACKOCX30.PlaybackOCX30ClientPropPage.1",
	0x11b5cb3, 0x12a3, 0x4bf3, 0x91, 0x3a, 0xf, 0x3, 0x21, 0x58, 0xc2, 0xe)
#endif



// CPlaybackOCX30PropPage::CPlaybackOCX30PropPageFactory::UpdateRegistry -
// 添加或移除 CPlaybackOCX30PropPage 的系统注册表项

BOOL CPlaybackOCX30PropPage::CPlaybackOCX30PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_PLAYBACKOCX30_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CPlaybackOCX30PropPage::CPlaybackOCX30PropPage - 构造函数

CPlaybackOCX30PropPage::CPlaybackOCX30PropPage() :
	COlePropertyPage(IDD, IDS_PLAYBACKOCX30_PPG_CAPTION)
{
}



// CPlaybackOCX30PropPage::DoDataExchange - 在页和属性间移动数据

void CPlaybackOCX30PropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CPlaybackOCX30PropPage 消息处理程序
