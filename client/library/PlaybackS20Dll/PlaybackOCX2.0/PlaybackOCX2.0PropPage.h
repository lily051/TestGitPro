#pragma once

// PlaybackOCX3.0PropPage.h : CPlaybackOCX30PropPage ����ҳ���������


// CPlaybackOCX30PropPage : �й�ʵ�ֵ���Ϣ������� PlaybackOCX3.0PropPage.cpp��

class CPlaybackOCX30PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPlaybackOCX30PropPage)
	DECLARE_OLECREATE_EX(CPlaybackOCX30PropPage)

// ���캯��
public:
	CPlaybackOCX30PropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_PLAYBACKOCX30 };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

