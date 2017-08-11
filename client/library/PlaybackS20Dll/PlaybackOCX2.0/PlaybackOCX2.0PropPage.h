#pragma once

// PlaybackOCX3.0PropPage.h : CPlaybackOCX30PropPage 属性页类的声明。


// CPlaybackOCX30PropPage : 有关实现的信息，请参阅 PlaybackOCX3.0PropPage.cpp。

class CPlaybackOCX30PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPlaybackOCX30PropPage)
	DECLARE_OLECREATE_EX(CPlaybackOCX30PropPage)

// 构造函数
public:
	CPlaybackOCX30PropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PLAYBACKOCX30 };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

