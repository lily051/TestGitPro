#pragma once

// CLocalWndView

class CLocalWndView : public CWnd
{
	DECLARE_DYNAMIC(CLocalWndView)

public:
	CLocalWndView();
	virtual ~CLocalWndView();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

protected:
	// ����ɫ.
	COLORREF m_clrBk;  

private:
	// �Ƿ��ڲ�����
	BOOL m_bIsPlaying;
	// �Ƿ�����ͣ��
	BOOL m_bIsPausing;
	// ��Ƶ�Ƿ�����
	BOOL m_bIsSounding;
	// �Ƿ�ȫ����
	BOOL m_bIsFullScreening;
	// �Ҽ������˵�
	void PopupMenu(BOOL bJustClose = FALSE);
	// ִ�в˵�
	void ExcuteSelectOption(int nCmd);

public:
	// �޸Ĳ���״̬
	void UpdatePlayStatus(BOOL bPlay);
	// �޸Ĳ˵�״̬
	void UpdateMenuStatus(int nBtnType, BOOL bStatus);

};


