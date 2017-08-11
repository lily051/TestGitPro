#pragma once

/*****************************************************************
*   Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlayTitleWnd.h
*   Description:    �ط���Ƶ�ؼ��࣬ �����ڽ�����Ƶ
*   Author:         yudan
*   Date:			2011-1-30 09:10
*   Modification History:
*    <version>      <time>          <author>        <desc>
      v1.1          2011-1-30 09:10   yudan          create
*****************************************************************/


class CPlayTitleWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayTitleWnd)

public:
	CPlayTitleWnd();
	virtual ~CPlayTitleWnd();

private:
	COLORREF m_clrBk;  //����ɫ
	CString m_strText;  //��������
	COLORREF m_clrText;  //����������ɫ
	CFont m_fTextFont;  //������������
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
	//�����ı�
	void SetText(CString strText);
	//���ñ����ı���ɫ
	void SetTextColour(COLORREF clrText);
	//�����ı�����
	void SetTextFont(CFont* fTextFont);
	//ˢ��
	void UpdateSurface();
	//��ȡ��������
	CString GetText();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
