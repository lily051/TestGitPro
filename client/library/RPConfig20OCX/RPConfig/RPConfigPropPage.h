#pragma once

// RPConfigPropPage.h : CRPConfigPropPage ����ҳ���������


// CRPConfigPropPage : �й�ʵ�ֵ���Ϣ������� RPConfigPropPage.cpp��

class CRPConfigPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRPConfigPropPage)
	DECLARE_OLECREATE_EX(CRPConfigPropPage)

// ���캯��
public:
	CRPConfigPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_RPCONFIG };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

