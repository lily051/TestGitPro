// RPConfigPropPage.cpp : CRPConfigPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "RPConfig.h"
#include "RPConfigPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CRPConfigPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CRPConfigPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CRPConfigPropPage, "RPCONFIG.RPConfigPropPage.1",
	0xb482cba, 0xefb9, 0x4279, 0xbf, 0xa3, 0x9a, 0xf3, 0xd1, 0xbe, 0x9b, 0xd8)



// CRPConfigPropPage::CRPConfigPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CRPConfigPropPage ��ϵͳע�����

BOOL CRPConfigPropPage::CRPConfigPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_RPCONFIG_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CRPConfigPropPage::CRPConfigPropPage - ���캯��

CRPConfigPropPage::CRPConfigPropPage() :
	COlePropertyPage(IDD, IDS_RPCONFIG_PPG_CAPTION)
{
}



// CRPConfigPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CRPConfigPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CRPConfigPropPage ��Ϣ�������
