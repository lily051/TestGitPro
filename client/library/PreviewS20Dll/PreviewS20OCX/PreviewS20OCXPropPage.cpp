// PreviewS20OCXPropPage.cpp : CPreviewS20OCXPropPage ����ҳ���ʵ�֡�
//lint -library
#include "stdafx.h"
#include "PreviewS20OCX.h"
#include "PreviewS20OCXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPreviewS20OCXPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CPreviewS20OCXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CPreviewS20OCXPropPage, "PREVIEWS20OCX.PreviewS20OCXPropPage.1",
	0xa4c3b9eb, 0x63b1, 0x47f7, 0xbb, 0x63, 0xb4, 0x29, 0xce, 0x67, 0xad, 0x98)



// CPreviewS20OCXPropPage::CPreviewS20OCXPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CPreviewS20OCXPropPage ��ϵͳע�����

BOOL CPreviewS20OCXPropPage::CPreviewS20OCXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_PREVIEWS20OCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CPreviewS20OCXPropPage::CPreviewS20OCXPropPage - ���캯��

CPreviewS20OCXPropPage::CPreviewS20OCXPropPage() :
	COlePropertyPage(IDD, IDS_PREVIEWS20OCX_PPG_CAPTION)
{
}



// CPreviewS20OCXPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CPreviewS20OCXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CPreviewS20OCXPropPage ��Ϣ�������
