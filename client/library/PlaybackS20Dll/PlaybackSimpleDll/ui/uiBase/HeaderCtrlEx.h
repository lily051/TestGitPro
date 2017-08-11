/**************************************************************************
*   Copyright 2008-2009 Hikvision Digital Technology Co., Ltd.
*   FileName:       HearderCtrlEx.h
*   Description     �Զ����б�ؼ�
*   Author:         Mayonglai
*   Date:           2009-05-10
**************************************************************************/
#pragma once


// CHeaderCtrlEx

class CHeaderCtrlEx : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderCtrlEx)

public:
	CHeaderCtrlEx();
	virtual ~CHeaderCtrlEx();
private:
	HBITMAP CopyBitmap(HBITMAP  hSourceHbitmap);              //����bmpͼƬ

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();                                      //���Ʊ�ͷ
	afx_msg LRESULT OnLayOut(WPARAM wParam, LPARAM lParam);      //��Ӧ����Ϣ�޸ı�ͷ�߶�
	void SetListTitleBMP(HBITMAP hLeft, HBITMAP hMid, HBITMAP hRight);
private:
	CBitmap m_bmpLeft;
	CBitmap m_bmpMid;
	CBitmap m_bmpRight;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


