#pragma once

#include "../ui/uiBase/ButtonST_Skin.h"

// CPlayMiniToolBarExpandWnd

class CPlayMiniToolBarExpandWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayMiniToolBarExpandWnd)

public:
	CPlayMiniToolBarExpandWnd(BOOL bOpenSound, BOOL bSaveData);
	virtual ~CPlayMiniToolBarExpandWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnToolBarBtnClkCapture();
	afx_msg void OnToolBarBtnClkSave();
	afx_msg void OnToolBarBtnClkSound();
	afx_msg void OnToolBarBtnClkDownload();

private:
	CButtonST_Skin m_btnDownload;
	CButtonST_Skin m_btnCapture;
	CButtonST_Skin m_btnSave;
	CButtonST_Skin m_btnSound;

	CPen m_penBorder;			// �߿򻭱�.
	int m_nBtnNum;				// ��ť����.
	int m_nBtnWidth;			// ��ť���.
	int m_nBtnHeight;			// ��ť�߶�.
	int m_nBtnSpace;			// ��ť���.
	UINT m_uiIdleTime;			// �ر�ʱ��.
	UINT m_uiCloseTimer;		//ʱ�Ӿ��
	CWnd* m_pParentWnd;			// ������ָ��.
	BOOL m_bOpenSound;			// �Ƿ��������.
	BOOL m_bSaveData;			// �Ƿ��Ѿ�����������.

private:
	// ���Ʊ߿�.
	void DrawBorder(CDC* pDC);

	void OnBtnClick(UINT nID);
	void CreateChildWnd();
	void InitCtrl();
	void InitPos();
	void SetToolBarBtnTips();

public:
	// ��ȡ���ڿ��.
	int GetWndWidth();
	// ��ȡ���ڸ߶�.
	int GetWndHeight();
	// ������ʱ��.
	BOOL SetCloseTimer(void);
	// �رն�ʱ��.
	void KillColseTimer(void);
	// ��������.
	BOOL CreateFloatWindow(CWnd *pParentWnd);
	// ���°�ť״̬.
	void UpdateBtnState(BOOL bOpenSound, BOOL bSave);
	// ���°�ťtips.
	void UpdateButtonTips();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


