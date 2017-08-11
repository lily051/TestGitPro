#pragma once

// PreviewS20OCXPropPage.h : CPreviewS20OCXPropPage 属性页类的声明。


// CPreviewS20OCXPropPage : 有关实现的信息，请参阅 PreviewS20OCXPropPage.cpp。

class CPreviewS20OCXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPreviewS20OCXPropPage)
	DECLARE_OLECREATE_EX(CPreviewS20OCXPropPage)

// 构造函数
public:
	CPreviewS20OCXPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PREVIEWS20OCX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

