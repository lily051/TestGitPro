#pragma once

// RPConfigPropPage.h : CRPConfigPropPage 属性页类的声明。


// CRPConfigPropPage : 有关实现的信息，请参阅 RPConfigPropPage.cpp。

class CRPConfigPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRPConfigPropPage)
	DECLARE_OLECREATE_EX(CRPConfigPropPage)

// 构造函数
public:
	CRPConfigPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_RPCONFIG };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

