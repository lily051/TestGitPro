#pragma once
#include "../ui/uiBase/DrawStripFun.h"
#include "../ui/uiBase/ButtonST_Msg.h"
#include "../ui/uiBase/ButtonST_Skin.h"
#include "../ui/uiCtrl/BitmapSlider.h"
#include "CToolTips.h"
#include <vector>

// CPlayToolBarDlg �Ի���

class CPlayToolBarWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayToolBarWnd)

public:
	CPlayToolBarWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayToolBarWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	BOOL m_bPlayState;

public:
	CButtonST_Skin m_btnGroupCfg;

	CButtonST_Skin m_btnStopAll;
	CButtonST_Skin m_btnSound;
	CButtonST_Skin m_btnPlay;
    CButtonST_Skin m_btnAllCapture;
	CButtonST_Skin m_btnFullScreen;
	CDrawStripFun m_DrawBakeFun;

private:
	//�����ӿؼ�
	void CreateChildCtrl();
	//��ʼ���ӿؼ�
	void InitCtrl();
	//�ƶ��ؼ�
	void MoveCtrl();

	//��ť�����Ϣ����
	void OnClkToolBarBtn(WPARAM wParam, LPARAM lParam);

	//���ð�ť��ʾ��
	void SetButtonTips();
	

public:
	//ˢ�°�ť״̬
	void UpdateBtnState(LPVOID lpPlayWnd);
	//����ȫ����ť״̬
	void SetFullScreenOn(BOOL bFull);
	//���Ű�ť״̬
	void UpdatePlayState(BOOL bState);
	// ���°�ť��ʾ��.
	void UpdateButtonTips();

public:
	//��ť����¼�
	afx_msg void OnBnClickPlay();
	afx_msg void OnBnClickReversePlay();
	afx_msg void OnBnClickStopAll();
	afx_msg void OnBnClickVolumn();
	afx_msg void OnBnClickFullScreen();
	afx_msg void OnBnClickPlayGroupCfg();
	afx_msg void OnBnClickSound();
    afx_msg void OnBnClickAllCapture();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
