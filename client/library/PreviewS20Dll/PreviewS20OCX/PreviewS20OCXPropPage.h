#pragma once

// PreviewS20OCXPropPage.h : CPreviewS20OCXPropPage ����ҳ���������


// CPreviewS20OCXPropPage : �й�ʵ�ֵ���Ϣ������� PreviewS20OCXPropPage.cpp��

class CPreviewS20OCXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPreviewS20OCXPropPage)
	DECLARE_OLECREATE_EX(CPreviewS20OCXPropPage)

// ���캯��
public:
	CPreviewS20OCXPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_PREVIEWS20OCX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

