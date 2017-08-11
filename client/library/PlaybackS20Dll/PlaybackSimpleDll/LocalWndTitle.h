#pragma once


// CLocalWndTitle

class CLocalWndTitle : public CWnd
{
	DECLARE_DYNAMIC(CLocalWndTitle)

public:
	CLocalWndTitle();
	virtual ~CLocalWndTitle();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	//�����ı�
	void SetText(CString strText);
	//ˢ��
	void Update();
private:
	COLORREF m_clrBk;    //����ɫ
	CString m_strText;   //��������
	COLORREF m_clrText;  //����������ɫ
	CFont m_fTextFont;   //������������
};


