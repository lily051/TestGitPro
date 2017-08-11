// RPConfigPropPage.cpp : CRPConfigPropPage 属性页类的实现。

#include "stdafx.h"
#include "RPConfig.h"
#include "RPConfigPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CRPConfigPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CRPConfigPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CRPConfigPropPage, "RPCONFIG.RPConfigPropPage.1",
	0xb482cba, 0xefb9, 0x4279, 0xbf, 0xa3, 0x9a, 0xf3, 0xd1, 0xbe, 0x9b, 0xd8)



// CRPConfigPropPage::CRPConfigPropPageFactory::UpdateRegistry -
// 添加或移除 CRPConfigPropPage 的系统注册表项

BOOL CRPConfigPropPage::CRPConfigPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_RPCONFIG_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CRPConfigPropPage::CRPConfigPropPage - 构造函数

CRPConfigPropPage::CRPConfigPropPage() :
	COlePropertyPage(IDD, IDS_RPCONFIG_PPG_CAPTION)
{
}



// CRPConfigPropPage::DoDataExchange - 在页和属性间移动数据

void CRPConfigPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CRPConfigPropPage 消息处理程序
